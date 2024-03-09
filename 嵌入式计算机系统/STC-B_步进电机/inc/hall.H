/**********************************hall V2.0 说明 ************************************************************************
Hall模块用于获取“STC-B学习板”上hall传感器状态。hall模块共提供1个加载函数、1个应用函数，一个Hall事件：enumEventHall	
 （1）HallInit()：hall模块初始化函数
 （2）unsigned char GetHallAct(void)：获取hall事件。
		函数返回值：
			enumHallNull（无变化）
			enumHallGetClose（磁场接近）
			enumHallGetAway（磁场离开）	
        查询一次后,事件值变成enumEventHall	(仅查询一次有效)					
 （3）hall传感器事件：
      当Hall检测到有"磁场接近"或"磁场离开"事件时，将产生一个Hall传感器事件(enumEventHall).响应事件的用户处理函数由用户编写,	并有sys中提供的SetEventCallBack()函数设置事件响应函数.	

				 
编写：徐成（电话18008400450）   2021年3月15日设计  2021年3月26日修改
*/

#ifndef _hall_H_
#define _hall_H_

extern void HallInit(void);                    //加载hall模块
extern unsigned char GetHallAct(void);         //获取hall事件				 	     
enum HallActName {enumHallNull,enumHallGetClose,enumHallGetAway};  //返回值：enumHallNull——无，enumHallGetClose——下降沿（磁场接近），enumHallGetAway——上升沿（磁场离开）	

#endif                                             