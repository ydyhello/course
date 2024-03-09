/**********************
�ļ����ƣ�realtimeclock.c
���ߣ�
˵��������ʵʱʱ��ģ����Ե�����
���ö�ʱ��0����ʽ1ͨ������ܶ�ʱ�������ʾ������ʱ������޸�
�޸ļ�¼��
***********************/
/**********************
����STC15F2K60S2ϵ�е�Ƭ��C���Ա��ʵ��
ʹ������ͷ�ļ������������ٰ���"REG51.H"
***********************/
#include"STC15F2K60S2.H" //ͷ�ļ�
#include"intrins.H" //ͷ�ļ�
#include"ctype.h"
#define uchar unsigned char 	   //�궨��
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
/***********ʱ����д�Ĵ���**************/
#define DS1302_SECOND_WRITE 0x80			
#define DS1302_MINUTE_WRITE 0x82
#define DS1302_HOUR_WRITE   0x84 
#define DS1302_WEEK_WRITE   0x8A
#define DS1302_DAY_WRITE    0x86
#define DS1302_MONTH_WRITE  0x88
#define DS1302_YEAR_WRITE   0x8C
#define ADC_POWER 0X80
#define ADC_FLAG 0X10  /*��A/Dת����ɺ�ADC_FLAGҪ�������*/
#define ADC_START 0X08
#define ADC_SPEED_90 0X60
/***********ʱ������Ĵ���**************/
#define DS1302_SECOND_READ  0x81
#define DS1302_MINUTE_READ  0x83
#define DS1302_HOUR_READ    0x85 
#define DS1302_WEEK_READ    0x8B
#define DS1302_DAY_READ     0x87
#define DS1302_MONTH_READ   0x89
#define DS1302_YEAR_READ    0x8D
#define	P1_7_ADC 0x80
/*ʱ���֡����־*/
bit set_H_flag;
bit set_Ms_flag;
bit set_S_flag;

/*ʱ���֡���ֵ*/
uint set_H_val;
uint set_Ms_val;
uint set_S_val;

bit set_HMS_done;  //ʱ����������
bit show_set_HMS;  //��ʾʱ����
bit show_HMS;  //��ʾʱ����
bit show_key_val;
unsigned char key_val;

/**********************
���ű�������
***********************/
/********DS1302*******/
sbit Rtc_sclk = P1^5;			//ʱ��������,�������ݵ����������
sbit Rtc_rst  = P1^6;			//CE������,����д����ʱ������Ϊ�ߵ�ƽ
sbit Rtc_io   = P5^4;			//ʵʱʱ�ӵ�����������
/********�������ʾ******/
sbit Led_sel = P2^3;		   //��ˮ�ƺ������ѡͨ����
sbit Sel0    = P2^0;		   //Sel0��Sel1��Sel2��λ�����ƽ��������λѡ0-7  	
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
��������
***********************/
uchar duanxuan[]={ 					  //�������ʾ��Ҫ�õĶ���
 
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
                0x00,  //Ϩ��
                0x00  //�Զ���
 
                };

uchar weixuan[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};	   //�����λѡ����
uchar flag;					 //��ѡ������������0-7��־λ
uchar temp;					 //Ҫд�뵽DS1302������
/*******************************
 * ��������Delayms
 * ����  ��������ʱ����
 * ����  ����ʱi����
 * ���  ����
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
    P2M0 = 0x0f ;       //P2.0�~P2.3???
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
����ʱ��ṹ��
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
}SYSTEMTIME; 			//�����ʱ������
SYSTEMTIME t;			
/**********************
�������ƣ�Ds1302_write
����������Ds1302д��һ�ֽ�
��ڲ�����uchar Ҫд������� 
***********************/
void Ds1302_write(uchar temp) 		 //temp:Ҫд�������
{
	uchar i;
	for(i=0;i<8;i++)				 //ѭ��8��д��һ���ֽ�
	{
		Rtc_sclk=0;					//ʱ����������
		Rtc_io=(bit)(temp&0x01);	 //ȡ���λ����
		temp>>=1;					 //����һλ
		Rtc_sclk=1;					  //���������� 	
	}
}

/**********************
�������ƣ�Ds1302_read
����������Ds1302��ȡһ�ֽ�
��ڲ�������
���ڲ���������uchar ����������
***********************/
uchar Ds1302_read()	 
{
	 uchar i, dat;
	 for(i=0;i<8;i++)			
	 {
	 	Rtc_sclk=0;				//ʱ����������	
	 	dat>>=1;				//Ҫ���ص���������һλ	 
		if(Rtc_io==1)			//������Ϊ�ߣ�֤����λ����Ϊ1
		dat|=0x80;				//Ҫ�������ݵĵ�ǰֵ��Ϊ1�������ǣ���Ϊ0
		Rtc_sclk=1;				//����ʱ���� 
	 }
	 return dat;				//���ض�ȡ��������
}


