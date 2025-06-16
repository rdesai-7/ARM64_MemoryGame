#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "parse_DP.h"
#include "assembler_state.h"
#include "parse_loadstore.h"

//ADD TOKEN COUNT CHECKS

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

extern uint32_t parse_shift_type(const char* shift_type_str) {
    if (strcmp(shift_type_str, "lsl") == 0) {
            return 0x0;
        } else if (strcmp(shift_type_str, "lsr") == 0) {
            return 0x1;
        } else if (strcmp(shift_type_str, "asr") == 0) {
            return 0x2;
        } else if (strcmp(shift_type_str, "ror") == 0) {
            return 0x3;
        } else {
            fprintf(stderr, "Error: Invalid shift type '%s'\n", shift_type_str);
            return 0;
        }

}

uint32_t parse_register_token(char* reg_token, uint32_t* sf_bit) {
    
    if (strcmp(reg_token, "sp") == 0 || strcmp(reg_token, "xzr") == 0) {
        if (sf_bit != NULL) {
            *sf_bit = 1; 
        }
        return ZERO_REGISTER; 
    }

    if (strcmp(reg_token, "wsp") == 0 || strcmp(reg_token, "wzr") == 0) {
        if (sf_bit != NULL) {
            *sf_bit = 0; 
        }
        return ZERO_REGISTER; 
    }

    if (strcmp(reg_token, "ZR") == 0) {
        return ZERO_REGISTER; 
    }

    char reg_type = reg_token[0];
    uint32_t reg_num = strtoul(reg_token + 1, NULL, 10); 

    if (sf_bit != NULL) {
        if (reg_type == 'W' || reg_type == 'w') {
            *sf_bit = 0; // 32-bit register
        } else {
            *sf_bit = 1; // 64-bit register
        }
    }
    return reg_num;
}


//CHANGE THIS TO SAME LOGIC AS OTTHER FUNCTIONS
uint32_t get_logic_opcode(const char* mnemonic, uint32_t *N) {
    for (size_t i = 0; i < NUM_OPCODE_ENTRIES; ++i) {
        if (strcmp(mnemonic, opcode_lookup_table[i].mnemonic) == 0) {
            *N = opcode_lookup_table[i].N;
            return opcode_lookup_table[i].opc;
        }
    }
    return 0;
}

int get_add_sub_opcode(const char* mnemonic, uint32_t* opc_out) {
    for (size_t i = 0; i < NUM_ADD_SUB_ENTRIES; ++i) {
        if (strcmp(mnemonic, add_sub_opcode_table[i].mnemonic) == 0) {
            *opc_out = add_sub_opcode_table[i].opc;
            return 1;
        }
    }
    return 0;
}

int get_mov_opcode(const char* mnemonic, uint32_t* opc_out) {
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
    uint32_t instruction = MUL_INITIAL_STATE;
    uint32_t sf_bit = 0;

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    rm_reg_num = parse_register_token(tokens[3], &sf_bit);
    ra_reg_num = parse_register_token(tokens[4], &sf_bit);

    //ADD ALL IN ONE LINE

    instruction |= (sf_bit & 0x1) << 31;

    instruction |= (rm_reg_num & 0x1F) << 16;

    if (strcmp(tokens[0], "msub") == 0) {
        instruction |= (1 << 15);
    }

    instruction |= (ra_reg_num & 0x1F) << 10;

    instruction |= (rn_reg_num & 0x1F) << 5;

    instruction |= (rd_reg_num & 0x1F);

    return instruction;
}

uint32_t bit_logic_assembly(char** tokens, int token_count, ARM_STATE *state) {
    uint32_t rd_reg_num, rn_reg_num, rm_reg_num;
    uint32_t opcode;
    uint32_t N_bit;
    uint32_t shift_type_bits = 0x0;
    uint32_t imm6_val = 0;
    uint32_t instruction = BIT_LOGIC_INITIAL_STATE;
    uint32_t sf_bit = 0;

    opcode = get_logic_opcode(tokens[0], &N_bit);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    rm_reg_num = parse_register_token(tokens[3], &sf_bit);

    if (token_count == 6) {
        char* shift_type_str = tokens[4];
        imm6_val = parse_imm(tokens[5]);

        shift_type_bits = parse_shift_type(shift_type_str);
    }

    //PUT IT ALL IN ONE LINE

    instruction |= (sf_bit & 0x1) << 31;

    instruction |= (opcode & 0x3) << 29;

    instruction |= (shift_type_bits & 0x3) << 22;

    instruction |= (N_bit & 0x1) << 21;

    instruction |= (rm_reg_num & 0x1F) << 16;

    instruction |= (imm6_val & 0x3F) << 10;

    instruction |= (rn_reg_num & 0x1F) << 5;

    instruction |= (rd_reg_num & 0x1F);

    return instruction;
}

