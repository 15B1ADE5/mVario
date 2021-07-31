TOP_DIR    = ./

include $(TOP_DIR)/make_variables.mk

OBJECTS    = $(wildcard $(OBJ_DIR)/*.o)


all: usage

build:
	+$(MAKE) -C src

flash: $(HEX_FILE)
	$(AVRDUDE) -U flash:w:$(HEX_FILE):i

$(HEX_FILE): $(ELF_FILE)
	avr-objcopy -O ihex -R .eeprom $< $@

usage: $(ELF_FILE)
	$(AVRSIZE) $<

$(ELF_FILE): build
	$(CCPP) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -fr $(BIN_DIR)
