#include "delay.h"

// Function prototypes
void i2c_eeprom_write(u8 , u8 , u8 );         // Write single byte to EEPROM
u8 i2c_eeprom_read(u8 , u8);                  // Read single byte from EEPROM
void i2c_eeprom_pagewrite(u8 , u8, u8 *, u8 ); // Write multiple bytes (page write)
void i2c_eeprom_seqread(u8 , u8 , u8 *, u8 );  // Read multiple bytes sequentially

// Single byte write
void i2c_eeprom_write(u8 slaveaddr, u8 buffaddr, u8 data)
{
    i2c_start();               // Generate I2C start condition
    i2c_write(slaveaddr << 1); // Send EEPROM slave address with write bit
    i2c_write(buffaddr);       // Send EEPROM memory address
    i2c_write(data);           // Send data byte to write
    i2c_stop();                // Generate I2C stop condition
    delay_ms(10);              // Wait for EEPROM write cycle to complete
}

// Single byte read
u8 i2c_eeprom_read(u8 slaveaddr, u8 buffaddr)
{   
    u8 data;
    i2c_start();                    // Start condition
    i2c_write(slaveaddr << 1);      // Send slave address with write bit
    i2c_write(buffaddr);            // Send memory address to read from
    i2c_restart();                  // Repeated start condition
    i2c_write((slaveaddr << 1) | 1); // Send slave address with read bit
    data = i2c_nack();              // Read data and send NACK
    i2c_stop();                     // Stop condition
    return data;                    // Return the read data
}

// Page write (multiple bytes)
void i2c_eeprom_pagewrite(u8 slaveaddr, u8 buffaddr, u8 *p, u8 nbyte)
{
    u8 i;
    i2c_start();                   // Start condition
    i2c_write(slaveaddr << 1);     // Send slave address with write bit
    i2c_write(buffaddr);           // Send starting memory address
    for(i = 0; i < nbyte; i++){
        i2c_write(p[i]);           // Write each byte in the page
    }
    i2c_stop();                     // Stop condition
    delay_ms(10);                   // Wait for EEPROM write cycle
}

// Sequential read (multiple bytes)
void i2c_eeprom_seqread(u8 slaveaddr, u8 buffaddr, u8 *p, u8 nbyte)
{
    u8 i;
    i2c_start();                    // Start condition
    i2c_write(slaveaddr << 1);      // Send slave address with write bit
    i2c_write(buffaddr);            // Send starting memory address
    i2c_restart();                  // Repeated start condition
    i2c_write((slaveaddr << 1) | 1); // Send slave address with read bit
    for(i = 0; i < nbyte-1; i++){
        p[i] = i2c_mack();          // Read byte and send ACK
    }
    p[i] = i2c_nack();              // Read last byte and send NACK
    i2c_stop();                     // Stop condition
}
