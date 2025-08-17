#include <lpc21xx.h>
//#include"timer.h"
#include "lcd1.h"
#include <string.h>
#include "uart0.h"

// Define keypad columns (input pins)
#define C0 (IOPIN0 & (1<<16))
#define C1 (IOPIN0 & (1<<17))
#define C2 (IOPIN0 & (1<<18))
#define C3 (IOPIN0 & (1<<19))

// Define keypad rows (output pins)
#define R0 1<<20
#define R1 1<<21
#define R2 1<<22
#define R3 1<<23

#define sw 1<<14  // Optional switch

// Keypad mapping: 4x4 keypad lookup table
unsigned char key_lut[4][4] = {
    {'7','8','9','A'},
    {'4','5','6','B'},
    {'1','2','3','C'},
    {'*','0','#','D'}
};

unsigned int row_val, col_val; // Store row and column of pressed key
char d[5];                     // Store entered password
int i = 0;

// Function to read a key from 4x4 keypad
unsigned char Keypad()
{
    int j;
    char pass_encrypt[5];

    PINSEL1 |= 0x00000000;          // Configure P0.16-P0.23 as GPIO
    IODIR0 |= R0 | R1 | R2 | R3;    // Rows as output

    lcd_command(0x80);
    lcd_string("Enter Password");   // Prompt on LCD
    string("PASSWORD IS BEING ENTERED\r\n"); // Send prompt via UART

    while(1)
    {
        // Reset rows and set columns
        IOCLR0 |= R0 | R1 | R2 | R3;
        IOSET0 |= C0 | C1 | C2 | C3;

        // Wait until a key is pressed
        while((C0 && C1 && C2 && C3) == 1);

        // Check which row is active
        IOCLR0 |= R0; IOSET0 |= R1 | R2 | R3;
        if((C0 && C1 && C2 && C3) == 0) { row_val = 0; break; }

        IOCLR0 |= R1; IOSET0 |= R0 | R2 | R3;
        if((C0 && C1 && C2 && C3) == 0) { row_val = 1; break; }

        IOCLR0 |= R2; IOSET0 |= R0 | R1 | R3;
        if((C0 && C1 && C2 && C3) == 0) { row_val = 2; break; }

        IOCLR0 |= R3; IOSET0 |= R0 | R1 | R2;
        if((C0 && C1 && C2 && C3) == 0) { row_val = 3; break; }
    }

    // Determine which column is active
    if((C0 && C1 && C2 && C3) == 0)
    {
        if(C0 == 0) col_val = 0;
        else if(C1 == 0) col_val = 1;
        else if(C2 == 0) col_val = 2;
        else col_val = 3;
    }

    delay_ms(150);                         // Debounce delay
    while((C0 && C1 && C2 && C3) == 0);    // Wait till key release

    return key_lut[row_val][col_val];      // Return the key pressed
}

int main()
{
    IODIR0 = 1 << 2;          // Set P0.2 as output for LED/door control
    lcd_init();               // Initialize LCD
    UART0_CONFIG();           // Initialize UART0

    lcd_command(0x82);
    lcd_string("***KEYPAD***"); // Display initial message
    delay_ms(500);
    lcd_command(0x01);        // Clear LCD
    lcd_command(0xC0);        // Move cursor to second line

    // Read 4-digit password from keypad
    while(i < 4)
    {
        d[i] = Keypad();      // Read a key
        lcd_data('*');         // Display '*' for each key press
        i++;
    }

    // Check entered password
    if(strcmp(d, "9087") == 0)   // If password matches
    {
        lcd_command(0xC0);
        lcd_string("door unlocked");  // Display on LCD
        string("DOOR OPENED");        // Send via UART
        IOSET0 = 1 << 2;              // Turn on LED / unlock door
    }
    else                          // If password is wrong
    {
        lcd_command(0xC0);
        lcd_string("door locked");   // Display on LCD
        string("DOOR LOCKED OOPS! TRY NEXT TIME"); // Send via UART
        IOCLR0 = 1 << 2;             // Turn off LED / keep door locked
    }
}
