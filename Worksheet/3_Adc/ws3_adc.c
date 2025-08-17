#include <lpc21xx.h>
#include "lcd1.h"

int adc_value = 0;       // Variable to store ADC result
float voltage = 0.0;     // Variable to store calculated voltage

// Function to perform ADC conversion
int ADC_Conversion()
{
    int ab;                                // Variable to temporarily store ADC raw value

    ADCR = ADCR | 0x01000000;              // Start ADC conversion (set START bit)
    while((ADDR & 0x80000000) != 0x80000000); // Wait till conversion is done (DONE bit)
    
    ab = ADDR;                             // Read ADC data register
    ab = (ab >> 6) & 0x3FF;                // Extract 10-bit result (bits 6-15)
    
    voltage = ((3.3 * ab) / 1023) * 100;   // Convert ADC value to voltage (scaled as needed)
    
    return voltage;                         // Return voltage value
}

// Function to configure ADC and display result on LCD
void ADC_Check()
{
    ADCR = (1 << 2) | (6 << 8) | (1 << 21); // Configure ADC: channel 2, CLKDIV=6, PDN=1
    lcd_command(0x80);                        // Move LCD cursor to first line
    lcd_string("ADC:");                        // Display label
    
    adc_value = ADC_Conversion();             // Perform ADC conversion
    lcd_command(0x85);                        // Move cursor to first line, 5th column
    lcd_int(adc_value);                        // Display ADC value
    delay_ms(100);                             // Small delay
}

int main()
{
    PINSEL1 |= 0x04000000;  // Configure P0.29 as ADC0.2 input
    lcd_init();              // Initialize LCD

    while (1)
    {
        ADC_Check();         // Continuously read ADC and update LCD
        delay_ms(500);       // Wait 500 ms before next reading
    }
}
