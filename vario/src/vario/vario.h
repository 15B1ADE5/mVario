#ifndef VARIO_H
#define VARIO_H

#include <stdint.h>

#include "../utils/display/display.h"
#include "../utils/data_filter/data_filter.h"
#include "../hardware/bme280/bme280.h"
#include "../hardware/toneAC/toneAC.h"
#include "../hardware/battery/battery.h"
#include "../hardware/buttons/buttons.h"

class Vario
{
	BME280 *sensor;
	Display *display;


	float pressure = 0;
	float temperature = 0;
	float humidity = 0;

	float altitude = 0;
	float altitude_prev = 0;

	float zero_altitude = 0;
	DataFilter speed;

	uint16_t battery_level = 0;

	float speed_v = 0;

	void measure();
	void measureBattery();

	void drawBase();
	void drawMain();
	void drawSec();
	void drawZeroAlt();
	void drawBattery();
	void draw();

	void setZeroAltitude();

	void loop();

public:
	Vario(BME280 *sensor, Display *display);

	void enter();
};


#endif // VARIO_H