#include<lpc21xx.h>
#include"pin_gpio.h"
#include"port_gpio.h"
int main()
{
	int n,m;
	pinWrite(2,1);
	pinWrite(4,1);
	pinWrite(120,0);
	pinWrite(124,1);
	portWrite(0,1);
	portWrite(2,0);
	portWrite(12,1);
	portWrite(13,0);
	n=pinRead(2);
	m=pinRead(20);
	

}
