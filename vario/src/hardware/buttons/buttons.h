
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

#define BTN_A_BIT                0b00000100 // PD2
#define BTN_B_BIT                0b00001000 // PD3
#define BTN_C_BIT                0b00010000 // PD4

#define BTN_DEBOUNCE_COUNT       4

#define BTN_DELAY_COUNT          8

typedef union
{
	struct
	{
		uint8_t btn_a : 1;
		uint8_t btn_b : 1;
		uint8_t btn_c : 1;
		uint8_t btn_ab : 1;
		uint8_t btn_bc : 1;
		uint8_t btn_ac : 1;
		
		uint8_t : 2;
	} __attribute__((__packed__));
	uint8_t raw;
} BTNstatus;

void btn_init();

BTNstatus delay_btn_read();
BTNstatus debounce_btn_read();

#endif