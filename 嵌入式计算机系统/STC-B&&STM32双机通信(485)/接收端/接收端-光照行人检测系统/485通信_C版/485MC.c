/**********************
myM485 485双机通信例程测试
型号:STC15F2K60S2 主频:11.0592MHz
************************/
#include <STC15F2K60S2.H>
#include <intrins.h>

#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long
	
#define NMAX_KEY 100

/*---------宏定义---------*/
#define cstUart2Ri  0x01                        //接收中断请求标志位
#define cstUart2Ti  0x02                        //发送中断请求标志位

#define cstNoneParity           0           //无校验
#define PARITYBIT cstNoneParity         //定义校验位

/*串口波特率相关*/
#define cstFosc 11059200L               //系统时钟频率 
#define cstBaud2    9600                    //串口波特率    
#define cstT2HL     (65536-(cstFosc/4/cstBaud2))    //定时器初始时间

/*---------引脚别名定义---------*/
sbit sbtKey1 = P3 ^ 2 ;                 //启动发送
sbit sbtKey2 = P3 ^ 3 ;                 //数字减少
sbit sbtKey3 = P1 ^ 7 ;                 //数字增加

sbit sbtSel0 = P2 ^ 0 ;
sbit sbtSel1 = P2 ^ 1 ;
sbit sbtSel2 = P2 ^ 2 ;
sbit sbtLedSel = P2 ^ 3 ;

sbit sbtM485_TRN  = P3 ^ 7 ;   //定义MAX485使能引脚，为1时发送，为0时接收

sbit beep=P3^4;

/*---------变量定义---------*/
bit btSendBusy ;                            //为1时忙（发送数据），为0时闲
uchar ucGetDataTmp ;          //接收数据暂存
uchar ucPutDataTmp ;          //发送数据暂存
uchar arrSegSelect[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                        0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                        0x77, 0x7c, 0x39, 0x5e, 0x79,
                        0x71, 0x40, 0x00
                       };   //段选，显示0-f

uint l=0;						
uint suml=0;				
uint datelight=0;		
uint light_old=1;		
uint light_new=1;		

uint light_bai=0;		
uint light_shi=0;		
uint light_ge=0;		
uint time=0;				
			
uint light=0;				//?
uint light_down=20; //????
uint light_up=100;	//????
uchar flag_beep=0;												 

uint down_bai=0;		//??
uint down_shi=0;		//??
uint down_ge =0;		//??

uint up_bai=0;		//??
uint up_shi=0;		//??
uint up_ge =0;		//??

uint flag=0;

uchar G_count;
uchar display;
uint Key1_count;
uint Key2_count;
uint Key3_count;
uint Key_count;

