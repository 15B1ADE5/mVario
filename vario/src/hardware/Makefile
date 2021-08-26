TOP_DIR    = ../../

include $(TOP_DIR)/make_variables.mk

all: $(OBJECTS)
	+$(MAKE) -C uart
	+$(MAKE) -C i2cmaster
	+$(MAKE) -C bme280
	+$(MAKE) -C ssd1306
	+$(MAKE) -C toneAC
	+$(MAKE) -C battery
	+$(MAKE) -C buttons
	+$(MAKE) -C led
	+$(MAKE) -C sound
