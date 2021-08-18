#include "menu.h"

#include "icons/icon_default.h"
#include "icons/icon_menu.h"


static BME280 *sensor;
static Display *display;

void menu_init(BME280 *_sensor, Display *_display)
{
	sensor = _sensor;
	display = _display;
}

MenuItem::MenuItem()
{
	_icon = _icon_default;
}

uint8_t MenuItem::icon(uint8_t byte)
{
	return pgm_read_byte( &(_icon[byte]) );
}

Menu::Menu()
{
	_icon = _icon_menu;
}

static uint8_t display_buffer[32] = {0}; 

void Menu::drawItem(MenuItem * item, uint8_t pos, bool selected)
{
	pos *= 2;
	for(uint8_t byte = 0; byte < 32; byte++) 
	{
		display_buffer[byte] = selected ? ~(item->icon(byte) ) : item->icon(byte);
	}
	display->driver()->setColumnRange(0, 15);
	display->driver()->setPagesRange(pos, pos + 1);
	display->driver()->sendData(display_buffer, 32);

	if(selected) {
		for(uint8_t line = 0; line < 4; line++) display_buffer[line] = 0xFF;
		display->driver()->setColumnRange(16, 17);
		display->driver()->sendData(display_buffer, 4);
	}

	display->print(
		item->name(),
		font_2x7,
		false,
		pos,
		18,
		2,
		selected
	);
}

Dummy dumb;

void Menu::draw()
{
	if(position_offset > 3) position_offset = 0;
	if( (position - position_offset) > menu_length) position_offset = 0;
	display->driver()->clearBuffer();
	for(uint8_t item = 0; item < 4; item++)
	{
		if( (position - position_offset + item) < menu_length)
		{
			drawItem(
				&dumb,//item_list[position - position_offset + item],
				item,
				(position_offset == item) ? true : false
			);
		}
	}
	
	for(uint8_t line = 0; line < 8; line++) display_buffer[line] = 0xFF;
	display->driver()->setColumnRange(126, 126);
	display->driver()->setPagesRange(0, 7);
	display->driver()->sendData(display_buffer, 8);
}

void Menu::enter()
{
	menu_length = 6;
	position = 5;
	position_offset = 0;
	draw();
}
