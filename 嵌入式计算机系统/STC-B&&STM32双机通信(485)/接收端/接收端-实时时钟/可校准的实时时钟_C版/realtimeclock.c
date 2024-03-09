/**********************
文件名称：realtimeclock.c
作者：
说明：进行实时时钟模块测试的例程
采用定时器0，方式1通过数码管对时间进行显示并进行时分秒的修改
修改记录：
***********************/
/**********************
基于STC15F2K60S2系列单片机C语言编程实现
使用如下头文件，不用另外再包含"REG51.H"
***********************/
#include"STC15F2K60S2.H" //头文件
#include"intrins.H" //头文件
#include"ctype.h"
#define uchar unsigned char 	   //宏定义
#define uint unsigned int
	#define ulong unsigned long
#define ADC_CHS1_7 0X07
	
	
	#define cstUart2Ri  0x01                        //?????????
#define cstUart2Ti  0x02                        //?????????

#define cstNoneParity           0           //???
#define PARITYBIT cstNoneParity    


/*???????*/
#define cstFosc 11059200L               //?????? 
#define cstBaud2    9600                    //?????    
#define cstT2HL     (65536-(cstFosc/4/cstBaud2))    //???????  
/***********时分秒写寄存器**************/
#define DS1302_SECOND_WRITE 0x80			
#define DS1302_MINUTE_WRITE 0x82
#define DS1302_HOUR_WRITE   0x84 
#define DS1302_WEEK_WRITE   0x8A
#define DS1302_DAY_WRITE    0x86
#define DS1302_MONTH_WRITE  0x88
#define DS1302_YEAR_WRITE   0x8C
#define ADC_POWER 0X80
#define ADC_FLAG 0X10  /*当A/D转换完成后，ADC_FLAG要软件清零*/
#define ADC_START 0X08
#define ADC_SPEED_90 0X60
/***********时分秒读寄存器**************/
#define DS1302_SECOND_READ  0x81
#define DS1302_MINUTE_READ  0x83
#define DS1302_HOUR_READ    0x85 
#define DS1302_WEEK_READ    0x8B
#define DS1302_DAY_READ     0x87
#define DS1302_MONTH_READ   0x89
#define DS1302_YEAR_READ    0x8D
#define	P1_7_ADC 0x80
/*时、分、秒标志*/
bit set_H_flag;
bit set_Ms_flag;
bit set_S_flag;

/*时、分、秒值*/
uint set_H_val;
uint set_Ms_val;
uint set_S_val;

bit set_HMS_done;  //时分秒设置完
bit show_set_HMS;  //显示时分秒
bit show_HMS;  //显示时分秒
bit show_key_val;
unsigned char key_val;

/**********************
引脚别名定义
***********************/
/********DS1302*******/
sbit Rtc_sclk = P1^5;			//时钟线引脚,控制数据的输入与输出
sbit Rtc_rst  = P1^6;			//CE线引脚,读、写数据时必须置为高电平
sbit Rtc_io   = P5^4;			//实时时钟的数据线引脚
/********数码管显示******/
sbit Led_sel = P2^3;		   //流水灯和数码管选通引脚
sbit Sel0    = P2^0;		   //Sel0、Sel1、Sel2三位二进制进行数码管位选0-7  	
sbit Sel1    = P2^1;
sbit Sel2    = P2^2;
sbit KEY1=P3^2;	 			  //
sbit KEY2=P3^3;	
sbit KEY3=P1^7;	
sbit sbtM485_TRN  = P3 ^ 7 ;

bit btSendBusy ;                            //?1??(????),?0??
uchar ucGetDataTmp ;          //??????
uchar ucPutDataTmp ;          //??????
uchar arrSegSelect[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                        0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                        0x77, 0x7c, 0x39, 0x5e, 0x79,
                        0x71, 0x40, 0x00
                       };   //??,??0-f
/**********************
变量定义
***********************/
uchar duanxuan[]={ 					  //数码管显示所要用的段码
 
                0x3F,  //"0"
                0x06,  //"1"
                0x5B,  //"2"
                0x4F,  //"3"
                0x66,  //"4"
                0x6D,  //"5"
                0x7D,  //"6"
                0x07,  //"7"
                0x7F,  //"8"
                0x6F,  //"9"
                0x77,  //"A"
                0x7C,  //"B"
                0x39,  //"C"
                0x5E,  //"D"
                0x79,  //"E"
                0x71,  //"F"
                0x76,  //"H"
                0x38,  //"L"
                0x37,  //"n"
                0x3E,  //"u"
                0x73,  //"P"
                0x5C,  //"o"
                0x40,  //"-"
                0x00,  //熄灭
                0x00  //自定义
 
                };

