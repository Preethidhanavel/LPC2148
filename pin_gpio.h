void pinWrite(int pinnr,int pinval)
{
	if(pinnr<100)
	{
		IODIR0|=1<<pinnr;
		if(pinval==0)
		{
			IOCLR0|=1<<pinnr;
		}
		else
		{
			IOSET0|=1<<pinnr;
		}
	}
	
	else
	{
		pinnr-=100;
		IODIR1|=1<<pinnr;
		if(pinval==0)
		{
			IOCLR1|=1<<pinnr;
		}
		else
		{
			IOSET1|=1<<pinnr;
		}
	}
}

int pinRead(int pinnr)
{
	if(pinnr<100)
	{
		IODIR0&=~(1<<pinnr);
		return (IOPIN0>>pinnr)&1;
	}
	else
	{
		pinnr-=100;
		IODIR1&=~(1<<pinnr);
		return (IOPIN1>>pinnr)&1;
	}
}
		
