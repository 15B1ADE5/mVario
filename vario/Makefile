TOP_DIR    = ./

include $(TOP_DIR)/make_variables.mk

IMAGE      = main.hex
OBJECTS    = $(wildcard $(OBJ_DIR)/*.o)


all:
	+$(MAKE) -C src

flash: $(BIN_DIR)/$(IMAGE)
	$(AVRDUDE) -U flash:w:$(BIN_DIR)/$(IMAGE):i

$(BIN_DIR)/%.hex: $(BIN_DIR)/%.elf
	avr-objcopy -O ihex -R .eeprom $< $@

$(BIN_DIR)/%.elf: all
	$(CCPP) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -fr $(BIN_DIR)
