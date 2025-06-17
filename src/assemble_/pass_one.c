#include "pass_one.h"

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

        if( !is_line_empty(line) ) {
            char label_name[MAX_LABEL_LENGTH];
            
            // Check if line is a label and stores it in label_name
            if( is_label( line, label_name ) ) {
                addSymbolEntry( st, label_name, curr_address );
            } else {
                // line must be a 4-byte instruction or directive, so increment by 4
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
    *program_size = curr_address;
    return true;
}


