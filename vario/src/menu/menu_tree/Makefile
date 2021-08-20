TOP_DIR    = ../../../

include $(TOP_DIR)/make_variables.mk

DEPS       = menu_tree.h display_settings_tree.h sensor_settings_tree.h
SRCS       = menu_tree.cpp display_settings_tree.cpp sensor_settings_tree.cpp
OBJECTS    = $(SRCS:.cpp=.o)
OBJECTS    := $(addprefix $(OBJ_DIR)/,$(OBJECTS))


all: $(OBJECTS)

$(OBJ_DIR)/%.o: %.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)