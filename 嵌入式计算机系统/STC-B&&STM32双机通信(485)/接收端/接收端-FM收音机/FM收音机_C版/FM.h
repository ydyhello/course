#ifndef _FM_H_
#define _FM_H_

#include "reg51.h"
#include "Delay.h"
#include "led.h"
//#include "volume.h"

#define MAX_Frequency 108000			  //����̨Ƶ��
#define MIN_Frequency 87500				  //��С��̨Ƶ��
#define DOWN 0							 //�ߵ�ƽֵ
#define UP 1							   //�͵�ƽֵ
/*******���ű�������*****/
sbit FM_CLOCK=P4^5;
sbit FM_DATA=P2^7;

extern unsigned char Radio_Write_Data[8];		 //д��RDA5807PоƬ��ǰ�˸��ֽ�����
extern unsigned long frequency;					//��ǰ�����������ĵ�̨Ƶ��
extern unsigned int chan;						//CHANֵ���ο�ԭ��˵���ĵ���
extern unsigned char volume;					//��̨����ֵ

void Init_Radio();
/*******************************IIC***********************************/
void IIC_Radio_Start();
void IIC_Radio_Stop();
void IIC_Radio_MACK();
void IIC_Radio_ACK();		   
void IIC_Radio_WriteByte(unsigned char Data);
void IIC_Radio_WriteData();

/*******************************3-wire Bus***************************/
void Radio_WriteByte(unsigned char dat);
void Radio_WriteData();
/*******************************General******************************/
void Get_CHAN(); //Ƶ��תCHAN
void Get_Frequency(); //CHANֵתƵ��ֵ
void FrequencyToChan(void); //Ƶ��תCHAN
void Manual_Search(unsigned char flag);
void Frequency_Display();
void SetVolume(unsigned char flag); //��������
#endif