#include "toneAC.h"

#include <avr/interrupt.h>
#include <util/atomic.h>

static PulseToneState pulse_state;

void toneAC(const uint32_t frequency, const uint8_t volume) {
  if (frequency == NOTONEAC || volume == 0) { noToneAC(); return; }

  PWMT1DREG |= _BV(PWMT1AMASK) | _BV(PWMT1BMASK); // Set timer 1 PWM pins to OUTPUT (because analogWrite does it too).

  uint8_t prescaler = _BV(CS10);                  // Try using prescaler 1 first.
  uint32_t top = F_CPU / frequency / 2 - 1;  // Calculate the top.
  if (top > 65535) {                              // If not in the range for prescaler 1, use prescaler 256 (122 Hz and lower @ 16 MHz).
    prescaler = _BV(CS12);                        // Set the 256 prescaler bit.
    top = top / 256 - 1;                          // Calculate the top using prescaler 256.
  }

  ICR1   = top;                                     // Set the top.
  if (TCNT1 > top) TCNT1 = top;                     // Counter over the top, put within range.
  TCCR1B = _BV(WGM13)  | prescaler;                 // Set PWM, phase and frequency corrected (top=ICR1) and prescaler.
  OCR1A  = OCR1B = top / volume;   // Calculate & set the duty cycle (volume).
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0); // Inverted/non-inverted mode (AC).
}

void toneACopt(const uint32_t &frequency) {
  PWMT1DREG |= _BV(PWMT1AMASK) | _BV(PWMT1BMASK); // Set timer 1 PWM pins to OUTPUT (because analogWrite does it too).

  uint8_t prescaler = _BV(CS10);                  // Try using prescaler 1 first.
  uint32_t top = F_CPU / frequency / 2 - 1;  // Calculate the top.
  if (top > 65535) {                              // If not in the range for prescaler 1, use prescaler 256 (122 Hz and lower @ 16 MHz).
    prescaler = _BV(CS12);                        // Set the 256 prescaler bit.
    top = top / 256 - 1;                          // Calculate the top using prescaler 256.
  }

  ICR1   = top;                                     // Set the top.
  if (TCNT1 > top) TCNT1 = top;                     // Counter over the top, put within range.
  TCCR1B = _BV(WGM13)  | prescaler;                 // Set PWM, phase and frequency corrected (top=ICR1) and prescaler.
  OCR1A  = OCR1B = top / TONEAC_VOL_10;   // Calculate & set the duty cycle (volume).
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0); // Inverted/non-inverted mode (AC).
}

void noToneAC() {
  //TIMSK1 &= ~_BV(OCIE1A);        // Remove the timer interrupt.
  TCCR1B  = _BV(CS11);           // Default clock prescaler of 8.
  TCCR1A  = _BV(WGM10);          // Set to defaults so PWM can work like normal (PWM, phase corrected, 8bit).
  PWMT1PORT &= ~_BV(PWMT1AMASK); // Set timer 1 PWM pins to LOW.
  PWMT1PORT &= ~_BV(PWMT1BMASK); // Other timer 1 PWM pin also to LOW.
}

void pulseToneInit() {
  TCCR0B |= (1 << CS02);
  // TCCR0B |= (1 << CS02) | (1 << CS00); // pulse freq 4x lower
  TCNT0 = 0;
  sei();

  // To stop timer:
  // TCCR0B &= ~(1 << CS02) & ~(1 << CS00) & ~(1 << CS00);
}

void pulseToneStart() {
  TIMSK0 |= (1 << TOIE0);
}

void pulseToneStop() {
  TIMSK0 &= ~(1 << TOIE0);
  noToneAC();
}

void pulseToneSet(const uint32_t &frequency, const uint16_t beep_len, const uint16_t mute_len) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pulse_state.tone_frequency = frequency;
    pulse_state.beep_len = beep_len;
    pulse_state.mute_len = mute_len;
  }
}

ISR(TIMER0_OVF_vect) {
  if(pulse_state.beep) {
    if(pulse_state.beep == 1){
      noToneAC();
      pulse_state.mute = pulse_state.mute_len;
    }
    pulse_state.beep--;
  }
  else if (pulse_state.mute) {
    if(pulse_state.mute == 1){
      toneAC(pulse_state.tone_frequency);
      pulse_state.beep = pulse_state.beep_len;
    }
    pulse_state.mute--;
  }
  else if(!pulse_state.beep && !pulse_state.mute) pulse_state.beep = pulse_state.beep_len;
}