#include "FM.h"	
/*��ʼ����̨����
*����̨��ʼ������Ƶ������Ϊ87500
*/
void Init_Radio()
{
	Get_CHAN();
	Get_Frequency();	
	IIC_Radio_WriteData();
	Delay(50);
}

/************************************RDA5807PоƬ��IIC��������***************************************/
//IICͨ�ſ�ʼ
void IIC_Radio_Start()
{
	FM_DATA=UP;
	FM_CLOCK=UP;
	FM_DATA=DOWN;
	Delayus(10);
	FM_CLOCK=DOWN;
	Delayus(10);
}
//IICͨ�Ž���
void IIC_Radio_Stop()
{
	FM_DATA=DOWN;
	FM_CLOCK=UP;
	Delayus(10);
	FM_DATA=UP;
	Delayus(10);
}
//����Ӧ���ź�
void IIC_Radio_MACK()
{
   FM_DATA=DOWN;
   Delayus(10);
   FM_CLOCK=UP;
   Delayus(10);
   FM_CLOCK=DOWN;
   Delayus(10);
   FM_DATA=UP;
}
//�ӻ�Ӧ���ź�
void IIC_Radio_ACK()
{
	unsigned char i=0;
	FM_CLOCK=UP;
	while((FM_DATA==1)&&(i<250))	i++;
	FM_CLOCK=DOWN;
	Delayus(10);	
}
//д��һ�ֽ�����
void IIC_Radio_WriteByte(unsigned char Data)
{
	unsigned char i;

	for(i=0;i!=8;i++)
	{
		FM_CLOCK=DOWN;
		Delayus(10);
		FM_DATA=(bit) (Data&0x80);
		Data=Data<<1;
		Delayus(10);
		FM_CLOCK=UP;
		Delayus(10);	
	}	
	FM_CLOCK=DOWN;
	Delayus(10);
	FM_CLOCK=UP;
	Delayus(10);
}
//���ӻ�һ��д����ֽ�����
void IIC_Radio_WriteData()
{
	unsigned char i;
	IIC_Radio_Start();			  //IIC��ʼ
	IIC_Radio_WriteByte(0x20);					 //������IIC������д��ӻ��ĵ�ַ��Ϣ����ӻ�����ͨ��
	IIC_Radio_ACK();							 //�ӻ��յ����ӦACK
	for(i=0;i<8;i++)								   //����д����ֽ�����
	{
		IIC_Radio_WriteByte(Radio_Write_Data[i]);
		IIC_Radio_MACK();
	}
	IIC_Radio_Stop();									//IICֹͣ
}	
 //��ȡCHANֵ
void Get_CHAN()
{
	chan = Radio_Write_Data[2];
	chan = chan*4 + Radio_Write_Data[3]/64;
}
//��ȡ��̨Ƶ��ֵ����CHANתƵ��
void Get_Frequency()
{
	frequency = 100*chan + 87000;
}	
//����̨Ƶ��ת��ΪPLLֵ�ĺ���
void FrequencyToChan(void) //Ƶ��תCHAN
{
	chan = (frequency-87000)/100;
}
//�ֶ���̨����������flag���������ϻ���������̨��ÿ��ֻ�ܵ�Ƶ100KHZ
void Manual_Search(unsigned char flag)
{
	if( flag==1 )							   //flag==1,��ʾ������̨�����ұ�֤��̨Ƶ�ʲ��ܳ���	87500~108000
	{
		frequency += 100;
		if( frequency>MAX_Frequency )
			frequency = MIN_Frequency;
	}
	else if( flag==-1 )								   //flag==1,��ʾ������̨
	{
		frequency -= 100;
		if( frequency<MIN_Frequency )
			frequency = MAX_Frequency;
	}
	FrequencyToChan();										  //��Ƶ��ת��ΪCHAN
	Radio_Write_Data[2] = chan/4;
	Radio_Write_Data[3] = ((chan%4)<<6)|0x10;
	IIC_Radio_WriteData();									  //Ȼ��CHANд��������оƬ����ؼĴ���ʵ���ֶ���Ƶ�Ĺ���
}
//��������������������flag�����������Ǽ�С����
void SetVolume(unsigned char flag)
{
	volume = Radio_Write_Data[7] & 0x0f;
	if( flag==1 )				  //flag==1��������
	{
		if(volume==15)
			volume=0;
		else 
			volume++;
	}
	else 
	{
		if( volume==0 )			  //flag==0��������
			volume=15;
		else
			volume--;
	}
	Radio_Write_Data[7] = (Radio_Write_Data[7]&0xf0) | (volume&0x0f);
	IIC_Radio_WriteData();							   //���ı�������ֵд��������оƬ����ؼĴ����Ӷ��ﵽ��������������Ч��
}
//����̨Ƶ�ʺ�����ֵ��ʾ����Ӧ�������
void Frequency_Display()
{

	unsigned long fre;
	fre = frequency/100;
	volume = Radio_Write_Data[7] & 0x0f;
	
	//������ʾ
	if( volume<10 )
		Display_LED_Num( volume,1,0);
	else
	{
		Display_LED_Num( volume/10,1,0);
		Display_LED_Num( volume%10,2,0);
	}
	
	//Ƶ����ʾ
	if(fre/1000)
	{
		Display_LED_Num(fre/1000,5,0);
		Display_LED_Num(fre%1000/100,6,0);
		Display_LED_Num(fre%1000%100/10,7,1);
		Display_LED_Num(fre%1000%100%10,8,0);	
	}	
	else
	{
		Display_LED_Num(fre/100,6,0);
		Display_LED_Num(fre%100/10,7,1);
		Display_LED_Num(fre%100%10,8,0);
	}
}
