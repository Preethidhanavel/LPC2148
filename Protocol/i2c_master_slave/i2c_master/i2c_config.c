// I2C pin definitions
#define scl_pin 0X00000010   // P0.4 as SCL
#define sda_pin 0X00000040   // P0.6 as SDA

// Clock settings
#define cclk 60000000        // CPU Clock
#define pclk cclk/4          // Peripheral Clock
#define i2c_speed 100000     // I2C speed in Hz
#define loadval ((pclk/i2c_speed)/2) // Clock divider for SCL

// I2C control bits
#define i2c_en 6
#define sta_bit 5
#define si_bit 3
#define sto_bit 4
#define aa_bit 2

// Data types
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

// Function prototypes
void i2c_init(void);
void i2c_start(void);
void i2c_write(u8);
void i2c_restart(void);
u8 i2c_nack(void);
void i2c_stop(void);   
u8 i2c_mack(void);
u8 i2c_read_nack(void);

// Initialize I2C peripheral
void i2c_init(void)
{
    PINSEL0 |= scl_pin | sda_pin; // Configure pins for I2C function
    I2SCLL = loadval;             // Set SCL low time
    I2SCLH = loadval;             // Set SCL high time
    I2CONSET = 1 << i2c_en;       // Enable I2C
}

// Generate I2C start condition
void i2c_start(void)
{
    I2CONSET = 1 << sta_bit;                 // Set START bit
    while(((I2CONSET >> si_bit) & 1) == 0); // Wait until SI flag set
    I2CONCLR = 1 << sta_bit;                 // Clear START bit
}

// Generate I2C repeated start
void i2c_restart(void)
{
    I2CONSET = 1 << sta_bit;                 
    I2CONCLR = 1 << si_bit;                 
    while(((I2CONSET >> si_bit) & 1) == 0); 
    I2CONCLR = 1 << sta_bit;                
}

// Write a byte to I2C bus
void i2c_write(u8 s)
{
    I2DAT = s;                               // Load data into I2DAT
    I2CONCLR = 1 << si_bit;                  // Clear SI flag
    while(((I2CONSET >> si_bit) & 1) == 0); // Wait until transmission complete
}

// Generate I2C stop condition
void i2c_stop(void)
{
    I2CONSET = 1 << sto_bit;  // Set STOP bit
    I2CONCLR = 1 << si_bit;   // Clear SI flag
}

// Read a byte from I2C and send NACK
u8 i2c_nack(void)
{
    I2CONSET = 0x00;                  
    I2CONCLR = 1 << si_bit;           
    while(((I2CONSET >> si_bit) & 1) == 0); // Wait until data received
    return I2DAT;                        // Return data
}

// Read a byte from I2C and send ACK
u8 i2c_mack(void)
{
    I2CONSET = 1 << aa_bit;                
    I2CONCLR = 1 << si_bit;                
    while(((I2CONSET >> si_bit) & 1) == 0);
    I2CONCLR = 1 << aa_bit;                
    return I2DAT;                          
}

// Read a byte from I2C without sending ACK (NACK)
u8 i2c_read_nack(void)
{
    I2CONCLR = 1 << aa_bit;                // Clear AA to send NACK
    I2CONCLR = 1 << si_bit;                // Clear SI flag
    while((I2CONSET & (1 << si_bit)) == 0); // Wait till SI set
    return I2DAT;                          // Return received data
}
