TOP_DIR    = ../../

include $(TOP_DIR)/make_variables.mk

all: $(OBJECTS)
	+$(MAKE) -C buffer
	+$(MAKE) -C data_filter
	+$(MAKE) -C display
	+$(MAKE) -C time_clock