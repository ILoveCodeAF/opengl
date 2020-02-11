#.RECIPEPREFIX = \t
CXX := g++
CC := gcc
RM := rm -f
MKDIR := mkdir -p

INCLUDE_DIR := .
SRC_DIR := .
DEP_DIR := .depend
OBJ_DIR := .object

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(patsubst $(SRC_DIR)/%.cpp, $(DEP_DIR)/%.d,$(SRCS))

CFLAGS := -Wall -I$(INCLUDE_DIR)
LDLIBS := -lGL -lGLEW -lglfw
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
POSTCOMPILE = mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d


TARGET := opengl

all: option $(TARGET)
	@echo "done."

option:
	@echo "compiling..."

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(CFLAGS) $(LDLIBS) $(OBJS)

# $@= target_name; $< first_dependency
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(DEP_DIR) $(OBJ_DIR)
	$(CXX) -o $@ $(DEPFLAGS) $(CFLAGS) $(LDLIBS) -c $<
	$(POSTCOMPILE)

$(OBJ_DIR):
	@mkdir -p $@

$(DEP_DIR):
	@mkdir -p $@

# check if dependencies exsit
$(DEPS):

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)

distclean: clean
	$(RM) $(TARGET)

include $(wildcard $(DEPS))
.PHONY: all clean distclean option
