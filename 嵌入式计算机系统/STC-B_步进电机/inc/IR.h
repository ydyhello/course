/********************************** IR V3.5a 说明 ***********************************************

IR模块用于控制“STC-B学习板”上红外发送与接收控制，支持PWM、PPM红外编码协议的发送，PWM红外编码的接收，可用于制作红外遥控器、红外通信等。
IR模块提供1个驱动函数、5个API应用函数、1个红外接收事件（enumEventIrRxd：红外Ir上收到一个数据包）。
IR模块已不与串口通信（uart和uart2）冲突，可用与它们同时工作。（以前冲突）

  (1) void IrInit(unsigned char Protocol)：IR模块初始化函数。
	      函数参数：unsigned char Protocol，定义红外协议。
				          Protocol 暂仅提供取值：NEC_R05d（定义红外协议基本时间片时长 = 13.1*Protocol uS）
				函数返回值：无
				
	(2) char IrTxdSet(unsigned char *pt,unsigned char num)：以自由编码方式控制IR发送：可用于编写任意编码协议的红外发送，如各种电器红外遥控器等
	      函数参数：unsigned char *pt，指向待发送红外编码数据的首地址。编码规则如下：
				                  码1红外发送时长，码1红外发送停止时长        //单位：协议基本时间片的个数值，最大255。如当协议基本时间片为0.56mS时，
												，码2红外发送时长，码2红外发送停止时长        //如当协议基本时间片为0.56mS时，数值1代表0.56mS时长，3代表1.68mS时长，......
												  ......
												  ......											
												，码n红外发送时长，码n红外发送停止时长
				          unsigned char num，待发送红外编码数据的大小（字节数）
				函数返回值：enumIrTxOK：调用成功，即所设定的发送数据包请求已被系统sys正确接受，sys将尽硬件资源最大可能及时发送数据。	
						        enumIrTxFailure：调用失败（主要原因是：红外发送正忙（上一数据包未发完）、或红外正在接收一个数据包进行中
										（同IrPrint()函数返回值）
				补充说明：参照该格式定义和电器遥控器编码格式，可实现任何38KHz红外遥控器功能。
  	
	(3) char IrPrint(void *pt, unsigned char num)：//以NEC的PWM编码方式发送数据，可用于符合该函数发送格式的部分电器遥控器；与GetIrRxNum()、SetIrRxd()配合，可进行红外双机通信；等
	                                               //红外发送数据格式为：引导码：发（16*基本时间片），停（8*基本时间片）。0.56mS时：发9mS、停4.5mS
																								                       数据编码："0" -- 发（1*基本时间片），停（1*基本时间片）
																																			           "1" -- 发（1*基本时间片），停（3*基本时间片）
																																								 先发高位、后发低位
																																			 结束码：发（1*基本时间片），停（1*基本时间片）			 
	                                               //非阻塞函数，该函数从被调用到返回大约1uS左右时间,但所指定的数据经红外发送完毕则需要较长时间（每字节大约需要10mS量级时间）。
	                                               //IrPrint()函数用法完全类似与uart模块的Uart1Print()和Uart2Print()用法，（仅num参数为unsigned char，可参照使用）
	      函数参数：void *pt ：指定发送数据包位置（数据包不含引导码、结束码信息，仅待发送的有效数据） 
	                unsigned char num：发送数据包大小（字节数，不含引导码、结束码）						
        函数返回值：enumIrTxOK：调用成功，即所设定的发送数据包请求已被系统sys正确接受，sys将尽硬件资源最大可能及时发送数据。	
			              enumIrTxFailure：调用失败（主要原因是：红外发送正忙（上一数据包未发完）、或红外正在接收一个数据包进行中
                    （同IrTxdSet()函数返回值）
										
  (4) void SetIrRxd(void *RxdPt,unsigned char RxdNmax)：设置红外接收数据包存放位置、每个数据包最大字节数。
	                                                      收到一个数据包（至少1字节数据）时将产生numEventIrRxd事件。与它机IrPrint()函数配合，可实现红外数据通信
	     函数参数： void char *RxdPt：指定接收数据包存放区（首地址）
			            unsigned char RxdNmax：指定每个数据包接收最大字节数。当收到的数据大于Nmax后，将被丢弃。
		   函数返回值：无
			 
	(5) unsigned char GetIrRxNum(void)：获取收到的红外接收数据包实际字节数。与SetIrRxd()配合，可实现红外数据包接收。（它机应使用IrPrint()函数发送数据包）
	                                    返回取值 <= Ir接收缓冲区最大字节数（RxdNmax定义）。如果实际红外数据包字节数大于RxdNmax，收到的多余字节数将不被存储（也不进行计数） 
       函数参数： 无
		   函数返回值：红外接收数据包大小（字节数）。
			             当收到一个数据包的numEventIrRxd事件产生后，可用该函数获取红外接收数据包的大小（字节数）。其它时间访问，其值不确定
		
  (6)  char GetIrStatus(void)： 获取Ir状态
	 函数返回值：enumIrFree：红外空闲
				       enumIrBusy：红外正忙（正在发送数据包，或正在接收数据包）
							 
  (7) 红外接收事件enumEventIrRxd：红外Ir上收到一个符合格式的数据包（红外格式见IrPrint()函数说明）。
	                                如果接收数据包实际字节数大于Ir接收缓冲区最大字节数（RxdNmax定义），也在收到数据包结束符后产生enumEventIrRxd事件	
			 
补充说明：
  (1) 串口1、串口2波特率可独立设置，互不影响.
	(2) 串口1、串口2、红外通信可同时工作，互不影响
	(3) 串口1、串口2用法基本上完全一致，红外通信用法也基本相同。不同地方是：
          a，串口1固定在USB接口上，可用于与计算机通信；而串口2可初始化在EXT扩展、或485接口上（在485接口上时仅单工工作）；
		      b，红外通信速率固定不可变（大约相当于500～800 bps），通信时没有包头匹配功能。红外模块除通信功能外，还提供用于电器红外遥控的应用函数；
          c，红外通信模块仅为单工工作。不发送时自动进入接收状态；有数据发送时自动进入发送状态，但正在接收数据包过程中不会进入发送状态。
			
编写：徐成（电话18008400450）   2021年8月24日设计，2021年11月5日更新
*/

