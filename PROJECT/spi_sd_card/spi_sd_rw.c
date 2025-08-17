#include <lpc214x.h>
#include <stdint.h>
#include <string.h>
#include "lcd1.h"  
#include "uart0.h"

#define CMD0   (0x40+0)   // GO_IDLE_STATE
#define CMD17  (0x40+17)  // READ_SINGLE_BLOCK
#define CMD24  (0x40+24)  // WRITE_SINGLE_BLOCK
#define DUMMY  0xFF       // Dummy byte for SPI

//---------------- SPI INITIALIZATION ----------------//
void SPI_Init(void) 
{
    PINSEL0 |= 0x00001500;       // Select P0.4 (SCK), P0.5 (MISO), P0.6 (MOSI)
    IO0DIR |= (1 << 7);          // P0.7 as SSEL (Chip Select)  output
    IO0SET = (1 << 7);           // Keep SSEL high (inactive)
    S0SPCR = (1 << 5) | (1 << 2); // Enable SPI in Master mode
    S0SPCCR = 8;                 // SPI clock prescaler (must be even >= 8)
}

// Send & Receive one byte over SPI
uint8_t SPI_Transfer(uint8_t data) 
{
    S0SPDR = data;                     // Write data
    while (!(S0SPSR & (1 << 7)));      // Wait until transfer is complete
    return S0SPDR;                     // Return received byte
}

//---------------- SD CARD COMMAND FUNCTIONS ----------------//

// Send command (6 bytes total)
void SD_Command(uint8_t cmd, uint32_t arg, uint8_t crc) 
{
    SPI_Transfer(cmd);
    SPI_Transfer(arg >> 24);
    SPI_Transfer(arg >> 16);
    SPI_Transfer(arg >> 8);
    SPI_Transfer(arg);
    SPI_Transfer(crc);
}

// Wait for R1 response from SD
uint8_t SD_WaitR1(void) 
{
    uint8_t res;
    int i;
    for (i = 0; i < 100; i++) {
        res = SPI_Transfer(0xFF);
        if ((res & 0x80) == 0) return res; // Valid response
    }
    return 0xFF; // Timeout
}

// Wait until card is not busy (sends 0xFF)
void SD_WaitNotBusy() 
{
    while (SPI_Transfer(0xFF) != 0xFF);
}

//---------------- SD INITIALIZATION ----------------//

int SD_Init(void) 
{
    int i;
    IO0SET = (1 << 7);              // CS high
    for (i = 0; i < 10; i++) SPI_Transfer(0xFF); // Send 80 clocks (dummy)

    IO0CLR = (1 << 7);              // CS low
    SD_Command(CMD0, 0, 0x95);      // Send CMD0 with correct CRC
    if (SD_WaitR1() != 0x01) {      // Expect idle response
        IO0SET = (1 << 7);
        return -1;                  // Fail
    }
    IO0SET = (1 << 7);
    SPI_Transfer(0xFF);
    return 0;                       // Success
}

//---------------- SD READ ----------------//

int SD_ReadBlock(uint8_t *buf, uint32_t addr) 
{
    uint8_t token;
    int i;

    IO0CLR = (1 << 7);              // CS low
    SD_Command(CMD17, addr, 0xFF);  // Read single block

    if (SD_WaitR1() != 0x00) {      // Expect R1 = 0 (OK)
        IO0SET = (1 << 7);
        return -1;
    }

    // Wait for data token (0xFE)
    for (i = 0; i < 1000; i++) {
        token = SPI_Transfer(0xFF);
        if (token == 0xFE) break;
    }
    if (token != 0xFE) {
        IO0SET = (1 << 7);
        return -2;                  // No data token
    }

    // Read 512 bytes of data
    for (i = 0; i < 512; i++) 
        buf[i] = SPI_Transfer(0xFF);

    SPI_Transfer(0xFF);             // Ignore CRC
    SPI_Transfer(0xFF);

    IO0SET = (1 << 7);              // CS high
    SPI_Transfer(0xFF);             // Extra clock
    return 0;                       // Success
}

//---------------- SD WRITE ----------------//

uint8_t SD_WriteBlock(uint8_t *buf, uint32_t addr) 
{
    int i;
    uint8_t response;

    IO0CLR = (1 << 7);              // CS low
    SD_Command(CMD24, addr, 0xFF);  // Write single block

    if (SD_WaitR1() != 0x00) {      // Expect R1 = 0
        IO0SET = (1 << 7);
        return 1;                   // Command fail
    }

    SPI_Transfer(0xFE);             // Send start token

    // Write 512 bytes
    for (i = 0; i < 512; i++) 
        SPI_Transfer(buf[i]);

    SPI_Transfer(0xFF);             // Dummy CRC
    SPI_Transfer(0xFF);

    response = SPI_Transfer(0xFF);  // Get data response
    if ((response & 0x1F) != 0x05) {
        IO0SET = (1 << 7);
        return 2;                   // Write fail
    }

    SD_WaitNotBusy();               // Wait till card finishes write
    IO0SET = (1 << 7);              // CS high
    SPI_Transfer(0xFF);
    return 0;                       // Success
}

//---------------- MAIN FUNCTION ----------------//

int main(void) 
{
    int i;
    uint8_t write_buf[512], read_buf[512];

    lcd_init();
    SPI_Init();
    UART0_CONFIG();

    lcd_string("Init SD...");
    string("INIT SDCARD...");

    if (SD_Init()) {                      // Initialize SD card
        lcd_command(0xC0);
        lcd_string("Init Fail");
        string("FAILED TO OPEN SD");
        while (1);                        // Stop on failure
    }

    lcd_command(0xC0);
    lcd_string("Writing block...");
    string("WRITING INTO SD\n");

    // Fill write buffer with pattern 'A'..'Z'
    for (i = 0; i < 512; i++) 
        write_buf[i] = 'A' + (i % 26);

    // Write one block to address 0x00000000
    if (SD_WriteBlock(write_buf, 0x00000000) != 0) {
        lcd_command(0xC0);
        lcd_string("Write Fail");
        string("WRITE FAIL");
        while (1);
    }

    delay_ms(1000);
    lcd_command(0x01);
    lcd_string("Reading block...");
    string("READING BLOCK...");

    // Read back block
    if (SD_ReadBlock(read_buf, 0x00000000) != 0) {
        lcd_command(0xC0);
        lcd_string("Read Fail");
        string("READ FAIL");
        while (1);
    }

    read_buf[511] = '\0';                 // Null terminate buffer

    lcd_command(0xC0);                    // Display first few chars
    lcd_data(read_buf[0]);
    lcd_data(read_buf[1]);
    lcd_data(read_buf[2]);

    string(read_buf);                     // Send entire block over UART

    while (1);                            // Infinite loop
}
