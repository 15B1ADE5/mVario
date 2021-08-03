#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#include "hardware/uart/uart.h"
#include "hardware/i2cmaster/i2cmaster.h"
#include "hardware/bme280/bme280.h"
#include "hardware/ssd1306/SSD1306.h"
#include "hardware/toneAC/toneAC.h"


int main(void) {
    //PORTC = (1<<PC5)|(1<<PC4);

    toneAC(2500);
    _delay_us(60000);
    noToneAC();

    uart_init();
    i2c_init();
    
    BME280driver sensor;
    if(sensor.deviceOK()) printf("BME280: OK\n");


    uint8_t buffer[1024];
    for(int i = 0; i < 1024; i++) buffer[i] = 0x00;

    SSD1306 myDisplay;
    myDisplay.sendFramebuffer(buffer);

    // myDisplay.invert(1);
    // myDisplay.invert(0);


    // if (0 == i2c_start(0x76)) {
    //     printf("Found device: 0x76!\n");
    // }
    // if (0 == i2c_start(0x3C)) {
    //     printf("Found device: 0x3C!\n");
    // }

    puts("Hi!\n");
    // for (uint8_t adr = 0; adr < 128; adr++) {
    //     printf("Scan: 0x%x (%x)\n", adr, (adr<<1 | I2C_READ));
    //     if (0 == i2c_start(adr<<1)) {
    //         printf("Found device: 0x%x!\n", adr);
    //         i2c_stop();
    //     }
    //     _delay_ms(100);
    // }

    char input;
    int i = 0;
    while(1) {
        input = getchar();
        printf("%c", input);        
        buffer[i] = input;
        myDisplay.sendFramebuffer(buffer);
        i++;
    }
        
    return 0;
}