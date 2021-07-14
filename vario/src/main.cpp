#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#include "hardware/uart/uart.h"
#include "hardware/i2cmaster/i2cmaster.h"
#include "hardware/ssd1306/SSD1306.h"

#define BME280_ADDRESS (0x76)
enum {
  BME280_REGISTER_DIG_T1 = 0x88,
  BME280_REGISTER_DIG_T2 = 0x8A,
  BME280_REGISTER_DIG_T3 = 0x8C,

  BME280_REGISTER_DIG_P1 = 0x8E,
  BME280_REGISTER_DIG_P2 = 0x90,
  BME280_REGISTER_DIG_P3 = 0x92,
  BME280_REGISTER_DIG_P4 = 0x94,
  BME280_REGISTER_DIG_P5 = 0x96,
  BME280_REGISTER_DIG_P6 = 0x98,
  BME280_REGISTER_DIG_P7 = 0x9A,
  BME280_REGISTER_DIG_P8 = 0x9C,
  BME280_REGISTER_DIG_P9 = 0x9E,

  BME280_REGISTER_DIG_H1 = 0xA1,
  BME280_REGISTER_DIG_H2 = 0xE1,
  BME280_REGISTER_DIG_H3 = 0xE3,
  BME280_REGISTER_DIG_H4 = 0xE4,
  BME280_REGISTER_DIG_H5 = 0xE5,
  BME280_REGISTER_DIG_H6 = 0xE7,

  BME280_REGISTER_CHIPID = 0xD0,
  BME280_REGISTER_VERSION = 0xD1,
  BME280_REGISTER_SOFTRESET = 0xE0,

  BME280_REGISTER_CAL26 = 0xE1, // R calibration stored in 0xE1-0xF0

  BME280_REGISTER_CONTROLHUMID = 0xF2,
  BME280_REGISTER_STATUS = 0XF3,
  BME280_REGISTER_CONTROL = 0xF4,
  BME280_REGISTER_CONFIG = 0xF5,
  BME280_REGISTER_PRESSUREDATA = 0xF7,
  BME280_REGISTER_TEMPDATA = 0xFA,
  BME280_REGISTER_HUMIDDATA = 0xFD
};




int main(void) {
    //PORTC = (1<<PC5)|(1<<PC4);

    uart_init();
    //i2c_init();
    
    uint8_t buffer[1024];
    for(int i = 0; i < 1024; i++) buffer[i] = 0x1;

    SSD1306 myDisplay;
    myDisplay.sendFramebuffer(buffer);

    //myDisplay.invert(1);
    //myDisplay.invert(0);


    // if (0 == i2c_start(0x76)) {
    //     printf("Found device: 0x76!\n");
    // }
    // if (0 == i2c_start(0x3C)) {
    //     printf("Found device: 0x3C!\n");
    // }

    puts("Hi!\n");
    for (uint8_t adr = 0; adr < 128; adr++) {
        printf("Scan: 0x%x (%x)\n", adr, (adr<<1 | I2C_READ));
        if (0 == i2c_start(adr<<1)) {
            printf("Found device: 0x%x!\n", adr);
            i2c_stop();
        }
        _delay_ms(100);
    }

    char input;
    while(1) {
        input = getchar();
        printf("%c", input);        
    }
        
    return 0;
}