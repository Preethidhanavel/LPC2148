// Generate delay in seconds
void delay_sec(unsigned int seconds)
{
    T0PR = 15000000 - 1;       // Set prescale register to divide clock (adjust for 1 second)
    T0TCR = 0x01;               // Enable timer and start counting
    while(T0TC < seconds);      // Wait until timer counter reaches desired seconds
    T0TCR = 0x03;               // Reset timer counter
    T0TCR = 0x00;               // Turn off timer
}

// Generate delay in milliseconds
void delay_ms(unsigned int ms)
{
    T0PR = 15000 - 1;           // Set prescale for 1 ms count
    T0TCR = 0x01;               // Enable timer
    while(T0TC < ms);           // Wait until timer counter reaches milliseconds
    T0TCR = 0x03;               // Reset timer counter
    T0TCR = 0x00;               // Turn off timer
}

// Generate delay in microseconds
void delay_microseconds(unsigned int microsec)
{
    T0PR = 15000000 - 1;        // Set prescale (adjust according to system clock)
    T0TCR = 0x01;               // Enable timer
    while(T0TC < microsec);     // Wait until timer counter reaches microseconds
    T0TCR = 0x03;               // Reset timer counter
    T0TCR = 0x00;               // Turn off timer
}
