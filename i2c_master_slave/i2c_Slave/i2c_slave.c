#include <lpc21xx.h>
#include "lcd1.h"

void i2c_slave_init(void) {
    PINSEL0 |= 0X10; // SDA0
    PINSEL0 |= 0X40; // SCL0

    I2CONCLR = 0xFF;
    I2SCLL = 75;
    I2SCLH = 75;

    I2ADR = 0xA0;  // Slave address
    I2CONSET = 0x44;  
}

void i2c_slave_handler(void) {
	unsigned char data;
	IODIR0|=1<<6;
    switch (I2STAT) {
        case 0x60:  
            I2CONSET = 0x04;  // Set ACK
            I2CONCLR = 0x08;  // Clear SI
            break;
        case 0x80:  // Data byte received
            data = I2DAT;
						lcd_command(0Xc0);
						lcd_data(data);
						IOSET0=1<<6;
            I2CONSET = 0x04;
            I2CONCLR = 0x08;
            break;
        case 0xA0:  // STOP or repeated START
            I2CONSET = 0x04;
            I2CONCLR = 0x08;
            break;
    }

}

int main(void) {
	unsigned char ch;
    i2c_slave_init();
		lcd_init();
		lcd_string("RECEIVED");
    while (1) {
        if (I2CONSET & 0x08) 
				{  
            i2c_slave_handler();
					
        }
    }
}
