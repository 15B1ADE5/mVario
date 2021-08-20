#include "vario.h"

#include <stdio.h>

Vario::Vario(BME280 *sensor, Display *display)
{
	this->sensor = sensor;
	this->display = display;

	measure();
	altitude_prev = altitude;
	measureBattery();
}



void Vario::measure()
{
	static float a_acc[32] = {1.1};
	static float acc;
	static uint8_t acc_p = 0;

	sensor->readData(&pressure, &temperature, &humidity);
	altitude_prev = altitude;
	altitude = BME280calcAltitude(pressure);

	if(!acc_p) acc_p = 32;
	a_acc[acc_p - 1] = (altitude - altitude_prev) / 0.131072;
	acc = 0;
	for(int c = 0; c < 32; c++) acc += a_acc[c];
	speed = acc / 32;

	//pulseToneSet(440, 6, 116);
	//
	//printf("%f: %d\n", speed, uint16_t(B_MAX - (speed - V_MIN) / (V_MAX - V_MIN) * (B_MAX - B_MIN) ) );
	
	//toneACopt(BEEP_A + BEEP_B * (1.5 + acc / 16) );

	// if(speed > 0.0) pulseToneSet(
	// 	uint32_t(F_MIN + (speed - V_MIN) / (V_MAX - V_MIN) * (F_MAX - F_MIN) ),
	// 	//uint32_t(BEEP_A + BEEP_B * (0.3 + speed) ),
	// 	uint16_t(B_MAX - (speed - V_MIN) / (V_MAX - V_MIN) * (B_MAX - B_MIN) ),
	// 	6
	// );
	// if(speed > 0.0) toneACopt(
	// 	uint32_t(F_MIN + (speed - V_MIN) / (V_MAX - V_MIN) * (F_MAX - F_MIN) )
	// );

	// if(speed > 0.0)
	// {
		
	// }
	// else
	// {
	// 	noToneAC();
	// }
	acc_p--;
}

void Vario::measureBattery()
{
	battery_level = get_battery_voltage();
}


static char buffer[8] = {0};

static float data_buf[64] = {0};

void Vario::drawBase()
{
	for(int i = 0; i < 32; i++) sprintf(buffer, "%6.1f", data_buf[i]);
	
	// altitude
	display->print("m", font_2x7, true, 4, 13, 1, false);

	// speed
	display->print("m/s", font_1x4, true, 7, 8);

	// pressure
	display->print("hPa", font_1x4, false, 0, 36);

	// temperature
	display->print("oC", font_1x4, false, 0, 82);

	// battery
	display->print("%", font_1x4, true, 0, 0);

	// zero_altitude
	display->print("m", font_1x4, false, 1, 31);

	// humidity
	display->print("%", font_1x4, false, 1, 87);
}

void Vario::drawMain()
{
	// altitude
	sprintf(buffer, "%6.1f", altitude - zero_altitude);
	buffer[6] = 0;
	display->print(
		buffer,
		font_numbers_4x14,
		true,
		2,
		21,
		2,
		false
	);

	//speed
	sprintf(buffer, "%4.1f", speed);
	buffer[4] = 0;
	display->print(
		buffer,
		font_2x7,
		true,
		6,
		24,
		2,
		false
	);
}

void Vario::drawSec()
{
	// pressure
	sprintf(buffer, "%7.2f", pressure / 100);
	buffer[7] = 0;
	display->print(
		buffer,
		font_1x4,
		false,
		0,
		0
	);

	// temperature
	sprintf(buffer, "%4.1f", temperature);
	buffer[4] = 0;
	display->print(
		buffer,
		font_1x4,
		false,
		0,
		61
	);

	// humidity
	sprintf(buffer, "%5.1f", humidity);
	buffer[5] = 0;
	display->print(
		buffer,
		font_1x4,
		false,
		1,
		61
	);
}

void Vario::drawZeroAlt()
{
	// zero_altitude
	sprintf(buffer, "%6.1f", zero_altitude);
	buffer[6] = 0;
	display->print(
		buffer,
		font_1x4,
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
		font_1x4,
		true,
		0,
		6
	);
}

void Vario::draw()
{
	printf("drw\n");
	drawBase();
	drawMain();
	drawSec();
	drawZeroAlt();
	drawBattery();
}