#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

#include "font_1x4/font_1x4.h"
#include "font_2x7/font_2x7.h"
#include "font_numbers_4x14/font_numbers_4x14.h"
#include "font_digits_4x14/font_digits_4x14.h"

struct Font
{
	uint8_t char_height = 1; // number of 8px lines
	uint8_t char_width = 4;
	uint8_t range_start = 32;
	uint8_t range_end = 127;
	const uint8_t *font = nullptr;

	uint8_t get_byte(const char ascii_n, const uint8_t x, const uint8_t line) const
	{
		uint16_t pos = uint8_t(ascii_n);
		if( (pos < range_start) || (pos > range_end) ) return (uint8_t)pos;

		pos = (pos - range_start) * char_width + line * (range_end - range_start + 1) * char_width;
		return pgm_read_byte( &(font[pos + x]) );
	}
};

const Font font_1x4 {
	.char_height = 1,
	.char_width = 4,
	.range_start = 32,
	.range_end = 127,
	.font = _font_1x4
};

const Font font_2x7 {
	.char_height = 2,
	.char_width = 7,
	.range_start = 32,
	.range_end = 127,
	.font = _font_2x7
};

const Font font_numbers_4x14 {
	.char_height = 4,
	.char_width = 14,
	.range_start = 32,
	.range_end = 63,
	.font = _font_numbers_4x14
};

const Font font_digits_4x14 {
	.char_height = 4,
	.char_width = 14,
	.range_start = 45,
	.range_end = 57,
	.font = _font_digits_4x14
};

#endif // FONTS_H