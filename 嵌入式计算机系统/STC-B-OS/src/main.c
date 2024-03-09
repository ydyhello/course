#include "global.h"
#include "stack.h"
#include "xstack.h"
#include "scheduler.h"
#include "bit_ops.h"
#include "timer0_isr.h"
#include "syscall.h"
#include "semaphore.h"
#include "events.h"
#include "seg_led.h"
#include "button.h"
#include "usbcom.h"
#include "rs485.h"
#include "random.h"

#define TIMESLICE_MS	1
#define T12RL		(65536 - MAIN_Fosc*TIMESLICE_MS/12/1000) 

//game_pro
XDATA u8 playerid=0;

XDATA u8 player1_num1=0;
XDATA u8 player1_num2=0;
XDATA u8 player2_num1=0;
XDATA u8 player2_num2=0;

XDATA u8 pos1=1;
XDATA u8 pos2=1;

XDATA u8 num1=0;
XDATA u8 num2=0;

XDATA u8 flag1=0;//开关
XDATA u8 flag2=0;

XDATA u8 count=0;

XDATA u8 Case;


void testproc(u16 param) large reentrant
{
	while(1)
	{
		if((timer0_cnt>>5) & BIT(param))
		{
			SETBIT(led_display_content, param);
		}
		else
		{
			CLEARBIT(led_display_content, param);
		}
	}
}

void testproc2(u16 param) large reentrant
{
	while(1)
	{
		proc_sleep(param);
		led_display_content = ~led_display_content;
	}
}

void testproc3(u16 param) large reentrant
{
	while(1)
	{
		proc_sleep(param);
		led_display_content ^= 0x0f;
	}
}

void testproc4() large reentrant
{
	while(1)
	{
		proc_sleep(500);
		sem_post(0);
		led_display_content |= 0x80;
		sem_wait(0);
		led_display_content &= ~0x80;
	}
}

void testproc5() large reentrant
{
	sem_init(0,0);
	while(1)
	{
		sem_wait(0);
		led_display_content |= 0x40;
		proc_sleep(500);
		sem_post(0);
		led_display_content &= ~0x40;
	}
}

void testproc6(u16 param) large reentrant
{
	while(1)
	{
		proc_wait_evts(EVT_BTN1_DN);//k1按下
		seg_set_str("HELLO   ");
		usbcom_write("hello \0",0);
		proc_wait_evts(EVT_NAV_R);//导航右
		seg_set_str("WORLD   ");
		usbcom_write("world\r\n\0",0);
		proc_wait_evts(EVT_UART1_RECV);//usb串口接收
		seg_set_str(usbcom_buf);
	}
}

void testproc7(u16 param) large reentrant
{
	while(1)
	{
		proc_wait_evts(EVT_UART2_RECV | EVT_BTN1_DN);//RS485 串口接收|K1按下
		if(MY_EVENTS & EVT_BTN1_DN)
		{
			*((u32 *)rs485_buf) = rand32();
			rs485_write(rs485_buf, 4);
			seg_set_number(*((u32 *)rs485_buf));
		}
		else
		{
			seg_set_number(*((u32 *)rs485_buf));
		}
	}
}
void testRR0(u16 param)
{
	while(1){
		seg_set_number(00000000);
	}
}
void testRR1(u16 param)
{
	while(1){
		seg_set_number(11111111);
	}
}
void testRR2(u16 param)
{
	while(1){
		seg_set_number(22222222);
	}
}

