# Makefile
#
# Project:      Telvin Interpreter (University of Media)
# Version:      0.0.1
# Last edit:    17.10.2017



##########################################
#           Editable options             #
##########################################


## Compiler options
# DEBUG_FLAGS=-DDEBUG
CFLAGS=$(DEBUG_FLAGS) -g -Wall -c -O0

APPLICATION = telvin

## Compiling files
OBJS=main.o \
     reader.o \
		 memory.o \
     printer.o \
		 eval.o \
     environment.o \
		 selftest.o \
		 utils.o \




##########################################
#           Operations                   #
##########################################


## Linking all object files togehter
$(APPLICATION): $(OBJS)
		$(CC) -o $(APPLICATION) $(OBJS)

## Clean up
clean:
	rm -f $(OBJS) telvin

## run in eclipse
all: $(APPLICATION)
