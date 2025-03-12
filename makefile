# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -O2 -I./src
LDFLAGS := 
LDLIBS := -lncurses

# Dependency flags
DEPFLAGS = -MMD -MP

# Project structure
SRC_DIR := src
BUILD_DIR := build
TARGET := program

# File lists
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)
HEADERS := $(wildcard $(SRC_DIR)/*.h $(SRC_DIR)/*.hpp)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# Compile cpp files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# Include dependency files
-include $(DEP)

# Clean build artifacts
clean:
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)

# Debug build
debug: CXXFLAGS += -g
debug: clean all

# Install and uninstall
install: $(TARGET)
	install -m 0755 $(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Phony targets
.PHONY: all clean debug install uninstall
