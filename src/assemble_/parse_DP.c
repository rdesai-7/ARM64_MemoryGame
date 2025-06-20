#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "parse_DP.h"
#include "assembler_state.h"
#include "parse_loadstore.h"

//ADD TOKEN COUNT CHECKS (using assert)

// lookup tables for mnemonics
const InstructionOpcodeData opcode_lookup_table[] = {
    {"and",  0x0, 0},
    {"bic",  0x0, 1},
    {"orr",  0x1, 0},
    {"orn",  0x1, 1},
    {"eor",  0x2, 0},
    {"eon",  0x2, 1},
    {"ands", 0x3, 0},
    {"bics", 0x3, 1},
};
const size_t NUM_OPCODE_ENTRIES = sizeof(opcode_lookup_table) / sizeof(InstructionOpcodeData);

const AddSubOpcodeData add_sub_opcode_table[] = {
    {"add",  0x0},
    {"adds", 0x1},
    {"sub",  0x2},
    {"subs", 0x3},
};
const size_t NUM_ADD_SUB_ENTRIES = sizeof(add_sub_opcode_table) / sizeof(AddSubOpcodeData);

const MovWOpcData mov_w_opcode_table[] = {
    {"movn", 0x0},
    {"movz", 0x2},
    {"movk", 0x3},
};
const size_t NUM_MOV_W_ENTRIES = sizeof(mov_w_opcode_table) / sizeof(MovWOpcData);


//CHANGE THIS TO SAME LOGIC AS OTTHER FUNCTIONS
static int get_logic_opcode(const char* mnemonic, uint32_t *N, uint32_t *opc_out) {
    for (size_t i = 0; i < NUM_OPCODE_ENTRIES; ++i) {
        if (strcmp(mnemonic, opcode_lookup_table[i].mnemonic) == 0) {
            *N = opcode_lookup_table[i].N;
            *opc_out = opcode_lookup_table[i].opc;
            return 1;
        }
    }
    return 0;
}

static int get_add_sub_opcode(const char* mnemonic, uint32_t* opc_out) {
    for (size_t i = 0; i < NUM_ADD_SUB_ENTRIES; ++i) {
        if (strcmp(mnemonic, add_sub_opcode_table[i].mnemonic) == 0) {
            *opc_out = add_sub_opcode_table[i].opc;
            return 1;
        }
    }
    return 0;
}

static int get_mov_opcode(const char* mnemonic, uint32_t* opc_out) {
    for (size_t i = 0; i < NUM_MOV_W_ENTRIES; ++i) {
        if (strcmp(mnemonic, mov_w_opcode_table[i].mnemonic) == 0) {
            *opc_out = mov_w_opcode_table[i].opc;
            return 1;
        }
    }
    return 0;
}

uint32_t multiply_assembly(char** tokens, int token_count, ARM_STATE *state) {
    uint32_t rd_reg_num, rn_reg_num, rm_reg_num, ra_reg_num;
    uint32_t sf_bit;

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    rm_reg_num = parse_register_token(tokens[3], &sf_bit);
    ra_reg_num = parse_register_token(tokens[4], &sf_bit);

    uint32_t instruction = MUL_INITIAL_STATE | (sf_bit & 0x1) << 31 | (rm_reg_num & 0x1F) << 16 
     | (ra_reg_num & 0x1F) << 10 | (rn_reg_num & 0x1F) << 5| (rd_reg_num & 0x1F);

    if (strcmp(tokens[0], "msub") == 0) {
        instruction |= (1 << 15);
    }

    return instruction;
}

uint32_t bit_logic_assembly(char** tokens, int token_count, ARM_STATE *state) {
    uint32_t rd_reg_num, rn_reg_num, rm_reg_num;
    uint32_t opcode;
    uint32_t N_bit;
    uint32_t shift_type_bits = 0x0;
    uint32_t imm6_val = 0;
    uint32_t sf_bit;

    assert(get_logic_opcode(tokens[0], &N_bit, &opcode) == 1);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    rm_reg_num = parse_register_token(tokens[3], &sf_bit);

    if (token_count == 6) {
        char* shift_type_str = tokens[4];
        imm6_val = parse_imm(tokens[5]);

        shift_type_bits = parse_shift_type(shift_type_str);
    }
    
    uint32_t instruction = BIT_LOGIC_INITIAL_STATE
                     | ((sf_bit & 0x1) << 31)
                     | ((opcode & 0x3) << 29)
                     | ((shift_type_bits & 0x3) << 22)
                     | ((N_bit & 0x1) << 21)
                     | ((rm_reg_num & 0x1F) << 16)
                     | ((imm6_val & 0x3F) << 10)
                     | ((rn_reg_num & 0x1F) << 5)
                     | (rd_reg_num & 0x1F);

    return instruction;
}