uchar weixuan[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};	   //数码管位选数组
uchar flag;					 //所选择点亮的数码管0-7标志位
uchar temp;					 //要写入到DS1302的数据
/*******************************
 * 函数名：Delayms
 * 描述  ：毫秒延时程序
 * 输入  ：延时i毫秒
 * 输出  ：无
 *******************************/
void Uart2Init( void )
{
    S2CON = 0x10 ;      //??????,?????2??
    T2L = cstT2HL ;             //????????
    T2H = cstT2HL >> 8 ;
    AUXR |= 0x14 ;      //T2?1T??,??????2
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
    Led_sel = 0 ;     //???????
    Sel0 = 1 ;
    Sel1 = 1 ;
    Sel2 = 1 ;           //??????????
    ucPutDataTmp = 0 ;
}
void delay( void )
{
    uchar i, j;
    for( i = 0; i < 255; i++ )
        for( j = 0; j < 255; j++ )
            ;
}
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
void ExInt0_Process() interrupt 0
{
    sbtM485_TRN = 1 ;
    S2BUF = ucPutDataTmp ;
    while( btSendBusy ) ;
    btSendBusy = 1 ;
    sbtM485_TRN = 0 ;
}

void Delayms(char i) 			
{
	while(i--)
	{	
		int n=500;
		while (n--)
	    {
	        _nop_();
	        _nop_();
	        _nop_();
	        _nop_();
	        _nop_();
			_nop_();
	    }
	}
}
/**********************
定义时间结构体
***********************/
typedef struct __SYSTEMTIME__
{
	uchar Second;
	uchar Minute;
	uchar Hour;
	uchar Week;
	uchar Day;
	uchar Month;
	uchar Year;
}SYSTEMTIME; 			//定义的时间类型
SYSTEMTIME t;			
/**********************
函数名称：Ds1302_write
功能描述：Ds1302写入一字节
入口参数：uchar 要写入的数据 
***********************/
void Ds1302_write(uchar temp) 		 //temp:要写入的数据
{
	uchar i;
	for(i=0;i<8;i++)				 //循环8次写入一个字节
	{
		Rtc_sclk=0;					//时钟脉冲拉低
		Rtc_io=(bit)(temp&0x01);	 //取最低位数据
		temp>>=1;					 //右移一位
		Rtc_sclk=1;					  //上升沿输入 	
	}
}

/**********************
函数名称：Ds1302_read
功能描述：Ds1302读取一字节
入口参数：无
出口参数：返回uchar 读出的数据
***********************/
uchar Ds1302_read()	 
{
	 uchar i, dat;
	 for(i=0;i<8;i++)			
	 {
	 	Rtc_sclk=0;				//时钟脉冲拉低	
	 	dat>>=1;				//要返回的数据右移一位	 
		if(Rtc_io==1)			//数据线为高，证明该位数据为1
		dat|=0x80;				//要传输数据的当前值置为1，若不是，则为0
		Rtc_sclk=1;				//拉高时钟线 
	 }
	 return dat;				//返回读取出的数据
}


/**********************
函数名称：WriteDS1302
功能描述：向Addr对应的寄存器中写入数据Data
入口参数：uchar Addr 寄存器地址, uchar Data 要写入寄存器的数据
***********************/
void WriteDS1302(uchar Addr, uchar Data) 	   //Addr: DS1302地址,Data: 要写的数据
{
    Rtc_rst = 0;						 //初始CE线置0
    Rtc_sclk = 0;						 //初始时钟线置0
    Rtc_rst = 1;						 //初始CE线置为1，传输开始
	Ds1302_write(Addr);					 // 地址，传输命令字
	Ds1302_write(Data);			   		 // 写1Byte数据 
	Rtc_rst = 0;   						 //读取结束，CE置0，结束数据的传输
    Rtc_sclk = 1;						 //时钟线拉高
}

/**********************
函数名称：ReadDS1302
功能描述：读出Addr对应的寄存器中的数据
入口参数：uchar cmd 寄存器地址
***********************/
uchar ReadDS1302(uchar cmd)
{
    uchar Data;
    Rtc_rst = 0;			  //初始CE线置为0
    Rtc_sclk = 0;			  //初始时钟线置为0
    Rtc_rst = 1;			  //初始CE置为1，传输开始
    Ds1302_write(cmd);        // 地址，传输命令字 
    Data =Ds1302_read();      // 读1Byte数据	  
	Rtc_rst = 0;			  //读取结束，CE置为0，结束数据的传输
    Rtc_sclk = 1; 			  //时钟线拉高
    return Data;			  //返回得到的数据
}

