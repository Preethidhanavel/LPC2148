// Function prototypes
void UART0_CONFIG(void);
void UART0_TX(unsigned char);
unsigned char UART0_RX(void);
void string(unsigned char *);

// UART0 initialization
void UART0_CONFIG(void)
{
    PINSEL0 |= 0x05;   // Select TXD0 and RXD0 pins for UART0
    U0LCR = 0x83;      // 8-bit data, 1 stop bit, no parity, DLAB=1
    U0DLL = 97;        // Set baud rate divisor (for 9600 baud with PCLK)
    U0DLM = 0;         // Set high byte of divisor to 0
    U0LCR = 0x03;      // DLAB=0, keep other settings (ready for transmit/receive)
}

// Transmit a single character via UART0
void UART0_TX(unsigned char d)
{
    U0THR = d;                   // Load data into Transmit Holding Register
    while(((U0LSR >> 5) & 1) == 0);  // Wait until THR is empty
}

// Receive a single character via UART0
unsigned char UART0_RX(void)
{
    while((U0LSR & 1) == 0);    // Wait until data is available in RBR
    return U0RBR;                // Read received character
}

// Transmit a string via UART0
void string(unsigned char *s)
{
    while(*s)
    {
        UART0_TX(*s++);          // Send each character one by one
    }
}
