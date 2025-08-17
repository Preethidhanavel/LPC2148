#include <lpc21xx.h>
#include "lcd1.h"
#include "uart0.h"

int adc_value = 0;     // Store ADC value
float voltage = 0.0;   // Store calculated voltage

// Function to perform ADC conversion
int ADC_Conversion()
{
    int ab;                                
    ADCR = ADCR | 0x01000000;              // Start ADC conversion                    
    while((ADDR & 0x80000000) != 0x80000000);  // Wait until conversion completes    
    ab = ADDR;                             // Read ADC data                
    ab = (ab >> 6) & 0x3FF;                // Right shift to get 10-bit result                  
    voltage = ((3.3 * ab) / 1023) * 100;   // Convert to voltage (optional) 
    return ab;                             // Return ADC raw value
}

// Function to check ADC and update LCD
int ADC_Check()
{
    ADCR = (1 << 2) | (6 << 8) | (1 << 21); // PDN=1, CLKDIV=6, channel AD0.2
    lcd_command(0x80);                      // LCD cursor to 1st row, 1st column
    lcd_string("TEMP:");                    
    adc_value = ADC_Conversion();           // Get ADC value
    lcd_command(0x85);                      // LCD cursor to 1st row, 5th column
    lcd_int(adc_value);                     // Display ADC value
    delay_ms(100);
    return adc_value;
}

int main()
{
    int d;
    PINSEL1 |= 0x04000000;   // Set P0.29 as ADC input (AD0.2)
    IODIR0 = 0x0C;           // Set P0.2 and P0.3 as outputs
    IOCLR0 = 0x0C;           // Initialize outputs to 0
    lcd_init();  
    UART0_CONFIG();          // Initialize UART

    while (1)
    {
        d = ADC_Check();      // Read ADC and display on LCD
        lcd_command(0x80);
        lcd_string("TEMP:");
        lcd_int(d);
        delay_ms(500);

        // Temperature threshold check
        if(d > 27)
        {
            IOSET0 = 0x04;    // Turn on LED on P0.2
            delay_ms(500);
            string("Temperature exceeded threshold value\r\n");
        }
        else
        {
            IOCLR0 = 0x04;    // Turn off LED
            string("Temperature is within the limit\r\n");
        }

        // Motion sensor input check (P0.14)
        if(((IOPIN0 >> 14) & 1) == 0)
        {
            IOSET0 = 0x08;       // Turn on LED on P0.3
            lcd_command(0xC0);   // Move LCD cursor to 2nd row
            string("MOTION DETECTED\r\n");
            lcd_string("MOTION DETECT");
        }
        else
        {   
            IOCLR0 = 0x08;       // Turn off LED
            lcd_command(0xC0);
            string("MOTION NOT DETECTED\r\n");
            lcd_string("NO MOTION          ");
        }
    }
}
