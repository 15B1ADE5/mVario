#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#include "../utils/display/display.h"
#include "../hardware/bme280/bme280.h"

void menu_init(BME280 *_sensor, Display *_display);

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

#endif // MENU_H