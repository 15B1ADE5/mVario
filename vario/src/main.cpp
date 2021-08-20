#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "hardware/uart/uart.h"
#include "hardware/i2cmaster/i2cmaster.h"
#include "hardware/bme280/bme280.h"
#include "hardware/ssd1306/SSD1306.h"
#include "hardware/toneAC/toneAC.h"
#include "hardware/battery/battery.h"
#include "hardware/buttons/buttons.h"

#include "utils/time_clock/time_clock.h"
#include "utils/display/display.h"
#include "vario/vario.h"
#include "menu/menu.h"
#include "menu/value_edit.h"

#include <avr/pgmspace.h>

class Base
{
	virtual void run() { printf("Base\n"); }
public:
	void doo() { run(); }
};

class Derr : public Base
{
	void run() override { printf("Derr\n"); }
};


	const PROGMEM char edit_text[] = {"Edit"};
	const PROGMEM char item0_text[] = {"IItem  "};
	const PROGMEM char item1_text[] = {"item_1"};
	const PROGMEM char item2_text[] = {"list"};
	const PROGMEM char item3_text[] = {"item_3ABC"};
	const PROGMEM char item4_text[] = {"item_4"};
	const PROGMEM char item5_text[] = {"item_5"};
	const PROGMEM char item6_text[] = {"item_6"};
	const PROGMEM char item7_text[] = {"item_7"};


int main(void) {
	
	// toneAC(880);

	time_clock_init();
	uart_init();
	i2c_init();
	pulseToneInit();
	btn_init();


	printf("F_CPU: %d MHz\n", F_CPU / 1000000);
	printf("BAT_V: %d (?)\n", get_battery_voltage());


	SSD1306driver ssd1306;
	if(ssd1306.deviceOK()) printf("SSD1306: OK\n");
	Display disp(&ssd1306);

	BME280 sensor;
	if(sensor.deviceOK()) printf("BME280: OK\n");

	menu_init(&sensor, &disp);
	Vario vario(&sensor, &disp);

	printf("Boot time: %lu ms\n", timer_get());


	MenuValueEdit<float> edit(edit_text, -12.334, -1000, 1000, 6, 6);
	
	MenuListItem item0(item0_text);
	MenuListItem item1(item1_text);
	MenuListItem item2(item2_text);
	MenuListItem item3(item3_text);
	MenuListItem item4(item4_text);
	MenuListItem item5(item5_text);
	MenuListItem item6(item6_text);
	MenuListItem item7(item7_text);

	MenuListItem *item_arr[] = {
		&edit,
		&menu_entry_back,
		&item0,
		&item1,
		&item2,
		&item3,
		&item4,
		&item5,
		&item6,
		&item7
	};
	// item_arr[0] = &item0;


	MenuList item_list(item2_text, item_arr, 10, 1);
	item_arr[2] = &item_list;

	item_list.enter();
	ssd1306.clearBuffer();


	//Dummy menu;
	
	//menu.enter();

	

	//vario.draw();
	//vario.initTimerInterrupt();


	// toneAC(1760);
	// _delay_us(100000);
	// noToneAC();

	// for(;;)
	// {
	// 	printf("time: %lu us\n", timer_get_reset());
	// 	_delay_ms(100);
	// }

	/*
	float pressure, temperature, humidity;
	sensor.setPressureSampling(BME280::SAMPLING_X16);
	sensor.setTemperatureSampling(BME280::SAMPLING_X16);
	sensor.setHumiditySampling(BME280::SAMPLING_X16);
	sensor.setFilter(BME280::FILTER_X16);
	sensor.setStandbyDuration(BME280::STANDBY_MS_0_5);

	_delay_us(100000);
	//sensor.startNormalACQ();
	int8_t res = 0;
	uint32_t acc = 0;
	for(int i = 0; i < 32; i++)
	{
		vario.measure();
		vario.draw();
		//printf ("---\n");
		//sensor.readData(&pressure, &temperature, &humidity);
		//sensor.singleMeasure(&pressure, &temperature, &humidity);
		acc += timer_get_reset();
		//printf("%d: p: %f, t: %f, h: %f\n", res, pressure, temperature, humidity);
		//printf("time: %lu ms\n", timer_get_reset());
		//_delay_us(100000);
	}
	printf("time: %lu ms\n", acc/32);
	*/

	/*
	bool show = true;
	bool show2 = true;
	
	
	BTNstatus btn;
	for(;;)
	{
		vario.measure();
		//vario_ptr->drawSec();
		//vario_ptr->drawMain();
		_delay_us(200);
		btn = debounce_btn_read();

		if(btn.btn_a) 
		{
			vario.setZeroAltitude();
		}
		if(btn.btn_b) 
		{
			ssd1306.wakeup();
			show2 = true;
		}
		if(btn.btn_c)
		{
			ssd1306.sleep();
			show = false;
			show2 = false;
		}
	}	

	
	
	int8_t ret = disp.print(
		"I'd Lo",//ve The Monkey_HEAD!",
		font_1x4,
		false,
		0,
		0,
		4,
		4
	);
	printf("disp.print: %d\n", ret);
	
	
	
	
	
	
	

	char buffer[32] = {0};
	//float acc;
	float a_acc[16] = {0};
	float alt, l_a = 0;
	for(int i = 0; i < 8024; i++) 
	{
		toneAC(880);
		_delay_us(10000);
		noToneAC();
		//for(int m = 0; m < 16; m++) 
		//{   
			sensor.singleMeasure(&pressure, &temperature, nullptr);
			alt = BME280calcAltitude(pressure);
			//a_acc[m] = alt;
			//acc = 0;
			//for(int c = 0; c < 15; c++) acc += a_acc[c];

			
			//_delay_ms(20);
		//}
		sprintf(buffer, "%6.1f", alt);//acc / 16);
		disp.print(
			buffer,
			font_1x4,
			true,
			2,
			1
		);
		//d_print(buffer);
	}
	

	
	puts("Hi!\n");


	char input;
	while(1) {
		input = getchar();
		//d_print(input);
		//printf("%c", input);		
		//buffer[i] = input;
	}
	*/
	return 0;
}