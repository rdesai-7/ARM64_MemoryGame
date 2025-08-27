#include "symbol_table.h"

#define INITIAL_CAP 1

SymbolTable_t makeSymbolTable( void ) {
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

void getSymbolEntry( SymbolTable_t st, const char *name, uint32_t *address_out ) {
    for(int i = 0; i < st->no_entries; i++) {
        if(strcmp(st->entries[i].name, name) == 0) {
            // Stores the value of the address in the address of address_out
            *address_out = st->entries[i].address;
            return;
        }
    }
    fprintf(stderr, "Name not found in lookup table");
}

void destroySymbolTable( SymbolTable_t st ) {
    free(st->entries);
    free(st);
}

void printSymbolTable ( SymbolTable_t st) {
    fprintf(stdout, "PRINTING SYMBOL TABLE.. \n");
    fprintf(stdout, "Number of entries: %d \n", st->no_entries);
    for (int i = 0; i < st->no_entries; i++) {
        fprintf(stdout, "Name: %s, Address: %d \n", st->entries[i].name, st->entries[i].address ); //DEBUG
    }
}