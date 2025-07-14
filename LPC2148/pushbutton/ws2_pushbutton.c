#include<lpc21xx.h>
#include"timer.h"
#include"uart.h"
int main()
{
unsigned int count =0;
IODIR0|=0X6c;
IOCLR0|=0X6c;
uart0_config();
while(1)
{
	if(((IOPIN0>>4)&1)==0)
	{
		if(count>=9)
			count=0;
		count++;
		delay_ms(250);
		uart0_tx(count+48);
	
	
		if(count%4==1)
			IOSET0=0X020;
		else if (count%4==2)
			IOSET0=0X040;
		else if(count%4==3)
			IOSET0=0X4;
		else if(count%4==0)
		{
			IOSET0=0X08;
			delay_ms(500);
			IOCLR0=0X6c;
		}
	}
}
}
