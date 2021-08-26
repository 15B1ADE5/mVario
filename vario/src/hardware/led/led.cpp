#include "led.h"

#include <avr/io.h>

void led_init()
{
	DDRB |= 0b00100000;
	led_enable();
}

void led_enable()
{
	PORTB |= 0b00100000;
}

void led_disable()
{
	PORTB &= 0b11011111;
}
