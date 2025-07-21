#include <SPI.h>

volatile byte receivedData = 0;
volatile byte sendData = 0;

void setup() {
  pinMode(MISO, OUTPUT);           // MISO as output
  SPCR |= _BV(SPE);                // Enable SPI (slave mode)
  SPCR &= ~(_BV(CPOL) | _BV(CPHA)); // Mode 0 (CPOL=0, CPHA=0)
  SPCR |= _BV(SPIE);               // Enable SPI interrupt
  Serial.begin(9600);
}

ISR(SPI_STC_vect) {
  receivedData = SPDR;             // Read master's data
  sendData = receivedData + 1;     // Increment by 1
  SPDR = sendData;                 // Prepare next byte to send
}

void loop() {
  // Debugging (optional)
  Serial.print("Received: ");
  Serial.print(receivedData);
  Serial.print(" | Sent: ");
  Serial.println(sendData);
  delay(1000);
}