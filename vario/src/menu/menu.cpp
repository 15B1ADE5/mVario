#include "menu.h"

#include <util/delay.h>

#include "../hardware/buttons/buttons.h"


#include "icons/icon_back.h"

static BME280 *sensor;
static Display *display;

void menu_init(BME280 *_sensor, Display *_display)
{
	sensor = _sensor;
	display = _display;
}

const PROGMEM char menu_entry_back_text[] = {".."};

MenuListItem menu_entry_back(menu_entry_back_text, _icon_back);

#include <stdio.h>
//////////////////////////////////////////////////
// MenuListItem:
//////////////////////////////////////////////////

char MenuListItem::text_buffer[LIST_ITEM_TEXT_BUFFER_LEN] = {0};
uint8_t MenuListItem::display_buffer[LIST_ITEM_DISP_BUFFER_LEN] = {0};

MenuListItem::MenuListItem(const char * text, const uint8_t * icon)
{
	setup(text, icon);
}

void MenuListItem::setup(const char * text, const uint8_t * icon)
{
	_icon = icon;
	_text = text;
}

char* MenuListItem::text()
{
	uint8_t c = 0;
	do
	{
		text_buffer[c] = pgm_read_byte( &(_text[c]) );
		c++;
	}
	while( ( (text_buffer[c - 1]) != 0) && (c < LIST_ITEM_TEXT_BUFFER_LEN) );
	
	return text_buffer;
}

uint8_t * MenuListItem::icon()
{
	for(uint8_t byte = 0; byte < LIST_ITEM_ICON_LEN; byte++)
	{
		display_buffer[byte] = pgm_read_byte( &(_icon[byte]) );
	}
	return display_buffer;
}


//////////////////////////////////////////////////
// MenuList:
//////////////////////////////////////////////////


MenuList::MenuList(
	const char * text,
	MenuListItem ** list,
	const uint8_t list_length,
	const uint8_t exit_entry,
	const uint8_t * icon
) : MenuListItem(text, icon)
{
	this->list = list;
	this->list_length = list_length;
	this->exit_entry = exit_entry;
	this->position = 0;
	this->position_offset = 0;
}

void MenuList::setup(
	const char * text,
	MenuListItem ** list,
	const uint8_t list_length,
	const uint8_t exit_entry,
	const uint8_t * icon
)
{
	this->_text = text;
	this->_icon = icon;
	this->list = list;
	this->list_length = list_length;
	this->exit_entry = exit_entry;
	this->position = 0;
	this->position_offset = 0;
}

MenuListItem * MenuList::getMenuListItem(uint8_t pos)
{
	if(pos >= list_length) return nullptr;
	return list[pos];
}

void MenuList::drawItem(MenuListItem * item, uint8_t pos, bool selected)
{
	pos *= 2;
	
	uint8_t * icon = item->icon();
	if(selected)
	{
		for(uint8_t byte = 0; byte < LIST_ITEM_ICON_LEN; byte++) icon[byte] = ~icon[byte];
	}
	display->driver()->setPagesRange(pos, pos + 1);
	display->driver()->setColumnRange(0, 15);
	display->driver()->sendData(display_buffer, 32);

	if(selected)
	{
		for(uint8_t line = 0; line < 4; line++) display_buffer[line] = 0xFF;
	}
	else
	{
		for(uint8_t line = 0; line < 4; line++) display_buffer[line] = 0x00;
	}
	display->driver()->setColumnRange(16, 17);
	display->driver()->sendData(display_buffer, 4);

	char * entry_text = item->text();
	uint8_t c = 0;
	while(entry_text[c] != 0) c++;
	
	for(; c < (LIST_ITEM_TEXT_BUFFER_LEN - 1); c++) entry_text[c] = ' ';

	display->print(
		entry_text,
		font_2x7,
		false,
		pos,
		18,
		2,
		selected
	);
}

void MenuList::draw()
{
	//display->driver()->clearBuffer();

	if(position_offset > 3) position_offset = 0;
	if( (position - position_offset) > list_length) position_offset = 0;
	for(uint8_t item = 0; item < 4; item++)
	{
		if( (position - position_offset + item) < list_length)
		{
			drawItem(
				list[position - position_offset + item],
				item,
				(position_offset == item) ? true : false
			);
		}
	}

	uint8_t bar = (uint16_t)position * 16 / list_length;
	uint8_t pos = 0;
	for(uint8_t bar_line = 0; bar_line < 8; bar_line++)
	{
		pos = bar_line * 4;
		if(bar_line == (bar / 2) )
		{
			display_buffer[pos] = 0x00;
			display_buffer[pos + 1] = 0xFF;
			display_buffer[pos + 2] = 0xFF;
			display_buffer[pos + 3] = 0xFF;
		}
		else
		{
			display_buffer[pos] = 0x00;
			display_buffer[pos + 1] = 0x00;
			display_buffer[pos + 2] = 0xAA;
			display_buffer[pos + 3] = 0x00;
		}
	}
	
	display->driver()->setColumnRange(124, 127);
	display->driver()->setPagesRange(0, 7);
	display->driver()->sendData(display_buffer, LIST_ITEM_DISP_BUFFER_LEN);
}

void MenuList::up()
{
	if(position_offset > 0) position_offset--;
	
	if(position > 0) position--;
	else 
	{
		position = (list_length - 1);
		if(3 >= list_length) position_offset = list_length - 1;
		else position_offset = 3;
	}

	draw();
}

void MenuList::down()
{
	if(position_offset < 3) position_offset++;

	if(position < (list_length - 1) ) position++;
	else 
	{
		position = 0;
		position_offset = 0;
	}

	draw();
}

void MenuList::select()
{
	list[position]->enter();
	draw();
}

void MenuList::enter()
{
	//display->driver()->clearBuffer();
	draw();
	
	BTNstatus btn;

	uint8_t key_a_delay = 0;
	uint8_t key_c_delay = 0;
	uint8_t btn_ticks = 0;
	while(true)
	{
		for(btn_ticks = 0; btn_ticks < 3; btn_ticks++)
		{
			debounce_btn_read();
			_delay_ms(10);
		}

		btn = delay_btn_read();

		if(btn.btn_ac) 
		{
			break;
		}

		if(btn.btn_b) 
		{
			if(position == exit_entry) break;
			select();
		}

		if(btn.btn_a) up();
		if(btn.btn_c) down();
	}
}