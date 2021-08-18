#ifndef TIME_CLOCK_H
#define TIME_CLOCK_H

#include <util/delay.h>

#define delay_ms(ms)                \
{                                   \
	uint32_t _delay_ms_counter = ms; \
	while (_delay_ms_counter--)      \
	{                                \
		_delay_ms(1);                 \
	}                                \
}

void time_clock_init();

void timer_reset();
uint32_t timer_get_reset();
uint32_t timer_get();

#endif // TIME_CLOCK_H