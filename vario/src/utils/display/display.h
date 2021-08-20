#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#include "../../hardware/ssd1306/SSD1306.h"
#include "fonts/fonts.h"

#define DISPLAY_TMP_BUFFER_SIZE         400

#define DISPLAY_MAX_HEIGHT              SSD1306_MAX_HEIGHT
#define DISPLAY_MAX_WIDTH               SSD1306_MAX_WIDTH


#define DISPLAY_OK                      0
#define DISPLAY_ERR_SCREEN_DRIVER       -1
#define DISPLAY_ERR_BUFFER_OVERFLOW     -2


class Display
{
	SSD1306driver *display;

	uint8_t scale_bit(const uint8_t input, const uint8_t scale, const uint8_t line);
	void invert_buffer(int16_t n);
public:
	Display(SSD1306driver *display);

	SSD1306driver *driver() { return display; }

	int8_t print(
		const char *string,
		const Font &font = font_1x4,
		const bool align_right = false,
		const uint8_t v_offset = 0,
		const uint8_t x_offset = 0,
		const uint8_t spacing = 1,
		const bool invert = false
	);

	int8_t printScaled(
		const char *string,
		const Font &font = font_1x4,
		const bool align_right = false,
		const uint8_t v_offset = 0,
		const uint8_t x_offset = 0,
		const uint8_t spacing = 1,
		const bool invert = false,
		const uint8_t v_scale = 1,
		const uint8_t h_scale = 1
	);
};


#endif // DISPLAY_H