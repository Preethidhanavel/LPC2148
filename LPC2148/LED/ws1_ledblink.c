#include<lpc21xx.h>
#include"timer.h"
int main()
{
	IODIR0=0XFF;   //making port0 0 to 7 pin as output
	IOCLR0=0XFF;   //making the pin value as zero initially
while(1)
{
	IOSET0=0X01;
	delay_ms(250);
	IOCLR0=0X01;
	
	IOSET0=0X02;
	delay_ms(250);
	IOCLR0=0X02;
	
	IOSET0=0X04;
	delay_ms(250);
	IOCLR0=0X04;
	
	IOSET0=0X08;
	delay_ms(250);
	IOCLR0=0X08;
	
	IOSET0=0X010;
	delay_ms(250);
	IOCLR0=0X010;
	
	IOSET0=0X020;
	delay_ms(250);
	IOCLR0=0X020;
	
	IOSET0=0X040;
	delay_ms(250);
	IOCLR0=0X040;
	
	IOSET0=0X080;
	delay_ms(250);
	IOCLR0=0X080;
}
	
	}
	