/********************************** EXT V1.0 说明 ***********************************************

EXT模块用于控制“STC-B学习板”上扩展接口EXT上相关操作。
EXT模块根据应用需要，在外接相应模块或部件后，可实现多种相应功能。这里提供部分应用驱动和API函数。
EXT模块这里提供1个驱动函数和若干个应用层API函数。
EXT模块的API函数不是同时有效的，而是根据初始化函数参数不同而分别有效。

  (1) void EXTInit(char EXTfunction)：EXT初始化函数。
	      函数参数：EXTfunction。定义EXT接口功能
				          EXTfunction 取值：enumEXTWeight（秤重：由HX710、HX711组成的电子秤）
									                  enumEXTPWM   （脉宽调制：可用于控制直流电机正反转、速度，灯亮度等）
																	  enumEXTDecode（增量式计数）
																		enumEXTUltraSonic （超声波测距）  
				函数返回值：无		
     注意：当EXT扩展接口用于串口2时（TTL Uart，可用于串行通信、蓝牙模块连接等），在使用Uart2Init()函数初始化EXT后，不要再使用EXTInit()函数，以免引起功能冲突

	(2)API函数
		(a) 电子秤。当EXTInit(char EXTfunction)使用enumEXTWeight参数时，GetWeight函数有效。
         int GetWeight(void)		//获取电子秤ADC秤重数据（16bit，带符号整数。未清零、未标定。参见HX710、HX711数据手册（高16bit））
		
		(b) PWM脉宽调制输出。当EXTInit(char EXTfunction)使用enumEXTPWM参数时，SetPWM函数有效。
				 void SetPWM(unsigned char PWM1, unsigned char freq1, unsigned char PWM2, unsigned char freq2);  
                     //设置EXT上输出PWM（参数PWM为占空比（输出高电平时间的比例）：0～100,单位%）。freqx为频率：1～255Hz）
										 //实际频率 = 1000/int(1000/freqx)。即：1000/i=4，5，6...1000，或250，200，167，143，125，111，100，91，83...1 
                     //可用于控制直流电机正反转、转速（配合H型桥式电路），灯亮度，等
										 
		(c) 旋转编码器、或增量式编码器。当EXTInit(char EXTfunction)使用enumEXTDecode参数时，GetDecode函数有效。
		     int GetDecode(void);         //获取增量编码器增量值（相对上次读取后的新增量）
		
		(d) 超声波测距。当EXTInit(char EXTfunction)使用enumEXTUltraSonic参数时，GetUltraSonic函数有效。
		     int GetUltraSonic(void);     //获取超声波测距值（每秒5次测量，返回值单位：cm）
				 
		(e) RFID读卡（暂缓）
		(g) （蓝牙、串口）见串口2
		(h) （气敏、数据采集、电子尺、额温）见ADC
编程注意事项：
      
			
编写：徐成（电话18008400450）   2021年8月24日设计
*/

#ifndef _EXT_H_
#define _EXT_H_

extern void EXTInit(char EXTfunction);    //扩展接口初始化
enum EXTname {enumEXTWeight       //电子秤
             ,enumEXTPWM,         //PWM，控制直流电机转动方向、快慢
             ,enumEXTDecode       //增量式计数（旋转编码器）
	           ,enumEXTUltraSonic   //超声波测距
                                  //串口2，蓝牙：见uart2模块
                                  //气敏、数据采集、电子尺、额温枪等：见ADC  
             };   
extern int GetWeight(void);         //获取电子秤ADC秤重数据（HX710、HX711输出高16bit，带符号数整数。未清零、未标定）
extern int GetDecode(void);         //获取增量编码器增量值（相对上次读取后的新增量）
extern int GetUltraSonic(void);     //获取超声波测距值（每秒5次测量，返回值单位：cm）
extern void SetPWM(unsigned char PWM1, unsigned char freq1, unsigned char PWM2, unsigned char freq2);  
                     //设置EXT上输出PWM（参数PWM为占空比（输出高电平时间的比例）：0～100,单位%）。freqx为频率：1～255Hz）
										 //实际频率 = 1000/int(1000/freqx)。即：1000/i=4，5，6...1000，或250，200，167，143，125，111，100，91，83...1 
                     //可用于控制直流电机正反转、转速（配合H型桥式电路），灯亮度，等

 


#endif