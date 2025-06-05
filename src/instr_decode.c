#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"
#include "instr_decode.h"

//DECODE FUNCTIONS
void decodeDPImmediate ( DECODED_INSTR *decoded, uint32_t instr ) {
  //check opi to see if arithmetic or wide move
  decoded->sf = get_bits(instr, 31, 31);
  decoded->opc = get_bits(instr, 30, 29);
  decoded->opi = get_bits(instr, 25, 23);
  decoded->rd = get_bits(instr, 4, 0);
}

void decodeDPRegister ( DECODED_INSTR *decoded, uint32_t instruction ) {
  //check m for multiply
  //check bit 24 for arithmetic/logic
}
void decodeLoadStore ( DECODED_INSTR *decoded, uint32_t instruction ) {}
void decodeBranch ( DECODED_INSTR *decoded, uint32_t instruction ) {}

void decode (ARM_STATE *state) {
  uint32_t instruction = state->instruction;
  DECODED_INSTR *decoded = state->decoded;
  if (instruction == HALT_INSTRUCTION) {
    return;
  } else {
    instr_type type = getInstructionType(instruction);
    // POTENTIALLY PASS POINTERS TO FUNCTION FOR EFFICIENCY 
    func_decode decodeFunctions[] = {decodeDPImmediate, decodeDPRegister, decodeLoadStore, decodeBranch};
    decodeFunctions[type](decoded, instruction);
    state->instruction_type = type;
  }
}