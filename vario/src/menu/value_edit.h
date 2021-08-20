#ifndef VALUE_EDIT_H
#define VALUE_EDIT_H

#include <stdint.h>

//#include "../utils/display/display.h"
//#include "../hardware/bme280/bme280.h"

#include "menu.h"

#define VALUE_EDIT_MAX_STRING_LEN          15
#define VALUE_EDIT_MAX_DIGITS              13

#define VALUE_EDIT_INT_DIGITS              13
#define VALUE_EDIT_FRAQ_DIGITS             13

template <class T>
class MenuValueEdit : public MenuListItem
{
protected:
	T value, value_back, min, max;
	uint8_t int_digits, fraq_digits;
	int8_t pos;

	void draw();
	void drawValue();
	void drawArrow();
	void up();
	void down();
	void select();

	virtual void value_change_action() {};
	virtual void save_action() {};
	virtual void exit_action() {};

public:
	MenuValueEdit(
		const char * text,
		const T &value,
		const T &min,
		const T &max,
		const uint8_t int_digits = 1,
		const uint8_t fraq_digits = 0,
		const uint8_t * icon = _icon_default
	);
	void setup(
		const char * text,
		const T &value,
		const T &min,
		const T &max,
		const uint8_t int_digits = 1,
		const uint8_t fraq_digits = 0,
		const uint8_t * icon = _icon_default
	);

	void enter() override;
	T getValue() { return value; };
};

#endif // VALUE_EDIT_H