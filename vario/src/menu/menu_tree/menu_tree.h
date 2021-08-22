#ifndef MENU_TREE_H
#define MENU_TREE_H

#include <stdint.h>

#include "sensor_settings_tree.h"
#include "display_settings_tree.h"
#include "../menu.h"



class SettingsTree : public MenuList
{
	MenuListItem *entry_list[3];
	
	SensorTree sensor_entry;
	//DisplayTree display_entry;

public:
	SettingsTree();
};

class MenuTree : public MenuList
{
	MenuListItem *entry_list[2];

	SettingsTree settings_entry;

public:
	MenuTree();
};

#endif // MENU_TREE_H