/**********************
�������ƣ�WriteDS1302
������������Addr��Ӧ�ļĴ�����д������Data
��ڲ�����uchar Addr �Ĵ�����ַ, uchar Data Ҫд��Ĵ���������
***********************/
void WriteDS1302(uchar Addr, uchar Data) 	   //Addr: DS1302��ַ,Data: Ҫд������
{
    Rtc_rst = 0;						 //��ʼCE����0
    Rtc_sclk = 0;						 //��ʼʱ������0
    Rtc_rst = 1;						 //��ʼCE����Ϊ1�����俪ʼ
	Ds1302_write(Addr);					 // ��ַ������������
	Ds1302_write(Data);			   		 // д1Byte���� 
	Rtc_rst = 0;   						 //��ȡ������CE��0���������ݵĴ���
    Rtc_sclk = 1;						 //ʱ��������
}

/**********************
�������ƣ�ReadDS1302
��������������Addr��Ӧ�ļĴ����е�����
��ڲ�����uchar cmd �Ĵ�����ַ
***********************/
uchar ReadDS1302(uchar cmd)
{
    uchar Data;
    Rtc_rst = 0;			  //��ʼCE����Ϊ0
    Rtc_sclk = 0;			  //��ʼʱ������Ϊ0
    Rtc_rst = 1;			  //��ʼCE��Ϊ1�����俪ʼ
    Ds1302_write(cmd);        // ��ַ������������ 
    Data =Ds1302_read();      // ��1Byte����	  
	Rtc_rst = 0;			  //��ȡ������CE��Ϊ0���������ݵĴ���
    Rtc_sclk = 1; 			  //ʱ��������
    return Data;			  //���صõ�������
}

/**********************
�������ƣ�DS1302_GetTime_BCD
��������������DS1302��ʱ���֡��롢�ꡢ�¡��ռĴ����ж�Ӧ������
***********************/

SYSTEMTIME DS1302_GetTime()
{
	SYSTEMTIME Time;
 	uchar ReadValue;
 	ReadValue = ReadDS1302(DS1302_SECOND_READ);
 	//��BCD��ת����ʮ������
 	Time.Second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //��ȡ��Ĵ����е�����
	ReadValue=ReadDS1302(DS1302_MINUTE_READ);
 	Time.Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //��ȡ�ּĴ����е�����
 	ReadValue = ReadDS1302(DS1302_HOUR_READ);
 	Time.Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //��ȡʱ�Ĵ����е�����
	ReadValue = ReadDS1302(DS1302_DAY_READ);
 	Time.Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 		  //��ȡ�ռĴ����е�����
 	ReadValue = ReadDS1302(DS1302_WEEK_READ);
 	Time.Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //��ȡ�ܼĴ����е�����
 	ReadValue = ReadDS1302(DS1302_MONTH_READ);
 	Time.Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //��ȡ�¼Ĵ����е�����
 	ReadValue = ReadDS1302(DS1302_YEAR_READ);
 	Time.Year=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 			  //��ȡ��Ĵ����е�����

	return Time;
}

/**********************
�������ƣ�Init
������������ɸ����ֹ���ģ��ĳ�ʼ��
��ڲ������� 
���ڲ�������
��ע��
***********************/
void Init()
{	P3=0xEF;				  //�ط�����
 	P2M0=0xff;				 //��������ģʽ
   	P2M1=0x00;
    P0M0=0xff;
    P0M1=0x00;
	Led_sel=0;				 //ѡͨ�����

 	TMOD=0x01;				//��ʱ��0����ʽ1
	EA=1;					//���ܵ��ж�
	ET0=1;					//������ʱ���ж�	
 	TH0=(65535-1000)/256;	//���ö�ʱ��ֵ
	TL0=(65535-1000)%256;
	TR0=1;					//������ʱ��
}
void Init_key()
{
	/*ʱ���֡���ֵ*/
	set_H_val=7;
	set_Ms_val=8;
	set_S_val=9;
	set_HMS_done=0;  //ʱ����������
	/*ʱ���֡����־*/
	set_H_flag=0;
	set_Ms_flag=0;
	set_S_flag=0;
	/*Ĭ����ʾʱ���֡����־*/
	show_HMS=1;
	show_set_HMS=0;
	show_key_val=0;
	key_val=0x00;
//	P3M0=0x10;			   //��������ģʽ	 ��������������
//	P3M1=0x00;
//	P1M0=0x00;
//	P1M1=0x00;
}
/**************************************
 * ��������GetADC
 * ����  �����ADת����ֵ,û������A/Dת���жϣ����忴IE��IP��
 * ����  ����
 * ���  ��ADת���Ľ��
 **************************************/
