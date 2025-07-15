#include <lpc214x.h>
#include <stdint.h>
#include <string.h>
#include "lcd1.h"  
#include "uart0.h"
#define CMD0   (0x40+0)
#define CMD17  (0x40+17)
#define CMD24  (0x40+24)
#define DUMMY  0xFF



void SPI_Init(void) 
{
    PINSEL0 |= 0x00001500;       // P0.4-P0.6 as SCK, MISO, MOSI
    IO0DIR |= (1 << 7);          // P0.7 as SSEL output
    IO0SET = (1 << 7);           // SSEL high (inactive)
    S0SPCR = (1 << 5) | (1 << 2); // Master mode, SPI enabled
    S0SPCCR = 8;                 // Clock prescaler (must be even)
}

uint8_t SPI_Transfer(uint8_t data) 
{
    S0SPDR = data;
    while (!(S0SPSR & (1 << 7)));
    return S0SPDR;
}

void SD_Command(uint8_t cmd, uint32_t arg, uint8_t crc) 
{
    SPI_Transfer(cmd);
    SPI_Transfer(arg >> 24);
    SPI_Transfer(arg >> 16);
    SPI_Transfer(arg >> 8);
    SPI_Transfer(arg);
    SPI_Transfer(crc);
}

uint8_t SD_WaitR1(void) 
{
    uint8_t res;
	int i;
    for ( i = 0; i < 100; i++) {
        res = SPI_Transfer(0xFF);
        if ((res & 0x80) == 0) return res;
    }
    return 0xFF;  // Timeout
}

void SD_WaitNotBusy() 
{
    while (SPI_Transfer(0xFF) != 0xFF);
}

int SD_Init(void) 
{
	int i;
    IO0SET = (1 << 7);
    for ( i = 0; i < 10; i++) SPI_Transfer(0xFF); // Send 80 clocks

    IO0CLR = (1 << 7);
    SD_Command(CMD0, 0, 0x95);
    if (SD_WaitR1() != 0x01) {
        IO0SET = (1 << 7);
        return -1;
    }
    IO0SET = (1 << 7);
    SPI_Transfer(0xFF);
    return 0;
}

int SD_ReadBlock(uint8_t *buf, uint32_t addr) 
{
	uint8_t token;
	int i;
    IO0CLR = (1 << 7);
    SD_Command(CMD17, addr, 0xFF);

    if (SD_WaitR1() != 0x00) 
		{
        IO0SET = (1 << 7);
        return -1;
    }

    
    for ( i = 0; i < 1000; i++) 
		{
        token = SPI_Transfer(0xFF);
        if (token == 0xFE) break;
    }
    if (token != 0xFE) 
		{
        IO0SET = (1 << 7);
        return -2;
    }

    for (i = 0; i < 512; i++) 
			buf[i] = SPI_Transfer(0xFF);
		
			SPI_Transfer(0xFF); 
			SPI_Transfer(0xFF); // Ignore CRC

    IO0SET = (1 << 7);
    SPI_Transfer(0xFF);
    return 0;
}

uint8_t SD_WriteBlock(uint8_t *buf, uint32_t addr) 
{
		int i;
		uint8_t response;
    IO0CLR = (1 << 7);
    SD_Command(CMD24, addr, 0xFF);

    if (SD_WaitR1() != 0x00) 
		{
        IO0SET = (1 << 7);
        return 1;
    }

    SPI_Transfer(0xFE); // Start token

    for (i = 0; i < 512; i++) 
			SPI_Transfer(buf[i]);

    SPI_Transfer(0xFF); SPI_Transfer(0xFF); // Dummy CRC

    response = SPI_Transfer(0xFF);
    if ((response & 0x1F) != 0x05) 
		{
        IO0SET = (1 << 7);
        return 2;
    }

    SD_WaitNotBusy();
    IO0SET = (1 << 7);
    SPI_Transfer(0xFF);
    return 0;
}

int main(void) 
{
		int i;
		uint8_t write_buf[512], read_buf[512];
    lcd_init();
    SPI_Init();
		UART0_CONFIG();
    lcd_string("Init SD...");
		string("INIT SDCARD...");
    if (SD_Init()) 
		{
        lcd_command(0xC0);
        lcd_string("Init Fail");
				string("FAILED TO OPEN SD");
        while (1);
    }

    lcd_command(0xC0);
    lcd_string("Writing block...");
		string("WRTING INTO SD\n");
    for (i = 0; i < 512; i++) 
			write_buf[i] = 'A' + (i % 26);

    if (SD_WriteBlock(write_buf, 0x00000000) != 0) 
		{
        lcd_command(0xC0);
        lcd_string("Write Fail");
				string("write fail");
        while (1);
    }

    delay_ms(1000);
    lcd_command(0x01);
    lcd_string("Reading block...");
		string("READING BLOCK...");
    if (SD_ReadBlock(read_buf, 0x00000000) != 0) 
		{
        lcd_command(0xC0);
        lcd_string("Read Fail");
				string("READ FAIL");
        while (1);
    }
		read_buf[511]='\0';
    lcd_command(0xC0);
    lcd_data(read_buf[0]);
    lcd_data(read_buf[1]);
    lcd_data(read_buf[2]);
		string(read_buf);
    while (1);
}
