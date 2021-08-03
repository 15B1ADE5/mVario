#include "battery.h"

#include <avr/io.h>

void adc_init()
{
	ADCSRA = 0b10000111;
	//ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );
	ADMUX |= ( 1 << REFS0 );
	ADCSRA |= ( 1 << ADEN );
	ADCSRA |= ( 1 << ADSC );
}

void adc_disable()
{
	ADCSRA = 0;
}

uint16_t adc_read(uint8_t adc_pin)
{
	ADMUX &= 0xf0;
	ADMUX |= adc_pin;

	ADCSRA |= ( 1 << ADSC );

	while( !(ADCSRA & (1<<ADIF)) );
	uint16_t res = ADC;
	ADCSRA |= ( 1 << ADIF );
	
	return res;
}

uint16_t get_battery_voltage()
{
	adc_init();
	uint32_t acc = 0;
	for (uint8_t m = 0; m < 32; m++) acc += adc_read(BATTERY_VOLTAGE_PIN);
	adc_disable();
	return acc / 32;
}