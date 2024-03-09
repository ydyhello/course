#ifndef _selfstepmotor_H_
#define _selfstepmotor_H_
extern void motorInit();    //初始化接口   	
extern void speedUp(unsigned char motorInCtrl); //加速
extern void speedDown(unsigned char motorInCtrl);   //刹车制动
extern unsigned char Getspeed_level(unsigned char motorInCtrl); //获取挡位
extern void turn(unsigned char motorInCtrl);    //变向
extern unsigned char GetDirection(unsigned char motorInCtrl);  //获取方向
extern unsigned char Getnum(); //获取当前控制状态
extern void numUp();  //上切控制
extern void numDown();//下切控制
extern void SM_Ctrl(unsigned char SM_val);//SM端口控制
#endif  