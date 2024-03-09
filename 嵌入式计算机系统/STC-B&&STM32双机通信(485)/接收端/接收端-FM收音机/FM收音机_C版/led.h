#ifndef _LED_H_
#define _LED_H_

/* Includes ------------------------------------------------------------------*/
#include "reg51.h"
#include "Delay.h"
/****引脚别名定义****/
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;


void Config_LED();
void Display_LED_Num(int i,int j,unsigned char flag);

#endif