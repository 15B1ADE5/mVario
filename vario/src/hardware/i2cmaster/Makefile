TOP_DIR    = ../../../

include $(TOP_DIR)/make_variables.mk

DEPS       = i2cmaster.h
SRCS       = i2cmaster.S
OBJECTS    = $(SRCS:.S=.o)
OBJECTS    := $(addprefix $(OBJ_DIR)/,$(OBJECTS))


all: $(OBJECTS)

$(OBJ_DIR)/%.o: %.S $(DEPS) | $(OBJ_DIR)
	$(CC) $(ASFLAGS) -I. $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)