uint32_t add_sub_assembly(char** tokens, int token_count, ARM_STATE *state) {
    uint32_t rd_reg_num, rn_reg_num, rm_reg_num;
    uint32_t opcode;
    uint32_t imm6_val = 0;
    uint32_t shift_type_bits = 0x0;
    uint32_t instruction = ADD_SUB_REGISTER_INITIAL_STATE;
    uint32_t sf_bit = 0;

    get_add_sub_opcode(tokens[0], &opcode);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    rm_reg_num = parse_register_token(tokens[3], &sf_bit);

    if (token_count == 6) {
        char* shift_type_str = tokens[4];
        imm6_val = parse_imm(tokens[5]);

        shift_type_bits = parse_shift_type(shift_type_str);
    }

    // ADD ALL INTO ONE LINE 

    instruction |= (sf_bit & 0x1) << 31;

    instruction |= (opcode & 0x3) << 29;

    instruction |= (shift_type_bits & 0x3) << 22;

    instruction |= (rm_reg_num & 0x1F) << 16;

    instruction |= (imm6_val & 0x3F) << 10;

    instruction |= (rn_reg_num & 0x1F) << 5;

    instruction |= (rd_reg_num & 0x1F);

    return instruction;
}

uint32_t add_sub_immediate_assembly(char** tokens, int token_count, ARM_STATE *state) {
    uint32_t rd_reg_num, rn_reg_num;
    uint32_t opcode;
    uint32_t imm12_val;
    uint32_t sh_bit = 0;
    uint32_t instruction = ADD_SUB_IMMEDIATE_INITIAL_STATE;
    uint32_t sf_bit = 0;

    get_add_sub_opcode(tokens[0], &opcode);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    imm12_val = parse_imm(tokens[3]);

    if (token_count == 6) {
        if (parse_imm(tokens[5]) == 12) {
            sh_bit = 1;
        } 
    }

    //ALL INTO ONE LINE

    instruction |= (sf_bit & 0x1) << 31;

    instruction |= (opcode & 0x3) << 29;

    instruction |= (sh_bit & 0x1) << 22;

    instruction |= (imm12_val & 0xFFF) << 10;

    instruction |= (rn_reg_num & 0x1F) << 5;

    instruction |= (rd_reg_num & 0x1F) << 0;

    return instruction;
}

uint32_t assemble_add_sub_instruction(char** tokens, int token_count, ARM_STATE *state) {
    assert(token_count < 4);

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
    uint32_t instruction = MOV_INITIAL_STATE;
    uint32_t sf_bit = 0;

    get_mov_opcode(tokens[0], &opc_val);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    imm16_val = parse_imm(tokens[2]);

    uint32_t shift_amount = 0;
    if (token_count == 5) {
        shift_amount = parse_imm(tokens[4]) / 16;
    }

    // MOVE ALL INTO ONE LINE

    instruction |= (sf_bit & 0x1) << 31;

    instruction |= (opc_val & 0x3) << 29;

    instruction |= (shift_amount & 0x3) << 21;

    instruction |= (imm16_val & 0xFFFF) << 5;

    instruction |= (rd_reg_num & 0x1F) << 0;

    return instruction;
}

uint32_t cmp_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[4];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "subs";
    new_tokens[1] = "ZR";
    new_tokens[2] = tokens[1];
    new_tokens[3] = tokens[2];

    return assemble_add_sub_instruction(new_tokens, 4, state);
}

uint32_t cmn_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[4];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "adds";
    new_tokens[1] = "ZR";
    new_tokens[2] = tokens[1];
    new_tokens[3] = tokens[2];

    return assemble_add_sub_instruction(new_tokens, 4, state);
}

uint32_t neg_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[4];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "sub";
    new_tokens[1] = tokens[1];
    new_tokens[2] = "ZR";
    new_tokens[3] = tokens[2];

    return assemble_add_sub_instruction(new_tokens, 4, state);
}

uint32_t negs_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[4];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "subs";
    new_tokens[1] = tokens[1];
    new_tokens[2] = "ZR";
    new_tokens[3] = tokens[2];

    return assemble_add_sub_instruction(new_tokens, 4, state);
}

uint32_t tst_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[4];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "ands";
    new_tokens[1] = "ZR";
    new_tokens[2] = tokens[1];
    new_tokens[3] = tokens[2];

    return bit_logic_assembly(new_tokens, 4, state);
}

uint32_t mvn_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[4];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "orn";
    new_tokens[1] = tokens[1];
    new_tokens[2] = "ZR";
    new_tokens[3] = tokens[2];

    return bit_logic_assembly(new_tokens, 4, state);
}

uint32_t mov_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[4];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "orr";
    new_tokens[1] = tokens[1];
    new_tokens[2] = "ZR";
    new_tokens[3] = tokens[2];
    
    return bit_logic_assembly(new_tokens, 4, state);
}

uint32_t mul_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[5];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "madd";
    new_tokens[1] = tokens[1];
    new_tokens[2] = tokens[2];
    new_tokens[3] = tokens[3];
    new_tokens[4] = "ZR";

    return multiply_assembly(new_tokens, 5, state);
}

uint32_t mneg_assembly(char** tokens, int token_count, ARM_STATE *state) {
    char* new_tokens[5];
    uint32_t sf_bit = 0;
    parse_register_token(tokens[1], &sf_bit);
    new_tokens[0] = "msub";
    new_tokens[1] = tokens[1];
    new_tokens[2] = tokens[2];
    new_tokens[3] = tokens[3];
    new_tokens[4] = "ZR";

    return multiply_assembly(new_tokens, 5, state);
}