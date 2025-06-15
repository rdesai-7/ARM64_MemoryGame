#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "parser.h"

// CHECK IF REGISTER IS W OR X WHEN CALLING 0 REGISTER FOR ALIASES
// STACK POINTER LOGIC

const InstructionOpcodeData opcode_lookup_table[] = {
    {"and",  0b00, 0},
    {"bic",  0b00, 1},
    {"orr",  0b01, 0},
    {"orn",  0b01, 1},
    {"eor",  0b10, 0},
    {"eon",  0b10, 1},
    {"ands", 0b11, 0},
    {"bics", 0b11, 1},
};
const size_t NUM_OPCODE_ENTRIES = sizeof(opcode_lookup_table) / sizeof(InstructionOpcodeData);

const AddSubOpcodeData add_sub_opcode_table[] = {
    {"add",  0b0, 0b0},
    {"adds", 0b0, 0b1},
    {"sub",  0b1, 0b0},
    {"subs", 0b1, 0b1},
};
const size_t NUM_ADD_SUB_ENTRIES = sizeof(add_sub_opcode_table) / sizeof(AddSubOpcodeData);

const MovWOpcData mov_w_opcode_table[] = {
    {"movn", 0b00},
    {"movz", 0b10},
    {"movk", 0b11},
};
const size_t NUM_MOV_W_ENTRIES = sizeof(mov_w_opcode_table) / sizeof(MovWOpcData);

static uint32_t parse_mov_w_shift_pos(uint32_t shift_amount) {
    switch (shift_amount) {
        case 0:  return 0b00;
        case 16: return 0b01;
        case 32: return 0b10;
        case 48: return 0b11;
        default: return 0;
    }
}

static uint32_t parse_register_token(char* reg_token, uint32_t* sf_bit) {

    
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

    if (strcmp(reg_token, "SP") == 0 || strcmp(reg_token, "ZR") == 0) {
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

uint32_t get_logic_opcode(const char* mnemonic, uint32_t *N) {
    for (size_t i = 0; i < NUM_OPCODE_ENTRIES; ++i) {
        if (strcmp(mnemonic, opcode_lookup_table[i].mnemonic) == 0) {
            *N = opcode_lookup_table[i].N;
            return opcode_lookup_table[i].opc;
        }
    }
    return 0;
}

int get_add_sub_opcode(const char* mnemonic, uint8_t* op_out, uint8_t* S_out) {
    for (size_t i = 0; i < NUM_ADD_SUB_ENTRIES; ++i) {
        if (strcmp(mnemonic, add_sub_opcode_table[i].mnemonic) == 0) {
            *op_out = add_sub_opcode_table[i].op_bit;
            *S_out = add_sub_opcode_table[i].S_bit;
            return 1; // Indicate success
        }
    }
    return 0; // Not found
}

int get_mov_opcode(const char* mnemonic, uint8_t* opc_out) {
    for (size_t i = 0; i < NUM_MOV_W_ENTRIES; ++i) {
        if (strcmp(mnemonic, mov_w_opcode_table[i].mnemonic) == 0) {
            *opc_out = mov_w_opcode_table[i].opc;
            return 1;
        }
    }
    return 0;
}


uint32_t multiply_assembly(char** tokens, uint32_t token_count) {
    uint32_t rd_reg_num, rn_reg_num, rm_reg_num, ra_reg_num;
    uint32_t instruction = MUL_INITIAL_STATE; 
    uint32_t sf_bit;


    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], &sf_bit);
    rm_reg_num = parse_register_token(tokens[3], &sf_bit);
    ra_reg_num = parse_register_token(tokens[4], &sf_bit);

    // sf bit (bit 31)
    instruction |= (sf_bit & 0x1) << 31;

    // rm field (bits 20-16)
    instruction |= (rm_reg_num & 0x1F) << 16;

    // x bit (bit 15) - determines madd (0) or msub (1)
    if (strcmp(tokens[0], "msub") == 0) {
        instruction |= (1 << 15);
    }

    // ra field (bits 14-10)
    instruction |= (ra_reg_num & 0x1F) << 10;

    // rn field (bits 9-5)
    instruction |= (rn_reg_num & 0x1F) << 5;

    // rd field (bits 4-0)
    instruction |= (rd_reg_num & 0x1F) << 0;

    return instruction;
}

