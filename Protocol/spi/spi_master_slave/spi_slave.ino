#include <SPI.h>

volatile byte receivedData = 0;  // Variable to store data received from master
volatile byte sendData = 0;      // Variable to store data to send back to master

void setup() {
  pinMode(MISO, OUTPUT);          // Set MISO pin as output (slave sends data here)
  
  SPCR |= _BV(SPE);               // Enable SPI in slave mode
  SPCR &= ~(_BV(CPOL) | _BV(CPHA)); // Set SPI mode 0 (CPOL=0, CPHA=0)
  SPCR |= _BV(SPIE);              // Enable SPI interrupt (to handle incoming data automatically)

  Serial.begin(9600);             // Initialize Serial for debugging
}

// SPI interrupt routine called when a byte is received from master
ISR(SPI_STC_vect) {
  receivedData = SPDR;            // Read data sent by master
  sendData = receivedData + 1;    // Prepare response (increment received value)
  SPDR = sendData;                // Load data to SPI data register to send back to master
}

void loop() {
  // Optional debugging: print received and sent data every 1 second
  Serial.print("Received: ");
  Serial.print(receivedData);
  Serial.print(" | Sent: ");
  Serial.println(sendData);
  delay(1000);
}
