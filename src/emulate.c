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

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}