uint32_t bit_logic_assembly(char** tokens, uint32_t token_count) {
    uint32_t rd_reg_num, rn_reg_num, rm_reg_num;
    uint32_t opcode;
    uint32_t N_bit;
    uint32_t shift_type_bits = 0b00;
    uint32_t imm6_val = 0;
    uint32_t instruction = BIT_LOGIC_INITIAL_STATE;
    uint32_t sf_bit = 0;

    opcode = get_logic_opcode(tokens[0], &N_bit);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], NULL);
    rm_reg_num = parse_register_token(tokens[3], NULL);

    if (token_count == 6) {
        char* shift_type_str = tokens[4];
        imm6_val = parse_imm(tokens[5]);

        if (strcmp(shift_type_str, "LSL") == 0) {
            shift_type_bits = 0b00;
        } else if (strcmp(shift_type_str, "LSR") == 0) {
            shift_type_bits = 0b01;
        } else if (strcmp(shift_type_str, "ASR") == 0) {
            shift_type_bits = 0b10;
        } else if (strcmp(shift_type_str, "ROR") == 0) {
            shift_type_bits = 0b11;
        } else {
            fprintf(stderr, "Error: Invalid shift type '%s'\n", shift_type_str);
            return 0;
        }
    } else if (token_count == 4) {
    } else {
        fprintf(stderr, "Error: Invalid token count for logical instruction\n");
        return 0;
    }

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

uint32_t add_sub_assembly(char** tokens, uint32_t token_count) {
    uint32_t rd_reg_num, rn_reg_num, rm_reg_num;
    uint8_t op_bit, S_bit;
    uint32_t imm6_val = 0;
    uint32_t shift_type_bits = 0b00;
    uint32_t instruction = ADD_SUB_REGISTER_INITIAL_STATE;
    uint32_t sf_bit = 0;

    get_add_sub_opcode(tokens[0], &op_bit, &S_bit);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], NULL);
    rm_reg_num = parse_register_token(tokens[3], NULL);

    if (token_count == 6) {
        char* shift_type_str = tokens[4];
        imm6_val = parse_imm(tokens[5]);

        if (strcmp(shift_type_str, "LSL") == 0) {
            shift_type_bits = 0b00;
        } else if (strcmp(shift_type_str, "LSR") == 0) {
            shift_type_bits = 0b01;
        } else if (strcmp(shift_type_str, "ASR") == 0) {
            shift_type_bits = 0b10;
        } else if (strcmp(shift_type_str, "ROR") == 0) {
            shift_type_bits = 0b11;
        } else {
            fprintf(stderr, "Error: Invalid shift type '%s'\n", shift_type_str);
            return 0;
        }
    } else if (token_count == 4) {
    } else {
        fprintf(stderr, "Error: Invalid token count for add/sub register instruction\n");
        return 0;
    }

    instruction |= (sf_bit & 0x1) << 31;

    instruction |= (op_bit & 0x1) << 30;

    instruction |= (S_bit & 0x1) << 29;

    instruction |= (shift_type_bits & 0x3) << 22;

    instruction |= (rm_reg_num & 0x1F) << 16;

    instruction |= (imm6_val & 0x3F) << 10;

    instruction |= (rn_reg_num & 0x1F) << 5;

    instruction |= (rd_reg_num & 0x1F) << 0;

    return instruction;
}

uint32_t add_sub_immediate_assembly(char** tokens, uint32_t token_count) {
    uint32_t rd_reg_num, rn_reg_num;
    uint8_t op_bit, S_bit;
    uint32_t imm12_val;
    uint32_t sh_bit = 0;
    uint32_t instruction = ADD_SUB_IMMEDIATE_INITIAL_STATE;
    uint32_t sf_bit = 0;

    get_add_sub_opcode(tokens[0], &op_bit, &S_bit);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    rn_reg_num = parse_register_token(tokens[2], NULL);
    imm12_val = parse_imm(tokens[3]);

    if (token_count == 6) {
        if (strcmp(tokens[4], "LSL") == 0 && parse_imm(tokens[5]) == 12) {
            sh_bit = 1;
        } else {
            fprintf(stderr, "Error: Invalid shift for add/sub immediate. Only LSL #12 is allowed.\n");
            return 0;
        }
    } else if (token_count != 4) {
        fprintf(stderr, "Error: Invalid token count for add/sub immediate instruction\n");
        return 0;
    }

    instruction |= (sf_bit & 0x1) << 31;

    instruction |= (op_bit & 0x1) << 30;

    instruction |= (S_bit & 0x1) << 29;

    instruction |= (sh_bit & 0x1) << 22;

    instruction |= (imm12_val & 0xFFF) << 10;

    instruction |= (rn_reg_num & 0x1F) << 5;

    instruction |= (rd_reg_num & 0x1F) << 0;

    return instruction;
}

uint32_t assemble_add_sub_instruction(char** tokens, uint32_t token_count) {
    if (token_count < 4) {
        fprintf(stderr, "Error: Not enough operands for add/sub instruction.\n");
        return 0;
    }

    if (tokens[3][0] == '#') {
        return add_sub_immediate_assembly(tokens, token_count);
    } else {
        return add_sub_assembly(tokens, token_count);
    }
}

