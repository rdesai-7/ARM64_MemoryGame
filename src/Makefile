# Makefile for the 'assemble' target
CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
            -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
            -Wall -Werror -pedantic

# Executable target
TARGET = assemble

# Object files for the target
OBJS = assemble.o pass_one.o pass_two.o symbol_table.o

.SUFFIXES: .c .o

.PHONY: all clean

# Default target
all: $(TARGET)

# Build the assemble executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule to compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) *.o $(TARGET)