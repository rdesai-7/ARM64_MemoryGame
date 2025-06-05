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
    DECODED_INSTR dec_instr = state->decoded;
    bool is_64_bit = dec_instr.sf;

    // Check if it is arithmetic or wide move
    if (dec_instr.opi == 0x2) {
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

        set_reg_val(&state, dec_instr.rd, result, is_64_bit);

        // Updating pstate if needed
        if (update_flags) {
            update_pstate_arith( &state,  rn, imm12, result, is_sub, is_64_bit );
        }

    } else if (dec_instr.opi == 0x5) {
        // Wide Move

        if (is_64_bit == 0) {
            if (!(dec_instr.hw == 0x0 || dec_instr.hw == 0x1)) {
                fprintf(stderr, "Invalid opcode for 32 bit register");
                return;
            }
        }
        
        int sh_amt = dec_instr.hw * SHIFT_CONSTANT;
        uint64_t op = (uint64_t)dec_instr.imm16 << sh_amt;
        uint64_t new_rd_val;

        switch (dec_instr.opc) {
        case 0x0:
            if (is_64_bit) {
                new_rd_val = ~op;
            } else {// for 32 bit
                new_rd_val = ~((uint32_t)op);
            }
            // SET REGISTER VALUE
            set_reg_val(&state, dec_instr.rd, new_rd_val, is_64_bit);
            break;
        case 0x2:
            // PUT OPERAND IN REGISTER
            set_reg_val(&state, dec_instr.rd, op, is_64_bit);
            break;
        case 0x3:
            //GET CURRENT VALUE OF RD REGISTER
            uint64_t og_rd_val = get_reg_val(&state, dec_instr.rd, is_64_bit);
            uint64_t mask = ~((uint64_t)0xFFFF << sh_amt); // Creates a mask of 0s to clear the required bits 
            og_rd_val &= mask;
            new_rd_val = og_rd_val | op;
            // SET REGISTER VALUE
            set_reg_val( &state, dec_instr.rd, new_rd_val, is_64_bit );
        default:
            break;
        }
    }
}

void executeDPRegister( ARM_STATE *state);
void executeLoadStore( ARM_STATE *state);
void executeBranch( ARM_STATE *state);