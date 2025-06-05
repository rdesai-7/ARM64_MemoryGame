#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"
#include "instr_decode.h"

//DECODE FUNCTIONS
void decodeDPImmediate ( DECODED_INSTR *decoded, uint32_t instr ) {
  decoded->sf  = get_bits(instr, 31, 31);
  decoded->opc = get_bits(instr, 30, 29);
  decoded->opi = get_bits(instr, 25, 23);
  decoded->rd  = get_bits(instr, 4, 0);
  //check opi to see if arithmetic or wide move
  if (decoded->opi == 0x2) {
    decoded->sh    = get_bits(instr, 22, 22);
    decoded->imm12 = get_bits(instr, 21, 10);
    decoded->rn    = get_bits(instr, 9, 5);
  } else if (decoded->opi == 0x5) {
    decoded->hw    = get_bits(instr, 22, 21);
    decoded->imm16 = get_bits(instr, 20, 5);
  }
}

void decodeDPRegister ( DECODED_INSTR *decoded, uint32_t instr ) {
  decoded->sf  = get_bits(instr, 31, 31);
  decoded->opc = get_bits(instr, 30, 29);
  decoded->M   = get_bits(instr, 28, 28);
  decoded->opr = get_bits(instr, 24, 21);
  decoded->rm  = get_bits(instr, 20, 16);
  decoded->rn  = get_bits(instr, 9, 5);
  decoded->rd  = get_bits(instr, 4, 0);
  //check m for multiply
  //check bit 24 for arithmetic/logic
  if (decoded->M && decoded->opr == 0x8) {
    decoded->x  = get_bits(instr, 15, 15);
    decoded->ra = get_bits(instr, 14, 10);
  } else { //arith/logic
    decoded->shift = get_bits(instr, 23, 22);
    if ((decoded->opr >> 3) == 0) { //store N if logical
      decoded->N = get_bits(instr, 21, 21);
    }
  }
}

//potentially have an instr_subtype field in decoded instruction to avoid checking again in execute?

void decodeLoadStore ( DECODED_INSTR *decoded, uint32_t instr ) {
  decoded->sf = get_bits(instr, 30, 30);
  decoded->rt = get_bits(instr, 4, 0);
  //check if sdt or load literal
  if (get_bits(instr, 31, 31)) { //SDT
    decoded->U      = get_bits(instr, 24, 24);
    decoded->L      = get_bits(instr, 22, 22);
    decoded->offset = get_bits(instr, 21, 10); //offset can be decomposed in execute based on addressing mode?
    decoded->xn     = get_bits(instr, 9, 5);
  } else { //Load Literal
    decoded->simm19 = get_bits(instr, 23, 5);
  }
}

void decodeBranch ( DECODED_INSTR *decoded, uint32_t instr ) {
  decoded->branch_type = get_bits(instr, 31, 29);
  switch (decoded->branch_type) {
    case 0x0: //unconditional
      decoded->simm26 = get_bits(instr, 25, 0);
      break;
    case 0x6: //register
      decoded->xn = get_bits(instr, 9, 5);
      break;
    case 0x2: //conditional
      decoded->simm19 = get_bits(instr, 23, 5);
      decoded->cond = get_bits(instr, 3, 0);
      break;
    default:
      break;
  }
  //these branch types could be turned into an enum + stored in decoded?
}

void decode (ARM_STATE *state) {
  uint32_t instruction = state->instruction;
  DECODED_INSTR decoded = state->decoded;
  if (instruction == HALT_INSTRUCTION) {
    return;
  } else {
    instr_type type = getInstructionType(instruction);
    // POTENTIALLY PASS POINTERS TO FUNCTION FOR EFFICIENCY 
    func_decode decodeFunctions[] = {decodeDPImmediate, decodeDPRegister, decodeLoadStore, decodeBranch};
    decodeFunctions[type](&decoded, instruction);
    state->instruction_type = type;
  }
}