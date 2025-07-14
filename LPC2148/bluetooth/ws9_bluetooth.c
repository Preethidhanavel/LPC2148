#include<LPC21XX.H>
#define LED 1<<5
void UART0_CONFIG(void);
void UART0_TX(unsigned char);
unsigned char UART0_RX(void);
void string(unsigned char *);

int main()
{
   unsigned char r;
  UART0_CONFIG();
  IODIR0|=LED;
  IOSET0=LED;
  while(1)
  {
   	r=UART0_RX();
    
	if(r=='1')
	{
	 IOCLR0=LED;
	 string("LED is ON");

	}
	else if (r=='0')
	{
	 IOSET0=LED;
	 string("LED IS OFF");
	}
	else if (r!='\r') 
	{
	 string("give 0 or 1");
	}
  }
}
void UART0_CONFIG(void)
{
 PINSEL0 |=0X05;
 U0LCR =0X83;
 U0DLL=97;
 U0DLM=0;
 U0LCR=0X03;

}
void UART0_TX(unsigned char d)
{
 U0THR=d;
 while(((U0LSR>>5)&1)==0);
}
unsigned char UART0_RX(void)
{
 
 while((U0LSR&1)==0);
 return U0RBR;
}


void string(unsigned char *s)
{
 while(*s)
 {
  	UART0_TX(*s++);
 }
}
