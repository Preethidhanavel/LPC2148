#define scl_pin 0X00000010
#define sda_pin 0X00000040

#define cclk 60000000
#define pclk cclk/4
#define i2c_speed 100000
#define loadval ((pclk/i2c_speed)/2)
#define i2c_en 6
#define sta_bit 5
#define si_bit 3
#define sto_bit 4
#define aa_bit 2

typedef unsigned int u32;
typedef signed int s32;
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

void i2c_init(void);
void i2c_start(void);
void i2c_write(u8 );
void i2c_restart(void);
u8 i2c_nack(void);
void i2c_stop(void);   
u8 i2c_mack(void);

void i2c_init(void)
{
		PINSEL0|=scl_pin|sda_pin;
		I2SCLL=loadval;
		I2SCLH=loadval;
	//	I2ADR=0XA0;
		I2CONSET=1<<i2c_en;
}

void i2c_start(void)
{
		I2CONSET=1<<sta_bit;
		while(((I2CONSET>>si_bit)&1)==0);
		I2CONCLR= 1<<sta_bit;
}

void i2c_restart(void)
{
   I2CONSET=1<<sta_bit;
   I2CONCLR=1<<si_bit;
   while(((I2CONSET>>si_bit)&1)==0);
   I2CONCLR=1<<sta_bit;
}

void i2c_write(u8 s)
{
		I2DAT=s;
		I2CONCLR=1<<si_bit;
		while(((I2CONSET>>si_bit)&1)==0);
}

void i2c_stop(void)
{
		I2CONSET=1<<sto_bit;
		I2CONCLR=1<<si_bit;
}

u8 i2c_nack(void)
{		 
		I2CONSET = 0x00; 
		I2CONCLR = 1<<si_bit;
		while(((I2CONSET>>si_bit)&1)==0);
		return I2DAT;
}
u8 i2c_mack(void)
{
		I2CONSET=1<<aa_bit;
		I2CONCLR=1<<si_bit;
		while(((I2CONSET>>si_bit)&1)==0);
		I2CONCLR=1<<aa_bit;
		return I2DAT;
}
u8 i2c_read_nack(void)
{
    I2CONCLR = 1<<aa_bit;    // Clear AA
    I2CONCLR = 1<<si_bit;    // Clear SI
    while((I2CONSET & (1<<si_bit)) == 0);  // Wait till SI set
    return I2DAT;
}
