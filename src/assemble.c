#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <symbol_table_h>

#define ADDR_INCREMENT 0x4

int firstPass( const char *filename, SymbolTable_t st) {
  FILE *file = fopen(filename, "r");
  uint32_t currAddress = 0x0;
  int numInstructions = 0;

  while (true) {
    char buffer[MAX_LABEL_LENGTH];
    fscanf(file, "%[^\n]", buffer);
    if (feof(file)) { //stop reading at end of file
      break;
    }

    int strLength = strlen(buffer) + 1;
    if (buffer[strLength - 2] == ':') { //checking for labels
      char *label = strtok(buffer, ": ");
      addSymbolEntry(st, label, currAddress);
    } else {
      numInstructions++;
      currAddress += ADDR_INCREMENT;
    }
  }
  fclose(file);

  return numInstructions: //this will be stored somewhere so can be looped later
}

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