/**********************
函数名称：DS1302_GetTime_BCD
功能描述：读出DS1302中时、分、秒、年、月、日寄存器中对应的数据
***********************/

SYSTEMTIME DS1302_GetTime()
{
	SYSTEMTIME Time;
 	uchar ReadValue;
 	ReadValue = ReadDS1302(DS1302_SECOND_READ);
 	//将BCD码转换成十进制了
 	Time.Second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取秒寄存器中的数据
	ReadValue=ReadDS1302(DS1302_MINUTE_READ);
 	Time.Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取分寄存器中的数据
 	ReadValue = ReadDS1302(DS1302_HOUR_READ);
 	Time.Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取时寄存器中的数据
	ReadValue = ReadDS1302(DS1302_DAY_READ);
 	Time.Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 		  //读取日寄存器中的数据
 	ReadValue = ReadDS1302(DS1302_WEEK_READ);
 	Time.Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取周寄存器中的数据
 	ReadValue = ReadDS1302(DS1302_MONTH_READ);
 	Time.Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取月寄存器中的数据
 	ReadValue = ReadDS1302(DS1302_YEAR_READ);
 	Time.Year=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 			  //读取年寄存器中的数据

	return Time;
}

/**********************
函数名称：Init
功能描述：完成各部分功能模块的初始化
入口参数：无 
出口参数：无
备注：
***********************/
void Init()
{	P3=0xEF;				  //关蜂鸣器
 	P2M0=0xff;				 //设置推挽模式
   	P2M1=0x00;
    P0M0=0xff;
    P0M1=0x00;
	Led_sel=0;				 //选通数码管

 	TMOD=0x01;				//定时器0，方式1
	EA=1;					//打开总的中断
	ET0=1;					//开启定时器中断	
 	TH0=(65535-1000)/256;	//设置定时初值
	TL0=(65535-1000)%256;
	TR0=1;					//启动定时器
}
void Init_key()
{
	/*时、分、秒值*/
	set_H_val=7;
	set_Ms_val=8;
	set_S_val=9;
	set_HMS_done=0;  //时分秒设置完
	/*时、分、秒标志*/
	set_H_flag=0;
	set_Ms_flag=0;
	set_S_flag=0;
	/*默认显示时、分、秒标志*/
	show_HMS=1;
	show_set_HMS=0;
	show_key_val=0;
	key_val=0x00;
//	P3M0=0x10;			   //设置推挽模式	 蜂鸣器不用推挽
//	P3M1=0x00;
//	P1M0=0x00;
//	P1M1=0x00;
}
/**************************************
 * 函数名：GetADC
 * 描述  ：获得AD转换的值,没有设置A/D转换中断（具体看IE、IP）
 * 输入  ：无
 * 输出  ：AD转换的结果
 **************************************/
unsigned char GetADC()
{
	unsigned char result;
	ADC_CONTR = ADC_POWER | ADC_START | ADC_SPEED_90 | ADC_CHS1_7;//没有将ADC_FLAG置1，用于判断A/D是否结束
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR&ADC_FLAG));//等待直到A/D转换结束
	ADC_CONTR &= ~ADC_FLAG; //ADC_FLAGE软件清0
	result = ADC_RES; //获取AD的值
	return result;	  //返回ADC值
}

/********************************
 * 函数名：Fun_Keycheck()	   
 * 描述  ：检测功能键的上5、下2、左4、右1、确认键3、开关按键3（0），没按下返回0x07，返回相应的值  (包含消抖过程)
 * 输入  ：无
 * 输出  ：键对应的值
********************************/
unsigned char Fun_Keycheck()
{
	unsigned char key;
	key=GetADC();		  //获得ADC值赋值给key
	if(key!=255)
	{
		Delayms(1);
		key=GetADC();
		if(key!=255)	  //按键消抖
		{
	     	key=key&0xE0;//获取高3位，其他位清零
        	key=_cror_(key,5);//循环右移5位
			return key;
		}
	}
	return 0x07;
}

