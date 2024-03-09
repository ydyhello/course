#include "led.h"

/*
 * 函数名：Config_LED
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void Config_LED()
{
	CLK_DIV=0X00;
	P0M0=0XFF;
	P0M1=0X00;
	P2M0=0x00;
	P2M1=0x00;
}

/*
 * 函数名：Display_LED_Num
 * 描述  ：设置数码管显示的数
 * 输入  ：第j个数码管显示数为i,flag=1时显示的带小数点，flag=0时，不带小数点
 * 输出  ：无
 */
void Display_LED_Num(int i,int j,unsigned char flag)
{
	 unsigned char num[]={
	 						0x3f,0x06,0x5b,0x4f,
							0x66,0x6d,0x7d,0x07,
							0x7f,0x6f
						};
	 unsigned char num1[]={
	 						0xbf,0x86,0xdb,0xcf,
							0xe6,0xed,0xfd,0x87, 
							0xff,0xef		
						 };
	 switch(j)
	 {
	 	 case 1: P2_2=0; P2_1=0;P2_0=0;	break;			
		 case 2: P2_2=0; P2_1=0;P2_0=1;	break;				
		 case 3: P2_2=0; P2_1=1;P2_0=0;	break;	
		 case 4: P2_2=0; P2_1=1;P2_0=1; break; 		
		 case 5: P2_2=1; P2_1=0;P2_0=0; break;
		 case 6: P2_2=1; P2_1=0;P2_0=1; break;
		 case 7: P2_2=1; P2_1=1;P2_0=0;	break;
		 default: P2_2=1; P2_1=1;P2_0=1; break;
	 }
	 	
	 if(flag)
	 {
	 	P0=num1[i];		
	 	Delayus(500);
		P0=0x00;
	 	Delayus(500);		
	 }
	 else					
	 {
		 P0=num[i];
		 Delayus(500);
		 P0=0X00;
		 Delayus(500);
	 }
}


