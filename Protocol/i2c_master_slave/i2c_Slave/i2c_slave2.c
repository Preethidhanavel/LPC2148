#include <lpc21xx.h>
#include "lcd1.h"

// Initialize I2C as slave
void i2c_slave_init(void) {
    PINSEL0 |= 0x10;       // Set P0.2 as SDA0
    PINSEL0 |= 0x40;       // Set P0.3 as SCL0

    I2CONCLR = 0xFF;       // Clear all I2C control bits
    I2SCLL = 75;           // Set SCL low period
    I2SCLH = 75;           // Set SCL high period

    I2ADR = 0x51 << 1;     // Set slave address (shifted for 7-bit addressing)
    I2CONSET = 0x44;       // Enable I2C and ACK
}

// Handle I2C slave events
void i2c_slave_handler(void) {
    unsigned char data;
    IODIR0 |= 1 << 6;      // Set P0.6 as output (LED or indicator)

    switch (I2STAT) {
        case 0x60:          // SLA+W received, ACK returned
            I2CONSET = 0x04; // Set ACK
            I2CONCLR = 0x08; // Clear SI flag
            break;

        case 0x80:          // Data byte received
            data = I2DAT;       
            lcd_command(0xC0);  // Move cursor to second line
            lcd_data(data);     // Display received data on LCD
            IOSET0 = 1 << 6;    // Turn on LED to indicate data received
            I2CONSET = 0x04;    // Set ACK for next byte
            I2CONCLR = 0x08;    // Clear SI flag
            break;

        case 0xA0:          // STOP or repeated START condition
            I2CONSET = 0x04; // Set ACK
            I2CONCLR = 0x08; // Clear SI flag
            break;
    }
}

int main(void) {
    unsigned char ch;

    i2c_slave_init();        // Initialize I2C slave
    lcd_init();              // Initialize LCD
    lcd_string("RECEIVED");  // Display initial message

    while (1) {
        if (I2CONSET & 0x08) { // Check if SI flag is set (I2C event occurred)
            i2c_slave_handler(); // Handle I2C slave event
        }
    }
}
