#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#include "../utils/display/display.h"
#include "../hardware/bme280/bme280.h"

#include "icons/icon_default.h"
#include "icons/icon_menu.h"

void menu_init(BME280 *_sensor, Display *_display);

#define LIST_ITEM_TEXT_BUFFER_LEN     13
#define LIST_ITEM_ICON_LEN            32
#define LIST_ITEM_DISP_BUFFER_LEN     32

class MenuListItem
{
protected:
	const uint8_t * _icon;
	const char * _text;

	static char text_buffer[LIST_ITEM_TEXT_BUFFER_LEN]; // = {0};
	static uint8_t display_buffer[LIST_ITEM_DISP_BUFFER_LEN]; // = {0};

public:
	MenuListItem(const char * text, const uint8_t * icon = _icon_default);
	virtual void enter() {}
	
	// text
	virtual char* text();
	virtual uint8_t *icon();
};

class MenuList : public MenuListItem
{
protected:
	uint8_t list_length = 0;
	uint8_t exit_entry = 255;
	uint8_t position = 0;
	uint8_t position_offset = 0;

	MenuListItem ** list;

	virtual MenuListItem * getMenuListItem(uint8_t pos);

	void drawItem(MenuListItem * item, uint8_t pos = 0, bool selected = false);

	void draw();

	virtual void up();
	virtual void down();
	virtual void select();

public:
	MenuList(
		const char * text,
		MenuListItem ** list = nullptr,
		const uint8_t list_length = 0,
		const uint8_t exit_entry = 255,
		const uint8_t * icon = _icon_menu
	);

	void enter() override;
};


/*
class MenuItem
{
protected:
	const uint8_t * _icon;
public:
	MenuItem();
	virtual void enter() =0;
	virtual char* name() =0;
	uint8_t icon(uint8_t byte = 0);
};

class Menu : public MenuItem
{
	uint8_t menu_length = 0;
	uint8_t position = 0;
	uint8_t position_offset = 0;

	MenuItem ** item_list;

	void drawItem(MenuItem * item, uint8_t pos = 0, bool selected = false);
	void draw();
public:
	Menu();
	void enter() override;
};

class Dummy : public Menu
{
public:
	char* name() override { return "Dummy :/"; }
};
*/
#endif // MENU_H