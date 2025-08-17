#include<lpc21xx.h>   // Header file for LPC21xx microcontroller

// Simple delay function
void delay(unsigned int n)
{
	int i,j;
	for(i=0;i<n;i++)          // Outer loop for delay
		for(j=0;j<12000;j++); // Inner loop (approx. 1 ms delay per iteration)
}

int main()
{
	IODIR0 |= 0xFF;   // Set P0.0 - P0.7 as output (8 LEDs connected)

	while(1)          // Infinite loop
	{
		IOSET0 |= 0xFF;   // Turn ON LEDs (set P0.0 - P0.7 high)
		delay(500);       // Delay

		IOCLR0 |= 0xFF;   // Turn OFF LEDs (clear P0.0 - P0.7 low)
		delay(500);       // Delay
	}
}
