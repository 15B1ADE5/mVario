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
	if(btn_state.btn_ ## suffix != current_btn_state.btn_ ## suffix) {  \
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
	}

BTNstatus btn_read()
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