uint32_t add_sub_assembly(char** tokens, int token_count, ARM_STATE *state) {
    uint32_t rd_reg_num, rn_reg_num, rm_reg_num;
    uint32_t opcode;
    uint32_t imm6_val = 0;
    uint32_t shift_type_bits = 0x0;
    uint32_t sf_bit;

    assert(get_add_sub_opcode(tokens[0], &opcode) == 1);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    rm_reg_num = parse_register_token(tokens[3], &sf_bit);

    if (token_count == 6) {
        char* shift_type_str = tokens[4];
        imm6_val = parse_imm(tokens[5]);

        shift_type_bits = parse_shift_type(shift_type_str);
    }

    uint32_t instruction = ADD_SUB_REGISTER_INITIAL_STATE
                     | ((sf_bit & 0x1) << 31)
                     | ((opcode & 0x3) << 29)
                     | ((shift_type_bits & 0x3) << 22)
                     | ((rm_reg_num & 0x1F) << 16)
                     | ((imm6_val & 0x3F) << 10)
                     | ((rn_reg_num & 0x1F) << 5)
                     | (rd_reg_num & 0x1F);

    return instruction;
}

uint32_t add_sub_immediate_assembly(char** tokens, int token_count, ARM_STATE *state) {
    uint32_t rd_reg_num, rn_reg_num;
    uint32_t opcode;
    uint32_t imm12_val;
    uint32_t sh_bit = 0;
    uint32_t sf_bit;

    assert(get_add_sub_opcode(tokens[0], &opcode) == 1);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    imm12_val = parse_imm(tokens[3]);

    if (token_count == 6) {
        if (parse_imm(tokens[5]) == 12) {
            sh_bit = 1;
        } 
    }

    uint32_t instruction = ADD_SUB_IMMEDIATE_INITIAL_STATE
                     | ((sf_bit & 0x1) << 31)
                     | ((opcode & 0x3) << 29)
                     | ((sh_bit & 0x1) << 22)
                     | ((imm12_val & 0xFFF) << 10)
                     | ((rn_reg_num & 0x1F) << 5)
                     | ((rd_reg_num & 0x1F) << 0);

    return instruction;
}

uint32_t assemble_add_sub_instruction(char** tokens, int token_count, ARM_STATE *state) {
    assert(token_count >= 4);

    if (tokens[3][0] == '#') {
        return add_sub_immediate_assembly(tokens, token_count, state);
    } else {
        return add_sub_assembly(tokens, token_count, state);
    }
}

uint32_t mov_wide_assembly(char** tokens, int token_count, ARM_STATE *state) {
    uint32_t rd_reg_num;
    uint32_t opc_val;
    uint32_t imm16_val;
    uint32_t sf_bit;

    assert(get_mov_opcode(tokens[0], &opc_val) == 1);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    imm16_val = parse_imm(tokens[2]);

    uint32_t shift_amount = 0;
    if (token_count == 5) {
        shift_amount = parse_imm(tokens[4]) / 16;
    }

    uint32_t instruction = MOV_INITIAL_STATE
                     | ((sf_bit & 0x1) << 31)
                     | ((opc_val & 0x3) << 29)
                     | ((shift_amount & 0x3) << 21)
                     | ((imm16_val & 0xFFFF) << 5)
                     | ((rd_reg_num & 0x1F) << 0);

    return instruction;
}

uint32_t parse_compare(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[token_count + 1];
    uint32_t sf_bit;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = (strcmp(tokens[0], "cmp") == 0) ? "subs" : "adds";
    new_tokens[1] = "ZR";
    for (int i = 2; i < token_count + 1; i++) {
        new_tokens[i] = tokens[i - 1];
    }

    return assemble_add_sub_instruction(new_tokens, token_count + 1, state);
}

uint32_t parse_add_sub_alias(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[token_count + 1];
    uint32_t sf_bit;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = (strcmp(tokens[0], "neg") == 0)  ? "sub" :
                    (strcmp(tokens[0], "negs") == 0) ? "subs" :
                                                       "ands" ;
    new_tokens[1] = "ZR";
    for (int i = 2; i < token_count + 1; i++) {
        new_tokens[i] = tokens[i - 1];
    }

    return bit_logic_assembly(new_tokens, token_count + 1, state);
}

uint32_t parse_mov_alias(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[4];
    uint32_t sf_bit;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = (strcmp(tokens[0], "mov") == 0) ? "orr" : "orn";
    new_tokens[1] = tokens[1];
    new_tokens[2] = "ZR";
    new_tokens[3] = tokens[2];
    
    return bit_logic_assembly(new_tokens, 4, state);
}

uint32_t parse_mul_alias(char** tokens, int token_count, ARM_STATE *state) {char* new_tokens[5];
    uint32_t sf_bit;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = (strcmp(tokens[0], "mul") == 0) ? "madd" : "msub";
    new_tokens[1] = tokens[1];
    new_tokens[2] = tokens[2];
    new_tokens[3] = tokens[3];
    new_tokens[4] = "ZR";

    return multiply_assembly(new_tokens, 5, state);
}
