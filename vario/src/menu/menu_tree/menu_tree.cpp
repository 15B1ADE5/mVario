#include "menu_tree.h"

#include <avr/pgmspace.h>

const PROGMEM char menu_tree_text[] = {"Menu"};
const PROGMEM char settings_tree_text[] = {"Settings"};

MenuTree::MenuTree() : MenuList(menu_tree_text)
{
	entry_list[0] = &menu_entry_back;
	entry_list[1] = &settings_entry;

	setup(
		menu_tree_text,
		entry_list,
		2,
		0
	);
}

SettingsTree::SettingsTree() : MenuList(settings_tree_text)
{
	entry_list[0] = &menu_entry_back;
	entry_list[1] = &sensor_entry;
	//entry_list[2] = &display_entry;

	setup(
		settings_tree_text,
		entry_list,
		2, //3,
		0
	);
}