#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_LABEL_LENGTH 128

// Struct defines an entry with a name and its address
typedef struct {
    char name[MAX_LABEL_LENGTH];
    uint32_t address;
} SymbolEntry_t;

// Using a dynamic ADT resizing array list fpr symbol table
struct SymbolTable_t {
    SymbolEntry_t *entries; 
    int no_entries;
    int cap
};

typedef struct SymbolTable_t *SymbolTable_t;

extern SymbolTable_t makeSymboleTable( void );
extern void addSymbolEntry( SymbolTable_t st, const char *name , uint32_t address );
extern void getSymbolEntry( SymbolTable_t st, const char *name, uint32_t *address_out );
extern void destroySymbolEntry( SymbolTable_t st );

#endif