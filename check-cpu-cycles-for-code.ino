// Set Timer 1 to normal mode at F_CPU.
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
