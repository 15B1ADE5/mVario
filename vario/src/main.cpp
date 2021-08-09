#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#include "hardware/uart/uart.h"
#include "hardware/i2cmaster/i2cmaster.h"
#include "hardware/bme280/bme280.h"
#include "hardware/ssd1306/SSD1306.h"
#include "hardware/toneAC/toneAC.h"
#include "hardware/battery/battery.h"
#include "hardware/buttons/buttons.h"

uint8_t pic1[8] = {
    0b00011000,
    0b00101100,
    0b00101100,
    0b00101100,
    0b01101110,
    0b10001111,
    0b10011111,
    0b01100110,
};

uint8_t pic2[8] = {
    0b00111100,
    0b11111111,
    0b11111111,
    0b00110000,
    0b00110000,
    0b00011000,
    0b00011000,
    0b00110000,
};

uint8_t pic3[8] = {
    0b11111111,
    0b10000001,
    0b10010001,
    0b10111001,
    0b10010001,
    0b10011001,
    0b10000001,
    0b11111111,
};

uint8_t pic4[8] = {
    0b00111100,
    0b01111110,
    0b11111110,
    0b11001111,
    0b10100111,
    0b10010111,
    0b01001110,
    0b00111100,
};


#include "hardware/ssd1306/font0.h"


SSD1306driver disp;

void d_print(const char input)
{
    uint8_t buf[5] = {0};
    uint16_t pos = ( (uint16_t)input - 32) * 4;
    for(uint16_t vbit = 0; vbit < 4; vbit++) buf[vbit] = font0[pos + vbit];
    
    //printf("%c: %d: %d\n", input, (uint8_t)input, pos);

    disp.sendData(buf, 5);
}

void d_print(char *input)
{
    disp.cmd(SSD1306_CMD_SET_COL_ADDR);
    disp.cmd( SSD1306_COL_ADDR(0) );
    disp.cmd( SSD1306_COL_ADDR(127) );

    uint8_t c = 0;
    while(input[c])
    {
        d_print(input[c]);
        c++;
    }
}


int main(void) {
    uart_init();
    i2c_init();
    pulseToneInit();
    
    //toneAC(880);
    //_delay_us(100000);
    //toneAC(1760);
    //_delay_us(100000);
    //noToneAC();
    //pulseToneSet(1760, 6, 3125);
    //pulseToneStart();

    printf("BAT_V: %d (?)\n", get_battery_voltage());

    /*
    _delay_ms(2000);
    _delay_ms(8000);
    pulseToneStop();
    */


    if(disp.deviceOK()) printf("SSD1306: OK\n");

       BME280 sensor;
    if(sensor.deviceOK()) printf("BME280: OK\n");

	float pressure, temperature, humidity;

    sensor.setPressureSampling(BME280::SAMPLING_X2);
    sensor.setTemperatureSampling(BME280::SAMPLING_X1);
    sensor.setFilter(BME280::FILTER_X16);

    char buffer[32] = {0};
    float acc;
    float a_acc[16] = {0};
    float alt, l_a = 0;
    for(int i = 0; i < 1024; i++) 
    {
        for(int m = 0; m < 16; m++) 
        {   
            sensor.measure(&pressure, &temperature, nullptr);
            alt = BME280calcAltitude(pressure);
            a_acc[m] = alt;
            acc = 0;
            for(int c = 0; c < 15; c++) acc += a_acc[c];

            
            _delay_ms(20);
        }
        sprintf(buffer, "%f", acc / 16);
        d_print(buffer);
    }
    noToneAC();
    

    puts("Hi!\n");
    
    char input;
    int i = 0;
    while(1) {
        input = getchar();
        d_print(input);
        //printf("%c", input);        
        //buffer[i] = input;
        //disp.cmd(SSD1306_CMD_SET_COL_ADDR);
        //disp.cmd( SSD1306_COL_ADDR(24) );
        //disp.cmd( SSD1306_COL_ADDR(96) );
        //printf("%d", disp.sendData( ( (uint8_t *)&input), 1));
        
        i++;
    }
        
    return 0;
}