#include "FM.h"	
/*初始化电台函数
*将电台初始化收听频率设置为87500
*/
void Init_Radio()
{
	Get_CHAN();
	Get_Frequency();	
	IIC_Radio_WriteData();
	Delay(50);
}

/************************************RDA5807P芯片的IIC操作函数***************************************/
//IIC通信开始
void IIC_Radio_Start()
{
	FM_DATA=UP;
	FM_CLOCK=UP;
	FM_DATA=DOWN;
	Delayus(10);
	FM_CLOCK=DOWN;
	Delayus(10);
}
//IIC通信结束
void IIC_Radio_Stop()
{
	FM_DATA=DOWN;
	FM_CLOCK=UP;
	Delayus(10);
	FM_DATA=UP;
	Delayus(10);
}
//主机应答信号
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
//从机应答信号
void IIC_Radio_ACK()
{
	unsigned char i=0;
	FM_CLOCK=UP;
	while((FM_DATA==1)&&(i<250))	i++;
	FM_CLOCK=DOWN;
	Delayus(10);	
}
//写入一字节数据
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
//往从机一次写入八字节数据
void IIC_Radio_WriteData()
{
	unsigned char i;
	IIC_Radio_Start();			  //IIC开始
	IIC_Radio_WriteByte(0x20);					 //主机向IIC总线上写入从机的地址信息，与从机建立通信
	IIC_Radio_ACK();							 //从机收到后回应ACK
	for(i=0;i<8;i++)								   //连续写入八字节数据
	{
		IIC_Radio_WriteByte(Radio_Write_Data[i]);
		IIC_Radio_MACK();
	}
	IIC_Radio_Stop();									//IIC停止
}	
 //获取CHAN值
void Get_CHAN()
{
	chan = Radio_Write_Data[2];
	chan = chan*4 + Radio_Write_Data[3]/64;
}
//获取电台频率值，即CHAN转频率
void Get_Frequency()
{
	frequency = 100*chan + 87000;
}	
//将电台频率转化为PLL值的函数
void FrequencyToChan(void) //频率转CHAN
{
	chan = (frequency-87000)/100;
}
//手动搜台函数，参数flag决定是向上还是向下搜台，每次只能调频100KHZ
void Manual_Search(unsigned char flag)
{
	if( flag==1 )							   //flag==1,表示向上搜台，并且保证电台频率不能超出	87500~108000
	{
		frequency += 100;
		if( frequency>MAX_Frequency )
			frequency = MIN_Frequency;
	}
	else if( flag==-1 )								   //flag==1,表示向下搜台
	{
		frequency -= 100;
		if( frequency<MIN_Frequency )
			frequency = MAX_Frequency;
	}
	FrequencyToChan();										  //将频率转化为CHAN
	Radio_Write_Data[2] = chan/4;
	Radio_Write_Data[3] = ((chan%4)<<6)|0x10;
	IIC_Radio_WriteData();									  //然后将CHAN写入收音机芯片的相关寄存器实现手动调频的功能
}
//设置收音机音量，参数flag决定是增大还是减小音量
void SetVolume(unsigned char flag)
{
	volume = Radio_Write_Data[7] & 0x0f;
	if( flag==1 )				  //flag==1增大音量
	{
		if(volume==15)
			volume=0;
		else 
			volume++;
	}
	else 
	{
		if( volume==0 )			  //flag==0减少音量
			volume=15;
		else
			volume--;
	}
	Radio_Write_Data[7] = (Radio_Write_Data[7]&0xf0) | (volume&0x0f);
	IIC_Radio_WriteData();							   //将改变后的音量值写入收音机芯片的相关寄存器从而达到调节收音音量的效果
}
//将电台频率和音量值显示到相应的数码管
void Frequency_Display()
{

	unsigned long fre;
	fre = frequency/100;
	volume = Radio_Write_Data[7] & 0x0f;
	
	//音量显示
	if( volume<10 )
		Display_LED_Num( volume,1,0);
	else
	{
		Display_LED_Num( volume/10,1,0);
		Display_LED_Num( volume%10,2,0);
	}
	
	//频率显示
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
