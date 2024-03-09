#include "Delay.h"

/*
 * 函数名：Delayms
 * 描述  ：毫秒延时程序
 * 输入  ：延时i毫秒
 * 输出  ：无
 */
/*void Delayms(char i) 			
{	
	while(i--)
	{	
		int n=500;
		while (n--)
	    {
	        _nop_();
	        _nop_();
	        _nop_();
	        _nop_();
	        _nop_();
	    }
	}
} */

/*
 * 函数名：Delayus
 * 描述  ：微秒延时程序
 * 输入  ：延时i微秒
 * 输出  ：无
 */
void Delayus(int i)
{
	while (i--)
    {
        _nop_();
        _nop_();
    }
}

/*
 * 函数名：Delay
 * 描述  ：延时程序
 * 输入  ：无
 * 输出  ：无
 */
void Delay(char n)
{
	int x;
	while(n--)
	{
		x=5000;
		while(x--);
	}	
}