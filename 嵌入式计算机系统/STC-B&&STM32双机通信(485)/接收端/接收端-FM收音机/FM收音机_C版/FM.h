#ifndef _FM_H_
#define _FM_H_

#include "reg51.h"
#include "Delay.h"
#include "led.h"
//#include "volume.h"

#define MAX_Frequency 108000			  //最大电台频率
#define MIN_Frequency 87500				  //最小电台频率
#define DOWN 0							 //高电平值
#define UP 1							   //低电平值
/*******引脚别名定义*****/
sbit FM_CLOCK=P4^5;
sbit FM_DATA=P2^7;

extern unsigned char Radio_Write_Data[8];		 //写入RDA5807P芯片的前八个字节数据
extern unsigned long frequency;					//当前收音机收听的电台频率
extern unsigned int chan;						//CHAN值（参考原理说明文档）
extern unsigned char volume;					//电台音量值

void Init_Radio();
/*******************************IIC***********************************/
void IIC_Radio_Start();
void IIC_Radio_Stop();
void IIC_Radio_MACK();
void IIC_Radio_ACK();		   
void IIC_Radio_WriteByte(unsigned char Data);
void IIC_Radio_WriteData();

/*******************************3-wire Bus***************************/
void Radio_WriteByte(unsigned char dat);
void Radio_WriteData();
/*******************************General******************************/
void Get_CHAN(); //频率转CHAN
void Get_Frequency(); //CHAN值转频率值
void FrequencyToChan(void); //频率转CHAN
void Manual_Search(unsigned char flag);
void Frequency_Display();
void SetVolume(unsigned char flag); //设置音量
#endif