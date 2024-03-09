/**********************************StepMotor 说明 ************************************************************************
StepMotor用于STC-B板控制步进电机。共提供1个驱动函数、3个应用函数：
  (1)  StepMotorInit()：步进电机模块驱动函数
  (2)  SetStepMotor(char StepMotor,unsigned char speed ,int steps )  指定步进电机、按指定转动速度、转动指定步
           函数参数：StepMotor  指定步进电机，取值（enum StepMotorName中定义）
				                   enumStepMotor1：  SM 接口上的步进电机
				                   enumStepMotor2：  此时，用L0～L3四个LED模拟一个4相步进电机
				                   enumStepMotor3：  此时，用L4～L7四个LED模拟一个4相步进电机
				             speed      步进电机转动速度（0～255），单位：步/S。 (实际每步时间=int(1000mS/speed) mS），与设置速度可能存在一定误差
				             steps      步进电机转动步数（-32768～32767），负值表示反转
				   函数返回：enumSetStepMotorOK：  调用成功（enum StepMotorActName中定义）
				             enumSetStepMotorFail：调用失败（电机名不在指定范围，或speed=0,或调用时正在转动）
  （3）EmStop(char StepMotor) 紧急停止指定步进电机转动
	           函数参数：StepMotor  指定步进电机。函数参数不对将返回0值。
						 函数返回：剩余未转完的步数
  （4）GetStepMotorStatus(char StepMotor) 获取指定步进电机状态
	           函数参数：StepMotor  指定步进电机
					   函数返回：enumStepMotorFree:自由（enum StepMotorActName中定义）
									     enumStepMotorBusy,忙（正在转动）
											 enumSetStepMotorFail：调用失败（步进电机名不在指定范围）

编写：徐成（电话18008400450）   2021年4月16日设计，2021年4月18日更新
*/

#ifndef _StepMotor_H_			
#define _StepMotor_H_		

extern void StepMotorInit();										                              //  步进电机模块初始化
extern char SetStepMotor(char StepMotor,unsigned char speed ,int steps );     //  指定步进电机、按指定转动速度、转动指定步
																																									//  函数参数：StepMotor  指定步进电机，取值（enum StepMotorName中定义）
																																									//              enumStepMotor1：  SM 接口上的步进电机
																																									//              enumStepMotor2：  此时，用L0～L3四个LED模拟一个4相步进电机
																																									//              enumStepMotor3：  此时，用L4～L7四个LED模拟一个4相步进电机
																																									//            speed      步进电机转动速度（0～255），单位：步/S   (实际每步时间=int(1000mS/speed) mS），与设置速度可能存在一定误差
																																									//            steps      步进电机转动步数（-32768～32767），负值表示反转
																																									//  函数返回：
																																									//              enumSetStepMotorOK：调用成功   
																																									//              enumSetStepMotorFail：调用失败（电机名不在指定范围，或speed=0,或调用时正在转动）
extern int EmStop(char StepMotor);                                            //  紧急停止指定步进电机转动
	                                                                                //  函数参数：StepMotor  指定步进电机(函数参数不对将返回0值)
																																									//  函数返回：剩余未转完的步数
extern unsigned char GetStepMotorStatus(char StepMotor);                      //  获取指定步进电机状态
	                                                                                //  函数参数：StepMotor  指定步进电机
																																									//  函数返回：enumStepMotorFree:自由
																																									//            enumStepMotorBusy,忙（正在转动）
																																									//            enumSetStepMotorFail：调用失败（步进电机名不在指定范围）
enum StepMotorName    {enumStepMotor1=0,enumStepMotor2,enumStepMotor3}; 
enum StepMotorActName {enumStepMotorFree,enumStepMotorBusy,enumSetStepMotorOK,enumSetStepMotorFail}; 

#endif