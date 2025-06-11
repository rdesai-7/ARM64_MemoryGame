#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "symbol_table.h"

#define INITIAL_CAP 8

SymbolTable_t makeSymboleTable( void ) {
    SymbolTable_t new = malloc(sizeof(struct SymbolTable_t));
    assert(new != NULL);

    new->entries = malloc(sizeof(SymbolEntry_t) * INITIAL_CAP);
    assert(new->entries != NULL);

    new->no_entries = 0;
    new->cap = INITIAL_CAP;
    return new;
}

// Helper function to resize array
static void resize( SymbolTable_t st ) {
    if(st->no_entries + 1 >= st->cap) {
        st->cap *= 2;
        st->entries = realloc(st->entries, st->cap * sizeof(SymbolEntry_t));
        assert(st->entries != NULL);
    }
}

void addSymbolEntry( SymbolTable_t st, const char *name , uint32_t address ) {
    resize(st);
    strncpy(st->entries[st->no_entries].name, name, MAX_LABEL_LENGTH - 1);
    st->entries[st->no_entries].name[MAX_LABEL_LENGTH - 1] = '\0';
    st->entries[st->no_entries].address = address;
    st->no_entries++;
}