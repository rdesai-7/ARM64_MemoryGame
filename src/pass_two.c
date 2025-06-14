#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "symbol_table.h"
#include "pass_one.h"
#include "pass_two.h"

bool is_directive(char *s) {
    return strncmp(s, ".int", 4) == 0;
}

uint32_t parse_directive(char *line) {
    // PRE: is_directive(line) returns true
    int value;
    // sscanf parses hex and decimal values using %i
    assert(sscanf(line, ".int %i", &value) == 1);
    return (uint32_t) value;
}

void run_pass_two( const char *filename, SymbolTable_t st) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error Opening File");
        exit(1);
    }

    char line_buffer[LINE_BUFFER];
    uint32_t curr_address = 0x0;

    while( fgets(line_buffer, sizeof(line_buffer), file) != NULL ) {
        char *line = trim_whitespace(line_buffer);
        char label_name[MAX_LABEL_LENGTH];

        if( !is_line_empty(line) && !is_label(line,label_name)) {

            //deal with .int directives
            if (is_directive(line)) {
                uint32_t N = parse_directive(line);
                // idk what to do here...
                // store N at the curr_address i think. how do you write that in binary though?
            }

            //split into tokens

            //parse each instruction

            //call a parse function that calls one of 4 separate parse functions depending on instruction type


            curr_address += ADDR_INCREMENT;
        }
    }

    // Standard error checking
    if (ferror(file)) {
        fprintf(stderr, "Error reading from file");
        fclose(file);
        exit(1);
    }

    fclose(file);
}


//instructions need to be stored somewhere
//make an assembler state type that stores the symbol table, the binary instructions (in a memory array format), and the current address

