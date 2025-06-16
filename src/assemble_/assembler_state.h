#ifndef ASSEMBLER_STATE_H
#define ASSEMBLER_STATE_H

#include "symbol_table.h"

#define MAX_MEM_SIZE 2097152 // 2 * 2^20

typedef struct {
    uint32_t binaryInstructions[MAX_MEM_SIZE];
    uint8_t numInstructions;
    uint32_t currAddress;
    SymbolTable_t symbolTable;
} ARM_STATE;

#endif