#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
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

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}