#include <stdio.h>
#include "hardware/uart/uart.h"
#include "hardware/i2cmaster/i2cmaster.h"

int main(void) {    
    uart_init();
                
    puts("Hi!\n");

    char input;
    while(1) {
        input = getchar();
        printf("%c", input);        
    }
        
    return 0;
}