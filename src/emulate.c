#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"

void initialise ( ARM_STATE *state ) {
  memset(state->memory, 0, MEM_SIZE);
  memset(state->registers, 0, sizeof(state->registers));
  state->pc = 0x0;
  state->pstate.N = false;
  state->pstate.Z = true;
  state->pstate.C = false;
  state->pstate.V = false;
  state->halt_flag = false;
  state->output = stdout;
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

  //print final state

  return EXIT_SUCCESS;
}
