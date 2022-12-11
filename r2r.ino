
#include <Waveforms8.h>
#include <Waveforms6.h>
#define DAC_RESOLUTION 256 // MCP4802 has 8 bits resolution
#define SQUARE 3
#define SAWTEETH 2
#define TRIANGLE 1
#define SINE 0

const byte MCP4802_REGISTER  = 0b0001; // first bit (channel A or B --> 0 or 1), second bit (always 0), third bit (gain of 1 or 2, 1 or 0), fourth bit (always 1)
byte counter = 0;
unsigned int val = 0;
volatile byte digital_voltage = 0;

void init_adc() {
  // AVcc reference, ADC0, and left adjust the result
  ADMUX = (1 << REFS0) | (1 << ADLAR);

  // Enable the ADC in freerunning mode with a /64 prescaler
  ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1);

  ADCSRB = 0x00;
}
void setup() {
  Serial.begin(9600);
  TCCR1A = 0;
  TCCR1B = 1;
  cli();
  uint16_t start = TCNT1;

  // put your code here

  uint16_t finish = TCNT1;
  sei();
  uint16_t overhead = 8;
  uint16_t cycles = finish - start - overhead;
  Serial.print("program took ");
  Serial.print(cycles);
  Serial.println(" CPU cycles.");
 // DDRD = B00111111; //D5-D0 output
  // init_adc();
  /*DDRB |= (1 << DDB5) | (1 << DDB3) | (1 << DDB2); // SPI.begin();
    PORTB |= (1 << PORTB2);
    SPCR = 0b01010000;//(1 << SPE) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (8 << SPR0);*/
}

void loop() {
  digital_voltage = WAVEFORMS_6b[SINE][counter];
  PORTD = digital_voltage;
  /*mcp_register = WAVEFORMS[SINE][counter];
    PORTB &= ~(1 << PORTB2);
    SPDR = (MCP4802_REGISTER << 4) | (mcp_register >> 4);
    while (!(SPSR & (1 << SPIF)));
    SPDR = (mcp_register << 4); // dont mess ith >>4
    while (!(SPSR & (1 << SPIF)));
    PORTB |= (1 << PORTB2);*/
  counter = (counter + 1) % 256;
  // change frequency on fly
  /* val = analogRead(1);
    if (val > 530)
     delayMicroseconds(val - 530);*/
  // Serial.println(digital_voltage);
  // Serial.println(ADC);
}

ISR(ADC_vect) {
  Serial.println("Hello");
}
