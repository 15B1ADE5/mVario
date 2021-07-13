DEVICE     = atmega328p
CLOCK      = 8000000
UART_BAUD  = 9600
CC         = avr-gcc
CFLAGS     = -c -std=gnu99 -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) -DBAUD=$(UART_BAUD)
LINK       = -Os -mmcu=$(DEVICE) -ffunction-sections -fdata-sections -Wl,--gc-sections

PROGRAMMER = arduino
BAUD       = 57600
PORT       = /dev/ttyUSB0

BIN_DIR    = $(TOP_DIR)/bin
OBJ_DIR    = $(BIN_DIR)/obj


AVRDUDE = avrdude -F -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D 