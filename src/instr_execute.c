#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"
#include "instr_decode.h"
#include "instr_execute.h"

bool get_msb(uint64_t value, bool is_64bit) {
    if (is_64bit) {
        return (value >> 63) & 1;
    } else {
        return ((uint32_t)value >> 31) & 1;
    }
}

void update_pstate_arith( ARM_STATE *state, uint64_t operand1, uint64_t operand2, uint64_t result, bool is_sub, bool is_64_bit ) {
    if (is_64_bit) {
        state->pstate.Z = (result == 0);
        state->pstate.N = get_msb(result, true);
    } else {
        uint32_t result32bit = (uint32_t)result;
        state->pstate.Z = (result32bit == 0);
        state->pstate.N = get_msb(result32bit, false);
    }

    if (is_sub) { // Carry for subtraction
        if (is_64_bit) {
            state->pstate.C = (operand1 >= operand2);
        } else {
            state->pstate.C = ((uint32_t)operand1 >= (uint32_t)operand2);
        }
    } else {//Carry for addition
        if (is_64_bit) {
            state->pstate.C = (result < operand1);
        } else {
            state->pstate.C = ((uint32_t)result < (uint32_t)operand1);
        }
    }

    bool op1_sign = get_msb(operand1, is_64_bit);
    bool op2_sign = get_msb(operand2, is_64_bit);
    bool result_sign = state->pstate.N; // N flag already reflects the result's sign

    if (is_sub) { // Overflow for subtraction
        if (op1_sign != op2_sign && result_sign == op2_sign) {
            state->pstate.V = true;
        } else {
            state->pstate.V = false;
        }
    } else { // Overflow for additions
        if (op1_sign == op2_sign && result_sign != op1_sign) {
            state->pstate.V = true;
        } else {
            state->pstate.V = false;
        }
    }
}

//EXECUTE FUNCTIONS
void execute(ARM_STATE *state) {
    func_execute executeFunctions[] = {executeDPImmediate, executeDPRegister, executeLoadStore, executeBranch};
    executeFunctions[state->instruction_type](state);
}

void executeDPImmediate( ARM_STATE *state) {
  uint32_t instr = state->instruction;
  DECODED_INSTR dec_instr = state->decoded;
  decodeDPImmediate(&dec_instr, instr);
  // Check if it is arithmetic or wide move
  if (dec_instr.opi == 0x2) {
    dec_instr.sh = get_bits(instr, 22, 22);
    dec_instr.imm12 = get_bits(instr, 21, 10);
    dec_instr.rn = get_bits(instr, 9, 5);
    if (dec_instr.sh == 1) {
      dec_instr.imm12 <<= 12;
    }

    uint64_t result = 0;
    bool update_flags = false;
    bool is_sub = false;
    uint64_t rn = dec_instr.rn;
    uint64_t imm12 = dec_instr.imm12;

    switch (dec_instr.opc) {
    case 0x0:
      result = rn + imm12;
      break;
    case 0x1:
      result = rn + imm12;
      update_flags = true;
      break;
    case 0x2:
      result = rn - imm12;
      is_sub = true;
      break;
    case 0x3:
      result = rn - imm12;
      is_sub = true;
      update_flags = true;
      break;
    default:
      break;
    }
    //IMPLEMENT SET REGISTER VALUE FUNCTION

    // Updating pstate if needed
    if (update_flags) {
        bool is_64_bit = dec_instr.sf;
        update_pstate_arith( &state,  rn, imm12, result, is_sub, is_64_bit );
    }
  } else if (dec_instr.opi == 0x5) {
    // Wide Move
    dec_instr.hw = get_bits(instr, 22, 21);
    dec_instr.imm16 = get_bits(instr, 20, 5);
    
  }
}



void executeDPRegister( ARM_STATE *state);
void executeLoadStore( ARM_STATE *state);
void executeBranch( ARM_STATE *state);