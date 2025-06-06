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

    
    if (dec_instr.opi == 0x2) {
        if (dec_instr.sh == 1) {
        dec_instr.imm12 <<= 12;
        }

        uint64_t result = 0;
        bool update_flags = false;
        bool is_sub = false;
        uint64_t rn_val = get_reg_val(state, dec_instr.rn, is_64_bit);
        uint64_t imm12 = dec_instr.imm12;

        switch (dec_instr.opc) {
        case 0x0:
        result = rn_val + imm12;
        break;
        case 0x1:
        result = rn_val + imm12;
        update_flags = true;
        break;
        case 0x2:
        result = rn_val - imm12;
        is_sub = true;
        break;
        case 0x3:
        result = rn_val - imm12;
        is_sub = true;
        update_flags = true;
        break;
        default:
        break;
        }
        //IMPLEMENT SET REGISTER VALUE FUNCTION

        set_reg_val(state, dec_instr.rd, result, is_64_bit);

        // Updating pstate if needed
        if (update_flags) {
            update_pstate_arith( state,  rn_val, imm12, result, is_sub, is_64_bit );
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
        uint64_t new_rd_val = 0;
        uint64_t og_rd_val = 0;

        switch (dec_instr.opc) {
        case 0x0:
            if (is_64_bit) {
                new_rd_val = ~op;
            } else {// for 32 bit
                new_rd_val = ~((uint32_t)op);
            }
            // SET REGISTER VALUE
            set_reg_val(state, dec_instr.rd, new_rd_val, is_64_bit);
            break;
        case 0x2:
            // PUT OPERAND IN REGISTER
            set_reg_val(state, dec_instr.rd, op, is_64_bit);
            break;
        case 0x3:
            //GET CURRENT VALUE OF RD REGISTER
            og_rd_val = get_reg_val(state, dec_instr.rd, is_64_bit);
            uint64_t mask = ~((uint64_t)0xFFFF << sh_amt); // Creates a mask of 0s to clear the required bits 
            og_rd_val &= mask;
            new_rd_val = og_rd_val | op;
            // SET REGISTER VALUE
            set_reg_val( state, dec_instr.rd, new_rd_val, is_64_bit );
        default:
            break;
        }
    }
}

void executeDPRegister( ARM_STATE *state) {}

void executeLoadStore( ARM_STATE *state) {
    DECODED_INSTR dec_instr = state->decoded;
    bool sf = dec_instr.sf;
    uint64_t addr;
    int64_t simm9;
    switch (dec_instr.loadstore_type) {
        case SDT:
            switch (dec_instr.addr_mode) {
                case U_OFFSET: {
                    uint64_t uoffset;
                    if (sf) {
                        uoffset = dec_instr.offset << 3;
                    } else {
                        uoffset = dec_instr.offset << 2;
                    }
                    addr = get_reg_val(state,dec_instr.xn,true) + uoffset;
                    break;
                }
                case PRE_INDEXED:
                    // sign-extend simm9 to 64bit
                    simm9 = (int64_t)((dec_instr.simm9 << 7) >> 7); 
                    addr = get_reg_val(state,dec_instr.xn,true)  + simm9;
                    set_reg_val(state,dec_instr.xn,addr,true);
                    break;
                case POST_INDEXED:
                    addr = get_reg_val(state,dec_instr.xn,true);
                    simm9 = (int64_t)((dec_instr.simm9 << 7) >> 7); 
                    set_reg_val(state,dec_instr.xn,addr+simm9,true);
                    break;
                case REG_OFFSET:
                    addr = get_reg_val(state,dec_instr.xn,true) + get_reg_val(state,dec_instr.xm,true);
                    break;
                default:
                    break;
            }

            if (dec_instr.L == 1) {
                // load into rt
                uint64_t val = loadMemory(state, addr, sf);
                set_reg_val(state,dec_instr.rt, val, sf);
            } else {
                // store
                uint64_t val = get_reg_val(state,dec_instr.rt,true);
                storeMemory(state, addr, sf, val);
            }
            break;
        case LOADLITERAL: {
            // sign extend simm19 
            int32_t simm19_32 = (int32_t)(dec_instr.simm19 << 13) >> 13;
            int64_t offset = (int64_t)simm19_32 << 2;
            uint64_t start_address = state->pc + offset;

            // load the value into rt
            uint64_t val = loadMemory(state,start_address,sf);
            set_reg_val(state, dec_instr.rt, val, sf);

            // if (sf) {
            //     // target register is 64bit
            //     uint64_t val = (state->memory[start_address + 7] << 64) |
            //         (state->memory[start_address + 6]) << 56|
            //         (state->memory[start_address + 5] << 48) |
            //         (state->memory[start_address + 4] << 40) |
            //         (state->memory[start_address + 3]) << 32 | 
            //         (state->memory[start_address + 2] << 16) |
            //         (state->memory[start_address + 1] << 8) |
            //         (state->memory[start_address]);
            // } else {
            //     // target register is 32bit
            //     uint32_t val = (state->memory[start_address + 3] << 24) |
            //         (state->memory[start_address + 2] << 16) |
            //         (state->memory[start_address + 1] << 8) |
            //         (state->memory[start_address]);
            // }
            //set_reg_val(state, dec_instr.rt, val, sf)
            break;
        }
        default:
            break;
    }

}



void executeBranch( ARM_STATE *state) {
    DECODED_INSTR dec_instr = state->decoded;
    int64_t offset;
    switch (dec_instr.branch_type) {
        case UNCOND:;
            // sign extend simm26 
            int32_t simm26_32 = (int32_t)(dec_instr.simm26 << 6) >> 6;
            offset = (int64_t)simm26_32 << 2;
            // add offset to PC
            state->pc += offset;
            break;
        case REG:
            // set PC to 64-bit value from specified register
            state->pc = get_reg_val(state,dec_instr.xn,true);
            break;
        case COND:;
            // sign extend simm19 
            int32_t simm19_32 = (int32_t)(dec_instr.simm19 << 13) >> 13;
            offset = (int64_t)simm19_32 << 2;

            // add offset if cond is met
            if (check_branch_cond(dec_instr.cond,state->pstate)){
                state->pc += offset;
            }
            break;
        default:
            break;
    }
}

bool check_branch_cond(uint8_t cond, PSTATE_Flags pstate){
    switch (cond){
        case 0x0:
            // check EQ
            return (pstate.Z == 1);
        case 0x1:
            // check NE
            return (pstate.Z == 0);
        case 0xA:
            // check GE
            return (pstate.N == pstate.V);
        case 0xB:
            // check LT
            return (pstate.N != pstate.V);
        case 0xC:
            // check GT
            return (pstate.Z == 0 && pstate.N == pstate.V);
        case 0xD:
            // check LE
            return !(pstate.Z == 0 && pstate.N == pstate.V);
        case 0xE:
            // AL: always true
            return true;
        default:
            fprintf(stderr, "Unknown condition type");
            return false;
    }
}

void execute(ARM_STATE *state) {
    if (state->halt_flag) {
        return;
    }
    func_execute executeFunctions[] = {executeDPImmediate, executeDPRegister, executeLoadStore, executeBranch};
    executeFunctions[state->instruction_type](state);
}