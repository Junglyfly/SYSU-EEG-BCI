import serial
import pyqtgraph as pg
import numpy as np
import array
import threading
from time import sleep
import re
 
# -*- coding: gbk -*-
app = pg.mkQApp()
win = pg.GraphicsWindow()
win.setWindowTitle(u'波形显示')
p = win.addPlot()#win.addPlot()添加一个波形窗口，多次调用会将窗口分成多个界面
 
data_dict = {}#存放所有收到的数据
p.addLegend() #不添加就显示不了图例 ，一定要放在plot前调用
 
Max_count = 500 #页面最多显示的数据个数
 
#配置波形显示信息
def DisplayConfig():   
    p.showGrid(x=True,y=True, alpha=0.5)
    p.setLabels(left='y/V',bottom='x/point',title='eeg')#left纵坐标名 bottom横坐标名
    label = pg.TextItem()
    p.addItem(label)
    
 
#将串口收到的数据添加到字典
#数据格式 “name1,float;name2,flaot\n”  
def AddDataToDict(line):
    line = line.split("\\n") #目的是去除最后的\n换行，别的方式还没用明白
    str_arr = line[0].split(';')#因为上边分割了一下，所以是数组
    color = ['b','g','r', 'c','m','y', 'k','w']#颜色表，这些应该够了，最多8条线，在添加颜色可以用(r,g,b)表示
    for a in str_arr: #遍历获取单个变量 如“a,1;b,2;c,3”中的"a,1"
        s = a.split(',')#提取名称和数据部分
        if(len(s) != 2):#不等于2字符串可能错了，正确的只有名称和数据两个字符串
            return
        name = s[0]
        val_str = re.findall(r"^[-+]?([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][-+]?[0-9]+)?$",s[1])[0]#用正则表达式提取数字部分
        if(len(val_str)>0):#再判断下是否匹配到了数字
            val = float(val_str[0])#转成浮点型数字
            if(data_dict.get(name) == None): #判断是否存在添加当前键值，None则需要添加键值                  
                #curve = p.plot(pen = color[len(data_dict)],name=name,symbolBrush=color[len(data_dict)])#为新的变量添加新的曲线,显示数据点
                curve = p.plot(pen = color[len(data_dict)],name=name)#为新的变量添加新的曲线
                data_dict[name] = [curve]  #在字典中添加当前键值，并赋值曲线，字典数据格式{key:[curve,[dadta1,data2,...]]}
                data_dict[name].append([val])#将当前数据已列表的形式添加到字典对象中
            else:#键值存在直接添加到对应的数据部分
                if(len(data_dict[s[0]][1]) == Max_count):#限制一下页面数据个数
                    data_dict[s[0]][1]=data_dict[s[0]][1][1:-1]
                    data_dict[s[0]][1][-1] = float(s[1])
                else:
                    data_dict[s[0]][1].append(val)
        else:#接收错误
            print("error:" + a)
            return 
        

'''

        #将串口收到的数据添加到字典
#数据格式 “name1,float;name2,flaot\n”  
def AddDataToDict(line):
    line = line.split("\\n") #目的是去除最后的\n换行，别的方式还没用明白
    str_arr = line[0].split(';')#因为上边分割了一下，所以是数组
    color = ['b','g','r', 'c','m','y', 'k','w']#颜色表，这些应该够了，最多8条线，在添加颜色可以用(r,g,b)表示
    for a in str_arr: #遍历获取单个变量 如“a,1;b,2;c,3”中的"a,1"
        s = a.split(',')#提取名称和数据部分
        if(len(s) != 2):#不等于2字符串可能错了，正确的只有名称和数据两个字符串
            return
        name = s[0]
        val_str = re.findall(r"^[-+]?([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][-+]?[0-9]+)?$",s[1])[0]#用正则表达式提取数字部分
        if(len(val_str)>0):#再判断下是否匹配到了数字
            val = float(val_str[0])#转成浮点型数字
            if(data_dict.get(name) == None): #判断是否存在添加当前键值，None则需要添加键值                  
                #curve = p.plot(pen = color[len(data_dict)],name=name,symbolBrush=color[len(data_dict)])#为新的变量添加新的曲线,显示数据点
                curve = p.plot(pen = color[len(data_dict)],name=name)#为新的变量添加新的曲线
                data_dict[name] = [curve]  #在字典中添加当前键值，并赋值曲线，字典数据格式{key:[curve,[dadta1,data2,...]]}
                data_dict[name].append([val])#将当前数据已列表的形式添加到字典对象中
            else:#键值存在直接添加到对应的数据部分
                if(len(data_dict[s[0]][1]) == Max_count):#限制一下页面数据个数
                    data_dict[s[0]][1]=data_dict[s[0]][1][1:-1]
                    data_dict[s[0]][1][-1] = float(s[1])
                else:
                    data_dict[s[0]][1].append(val)
        else:#接收错误
            print("error:" + a)
            return 
        
'''
 
def addToDisplay():
    for i in data_dict.items():
        data = i[1][1] #数据部分
        curve = i[1][0] #当前的线
        if(len(data) > 1000):#一个界面都数据控制在1000个
            data=data[-1000:]
        else:
            data = data[:]
        curve.setData(data)#添加数据显示
 

 
#['c0', '0', '0', '1', 'ed', 'eb', '6', '95', '79', 'ff', '82', 'd4', '0', '5', '44', '0', '20', '97', '0', '72', '73', 'ff', '95', '32', 'ff', '8d', 'df', '\n']

def Data_unPack(line):
     line = line.split(",")
     DataBuff = [0,1,2,3,4,5,6,7]
     if line[0] == 'c0' :
        j = 0
        for i in range(3,27,3) :
            Data_temp = int('0x'+line[i],16)<<16 | int('0x'+line[i+1],16)<<8 | int('0x'+line[i+2],16)
            # if Data_temp & 1<<23 :
            #     Data_temp ^= 0x00FFFFFF
            #     Data_temp += 1
            #     Data_temp &= 0x00FFFFFF
            #     Data_temp *= -1
            Data_temp ^= 0x00800000
            DataBuff[j] = Data_temp
            j+=1

        Data_Str = "CH0," + str(DataBuff[0])  + ";CH1," + str(DataBuff[1])  + ";CH2," + str(DataBuff[2]) + ";CH3," + str(DataBuff[3]) + \
                    ";CH4," + str(DataBuff[4]) + ";CH5," + str(DataBuff[5]) + ";CH6," + str(DataBuff[6]) + ";CH7," + str(DataBuff[7]) 

        #  Data_Str = "CH6," + str(DataBuff[4]) + ";CH7," + str(DataBuff[5])          
        AddDataToDict(Data_Str)
        #  print(Data_Str)
     else :
        print("none")

  
def ComRecvDeal():
    t = InitCom('COM5',115200) #串口号和波特率自行设置
    if(t.isOpen() == True):      
        t.flushInput() #先清空一下缓冲区
        while(True):
            line = t.readline().decode() #line是bytes格式，使用decode()转成字符串

            #Data_unPack(line)
            AddDataToDict(line)#把收到的数据添加到字典中
    else:
        print("串口打开失败")
    
 
def InitCom(port,b):
    t = serial.Serial(port,b)
    return t
 
if __name__ == "__main__":
    th= threading.Thread(target=ComRecvDeal)#创建串口接收线程
    th.start()
    timer = pg.QtCore.QTimer()
    timer.timeout.connect(addToDisplay) #定时刷新数据显示
    timer.start(1)
    app.exec_()
    

