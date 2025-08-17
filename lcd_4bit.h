#include"timer.h"    // Include header file for delay function

// Define LCD pin connections (using Port1 pins P1.20 - P1.23 for data, P1.17-19 for control)
#define lcd_d 0x0f<<20   // LCD data pins connected to P1.20-P1.23 (D4-D7 for 4-bit mode)
#define rs 1<<17         // RS pin connected to P1.17 (Register Select)
#define rw 1<<18         // RW pin connected to P1.18 (Read/Write)
#define e  1<<19         // Enable pin connected to P1.19 (Enable signal)

// Function declarations
void lcd_init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);

// Initialize LCD
void lcd_init()
{
    IODIR1 |= lcd_d | rs | rw | e;   // Set LCD pins as output

    lcd_command(0x01);  // Clear display
    lcd_command(0x02);  // Return home
    lcd_command(0x0c);  // Display ON, cursor OFF
    lcd_command(0x28);  // 4-bit mode, 2-line display, 5x7 font
    lcd_command(0x80);  // Set cursor at first line, first column
}

// Send a command to LCD
void lcd_command(unsigned char cmd)
{
    // Send higher nibble
    IOCLR1 = lcd_d;                // Clear data lines
    IOSET1 = (cmd & 0xF0) << 16;   // Put higher nibble on P1.20-P1.23
    IOCLR1 = rs;                   // RS = 0 (command register)
    IOCLR1 = rw;                   // RW = 0 (write mode)
    IOSET1 = e;                    // Enable high
    delay_ms(2);
    IOCLR1 = e;                    // Enable low (command latched)

    // Send lower nibble
    IOCLR1 = lcd_d;                // Clear data lines
    IOSET1 = (cmd & 0x0F) << 20;   // Put lower nibble on P1.20-P1.23
    IOCLR1 = rs;                   // RS = 0 (command register)
    IOCLR1 = rw;                   // RW = 0 (write mode)
    IOSET1 = e;                    // Enable high
    delay_ms(2);
    IOCLR1 = e;                    // Enable low (command latched)
}

// Send data (character) to LCD
void lcd_data(unsigned char d)
{
    // Send higher nibble
    IOCLR1 = lcd_d;                // Clear data lines
    IOSET1 = (d & 0xF0) << 16;     // Put higher nibble on P1.20-P1.23
    IOSET1 = rs;                   // RS = 1 (data register)
    IOCLR1 = rw;                   // RW = 0 (write mode)
    IOSET1 = e;                    // Enable high
    delay_ms(2);
    IOCLR1 = e;                    // Enable low (data latched)

    // Send lower nibble
    IOCLR1 = lcd_d;                // Clear data lines
    IOSET1 = (d & 0x0F) << 20;     // Put lower nibble on P1.20-P1.23
    IOSET1 = rs;                   // RS = 1 (data register)
    IOCLR1 = rw;                   // RW = 0 (write mode)
    IOSET1 = e;                    // Enable high
    delay_ms(2);
    IOCLR1 = e;                    // Enable low (data latched)
}

// Display integer on LCD
void lcd_int(int a)
{
    unsigned char b[10];   // To store digits
    int i = 0;

    if (a == 0)
        lcd_data(0 + 48);  // If zero, directly print '0'
    else
    {
        if (a < 0)         // If number is negative
        {
            lcd_data('-'); // Print minus sign
            a = -a;        // Convert to positive
        }

        // Extract digits and store in reverse order
        while (a > 0)
        {
            b[i++] = a % 10;
            a /= 10;
        }

        // Print digits in correct order
        for (i--; i >= 0; i--)
            lcd_data(b[i] + 48);
    }
}

// Display string on LCD
void lcd_string(char *s)
{
    while (*s)              // Loop until NULL character
        lcd_data(*s++);     // Print each character
}

// Display floating-point number on LCD
void lcd_float(float f)
{
    int temp;
    temp = f;               // Extract integer part
    lcd_int(temp);          // Print integer part
    lcd_data('.');          // Print decimal point
    temp = (f - temp) * 1000;  // Extract 3 decimal places
    lcd_int(temp);          // Print fractional part
}
