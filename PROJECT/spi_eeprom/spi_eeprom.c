#include <lpc21xx.h>
#include "lcd1.h"

#define CS_LOW()  IO0CLR = (1 << 7)   // Chip Select Low (P0.7)
#define CS_HIGH() IO0SET = (1 << 7)   // Chip Select High (P0.7)

#define SPI_WREN   0x06  // Write Enable
#define SPI_WRITE  0x02  // Write Data
#define SPI_READ   0x03  // Read Data
#define SPI_RDSR   0x05  // Read Status Register
#define Mode_0     0x00  // CPOL = 0, CPHA = 0 (Stable for 25LC256)

#define LED1 (1 << 17)
#define LED2 (1 << 18)

void SPI_Init(void);
unsigned char SPI_Transfer(unsigned char data);
void EEPROM_WriteEnable(void);
void EEPROM_WriteByte(unsigned int addr, unsigned char data);
unsigned char EEPROM_ReadByte(unsigned int addr);
unsigned char EEPROM_ReadStatus(void);
void delay(unsigned int count);

int main() {
    unsigned char readValue;
    unsigned char writeValue = 'A';  // Data to write
    lcd_init();
    SPI_Init();  // Initialize SPI

    // Set P0.7 (CS), P0.17 (LED1), and P0.18 (LED2) as output
    IODIR0 |= LED1 | LED2 | (1 << 7); 
    CS_HIGH();  // CS inactive
    IOSET0 = LED1 | LED2;  // Turn both LEDs ON initially

    // Write data to EEPROM
    EEPROM_WriteEnable();
    EEPROM_WriteByte(0x0000, writeValue);

    // Wait for EEPROM write completion
    while (EEPROM_ReadStatus() & 0x01);  // Wait while Write-In-Progress (WIP) bit is set

    // Read data from EEPROM
    readValue = EEPROM_ReadByte(0x0000);
		lcd_command(0X80);
    lcd_data(readValue);

    // Check if read data matches written data
    if (readValue == writeValue) {
        IOCLR0 = LED1;  // Turn off LED1
        IOSET0 = LED2;  // Turn on LED2
        lcd_command(0xC0);
        lcd_string("Received");
    } 
		else {
        IOCLR0 = LED2;  // Turn off LED2
        IOSET0 = LED1;  // Turn on LED1
        lcd_command(0xC0);
        lcd_string("Not Received");
    }

    while (1);  // Infinite loop
}

// Initialize SPI0 in Master Mode
void SPI_Init(void) {
    // Configure SPI0 pins (P0.4: SCK, P0.5: MISO, P0.6: MOSI)
   // PINSEL0 |= (1 << 8) | (1 << 10) | (1 << 12);  // Set P0.4 (SCK), P0.5 (MISO), P0.6 (MOSI)
		PINSEL0|=0X00001500;
    S0SPCCR = 150;                 // Set SPI clock rate (
    S0SPCR = (1 << 5) | Mode_0;  // Master mode, Mode 0 (CPOL = 0, CPHA = 0)
}

// SPI Transfer Function
unsigned char SPI_Transfer(unsigned char data) {
		unsigned char stat;
		stat=S0SPSR;
    S0SPDR = data;  						// Load data into SPI Data Register
    while (!(S0SPSR & 0x80));  // Wait for transfer to complete (SPIF flag)
    return S0SPDR;  						// Return received data
}

// Enable EEPROM Write Operation
void EEPROM_WriteEnable(void) {
    CS_LOW();
    SPI_Transfer(SPI_WREN);
    CS_HIGH();
}

// Write a Byte to EEPROM at specified address
void EEPROM_WriteByte(unsigned int addr, unsigned char data) {
		CS_LOW();
    SPI_Transfer(SPI_WRITE);
    SPI_Transfer((addr >> 8) & 0xFF);  // Send high address byte
    SPI_Transfer(addr & 0xFF);         // Send low address byte
    SPI_Transfer(data);                // Send data byte
    CS_HIGH();
    
    delay_ms(5);  // Ensure write is completed (EEPROM requires up to 5ms)
}

// Read a Byte from EEPROM at specified address
unsigned char EEPROM_ReadByte(unsigned int addr) {
    unsigned char value;
    CS_LOW();
    SPI_Transfer(SPI_READ);
    SPI_Transfer((addr >> 8) & 0xFF);  // Send high address byte
    SPI_Transfer(addr & 0xFF);         // Send low address byte
    value = SPI_Transfer(0x00);        // Read data byte
    CS_HIGH();
    return value;
}

// Read EEPROM Status Register
unsigned char EEPROM_ReadStatus(void) {
    unsigned char status;
    CS_LOW();
    SPI_Transfer(SPI_RDSR);
    status = SPI_Transfer(0x00);  // Read status register
    CS_HIGH();
    return status;
}


