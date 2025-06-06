#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"
#include "instr_decode.h"
#include "instr_execute.h"

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
        break;
    }

}

perform_Shift( Shift_type_t shift_type, )

void executeDPRegister( ARM_STATE *state) {
    DECODED_INSTR dec_instr = state->decoded;
    bool is_64_bit = dec_instr.sf;


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

        uint64_t op2 = 


    }
}