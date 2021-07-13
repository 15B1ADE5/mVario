TOP_DIR    = ../

include $(TOP_DIR)/make_variables.mk

SRCS       = main.c
OBJECTS    = $(SRCS:.c=.o)
OBJECTS    := $(addprefix $(OBJ_DIR)/,$(OBJECTS))


all: $(OBJECTS)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJECTS)

test:
	@ echo $(OBJECTS)