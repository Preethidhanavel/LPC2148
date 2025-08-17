// Function to generate delay in seconds using Timer0
void delay_sec(unsigned int seconds)  
{
    T0PR = 15000000 - 1;   // Prescaler = 15 MHz - 1 -> 1 sec per increment
    T0TCR = 0x01;          // Enable Timer Counter and Prescale Counter
    while (T0TC < seconds); // Wait until Timer Counter reaches required seconds
    T0TCR = 0x03;          // Reset counter (TC and PC reset)
    T0TCR = 0x00;          // Turn off Timer0
}

// Function to generate delay in milliseconds using Timer0
void delay_ms(unsigned int ms)   
{
    T0PR = 15000 - 1;      // Prescaler = 15,000 - 1 -> 1 ms per increment
    T0TCR = 0x01;          // Enable Timer Counter and Prescale Counter
    while (T0TC < ms);     // Wait until Timer Counter reaches required ms
    T0TCR = 0x03;          // Reset counter
    T0TCR = 0x00;          // Turn off Timer0
}

// Function to generate delay in microseconds using Timer0
void delay_microseconds(unsigned int microsec) 
{
    T0PR = 15 - 1;         // Prescaler = 15 - 1 -> 1 microsec per increment
    T0TCR = 0x01;          // Enable Timer Counter and Prescale Counter
    while (T0TC < microsec); // Wait until Timer Counter reaches required microsec
    T0TCR = 0x03;          // Reset counter
    T0TCR = 0x00;          // Turn off Timer0
}