/**********************
函数名称：Fun_Key_task_HMS
功能描述：监听功能键，完成时分秒相关值的设置，左对应时、有对应秒、确认对应分；
		  上对应值加1、下对应值减1
入口参数：无 
出口参数：无
***********************/
void Fun_Key_task_HMS()
{
	key_val=Fun_Keycheck();
	switch(key_val)	   
	{
		case 0x05:
		if(set_H_flag)//小时加1
		set_H_val=(set_H_val+1)%24;
		if(set_Ms_flag)//分钟加1
		set_Ms_val=(set_Ms_val+1)%60;
		if(set_S_flag)//秒钟加1
	   	set_S_val=(set_S_val+1)%60;
		break; 	//上  加
	    case 0x02:
		if(set_H_flag)//小时减1
		set_H_val=(set_H_val>0)?set_H_val-1:23;
		if(set_Ms_flag)//分钟减1
		set_Ms_val=(set_Ms_val>0)?set_Ms_val-1:59;
		if(set_S_flag)//秒钟减1
	   	set_S_val=(set_S_val>0)?set_S_val-1:59;
		break;  //下  减
		case 0x04:
		{
			set_H_flag=1;
			set_S_flag=0;
			set_Ms_flag=0;
		}
		break; 	//左   时
	    case 0x01:
		{
			set_S_flag=1;
			set_H_flag=0;
			set_Ms_flag=0;
		}
		break;  //右	  秒
		case 0x03:
		{
			set_Ms_flag=1;
			set_H_flag=0;
			set_S_flag=0;
		}
		break;	//确认 分
		default:break;	  
	}
}
/**********************
函数名称：Fun_key1
功能描述：监听按键1，完成时分秒设置
入口参数：无 
出口参数：无
***********************/
void Fun_key1()
{
	if(KEY1==0)// 按键2控制时分秒的设置
	{		 	
 		Delayms(5);
		while(!KEY1); //消抖
		
		set_HMS_done=0;
		show_set_HMS=1;  //显示十分秒

		show_HMS=0;

		/*默认设置分*/
		set_H_flag=0;				   //设置时间时默认设置分钟
		set_Ms_flag=1;
		set_S_flag=0;

		set_H_val=t.Hour;				//把此时的时间作为设置时间的默认值
		set_Ms_val=t.Minute;
		set_S_val=t.Second;	

		while(1)
		{

			if(KEY1==0)
			{
				Delayms(5);
				while(!KEY1);					//按键消抖
				set_H_flag=0;
				set_Ms_flag=0;
				set_S_flag=0;
				set_HMS_done=1;				   //时间设置完成
				break;						   //退出时间设置
			}
			else
			{
				Fun_Key_task_HMS();			   //进入设置选项
				while(key_val!=0x07)
				{
					key_val=Fun_Keycheck();
				}
			}
		}
	}
}
/**********************
函数名称：Key_OFFON
功能描述：设置时分秒，进行时间的校准。
入口参数：无 
出口参数：无
***********************/
void Key_OFFON()
{
	uchar temp,dtime;
	uchar table_D_BCD[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
 	t=DS1302_GetTime();

    key_val=Fun_Keycheck();
	if(key_val==0x03)
	{
		while(key_val==0x03)
		{
			key_val=Fun_Keycheck();
		}
		set_HMS_done=0;
	
	}
	if(set_HMS_done) //完成设置 对应校时功能
	{				
		show_set_HMS=0;			
		show_HMS=1;//显示时分秒 
		WriteDS1302(0x8E,0x00);	 //禁止写保护位
		temp=ReadDS1302(DS1302_SECOND_READ)|0x80;
		WriteDS1302(0x80,temp);//晶振停止工作
		/*写入时、分、秒值*/	 
		dtime=(table_D_BCD[set_S_val/10]<<4)|table_D_BCD[set_S_val%10];	
		WriteDS1302(DS1302_SECOND_WRITE,dtime);
		dtime=(table_D_BCD[set_Ms_val/10]<<4)|table_D_BCD[set_Ms_val%10];
		WriteDS1302(DS1302_MINUTE_WRITE,dtime);
		dtime=(table_D_BCD[set_H_val/10]<<4)|table_D_BCD[set_H_val%10];	
		WriteDS1302(DS1302_HOUR_WRITE,dtime);
		WriteDS1302(0x8E,0x80); //写保护位置1
		set_HMS_done = 0;
	}
}

void Initial_DS1302(void)	
{	unsigned char hour,min,sec;
//    unsigned char code DataStr[]=__DATE__;	  //格式: "Jan 13 2017"   12字符（含结束符）
    unsigned char code DataStr[]=__TIME__;	  //格式："09:12:04"	  9字符（含结束符）

    hour=((toint(DataStr[0]))<<4)+toint(DataStr[1]);
	min=((toint(DataStr[3]))<<4)+toint(DataStr[4]);
	sec=((toint(DataStr[6]))<<4)+toint(DataStr[7]);

    WriteDS1302(0x8E,0x00);			 //禁止写保护位	
	WriteDS1302(DS1302_SECOND_WRITE,sec);
	WriteDS1302(DS1302_MINUTE_WRITE,min);
	WriteDS1302(DS1302_HOUR_WRITE,hour);
	
	temp=ReadDS1302(DS1302_SECOND_READ)&0x7f ;
	WriteDS1302(0x80,temp);		   	//晶振开始工作
//	WriteDS1302(0x90,0xa9);         //  充电设置：允许充电，2个二极管，2K电阻 		
	WriteDS1302(0x8E,0x80);			 //写保护位置1						 
}
	
void set_charge_DS1302()
{  WriteDS1302(0x8E,0x00);			 //禁止写保护位
   WriteDS1302(0x90,0xa9);         //  充电设置：允许充电，2个二极管，2K电阻 		
   WriteDS1302(0x8E,0x80);			 //写保护位置1
}
/********************************定时器中断处理程序************************************************/
void timer0() interrupt 1	  //把数码管的显示提到中断里面来了
 {
 	TH0=(65535-1000)/256;	  //重新填充初值
	TL0=(65535-1000)%256;

	/********显示程序*******/
	flag++;						  
	if(flag==8)					  
	flag=0;
	P0=0x00;							   
	P2=weixuan[flag];
	if(show_HMS)
	{  //显示格式14-23-54（14点23分54秒）
		switch(flag){
		case 0:P0=duanxuan[t.Hour/10];break; 
	    case 1:P0=duanxuan[t.Hour%10];break; 
		case 3:P0=duanxuan[t.Minute/10];break; 
		case 4:P0=duanxuan[t.Minute%10];break; 
		case 6:P0=duanxuan[t.Second/10];break; 
		case 7:P0=duanxuan[t.Second%10];break; 
		default:P0=duanxuan[22];break;
		}
	}
	if(show_set_HMS)
	{  //显示格式14-23-54（14点23分54秒）
		switch(flag){
		case 0:P0=duanxuan[set_H_val/10];break; 
		case 1:P0=(set_H_flag==1)?duanxuan[set_H_val%10]|0x80:duanxuan[set_H_val%10];break; 
		case 3:P0=duanxuan[set_Ms_val/10];break; 
		case 4:P0=(set_Ms_flag==1)?duanxuan[set_Ms_val%10]|0x80:duanxuan[set_Ms_val%10];break; 
		case 6:P0=duanxuan[set_S_val/10];break; 
		case 7:P0=(set_S_flag==1)?duanxuan[set_S_val%10]|0x80:duanxuan[set_S_val%10];break;  
		default:P0=duanxuan[22];break;
		}
	}
	if(show_key_val)
	{
		switch(flag){
		case 0:P0=duanxuan[key_val/10];break; 
		case 1:P0=duanxuan[key_val%10];break; 
		default:P0=duanxuan[22];break;
		}
	}
}
/**********************************
 * 函数名：Init_ADC
 * 描述  ：初始化P1.7口为ADC
 * 输入  ：无
 * 输出  ：无
 **********************************/
void Init_ADC()
{
	P1ASF=P1_7_ADC;//将P1ASF寄存器对应位置1
	ADC_RES = 0;//结果寄存器清零
//	ADC_RESL=0;
	ADC_CONTR = ADC_POWER | ADC_FLAG | ADC_START | ADC_SPEED_90 | ADC_CHS1_7;		//对应位赋值
	Delayms(2);
}
uint moshi1=1;
void main()
{
	Init();												 //系统初始化
	Init_ADC();											 //ADC初始化
	Init_key();											 //按键初始化
	Init_485();
	if (ReadDS1302(DS1302_SECOND_READ)&0x80)			  //判断掉电之后时钟是否还在运行
	Initial_DS1302();									  //若正在运行，则进行DS1302初始化
	set_charge_DS1302();
	while(1)
	{
		ucPutDataTmp %= 16 ;
		if(ucPutDataTmp%16==1){moshi1=2;}
		if(moshi1==2){
		Delayms(200);								  //延时取值，减少取值次数
		t=DS1302_GetTime();							  //从DS1302取值送给结构体t
		if(ucPutDataTmp%16==2) Fun_key1();										  
		Key_OFFON();			
		if (ReadDS1302(DS1302_SECOND_READ)&0x80)	
		Initial_DS1302();							 //取时间各数据放到结构体t中
	}
	}	

}