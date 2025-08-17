#include<lpc21xx.h>   // Include header file for LPC21xx microcontroller

// Define pins for LCD
#define lcd_d 0xff      // LCD data lines connected to P0.0-P0.7
#define rs 1<<8         // Register Select pin (P0.8)
#define e  1<<10        // Enable pin (P0.10)
#define rw 1<<9         // Read/Write pin (P0.9)

// Function declarations
void lcd_init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);

// Simple delay function (approx. milliseconds)
void delay_ms(unsigned int n)
{
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<12000;j++);   // Loop to create time delay
}

int main()
{
	unsigned char *a="Embedded System";  // String to display on LCD
	lcd_init();          // Initialize LCD
	lcd_command(0X80);   // Move cursor to first line, first position
	
	// Send characters of the string one by one
	while(*a)
		lcd_data(*a++);
}

// Function to initialize LCD
void lcd_init()
{
	IODIR0 = lcd_d | rs | e | rw;  // Set LCD pins as output
	IOCLR0 = rw;                   // Always write mode (RW = 0)
	
	lcd_command(0x01); // Clear display
	lcd_command(0x02); // Return home
	lcd_command(0x0c); // Display ON, cursor OFF
	lcd_command(0x38); // 8-bit mode, 2 lines, 5x7 font
	lcd_command(0x80); // Force cursor to beginning of first line
}

// Function to send command to LCD
void lcd_command(unsigned char cmd)
{
	IOCLR0 = lcd_d;     // Clear data pins
	IOSET0 = cmd;       // Place command on data lines
	IOCLR0 = rs;        // RS=0 command register
	IOSET0 = e;         // Enable high
	delay_ms(2);        // Small delay
	IOCLR0 = e;         // Enable low (command latched)
}

// Function to send data (character) to LCD
void lcd_data(unsigned char d)
{
	IOCLR0 = lcd_d;     // Clear data pins
	IOSET0 = d;         // Place data on data lines
	IOSET0 = rs;        // RS=1  data register
	IOSET0 = e;         // Enable high
	delay_ms(2);        // Small delay
	IOCLR0 = e;         // Enable low (data latched)
}
