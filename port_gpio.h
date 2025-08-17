// Function to write logic value (HIGH/LOW) to a group of 8 pins (port block)
void portWrite(int block,int pinval)
{
	if(block < 10)   // If block belongs to Port0
	{
		// Block 0 - P0.0 to P0.7
		if(block == 0)
		{
			IODIR0 |= 0xFF;        // Configure P0.0-P0.7 as output
			if(pinval == 0)
				IOCLR0 |= 0xFF;    // Clear all (set LOW)
			else
				IOSET0 |= 0xFF;    // Set all (set HIGH)
		}

		// Block 1 - P0.8 to P0.15
		if(block == 1)
		{
			IODIR0 |= 0xFF00;      // Configure P0.8-P0.15 as output
			if(pinval == 0)
				IOCLR0 |= 0xFF00;  // Clear all
			else
				IOSET0 |= 0xFF00;  // Set all
		}

		// Block 2 - P0.16 to P0.23
		if(block == 2)
		{
			IODIR0 |= 0xFF0000;     // Configure P0.16-P0.23 as output
			if(pinval == 0)
				IOCLR0 |= 0xFF0000; // Clear all
			else
				IOSET0 |= 0xFF0000; // Set all
		}

		// Block 3 - P0.24 to P0.31
		if(block == 3)
		{
			IODIR0 |= 0xFF000000;   // Configure P0.24-P0.31 as output
			if(pinval == 0)
				IOCLR0 |= 0xFF000000; // Clear all
			else
				IOSET0 |= 0xFF000000; // Set all
		}
	}
	else   // If block belongs to Port1
	{
		// Block 12 - P1.16 to P1.23
		if(block == 12)
		{
			IODIR1 |= 0x00FF0000;     // Configure P1.16-P1.23 as output
			if(pinval == 0)
				IOCLR1 |= 0x00FF0000; // Clear all
			else
				IOSET1 |= 0x00FF0000; // Set all
		}

		// Block 13 - P1.24 to P1.31
		else if(block == 13)
		{
			IODIR1 |= 0xFF000000;     // Configure P1.24-P1.31 as output
			if(pinval == 0)
				IOCLR1 |= 0xFF000000; // Clear all
			else
				IOSET1 |= 0xFF000000; // Set all
		}
	}
}
