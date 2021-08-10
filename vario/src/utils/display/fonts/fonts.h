#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

#include "font0.h"

struct Font
{
	uint8_t char_height = 1; // number of 8px lines
	uint8_t char_width = 4;
	uint8_t range_start = 32;
	uint8_t range_end = 127;
	const uint8_t *font = nullptr;

	uint8_t get_byte(const char &ascii_n, const uint8_t &x, const uint8_t &line) const
	{
		uint16_t pos = uint8_t(ascii_n);
		if( (pos < range_start) || (pos > range_end) ) return (uint8_t)pos;

		pos = (pos - range_start) * char_width + line * (127 - 32 + 1) * char_width;
		return font[pos + x];
	}
};

const Font font_7x4 {
	.char_height = 1,
	.char_width = 4,
	.range_start = 32,
	.range_end = 127,
	.font = font0
};

#endif // FONTS_H