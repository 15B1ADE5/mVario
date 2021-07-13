TOP_DIR    = ../../../

include $(TOP_DIR)/make_variables.mk

DEPS       = uart.h
SRCS       = uart.c
OBJECTS    = $(SRCS:.c=.o)
OBJECTS    := $(addprefix $(OBJ_DIR)/,$(OBJECTS))


all: $(OBJECTS)

$(OBJ_DIR)/%.o: %.c $(DEPS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)