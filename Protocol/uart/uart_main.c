#include <lpc21xx.h>
#include "uart0.h"   // UART functions (UART0_CONFIG, UART0_RX, string)
#include "lcd1.h"    // LCD functions (lcd_init, lcd_command, lcd_string)

int main()
{
    char d; 
    lcd_init();            // Initialize LCD
    UART0_CONFIG();        // Initialize UART0
    IODIR0 = 1 << 17;      // Set P0.17 as output (LED control)

    while(1)
    {
        d = UART0_RX();    // Receive character from UART

        if(d == '1'){      // If received '1', turn LED ON
            IOSET0 = 1 << 17;            // Set P0.17 high (LED ON)
            string("LIGHT ON\r\n");      // Send message via UART
            lcd_command(0x80);           // Move cursor to first line
            lcd_string("LIGHT ON ");     // Display message on LCD
        }
        else if(d == '0'){ // If received '0', turn LED OFF
            IOCLR0 = 1 << 17;            // Set P0.17 low (LED OFF)
            string("LIGHT OFF\r\n");     // Send message via UART
            lcd_command(0x80);           // Move cursor to first line
            lcd_string("LIGHT OFF");     // Display message on LCD
        }
        else{               // If any other character received
            lcd_command(0x01);           // Clear LCD
            lcd_string("ENTER 0/1:");   // Prompt user on LCD
            string("ENTER 0/1:\r\n");   // Prompt user via UART
        }
    }
}
