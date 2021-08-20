#ifndef SENSOR_SETTINGS_TREE_H
#define SENSOR_SETTINGS_TREE_H

#include <stdint.h>


#include "../menu.h"
#include "../value_edit.h"
#include "../../hardware/bme280/bme280.h"


class SensorFilter : public MenuList
{
	MenuListItem *entry_list[6];

	MenuListItem filter_off_entry;
	MenuListItem filter_x2_entry;
	MenuListItem filter_x4_entry;
	MenuListItem filter_x8_entry;
	MenuListItem filter_x16_entry;
	
protected:
	void select() override;

public:
	SensorFilter();
};

class SensorSampling : public MenuList
{
	MenuListItem *entry_list[7];

	static MenuListItem sampling_none_entry;
	static MenuListItem sampling_x1_entry;
	static MenuListItem sampling_x2_entry;
	static MenuListItem sampling_x4_entry;
	static MenuListItem sampling_x8_entry;
	static MenuListItem sampling_x16_entry;
	
protected:
	void select() override;
	virtual void apply_settings(BME280driver::Sampling value) = 0;
public:
	SensorSampling(const char * text = empty_text);
};

class SensorPressureSampling : public SensorSampling
{
protected:
	void apply_settings(BME280driver::Sampling value) override;
public:
	SensorPressureSampling();
};

class SensorTemperatureSampling : public SensorSampling
{
protected:
	void apply_settings(BME280driver::Sampling value) override;
public:
	SensorTemperatureSampling();
};

class SensorHumiditySampling : public SensorSampling
{
protected:
	void apply_settings(BME280driver::Sampling value) override;
public:
	SensorHumiditySampling();
};

class SensorStandbyDuration : public MenuList
{
	MenuListItem *entry_list[9];

	MenuListItem duration_0_5_entry;
	MenuListItem duration_10_entry;
	MenuListItem duration_20_entry;
	MenuListItem duration_62_5_entry;
	MenuListItem duration_125_entry;
	MenuListItem duration_250_entry;
	MenuListItem duration_500_entry;
	MenuListItem duration_1000_entry;
	
protected:
	void select() override;

public:
	SensorStandbyDuration();
};

class SensorTree : public MenuList
{
	MenuListItem *entry_list[6];

	SensorFilter filter_entry;
	SensorPressureSampling pressure_sampling_entry;
	SensorTemperatureSampling temperature_sampling_entry;
	SensorHumiditySampling humidity_sampling_entry;
	SensorStandbyDuration standby_duration_entry;
	

public:
	SensorTree();
};




#endif // SENSOR_SETTINGS_TREE_H