#include "toneAC.h"

#include <avr/interrupt.h>
#include <util/atomic.h>

static PulseToneState pulse_state;


void toneACinit()
{
	// Set prescaler
	PWMT1DREG |= _BV(PWMT1AMASK) | _BV(PWMT1BMASK); // Set timer 1 PWM pins to OUTPUT (because analogWrite does it too).

	uint8_t prescaler = _BV(CS10);                  // Try using prescaler 1 first.
	
	/*
	uint32_t top = F_CPU / frequency / 2 - 1;  // Calculate the top.
	if (top > 65535) {                              // If not in the range for prescaler 1, use prescaler 256 (122 Hz and lower @ 16 MHz).
	  prescaler = _BV(CS12);                        // Set the 256 prescaler bit.
	  top = top / 256 - 1;                          // Calculate the top using prescaler 256.
	}
	*/

	TCCR1B = _BV(WGM13)  | prescaler;                 // Set PWM, phase and frequency corrected (top=ICR1) and prescaler.
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0); // Inverted/non-inverted mode (AC).
}



void toneAC(const uint32_t frequency, const uint8_t volume) {
	if (frequency == NOTONEAC || volume == 0) { noToneAC(); return; }

	toneACinit();
	uint32_t top = F_CPU / frequency / 2 - 1;  // Calculate the top.
	/*
	uint8_t prescaler = _BV(CS10);                  // Try using prescaler 1 first.
	if (top > 65535) {                              // If not in the range for prescaler 1, use prescaler 256 (122 Hz and lower @ 16 MHz).
	  prescaler = _BV(CS12);                        // Set the 256 prescaler bit.
	  top = top / 256 - 1;                          // Calculate the top using prescaler 256.
	}
	*/

	ICR1   = top;                                     // Set the top.
	if (TCNT1 > top) TCNT1 = top;                     // Counter over the top, put within range.
	OCR1A  = OCR1B = top / TONEAC_VOL_10; //  / volume;   // Calculate & set the duty cycle (volume).
}

void toneACsetFrequency(const uint32_t &frequency) {
	uint32_t top = F_CPU / frequency / 2 - 1;  // Calculate the top.
	/*
	uint8_t prescaler = _BV(CS10);                  // Try using prescaler 1 first.
	if (top > 65535) {                              // If not in the range for prescaler 1, use prescaler 256 (122 Hz and lower @ 16 MHz).
	  prescaler = _BV(CS12);                        // Set the 256 prescaler bit.
	  top = top / 256 - 1;                          // Calculate the top using prescaler 256.
	}
	*/

	ICR1   = top;                                     // Set the top.
	if (TCNT1 > top) TCNT1 = top;                     // Counter over the top, put within range.
	OCR1A  = OCR1B = top / TONEAC_VOL_10; // / TONEAC_VOL_1;   // Calculate & set the duty cycle (volume).
}

void noToneAC() {
	TCCR1A  = _BV(WGM10);          // Set to defaults so PWM can work like normal (PWM, phase corrected, 8bit).
	toneAC_MUTE();
	TCCR1B  = _BV(CS11);           // Default clock prescaler of 8.
}

void pulseToneInit() {
	toneACinit();
	TCCR0B |= (1 << CS02);
	// TCCR0B |= (1 << CS02) | (1 << CS00); // pulse freq 4x lower 
	sei();
}


void pulseToneStart() {

	if(!pulse_state.running)
	{
		pulseToneInit();
		TCNT0 = 0;
		TIMSK0 |= (1 << TOIE0);
		pulse_state.beep = 0;
		pulse_state.mute = 1;
		pulse_state.running = 1;
	}
}

void pulseToneStop() {
	if(pulse_state.running) 
	{
		TIMSK0 &= ~(1 << TOIE0);
		noToneAC();
	}
	pulse_state.running = 0;
}

void pulseToneSet(const uint32_t &frequency, const uint16_t beep_len, const uint16_t mute_len) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		pulse_state.tone_frequency = frequency;
		pulse_state.beep_len = beep_len;
		pulse_state.mute_len = mute_len;
		if(pulse_state.beep) toneACsetFrequency(pulse_state.tone_frequency);
		//toneACsetFrequency(pulse_state.tone_frequency);
		if(pulse_state.beep > beep_len) pulse_state.beep = beep_len;
		if(pulse_state.mute > mute_len) pulse_state.mute = mute_len;
		
	}
}

ISR(TIMER0_OVF_vect) {
	if(pulse_state.beep) {
		if(pulse_state.beep == 1){
			toneAC_MUTE();
			pulse_state.mute = pulse_state.mute_len;
		}
		pulse_state.beep--;
	}
	else if (pulse_state.mute) {
		if(pulse_state.mute == 1) {
			toneACsetFrequency(pulse_state.tone_frequency);
			toneAC_PLAY();
			pulse_state.beep = pulse_state.beep_len;
		}
		pulse_state.mute--;
	}
	else if(!pulse_state.beep && !pulse_state.mute) pulse_state.beep = pulse_state.beep_len;
}