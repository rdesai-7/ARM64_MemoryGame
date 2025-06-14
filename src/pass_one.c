#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "symbol_table.h"
#include "pass_one.h"

char* trim_whitespace(char *s) {

    while( isspace(*s) ) s++;
    if(*s == '\0') return s;

    char *end = s + strlen(s) - 1;
    // Removes whitespace from the end
    while ( (end > s && isspace(*end)) ) end--;
    *(end + 1) = '\0';
    return s;
}

bool is_line_empty(const char *line) {
    if (line == NULL || *line == '\0') return true;
    return false;
}

bool is_label(const char *line, char *label_name_out) {
    // Trim whitespace should already be called

    // No need to check first character is valid label character  
    size_t len = strlen(line);
    if (len - 1 >= MAX_LABEL_LENGTH) {
        fprintf(stderr, "Label too long to fit");
        return false;
    }

    // Check last character is colon
    // Label_name_out will have enough space since it will be initialised with MAX_LABEL_LENGTH
    if( line[len - 1] == ':' ) {
        strncpy( label_name_out, line, len -1);
        label_name_out[len - 1] = '\0';
        return true;
    }

    return false;
}

bool run_pass_one( const char *filename, SymbolTable_t st, uint32_t *program_size ) {
    assert(program_size != NULL);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
      fprintf(stderr, "Error Opening File");
      return false;
    }

    char line_buffer[LINE_BUFFER];
    uint32_t curr_address = 0x0;

    while( fgets(line_buffer, sizeof(line_buffer), file) != NULL ) {
        char *line = trim_whitespace(line_buffer);
        // ISSUE: line may still end in a NEWLINE CHAR? 

        if( !is_line_empty(line) ) {
            char label_name[MAX_LABEL_LENGTH];
            
            // Check if the line is a label and stores it in label_name
            if( is_label( line, label_name ) ) {
                // Add label and address to symbol table should make function return a bool for error checking
                addSymbolEntry( st, label_name, curr_address );
            } else {
                // If its not a label or empty line then it will be an instruction or a directive and these are all 4 bytes so increment by 4
                curr_address += ADDR_INCREMENT;
            }
        }
    }
    
    // Standard error checking
    if (ferror(file)) {
        fprintf(stderr, "Error reading from file");
        fclose(file);
        return false;
    }

    fclose(file);
    // Store the size of the program
    // program size / ADDR_INCREMENT gives the number of instructions
    *program_size = curr_address;
    return true;
}


