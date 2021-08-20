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
#debug: BIN_DIR = $(TOP_DIR)/bin-debug

OBJ_DIR    = $(BIN_DIR)/obj

ELF_FILE   = $(BIN_DIR)/$(NAME).elf
HEX_FILE   = $(BIN_DIR)/$(NAME).hex


CC         = avr-gcc
CXX        = avr-g++

OPT_FLAGS  = -ffunction-sections -fdata-sections -fno-reorder-blocks  -fno-reorder-blocks-and-partition  -fno-reorder-functions -fno-toplevel-reorder -fshort-enums -mcall-prologues

CFLAGS     = -c -std=gnu99 -Os -Wall $(OPT_FLAGS)
CXXFLAGS   = -c -Os -Wall $(OPT_FLAGS)
ASFLAGS    = -c -Os -mmcu=$(DEVICE) -DF_CPU=$(F_CPU)UL -x assembler-with-cpp -Wall

CFLAGS     += -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -DBAUD=$(UART_BAUD)
CXXFLAGS   += -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -DBAUD=$(UART_BAUD)

LDFLAGS    = -Os -mmcu=$(DEVICE) $(OPT_FLAGS) -Wl,--relax,--gc-sections,-u,vfprintf -lprintf_flt -lm

#AVRDUDE = avrdude -F -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(FLASH_BAUD) -D 
AVRDUDE    = avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(FLASH_BAUD) -D

AVRSIZE    = avr-size --format=avr --mcu=$(DEVICE) -C -x

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
	CXXFLAGS += -DDEBUG
	ASFLAGS += -DDEBUG
endif