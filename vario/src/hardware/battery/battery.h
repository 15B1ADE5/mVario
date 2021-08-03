#ifndef BATTERY_H
#define BATTERY_H

#include <stdint.h>

#define BATTERY_VOLTAGE_PIN   0

void adc_init();
void adc_disable();
uint16_t adc_read(uint8_t adc_pin);

uint16_t get_battery_voltage();

#endif // BATTERY_H 