#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"

void initialise ( ARM_STATE *state ) {
  memset(state->memory, 0, MEM_SIZE);
  memset(state->registers, 0, sizeof(state->registers));
  state->pc = 0x0;
  state->instruction = 0x0;
  state->pstate.N = false;
  state->pstate.Z = true;
  state->pstate.C = false;
  state->pstate.V = false;
  state->halt_flag = false;
  state->output = stdout;
}

void fetch (ARM_STATE *state) {
  //check if pc is within memory size
  if (state->pc > MEM_SIZE) {
     fprintf(stderr, "Error: PC 0x%08x is out of bounds\n", state->pc);
  }
  //get instruction
  state->instruction = (state->memory[state->pc + 3] << 24) |
                      (state->memory[state->pc + 2] << 16) |
                      (state->memory[state->pc + 1] << 8) |
                      (state->memory[state->pc]);
  if (instruction == HALT_INSTRUCTION) {
    state->halt_flag = true;
  }
}

void incrementPC (ARM_STATE *state) {
  state->pc += PC_INCREMENT;
}

// Read binary file
bool readBinary ( ARM_STATE *state, const char *filename ) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
      fprintf(stderr, "Error Opening File");
      return false;
    }
    size_t nbytes_read =  fread(state->memory, 1, MEM_SIZE, file);

    if (ferror(file) != 0) { //
      fprintf(stderr, "Error Reading File");
      fclose(file);
      return false;
  }

  printf("We have read %zu bytes from file %s into memory", nbytes_read, filename);

  fclose(file);
  return true;
}

int main(int argc, char *argv[]) {
  if (argc < 2) { 
    fprintf(stderr, "Usage: ./emulate <binary_file> [output_file]\n");
    return EXIT_FAILURE; 
  }

  const char *binary_filename = argv[1];
  const char *output_filename = (argc > 2) ? argv[2] : NULL;
  //optional output file - used when printing final state

  ARM_STATE state;
  initialise(&state);

  if (state == NULL) {
    return EXIT_FAILURE; //some sort of initialisation failure
  }

  //edit output based on optional file
  if (output_filename != NULL) {
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
      fprintf(stderr, "Failed to open output file\n");
      return EXIT_FAILURE; 
    } 
    state.output = output_file;
  } 

  //read file into memory

  //fetch, decode, execute cycle
  while (!state->halt_flag) {
    fetch(state);
    incrementPC(state);
    //decode
    //execute
  }

  //print final state

  return EXIT_SUCCESS;
}