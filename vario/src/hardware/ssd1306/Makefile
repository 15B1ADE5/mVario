TOP_DIR    = ../../../

include $(TOP_DIR)/make_variables.mk

DEPS       = SSD1306.h
SRCS       = SSD1306.cpp
OBJECTS    = $(SRCS:.cpp=.o)
OBJECTS    := $(addprefix $(OBJ_DIR)/,$(OBJECTS))


all: $(OBJECTS)

$(OBJ_DIR)/%.o: %.cpp $(DEPS) | $(OBJ_DIR)
	$(CCPP) $(CPPFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)