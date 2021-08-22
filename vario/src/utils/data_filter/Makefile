TOP_DIR    = ../../../

include $(TOP_DIR)/make_variables.mk

DEPS       = data_filter.h
SRCS       = data_filter.cpp
OBJECTS    = $(SRCS:.cpp=.o)
OBJECTS    := $(addprefix $(OBJ_DIR)/,$(OBJECTS))


all: $(OBJECTS)

$(OBJ_DIR)/%.o: %.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)