#ifndef DISPLAY_SETTINGS_TREE_H
#define DISPLAY_SETTINGS_TREE_H

#include <stdint.h>


#include "../menu.h"
#include "../value_edit.h"


class DisplayCLKDivRatRate : public MenuValueEdit<uint8_t>
{
protected:
	void value_change_action() override;
	void save_action() override;
	void exit_action() override;
	
public:
	DisplayCLKDivRatRate();
};

class DisplayCLKFrequency : public MenuValueEdit<uint8_t>
{
protected:
	void value_change_action() override;
	void save_action() override;
	void exit_action() override;
	
public:
	DisplayCLKFrequency();
};

class DisplayContrast : public MenuValueEdit<uint8_t>
{
protected:
	void value_change_action() override;
	void save_action() override;
	void exit_action() override;
	
public:
	DisplayContrast();
};

class DisplayPreChargePeriod : public MenuValueEdit<uint8_t>
{
protected:
	void value_change_action() override;
	void save_action() override;
	void exit_action() override;
	
public:
	DisplayPreChargePeriod();
};

class DisplayVCOMlevel : public MenuValueEdit<uint8_t>
{
protected:
	void value_change_action() override;
	void save_action() override;
	void exit_action() override;
	
public:
	DisplayVCOMlevel();
};



class DisplayTree : public MenuList
{
	MenuListItem *entry_list[6];

	DisplayCLKDivRatRate clk_div_rat_entry;
	DisplayCLKFrequency frequency_entry;
	DisplayContrast contrast_entry;
	DisplayPreChargePeriod precharge_entry;
	DisplayVCOMlevel v_com_level_entry;

	
public:
	DisplayTree();
};

#endif // DISPLAY_SETTINGS_TREE_H