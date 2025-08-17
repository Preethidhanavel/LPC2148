#include<lpc21xx.h>    // Header file for LPC21xx microcontroller
#include"pin_gpio.h"   // GPIO pin control functions (pinRead, pinWrite)
#include"port_gpio.h"  // GPIO port control functions (portRead, portWrite)

int main()
{
	int n,m;   // Variables to store pin read values

	// Write logic levels to individual pins
	pinWrite(2,1);     // Set pin number 2 -> HIGH
	pinWrite(4,1);     // Set pin number 4 -> HIGH
	pinWrite(120,0);   // Set pin number port 1 pin 20-> LOW
	pinWrite(124,1);   // Set pin number port 1 pin 24 -> HIGH

	// Write logic levels to entire ports
	portWrite(0,1);    // Set port 0 -> HIGH (all pins of port0 = 1)
	portWrite(1,0);    // Set port 1 -> LOW  (all pins of port1 = 0)
	portWrite(12,1);   // Set port 12 -> HIGH
	portWrite(13,0);   // Set port 13 -> LOW

	// Read input pin values
	n = pinRead(2);    // Read value of pin number 2 and store in n
	m = pinRead(20);   // Read value of pin number 20 and store in m

	// Program ends (values of n and m can be checked in debugger)
}
