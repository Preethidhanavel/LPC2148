#include <lpc21xx.h>
#include "uart.h"     // UART functions (uart0_config, uart0_tx)
#include "lcd1.h"     // LCD functions (lcd_init, lcd_command, lcd_string, lcd_int)
#include <stdio.h>    // For sprintf

void uart0_tx_int(int d); // Function to send integer via UART
float voltage = 0.0;      // Store calculated voltage

// Function to perform ADC conversion
int ADC_Conversion()
{
    int ab;                                // Variable to store ADC raw value
    ADCR |= 1 << 24;                       // Start ADC conversion (set START bit)
    while((ADDR & 0x80000000) != 0x80000000); // Wait till conversion is done (DONE bit)
    
    ab = ADDR;                             // Read ADC data register
    ab = (ab >> 6) & 0x3FF;                // Extract 10-bit ADC result (bits 6-15)
    voltage = ((3.3 * ab) / 1023) * 100;   // Convert ADC value to voltage (scaled)
    
    return voltage;                         // Return calculated voltage
}

// Function to configure ADC, perform conversion, and display on LCD
int ADC_Check()
{
    int adc_value = 0;
    PINSEL1 |= 0x04000000;                 // Configure P0.29 as ADC0.2 input
    ADCR = (1 << 2) | (6 << 8) | (1 << 21); // ADC setup: channel 2, CLKDIV=6, PDN=1

    lcd_command(0x80);                      // Move LCD cursor to first line
    lcd_string("ADC:");                     // Display label

    adc_value = ADC_Conversion();           // Get ADC conversion result

    lcd_command(0x85);                      // Move cursor to first line, 5th column
    lcd_int(adc_value);                      // Display ADC value on LCD
    delay_ms(100);                           // Small delay
    return adc_value;                        // Return ADC value
}

int main()
{
    int d;
    lcd_init();       // Initialize LCD
    uart0_config();   // Initialize UART0

    while(1)
    {
        d = ADC_Check();          // Read ADC and update LCD
        uart0_tx_int(d);          // Send ADC value via UART
        delay_ms(500);            // Wait 500 ms before next reading
    }
}

// Function to send an integer via UART as ASCII characters
void uart0_tx_int(int d)
{
    char a[10];
    int i;
    sprintf(a, "%d", d);          // Convert integer to string
    for(i = 0; a[i]; i++)
    {
        uart0_tx(a[i]);           // Send each character via UART
    }
}
