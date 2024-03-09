#ifndef	_KEY_H_
#define _KEY_H_

/* Includes ------------------------------------------------------------------*/
#include "reg51.h"
#include "Delay.h"
#include "FM.h"

/*用来初始化A/D转化相关寄存器的数据****/
/* Define to prevent recursive inclusion -------------------------------------*/
#define	P1_7_ADC 0x80
#define ADC_POWER 0X80
#define ADC_FLAGE 0X10
#define ADC_START 0X08
#define ADC_SPEEDLL 0X00
#define ADC_SPEEDL 0X20
#define ADC_SPEEDH 0X40
#define ADC_SPEEDHH 0X60
#define ADC_CHANNEL 0X07
/******引脚别名定义****/
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P1^7; //Key3


/* Exported functions ------------------------------------------------------- */
void Init_KEY();
unsigned char GetADC();
void Search_Keyscan();
void KeyScan();

#endif