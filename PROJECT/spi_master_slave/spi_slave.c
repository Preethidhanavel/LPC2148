#include <REG51.H>
#include <string.h>

#define LCD_PORT P2
sbit RS = P3^0;
sbit RW = P3^1;
sbit EN = P3^2;

// SPI bit-banged pins
sbit MOSI = P1^5;  // Master's MOSI to slave input
sbit MISO = P1^6;  // Slave output to master
sbit SCK  = P1^7;  // Clock from master
sbit SS   = P1^4;  // Optional: Chip Select (not used here)

unsigned char receive[7];

// Dummy delay
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++);
}

void LCD_CMD(char cmd)
{
    LCD_PORT = cmd;
    RS = 0; RW = 0; EN = 1;
    delay_ms(2);
    EN = 0;
    delay_ms(5);
}

void LCD_INIT()
{
    LCD_CMD(0x38); // 8-bit mode
    LCD_CMD(0x0C); // Display on, cursor off
    LCD_CMD(0x06); // Increment cursor
    LCD_CMD(0x01); // Clear display
    LCD_CMD(0x80); // Cursor to first line
}

void LCD_CHAR(char ch)
{
    LCD_PORT = ch;
    RS = 1; RW = 0; EN = 1;
    delay_ms(2);
    EN = 0;
    delay_ms(5);
}

void LCD_STRING(char* str)
{
    while(*str)
    {
        LCD_CHAR(*str++);
    }
}

// Software SPI Receive (bit-banged 8-bit transfer)
unsigned char SPI_Slave_Receive()
{
    unsigned char i, rx = 0;

    // Wait for SCK falling edge 8 times to read 8 bits
    for(i = 0; i < 8; i++)
    {
        while(SCK == 0);       // Wait for rising edge
        rx <<= 1;
        if(MOSI) rx |= 1;
        while(SCK == 1);       // Wait for falling edge
    }

    return rx;
}

void main()
{
    unsigned char rx;
    unsigned int i = 0, j = 0, k = 0;

    LCD_INIT();
    LCD_STRING("Slave Receive");
		LCD_CMD(0xC0);
				LCD_CHAR('0');
    // Wait for start byte 0x01
    do {
        rx = SPI_Slave_Receive();
    } while (rx != 0x01);

    while(k != 16)
    {
        do
        {
            rx = SPI_Slave_Receive();
            receive[j++] = rx;
        } while(rx != 0);

        LCD_CMD(0xC0);
				LCD_CHAR('0');
				
        for(i = 0; i < j - 1; i++)
        {
					delay_ms(500);
					LCD_CHAR(receive[i]);
        }
        memset(receive, 0, 7);
        j = 0;
        k++;
    }

    while(1);
}
