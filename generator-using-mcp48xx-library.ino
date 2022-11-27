#include <MCP48xx.h> // Will include SPI.
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

byte waveform = SINE;
MCP4802 mcp4802(CS_PIN);

void setup() {
  digitalWrite(LDAC_PIN, LOW); // only if LDAC is LOW our MCP4802 will work
  Serial.begin(9600);
  mcp4802.init(); // must have
  mcp4802.setGainA(MCP4802::High); // Output voltage is 4.08 V always!
  mcp4802.turnOnChannelA(); // Channel A is 4.08 V, channel B is 0 V
  mcp4802.setVoltageA(0); // Clear voltage is was there
  mcp4802.updateDAC(); // Send signal to DAC
  pinMode(D8, INPUT);    // sets the digital pin 7 as input
  pinMode(D7, INPUT);    // sets the digital pin 6 as input
  pinMode(D6, INPUT);    // sets the digital pin 5 as input
  pinMode(D5, INPUT);    // sets the digital pin 4 as input
}

void loop()
{
  for (unsigned int i = 0; i < DAC_RESOLUTION; i++)
  {
    waveform = choose_waveform(waveform);
    mcp4802.setVoltageA((unsigned int)WAVEFORMS[waveform][i]);
    mcp4802.updateDAC();
    delayMicroseconds(map(analogRead(1), 512, 1023, 0, 5000)); // frequency control
    // Serial.println(analogRead(A0));
  }
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
