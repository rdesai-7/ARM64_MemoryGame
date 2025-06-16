#ifndef ASSEMBLER_STATE_H
#define ASSEMBLER_STATE_H

#include "symbol_table.h"

#define MAX_MEM_SIZE 2097152 // 2 * 2^20

typedef struct {
    uint32_t *binaryInstructions;
    uint8_t numInstructions;
    uint32_t currAddress;
    SymbolTable_t symbolTable;
} ARM_STATE;

// Common definitions
#define ADDR_INCREMENT 4
#define LINE_BUFFER 256
#define MAX_LABEL_LENGTH 128 
#define MAX_TOKENS 10
#define ZERO_REGISTER 31
#define STACK_POINTER 31 //is this ever used?

#endif