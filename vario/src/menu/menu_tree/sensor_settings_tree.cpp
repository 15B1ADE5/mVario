#include "sensor_settings_tree.h"

#include <avr/pgmspace.h>

#include "../../utils/settings/settings.h"


const PROGMEM char sensor_tree_text[] = {"Sensor"};

const PROGMEM char filter_text[] = {"Filter"};
const PROGMEM char pressure_sampling_text[] = {"Pres. samp."};
const PROGMEM char temperature_sampling_text[] = {"Temp. samp."};
const PROGMEM char humidity_sampling_text[] = {"Hum. samp."};
const PROGMEM char standby_text[] = {"Standby dur"};

const PROGMEM char sensor_off_selection_text[] = {"off"};
const PROGMEM char sensor_none_selection_text[] = {"none"};
const PROGMEM char sensor_x1_selection_text[] = {"x1"};
const PROGMEM char sensor_x2_selection_text[] = {"x2"};
const PROGMEM char sensor_x4_selection_text[] = {"x4"};
const PROGMEM char sensor_x8_selection_text[] = {"x8"};
const PROGMEM char sensor_x16_selection_text[] = {"x16"};

const PROGMEM char duration_0_5_text[] = {"0.5 ms"};
const PROGMEM char duration_10_text[] = {"10 ms"};
const PROGMEM char duration_20_text[] = {"20 ms"};
const PROGMEM char duration_62_5_text[] = {"62.5 ms"};
const PROGMEM char duration_125_text[] = {"125 ms"};
const PROGMEM char duration_250_text[] = {"250 ms"};
const PROGMEM char duration_500_text[] = {"500 ms"};
const PROGMEM char duration_1000_text[] = {"1000 ms"};


SensorTree::SensorTree() : MenuList(sensor_tree_text)
{
	entry_list[0] = &menu_entry_back;
	entry_list[1] = &filter_entry;
	entry_list[2] = &pressure_sampling_entry;
	entry_list[3] = &temperature_sampling_entry;
	entry_list[4] = &humidity_sampling_entry;
	entry_list[5] = &standby_duration_entry;

	setup(
		sensor_tree_text,
		entry_list,
		6,
		0
	);
}


SensorFilter::SensorFilter() : MenuList(filter_text)
{
	filter_off_entry.setup(sensor_off_selection_text);
	filter_x2_entry.setup(sensor_x2_selection_text);
	filter_x4_entry.setup(sensor_x4_selection_text);
	filter_x8_entry.setup(sensor_x8_selection_text);
	filter_x16_entry.setup(sensor_x16_selection_text);

	entry_list[0] = &menu_entry_back;
	entry_list[1] = &filter_off_entry;
	entry_list[2] = &filter_x2_entry;
	entry_list[3] = &filter_x4_entry;
	entry_list[4] = &filter_x8_entry;
	entry_list[5] = &filter_x16_entry;

	setup(
		filter_text,
		entry_list,
		6,
		0
	);
}

void SensorFilter::select()
{
	BME280driver::Filter filter_value[5] = {
		BME280driver::FILTER_OFF,
		BME280driver::FILTER_X2,
		BME280driver::FILTER_X4,
		BME280driver::FILTER_X8,
		BME280driver::FILTER_X16
	};

	menu_settings->sensor.filter = filter_value[position - 1];
	menu_sensor->setFilter(menu_settings->sensor.filter);
	exit = true;
}


MenuListItem SensorSampling::sampling_none_entry(sensor_none_selection_text);
MenuListItem SensorSampling::sampling_x1_entry(sensor_x1_selection_text);
MenuListItem SensorSampling::sampling_x2_entry(sensor_x2_selection_text);
MenuListItem SensorSampling::sampling_x4_entry(sensor_x4_selection_text);
MenuListItem SensorSampling::sampling_x8_entry(sensor_x8_selection_text);
MenuListItem SensorSampling::sampling_x16_entry(sensor_x16_selection_text);

SensorSampling::SensorSampling(const char * text) : MenuList(text)
{
	entry_list[0] = &menu_entry_back;
	entry_list[1] = &sampling_none_entry;
	entry_list[2] = &sampling_x1_entry;
	entry_list[3] = &sampling_x2_entry;
	entry_list[4] = &sampling_x4_entry;
	entry_list[5] = &sampling_x8_entry;
	entry_list[6] = &sampling_x16_entry;

	setup(
		text,
		entry_list,
		7,
		0
	);
}

void SensorSampling::select()
{
	BME280driver::Sampling sampling_value[6] = {
		BME280driver::SAMPLING_NONE,
		BME280driver::SAMPLING_X1,
		BME280driver::SAMPLING_X2,
		BME280driver::SAMPLING_X4,
		BME280driver::SAMPLING_X8,
		BME280driver::SAMPLING_X16
	};

	apply_settings(sampling_value[position - 1]);

	exit = true;
}

SensorPressureSampling::SensorPressureSampling() : SensorSampling(pressure_sampling_text) {}

void SensorPressureSampling::apply_settings(BME280driver::Sampling value)
{
	menu_settings->sensor.pressure_sampling = value;
	menu_sensor->setPressureSampling(menu_settings->sensor.pressure_sampling);
}

SensorTemperatureSampling::SensorTemperatureSampling() : SensorSampling(temperature_sampling_text) {}

void SensorTemperatureSampling::apply_settings(BME280driver::Sampling value)
{
	menu_settings->sensor.temperature_sampling = value;
	menu_sensor->setPressureSampling(menu_settings->sensor.temperature_sampling);
}

SensorHumiditySampling::SensorHumiditySampling() : SensorSampling(humidity_sampling_text) {}

void SensorHumiditySampling::apply_settings(BME280driver::Sampling value)
{
	menu_settings->sensor.humidity_sampling = value;
	menu_sensor->setPressureSampling(menu_settings->sensor.humidity_sampling);
}


SensorStandbyDuration::SensorStandbyDuration() : MenuList(standby_text)
{
	duration_0_5_entry.setup(duration_0_5_text);
	duration_10_entry.setup(duration_10_text);
	duration_20_entry.setup(duration_20_text);
	duration_62_5_entry.setup(duration_62_5_text);
	duration_125_entry.setup(duration_125_text);
	duration_250_entry.setup(duration_250_text);
	duration_500_entry.setup(duration_500_text);
	duration_1000_entry.setup(duration_1000_text);

	entry_list[0] = &menu_entry_back;
	entry_list[1] = &duration_0_5_entry;
	entry_list[2] = &duration_10_entry;
	entry_list[3] = &duration_20_entry;
	entry_list[4] = &duration_62_5_entry;
	entry_list[5] = &duration_125_entry;
	entry_list[6] = &duration_250_entry;
	entry_list[7] = &duration_500_entry;
	entry_list[8] = &duration_1000_entry;


	setup(
		standby_text,
		entry_list,
		9,
		0
	);
}

void SensorStandbyDuration::select()
{
	BME280driver::StandbyDuration duration_value[8] = {
		BME280driver::STANDBY_MS_0_5,
		BME280driver::STANDBY_MS_10,
		BME280driver::STANDBY_MS_20,
		BME280driver::STANDBY_MS_62_5,
		BME280driver::STANDBY_MS_125,
		BME280driver::STANDBY_MS_250,
		BME280driver::STANDBY_MS_500,
		BME280driver::STANDBY_MS_1000
	};

	menu_settings->sensor.standby_duration = duration_value[position - 1];
	menu_sensor->setStandbyDuration(menu_settings->sensor.standby_duration);
	exit = true;
}