unsigned char GetADC()
{
	unsigned char result;
	ADC_CONTR = ADC_POWER | ADC_START | ADC_SPEED_90 | ADC_CHS1_7;//û�н�ADC_FLAG��1�������ж�A/D�Ƿ����
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR&ADC_FLAG));//�ȴ�ֱ��A/Dת������
	ADC_CONTR &= ~ADC_FLAG; //ADC_FLAGE�����0
	result = ADC_RES; //��ȡAD��ֵ
	return result;	  //����ADCֵ
}

/********************************
 * ��������Fun_Keycheck()	   
 * ����  ����⹦�ܼ�����5����2����4����1��ȷ�ϼ�3�����ذ���3��0����û���·���0x07��������Ӧ��ֵ  (������������)
 * ����  ����
 * ���  ������Ӧ��ֵ
********************************/
unsigned char Fun_Keycheck()
{
	unsigned char key;
	key=GetADC();		  //���ADCֵ��ֵ��key
	if(key!=255)
	{
		Delayms(1);
		key=GetADC();
		if(key!=255)	  //��������
		{
	     	key=key&0xE0;//��ȡ��3λ������λ����
        	key=_cror_(key,5);//ѭ������5λ
			return key;
		}
	}
	return 0x07;
}

/**********************
�������ƣ�Fun_Key_task_HMS
�����������������ܼ������ʱ�������ֵ�����ã����Ӧʱ���ж�Ӧ�롢ȷ�϶�Ӧ�֣�
		  �϶�Ӧֵ��1���¶�Ӧֵ��1
��ڲ������� 
���ڲ�������
***********************/
void Fun_Key_task_HMS()
{
	key_val=Fun_Keycheck();
	switch(key_val)	   
	{
		case 0x05:
		if(set_H_flag)//Сʱ��1
		set_H_val=(set_H_val+1)%24;
		if(set_Ms_flag)//���Ӽ�1
		set_Ms_val=(set_Ms_val+1)%60;
		if(set_S_flag)//���Ӽ�1
	   	set_S_val=(set_S_val+1)%60;
		break; 	//��  ��
	    case 0x02:
		if(set_H_flag)//Сʱ��1
		set_H_val=(set_H_val>0)?set_H_val-1:23;
		if(set_Ms_flag)//���Ӽ�1
		set_Ms_val=(set_Ms_val>0)?set_Ms_val-1:59;
		if(set_S_flag)//���Ӽ�1
	   	set_S_val=(set_S_val>0)?set_S_val-1:59;
		break;  //��  ��
		case 0x04:
		{
			set_H_flag=1;
			set_S_flag=0;
			set_Ms_flag=0;
		}
		break; 	//��   ʱ
	    case 0x01:
		{
			set_S_flag=1;
			set_H_flag=0;
			set_Ms_flag=0;
		}
		break;  //��	  ��
		case 0x03:
		{
			set_Ms_flag=1;
			set_H_flag=0;
			set_S_flag=0;
		}
		break;	//ȷ�� ��
		default:break;	  
	}
}
/**********************
�������ƣ�Fun_key1
������������������1�����ʱ��������
��ڲ������� 
���ڲ�������
***********************/
void Fun_key1()
{
	if(KEY1==0)// ����2����ʱ���������
	{		 	
 		Delayms(5);
		while(!KEY1); //����
		
		set_HMS_done=0;
		show_set_HMS=1;  //��ʾʮ����

		show_HMS=0;

		/*Ĭ�����÷�*/
		set_H_flag=0;				   //����ʱ��ʱĬ�����÷���
		set_Ms_flag=1;
		set_S_flag=0;

		set_H_val=t.Hour;				//�Ѵ�ʱ��ʱ����Ϊ����ʱ���Ĭ��ֵ
		set_Ms_val=t.Minute;
		set_S_val=t.Second;	

		while(1)
		{

			if(KEY1==0)
			{
				Delayms(5);
				while(!KEY1);					//��������
				set_H_flag=0;
				set_Ms_flag=0;
				set_S_flag=0;
				set_HMS_done=1;				   //ʱ���������
				break;						   //�˳�ʱ������
			}
			else
			{
				Fun_Key_task_HMS();			   //��������ѡ��
				while(key_val!=0x07)
				{
					key_val=Fun_Keycheck();
				}
			}
		}
	}
}
/**********************
�������ƣ�Key_OFFON
��������������ʱ���룬����ʱ���У׼��
��ڲ������� 
���ڲ�������
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
	if(set_HMS_done) //������� ��ӦУʱ����
	{				
		show_set_HMS=0;			
		show_HMS=1;//��ʾʱ���� 
		WriteDS1302(0x8E,0x00);	 //��ֹд����λ
		temp=ReadDS1302(DS1302_SECOND_READ)|0x80;
		WriteDS1302(0x80,temp);//����ֹͣ����
		/*д��ʱ���֡���ֵ*/	 
		dtime=(table_D_BCD[set_S_val/10]<<4)|table_D_BCD[set_S_val%10];	
		WriteDS1302(DS1302_SECOND_WRITE,dtime);
		dtime=(table_D_BCD[set_Ms_val/10]<<4)|table_D_BCD[set_Ms_val%10];
		WriteDS1302(DS1302_MINUTE_WRITE,dtime);
		dtime=(table_D_BCD[set_H_val/10]<<4)|table_D_BCD[set_H_val%10];	
		WriteDS1302(DS1302_HOUR_WRITE,dtime);
		WriteDS1302(0x8E,0x80); //д����λ��1
		set_HMS_done = 0;
	}
}

