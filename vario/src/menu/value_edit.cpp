#include "value_edit.h"

#include <stdio.h>
#include <util/delay.h>

#include "../hardware/buttons/buttons.h"

template <class T>
MenuValueEdit<T>::MenuValueEdit(
	const char * text,
	const T &value,
	const T &min,
	const T &max,
	const uint8_t int_digits,
	const uint8_t fraq_digits,
	const uint8_t * icon
) : MenuListItem(text, icon)
{
	this->value = value;
	this->value_back = value;
	this->min = min;
	this->max = max;

	if(int_digits > VALUE_EDIT_INT_DIGITS) this->int_digits = VALUE_EDIT_INT_DIGITS;
	else this->int_digits = int_digits;

	if(fraq_digits > VALUE_EDIT_FRAQ_DIGITS) this->fraq_digits = VALUE_EDIT_FRAQ_DIGITS;
	else this->fraq_digits = fraq_digits;
}

template <class T>
void MenuValueEdit<T>::setup(
	const char * text,
	const T &value,
	const T &min,
	const T &max,
	const uint8_t int_digits,
	const uint8_t fraq_digits,
	const uint8_t * icon
)
{
	this->_text = text;
	this->_icon = icon;
	this->value = value;
	this->value_back = value;
	this->min = min;
	this->max = max;

	if(int_digits > VALUE_EDIT_INT_DIGITS) this->int_digits = VALUE_EDIT_INT_DIGITS;
	else this->int_digits = int_digits;

	if(fraq_digits > VALUE_EDIT_FRAQ_DIGITS) this->fraq_digits = VALUE_EDIT_FRAQ_DIGITS;
	else this->fraq_digits = fraq_digits;
}

template <class T>
void MenuValueEdit<T>::drawValue()
{
	sprintf(
		text_buffer,
		"% 014.6f",//"%*.*f",
		//(int_digits + fraq_digits),
		//fraq_digits,
		(float)value
	);
	//buffer[4] = 0;
	menu_display->print(
		text_buffer,
		font_2x7,
		false,
		4,
		3,
		2,
		false
	);
}

template <class T>
void MenuValueEdit<T>::drawArrow()
{

	if(pos)
	{
		uint8_t pointer_pos = 7 - pos;
		for(uint8_t c = 0; c < 14; c++) text_buffer[c] = ' ';
		text_buffer[pointer_pos] = '^';
		text_buffer[14] = 0;
		//sprintf(text_buffer, "  %d", pos);

		menu_display->print(
			text_buffer,
			font_2x7,
			false,
			6,
			3,
			2,
			false
		);
	}
	else
	{
		menu_display->print(
			"Save?",
			font_1x4,
			false,
			6,
			56,
			1,
			false
		);
		menu_display->print(
			"up=NO  down=YES",
			font_1x4,
			false,
			7,
			32,
			1,
			false
		);
	}

}

template <class T>
void MenuValueEdit<T>::draw()
{
	menu_display->driver()->clearBuffer();
	menu_display->print(
		text(),
		font_2x7,
		false,
		0,
		4,
		2
	);
	drawValue();
	drawArrow();
}

template <class T>
void MenuValueEdit<T>::up()
{
	T mod = 1;
	if(pos > 0) for(uint8_t p = 1; p < pos; p++) mod *= 10;
	else for(int8_t p = 0; p > pos; p--) mod /= 10;
	value += mod;

	printf("%d\n", value);

	if(value < min) value = max;
	else if(value > max) value = max;
	drawValue();
	value_change_action();
}

template <class T>
void MenuValueEdit<T>::down()
{
	T mod = 1;
	if(pos > 0) for(uint8_t p = 1; p < pos; p++) mod *= 10;
	else for(int8_t p = 0; p > pos; p--) mod /= 10;
	value -= mod;

	if(value < min) value = min;
	else if(value > max) value = min;
	drawValue();
	value_change_action();
}

template <class T>
void MenuValueEdit<T>::select()
{
	if( (pos > 1) || ( (pos < 0) && (pos > -fraq_digits) ) )
	{
		pos--;
	}
	else if(pos == 0)
	{
		pos = int_digits;
	}
	else if(pos == -fraq_digits)
	{
		pos = 0;
	}
	else if(pos == 1)
	{
		pos = fraq_digits ? -1 : 0;
	}
	else
	{
		pos = 1;
	}
	drawArrow();
}

template <class T>
uint8_t MenuValueEdit<T>::enter()
{
	pos = int_digits;
	draw();
	
	BTNstatus btn;
	uint8_t btn_ticks = 0;
	while(true)
	{
		for(btn_ticks = 0; btn_ticks < 3; btn_ticks++)
		{
			debounce_btn_read();
			_delay_ms(10);
		}

		btn = delay_btn_read();

		if(btn.btn_ac) break;

		if(btn.btn_a)
		{
			if(pos == 0) break;
			up();
		}
		if(btn.btn_c)
		{
			if(pos == 0) 
			{
				save_action();
				break;
			}
			down();
		}
		if(btn.btn_b) select();
	}
	exit_action();
	return pos;
}

template class MenuValueEdit<uint8_t>;
template class MenuValueEdit<float>;