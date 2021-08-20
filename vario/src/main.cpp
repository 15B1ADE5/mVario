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


Settings settings;

SSD1306driver ssd1306;
Display disp(&ssd1306);

BME280 sensor;


void run_vario()
{
	ssd1306.clearBuffer();
	Vario vario(&sensor, &disp);
	vario.draw();
	float buf[64] = {1};
	for(int i = 0; i < 64; i++) printf("%f\n", buf[i] *= i);
}

bool run_menu()
{
	ssd1306.clearBuffer();
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
		_delay_ms(3000);
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

	if(ssd1306.deviceOK()) printf("SSD1306: OK\n");
	if(sensor.deviceOK()) printf("BME280: OK\n");
	
	menu_init(&sensor, &disp, &settings);

	printf("Boot time: %lu ms\n", timer_get());

	ssd1306.clearBuffer();

	main_loop();

	return 0;
}