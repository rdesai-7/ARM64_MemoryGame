CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
           -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
           -Wall -Werror -pedantic

# Executable targets
TARGETS = assemble emulate

# Object files for each target
ASSEMBLE_OBJS = assemble.o
EMULATE_OBJS = emulate.o instr_decode.o instr_execute.o bit_shifts.o

.SUFFIXES: .c .o

.PHONY: all clean

# Default target
all: $(TARGETS)

# Build the assemble executable
assemble: $(ASSEMBLE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Build the emulator executable
emulate: $(EMULATE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule to compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) *.o $(TARGETS)

	
