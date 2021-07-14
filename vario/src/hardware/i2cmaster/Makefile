TOP_DIR    = ../../../

include $(TOP_DIR)/make_variables.mk

DEPS       = i2cmaster.h

ifeq ($(I2C_MODE),TWI)
   SRCS       = twimaster.c
	OBJECTS    = $(SRCS:.c=.o)
else
   SRCS       = i2cmaster.S
	OBJECTS    = $(SRCS:.S=.o)
endif

OBJECTS    := $(addprefix $(OBJ_DIR)/,$(OBJECTS))


all: $(OBJECTS)

ifeq ($(I2C_MODE),TWI)
   $(OBJ_DIR)/%.o: %.c $(DEPS) | $(OBJ_DIR)
		$(CC) $(CFLAGS) -I. $< -o $@
else
   $(OBJ_DIR)/%.o: %.S $(DEPS) | $(OBJ_DIR)
		$(CC) $(ASFLAGS) -I. $< -o $@
endif

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)