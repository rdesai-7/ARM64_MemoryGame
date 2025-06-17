#ifndef ASSEMBLER_STATE_H
#define ASSEMBLER_STATE_H

#include "symbol_table.h"

typedef struct {
    uint32_t *binaryInstructions;
    uint8_t numInstructions;
    uint32_t currAddress;
    SymbolTable_t symbolTable;
} ARM_STATE;

#endif