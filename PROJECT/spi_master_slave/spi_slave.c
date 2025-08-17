#include <REG51.H>      // Header for 8051 registers
#include <string.h>     // For memset()

#define LCD_PORT P2     // LCD data pins connected to Port2
sbit RS = P3^0;         // LCD RS pin
sbit RW = P3^1;         // LCD RW pin
sbit EN = P3^2;         // LCD EN pin

// SPI bit-banged pins
sbit MOSI = P1^5;       // Data from Master → Slave input
sbit MISO = P1^6;       // Data from Slave → Master output (not used here)
sbit SCK  = P1^7;       // Clock from Master
sbit SS   = P1^4;       // Chip Select (optional, not used here)

unsigned char receive[7];   // Buffer for received data

// Simple delay
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++);   // Approx 1ms delay
}

// Send command to LCD
void LCD_CMD(char cmd)
{
    LCD_PORT = cmd;
    RS = 0; RW = 0; EN = 1;
    delay_ms(2);
    EN = 0;
    delay_ms(5);
}

// Initialize LCD
void LCD_INIT()
{
    LCD_CMD(0x38); // 8-bit mode, 2 lines
    LCD_CMD(0x0C); // Display ON, Cursor OFF
    LCD_CMD(0x06); // Auto increment cursor
    LCD_CMD(0x01); // Clear display
    LCD_CMD(0x80); // Cursor to first line
}

// Display single character on LCD
void LCD_CHAR(char ch)
{
    LCD_PORT = ch;
    RS = 1; RW = 0; EN = 1;
    delay_ms(2);
    EN = 0;
    delay_ms(5);
}

// Display string on LCD
void LCD_STRING(char* str)
{
    while(*str)
    {
        LCD_CHAR(*str++);
    }
}

// Bit-banged SPI slave receive (8-bit)
unsigned char SPI_Slave_Receive()
{
    unsigned char i, rx = 0;

    for(i = 0; i < 8; i++)       // Read 8 bits
    {
        while(SCK == 0);        // Wait for clock HIGH
        rx <<= 1;               // Shift left for next bit
        if(MOSI) rx |= 1;       // Read MOSI line
        while(SCK == 1);        // Wait for clock LOW
    }
    return rx;                  // Return received byte
}

void main()
{
    unsigned char rx;           // Store single received byte
    unsigned int i = 0, j = 0, k = 0;

    LCD_INIT();                 // Initialize LCD
    LCD_STRING("Slave Receive"); // Show initial message
    LCD_CMD(0xC0);              // Move cursor to 2nd line
    LCD_CHAR('0');              // Print '0'

    // Wait until Master sends start byte (0x01)
    do {
        rx = SPI_Slave_Receive();
    } while (rx != 0x01);

    // Loop to receive 16 messages from Master
    while(k != 16)
    {
        // Receive characters until NULL (0x00) byte
        do
        {
            rx = SPI_Slave_Receive();
            receive[j++] = rx;   // Store in buffer
        } while(rx != 0);

        LCD_CMD(0xC0);          // Move cursor to 2nd line
        LCD_CHAR('0');          // Print '0' again as marker
		
        // Display received string on LCD
        for(i = 0; i < j - 1; i++)  // Exclude last NULL byte
        {
            delay_ms(500);
            LCD_CHAR(receive[i]);
        }

        memset(receive, 0, 7);  // Clear buffer
        j = 0;                  // Reset index
        k++;                    // Count messages received
    }

    while(1);   // Infinite loop
}
