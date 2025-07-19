void delay_sec(unsigned int seconds)  // generating delay in seconds
{
T0PR=15000000-1;   // setting value in prescale register
T0TCR=0X01;		   // enabling timer counter and prescale counter for counting
while(T0TC<seconds);//waiting till the timer counter reaches the seconds value
T0TCR=0X03; //reset counter
T0TCR=0X00; //turning off 
}

void delay_ms(unsigned int ms)   //generating delay in milliseconds
{
T0PR=15000-1;
T0TCR=0X01;
while(T0TC<ms);
T0TCR=0X03;
T0TCR=0X00;
}

void delay_microseconds(unsigned int microsec) // generating delay in microseconds
{
T0PR=15000000-1;
T0TCR=0X01;
while(T0TC<microsec);
T0TCR=0X03;
T0TCR=0X00;
}
