#include "Delay.h"

/*
 * ��������Delayms
 * ����  ��������ʱ����
 * ����  ����ʱi����
 * ���  ����
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
 * ��������Delayus
 * ����  ��΢����ʱ����
 * ����  ����ʱi΢��
 * ���  ����
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
 * ��������Delay
 * ����  ����ʱ����
 * ����  ����
 * ���  ����
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