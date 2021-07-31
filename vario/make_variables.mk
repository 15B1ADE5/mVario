NAME       = vario

DEVICE     = atmega328p
F_CPU      = 8000000
# I2C_MODE: TWI/I2C (ATmega328p is TWI compatible)
I2C_MODE   = TWI
UART_BAUD  = 9600

PROGRAMMER = arduino
FLASH_BAUD = 57600
PORT       = /dev/ttyUSB0

BIN_DIR    = $(TOP_DIR)/bin
OBJ_DIR    = $(BIN_DIR)/obj

ELF_FILE   = $(BIN_DIR)/$(NAME).elf
HEX_FILE   = $(BIN_DIR)/$(NAME).hex


CC         = avr-gcc
CCPP       = avr-g++

CFLAGS     = -c -std=gnu99 -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -DBAUD=$(UART_BAUD)
CPPFLAGS   = -c -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -DBAUD=$(UART_BAUD)
ASFLAGS    = -c -Os -mmcu=$(DEVICE) -DF_CPU=$(F_CPU)UL -x assembler-with-cpp -Wall

LDFLAGS    = -Os -mmcu=$(DEVICE) -ffunction-sections -fdata-sections -Wl,--gc-sections

#AVRDUDE = avrdude -F -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(FLASH_BAUD) -D 
AVRDUDE    = avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(FLASH_BAUD) -D

AVRSIZE    = avr-size --format=avr --mcu=$(DEVICE) -C -x