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


// Vario *vario_ptr;

// uint16_t timer2_counter = 0;
// ISR(TIMER2_OVF_vect) {
// 	if( !(timer2_counter % 8) ) 
// 	{
// 		vario_ptr->measure();
// 		// toneACopt(1760);
// 		// _delay_us(5000);
// 		// noToneAC();
// 	}
	
// 	// if( !(timer2_counter % 16) ) 
// 	// {
// 	// 	vario_ptr->drawMain();
// 	// }

// 	//vario_ptr->measure();

// 	if(!timer2_counter)
// 	{
// 		//vario_ptr->drawSec();
// 		//vario_ptr->drawMain();
// 		timer2_counter = 128;
// 	}
// 	timer2_counter--;
// }

int main(void) {
	
	// toneAC(880);

	time_clock_init();
	uart_init();
	i2c_init();
	pulseToneInit();
	//btn_init();


	printf("F_CPU: %d MHz\n", F_CPU / 1000000);
	printf("BAT_V: %d (?)\n", get_battery_voltage());


	SSD1306driver ssd1306;
	if(ssd1306.deviceOK()) printf("SSD1306: OK\n");
	Display disp(&ssd1306);

	BME280 sensor;
	if(sensor.deviceOK()) printf("BME280: OK\n");

	printf("Boot time: %lu ms\n", timer_get());

	
	Vario vario(&sensor, &disp);

	vario.draw();
	vario.initTimerInterrupt();


	// toneAC(1760);
	// _delay_us(100000);
	// noToneAC();

	// for(;;)
	// {
	// 	printf("time: %lu us\n", timer_get_reset());
	// 	_delay_ms(100);
	// }

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

	// bool show = true;
	// bool show2 = true;

	/*
	BTNstatus btn;
	for(;;)
	{
		vario.measure();
		vario_ptr->drawSec();
		vario_ptr->drawMain();
		_delay_us(200);
		btn = btn_read();

		if(btn.btn_a) 
		{
			vario.setZeroAltitude();
		}
		// if(btn.btn_b) 
		// {
		// 	ssd1306.wakeup();
		// 	show2 = true;
		// }
		// if(btn.btn_c)
		// {
		// 	ssd1306.sleep();
		// 	show = false;
		// 	show2 = false;
		// }
	}	

	*/
	/*
	int8_t ret = disp.print(
		"I'd Lo",//ve The Monkey_HEAD!",
		font_7x4,
		false,
		0,
		0,
		4,
		4
	);
	printf("disp.print: %d\n", ret);
	*/
	
	/*
	
	
	
	

	char buffer[32] = {0};
	float acc;
	float a_acc[16] = {0};
	float alt, l_a = 0;
	for(int i = 0; i < 8024; i++) 
	{
		toneAC(880);
		_delay_us(10000);
		noToneAC();
		//for(int m = 0; m < 16; m++) 
		//{   
			sensor.measure(&pressure, &temperature, nullptr);
			alt = BME280calcAltitude(pressure);
			//a_acc[m] = alt;
			//acc = 0;
			//for(int c = 0; c < 15; c++) acc += a_acc[c];

			
			//_delay_ms(20);
		//}
		sprintf(buffer, "%6.1f", alt);//acc / 16);
		disp.print(
			buffer,
			font_7x4,
			true,
			2,
			1,
			3,
			3,
			3,
			false
		);
		//d_print(buffer);
	}
	*/

	/*
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