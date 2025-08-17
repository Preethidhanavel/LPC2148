// Function to write logic value (HIGH/LOW) to a given pin
void pinWrite(int pinnr,int pinval)
{
	if(pinnr < 100)   // If pin number belongs to Port0 (0-99)
	{
		IODIR0 |= 1 << pinnr;   // Configure pin as OUTPUT

		if(pinval == 0)         // If value = 0 set LOW
		{
			IOCLR0 |= 1 << pinnr;  // Clear pin (logic 0)
		}
		else                    // If value = 1  set HIGH
		{
			IOSET0 |= 1 << pinnr;  // Set pin (logic 1)
		}
	}
	else              // If pin number belongs to Port1 (100-131)
	{
		pinnr -= 100;             // Convert to Port1 pin index
		IODIR1 |= 1 << pinnr;     // Configure pin as OUTPUT

		if(pinval == 0)           // If value = 0  set LOW
		{
			IOCLR1 |= 1 << pinnr;  // Clear pin (logic 0)
		}
		else                      // If value = 1  set HIGH
		{
			IOSET1 |= 1 << pinnr;  // Set pin (logic 1)
		}
	}
}

// Function to read logic value from a given pin
int pinRead(int pinnr)
{
	if(pinnr < 100)   // If pin number belongs to Port0
	{
		IODIR0 &= ~(1 << pinnr);        // Configure pin as INPUT
		return (IOPIN0 >> pinnr) & 1;   // Read pin state (0 or 1)
	}
	else              // If pin number belongs to Port1
	{
		pinnr -= 100;                   // Convert to Port1 pin index
		IODIR1 &= ~(1 << pinnr);        // Configure pin as INPUT
		return (IOPIN1 >> pinnr) & 1;   // Read pin state (0 or 1)
	}
}
