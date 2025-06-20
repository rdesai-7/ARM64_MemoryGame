#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "defns.h"
#include "assembler_state.h"
#include "symbol_table.h"
#include "pass_one.h"
#include "pass_two.h"

void initialise( ARM_STATE *state, int numInstructions, SymbolTable_t st) {
  state->binaryInstructions = malloc(numInstructions * sizeof(uint32_t));
  state->numInstructions = numInstructions;
  state->currAddress     = 0x0;
  state->symbolTable     = st;
}

void write_binary(const char *filename, ARM_STATE *state) {
  FILE *file = fopen(filename, "wb");
  assert (file != NULL);
  uint32_t *instructions = state->binaryInstructions;

  for (int i = 0; i < state->numInstructions; i++) {
    fwrite(&instructions[i], 4, 1, file);
  }
  fclose(file);
}

int main(int argc, char **argv) {
  if (argc != 3) { 
    fprintf(stderr, "Usage: ./assemble <file_in> <file_out> \n");
    return EXIT_FAILURE; 
  }

  const char *input_filename = argv[1];
  const char *output_filename = argv[2];

  //make symbol table and run first pass
  SymbolTable_t symbol_table = makeSymbolTable();
  uint32_t program_size;
  bool pass_one = run_pass_one(input_filename, symbol_table, &program_size);
  if (!pass_one) {
    fprintf(stderr, "Error running first pass");
    destroySymbolTable(symbol_table);
    return EXIT_FAILURE;
  }

  //initialise state
  uint8_t num_instructions = program_size / ADDR_INCREMENT;
  ARM_STATE state;
  initialise(&state, num_instructions, symbol_table);

  //run second pass, passing in state
  bool pass_two = run_pass_two(input_filename, &state);
  if (!pass_two) {
    fprintf(stderr, "Error running second pass");
    destroySymbolTable(symbol_table);
    free(state.binaryInstructions);
    return EXIT_FAILURE;
  }
  
  write_binary(output_filename, &state);

  destroySymbolTable(symbol_table);
  free(state.binaryInstructions);

  return EXIT_SUCCESS;
}
