#include "vario.h"

#include <stdio.h>
#include <math.h>
#include <util/delay.h>

#include "../utils/buffer/buffer.h"
#include "../utils/time_clock/time_clock.h"

Vario::Vario(BME280 *sensor, Display *display)
{
	this->sensor = sensor;
	this->display = display;

	sensor->startNormalACQ();
	timer_reset();
	measure();
	measureBattery();
}



void Vario::measure()
{
	altitude_prev = altitude;
	sensor->readData(&pressure, &temperature, &humidity);
	altitude = BME280calcAltitude(pressure);
	
	speed.push( (altitude - altitude_prev) * 1000.0 / (float)timer_get_reset() );
	
	//printf("%d\n", uint32_t(800.0 * pow(2.0, (speed.mean() / 2) ) ) );

	speed_v = speed.mean();
	if( (speed_v >= 0.2) && (speed_v <= 8.0) )
	{
		uint32_t tone = (450.0 * pow(2.0, (speed_v / 8.0) ) );
		pulseToneSet(
			tone,
			8 + (900 - tone) / 40,
			4 + (900 - tone) / 20
		);
		if(speed_v < 1.0) toneACsetFrequency(tone);
		pulseToneStart();
	}
	else if( (speed_v > 8.0) && (speed_v < 40.0) )
	{
		uint32_t tone = (450.0 * pow(2.0, (speed_v / 8.0) ) );
		pulseToneSet(
			tone,
			8,
			4
		);
		if(speed_v < 1.0) toneACsetFrequency(tone);
		pulseToneStart();
	}
	else
	{
		pulseToneStop();
	}
}

void Vario::setZeroAltitude()
{
	zero_altitude = altitude;
	drawZeroAlt();
}

void Vario::measureBattery()
{
	battery_level = get_battery_voltage();
}


static char buffer[8] = {0};

void Vario::drawBase()
{
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
	display->print("m", font_1x4, false, 1, 36);

	// humidity
	display->print("%", font_1x4, false, 1, 82);
}

void Vario::drawMain()
{
	// altitude
	sprintf(buffer, "%6.1f", altitude - zero_altitude);
	for(uint8_t c = 0; c < 6; c++)
	{
		if(buffer[c] == ' ')buffer[c] = '/';
	}
	buffer[6] = 0;
	display->print(
		buffer,
		font_digits_4x14,
		true,
		2,
		23,
		2,
		false
	);

	//speed
	sprintf(buffer, "%5.2f", speed_v);//speed.mean() );
	buffer[5] = 0;
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
		56
	);
}

void Vario::drawZeroAlt()
{
	// zero_altitude
	sprintf(buffer, "%7.1f", zero_altitude);
	buffer[7] = 0;
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
	drawBase();
	drawMain();
	drawSec();
	drawZeroAlt();
	drawBattery();
}


void Vario::enter()
{
	draw();
	loop();
	pulseToneStop();
}


void Vario::loop()
{
	BTNstatus btn;
	uint8_t cycle = 0;

	while(true)
	{
		measure();

		btn = delay_btn_read();

		if(btn.btn_ac) 
		{
			break;
		}
		else if(btn.btn_b) 
		{
			setZeroAltitude();
		}
		else if(btn.btn_a) 
		{
			speed_v += 0.05;
		}
		else if(btn.btn_c) 
		{
			speed_v -= 0.05;
		}
		
		drawMain();
		if((cycle % 32) == 0) drawSec();

		cycle++;
		//_delay_ms(100);
	}
}