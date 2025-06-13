#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "symbol_table.h"

/*
void secondPass( const char *filename, SymbolTable_t st); {
  FILE *file = fopen(filename, "r");

  while (true) {
    char buffer[MAX_LABEL_LENGTH];
    fscanf(file, "%[^\n]", buffer);
    if (feof(file)) { //stop reading at end of file
      break;
    }

    int strLength = strlen(buffer) + 1;
    if (buffer[strLength - 2] != ':') { //skipping past for labels
        //deal with .int directives
        //split into tokens
        //parse each instruction
        //call a parse function that calls one of 4 separate parse functions depending on instruction type
    }
  }
}
*/

//instructions need to be stored somewhere
//make an assembler state type that stores the symbol table, the binary instructions (in a memory array format), and the current address


int main(int argc, char **argv) {
  if (argc != 3) { 
    fprintf(stderr, "Usage: ./assemble <file_in> <file_out> \n");
    return EXIT_FAILURE; 
  }

  const char *input_filename = argv[1];
  const char *output_filename = argv[2];

  return EXIT_SUCCESS;
}
