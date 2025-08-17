#include<lpc21xx.h>   // Header file for LPC214x series
#include"delay.h"     // Custom delay functions

#define led0 1<<0     // LED connected to P0.0
#define eint0 14      // External interrupt 0
#define slot_en 1<<5  // Slot enable bit for VIC (Vectored Interrupt Controller)

//---------------- FIQ Interrupt Service Routine ----------------//
void FIQ_Handler(void) __irq   // FIQ interrupt handler function
{
    EXTINT = 0X01;        // Clear EINT0 interrupt flag
    IOSET0 = led0;        // Turn ON LED (P0.0 = 1)
    delay_ms(500);        // Delay 500ms
    IOCLR0 = led0;        // Turn OFF LED
    delay_ms(500);        // Delay 500ms
    VICVectAddr = 0;      // Indicate end of ISR to VIC
} 

int main()
{
    unsigned int count = 0;   

    PINSEL1 = 0x01;       // Configure P0.14 as EINT0 function
    
    IODIR0 |= led0;       // Set P0.0 as output (LED pin)

    VICIntSelect = 1<<14; // Select FIQ (not IRQ) for EINT0 interrupt
    
    VICVectCntl0 = slot_en | eint0;   // Enable vector slot and assign interrupt number
    VICVectAddr0 = (int)FIQ_Handler;  // Set ISR address

    EXTMODE = 0x01;       // Configure EINT0 as edge sensitive
    EXTPOLAR = 0x00;      // Configure falling edge trigger

    VICIntEnable = 1<<eint0;  // Enable EINT0 interrupt in VIC

    while(1) {
        count++;          // Dummy loop (keeps CPU busy)
    }
}
