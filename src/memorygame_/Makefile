# Makefile for the 'emulate' target
CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
            -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
            -Wall -Werror -pedantic 
LDFLAGS = -lgpiod

# Executable target
TARGET = memory_game

# Object files for the target
OBJS = main.o end_outputs.o button_inputs.o

.SUFFIXES: .c .o

.PHONY: all clean

# Default target
all: $(TARGET)

# Build the emulator executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Generic rule to compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) *.o $(TARGET)