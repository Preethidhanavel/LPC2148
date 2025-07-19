#include<lpc21xx.h>
#include"i2c_config.c"
#include"eeprom.c"
#define led0 1<<19
#define led1 1<<18
#include<string.h>
#include"lcd.h"

	
int main()
{

			lcd_init();
     	i2c_init();
			lcd_string("I2C");
			i2c_eeprom_write(0X50,0X0000,'A');
			lcd_command(0X01);
			lcd_string("Transfer over");
			delay_ms(500);
			
		 	while(1); 
	   	 }
		 
