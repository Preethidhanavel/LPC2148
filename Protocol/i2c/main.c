#include <lpc21xx.h>
#include "i2c_config.c"   // I2C low-level functions
#include "eeprom.c"       // EEPROM read/write functions
#include "lcd.h"          // LCD interface functions
#include <string.h>

#define led0 1<<19          // Define LED0 pin (if used)
#define led1 1<<18          // Define LED1 pin (if used)

int main()
{
    u8 ch;                 // Variable to store read data from EEPROM

    lcd_init();            // Initialize LCD
    i2c_init();            // Initialize I2C peripheral

    i2c_eeprom_write(0X50, 0X00, 'A');  // Write character 'A' to EEPROM address 0x00
    ch = i2c_eeprom_read(0X50, 0X00);   // Read back the value from EEPROM
    delay_ms(100);                       // Small delay for EEPROM timing

    if(ch == 'A'){                        // Check if read value matches written value
        lcd_data(ch);                     // Display character on LCD if success
    }
    else{
        lcd_string("failed");             // Display "failed" on LCD if mismatch
        delay_ms(500);
    }

    while(1); // Infinite loop to keep program running
}
