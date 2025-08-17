#include<lpc21xx.h>    // Header file for LPC21xx microcontroller
#include"delay.h"      // User-defined delay functions

// Define LED pins on PORT0
#define led0 1<<0
#define led1 1<<1
#define led2 1<<2
#define led3 1<<3

// External interrupt line definitions
#define slot_en 1<<5
#define eint0 14
#define eint1 15
#define eint2 16
#define eint3 17

// Non-vectored Interrupt Service Routine
void non_vect_isr(void) __irq
{
    // Check if EINT0 caused the interrupt
    if((VICIRQStatus >> eint0) & 1) {
        EXTINT = 0x01;         // Clear interrupt flag for EINT0
        IOSET0 = led0;         // Turn ON LED0
        delay_ms(500);
        IOCLR0 = led0;         // Turn OFF LED0
        delay_ms(500);
    }

    // Check if EINT1 caused the interrupt
    if((VICIRQStatus >> eint1) & 1) {
        EXTINT = 0x02;         // Clear interrupt flag for EINT1
        IOSET0 = led1;         // Turn ON LED1
        delay_ms(500);
        IOCLR0 = led1;         // Turn OFF LED1
        delay_ms(500);
    }

    // Check if EINT2 caused the interrupt
    if((VICIRQStatus >> eint2) & 1) {
        EXTINT = 0x04;         // Clear interrupt flag for EINT2
        IOSET0 = led2;         // Turn ON LED2
        delay_ms(500);
        IOCLR0 = led2;         // Turn OFF LED2
        delay_ms(500);
    }

    // Check if EINT3 caused the interrupt
    if((VICIRQStatus >> eint3) & 1) {
        EXTINT = 0x08;         // Clear interrupt flag for EINT3
        IOSET0 = led3;         // Turn ON LED3
        delay_ms(500);
        IOCLR0 = led3;         // Turn OFF LED3
        delay_ms(500);
    }

    VICVectAddr = 0;           // Acknowledge end of interrupt
}

int main()
{
    unsigned int count = 0;

    // Configure pins for EINT0, EINT1, EINT2, EINT3 (set as external interrupt)
    PINSEL0 |= (1<<29) | (1<<31);         // Configure pins for EINT3, etc.
    PINSEL1 |= (0x01) | (1<<9) | (1<<8);  // Configure pins for EINT0, EINT1, EINT2

    // Configure LED pins as output
    IODIR0 |= led0 | led1 | led2 | led3;

    // Select IRQ (not FIQ) for all external interrupts
    VICIntSelect = 0;

    // Assign default ISR (non-vectored ISR)
    VICDefVectAddr = (int) non_vect_isr;

    // Configure interrupts as edge-sensitive
    EXTMODE = 0x0F;

    // Configure as falling edge triggered
    EXTPOLAR = 0x00;

    // Enable EINT0, EINT1, EINT2, EINT3 interrupts
    VICIntEnable = (1<<eint0) | (1<<eint1) | (1<<eint2) | (1<<eint3);

    // Infinite loop (keeps program running)
    while(1)
        count++;   // Dummy increment
}
