#include<lpc21xx.h>
#include "uart0.h"
#include "lcd1.h"
int main()
{
		char d;
		lcd_init();
		UART0_CONFIG();
		IODIR0=1<<17;
	while(1)
	{
		d=UART0_RX();
		if(d=='1'){
			IOSET0=1<<17;
			string("LIGHT ON\r\n");
			lcd_command(0X80);
			lcd_string("LIGHT ON ");
		}
		else if (d=='0'){
			IOCLR0=1<<17;
			string("LIGHT OFF\r\n");
			lcd_command(0X80);
			lcd_string("LIGHT OFF");
		}
		else{
			lcd_command(0X01);
			lcd_string("ENTER 0/1:");
			string("ENTER 0/1:\r\n");
		}
	}
	}