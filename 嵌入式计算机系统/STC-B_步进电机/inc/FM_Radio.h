/********************************** FMRadio V1.1 说明 ************************************************************************
FMRadio模块用于控制“STC-B学习板”上FM收音机操作。
FMRadio模块模块共提供1个初始化函数、2个应用函数								
	
	(1)  void FMRadioInit(struct_FMRadio FMRadio); //收音机模块初始化函数。该函数只执行一次。初始化后，以下各应用函数方可正确使用。
				函数参数：FMRadio (见结构struct_FMRadio定义)
				函数返回值：无
				
	(2)  void SetFMRadio(struct_FMRadio FMRadio);  //设置FM收音机控制参数。
	      函数参数：FMRadio (见结构struct_FMRadio定义)
				函数返回值：无
				
 （3） struct_FMRadio GetFMRadio(void);          //获取当前FM收音机参数。
        函数参数：无
				函数返回值：返回FM控制模型数据(见结构struct_FMRadio定义)

结构struct_FMRadio定义：	
    	typedef struct          //FM收音机控制模型
					{ unsigned int frequency;	  // FM收音机收音频率   (frequency参数范围：887～1080。单位：0.1MHz，如887代表88.7MHz）。错误参数或参数与当前值无变化，将忽略
						unsigned char volume;	    // FM收音机音量       (volume参数范围：0～15。0为最小音量）。错误参数或参数与当前值无变化，将忽略
						unsigned char GP1;        // FM指示灯1。  =0 输出低，GP1亮；  !=0 输出高，GP1灭
						unsigned char GP2;        // FM指示灯2。  =0 输出低，GP1亮；  !=0 输出高，GP1灭	  
						unsigned char GP3;        // FM指示灯3。  =0 输出低，GP1亮；  !=0 输出高，GP1灭		  		  		
					} struct_FMRadio;

编程注意事项：
       1，本版本暂未输出调谐、自动搜索、电台信号等控制和状态信息，因此，暂不能完成自动搜索电台等收音机功能。
			 2，FM收音机借用USB转串口模块（CH340）时钟（硬件设计原因），需要连接USB接口（供电、通信）到USB主设备，CH340方有时钟产生和输出。
			    故STC-B学习板的USB接口需要连接到一个USB主设备方可收音，仅USB供电时（如USB充电宝、或计算机休眠时的USB接口），FM收音机可能不工作
			 
编写：徐成（电话18008400450）   2021年8月10日设计，2021年8月16日改进
*/

#ifndef _FM_Radio_H_
#define _FM_Radio_H_

typedef struct           //FM收音机控制模型
  { unsigned int frequency;		    // FM收音机收音频率   (frequency参数范围：887～1080。单位：0.1MHz，如887代表88.7MHz）。错误参数或参数与当前值无变化，将忽略
    unsigned char volume;	        // FM收音机音量       (volume参数范围：0～15。0为最小音量）。错误参数或参数与当前值无变化，将忽略
    unsigned char GP1;            // FM指示灯1。  =0 输出低，GP1亮；  !=0 输出高，GP1灭
    unsigned char GP2;            // FM指示灯2。  =0 输出低，GP1亮；  !=0 输出高，GP1灭	  
    unsigned char GP3;            // FM指示灯3。  =0 输出低，GP1亮；  !=0 输出高，GP1灭		  		  		
  } struct_FMRadio;
	
extern void FMRadioInit(struct_FMRadio FMRadio); //收音机模块初始化函数。输入FM控制模型数据，无返回值。初始化后，以下各应用函数方可正确使用
extern void SetFMRadio(struct_FMRadio FMRadio);  //设置FM收音机控制参数。输入FM控制模型数据，无返回值。
extern struct_FMRadio GetFMRadio(void);          //获取当前FM收音机参数。无输入参数，返回FM控制模型数据

#endif