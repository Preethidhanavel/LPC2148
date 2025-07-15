#include <lpc214x.h>
#include <stdint.h>
#include <string.h>
#include "lcd1.h"
#include <stdio.h>

void SPI_Init()
{
	PINSEL0 = PINSEL0 | 0x00001500; /* Select P0.4, P0.5, P0.6, P0.7 as SCK0, MISO0, MOSI0 and GPIO */
	S0SPCR = 0x0020; /* SPI Master mode, 8-bit data, SPI0 mode */
	S0SPCCR = 0x10; /* Even number, minimum value 8, pre scalar for SPI Clock */
}

void SPI_Write(char data)
{
	char flush;
	IO0CLR = (1<<7);  /* SSEL = 0, enable SPI communication with slave */
	S0SPDR = data;  /* Load data to be written into the data register */
	while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed */
	flush = S0SPDR;
	IO0SET = (1<<7);  /* SSEL = 1, disable SPI communication with slave */
}

char SPI_Read()
{
	IO0CLR = (1<<7);  /* SSEL = 0, enable SPI communication with slave */
	S0SPDR = 0xFF;  /* Transmit Flush byte */
	while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed */
	IO0SET = (1<<7);  /* SSEL = 1, disable SPI communication with slave */
	return S0SPDR;  /* Return the data received */
}

int main(void)
{
	int i,j;
	uint16_t rx1;
	char receive, transmit[7];
	i = 0;
	j = 0;
	
	
	memset(transmit,0,7);
	lcd_init();
	SPI_Init();
	lcd_string("Master send");
	SPI_Write(0x01);
	for(i=0;i<16;i++)
	{
		sprintf(transmit,"i = %d",i);		
		while(transmit[j] != 0)
		{			
			SPI_Write(transmit[j]);
			delay_ms(500);
			j++;
		}
		SPI_Write('\0');
		lcd_command(0xC0);
		lcd_string(transmit);	
		memset(transmit,0,7);
		j = 0;
	}
	return 0;
}