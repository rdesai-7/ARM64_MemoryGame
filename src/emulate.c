#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"
#include "instr_decode.h"
#include "instr_execute.h"

//Helper function to extract bits
uint32_t get_bits( uint32_t source, int start_bit, int end_bit ) {
    int length = start_bit - end_bit + 1;
    uint32_t mask = (1U << length) - 1;
    return (source >> end_bit) & mask;
}

uint64_t get_reg_val(ARM_STATE *state, uint8_t reg_id, bool is_64_bit) {
    if (reg_id == NUM_GPR) {
        return 0;
    }
    if (reg_id > NUM_GPR) {
        fprintf(stderr, "Index out of bounds %u for read.\n", reg_id);
        return 0;
    }

    if (is_64_bit) { 
        return state->registers[reg_id];
    } else {
        return (uint32_t)state->registers[reg_id];
    }
}

void set_reg_val(ARM_STATE *state, uint8_t reg_id, uint64_t value, bool is_64_bit) {
    if (reg_id == NUM_GPR) { // Writes to Zero Register are ignored
        fprintf(state->output, "zero reg found, so set is ignored"); // REMOVE TS
        return;
    }
    if (reg_id > NUM_GPR) {
        fprintf(stderr, "Index out of bounds %u for write.\n", reg_id);
        return; 
    }

    fprintf(state->output, "I AM SETTING SOMETHING TO A REGISTER NOW!"); // REMOVE TS
    if (is_64_bit) {
        state->registers[reg_id] = value;
    } else {
        state->registers[reg_id] = (uint64_t)((uint32_t)value); // Zeros upper bits
    }
}

uint64_t loadMemory(ARM_STATE *state, uint32_t addr, bool is_64_bit) {
  if (is_64_bit) {
    return (uint64_t) ((uint64_t)state->memory[addr + 7] << 56) |
           ((uint64_t)state->memory[addr + 6] << 48) |
           ((uint64_t)state->memory[addr + 5] << 40) |
           ((uint64_t)state->memory[addr + 4] << 32) |
           ((uint64_t)state->memory[addr + 3] << 24) | 
           ((uint64_t)state->memory[addr + 2] << 16) |
           ((uint64_t)state->memory[addr + 1] << 8) |
           ((uint64_t)state->memory[addr]);
  } else {
    return (uint32_t) ((uint32_t)state->memory[addr + 3] << 24) |
                    ((uint32_t)state->memory[addr + 2] << 16) |
                    ((uint32_t)state->memory[addr + 1] << 8) |
                    ((uint32_t)state->memory[addr]);
  }
} //NO TYPECASTING NEEDED??

void storeMemory(ARM_STATE *state, uint32_t addr, bool is_64_bit, uint64_t value) {
  if (is_64_bit) {
    state->memory[addr + 7] = value >> 56;
    state->memory[addr + 6] = (value << 8) >> 56;
    state->memory[addr + 5] = (value << 16) >> 56;
    state->memory[addr + 4] = (value << 24) >> 56;
    state->memory[addr + 3] = (value << 32) >> 56;
    state->memory[addr + 2] = (value << 40) >> 56;
    state->memory[addr + 1] = (value << 48) >> 56;
    state->memory[addr] = (value << 56) >> 56;
  } else {
    value = (uint32_t)value;
    state->memory[addr + 3] = value >> 24;
    state->memory[addr + 2] = (value << 8) >> 24;
    state->memory[addr + 1] = (value << 16) >> 24;
    state->memory[addr] = (value << 24) >> 24;
  }
}

void initialise ( ARM_STATE *state ) {
  memset(state->memory, 0, MEM_SIZE);
  memset(state->registers, 0, sizeof(state->registers));
  memset(&state->decoded, 0, sizeof(state->decoded));
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
     fprintf(stderr, "Error: PC 0x%08lx is out of bounds\n", state->pc);
  }
  //get instruction
  state->instruction = (state->memory[state->pc + 3] << 24) |
                      (state->memory[state->pc + 2] << 16) |
                      (state->memory[state->pc + 1] << 8) |
                      (state->memory[state->pc]);
  fprintf(stdout, "%x", state->instruction);
  if (state->instruction == HALT_INSTRUCTION) {
    state->halt_flag = true;
  }
}

void incrementPC (ARM_STATE *state) {
  if (!state->halt_flag) {
    state->pc += PC_INCREMENT;
  }
}

instr_type getInstructionType (uint32_t instr) {
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

void printBinary ( ARM_STATE *state ) {
  fprintf(state->output, "Registers:\n");
  for (int i = 0; i < NUM_GPR; i++) {
    fprintf(state->output, "X%02d    = %016lx\n", i, state->registers[i]);
  }
  fprintf(state->output, "PC     = %016lx\n", state->pc);
  fprintf(state->output, "PSTATE : %c%c%c%c\n",
    state->pstate.N ? 'N' : '-',
    state->pstate.Z ? 'Z' : '-',
    state->pstate.C ? 'C' : '-',
    state->pstate.V ? 'V' : '-');
  fprintf(state->output, "Non-zero memory:\n");
  for (int i = 0; i <= state->pc ; i += 4) {
    uint32_t nextMemWord = (state->memory[i + 3] << 24) |
                           (state->memory[i + 2] << 16) |
                           (state->memory[i + 1] << 8)  |
                           (state->memory[i]);
    if (nextMemWord != 0) {
      fprintf(state->output, "0x%08x: %08x\n", i, nextMemWord);
    }
  }
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

  // if (state == NULL) {
  //  return EXIT_FAILURE; //some sort of initialisation failure
  // }

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
  readBinary(&state, binary_filename);

  //fetch, decode, execute cycle
  while (!state.halt_flag) {
    //fprintf(state.output,"RUNNING FDE CYCLE NUMBER %x \n",i);//REMOVE TS
    fetch(&state);
    
    decode(&state);
    //fprintf(state.output,"decoded simm26 value is %d \n", state.decoded.simm26);
    execute(&state);
    if (state.instruction_type != BRANCH) {
      incrementPC(&state);
    }
    //fprintf(state.output,"the pc value is: %lx \n",state.pc); //REMOVE TS
  }

  //print final state (into stdout or output file)
  printBinary(&state);

  return EXIT_SUCCESS;
}