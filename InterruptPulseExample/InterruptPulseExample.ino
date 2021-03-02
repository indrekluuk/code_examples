// 1/16Mhz = 0.0625us, prescaler = 64
// Pulse time = period * 64 * 0.0625us
volatile int period = 25; // Pulse time = 25 * 64 * 0.0625 = 100us

void setup() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0; //initialize counter value to 0

  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10);    // 64 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  OCR1A = period - 1;
  interrupts();

  pinMode(8, OUTPUT);
}

ISR(TIMER1_COMPA_vect) {
  PORTB = 0b00000001;
  delayMicroseconds(5);
  PORTB = 0b00000000;
  OCR1A = period - 1;
}

int lastPotVal = 0;
void loop() {
  int potVal = analogRead(A0);
  if ((abs(potVal - lastPotVal) > 5)) {
    lastPotVal = potVal;

    // Pulse time = period * 64 * 0.0625us. (1/16Mhz = 0.0625us, prescaler = 64)
    // Minimum time = 25 * 64 * 0.0625us = 100us
    // Maximum time = 125 * 64 * 0.0625us = 500us
    int newPeriod = map(potVal, 0, 1023, 25, 125);
    
    noInterrupts();
    period = newPeriod;
    interrupts();
  }
}
