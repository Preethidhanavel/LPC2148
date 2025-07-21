#include <LPC21xx.h>
#include "lcd_header.h"

typedef unsigned char u8;

#define MSTR_BIT   5
#define SPIF_BIT   7
#define Mode_0     0x00
#define CS         7

#define SETBIT(WORD,BITPOS)   (WORD |= (1 << BITPOS))
#define CLRBIT(WORD,BITPOS)   (WORD &= ~(1 << BITPOS))
#define READBIT(WORD,BITPOS)  ((WORD >> BITPOS) & 1)

void Init_SPI0(void);
u8 SPI0_Transfer(u8 data);
void delay_ms(unsigned int ms);

int main() {
    u8 tx = 10;
    u8 rx;
    u8 dummy = 0;

    LCD_INIT();
    LCD_String("Sending - ");
    LCD_INTEGER(tx);

    delay_ms(1000);

    Init_SPI0();

    CLRBIT(IOPIN0, CS);    // Keep CS low for both transfers!
    SPI0_Transfer(tx);
    rx = SPI0_Transfer(dummy);
    SETBIT(IOPIN0, CS);

    LCD_COMMAND(0xC0);
    LCD_String("Received - ");
    LCD_INTEGER(rx);

    while (1);
}


void Init_SPI0(void) {
    PINSEL0 |= (1 << 8) | (1 << 10) | (1 << 12);
    PINSEL0 &= ~((1 << 9) | (1 << 11) | (1 << 13));

    IODIR0 |= (1 << CS);
    SETBIT(IOPIN0, CS);

    S0SPCCR = 150;
    S0SPCR = (1 << MSTR_BIT) | Mode_0;
}

u8 SPI0_Transfer(u8 data) {
    S0SPDR = data;
    while (!(S0SPSR & (1 << 7)));
    return S0SPDR;
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);
}
