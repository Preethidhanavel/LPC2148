#include<lpc21xx.h>
#include"delay.h"
#define led0 1<<0
#define led1 1<<1
#define led2 1<<2
#define led3 1<<3
#define slot_en 1<<5
#define eint0 14
#define eint1 15
#define eint2 16
#define eint3 17
 
 void non_vect_isr(void) __irq
		 {
		 if((VICIRQStatus>>eint0)&1){
		 EXTINT=0x01;
		 IOSET0=led0;
		 delay_ms(500);
		 IOCLR0=led0;  
		 delay_ms(500);
		 }
		 if((VICIRQStatus>>eint1)&1){
		 EXTINT=0X02;
		 IOSET0=led1;
		 delay_ms(500);
		 IOCLR0=led1;
		 delay_ms(500);
		 }
		if((VICIRQStatus>>eint2)&1){
		 EXTINT=0X04;
		 IOSET0=led2;
		 delay_ms(500);
		 IOCLR0=led2;
		 delay_ms(500);
		 }
		if((VICIRQStatus>>eint3)&1){
		 EXTINT=0X08;
		 IOSET0=led3;
		 delay_ms(500);
		 IOCLR0=led3;
		 delay_ms(500);
		 }
		 VICVectAddr=0;
 		 }
 int main()
 {
		 unsigned int count=0;
		 PINSEL0|=(1<<29)|(unsigned)1<<31; 
		// PINSEL0|=(1<<29)|80000000;
		 PINSEL1|=(0x01)|(1<<9)|(1<<8);
		 IODIR0|=led0|led1|led2|led3;
		 
		 
		 VICIntSelect=0;		  
		 VICDefVectAddr= (int) non_vect_isr;
		 
		 EXTMODE=0X0f;
		 EXTPOLAR=0X00;
		 
		 VICIntEnable=(1<<eint0)|(1<<eint1)|(1<<eint2)|(1<<eint3);
		 
		 while(1)
		 count++;
 }
