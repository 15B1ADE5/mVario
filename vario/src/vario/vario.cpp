#include "vario.h"

#include <stdio.h>

Vario::Vario(BME280 *sensor, Display *display)
{
	this->sensor = sensor;
	this->display = display;

	measure();
	measureBattery();
}

void Vario::measure()
{
	sensor->measure(&pressure, &temperature, &humidity);
	altitude = BME280calcAltitude(pressure);
}

void Vario::measureBattery()
{
	battery_level = get_battery_voltage();
}


static char buffer[8] = {0};

void Vario::drawBase()
{
	// altitude
	display->print("m", font_7x4, true, 4, 13, 2, 2, 0);

	// speed
	display->print("m/s", font_7x4, true, 7, 8);

	// pressure
	display->print("hPa", font_7x4, false, 0, 36);

	// temperature
	display->print("oC", font_7x4, false, 0, 82);

	// battery
	display->print("%", font_7x4, true, 0, 0);

	// zero_altitude
	display->print("m", font_7x4, false, 1, 31);

	// humidity
	display->print("%", font_7x4, false, 1, 87);
}

void Vario::drawMain()
{
	// altitude
	sprintf(buffer, "%6.1f", altitude - zero_altitude);
	display->print(
		buffer,
		font_7x4,
		true,
		2,
		21,
		4,
		3,
		3,
		false
	);

	//speed
	sprintf(buffer, "%4.1f", speed);
	display->print(
		buffer,
		font_7x4,
		true,
		6,
		24,
		2,
		2,
		2,
		false
	);
}

void Vario::drawSec()
{
	// pressure
	sprintf(buffer, "%7.2f", pressure / 100);
	display->print(
		buffer,
		font_7x4,
		false,
		0,
		0
	);

	// temperature
	sprintf(buffer, "%4.1f", temperature);
	display->print(
		buffer,
		font_7x4,
		false,
		0,
		61
	);

	// humidity
	sprintf(buffer, "%5.1f", humidity);
	display->print(
		buffer,
		font_7x4,
		false,
		1,
		61
	);
}

void Vario::drawZeroAlt()
{
	// zero_altitude
	sprintf(buffer, "%6.1f", zero_altitude);
	display->print(
		buffer,
		font_7x4,
		false,
		1,
		0
	);
}

void Vario::drawBattery()
{
	// battery_level
	sprintf(buffer, "%d", battery_level);
	display->print(
		buffer,
		font_7x4,
		true,
		0,
		6
	);
}

void Vario::draw()
{
	drawBase();
	drawMain();
	drawSec();
	drawZeroAlt();
	drawBattery();
}