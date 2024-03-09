#include "key.h"

/*
 * 函数名：Init_KEY
 * 描述  ：初始化P1.7口为ADC
 * 输入  ：无
 * 输出  ：无
 */
void Init_KEY()
{
	P1ASF = P1_7_ADC;
	ADC_RES = 0;
	ADC_CONTR = ADC_POWER | ADC_FLAGE | ADC_START | ADC_SPEEDHH | ADC_CHANNEL;
	Delay(2);
}

/*
 * 函数名：GetADC
 * 描述  ：获得AD转换的值
 * 输入  ：无
 * 输出  ：AD转换的结果
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
/*导航键扫描函数
*由于导航按键是模拟量按键，因此通过获取各按键的A/D转换值来判断相应的按键按下，然后执行相应的动作
*/
void Search_Keyscan()
{
	unsigned char key,num;
	key = GetADC();			//获取模拟按键按下的AD值
	if( (key>=96)&&(key<=127) )	//确定键 AD值111
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
	else if( (key>=128)&&(key<=160) )		//左键 AD值148，手动调台，减1
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
	else if( (key>=32)&&(key<=63) )	//右键	AD值38 手动调台，加1
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
/*按键1和按键2扫描函数
*  判断按键1和2是否按下，如果按下则执行相应的动作
*/
void KeyScan()
{
	if( KEY1==0 )			//按键1按下，调小音量
	{
		Delayus(50);
		while( !KEY1 )
			Frequency_Display();
		SetVolume(0); 
	}
	if( KEY2==0 )		//按键2按下，调大音量
	{
		Delayus(50);
		while( !KEY2 )
			Frequency_Display();
		SetVolume(1); 
	}
}

 