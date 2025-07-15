#include"timer.h"
#define lcd_d 0x0f<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19
void lcd_init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);



void lcd_init()
{
				IODIR1|=lcd_d|rs|rw|e;
				lcd_command(0x01);
				lcd_command(0x02);
				lcd_command(0x0c);
				lcd_command(0x28);
				lcd_command(0x80);
}

void lcd_command(unsigned char cmd)
{
				IOCLR1=lcd_d;
				IOSET1=(cmd&0Xf0)<<16;
				IOCLR1=rs;
				IOCLR1=rw;
				IOSET1=e;
				delay_ms(2);
				IOCLR1=e;
				
				IOCLR1=lcd_d;
				IOSET1=(cmd&0X0f)<<20;
				IOCLR1=rs;
				IOCLR1=rw;
				IOSET1=e;
				delay_ms(2);
				IOCLR1=e;
}

void lcd_data(unsigned char d)
{				
				IOCLR1=lcd_d;
				IOSET1=(d&0Xf0)<<16;
				IOSET1=rs;
				IOCLR1=rw;
				IOSET1=e;
				delay_ms(2);
				IOCLR1=e;
				
				IOCLR1=lcd_d;
				IOSET1=(d&0X0f)<<20;
				IOSET1=rs;
				IOCLR1=rw;
				IOSET1=e;
				delay_ms(2);
				IOCLR1=e;
}
void lcd_int(int a)
{
	unsigned char b[10];
	int i=0;
	if(a==0)
		lcd_data(0+48);
	else
		{
			if(a<0)
			{
				lcd_data('-');
				a=-a;
			}
			while(a>0)
			{
				b[i++]=a%10;
				a/=10;
			}
	for(i--;i>=0;i--)
		lcd_data(b[i]+48);
		}
}

void lcd_string(char *s)
{
	while(*s)
		lcd_data(*s++);
}

void lcd_float(float f)
{
	int temp;
	temp=f;
	lcd_int(temp);
	lcd_data('.');
	temp=(f-temp)*1000;
	lcd_int(temp);
}
