#include "STC15F2K60S2.H"        //���롣
#include "sys.H"                 //���롣
#include "displayer.H" 
#include "adc.H"
#include "key.H"
#include "selfstepmotor.h"
#include "beep.h"
code char Spin_table[]={0xc,0x6,0x3,0x9};      //�洢ʹ�����ת�ı��룬���ö��������ʽ
extern code char HighSpeed_Spin_table[]={0x8,0xc,0x4,0x6,0x2,0x3,0x1,0x9}; //���ϴ���
code unsigned long SysClock=11059200; 
unsigned char a=0x10,b=0x01,c=0,hc=0,k=0x11; //�ֱ���������������ʾ�����ж�
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x73, 0x37, 0xbf, 0x6d , 0x5e,
	              /* ���:   0   1    2	   3    4	    5    6	  7   8	   9	 10	   11		12   13    14     15    16    17 */
                /* ��ʾ:   0   1    2    3    4     5    6    7   8    9  (��)   ��-  ��-  P    n     0.    S     d */  
	                       0x3f|0x80,0x06|0x80,0x5b|0x80,0x4f|0x80,0x66|0x80,0x6d|0x80,0x7d|0x80,0x07|0x80,0x7f|0x80,0x6f|0x80 };  
             /* ��С����     0         1         2         3         4         5         6         7         8         9        */

void setspeed(){//ʵ�ְ�������,k1���٣�k2ɲ��
  char k = GetKeyAct(enumKey1),j = GetKeyAct(enumKey2);
	if(k == enumKeyPress){
    speedUp(Getnum());
    Seg7Print(14,15,Getnum()+1,-1,16,13,17,Getspeed_level(Getnum()));
  }
	if(j == enumKeyPress) {
    speedDown(Getnum());
    Seg7Print(14,15,Getnum()+1,-1,16,13,17,Getspeed_level(Getnum()));
  }
}

void changenum(){//ʵ�ֵ�������,�ϼ��¼��л�����Ŀ��,k3����
  char k = GetAdcNavAct(enumAdcNavKeyUp),j = GetAdcNavAct(enumAdcNavKeyDown),l=GetAdcNavAct(enumAdcNavKey3);
	if(k == enumKeyPress){//�����л�
    numUp();
		Seg7Print(14,15,Getnum()+1,-1,16,13,17,Getspeed_level(Getnum()));
  }
	if(j == enumKeyPress){//�����л�
    numDown();
		Seg7Print(14,15,Getnum()+1,-1,16,13,17,Getspeed_level(Getnum()));
  }
	if(l == enumKeyPress) turn(Getnum());
}

void num1mov()
{	if(GetDirection(0)==0){
        if(a==0x80) a=0x10;
				else a<<=1;
      }
			else{
				if(a==0x10) a=0x80;
				else if(a==0x80) a=0x40;
				else a>>=1;
      }
}

void num2mov()
{
      if(GetDirection(1)==0){
        if(b==0x08) b=0x01;
				else b<<=1;
      }
			else{
				if(b==0x01) b=0x08;
					else b>>=1;
      }
}

void num3mov()
{
		
			SM_Ctrl(HighSpeed_Spin_table[hc]);
      if(GetDirection(2)==0){
				if(hc==7) hc=0;
				else hc++;
     }
		  else{
        if(hc==0) hc=7;
				else hc--;
    }
  
}

int timex=0;
void speed_levelx()
{	timex++;
	k=a|b;
	LedPrint(k);
	if(timex%5==0)
	{
		if(Getspeed_level(0)==3) num1mov();
    if(Getspeed_level(1)==3) num2mov();
		if(Getspeed_level(2)==3) num3mov();
	}
	if(timex%100==0)
	{
		if(Getspeed_level(0)==2) num1mov();
    if(Getspeed_level(1)==2) num2mov();
		if(Getspeed_level(2)==2) num3mov();
	}
		if(timex%500==0)
	{
		if(Getspeed_level(0)==1) num1mov();
    if(Getspeed_level(1)==1) num2mov();
		if(Getspeed_level(2)==1) num3mov();
	}
}


void main() 
{ 
	KeyInit();
	AdcInit(ADCexpEXT);
	motorInit();
	BeepInit();
	DisplayerInit();
	LedPrint(a|b);
	Seg7Print(14,15,1,-1,16,13,17,0);
	SetEventCallBack(enumEventKey,setspeed);         //װ�ذ�������
	SetEventCallBack(enumEventNav,changenum);       //����������
	SetEventCallBack(enumEventSys1mS,speed_levelx);      //ʹ�ûص��������Ƶ�λ
  MySTC_Init();	    
	while(1)             	
		{ MySTC_OS();    
		}	             
}                 