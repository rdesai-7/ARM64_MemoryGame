#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"

void initialise ( ARM_STATE *state ) {
  memset(state->memory, 0, MEM_SIZE);
  memset(state->registers, 0, sizeof(state->registers));
  memset(state->decoded, 0, sizeof(state->decoded));
  state->pc = 0x0;
  state->instruction = 0x0;
  state->instruction_type = 0x0;
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

//DECODE FUNCTIONS
void decodeDPImmediate ( DECODED_INSTR *decoded, uint32_t instr ) {
  //check opi to see if arithmetic or wide move
  decoded->sf = get_bits(instr, 31, 31)
  decoded->opc = get_bits(instr, 30, 29)
  decoded->opi = get_bits(instr, 25, 23)
  decoded->rd = get_bits(instr, 4, 0)
}

void decodeDPRegister ( DECODED_INSTR *decoded, uint32_t instruction ) {
  //check m for multiply
  //check bit 24 for arithmetic/logic
}
void decodeLoadStore ( DECODED_INSTR *decoded, uint32_t instruction ) {}
void decodeBranch ( DECODED_INSTR *decoded, uint32_t instruction ) {}

instr_type getInstructionType (uint32t instr) {
  if (get_bits(instr, 28, 26) == 0x4) {
    return DP_IMMEDIATE;
  } else if (get_bits(instr, 27, 5) == 0x5) {
    return DP_REGISTER;
  } else if (get_bits(instr, 28, 26) == 0x5) {
    return BRANCH;
  } else {
    return LOAD_STORE;
  }
}

//Helper function to extract bits
uint32_t get_bits(uint32_t source, int start_bit, int end_bit) {
    int length = start_bit - end_bit + 1;
    uint32_t mask = (1U << length) - 1;
    return (source >> end_bit) & mask;
}

void decode (ARM_STATE *state) {
  uint32t instruction = state->instruction;
  DECODED_INSTR *decoded = state->decoded;
  if (instruction == HALT_INSTRUCTION) {
    continue;
  } else {
    instr_type type = getInstructionType(instruction);
    func_decode decodeFunctions[] = {decodeDPImmediate, decodeDPRegister, decodeLoadStore, decodeBranch};
    decodeFunctions[type](decoded, instruction);
    state->instruction_type = type;
  }
}

//EXECUTE FUNCTIONS
void execute(ARM_STATE *state) {
    func_execute executeFunctions[] = {executeDPImmediate, executeDPRegister, executeLoadStore, executeBranch};
    executeFunctions[state->instruction_type](state);
}

void executeDPImmediate( ARM_STATE *state) {}
void executeDPRegister( ARM_STATE *state) {}
void executeLoadStore( ARM_STATE *state) {}
void executeBranch( ARM_STATE *state) {}


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
    decode(state);
    //execute
  }

  //print final state

  return EXIT_SUCCESS;
}