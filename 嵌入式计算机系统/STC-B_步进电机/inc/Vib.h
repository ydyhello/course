/**********************************振动传感器SV V2.0 说明 ************************************************************************
SV用于获取"STC-B学习板"上Vib传感器状态.提供一个模块加载函数和一个应用函数,一个Vib事件enumEventVib：
  (1)  VibInit()：振动传感器Vib模块初始化函数；
  (2)  char GetVibAct()：获取Vib事件。
			  函数返回值：返回当前Vib传感器事件，返回值：enumVibNull——无，enumVibQuake——发生过振动
        查询一次后,事件值变成	enumVibNull	(仅查询一次有效)									
  (3)  Vib传感器事件enumEventVib：
          当Vib检测到有”振动“事件时，将产生一个”振动事件“，响应事件的用户处理函数由用户编写,
				并有sys中提供的SetEventCallBack()函数设置.	 
				 
编写：徐成（电话18008400450）   2021年3月5日设计，2021年3月26日更新
*/

#ifndef _Vib_H_
#define _Vib_H_

extern void VibInit();
extern unsigned char GetVibAct(void) reentrant;        //获取Vib事件							 	                    
enum VibActName {enumVibNull,enumVibQuake};            //返回值：enuVibNull——无，enuVibQuake——发生过振动

#endif