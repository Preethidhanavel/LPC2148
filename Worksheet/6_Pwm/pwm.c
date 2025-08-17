#include<lpc21xx.h>

unsigned int long x;
int main()
{
	x=300;
	PINSEL1=0X400;// P0.21 as PWM 
	PWMMR0=0Xf;   //period register
	PWMMR5=0X0;		//duty cycle regiter
	PWMMCR=0X3;		//PWMMR0=0 if PWMMR=PWMTC and set interrupt flag
	PWMPCR=0X2000; // PWM5 output enabled
	PWMTCR=0X9;    //PWM and counter enabled
	PWMLER=0X20;   // Latch enable register enabled
	while(1)
	{
	while(PWMIR!=0X1);  // wait here until interrupt raise
	PWMIR=0X01;  // clear interrupt register
	PWMMR5=x;  // update duty cycle
	if(x>=PWMMR0)
		x=0;
	x=x+300;  // increasing duty cycle
	PWMLER=0X20;
	}
}
