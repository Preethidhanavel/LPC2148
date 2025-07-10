#include<lpc21xx.h>
void delay(unsigned int n)
{
	int i,j;
	for(i=0;i<n;i++)
	for(j=0;j<12000;j++);
}

int main()
{
IODIR0|=0XFF;
while(1)
{
	IOSET0|=0Xff;
	delay(500);
	IOCLR0|=0Xff;
	delay(500);
}
}
