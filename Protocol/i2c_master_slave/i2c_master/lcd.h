// LCD pin definitions
#define LCD_D 0X0f<<20  // Data pins (P1.20 - P1.23)
#define RS 1<<17        // Register Select pin
#define RW 1<<18        // Read/Write pin
#define E 1<<19         // Enable pin

// Function prototypes
void lcd_init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_integer(int);
void lcd_string(unsigned char *);
void lcd_float(float);

// Initialize LCD
void lcd_init(void)
{
    IODIR1 = LCD_D | RS | RW | E;  // Set LCD pins as output
    lcd_command(0X01);              // Clear display
    lcd_command(0X02);              // Return home
    lcd_command(0X0C);              // Display ON, cursor OFF
    lcd_command(0X28);              // 4-bit mode, 2 lines, 5x8 font
    lcd_command(0X80);              // Move cursor to first line
}

// Send command to LCD
void lcd_command(unsigned char cmd)
{
    // Send higher nibble
    IOCLR1 = LCD_D;                  
    IOSET1 = (cmd & 0xf0) << 16;     // Put higher nibble on data pins
    IOCLR1 = RS;                      // RS=0 for command
    IOCLR1 = RW;                      // RW=0 for write
    IOSET1 = E;                       // Enable pulse
    delay_ms(2);                      
    IOCLR1 = E;                       // Disable pulse

    // Send lower nibble
    IOCLR1 = LCD_D;
    IOSET1 = (cmd & 0x0f) << 20;     // Put lower nibble on data pins
    IOCLR1 = RS;                      
    IOCLR1 = RW;                      
    IOSET1 = E;                       
    delay_ms(2);                      
    IOCLR1 = E;                       
}

// Send data (character) to LCD
void lcd_data(unsigned char d)
{
    // Send higher nibble
    IOCLR1 = LCD_D;
    IOSET1 = (d & 0xf0) << 16;       // Higher nibble
    IOSET1 = RS;                      // RS=1 for data
    IOCLR1 = RW;                      
    IOSET1 = E;                       
    delay_ms(2);                      
    IOCLR1 = E;                       

    // Send lower nibble
    IOCLR1 = LCD_D;
    IOSET1 = (d & 0x0f) << 20;       // Lower nibble
    IOSET1 = RS;                      
    IOCLR1 = RW;                      
    IOSET1 = E;                       
    delay_ms(2);                      
    IOCLR1 = E;                       
}

// Display integer on LCD
void lcd_integer(int a)
{
    unsigned char b[10];
    int i = 0;

    if(a == 0){
        lcd_data('0');               // Display 0 if number is zero
        return;
    }

    if(a < 0){
        a = -a;                      // Convert negative to positive
    }

    // Convert integer to digits
    while(a != 0){
        b[i++] = a % 10;
        a = a / 10;
    }

    // Display digits in correct order
    for(--i; i >= 0; i--){
        lcd_data(b[i] + 48);         // Convert digit to ASCII
    }
}

// Display string on LCD
void lcd_string(unsigned char *s)
{
    while(*s){
        lcd_data(*s++);              // Send each character
    }
}

// Display float number on LCD (up to 3 decimal places)
void lcd_float(float f)
{
    int temp;
    temp = f;                        // Get integer part
    lcd_integer(temp);               // Display integer part
    lcd_data('.');                   // Display decimal point
    temp = (f - temp) * 1000;        // Get fractional part
    lcd_integer(temp);               // Display fractional part
}
