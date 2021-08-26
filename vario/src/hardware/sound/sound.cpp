#include "sound.h"

#include <avr/io.h>

void sound_init()
{
	DDRD |= 0b01000000;
	sound_unmute();
}

void sound_unmute()
{
	PORTD |= 0b01000000;
}

void sound_mute()
{
	PORTD &= 0b10111111;
}