void Initial_DS1302(void)	
{	unsigned char hour,min,sec;
//    unsigned char code DataStr[]=__DATE__;	  //��ʽ: "Jan 13 2017"   12�ַ�������������
    unsigned char code DataStr[]=__TIME__;	  //��ʽ��"09:12:04"	  9�ַ�������������

    hour=((toint(DataStr[0]))<<4)+toint(DataStr[1]);
	min=((toint(DataStr[3]))<<4)+toint(DataStr[4]);
	sec=((toint(DataStr[6]))<<4)+toint(DataStr[7]);

    WriteDS1302(0x8E,0x00);			 //��ֹд����λ	
	WriteDS1302(DS1302_SECOND_WRITE,sec);
	WriteDS1302(DS1302_MINUTE_WRITE,min);
	WriteDS1302(DS1302_HOUR_WRITE,hour);
	
	temp=ReadDS1302(DS1302_SECOND_READ)&0x7f ;
	WriteDS1302(0x80,temp);		   	//����ʼ����
//	WriteDS1302(0x90,0xa9);         //  ������ã������磬2�������ܣ�2K���� 		
	WriteDS1302(0x8E,0x80);			 //д����λ��1						 
}
	
void set_charge_DS1302()
{  WriteDS1302(0x8E,0x00);			 //��ֹд����λ
   WriteDS1302(0x90,0xa9);         //  ������ã������磬2�������ܣ�2K���� 		
   WriteDS1302(0x8E,0x80);			 //д����λ��1
}
/********************************��ʱ���жϴ������************************************************/
void timer0() interrupt 1	  //������ܵ���ʾ�ᵽ�ж���������
 {
 	TH0=(65535-1000)/256;	  //��������ֵ
	TL0=(65535-1000)%256;

	/********��ʾ����*******/
	flag++;						  
	if(flag==8)					  
	flag=0;
	P0=0x00;							   
	P2=weixuan[flag];
	if(show_HMS)
	{  //��ʾ��ʽ14-23-54��14��23��54�룩
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
	{  //��ʾ��ʽ14-23-54��14��23��54�룩
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
 * ��������Init_ADC
 * ����  ����ʼ��P1.7��ΪADC
 * ����  ����
 * ���  ����
 **********************************/
void Init_ADC()
{
	P1ASF=P1_7_ADC;//��P1ASF�Ĵ�����Ӧλ��1
	ADC_RES = 0;//����Ĵ�������
//	ADC_RESL=0;
	ADC_CONTR = ADC_POWER | ADC_FLAG | ADC_START | ADC_SPEED_90 | ADC_CHS1_7;		//��Ӧλ��ֵ
	Delayms(2);
}
uint moshi1=1;
void main()
{
	Init();												 //ϵͳ��ʼ��
	Init_ADC();											 //ADC��ʼ��
	Init_key();											 //������ʼ��
	Init_485();
	if (ReadDS1302(DS1302_SECOND_READ)&0x80)			  //�жϵ���֮��ʱ���Ƿ�������
	Initial_DS1302();									  //���������У������DS1302��ʼ��
	set_charge_DS1302();
	while(1)
	{
		ucPutDataTmp %= 16 ;
		if(ucPutDataTmp%16==1){moshi1=2;}
		if(moshi1==2){
		Delayms(200);								  //��ʱȡֵ������ȡֵ����
		t=DS1302_GetTime();							  //��DS1302ȡֵ�͸��ṹ��t
		if(ucPutDataTmp%16==2) Fun_key1();										  
		Key_OFFON();			
		if (ReadDS1302(DS1302_SECOND_READ)&0x80)	
		Initial_DS1302();							 //ȡʱ������ݷŵ��ṹ��t��
	}
	}	

}