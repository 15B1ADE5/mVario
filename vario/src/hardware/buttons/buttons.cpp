#include "buttons.h"

#include <avr/io.h>

void btn_init()
{
	// Set Pull-Up's
	PORTD |= BTN_A_BIT | BTN_B_BIT | BTN_C_BIT;

	// Set inputs
	DDRD |= BTN_A_BIT | BTN_B_BIT | BTN_C_BIT;
}


#define BTN_DEBOUNCE(suffix)                                           \
{                                                                      \
	if(btn_state.btn_ ## suffix != current_btn_state.btn_ ## suffix)    \
	{                                                                   \
		count_ ## suffix++;                                              \
		if (count_ ## suffix >= BTN_DEBOUNCE_COUNT)                      \
		{                                                                \
			btn_state.btn_ ## suffix = current_btn_state.btn_ ## suffix;  \
			count_ ## suffix = 0;                                         \
		}                                                                \
	}                                                                   \
	else                                                                \
	{                                                                   \
		count_ ## suffix = 0;                                            \
	}                                                                   \
}

BTNstatus debounce_btn_read()
{
	static BTNstatus btn_state{ .raw= 0};

	static uint8_t count_a = 0;
	static uint8_t count_b = 0;
	static uint8_t count_c = 0;

	uint8_t current_state = ~(PIND);
	BTNstatus current_btn_state{ .raw= 0};
	
	current_btn_state.btn_a = bool(current_state & BTN_A_BIT);
	current_btn_state.btn_b = bool(current_state & BTN_B_BIT);
	current_btn_state.btn_c = bool(current_state & BTN_C_BIT);

	BTN_DEBOUNCE(a);
	BTN_DEBOUNCE(b);
	BTN_DEBOUNCE(c);

	return btn_state;
}



#define BTN_DELAY(suffix)                                              \
{                                                                      \
	static uint8_t key_ ## suffix ## _delay = 0;                        \
	if(btn.btn_ ## suffix)                                              \
	{                                                                   \
		if(key_ ## suffix ## _delay == 0)                                \
		{                                                                \
			key_ ## suffix ## _delay++;                                   \
		}                                                                \
		else if(key_ ## suffix ## _delay < BTN_DELAY_COUNT)              \
		{                                                                \
			btn.btn_ ## suffix = 0;                                       \
			key_ ## suffix ## _delay++;                                   \
		}                                                                \
	}                                                                   \
	else key_ ## suffix ## _delay = 0;                                  \
}

#include <stdio.h>
#define BTN_DELAY_2_ARGS(suffix1, suffix2)                             \
{                                                                      \
	static uint8_t key_ ## suffix1 ## suffix2 ## _delay = 0;            \
	if(btn.btn_ ## suffix1 && btn.btn_ ## suffix2)                      \
	{                                                                   \
		if(key_ ## suffix1 ## suffix2 ## _delay == 0)                    \
		{                                                                \
			key_ ## suffix1 ## suffix2 ## _delay++;                       \
			btn.btn_ ## suffix1 ## suffix2 = 1;                           \
		}                                                                \
		else if(key_ ## suffix1 ## suffix2 ## _delay < BTN_DELAY_COUNT)  \
		{                                                                \
			btn.btn_ ## suffix1 ## suffix2 = 0;                           \
			key_ ## suffix1 ## suffix2 ## _delay++;                       \
		}                                                                \
	}                                                                   \
	else key_ ## suffix1 ## suffix2 ## _delay = 0;                      \
}


BTNstatus delay_btn_read()
{
	BTNstatus btn = debounce_btn_read();

	BTN_DELAY_2_ARGS(a, b);
	BTN_DELAY_2_ARGS(b, c);
	BTN_DELAY_2_ARGS(a, c);

	BTN_DELAY(a);
	BTN_DELAY(b);
	BTN_DELAY(c);

	return btn;
}