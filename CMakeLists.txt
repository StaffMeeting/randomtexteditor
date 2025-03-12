cmake_minimum_required(VERSION 3.10)
project(MyApp)

# Find the ncurses library
find_package(Curses REQUIRED)

# List source files
set(SOURCES
  src/main.cpp
  src/display.hpp
  src/global.cpp
  src/global.h
)

# Create executable
add_executable(MyApp ${SOURCES})

# Include project headers and ncurses headers
target_include_directories(MyApp
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CURSES_INCLUDE_DIRS}  # Add ncurses headers
)

# Link ncurses library to the executable
target_link_libraries(MyApp
  PRIVATE
    ${CURSES_LIBRARIES}  # Link ncurses
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
