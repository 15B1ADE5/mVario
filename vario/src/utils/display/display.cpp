#include "display.h"

#include "../buffer/buffer.h"

Display::Display(SSD1306driver *display)
{
	this->display = display;
}

uint8_t Display::scale_bit(const uint8_t input, const uint8_t scale, const uint8_t line)
{
	uint8_t output = 0x00;
	for(uint8_t b = 0; b < 8; b++)
	{
		output |= ( (uint8_t) bool(input & (1 << ( (line * 8 + b) / scale) ) ) << b);
	}
	return output;
}

void Display::invert_buffer(int16_t n)
{
	n--;
	while(n >=0)
	{
		data_buffer[n] = ~data_buffer[n];
		n--;
	}
}

int8_t Display::print(
	const char *string,
	const Font &font,
	const bool align_right,
	const uint8_t v_offset,
	const uint8_t x_offset,
	const uint8_t spacing,
	const bool invert
) {
	uint8_t string_size = 0;
	while(string[string_size]) string_size++;

	uint8_t height = font.char_height;
	uint8_t width = font.char_width * string_size + spacing * (string_size - 1);

	if( (uint16_t)height * (uint16_t)width > BUFFER_SIZE) return DISPLAY_ERR_BUFFER_OVERFLOW;

	// Render text
	uint16_t buffer_byte = 0;
	for(uint8_t l = 0; l < font.char_height; l++)
	{
		for(uint8_t c = 0; c < string_size; c++)
		{
			for(uint8_t x = 0; x < font.char_width; x++)
			{
				data_buffer[buffer_byte] = font.get_byte(string[c], x, l);
				buffer_byte++;
			}
			if(c < (string_size - 1) )
			{
				for(uint8_t s = 0; s < spacing; s++)
				{
					data_buffer[buffer_byte] = 0x00;
					buffer_byte++;
				}
			}
		}
	}

	// Set ranges
	if(align_right) display->setColumnRange(DISPLAY_MAX_WIDTH - x_offset - width + 1, DISPLAY_MAX_WIDTH - x_offset);
	else display->setColumnRange(x_offset, x_offset + width -1);
	display->setPagesRange(v_offset, height + v_offset -1);

	// Invert
	if(invert) invert_buffer(buffer_byte);

	// Send buffer
	display->sendData(data_buffer, buffer_byte);

	return DISPLAY_OK;
}

int8_t Display::printScaled(
	const char *string,
	const Font &font,
	const bool align_right,
	const uint8_t v_offset,
	const uint8_t x_offset,
	const uint8_t spacing,
	const bool invert,
	const uint8_t v_scale,
	const uint8_t h_scale
) {
	uint8_t string_size = 0;
	while(string[string_size]) string_size++;

	uint8_t height = font.char_height * v_scale;
	uint8_t width = font.char_width * h_scale * string_size + spacing * (string_size - 1);

	if( (uint16_t)height * (uint16_t)width > BUFFER_SIZE) return DISPLAY_ERR_BUFFER_OVERFLOW;

	// Render text
	uint16_t buffer_byte = 0;
	for(uint8_t l = 0; l < font.char_height; l++)
	{
		for(uint8_t sl = 0; sl < v_scale; sl++)
		{
			for(uint8_t c = 0; c < string_size; c++)
			{
				for(uint8_t x = 0; x < font.char_width; x++)
				{
					for(uint8_t sx = 0; sx < h_scale; sx++)
					{
						data_buffer[buffer_byte] = scale_bit(
							font.get_byte(string[c], x, l),
							v_scale,
							sl
						);
						buffer_byte++;
					}
				}
				if(c < (string_size - 1) )
				{
					for(uint8_t s = 0; s < spacing; s++)
					{
						data_buffer[buffer_byte] = 0x00;
						buffer_byte++;
					}
				}
			}
		}
	}

	// Set ranges
	if(align_right) display->setColumnRange(DISPLAY_MAX_WIDTH - x_offset - width + 1, DISPLAY_MAX_WIDTH - x_offset);
	else display->setColumnRange(x_offset, x_offset + width -1);
	display->setPagesRange(v_offset, height + v_offset -1);

	// Invert
	if(invert) invert_buffer(buffer_byte);

	// Send buffer
	display->sendData(data_buffer, buffer_byte);

	return DISPLAY_OK;
}