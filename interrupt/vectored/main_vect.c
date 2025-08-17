#include<lpc21xx.h>     // Header file for LPC21xx microcontroller registers
#include"delay.h"       // Custom delay function header

#define led0 1<<0       // Define LED connected to P0.0
#define eint0 14        // External interrupt number for EINT0
#define eint0_en 1<<5   // Enable slot number (used in VICVectCntl)

// ISR (Interrupt Service Routine) for EINT0 interrupt
void eint0_int(void) __irq
{
    EXTINT = 0x01;        // Clear interrupt flag for EINT0
    IOSET0 = led0;        // Turn ON LED (P0.0 = 1)
    delay_ms(500);        // Delay 500 ms
    IOCLR0 = led0;        // Turn OFF LED (P0.0 = 0)
    delay_ms(500);        // Delay 500 ms
    VICVectAddr = 0;      // Signal end of interrupt to VIC
} 

int main()
{
    unsigned int count = 0;

    // Configure P0.14 pin for EINT0 function
    // PINSEL0=0x01;        // Alternate option for pin selection (commented)
    PINSEL1 = 0x01;        // Select function for P0.16 (EINT0)

    IODIR0 |= led0;        // Set P0.0 as output (LED pin)

    // Configure VIC (Vector Interrupt Controller)
    VICIntSelect = 0;                          // Select EINT0 as IRQ (not FIQ)
    VICVectCntl1 = eint0_en | eint0;           // Assign slot and enable EINT0
    VICVectAddr1 = (int)eint0_int;             // Set ISR address for EINT0

    // Configure EINT0 mode
    EXTMODE = 0x01;        // EINT0 is edge-sensitive
    EXTPOLAR = 0x01;       // Trigger on rising edge

    VICIntEnable = 1 << eint0;  // Enable EINT0 interrupt

    // Infinite loop (dummy counter increment)
    while(1){
        count++;
    }
}
