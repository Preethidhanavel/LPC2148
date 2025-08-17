#include <lpc21xx.h>
#include "i2c_config.c"   // Low-level I2C functions
#include "eeprom.c"       // EEPROM read/write functions
#include "lcd.h"          // LCD interface functions
#include <string.h>

#define led0 1<<19          // LED0 pin (not used here)
#define led1 1<<18          // LED1 pin (not used here)

int main()
{
    lcd_init();              // Initialize LCD
    i2c_init();              // Initialize I2C peripheral

    lcd_string("I2C");       // Display "I2C" on LCD

    i2c_eeprom_write(0X50, 0X0000, 'A'); // Write character 'A' to EEPROM address 0x0000

    lcd_command(0X01);       // Clear LCD
    lcd_string("Transfer over"); // Display message after I2C transfer
    delay_ms(500);           // Small delay to allow message display

    while(1);                // Infinite loop to keep program running
}
