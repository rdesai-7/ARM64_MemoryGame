#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"
#include "pass_one.h"
#include "pass_two.h"

#define MAX_TOKENS 10

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

void tokenize(char *str, char *delim, char *tokens[], int *num_tokens) {
    char *token;
    char *save_ptr = NULL;
    int curr_token_count = 0;

    token = strtok_r(str, delim, &save_ptr);
    while (token != NULL) {
        tokens[curr_token_count] = token;
        curr_token_count++;
        token = strtok_r(NULL, delim, &save_ptr);
    }
    *num_tokens = curr_token_count;
}

bool run_pass_two( const char *filename, ARM_STATE *state) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error Opening File");
        return false;
    }

    char line_buffer[LINE_BUFFER];
    uint32_t curr_address = state->currAddress;

    while( fgets(line_buffer, sizeof(line_buffer), file) != NULL ) {
        char *line = trim_whitespace(line_buffer);
        char label_name[MAX_LABEL_LENGTH];

        if( !is_line_empty(line) && !is_label(line, label_name)) {

            //deal with .int directives
            if (is_directive(line)) {
                uint32_t N = parse_directive(line);
                state->binaryInstructions[curr_address] = N;
            } else {
                //all other instructions
                
                //Tokenise the line
                char *tokens[MAX_TOKENS];
                int num_tokens = 0;
                //char line_buffer[256];
                //strcpy(line_buffer, line);
                tokenize(line, ", ", tokens, &num_tokens);

                //parse each instruction
                //call a parse function that calls one of 4 separate parse functions depending on instruction type
            }
            curr_address += ADDR_INCREMENT;
        }
    }

    // Standard error checking
    if (ferror(file)) {
        fprintf(stderr, "Error reading from file");
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}


//instructions need to be stored somewhere
//make an assembler state type that stores the symbol table, the binary instructions (in a memory array format), and the current address

