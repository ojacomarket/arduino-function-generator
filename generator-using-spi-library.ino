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

// 16 bit MCP4802 register (4 config bits -- 8 data bits -- 4 offset bits <<4)
const unsigned int MCP4802_REGISTER  = 0B0001000000000000; // first bit (channel A or B --> 0 or 1), second bit (always 0), third bit (gain of 1 or 2, 1 or 0), fourth bit (always 1)
byte waveform = SINE;
byte counter = 0;
unsigned int mcp_register = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LDAC_PIN, OUTPUT);    // sets the digital pin 7 as input
  pinMode(CS_PIN, OUTPUT);    // sets the digital pin 6 as input
  digitalWrite(LDAC_PIN, LOW); //
  digitalWrite(CS_PIN, HIGH); //
  pinMode(D8, INPUT);    // sets the digital pin 7 as input
  pinMode(D7, INPUT);    // sets the digital pin 6 as input
  pinMode(D6, INPUT);    // sets the digital pin 5 as input
  pinMode(D5, INPUT);    // sets the digital pin 4 as input
  SPI.begin();
}

void loop()
{
  waveform = choose_waveform(waveform);
  mcp_register = (unsigned int)WAVEFORMS[waveform][counter];
  /*if ((mcp_register << 4) > 4080) {
    Serial.println("Hello");
  } else {
    Serial.println("lol");
  }*/
  Serial.println((MCP4802_REGISTER | (mcp_register << 4)), BIN);
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_PIN, LOW);
  SPI.transfer16(MCP4802_REGISTER | mcp_register << 4);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  counter++;
  //Serial.println(analogRead(A0));
}
/*
   Choose a waveform among square, sawteeth, triangle and sine (default)
   return: index of waveform to be chosen (for details lookup for Waveform8.h)
*/
byte choose_waveform(byte current) {
  if (digitalRead(D8))
    return SQUARE;
  if (digitalRead(D7))
    return SAWTEETH;
  if (digitalRead(D6))
    return TRIANGLE;
  if (digitalRead(D5))
    return SINE;
  else
    return current;
}
