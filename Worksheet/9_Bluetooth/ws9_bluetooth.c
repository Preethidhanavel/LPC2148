#include <LPC21XX.H>

#define LED 1<<5  // P0.5 as LED

// Function prototypes
void UART0_CONFIG(void);
void UART0_TX(unsigned char);
unsigned char UART0_RX(void);
void string(unsigned char *);

int main()
{
    unsigned char r;        // Variable to store received UART data
    UART0_CONFIG();         // Initialize UART0
    IODIR0 |= LED;          // Set P0.5 as output (LED)
    IOCLR0 = LED;           // Turn LED off initially

    while(1)
    {
        r = UART0_RX();     // Wait and read a character from UART0

        if(r == '1')        // If received '1', turn LED on
        {
            IOSET0 = LED;
            string("LED is ON\r\n"); // Send message back via UART
        }
        else if (r == '0')  // If received '0', turn LED off
        {
            IOCLR0 = LED;
            string("LED is OFF\r\n"); // Send message back via UART
        }
        else if (r != '\r') // Ignore carriage return
        {
            string("Give 0 or 1\r\n"); // Prompt for valid input
        }
    }
}

// UART0 initialization function
void UART0_CONFIG(void)
{
    PINSEL0 |= 0x05;  // Set P0.0 as TXD0, P0.1 as RXD0
    U0LCR = 0x83;     // 8-bit data, 1 stop bit, no parity, DLAB=1
    U0DLL = 97;       // Set baud rate (e.g., 9600 for PCLK)
    U0DLM = 0;
    U0LCR = 0x03;     // DLAB = 0, lock in settings
}

// Send single character over UART0
void UART0_TX(unsigned char d)
{
    U0THR = d;                     // Load data into transmit register
    while(((U0LSR >> 5) & 1) == 0);// Wait until THR is empty
}

// Receive single character from UART0
unsigned char UART0_RX(void)
{
    while((U0LSR & 1) == 0);       // Wait until data received
    return U0RBR;                  // Read received data
}

// Send string over UART0
void string(unsigned char *s)
{
    while(*s)
    {
        UART0_TX(*s++);            // Send each character
    }
}
