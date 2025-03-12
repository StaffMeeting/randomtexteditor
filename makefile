OBJS	= main.o global.o
SOURCE	= main.cpp global.cpp
HEADER	= display.hpp global.h
OUT	= editfile
FLAGS	= -g -c -Wall
LDLIBS	= -lncurses
CC	= g++

all:	editfile

editfile: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

global.o: global.cpp
	$(CC) $(FLAGS) global.cpp 

clean:
	rm -f $(OBJS) $(OUT)

