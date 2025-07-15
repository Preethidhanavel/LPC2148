#include <lpc21xx.h>
#include "lcd1.h"
#include"uart0.h"
int adc_value=0;
float voltage=0.0;

int ADC_Conversion()
{
 int ab;                                //Variable to store ADC value 
 ADCR  = ADCR|0x01000000;               //Start conversion                    
 while((ADDR&0x80000000)!=0x80000000);  // Wait here till conversion is over    
 ab    = ADDR;             // Extracting the result               
 ab    = (ab>>6)&0X3ff;                       // Shift 6 bits right                  
 voltage = ((3.3*ab)/1023)*100;
 return ab;
}

 int ADC_Check()
{
 ADCR = (1 << 2) | (6 << 8) | (1 << 21);            // PDN=1,CLKDIV=6,channel=AD0.2         
 lcd_command(0x80);
 lcd_string("TEMP:");
 adc_value=ADC_Conversion();            /* Get the result of conversion         */
 lcd_command(0x85);                    /* 2nd row, 5th location                */
 lcd_int(adc_value);
 delay_ms(100);
 return adc_value;
}


int main()
{
		int d;
    PINSEL1 |= 0x04000000;  // Set P0.29 as ADC input (AD0.2)
    IODIR0=0X0c;
		IOCLR0=0X0c;
    lcd_init();  
		UART0_CONFIG();
    while (1)
    {
	
        d=ADC_Check();  // Continuously check ADC value and update LCD
				lcd_command(0X80);
				lcd_string("TEMP:");
				lcd_int(d);
				delay_ms(500);
		if(d>27)
		{
			IOSET0=0X04;
			delay_ms(500);
			string("Temperature exceeded threshold value\r\n");
		}
		else
		{
			IOCLR0=0X04;
			//delay_ms(500);
			string("Temperature is within the limit\r\n");
		}
		
		if(((IOPIN0>>14)&1)==0)
		{
			IOSET0=0X08;
			//delay_ms(500);
			lcd_command(0Xc0);
			string("MOTION DETECTED\r\n");
			lcd_string("MOTION DETECT");
		}
		else
		{	
			IOCLR0=0X08;
			//delay_ms(500);
			lcd_command(0Xc0);
			string("MOTION NOT DETECTED\r\n");
			lcd_string("NO MOTION          ");
		}
		
		
    }
}