void game_start()
{
	while(1){

		proc_wait_evts(EVT_BTN3_DN | EVT_UART2_RECV);
		if(MY_EVENTS & EVT_BTN3_DN)
		{
			led_display_content=0xff;
		}
		proc_wait_evts(EVT_BTN1_DN | EVT_BTN2_DN | EVT_UART2_RECV);
		if(MY_EVENTS & EVT_BTN1_DN)
		{
			playerid=1;//发送方
			led_display_content=0;
		}
		if(MY_EVENTS & EVT_BTN2_DN)
		{
			playerid=2;//接收方
			led_display_content=0;
		}
	}

}
void game_pro()
{
	while(1){
		
		proc_wait_evts(
			EVT_NAV_U | EVT_NAV_D |
			EVT_NAV_L | EVT_NAV_R |
			EVT_NAV_PUSH| EVT_BTN3_DN |
			EVT_BTN2_DN | EVT_USER1
		);
		if(MY_EVENTS & EVT_NAV_L){
			if(playerid==1 && flag1==0) pos1=2;
			if(playerid==2 && flag2==0) pos2=2;
		}
		if(MY_EVENTS & EVT_NAV_R){
			if(playerid==1 && flag1==0) pos1=1;
			if(playerid==2 && flag2==0) pos2=1;
		}

		//发送方
		if(playerid==1 && flag1==0){
			if(pos1==1){
				if(MY_EVENTS & EVT_NAV_U){
					if(player1_num1==9){
						player1_num1=0;
					}else{
						player1_num1++;
					}
				}
				if(MY_EVENTS & EVT_NAV_D){
					if(player1_num1==0){
						player1_num1=9;
					}else{
						player1_num1--;
					}
				}
			}
			if(pos1==2){
				if(MY_EVENTS & EVT_NAV_U){
					if(player1_num2==9){
						player1_num2=0;
					}else{
						player1_num2++;
					}
				}
				if(MY_EVENTS & EVT_NAV_D){
					if(player1_num2==0){
						player1_num2=9;
					}else{
						player1_num2--;
					}
				}
				
			}
		}
		
		//接收方
		if(playerid==2 && flag2==0){
			if(pos2==1){
				if(MY_EVENTS & EVT_NAV_U){
					if(player2_num1==9){
						player2_num1=0;
					}else{
						player2_num1++;
					}
				}
				if(MY_EVENTS & EVT_NAV_D){
					if(player2_num1==0){
						player2_num1=9;
					}else{
						player2_num1--;
					}
				}
			}
			if(pos2==2){
				if(MY_EVENTS & EVT_NAV_U){
					if(player2_num2==9){
						player2_num2=0;
					}else{
						player2_num2++;
					}
				}
				if(MY_EVENTS & EVT_NAV_D){
					if(player2_num2==0){
						player2_num2=9;
					}else{
						player2_num2--;
					}
				}
				
			}
		}
		//确定被猜的数字
		if(MY_EVENTS & EVT_NAV_PUSH){
			if(playerid==2){
				num2=player2_num2*10+player2_num1;
				flag2=1;
				rs485_buf[0] = num2;
				rs485_write(rs485_buf, 1);

			}
			if(playerid==1){
				num1=player1_num2*10+player1_num1;
				
				if(flag1==0) count++;
				//flag1=1;
				led_display_content=count;
				if(rs485_buf[0]==num1){
					//led_display_content=rs485_buf[0];
					flag1=1;
				}
				if(rs485_buf[0]<num1)
					Case=1;
				if(rs485_buf[0]>num1)
					Case=2;
				
				
			}
		}
	}
}
void game_ui()
{
	while(1){
		
		if(playerid==1 && (rs485_buf[0]!=num1 || (rs485_buf[0]==0 && num1==0))){
			//led_display_content=count;
			seg_display_content[0]=seg_decoder['n'];
			seg_display_content[1]=seg_decoder['o'];
			seg_display_content[2]=seg_decoder[1];
			
			seg_display_content[3]=seg_decoder[' '];
			//seg_display_content[4]=seg_decoder[' '];
			seg_display_content[5]=seg_decoder[' '];
			if(Case==1)
				seg_display_content[4]=seg_decoder['u'];
			if(Case==2)
				seg_display_content[4]=seg_decoder['N'];

			if(pos1==1){
				seg_display_content[6]=seg_decoder[player1_num2];
				seg_display_content[7]=seg_decoder[player1_num1]|0x80;
			}
			if(pos1==2){
				seg_display_content[6]=seg_decoder[player1_num2]|0x80;
				seg_display_content[7]=seg_decoder[player1_num1];
			}
		}
		if(playerid==2){
			seg_display_content[0]=seg_decoder['n'];
			seg_display_content[1]=seg_decoder['o'];
			seg_display_content[2]=seg_decoder[2];
			
			seg_display_content[3]=seg_decoder[' '];
			seg_display_content[4]=seg_decoder[' '];
			seg_display_content[5]=seg_decoder[' '];

			if(pos2==1){
				seg_display_content[6]=seg_decoder[player2_num2];
				seg_display_content[7]=seg_decoder[player2_num1]|0x80;
			}
			if(pos2==2){
				seg_display_content[6]=seg_decoder[player2_num2]|0x80;
				seg_display_content[7]=seg_decoder[player2_num1];
			}

		}
		if(playerid==1 && rs485_buf[0]==num1 && rs485_buf[0]!=0 && num1!=0){
			seg_display_content[0]=seg_decoder['G'];
			seg_display_content[1]=seg_decoder['U'];
			seg_display_content[2]=seg_decoder['E'];
			seg_display_content[3]=seg_decoder['S'];
			seg_display_content[4]=seg_decoder['S'];
			seg_display_content[5]=seg_decoder[' '];
			if(count<10){
				seg_display_content[6]=seg_decoder[' '];
				seg_display_content[7]=seg_decoder[count];
			}else{
				seg_display_content[6]=seg_decoder[count/10];
				seg_display_content[7]=seg_decoder[count%10];
			}
		}


	}
}

