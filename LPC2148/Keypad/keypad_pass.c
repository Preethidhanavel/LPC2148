#include<lpc21xx.h>
//#include"timer.h"
#include"lcd1.h"
#include<string.h>
#include"uart0.h"
#define C0 (IOPIN0 & (1<<16))
#define C1 (IOPIN0 & (1<<17))
#define C2 (IOPIN0 & (1<<18))
#define C3 (IOPIN0 & (1<<19))
#define R0 1<<20
#define R1 1<<21
#define R2 1<<22
#define R3 1<<23

#define sw 1<<14 

unsigned char key_lut[4][4]={'7','8','9','A','4','5','6','B','1','2','3','C','*','0','#','D'};

unsigned int row_val,col_val;
char d[5];
int i=0;
unsigned char Keypad()
{
	int j;
	char pass_encrypt[5];

	PINSEL1|=0X00000000;
	IODIR0|=R0|R1|R2|R3;

	lcd_command(0X80);
	lcd_string("Enter Password");
	string("PASSWORD IS BEING ENTERD\r\n");
	
		while(1)
	{
		IOCLR0|=R0|R1|R2|R3;
		IOSET0|=C0|C1|C2|C3;

		while((C0&&C1&&C2&&C3)==1);

		IOCLR0|=R0;
		IOSET0|=R1|R2|R3;
		if((C0&&C1&&C2&&C3)==0)
		{
		row_val=0;
		break;
		}

		IOCLR0|=R1;
		IOSET0|=R0|R2|R3;
		if((C0&&C1&&C2&&C3)==0)
		{
		row_val=1;
			break;
		}
		
		IOCLR0|=R2;
		IOSET0|=R1|R0|R3;
		if((C0&&C1&&C2&&C3)==0)
		{
		row_val=2;
			break;
		}

		IOCLR0|=R3;
		IOSET0|=R1|R2|R0;
		if((C0&&C1&&C2&&C3)==0)
		{
		row_val=3;
			break;
		}
}

		if((C0&&C1&&C2&&C3)==0)
		{
		if(C0==0)
			col_val=0;
		else if(C1==0)
			col_val=1;
		else if(C2==0)
			col_val=2;
		else
			col_val=3;
	}

			delay_ms(150);
			while((C0&&C1&&C2&&C3)==0);
			return key_lut[row_val][col_val];
		
}

int main()
{
		
		 IODIR0=1<<2;
     lcd_init();
		 UART0_CONFIG();
		 lcd_command(0X82);
		 lcd_string("***KEYPAD***");
		 delay_ms(500);
		 lcd_command(0X01);
			lcd_command(0Xc0);
			while(i<4){
					d[i]=Keypad();
					lcd_data('*');
					i++;					
			}				
				/*lcd_command(0Xc4);
				lcd_string(d);
				delay_ms(500);
				lcd_command(0X01);*/
			  if (strcmp(d,"9087")==0)
				{
					lcd_command(0Xc0);
					lcd_string("door unlocked");
					string("DOOR OPENED");
					IOSET0=1<<2;
				}
				else
				{
					lcd_command(0Xc0);
					lcd_string("door locked");
					string("DOOR LOCKED OOPS! TRY NEXT TIME");
					IOCLR0=1<<2;
				}
}

