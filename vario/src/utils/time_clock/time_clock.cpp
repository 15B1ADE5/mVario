#include "time_clock.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static uint32_t time_count = 0;

void time_clock_init()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		// Set prescaler to 64
		TCCR2A |= (1 << WGM21);
		TCCR2B |= (1 << CS22);

		TIMSK2 |= (1 << OCIE2A);
		
		// Set compare match value
		OCR2A = 124; // 8MHz/(64*(1+124) = 1kHz (1 ms period)

		// Reset Timer2 counter 
		TCNT2 = 0;
		// Reset us counter
		time_count = 0;

		sei();
	}
}

void timer_reset()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		time_count = 0;
	}
}

uint32_t timer_get_reset()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		uint32_t time_count_out = time_count;
		time_count = 0;
		return time_count_out;
	}
}

uint32_t timer_get()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		return time_count;
	}
}

ISR(TIMER2_COMPA_vect) {
	time_count++;
}