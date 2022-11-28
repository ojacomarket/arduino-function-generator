#include <SPI.h>
#include <Waveforms8.h>
#define CS_PIN 10 // PIN on Arduino must be labeled as SS and 10 means that MCP4802 will connect to Arduino on pin 10
#define LDAC_PIN 9 // any digital pin to send LDAC signal
#define DAC_RESOLUTION 256 // MCP4802 has 8 bits resolution
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
  cli();//stop interrupts

  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 532;// = (16e-6) / (1*2) - 1
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // CS10 bits for 2 prescaler
  TCCR1B |= (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
  Serial.begin(9600);
  pinMode(LDAC_PIN, OUTPUT);    // sets the digital pin 7 as input
  pinMode(CS_PIN, OUTPUT);    // sets the digital pin 6 as input
  digitalWrite(LDAC_PIN, LOW); //
  digitalWrite(CS_PIN, HIGH); //
  SPI.begin();
}

void loop() {}

ISR(TIMER1_COMPA_vect) {
  mcp_register = (unsigned int)WAVEFORMS[SINE][counter];
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_PIN, LOW);
  SPI.transfer16(MCP4802_REGISTER | mcp_register << 4);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  counter++;
  // Serial.println(analogRead(A0));
}
