#include<lpc21xx.h>
#include"i2c_config.c"
#include"eeprom.c"
#define led0 1<<19
#define led1 1<<18
#include<string.h>
#include"lcd.h"

	
int main()
{

			u8 ch;
			lcd_init();
     	i2c_init();
	   i2c_eeprom_write(0X50,0X00,'A');
	   ch=i2c_eeprom_read(0X50,0X00);
	   delay_ms(100);
			if(ch=='A'){
				lcd_data(ch);
			}
			else{
				lcd_string("failed");
				delay_ms(500);
			} 
   
		 	while(1); 
	   	 }
		 
