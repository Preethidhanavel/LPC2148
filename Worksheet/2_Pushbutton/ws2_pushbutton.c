#include <lpc21xx.h>
#include "timer.h"
#include "uart.h"

int main()
{
    unsigned int count = 0;
    IODIR0 |= 0x6C;   // Set P0.2, P0.3, P0.5, P0.6 as output (LEDs)
    IOCLR0 |= 0x6C;   // Initialize LEDs to OFF
    uart0_config();    // Initialize UART0

    while(1)
    {
        // Check if button connected to P0.4 is pressed (active low)
        if(((IOPIN0 >> 4) & 1) == 0)
        {
            if(count >= 9) 
                count = 0;   // Reset count after 9
            count++;         // Increment count
            delay_ms(250);   // Debounce delay

            uart0_tx(count + 48); // Send count as ASCII via UART

            // Light up LEDs based on count modulo 4
            if(count % 4 == 1)
                IOSET0 = 0x20;   // Turn on LED P0.5
            else if(count % 4 == 2)
                IOSET0 = 0x40;   // Turn on LED P0.6
            else if(count % 4 == 3)
                IOSET0 = 0x04;   // Turn on LED P0.2
            else if(count % 4 == 0)
            {
                IOSET0 = 0x08;   // Turn on LED P0.3
                delay_ms(500);   // Keep LED ON for 500 ms
                IOCLR0 = 0x6C;   // Turn off all LEDs
            }
        }
    }
}
