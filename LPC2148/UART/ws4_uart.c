#include<lpc21xx.h>
#include"uart.h"
#include"lcd1.h"
#include<stdio.h>
void uart0_tx_int(int d);
float voltage=0.0;

int ADC_Conversion()
{
 int ab;                                //Variable to store ADC value 
 ADCR  |= 1<<24;             //Start conversion                    
 while((ADDR&0x80000000)!=0x80000000);  // Wait here till conversion is over    
 ab    = ADDR;             // Extracting the result          
 ab    = (ab>>6)&0X3ff;                       // Shift 6 bits right                  
 voltage = ((3.3*ab)/1023)*100;

 return voltage;

}

 int ADC_Check()
{
 int adc_value=0;
 PINSEL1 |= 0x04000000;  									// Set P0.29 as ADC input (AD0.2)
 ADCR = (1 << 2) | (6 << 8) | (1 << 21);                      // PDN=1,CLKDIV=6,channel=AD0.2         
 lcd_command(0x80);
 lcd_string("ADC:");
 adc_value=ADC_Conversion();            /* Get the result of conversion         */
 lcd_command(0x85);                    /* 2nd row, 5th location                */
 lcd_int(adc_value);
 delay_ms(100);
 return adc_value;
}
int main()
{
	int d;
	lcd_init();
	uart0_config();
	while(1)
	{
		d=ADC_Check();
		uart0_tx_int(d);
		delay_ms(500);
	}
}
void uart0_tx_int(int d)
{
	char a[10];
	int i;
	sprintf(a,"%d",d);
	for (i=0;a[i];i++)
	{
		uart0_tx(a[i]);
	}
}

  
