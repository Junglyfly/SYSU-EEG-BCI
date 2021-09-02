# EEG-BCI
脑电信号采集、目前在做硬件部分。


一、硬件设计注意点

1）关于SRB和BIAS引脚说明 ：
{ 
    ①website :  https://e2e.ti.com/support/data-converters-group/data-converters/f/data-converters-forum/551341/ads1299-srb-bias-and-reference
    ②The SRB pins are essentially inputs for a reference input that you intend to connect to multiple channels.
    ③The SRB2 pin can be connected to individual channels' positive inputs.
    ④The SRB1 pin can be connected to all channels' negative inputs.
    ⑤The BIAS pin is the equivalent of a right-leg driver for an ECG measurement.
    ⑥It will output a signal that attempts to keep the patient's common-mode voltage within the valid range for the ADC.
}


2）关于多块ADS1299连接 ：
{
   ①I recommend you simply connect all INxN inputs to analog ground (assuming you are using bipolar supplies) and find each electrode voltage in a single-ended fashion. 
   ②To reduce pin count, set the START pin low and use the START serial command to synchronize and start conversions.  (datasheet 65)
   ③BIAS Drive Connection for Multiple Devices. (datashett 43 figure 39) 
   ④BIASOUT BIASIN (datasheet 33)
}


3)关于PCB设计注意点
{
  ①EEG的模拟小信号走线尽量至宽，短；
  ②EEG部分AGND和DGND分开，注意做单点接地；
  ③模拟小信号边上铺铜接AGND，也准备一些过孔；模拟小信号走线注意不要过细过弯耦合噪声。
}
