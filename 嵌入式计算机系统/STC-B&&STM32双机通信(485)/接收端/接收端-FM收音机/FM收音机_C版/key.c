#include "key.h"

/*
 * ��������Init_KEY
 * ����  ����ʼ��P1.7��ΪADC
 * ����  ����
 * ���  ����
 */
void Init_KEY()
{
	P1ASF = P1_7_ADC;
	ADC_RES = 0;
	ADC_CONTR = ADC_POWER | ADC_FLAGE | ADC_START | ADC_SPEEDHH | ADC_CHANNEL;
	Delay(2);
}

/*
 * ��������GetADC
 * ����  �����ADת����ֵ
 * ����  ����
 * ���  ��ADת���Ľ��
*/
unsigned char GetADC()
{
	unsigned char result;
	ADC_CONTR = ADC_POWER | ADC_START | ADC_SPEEDHH | ADC_CHANNEL;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR&ADC_FLAGE));
	ADC_CONTR &= ~ADC_FLAGE;
	result = ADC_RES;
	return result;
} 
/*������ɨ�躯��
*���ڵ���������ģ�������������ͨ����ȡ��������A/Dת��ֵ���ж���Ӧ�İ������£�Ȼ��ִ����Ӧ�Ķ���
*/
void Search_Keyscan()
{
	unsigned char key,num;
	key = GetADC();			//��ȡģ�ⰴ�����µ�ADֵ
	if( (key>=96)&&(key<=127) )	//ȷ���� ADֵ111
	{
		key = GetADC();
		if( (key>=96)&&(key<=127) )
		{
			while( 1 )
			{
				key = GetADC();
				Frequency_Display();
				if( key>240 ) 
					break;
			}
		} 		
	}
	else if( (key>=128)&&(key<=160) )		//��� ADֵ148���ֶ���̨����1
	{
		key = GetADC();
		if( (key>=128)&&(key<=160) )
		{
			while(1)
			{
				key = GetADC();
				Frequency_Display();
				if( key>240 ) 
					break;
			}
			num = -1;
			Manual_Search( num );
		}
	}
	else if( (key>=32)&&(key<=63) )	//�Ҽ�	ADֵ38 �ֶ���̨����1
	{
		key = GetADC();
		if( (key>=32)&&(key<=63) )
		{
			while(1)
			{
				key = GetADC();
				Frequency_Display();
				if( key>240 ) 
					break;
			}
			num = 1;
			Manual_Search(num);
		}
	}
	else
		return ;
}
/*����1�Ͱ���2ɨ�躯��
*  �жϰ���1��2�Ƿ��£����������ִ����Ӧ�Ķ���
*/
void KeyScan()
{
	if( KEY1==0 )			//����1���£���С����
	{
		Delayus(50);
		while( !KEY1 )
			Frequency_Display();
		SetVolume(0); 
	}
	if( KEY2==0 )		//����2���£���������
	{
		Delayus(50);
		while( !KEY2 )
			Frequency_Display();
		SetVolume(1); 
	}
}

 