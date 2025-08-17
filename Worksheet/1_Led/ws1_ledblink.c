#include <lpc21xx.h>
#include "timer.h"

int main()
{
    IODIR0 = 0xFF;   // Set P0.0 to P0.7 as output (8 LEDs)
    IOCLR0 = 0xFF;   // Initialize all LEDs to OFF

    while(1)
    {
        IOSET0 = 0x01;   // Turn on LED at P0.0
        delay_ms(250);   // Wait 250 ms
        IOCLR0 = 0x01;   // Turn off LED at P0.0

        IOSET0 = 0x02;   // Turn on LED at P0.1
        delay_ms(250);   // Wait 250 ms
        IOCLR0 = 0x02;   // Turn off LED at P0.1

        IOSET0 = 0x04;   // Turn on LED at P0.2
        delay_ms(250);   
        IOCLR0 = 0x04;   

        IOSET0 = 0x08;   // Turn on LED at P0.3
        delay_ms(250);   
        IOCLR0 = 0x08;   

        IOSET0 = 0x10;   // Turn on LED at P0.4
        delay_ms(250);   
        IOCLR0 = 0x10;   

        IOSET0 = 0x20;   // Turn on LED at P0.5
        delay_ms(250);   
        IOCLR0 = 0x20;   

        IOSET0 = 0x40;   // Turn on LED at P0.6
        delay_ms(250);   
        IOCLR0 = 0x40;   

        IOSET0 = 0x80;   // Turn on LED at P0.7
        delay_ms(250);   
        IOCLR0 = 0x80;   
    }
}
