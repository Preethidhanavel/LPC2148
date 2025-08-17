// Function prototypes
void UART0_CONFIG(void);
void UART0_TX(unsigned char);
unsigned char UART0_RX(void);
void string(unsigned char *);

// Configure UART0
void UART0_CONFIG(void)
{
    PINSEL0 |= 0x05;    // Set P0.0 = TXD0, P0.1 = RXD0
    U0LCR = 0x83;       // 8-bit data, 1 stop bit, no parity, enable DLAB
    U0DLL = 97;         // Set baud rate low byte (for 9600 bps with PCLK)
    U0DLM = 0;          // Set baud rate high byte
    U0LCR = 0x03;       // Disable DLAB, 8-bit data, 1 stop bit
}

// Transmit one byte via UART0
void UART0_TX(unsigned char d)
{
    U0THR = d;                  // Load data into Transmit Holding Register
    while (((U0LSR >> 5) & 1) == 0);  // Wait until THR empty
}

// Receive one byte from UART0
unsigned char UART0_RX(void)
{
    while ((U0LSR & 1) == 0);   // Wait until data is available
    return U0RBR;               // Return received byte
}

// Send a string via UART0
void string(unsigned char *s)
{
    while (*s) {
        UART0_TX(*s++);         // Transmit each character
        // delay_ms(150);       // Optional delay between characters
    }
}
