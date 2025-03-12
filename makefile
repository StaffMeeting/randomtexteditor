# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -O2
LDFLAGS := 
LDLIBS := -lncursesw

# Project structure
TARGET := editit
SRC := main.cpp global.cpp
OBJ := $(SRC:.cpp=.o)
HEADERS := display.hpp global.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# Compile main.cpp with dependencies
main.o: main.cpp display.hpp global.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile global.cpp with dependencies
global.o: global.cpp global.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(TARGET) $(OBJ)

# Phony targets (not actual files)
.PHONY: all clean