uint32_t mov_wide_assembly(char** tokens, uint32_t token_count) {
    uint32_t rd_reg_num;
    uint8_t opc_val;
    uint32_t imm16_val;
    uint32_t pos_val;  
    uint32_t instruction = MOV_INITIAL_STATE; 
    uint32_t sf_bit;

    get_mov_opcode(tokens[0], &opc_val);

    rd_reg_num = parse_register_token(tokens[1], &sf_bit);
    imm16_val = parse_imm(tokens[2]);

    uint32_t shift_amount = 0;
    if (token_count == 5) { 
        shift_amount = parse_imm(tokens[4]);
    }
    pos_val = parse_mov_w_shift_pos(shift_amount);

    instruction |= (sf_bit) << 31;

    instruction |= (opc_val) << 29;

    instruction |= (pos_val) << 21; 

    instruction |= (imm16_val) << 5; 

    instruction |= (rd_reg_num);

    return instruction;
}

uint32_t cmp_assembly(char** tokens, uint32_t token_count) {
    char* new_tokens[4];
    new_tokens[0] = "subs";
    new_tokens[1] = "ZR";
    new_tokens[2] = tokens[1];
    new_tokens[3] = tokens[2];

    return assemble_add_sub_instruction(new_tokens, 4);

}

uint32_t cmn_assembly(char** tokens, uint32_t token_count) {
    char* new_tokens[4];
    new_tokens[0] = "adds";
    new_tokens[1] = "ZR";
    new_tokens[2] = tokens[1];
    new_tokens[3] = tokens[2];

    return assemble_add_sub_instruction(new_tokens, 4);

}

// SHOULD COMBINE NEG AND NEGS


uint32_t neg_assembly(char** tokens, uint32_t token_count) {
    char* new_tokens[4];
    new_tokens[0] = "sub";
    new_tokens[1] = tokens[1];
    new_tokens[2] = "ZR";
    new_tokens[3] = tokens[2];

    return assemble_add_sub_instruction(new_tokens, 4);

}

uint32_t negs_assembly(char** tokens, uint32_t token_count) {
    char* new_tokens[4];
    new_tokens[0] = "subs";
    new_tokens[1] = tokens[1];
    new_tokens[2] = "ZR";
    new_tokens[3] = tokens[2];

    return assemble_add_sub_instruction(new_tokens, 4);

}

uint32_t tst_assembly(char** tokens, uint32_t token_count) {
    
    char* new_tokens[4];
    new_tokens[0] = "ands";
    new_tokens[1] = "ZR";
    new_tokens[2] = tokens[1];
    new_tokens[3] = tokens[2];

    return bit_logic_assembly(new_tokens, 4);
}

uint32_t mvn_assembly(char** tokens, uint32_t token_count) {
    char* new_tokens[4]; // ORN Rd, XZR, Rn will always be 4 tokens
    new_tokens[0] = "orn";
    new_tokens[1] = tokens[1]; // Rd (destination)
    new_tokens[2] = "ZR";     // Rn (first source is zero register)
    new_tokens[3] = tokens[2]; // Rm (second source is original Rn)

    return bitwise_logic_assembly(new_tokens, 4);
}


uint32_t mov_assembly(char** tokens, uint32_t token_count) {
    char* new_tokens[4]; // ORR Rd, XZR, Rn will always be 4 tokens
    new_tokens[0] = "orr";
    new_tokens[1] = tokens[1]; // Rd (destination)
    new_tokens[2] = "ZR";     // Rn (first source is zero register)
    new_tokens[3] = tokens[2]; // Rm (second source is original Rn)

    return bitwise_logic_assembly(new_tokens, 4);
}

uint32_t mul_assembly(char** tokens, uint32_t token_count) {
    char* new_tokens[5]; // ORN Rd, XZR, Rn will always be 4 tokens
    new_tokens[0] = "madd";
    new_tokens[1] = tokens[1]; // Rd (destination)
    new_tokens[2] = tokens[2];     // Rn (first source is zero register)
    new_tokens[3] = tokens[3]; // Rm (second source is original Rn)
    new_tokens[4] = "ZR";

    return multiply_assembly(new_tokens, 5);
}

uint32_t mneg_assembly(char** tokens, uint32_t token_count) {
    char* new_tokens[5]; // ORN Rd, XZR, Rn will always be 4 tokens
    new_tokens[0] = "msub";
    new_tokens[1] = tokens[1]; // Rd (destination)
    new_tokens[2] = tokens[2];     // Rn (first source is zero register)
    new_tokens[3] = tokens[3]; // Rm (second source is original Rn)
    new_tokens[4] = "ZR";

    return multiply_assembly(new_tokens, 5);
}