main()
{
	//initialize kernel stack and xstack pointer
	SP = kernel_stack;
	setxbp(kernel_xstack + KERNEL_XSTACKSIZE);
	
	//set process stacks and swap stacks owner
	process_stack[0][PROCESS_STACKSIZE-1] = 0;
	process_stack[1][PROCESS_STACKSIZE-1] = 1;
	process_stack[2][PROCESS_STACKSIZE-1] = 2;
	process_stack[3][PROCESS_STACKSIZE-1] = 3;
	process_stack[4][PROCESS_STACKSIZE-1] = 4;
	process_stack_swap[0][PROCESS_STACKSIZE-1] = 5;
	process_stack_swap[1][PROCESS_STACKSIZE-1] = 6;
	process_stack_swap[2][PROCESS_STACKSIZE-1] = 7;
	
	//initialize LED pins
	P0M1 &= 0x00;
	P0M0 |= 0xff;
	P2M1 &= 0xf0;
	P2M0 |= 0x0f;
	//select LED, set all off
	P23 = 1;
	P0 = 0;

	//initialize buttons
	buttons_init();
	
	//initialize serial ports
	usbcom_init(115200);
	rs485_init(115200);
		
	//start process
	//优先级
	
	start_process((u16)testRR0,0,0,2);
	start_process((u16)testRR1,1,0,1);
	start_process((u16)testRR2,2,0,3);
	
	//RR
	/*
	start_process((u16)testRR0,0,0);
	start_process((u16)testRR1,1,0);
	start_process((u16)testRR2,2,0);
	*/
	
	//猜数
	/*
	seg_set_str("GUESS PL");
	start_process((u16)game_start,0,0,1);
	start_process((u16)game_ui,1,0,2);
	start_process((u16)game_pro,2,0,3);
	*/
	
	
	
	

			
		
	//initialize PCA2 interrupt (as syscall interrupt)
	//clear CCF2
	CCON &= ~0x04;
	//disable PCA2 module and set ECCF2
	CCAPM2 = 1;
	//low priority interrupt
	PPCA = 0;
	
	//start main timer
	TR0 = 0;														//stop timer
	TMOD &= 0xF0;												//timer mode, 16b autoreload
	AUXR &= 0x7F;												//12T mode
	TL0 = T12RL & 0xff;							//set reload value
	TH0 = (T12RL & 0xff00) >> 8;
	ET0 = EA = 1;												//set interrupt enable
	PT0 = 0;														//set priority to low
	TR0 = 1;														//start timer
	
	//spin
	while(1);
}