bit Key1_C;				//key1?????
bit Key1_P;				//key1?????
char segtable[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void Delay(int n);
void SEG_Display();
void InitADC_light();
void date_processlight();

void InitADC_light()
{
	P1ASF=0xff;
	ADC_RES=0;
	ADC_RESL=0;
	ADC_CONTR=0X8C;
	CLK_DIV=0X20;
}

//??????????
void date_processlight()
{
	light_bai=datelight%1000/100;
	light_shi=datelight%100/10;
	light_ge=datelight%10;	
}
void date_processlight_down()
{
	down_bai=light_down%1000/100;
	down_shi=light_down%100/10;
	down_ge =light_down%10;
}
void date_processlight_up()
{
	up_bai=light_up%1000/100;
	up_shi=light_up%100/10;
	up_ge =light_up%10;
}

void check()
{
			if(sbtKey1==0)
			Key1_count--;
	
			Key_count--;					//?????1

			if(Key_count==0)			//100???
			{
	
				if(Key1_count<0x80)
				{
					Key1_C=0;
					if(Key1_P==1)			//???(?????)
					{
						Key1_P=0;
						flag_beep=!flag_beep;
					}
				}
				if(Key1_count>=0x80)
				{
					Key1_C=1;
					if(Key1_P==0)
					   Key1_P=1;			//???(??????????)
				}
	
				//??????
				Key1_count=0x80+NMAX_KEY/3*2;
				Key_count=NMAX_KEY;
			}
}

//AD??
void adc_isr() interrupt 5 using 1
{
  time++;
	IE=0x00;											//????
	
	if(time>2500)
	{
			light_new=(suml+l/2)/l;		//????
			suml=0;
			l=0;
			time=0;
		if(ucPutDataTmp%16!=3){
			if((light_old/light_new)>1.30)
			{					  
			  datelight=datelight+1;
flag=~flag;				
			}}
	   	date_processlight();
			if(ucPutDataTmp%16==3){
			date_processlight_down();
			date_processlight_up();	
			}		
			else time=0;	 
			light_old=light_new; 
	}

	//????????
	l++;
	suml+=ADC_RES*256+ADC_RESL; 	//?l?AD???

	ADC_CONTR&=~0X10;  					 //?????,ADC_FLAG??
	ADC_CONTR|=0X08;	 					 //?????,ADC_START?1
	
	
	//IE=0xa0;					 					 //????
	if(ucPutDataTmp%16!=3) IE=0xa0;
	else IE=0xa8;
	
}

void Timer1_Routine() interrupt 3	//3????1????
{
	IE=0x00;												//?????
	TR1=0;													//???1??
	TH1=(65535-100)/256;						//????
	TL1=(65535-100)%256;

	check();
	//beep=1;
	
	if(flag_beep==0)
	{
		if(light<light_down)
				beep=~beep;				  		 //???????????
		else beep=0;								 //???????,????0棗?????
	}
	if(flag_beep==1)
	{
		if(light>light_up)
				beep=~beep;				 		  //???????????
		else beep=0;
	}

	IE=0xa8;											//?????
	TR1=1;												//?????1
}
//???????
void weixuan(char i)
{
	sbtSel2=i/4;
	sbtSel1=i%4/2;
	sbtSel0=i%2;
}
void SEG_Display()
{
	P0=0;
	weixuan(5);
	P0=segtable[light_bai];
	Delay(10);
	P0=0;
	weixuan(6);
	P0=segtable[light_shi];
	Delay(10);
	P0=0;
	weixuan(7);
	P0=segtable[light_ge];
	Delay(10);
}

void SEG_Display_beep()
{
	//????????
	P0=0;
	weixuan(5);
	P0=segtable[light_bai];
	Delay(10);
	
	P0=0;
	weixuan(6);
	P0=segtable[light_shi];
	Delay(10);
	
	P0=0;
	weixuan(7);
	P0=segtable[light_ge];
	Delay(10);
	
	if(flag_beep==0)    //flag?0 ????? 020
	{
		P0=0;
		weixuan(0);
		P0=segtable[down_bai];
		Delay(10);
	
		P0=0;
		weixuan(1);
		P0=segtable[down_shi];
		Delay(10);
	
		P0=0;
		weixuan(2);
		P0=segtable[down_ge];
		Delay(10);
	} 
	if(flag_beep==1)   //flag?1 ????? 100
	{
		P0=0;
		weixuan(0);
		P0=segtable[up_bai];
		Delay(10);
	
		P0=0;
		weixuan(1);
		P0=segtable[up_shi];
		Delay(10);
	
		P0=0;
		weixuan(2);
		P0=segtable[up_ge];
		Delay(10);
	}
}

void Delay(int n)
{
	int x;
	while(n--)
	{
		x=60;
		while(x--);
	}
}
											 
/*---------串口2初始化及波特率发生函数---------*/
void Uart2Init( void )
{
    S2CON = 0x10 ;      //定义无校验位，允许串行口2接收
    T2L = cstT2HL ;             //设置波特率重装值
    T2H = cstT2HL >> 8 ;
    AUXR |= 0x14 ;      //T2为1T模式，并启动定时器2
}

/*---------系统硬件、变量初始化函数---------*/
void Init()
{
    P0M0 = 0xff ;       //P0口推挽（放大电流）
    P0M1 = 0x00 ;
    //P2M0 = 0x0f ;       //P2.0—~P2.3口推挽
    P2M0 = 0x08;
	P2M1 = 0x00 ;
    //P3M0 = 0x00 ;
	P3M0 = 0x10 ;
    P3M1 = 0x00 ;
    //P1M0 = 0x00 ;
	P1M0 = 0x80 ;
    P1M1 = 0x00 ;       //P1、P3准双向口
    //外部中断0
    IT0 = 1 ;           //下降沿触发中断（为0则下降沿和上升沿均会触发中断）
    EX0 = 1 ;           //允许外部中断0
    PX0 = 0 ;           //外部中断：低优先级
    //485初始化  波特率生成
    sbtM485_TRN = 0 ;       //初始为接收状态
    P_SW2 |= 0x01 ;     //切换串口2的管脚到P4.6,P4.7
    Uart2Init() ;
    btSendBusy = 1 ;
    IE2 |= 0x01 ;       //开串行口2中断
    IP2 |= 0x01 ;       //设置串行口中断：高优先级
    EA = 1 ;            //开总中断
    //数码管选择
    sbtLedSel = 0 ;     //开启数码管显示
    sbtSel0 = 1 ;
    sbtSel1 = 1 ;
    sbtSel2 = 1 ;           //选择第八位数码管显示
    ucPutDataTmp = 0 ;
}

/*---------延时函数---------*/
void delay( void )
{
    uchar i, j;
    for( i = 0; i < 255; i++ )
        for( j = 0; j < 255; j++ )
            ;
}

/*---------串口2中断处理程序---------*/
void Uart2_Process( void ) interrupt 8 using 1
{
    if( S2CON & cstUart2Ri )
    {
        ucGetDataTmp = S2BUF ;
        ucPutDataTmp = ucGetDataTmp ;
        S2CON &= ~cstUart2Ri;   //接收中断标志位清0
    }
    if( S2CON & cstUart2Ti )
    {
        btSendBusy = 0 ;            //清除忙信号
        S2CON &= ~cstUart2Ti ;      //发送中断标志位清0
    }
}

/*---------外部中断0处理程序---------*/
void ExInt0_Process() interrupt 0
{
    sbtM485_TRN = 1 ;
    S2BUF = ucPutDataTmp ;
    while( btSendBusy ) ;
    btSendBusy = 1 ;
    sbtM485_TRN = 0 ;
}

/*---------主函数---------*/
void main( void )
{
    Init() ;
		//P0M1=0x00;						//??P0?????,?????
	//P0M0=0xff;
	//P2M1=0x00;
	//P2M0=0x08;
	//sbtLedSel=0;	
	//IE=0xa0;	
		
	
	
	//beep
	
		Key1_C=1;							//key1?????
	Key1_P=1;							//key1?????
	Key1_count=0x80+NMAX_KEY/3*2;
	Key_count=NMAX_KEY;
	
	IE=0xa8;
	TMOD=0x10;
	TH1=(65535-100)/256;
	TL1=(65535-100)%256;
	TR1=1;
	
	InitADC_light();
	
	//beep
	
    while( 1 )
    {
        ucPutDataTmp %= 16 ;
			if(ucPutDataTmp%16==1) SEG_Display();
			if(ucPutDataTmp%16==2){
					//P0=arrSegSelect[ucPutDataTmp];
				if(flag)
				{
					P0=arrSegSelect[10];
				}}
			if(ucPutDataTmp%16==3) SEG_Display_beep();
        P0 = arrSegSelect[ucPutDataTmp] ;
		/*
        if( sbtKey3 == 0 )
        {
            delay();
            if( sbtKey3 == 0 )
            {
                while( !sbtKey3 );
                ucPutDataTmp++ ;
            }
        }
        if( sbtKey2 == 0 )
        {
            delay();
            if( sbtKey2 == 0 )
            {
                while( !sbtKey2 );
                ucPutDataTmp-- ;
            }
        }*/
    }
}
