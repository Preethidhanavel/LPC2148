#include<lpc21xx.h>
#include"delay.h"
#define led0 1<<0
#define eint0 14
#define slot_en 1<<5

void FIQ_Handler(void) __irq
{
		EXTINT=0X01;
		IOSET0=led0;
		delay_ms(500);
		IOCLR0=led0;
		delay_ms(500);
		VICVectAddr=0;
} 
int main()
{
		unsigned int count=0;
		//PINSEL0=0x01;
		PINSEL1=0x01;
		
		IODIR0|=led0;
	//	IOSET0=led0;
		
		VICIntSelect=1<<14;
		
		VICVectCntl0= slot_en|eint0;
		VICVectAddr0=(int)FIQ_Handler;

		EXTMODE=0x01;
		EXTPOLAR=0x00;

		VICIntEnable=1<<eint0;

		while(1){
		count++;
		}
}
