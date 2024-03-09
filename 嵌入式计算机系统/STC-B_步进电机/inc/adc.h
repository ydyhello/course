/**********************************ADC V3.5a 说明 ************************************************************************
ADC模块用于“STC-B学习板”上与ADC相关电路:温度Rt、光照Rop、导航按键Nav、扩展接口EXT上的ADC转换。提供ADC模块初始化函数、2个应用函数,2个事件：
  (1)  AdcInit(char ADCsel)：ADC模块初始化函数；
	  函数参数: ADCsel选择扩展接口EXT是否用作ADC功能，取值：
      ADCincEXT   :  含对扩展接口EXT设置ADC功能（EXT上P1.0、P1.1不可作数字IO功能使用）
      ADCexpEXT   :  不含对扩展接口EXT设置ADC功能（EXT上P1.0、P1.1可作数字IO功能使用） 		
  (2)  struct_ADC GetADC()：获取ADC值。
		函数参数：无
		函数返回值：返回数据结构 struct_ADC。
		数据结构定义：
		typedef struct                //ADC转换结果
			{ unsigned int EXT_P10;		      // 扩展接口EXT上P1.0脚ADC（10bit）
				unsigned int EXT_P11;	        // 扩展接口EXT上P1.1脚ADC（10bit）
				unsigned int Rt;              // 热敏电阻上ADC（10bit）
				unsigned int Rop;             // 光敏电阻上ADC（10bit）
				unsigned int Nav;             // 导航按键上ADC（10bit）	  		
			} struct_ADC;

说明1：每个数字表示  VCC/1024（单位：V），其中VCC为电路板供电电压（USB接口一般为5V左右） 
说明2：对于导航按键，下面GetAdcNavAct函数输出消抖后导航按键事件和状态，更方便使用；

   (3)  char GetAdcNavAct(char Nav_button)：获取导航按键（包含K3）状态
	 函数参数：Nav_button：指定要获取状态的导航按键。取值：
					enumAdcNavKey3（K3键）,   
					enumAdcNavKeyRight（右按）,
					enumAdcNavKeyDown（下按）,
					enumAdcNavKeyCenter（中心按）
          enumAdcNavKeyLeft（左按）,
          enumAdcNavKeyUp（上按）.
（当参数取值不在有效范围，函数将返回enumKeyFail）
	 函数返回值：返回当前按键事件，返回值：（同Key模块GetAdcKeyAct（）返回值）
					enumKeyNull（无按键动作）,
					enumKeyPress（按下）,
					enumKeyRelease（抬起）,
					enumKeyFail（失败） 
	 返回值是经过多次检测按键实时状态和统计检测结果后（软件消抖）的有效事件。
     每个按键查询一次后,事件值变成	enumKeyNull	(仅查询一次有效)				
  (4)  导航按键事件“：enumEventNav
      当导航按键5个方向或按键K3有任意”按下“或”抬起“动作时，将产生一个”导航按键事件“enumEventNav。响应导航按键事件的用户回调函数由用户编写,并由sys提供的SetEventCallBack()函数设置响应函数.	 
  (5)  扩展接口EXT上P1.0、P1.1两个端口有新的AD值事件“：enumEventXADC
     当ADC模块对P1.0、P1.1进行ADC转换，获得了它们新的ADC结果时，将产生enumEventXADC事件，通知用户进行处理。响应enumEventXADC事件的用户回调函数由用户编写,并有sys提供的SetEventCallBack()函数设置响应函数.
     ADC模块对P1.0、P1.1进行ADC转换速度为3mS，也即每3mS或每秒钟333次转换.
  (6)  补充说明： 
	 a：对EXT上P1.0、P1.0的转换速度为3mS，也即每秒钟提供333次转换结果，提供了有新转换结果事件：enumEventXADC，方便用户处理
b：对于Rt、Rop，转换速度为9mS，也即每秒钟提供111次转换结果。没有提供相应“事件”，用户随时用函数GetAdcResult()查询和使用
	 c：对导航按键进行了软件消抖处理，最快可支持导航按键每秒12次操作速度，提供了导航按键发生了操作事件：enumEventNav
	 d：由于导航按键与K3键共用了单片机同一个端口（P1.7），启用ADC模块后，P1.7口IO功能失效，只能用GetAdcNavAct(char Nav_button)函数获取K3按键的事件或状态。
	 e: STC-B板上Rt型号为：10K/3950 NTC热敏电阻，光敏电阻Rop型号为：GL5516. 它们的ADC值与温度、光照强度关系请参阅它们的数据手册与STC-B电路图进行换算。


编写：徐成（电话18008400450）   2021年3月25日设计,2021年8月30日修改
*/

#ifndef _adc_H_
#define _adc_H_

#define ADCincEXT 0x9B
#define ADCexpEXT 0x98

typedef struct                //ADC转换结果
  { unsigned int EXT_P10;		      // 扩展接口EXT上P1.0脚ADC
    unsigned int EXT_P11;	        // 扩展接口EXT上P1.1脚ADC
    unsigned int Rt;              // 热敏电阻上ADC
    unsigned int Rop;             // 光敏电阻上ADC
    unsigned int Nav;             // 导航按键上ADC  		  		
  } struct_ADC;

extern void AdcInit(char ADCsel);

extern struct_ADC GetADC();
extern unsigned char GetAdcNavAct(char Nav_button);                  //获取导航按键5个方向（右、下、中心、左、上）操作，以及按键K3操作的事件
																																		 //返回值：enuKeyNull（无按键动作）,enuKeyPress（按下）,enuKeyRelease（抬起）,enuKeyFail（失败） 
enum KN_name  {enumAdcNavKey3=0,      //导航按键：按键K3
               enumAdcNavKeyRight,    //导航按键：右
               enumAdcNavKeyDown,     //导航按键：下
               enumAdcNavKeyCenter,   //导航按键：中心
               enumAdcNavKeyLeft,     //导航按键：左
               enumAdcNavKeyUp};      //导航按键：上
#endif