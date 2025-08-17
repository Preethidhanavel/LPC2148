#include <lpc214x.h>   // Header for LPC2148 registers
#include <stdint.h>
#include <string.h>
#include "lcd1.h"       // LCD header file (user-defined)
#include <stdio.h>

// Function to initialize SPI0
void SPI_Init()
{
	PINSEL0 = PINSEL0 | 0x00001500;  // Configure P0.4(SCK0), P0.5(MISO0), P0.6(MOSI0), P0.7(SSEL0)
	S0SPCR = 0x0020;                 // Enable SPI in Master mode, 8-bit data transfer
	S0SPCCR = 0x10;                  // Clock prescaler (must be even, min 8) -> SPI clock speed
}

// Function to send 1 byte of data via SPI
void SPI_Write(char data)
{
	char flush;
	IO0CLR = (1<<7);          // Pull SSEL low -> enable SPI slave
	S0SPDR = data;            // Load data into SPI data register
	while ((S0SPSR & 0x80) == 0); // Wait until transfer completes (SPIF = 1)
	flush = S0SPDR;           // Dummy read to clear status register
	IO0SET = (1<<7);          // Pull SSEL high -> disable SPI slave
}

// Function to receive 1 byte via SPI
char SPI_Read()
{
	IO0CLR = (1<<7);          // Pull SSEL low -> enable SPI slave
	S0SPDR = 0xFF;            // Send dummy data (needed for clock generation)
	while ((S0SPSR & 0x80) == 0); // Wait until transfer completes
	IO0SET = (1<<7);          // Pull SSEL high -> disable SPI slave
	return S0SPDR;            // Return received data
}

int main(void)
{
	int i, j;
	uint16_t rx1;        // Variable to hold received data (not used here)
	char receive, transmit[7];  // Buffers for transmit & receive
	i = 0;
	j = 0;
	
	memset(transmit, 0, 7);   // Clear transmit buffer
	lcd_init();               // Initialize LCD
	SPI_Init();               // Initialize SPI
	
	lcd_string("Master send"); // Show message on LCD
	
	SPI_Write(0x01);          // Send initial command/data (0x01) to slave
	
	// Loop to send data "i=0" to "i=15"
	for(i = 0; i < 16; i++)
	{
		sprintf(transmit, "i = %d", i);  // Convert integer i to string and store in transmit[]
		
		while(transmit[j] != 0)          // Send string character by character
		{			
			SPI_Write(transmit[j]);     // Send one character
			delay_ms(500);              // Small delay
			j++;
		}
		
		SPI_Write('\0');                // Send null terminator at end of string
		
		lcd_command(0xC0);              // Move LCD cursor to 2nd line
		lcd_string(transmit);           // Display the sent string on LCD
		
		memset(transmit, 0, 7);         // Clear buffer for next iteration
		j = 0;                          // Reset index
	}
	return 0;
}