#ifndef _IR_H_
#define _IR_H_

extern void IrInit(unsigned char Protocol);                         //IR模块初始化。参数：定义红外IR基本周期
enum IrProtocalName {NEC_R05d=43};                                  //定义红外IR协议基本周期 = 43*1000/76 = 566 (uS)

extern char IrTxdSet(unsigned char *pt,unsigned char num);          //以自由编码方式控制IR发送：可用于编写任意编码协议的红外发送，如各种电器红外遥控器等
extern char IrPrint(void *pt, unsigned char num);                   //以NEC的PWM编码方式发送数据，可用于符合该函数发送格式的部分电器遥控器；与SetIrRxd()配合，可进行红外双机通信；等
extern void SetIrRxd(void *RxdPt,unsigned char RxdNmax);            //设置红外接收数据包存放位置、每个数据包最大字节数。收到一个数据包（至少1字节数据）时将产生numEventIrRxd事件。与它机IrPrint()函数配合，可实现红外数据通信
extern unsigned char GetIrRxNum(void);                              //当收到一个数据包的numEventIrRxd事件产生后，可用该函数获取红外接收数据包的大小（字节数）。其它时间访问，其值不确定
extern char GetIrStatus(void);                                      //获取Ir状态,enumIrFree:红外接口空闲, enumIrBusy,红外接口正忙                       

enum IrActName {enumIrFree=0                  //红外发送空闲
               ,enumIrBusy                    //        忙
               ,enumIrTxOK                    //        发送成功
               ,enumIrTxFailure};             //        发送失败（正忙）
#endif