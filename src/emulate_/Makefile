# Makefile for the 'emulate' target
CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
            -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
            -Wall -Werror -pedantic

# Executable target
TARGET = emulate

# Object files for the target
OBJS = emulate.o instr_decode.o instr_execute.o bit_shifts.o

.SUFFIXES: .c .o

.PHONY: all clean

# Default target
all: $(TARGET)

# Build the emulator executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule to compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) *.o $(TARGET)