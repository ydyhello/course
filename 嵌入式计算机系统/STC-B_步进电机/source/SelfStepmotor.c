#include "STC15F2K60S2.H"
#include "SelfStepMotor.h"
#include "displayer.h"
#include "beep.h"
/*
1��2��Ϊ4λLED�ƴ���Ĳ��������3��Ϊ��ʵ�Ĳ��������
�л����ƵĲ�����main������ʵ��
����ʵ�ּ��ټ��ٲ����ͳ�ʼ������
*/

unsigned char num;		 //��ǰ���ƵĻ������ 
unsigned char direction[3];	   //������/�ң�
unsigned char speed_level[3];         //��λ

extern code char Spin_table[];      //�洢ʹ�����ת�ı��룬���ö��������ʽ
extern code char HighSpeed_Spin_table[];      //�洢ʹ�����ת�ı��룬����һ-���������ʽ
	/*
	       a  b  c  d
	Step1  1  1  0  0
	Step2  0  1  1  0
	Step3  0  0  1  1
	Step4  1  0  0  1
	�ó��ĸ��������ֱ�Ϊ0xc,0x6,0x3,0x9
	һ-���������ʽ��8���������ֱ�Ϊ0x8,0xc,0x4,0x6,0x2,0x3,0x1,0x9
  */

char i=0;
void motorInit(){
	//��ԭ��ͼ�ã�SM�ӿڶ�Ӧ���ĸ�����ΪP41~P44
	//1~4�ֱ��Ӧp41~p44
  DisplayerInit(); //����ܡ�led�Ƶĳ�ʼ����displayer.h���Ѿ�ʵ��
	SetDisplayerArea(0,7);
	//���潫P41~P44����Ϊ����ģʽ
	P4 &= 0xe1;   
	P4M0 |= 0x1e; 
	P4M1 &= 0xe1; 
	for(;i<3;i++){
    direction[i]=0; //�����ʼΪ������0��1�ң�
		speed_level[i]=0;      //��ʼΪ0������ֹ��
  }
	num=0;    //��ʼ������1��
}

void speedUp(unsigned char motorInCtrl){//�ӵ�
	if(speed_level[motorInCtrl]==3) ;
	else {SetBeep(speed_level[motorInCtrl]*1000, 20*speed_level[motorInCtrl]);
				speed_level[motorInCtrl]++;
	}
}

void speeddown(unsigned char motorInCtrl){//����
  if(speed_level[motorInCtrl]==0) ;
	else speed_level[motorInCtrl]--;
}


unsigned char Getspeed_level(unsigned char motorInCtrl){//��ȡ��Ӧ��ŵ���ĵ�λ
  return speed_level[motorInCtrl];
}

void turn(unsigned char motorInCtrl){//����
  direction[motorInCtrl]= !direction[motorInCtrl];
}

void numUp(){//�����л�
  if(num==2) num=0;
	else num++;
}
void numDown(){//�����л�
	if(num==0) num=2;
	else num--;
}  

unsigned char Getnum(){//��ȡ��ǰ���ڿ��Ƶĵ�����
  return num;
}

unsigned char GetDirection(unsigned char motorInCtrl){//��ȡ��Ӧ��ŵ���ķ���
  return direction[motorInCtrl];
}

void SM_Ctrl(unsigned char SM_val){
  P41 = (SM_val>>3)&0x1;
	P42 = (SM_val>>2)&0x1;
  P43 = (SM_val>>1)&0x1;
  P44 = SM_val&0x1;	
}
