#include "STC15F2K60S2.H"
#include "SelfStepMotor.h"
#include "displayer.h"
#include "beep.h"
/*
1、2号为4位LED灯代替的步进电机，3号为真实的步进电机。
切换控制的操作在main函数中实现
这里实现加速减速操作和初始化操作
*/

unsigned char num;		 //当前控制的机器编号 
unsigned char direction[3];	   //方向（左/右）
unsigned char speed_level[3];         //挡位

extern code char Spin_table[];      //存储使电机旋转的编码，采用二相磁励方式
extern code char HighSpeed_Spin_table[];      //存储使电机旋转的编码，采用一-二相磁励方式
	/*
	       a  b  c  d
	Step1  1  1  0  0
	Step2  0  1  1  0
	Step3  0  0  1  1
	Step4  1  0  0  1
	得出四个控制数分别为0xc,0x6,0x3,0x9
	一-二相磁励方式中8个控制数分别为0x8,0xc,0x4,0x6,0x2,0x3,0x1,0x9
  */

char i=0;
void motorInit(){
	//查原理图得，SM接口对应的四个引脚为P41~P44
	//1~4分别对应p41~p44
  DisplayerInit(); //数码管、led灯的初始化在displayer.h中已经实现
	SetDisplayerArea(0,7);
	//下面将P41~P44设置为推挽模式
	P4 &= 0xe1;   
	P4M0 |= 0x1e; 
	P4M1 &= 0xe1; 
	for(;i<3;i++){
    direction[i]=0; //方向初始为左旋（0左1右）
		speed_level[i]=0;      //初始为0档（静止）
  }
	num=0;    //初始化控制1号
}

void speedUp(unsigned char motorInCtrl){//加挡
	if(speed_level[motorInCtrl]==3) ;
	else {SetBeep(speed_level[motorInCtrl]*1000, 20*speed_level[motorInCtrl]);
				speed_level[motorInCtrl]++;
	}
}

void speeddown(unsigned char motorInCtrl){//减挡
  if(speed_level[motorInCtrl]==0) ;
	else speed_level[motorInCtrl]--;
}


unsigned char Getspeed_level(unsigned char motorInCtrl){//获取对应序号电机的挡位
  return speed_level[motorInCtrl];
}

void turn(unsigned char motorInCtrl){//变向
  direction[motorInCtrl]= !direction[motorInCtrl];
}

void numUp(){//向上切换
  if(num==2) num=0;
	else num++;
}
void numDown(){//向下切换
	if(num==0) num=2;
	else num--;
}  

unsigned char Getnum(){//获取当前正在控制的电机序号
  return num;
}

unsigned char GetDirection(unsigned char motorInCtrl){//获取对应序号电机的方向
  return direction[motorInCtrl];
}

void SM_Ctrl(unsigned char SM_val){
  P41 = (SM_val>>3)&0x1;
	P42 = (SM_val>>2)&0x1;
  P43 = (SM_val>>1)&0x1;
  P44 = SM_val&0x1;	
}
