
DEVICE     = atmega328p
CLOCK      = 8000000
CC         = avr-gcc
CFLAGS     = -c -std=gnu99 -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(DEVICE) -DF_CPU=$(CLOCK)
LINK       = -Os -mmcu=$(DEVICE) -ffunction-sections -fdata-sections -Wl,--gc-sections


PROGRAMMER = arduino
BAUD       = 57600
PORT       = /dev/ttyUSB0

OBJECTS    = main.o
IMAGE      = main.hex


AVRDUDE = avrdude -F -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D 

# symbolic targets:
all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.elf: $(OBJECTS)
	$(CC) $(LINK) $(OBJECTS) -o $@

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@

flash: $(IMAGE)
	sudo $(AVRDUDE) -U flash:w:$(IMAGE):i

clean:
	rm -f main.hex main.elf $(OBJECTS)
