#include "KEY.H"
//#include "led.h"
#include "Delay.h"
#include "FM.h"
#include <STC15F2K60S2.H>
#include <intrins.h>		
#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long
unsigned char Radio_Write_Data[8]={					 
	0xc0,0x11,          //02H:音频输出，静音禁用，12MHZ，启用状态
	0x1a,0x50,          //03H:97500KHZ,频率使能87-108M(US/Europe)，步进100KHZ
	0x40,0x02,		    //04H:1-0为GPIO1(10为低，灯亮；11为高，灯灭)，...
	0x88,0xa5 };	    //a5中的5为初始音量
unsigned long frequency=0;
unsigned int chan=0;
unsigned char volume;
unsigned int moshi1=0	;
	
	
	#define cstUart2Ri  0x01                        //?????????
#define cstUart2Ti  0x02                        //?????????

#define cstNoneParity           0           //???
#define PARITYBIT cstNoneParity         //?????
	
	#define cstFosc 11059200L               //?????? 
#define cstBaud2    9600                    //?????    
#define cstT2HL     (65536-(cstFosc/4/cstBaud2))    //???????

	sbit sbtKey1 = P3 ^ 2 ;                 //????
sbit sbtKey2 = P3 ^ 3 ;                 //????
sbit sbtKey3 = P1 ^ 7 ;                 //????

sbit sbtSel0 = P2 ^ 0 ;
sbit sbtSel1 = P2 ^ 1 ;
sbit sbtSel2 = P2 ^ 2 ;
sbit sbtLedSel = P2 ^ 3 ;

sbit sbtM485_TRN  = P3 ^ 7 ;   //??MAX485????,?1???,?0???

bit btSendBusy ;                           
uchar ucGetDataTmp ;          //??????
uchar ucPutDataTmp ;          //??????
uchar arrSegSelect[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                        0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                        0x77, 0x7c, 0x39, 0x5e, 0x79,
                        0x71, 0x40, 0x00
                       };   //??,??0-f
/*系统初始化函数
*初始化系统的A/D,IO口以及收音机芯片（RDA5807P）
*/
											 void Uart2Init( void )
{
    S2CON = 0x10 ;      //??????,?????2??
    T2L = cstT2HL ;             //????????
    T2H = cstT2HL >> 8 ;
    AUXR |= 0x14 ;      //T2?1T??,??????2
}
void Init()
{ 
	Init_KEY();		
	Config_LED();
		
	/*FM硬件初始化*/
	 P4M0=0x00;
   P4M1=0x00;
	
	 P5M0=0xff;
	 P5M1=0x00;
	
	 Init_Radio();
	P2_3 = 0;
}
void Init_485()
{
    P0M0 = 0xff ;       //P0???(????)
    P0M1 = 0x00 ;
    P2M0 = 0x0f ;       //P2.0梸P2.3???
    P2M1 = 0x00 ;
    P3M0 = 0x00 ;
    P3M1 = 0x00 ;
    P1M0 = 0x00 ;
    P1M1 = 0x00 ;       //P1?P3????
    //????0
    IT0 = 1 ;           //???????(?0??????????????)
    EX0 = 1 ;           //??????0
    PX0 = 0 ;           //????:????
    //485???  ?????
    sbtM485_TRN = 0 ;       //???????
    P_SW2 |= 0x01 ;     //????2????P4.6,P4.7
    Uart2Init() ;
    btSendBusy = 1 ;
    IE2 |= 0x01 ;       //????2??
    IP2 |= 0x01 ;       //???????:????
    EA = 1 ;            //????
    //?????
    sbtLedSel = 0 ;     //???????
    sbtSel0 = 1 ;
    sbtSel1 = 1 ;
    sbtSel2 = 1 ;           //??????????
    ucPutDataTmp = 0 ;
}
/*---------??2??????---------*/
void Uart2_Process( void ) interrupt 8 using 1
{
    if( S2CON & cstUart2Ri )
    {
        ucGetDataTmp = S2BUF ;
        ucPutDataTmp = ucGetDataTmp ;
        S2CON &= ~cstUart2Ri;   //????????0
    }
    if( S2CON & cstUart2Ti )
    {
        btSendBusy = 0 ;            //?????
        S2CON &= ~cstUart2Ti ;      //????????0
    }
}

/*---------????0????---------*/
void ExInt0_Process() interrupt 0
{
    sbtM485_TRN = 1 ;
    S2BUF = ucPutDataTmp ;
    while( btSendBusy ) ;
    btSendBusy = 1 ;
    sbtM485_TRN = 0 ;
}

/*主函数
*初始化系统，然后循环调用按键监听函数以及数码管显示函数
*/
void main()
{
	Init();	
	Init_485();
	while(1)
	{
		ucPutDataTmp %= 16 ;
		if(ucPutDataTmp%16==1) moshi1=2;
		if(moshi1==2) Frequency_Display();
		if(ucPutDataTmp%16==2) Search_Keyscan();
		if(ucPutDataTmp%16==3) KeyScan();	
	}
}