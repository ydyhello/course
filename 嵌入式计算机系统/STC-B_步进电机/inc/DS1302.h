/**********************************DS1302 V1.1 说明 ************************************************************************
DS1302模块用于控制“STC-B学习板”上DS1302芯片操作。
DS1302提供RTC（实时时钟）和NVM（非易失存储器）功能（断电后，RTC和NVM是依靠纽扣电池BAT维持工作的）。其中：
    RTC提供：年、月、日、星期、时、分、秒功能
		NVM提供：31 Bytes非易失存储器功能(地址为：0～30）。其中，地址为30的单元被DS1302Init()函数用于检测DS1302是否掉电，用户不能使用)
DS1302模块共提供1个驱动函数、4个应用函数：

  (1)  void DS1302Init(struct_DS1302_RTC time)：DS1302驱动函数。使用DS1302，需用该函数初始化和驱动一次
	      函数参数：结构struct_DS1302_RTC time
				          如果DS1302掉电（初始化时检测RTC数据失效），则以参数time定义的时间初始化RTC
				函数返回值：无					
	
  (2)  struct_DS1302_RTC RTC_Read(void)：读取DS1302内部实时时钟RTC内容
        函数参数：无
			  函数返回值：结构struct_DS1302（见结构struct_DS1302定义）
										
	(3)  void RTC_Write(struct_DS1302_RTC time) ：写DS1302内部实时时钟RTC内容
	      函数参数：结构struct_DS1302 time（见结构struct_DS1302定义）
			  函数返回值：无	
										
	(4)  unsigned char NVM_Read(unsigned char NVM_addr):	读取NVM一个指定地址内容
	      函数参数：  
				      NVM_addr：指定非易失存储单元地址，有效值0～30（共31个单元）
	      函数返回值：当函数参数正常时，返回NVM中对应单元的存储数值（1Byte）
				            当函数参数错误时，返回enumDS1302_error
				
	(5)  unsigned char NVM_Write(unsigned char NVM_addr, unsigned char NVM_data)：向NVM一个指定地址写入新值
	      函数参数：  
				      NVM_addr：指定非易失存储单元地址，有效值：0～30（共31个单元。其中，第30单元被DS1302Init()函数用于检测DS1302是否掉电，用户不能使用)）
	            NVM_data：待写入NVM单元的新值（1Byte）
			  函数返回值：当函数参数正常时，返回enumDS1302_OK		
				            当函数参数错误时，返回enumDS1302_error	

结构struct_DS1302_RTC定义：（参见DS1302Z数据手册）
typedef struct 
  { unsigned char second;		    //秒（BCD码，以下均为BCD码）
    unsigned char minute;	      //分
    unsigned char hour;         //时
    unsigned char day;		      //日
    unsigned char month;		    //月
    unsigned char week;		      //星期
    unsigned char year;		  		//年
  } struct_DS1302_RTC;    

关于DS1302内部非易失性存储器补充说明：
      DS1302提供的非易失性存储器为低功耗RAM结构，靠纽扣电池保持掉电后其存储内容不变。
			与M24C01区别：容量小（仅31字节），但无”写“寿命问题，且写周期很短（可忽略：即两次写操作之间无需等待）；
			读、写DS1302内部NVM每一个字节均需要花费一定操作时间（数十uS）；
			仅在需要时使用以上读或写函数读写需要的特定字节内容，应避免对其进行无效、大量、重复操作！
			
编写：徐成（电话18008400450）   2021年8月5日设计，2021年8月15日改进
*/

#ifndef _DS1302_H_
#define _DS1302_H_

typedef struct 
  { unsigned char second;		    //秒（BCD码，以下均为BCD码）
    unsigned char minute;	      //分
    unsigned char hour;         //时
    unsigned char day;		      //日
    unsigned char month;		    //月
    unsigned char week;		      //星期
    unsigned char year;		  		//年
  } struct_DS1302_RTC;    

extern void DS1302Init(struct_DS1302_RTC time);                 //DS1302初始化
extern struct_DS1302_RTC RTC_Read(void);                                         //读RTC（读RTC时钟内容）
extern void RTC_Write(struct_DS1302_RTC time);                                   //写RTC（校对RTC时钟）
extern unsigned char NVM_Read(unsigned char NVM_addr);                           //读NVM（读DS1302中的非易失存储单元内容）
extern unsigned char NVM_Write(unsigned char NVM_addr, unsigned char NVM_data);  //写NVM（写DS1302中的非易失存储单元）

enum DS1302name {enumDS1302_OK,enumDS1302_error};

#endif