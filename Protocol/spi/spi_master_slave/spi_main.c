#include <LPC21xx.h>
#include "lcd_header.h"

typedef unsigned char u8;

// SPI control bits and settings
#define MSTR_BIT   5       // Master mode bit in SPCR
#define SPIF_BIT   7       // SPI transfer complete flag
#define Mode_0     0x00    // SPI Mode 0 (CPOL=0, CPHA=0)
#define CS         7       // Chip Select pin

// Bit manipulation macros
#define SETBIT(WORD,BITPOS)   (WORD |= (1 << BITPOS))   // Set bit
#define CLRBIT(WORD,BITPOS)   (WORD &= ~(1 << BITPOS))  // Clear bit
#define READBIT(WORD,BITPOS)  ((WORD >> BITPOS) & 1)    // Read bit

// Function prototypes
void Init_SPI0(void);
u8 SPI0_Transfer(u8 data);
void delay_ms(unsigned int ms);

int main() {
    u8 tx = 10;           // Data to send
    u8 rx;                // Received data
    u8 dummy = 0;         // Dummy byte for SPI read

    LCD_INIT();           
    LCD_String("Sending - ");  
    LCD_INTEGER(tx);          // Display data to send on LCD

    delay_ms(1000);           // Small delay

    Init_SPI0();              // Initialize SPI0

    CLRBIT(IOPIN0, CS);       // Pull CS low to start SPI transfer
    SPI0_Transfer(tx);        // Send data
    rx = SPI0_Transfer(dummy); // Receive data from slave
    SETBIT(IOPIN0, CS);       // Pull CS high to end SPI transfer

    LCD_COMMAND(0xC0);        // Move cursor to second line
    LCD_String("Received - ");
    LCD_INTEGER(rx);          // Display received data

    while (1);                // Infinite loop
}

// Initialize SPI0 in master mode
void Init_SPI0(void) {
    // Configure SPI pins: P0.4(SCK), P0.5(MISO), P0.6(MOSI)
    PINSEL0 |= (1 << 8) | (1 << 10) | (1 << 12);  
    PINSEL0 &= ~((1 << 9) | (1 << 11) | (1 << 13));

    IODIR0 |= (1 << CS);      // Set CS as output
    SETBIT(IOPIN0, CS);       // Pull CS high initially

    S0SPCCR = 150;            // SPI clock rate
    S0SPCR = (1 << MSTR_BIT) | Mode_0;  // Master mode, SPI mode 0
}

// Send and receive one byte via SPI0
u8 SPI0_Transfer(u8 data) {
    S0SPDR = data;                      // Load data to SPI data register
    while (!(S0SPSR & (1 << 7)));       // Wait until transfer complete (SPIF)
    return S0SPDR;                      // Return received data
}

// Simple software delay in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);   // Nested loop for delay
}
