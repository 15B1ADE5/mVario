#ifndef UART_H
#define UART_H

// UART example from https://github.com/tuupola/avr_demo/tree/master/blog/simple_usart

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <stdio.h>
#include <util/setbaud.h>

void uart_putc(char c, FILE *stream);
char uart_getc(FILE *stream);

void uart_init(void);


#endif // UART_H