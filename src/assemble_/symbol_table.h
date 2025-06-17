#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "defns.h"

// Struct defines an entry with a name and its address
typedef struct {
    char name[MAX_LABEL_LENGTH];
    uint32_t address;
} SymbolEntry_t;

// Using a dynamic ADT resizing array list fpr symbol table
struct SymbolTable_t {
    SymbolEntry_t *entries; 
    int no_entries;
    int cap;
};

typedef struct SymbolTable_t *SymbolTable_t;

extern SymbolTable_t makeSymbolTable( void );
extern void addSymbolEntry( SymbolTable_t st, const char *name , uint32_t address );
extern void getSymbolEntry( SymbolTable_t st, const char *name, uint32_t *address_out );
extern void destroySymbolTable( SymbolTable_t st );
extern void printSymbolTable ( SymbolTable_t st);

#endif