#ifndef _485MC_H_
#define _485MC_H_

#include "STC15F2K60S2.H" 
#include <intrins.h>

#define S2RI  0x01
#define S2TI  0x02
#define S2RB8 0x04						//S2CON.2
#define S2TB8 0x08						//S2CON.3

#define NONE_PARITY			0			//无校验
#define	ODD_PARITY			1			//奇校验
#define EVEN_PARITY			2			//偶校验

#define PARITYBIT NONE_PARITY			//定义校验位

#define FOSC	11059200L				//系统时钟频率 
#define BAUD	9600					//串口波特率	
#define TM		(65536-(FOSC/4/BAUD))	//定时器初始时间

typedef unsigned char	uint8;
typedef unsigned int	uint16;
typedef unsigned long	uint32;


/********数码管显示******/
/*位选*/
sbit Sel0=P2^0;			  
sbit Sel1=P2^1;
sbit Sel2=P2^2;
/*数码管总开关*/
sbit LED_SEL=P2^3;

/********按键******/
sbit Key1=P3^2;	   //Key1
sbit Key2=P3^3;	   //Key2
sbit Key3=P1^7;	   //Key3

sbit RS485E  = P3^7 ;				//定义MAX485使能引脚，为1时发送，为0时接收


/*变量定义*/
bit busy ;							//为1时忙（发送数据），为0时闲
uint8 rev_data ;
uint8 display ;

unsigned char duanxuan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

/*函数定义*/
void Init_Uart2(void) ;
void CPUInit(void);
void delay(void) ;				
#endif