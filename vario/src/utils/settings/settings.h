#ifndef SETTINGS_H
#define SETTINGS_H


#include "../../hardware/bme280/bme280.h"
#include "../../hardware/ssd1306/SSD1306.h"

#define SETTINGS_DEFAULT_SENSOR_FILTER             BME280driver::FILTER_X16
#define SETTINGS_DEFAULT_SENSOR_STANDBY_DURATION   BME280driver::STANDBY_MS_0_5
#define SETTINGS_DEFAULT_SENSOR_P_SAMPLING         BME280driver::SAMPLING_X16
#define SETTINGS_DEFAULT_SENSOR_T_SAMPLING         BME280driver::SAMPLING_X16
#define SETTINGS_DEFAULT_SENSOR_H_SAMPLING         BME280driver::SAMPLING_X16

struct DisplaySettings
{
	uint8_t clock_divide_ratio = SSD1306_DEFAULT_CLOCK_DIV_RATIO;
	uint8_t clock_frequency = SSD1306_DEFAULT_CLOCK_FREQUENCY;

	uint8_t contrast = SSD1306_DEFAULT_CONTRAST;
	uint8_t precharge_period = SSD1306_DEFAULT_PRECHARGE_PERIOD;
	uint8_t v_com_deselectlevel = SSD1306_DEFAULT_V_COM_DESELECT;
};

struct SensorSettings
{
	BME280driver::Filter filter = SETTINGS_DEFAULT_SENSOR_FILTER;
	BME280driver::StandbyDuration standby_duration = SETTINGS_DEFAULT_SENSOR_STANDBY_DURATION;

	BME280driver::Sampling pressure_sampling = SETTINGS_DEFAULT_SENSOR_P_SAMPLING;
	BME280driver::Sampling temperature_sampling = SETTINGS_DEFAULT_SENSOR_T_SAMPLING;
	BME280driver::Sampling humidity_sampling = SETTINGS_DEFAULT_SENSOR_H_SAMPLING;
};

struct Settings
{
	DisplaySettings display;
	SensorSettings sensor;
};

#endif // SETTINGS_H