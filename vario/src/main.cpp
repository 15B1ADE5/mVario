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

int main(void) {
    //PORTC = (1<<PC5)|(1<<PC4);

    uart_init();
    i2c_init();

    
    toneAC(880);
    _delay_us(100000);
    toneAC(1760);
    _delay_us(100000);
    noToneAC();
    

    printf("BAT_V: %d (?)\n", get_battery_voltage());


    uint8_t buffer[1024];

    //uint8_t buffer2[1024];

    SSD1306driver disp;

    for(int gg = 0; gg < 10; gg++)
    {
    for(int i = 0; i < 1024; i++) buffer[i] = 0x00;//(uint8_t)0x100 - (uint8_t) (i % 0x100);

    for(int l = 0; l < 2; l++)
    {
        for(int r = 0; r < 8; r++)
        {
            for(int p = 0; p < 8; p++)
            {
                buffer[l*256 + r*16 + p] = pic4[p];
                buffer[128 + l*256 + 8 + r*16 + p] = pic3[p];
                buffer[512 + l*256 + r*16 + p] = pic2[p];
                buffer[512 + 128 + l*256 + 8 + r*16 + p] = pic1[p];
            }
        }
    }
        disp.sendFramebuffer(buffer);
        _delay_us(2000000);

    for(int i = 0; i < 1024; i++) buffer[i] = 0x00;//(uint8_t)0x100 - (uint8_t) (i % 0x100);

    for(int l = 0; l < 2; l++)
    {
        for(int r = 0; r < 8; r++)
        {
            for(int p = 0; p < 8; p++)
            {
                buffer[l*256 + 8 + r*16 + p] = pic1[p];
                buffer[128 + l*256 + r*16 + p] = pic2[p];
                buffer[512 + l*256 + 8 + r*16 + p] = pic3[p];
                buffer[512 + 128 + l*256 + r*16 + p] = pic4[p];
            }
        }
    }
        disp.sendFramebuffer(buffer);
        _delay_us(2000000);
    }


    puts("Hi!\n");
    
    char input;
    int i = 0;
    while(1) {
        input = getchar();
        printf("%c", input);        
        buffer[i] = input;
        disp.sendFramebuffer(buffer);
        i++;
    }
        
    return 0;
}