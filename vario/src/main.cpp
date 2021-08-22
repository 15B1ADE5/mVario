#include <stdio.h>
#include <util/delay.h>

#include "hardware/uart/uart.h"
#include "hardware/i2cmaster/i2cmaster.h"
#include "hardware/bme280/bme280.h"
#include "hardware/ssd1306/SSD1306.h"
#include "hardware/toneAC/toneAC.h"
#include "hardware/battery/battery.h"
#include "hardware/buttons/buttons.h"

#include "utils/time_clock/time_clock.h"
#include "utils/display/display.h"
#include "utils/settings/settings.h"
#include "vario/vario.h"
#include "menu/menu_tree/menu_tree.h"


Display * _display;
BME280 * _sensor;

void run_vario()
{
	_display->driver()->clearBuffer();
	Vario vario(_sensor, _display);
	vario.enter();
}

bool run_menu()
{
	_display->driver()->clearBuffer();
	MenuTree menu;
	menu.enter();
	return false;
}

void main_loop()
{
	while(true)
	{
		toneAC(1760);
		_delay_us(100000);
		noToneAC();

		run_vario();
		run_menu();
	}
}

int main(void) {
	toneAC(880);

	time_clock_init();
	uart_init();
	i2c_init();
	pulseToneInit();
	btn_init();


	printf("F_CPU: %d MHz\n", F_CPU / 1000000);
	printf("BAT_V: %d (?)\n", get_battery_voltage());

	Settings settings;

	SSD1306driver ssd1306;
	if(ssd1306.deviceOK()) printf("SSD1306: OK\n");

	Display display(&ssd1306);
	_display = &display;

	BME280 sensor;
	if(sensor.deviceOK()) printf("BME280: OK\n");
	_sensor = &sensor;

	sensor.setFilter(settings.sensor.filter);
	sensor.setPressureSampling(settings.sensor.pressure_sampling);
	sensor.setTemperatureSampling(settings.sensor.temperature_sampling);
	sensor.setHumiditySampling(settings.sensor.humidity_sampling);
	sensor.setStandbyDuration(settings.sensor.standby_duration);
	
	menu_init(&sensor, &display, &settings);

	printf("Boot time: %lu ms\n", timer_get());

	ssd1306.clearBuffer();

	main_loop();

	return 0;
}