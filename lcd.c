#include<lpc21xx.h>

#define lcd_d 0xff
#define rs 1<<8
#define e 1<<10
#define rw 1<<9

void lcd_init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void delay_ms(unsigned int n)
{
	int i,j;
	for(i=0;i<n;i++)
	for(j=0;j<12000;j++);
}
int main()
{
	unsigned char *a="Embedded System";
	lcd_init();
	lcd_command(0X80);
	while(*a)
		lcd_data(*a++);

}
void lcd_init()
{
	IODIR0=lcd_d|rs|e|rw;
	IOCLR0=rw;
	lcd_command(0x01);
	lcd_command(0x02);
	lcd_command(0x0c);
	lcd_command(0x38);
	lcd_command(0x80);
}

	
void lcd_command(unsigned char cmd)
{
	IOCLR0=lcd_d;
	IOSET0=cmd;
	IOCLR0=rs;//command register
	IOSET0=e;
	delay_ms(2);
	IOCLR0=e;
}
void lcd_data(unsigned char d)
{
	IOCLR0=lcd_d;
	IOSET0=d;
	IOSET0=rs;//data register
	IOSET0=e;
	delay_ms(2);
	IOCLR0=e;
}
