#include "display_settings_tree.h"

#include <avr/pgmspace.h>

#include "../../utils/display/display.h"
#include "../../utils/settings/settings.h"


const PROGMEM char display_tree_text[] = {"Display"};

const PROGMEM char display_clk_div_rat_text[] = {"CLK div rat"};
const PROGMEM char display_clk_frequency_text[] = {"CLK freq"};
const PROGMEM char display_contrast_text[] = {"Contrast"};
const PROGMEM char display_precharge_p_text[] = {"Pre-Charge p"};
const PROGMEM char display_v_com_level_text[] = {"Vcom level"};

DisplayTree::DisplayTree() : MenuList(display_tree_text)
{
	entry_list[0] = &menu_entry_back;
	entry_list[1] = &contrast_entry;
	entry_list[2] = &precharge_entry;
	entry_list[3] = &v_com_level_entry;
	entry_list[4] = &clk_div_rat_entry;
	entry_list[5] = &frequency_entry;

	setup(
		display_tree_text,
		entry_list,
		6,
		0
	);
}


DisplayCLKDivRatRate::DisplayCLKDivRatRate() : MenuValueEdit(
		display_clk_div_rat_text,
		menu_settings->display.clock_divide_ratio,
		0x00,
		0x07,
		1,
		0,
		_icon_default
) {}

void DisplayCLKDivRatRate::value_change_action()
{
	menu_display->driver()->setRefreshRate(value, menu_settings->display.clock_frequency);
}
void DisplayCLKDivRatRate::save_action()
{
	menu_settings->display.clock_divide_ratio = value;
}
void DisplayCLKDivRatRate::exit_action()
{
	menu_display->driver()->setRefreshRate(menu_settings->display.clock_divide_ratio, menu_settings->display.clock_frequency);
}


DisplayCLKFrequency::DisplayCLKFrequency() : MenuValueEdit(
		display_clk_frequency_text,
		menu_settings->display.clock_frequency,
		0x00,
		0x1F,
		2,
		0,
		_icon_default
) {}

void DisplayCLKFrequency::value_change_action()
{
	menu_display->driver()->setRefreshRate(menu_settings->display.clock_divide_ratio, value);
}
void DisplayCLKFrequency::save_action()
{
	menu_settings->display.clock_frequency = value;
}
void DisplayCLKFrequency::exit_action()
{
	menu_display->driver()->setRefreshRate(menu_settings->display.clock_divide_ratio, menu_settings->display.clock_frequency);
}


DisplayContrast::DisplayContrast() : MenuValueEdit(
		display_contrast_text,
		menu_settings->display.contrast,
		0x00,
		0xFF,
		3,
		0,
		_icon_default
) {}

void DisplayContrast::value_change_action()
{
	menu_display->driver()->setContrast(value);
}
void DisplayContrast::save_action()
{
	menu_settings->display.contrast = value;
}
void DisplayContrast::exit_action()
{
	menu_display->driver()->setContrast(menu_settings->display.contrast);
}


DisplayPreChargePeriod::DisplayPreChargePeriod() : MenuValueEdit(
		display_precharge_p_text,
		menu_settings->display.precharge_period,
		0x00,
		0xFF,
		3,
		0,
		_icon_default
) {}

void DisplayPreChargePeriod::value_change_action()
{
	menu_display->driver()->setPreChargePeriod(value);
}
void DisplayPreChargePeriod::save_action()
{
	menu_settings->display.precharge_period = value;
}
void DisplayPreChargePeriod::exit_action()
{
	menu_display->driver()->setPreChargePeriod(menu_settings->display.precharge_period);
}

DisplayVCOMlevel::DisplayVCOMlevel() : MenuValueEdit(
		display_v_com_level_text,
		menu_settings->display.v_com_deselectlevel,
		0x00,
		0x07,
		1,
		0,
		_icon_default
) {}

void DisplayVCOMlevel::value_change_action()
{
	menu_display->driver()->setVCOMHdeselectLevel(value);
}
void DisplayVCOMlevel::save_action()
{
	menu_settings->display.v_com_deselectlevel = value;
}
void DisplayVCOMlevel::exit_action()
{
	menu_display->driver()->setVCOMHdeselectLevel(menu_settings->display.v_com_deselectlevel);
}

