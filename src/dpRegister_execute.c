#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"
#include "instr_decode.h"
#include "instr_execute.h"
#include "bit_shift.h"

typedef enum {
    LSL,
    LSR,
    ASR,
    ROR,
} Shift_type_t;

Shift_type_t getShiftType( uint8_t shift ) {
    switch (shift) {
    case 0x0:
        return LSL;
        break;
    case 0x1:
        return LSR;
        break;
    case 0x2:
        return ASR;
        break;
    case 0x3:
        return ROR;
    default:
        return LSL;
        break;
    }
}

void update_pstate_logical( ARM_STATE *state, uint64_t result, bool is_64_bit ) {
    if (is_64_bit) {
        state->pstate.Z = (result == 0);
        state->pstate.N = get_msb(result, true);
    } else {
        uint32_t result32bit = (uint32_t)result;
        state->pstate.Z = (result32bit == 0);
        state->pstate.N = get_msb(result32bit, false);
    }
    
    state->pstate.C = 0;
    state->pstate.V = 0;
}

void executeDPRegister( ARM_STATE *state) {
    DECODED_INSTR dec_instr = state->decoded;
    bool is_64_bit = dec_instr.sf;
    uint64_t rn_val = get_reg_val(state, dec_instr.rn, is_64_bit);
    uint64_t rm_val = get_reg_val(state, dec_instr.rm, is_64_bit);

    // ARITHMETIC AND LOGICAL OPS
    if ( dec_instr.M == 0 ) {

        //Check if its 32 bit the operation is the correct size
        if (!is_64_bit && dec_instr.operand) {
            fprintf(stderr, "Shift Amount greater than 32 bits");
            state->halt_flag = true;
            return;
        }

        Shift_type_t shift_type = getShiftType(dec_instr.shift);

        bool is_arith = dec_instr.opr >> 3; // Get msb of opr

        if (is_arith) {
            // Check rotate instruction isnt used
            if (shift_type == 3) {
                fprintf(stderr, "Rotate instruction invalid for arithmetic");
                state->halt_flag = true;
                return;
            }
        }

        shift_func_t shift_func = NULL;
        uint64_t op2 = 0;

        switch (shift_type) {
        case LSL:
            shift_func = &lsl;
            break;
        case LSR:
            shift_func = &lsr;
            break;
        case ASR:
            shift_func = &asr;
            break;
        case ROR:
            shift_func = &ror;
            break;
        default:
            break;
        }

        op2 = (*shift_func) (rm_val, dec_instr.operand, is_64_bit);

        //Check if it needs to be negated
        if (!is_arith && dec_instr.N == 1) {
            op2 = ~op2;
            if (!is_64_bit) {
                op2 = (uint32_t)op2;
            }
        }

        //Do arithmetic or logical operation
        uint64_t result = 0;
        bool update_flags = false;

        if (is_arith) { //Arithmetic instructions
            bool is_sub = false;
            switch (dec_instr.opc) {
                case 0x0:
                    result = rn_val + op2;
                    break;
                case 0x1:
                    result = rn_val + op2;
                    update_flags = true;
                    break;
                case 0x2:
                    result = rn_val - op2;
                    is_sub = true;
                    break;
                case 0x3:
                    result = rn_val - op2;
                    is_sub = true;
                    update_flags = true;
                    break;
                default:
                    break;
            }

            set_reg_val(state, dec_instr.rd, result, is_64_bit);

            // Updating pstate if needed
            if (update_flags) {
                update_pstate_arith( state,  rn_val, op2, result, is_sub, is_64_bit );
            }
        } else {//Bit logic
            switch (dec_instr.opc) {
            case 0x0:
                result = rn_val & op2;
                break;
            case 0x1:
                result = rn_val | op2;
                break;
            case 0x2:
                result = rn_val ^ op2;
                break;
            case 0x3:
                result = rn_val & op2;
                update_flags = true;
            default:
                break;
            }

            set_reg_val(state, dec_instr.rd, result, is_64_bit);

            // Updating pstate if needed
            if (update_flags) {
                update_pstate_logical( state, result, is_64_bit );
            }
        }


    } else if ( dec_instr.M == 1 && dec_instr.opc == 0x0 && dec_instr.opr == 0x8) {
        uint64_t ra_val = get_reg_val(state, dec_instr.ra, is_64_bit);
        uint64_t product = rn_val * rm_val;
        uint64_t result = 0;

        if (dec_instr.x) {
            result = ra_val - product;
        } else {
            result = ra_val + product;
        }
        set_reg_val(state, dec_instr.rd, result, is_64_bit);
    } else {
        fprintf(stderr, "Unknown DP-Register instruction \n");
        state->halt_flag = true;
    }
    return;
}