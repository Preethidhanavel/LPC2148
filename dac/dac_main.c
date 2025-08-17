#include <lpc214x.h>   // Header file for LPC214x microcontrollers
#include <stdint.h>    // Standard integer type definitions


// Delay function in milliseconds
void delay_ms(uint16_t j)
{
    uint16_t x, i;
    for(i = 0; i < j; i++)       // Outer loop for 'j' milliseconds
    {
        for(x = 0; x < 6000; x++);  // Inner loop ~1 ms delay (for Cclk = 60 MHz)
    }
}

int main (void)
{
    uint16_t value;   // Variable to hold DAC output value
    uint8_t i;        // Index for sine wave lookup table

    // Lookup table for one cycle of sine wave (42 samples, amplitude range 0-1023)
    uint16_t sin_wave[42] = { 
        512,591,665,742,808,873,926,968,998,1017,1023,1017,998,968,926,873,808,742,665,591,512,
        436,359,282,216,211,151,97,55,25,6,0,6,25,55,97,151,211,216,282,359,436 
    };

    i = 0; // Start from first sample

    PINSEL1 = 0x00080000;      // Configure P0.25 as DAC output (DAC AOUT pin)
    IO0DIR = ( IO0DIR & 0xFFFFF0FF ); // Set P0.8-P0.11 as input pins for switches

    while(1)  // Infinite loop
    {		
        if ( !(IO0PIN & (1<<8)) )	/* If switch on P0.8 is pressed -> Generate SINE wave */
        {
            while(i != 42)  // Traverse through 42 sine wave samples
            {
                value = sin_wave[i];                         // Get sample value
                DACR = ( (1<<16) | (value<<6) );             // Send to DAC (10-bit left-aligned)
                delay_ms(1);                                 // Delay for wave shaping
                i++;
            }
            i = 0;  // Reset index for next cycle
        }
        else if ( !(IO0PIN & (1<<9)) )	/* If switch on P0.9 is pressed -> Generate TRIANGULAR wave */
        {
            value = 0;
            while ( value != 1023 )   // Rising slope
            {
                DACR = ( (1<<16) | (value<<6) );
                value++;
            }	
            while ( value != 0 )      // Falling slope
            {
                DACR = ( (1<<16) | (value<<6) );
                value--;
            }
        }
        else if ( !(IO0PIN & (1<<10)) )	/* If switch on P0.10 is pressed -> Generate SAWTOOTH wave */
        {
            value = 0;
            while ( value != 1023 )   // Ramp up continuously
            {
                DACR = ( (1<<16) | (value<<6) );
                value++;
            }
        }
        else if ( !(IO0PIN & (1<<11)) )	/* If switch on P0.11 is pressed -> Generate SQUARE wave */
        {
            value = 1023;   // High level
            DACR = ( (1<<16) | (value<<6) );
            delay_ms(100);

            value = 0;      // Low level
            DACR = ( (1<<16) | (value<<6) );
            delay_ms(100);
        }
        else	/* If no switch is pressed -> Output constant DC (3.3V) */
        {
            value = 1023;   // Max DAC output = 3.3V
            DACR = ( (1<<16) | (value<<6) );
        }
    }
}
