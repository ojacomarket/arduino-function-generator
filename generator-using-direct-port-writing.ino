#include <Waveforms8.h>
#define DAC_RESOLUTION 256 // MCP4802 has 8 bits resolution
#define SQUARE 3
#define SAWTEETH 2
#define TRIANGLE 1
#define SINE 0

const byte MCP4802_REGISTER  = 0b0001; // first bit (channel A or B --> 0 or 1), second bit (always 0), third bit (gain of 1 or 2, 1 or 0), fourth bit (always 1)
byte counter = 0;
volatile byte mcp_register = 0;
void setup() {
  DDRB |= (1 << DDB5) | (1 << DDB3) | (1 << DDB2); // SPI.begin();
  PORTB |= (1 << PORTB2);
  SPCR = 0b01010000;//(1 << SPE) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (8 << SPR0);
}

void loop() {
  mcp_register = WAVEE[SINE][counter];
  PORTB &= ~(1 << PORTB2);
  SPDR = (MCP4802_REGISTER << 4) | (mcp_register >> 4);
  while (!(SPSR & (1 << SPIF)));
  SPDR = (mcp_register >> 4);
  while (!(SPSR & (1 << SPIF)));
  PORTB |= (1 << PORTB2);
  counter = (counter + 1) % 64;
  // change frequency on fly
  /* val = analogRead(1);
    if (val <= 530)
     delayMicroseconds(0);
    else
     delayMicroseconds(val - 530);*/
}
