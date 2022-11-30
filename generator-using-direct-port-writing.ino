#include <SPI.h>
#include <Waveforms8.h>
#define CS_PIN 10 // PIN on Arduino must be labeled as SS and 10 means that MCP4802 will connect to Arduino on pin 10
#define LDAC_PIN 9 // any digital pin to send LDAC signal
#define DAC_RESOLUTION 256 // MCP4802 has 8 bits resolution
#define D8 8 // square wave button
#define D7 7 // sawteeth wave button
#define D6 6 // triangle wave button
#define D5 5 // square wave button
#define SQUARE 3
#define SAWTEETH 2
#define TRIANGLE 1
#define SINE 0


#define DATAOUT 11//COPI
#define DATAIN  12//CIPO
#define SPICLOCK  13//sck
#define CHIPSELECT 10//cs


//opcodes
#define WREN  6
#define WRDI  4
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2

byte eeprom_output_data;
byte eeprom_input_data = 0;
byte clr;
int address = 0;

const byte MCP4802_REGISTER  = 0b0001; // first bit (channel A or B --> 0 or 1), second bit (always 0), third bit (gain of 1 or 2, 1 or 0), fourth bit (always 1)
byte waveform = SINE;
byte sinus = 0;
byte counter = 0;
unsigned int val;
volatile byte mcp_register = 0;
const volatile byte WAVEE[2][64] = {
  {
    //saw
    0x00, 0x04,
    0x08, 0x0C,
    0x10, 0x14,
    0x18, 0x1F,
    0x20, 0x24,
    0x28, 0x2F,
    0x30, 0x34,
    0x38, 0x3C,
    0x40, 0x44,
    0x48, 0x4C,
    0x50, 0x54,
    0x58, 0x5F,
    0x60, 0x67,
    0x68, 0x6F,
    0x70, 0x77,
    0x78, 0x7F,
    0x80, 0x84,
    0x88, 0x8F,
    0x90, 0x94,
    0x98, 0x9C,
    0xA0, 0xA4,
    0xA8, 0xAC,
    0xB0, 0xB4,
    0xB8, 0xBC,
    0xC0, 0xC4,
    0xC8, 0xCC,
    0xD0, 0xD4,
    0xD8, 0xDC,
    0xE0, 0xE4,
    0xE8, 0xEC,
    0xF0, 0xF4,
    0xF8, 0xFF
  },
  {
    0x00, 0x08,
    0x10, 0x18,
    0x20, 0x28,
    0x30, 0x38,
    0x40, 0x48,
    0x50, 0x58,
    0x60, 0x68,
    0x70, 0x78,
    0x80, 0x88,
    0x90, 0x98,
    0xA0, 0xA8,
    0xB0, 0xB8,
    0xC0, 0xC8,
    0xD0, 0xD8,
    0xE0, 0xE8,
    0xF0, 0xF8,
    0xFF, 0xF6,
    0xEE, 0xE6,
    0xDE, 0xD6,
    0xCE, 0xC6,
    0xBE, 0xB6,
    0xAE, 0xA6,
    0x9E, 0x96,
    0x8E, 0x86,
    0x7E, 0x76,
    0x6E, 0x66,
    0x5E, 0x56,
    0x4E, 0x46,
    0x3E, 0x36,
    0x2E, 0x26,
    0x1E, 0x16,
    0x0E, 0x06
  }
};
//MCP4802 mcp4802(CS_PIN);
//unsigned int mcp_register = 0;
void setup() {
  Serial.begin(9600);
  DDRB |= (1 << DDB5) | (1 << DDB3) | (1 << DDB2); // SPI.begin();
  PORTB |= (1 << PORTB2);
  SPCR = 0b01010000;//(1 << SPE) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (8 << SPR0);
}

void loop() {
  mcp_register = WAVEE[1][counter];
  PORTB &= ~(1 << PORTB2);
  SPDR = (MCP4802_REGISTER << 4) | (mcp_register >> 4);
  while (!(SPSR & (1 << SPIF)));
  SPDR = (mcp_register >> 4);
  while (!(SPSR & (1 << SPIF)));
  PORTB |= (1 << PORTB2);
  counter = (counter + 1) % 64;
  /* val = analogRead(1);
    if (val <= 530)
     delayMicroseconds(0);
    else
     delayMicroseconds(val - 530);*/
}
