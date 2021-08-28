#-*- coding: utf-8 -*-
# 串口测试程序
import serial
import matplotlib.pyplot as plt
import numpy as np
import time
import re


# -*- coding: gbk -*-
# # User input comport and bundrate
# comport = input('Please input comport (like COM3) for your connected device: ')
# baudrate = input('Please input baudrate (like 9600) for your connected device: ')
# bytes = input('Please input bytes type of uart data (1->1 byte, 2->2 bytes): ')
# bytes = int(bytes)
# print('You selected %s, baudrate %d, %d byte.' % (comport, int(baudrate), bytes))
  
serialport = serial.Serial("COM5", int(115200))


if serialport.isOpen():  
    serialport.flushInput() #先清空一下缓冲区
    print("open success")
else:
    print("open failed")
  
com = serialport



plt.grid(True) # 添加网格
plt.ion()   # interactive mode
plt.figure(1)
plt.xlabel('times')
plt.ylabel('data')
plt.title('Diagram of UART data by Python')
t = [0]
m = [0]
i = 0




def Data_unPack(line):
    line = line.split(",")
    DataBuff = [0,1,2,3,4,5,6,7]
    j = 0
    if line[0] == 'c0' :
        for i in range(3,27,3) :
            if int('0x'+line[i],16) & 1<<8 :  #最高位为1，为负数
                temp = int('0x'+line[i],16) & (~(1<<8))
                DataBuff[j] = ~(temp<<16 | int('0x'+line[i+1],16)<<8 | int('0x'+line[i+2],16))+1
                DataBuff[j] = -DataBuff[j]

            else:
                DataBuff[j] = int('0x'+line[i],16)<<16 | int('0x'+line[i+1],16)<<8 | int('0x'+line[i+2],16)

            j+=1
        print(DataBuff)
        return DataBuff

    else :
        return DataBuff





while(True):
    if i > 300:  # 300次数据后，清除画布，重新开始，避免数据量过大导致卡顿。
        t = [0]
        m = [0]
        i = 0
        plt.cla()

    line = com.readline().decode() #line是bytes格式，使用decode()转成字符串
    intdata = Data_unPack(line)
    i = i+1
    t.append(i)
    m.append(intdata[2])
    plt.plot(t, m, '-r')   
    # plt.scatter(i, intdata)
    plt.draw()
  

    plt.pause(0.02)









# while True:
#     if i > 300:  # 300次数据后，清除画布，重新开始，避免数据量过大导致卡顿。
#         t = [0]
#         m = [0]
#         i = 0
#         plt.cla()
#     count = serialport.inWaiting()
#     if count > 0 :
#         if (bytes == 1):
#             data = serialport.read(1)
#         elif (bytes == 2):
#             data = serialport.read(2)
#         if data !='':
#             intdata = int.from_bytes(data, byteorder='big', signed = False)
#             print('%d byte data %d' % (bytes, intdata))
#             i = i+1
#             t.append(i)
#             m.append(intdata)
#             plt.plot(t, m, '-r')   
#             # plt.scatter(i, intdata)
#             plt.draw()
  
#     plt.pause(0.002)
