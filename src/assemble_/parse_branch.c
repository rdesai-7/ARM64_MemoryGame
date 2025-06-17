#include "parse_branch.h"

// parse b conditional and unconditional could be generalised to prevent reuse of code?

static uint32_t map_cond_code( char *cond ) {
    if (strcmp(cond, "b.eq") == 0) { return COND_EQ; }
    if (strcmp(cond, "b.ne") == 0) { return COND_NE; }
    if (strcmp(cond, "b.ge") == 0) { return COND_GE; }
    if (strcmp(cond, "b.lt") == 0) { return COND_LT; }
    if (strcmp(cond, "b.gt") == 0) { return COND_GT; }
    if (strcmp(cond, "b.le") == 0) { return COND_LE; }
    if (strcmp(cond, "b.al") == 0) { return COND_AL; }
    fprintf(stderr, "Error: Unknown conditional branch mnemonic \n");
    return 0;
}

uint32_t parse_b_conditional( char **tokens, int num_tokens, ARM_STATE *state ) {
    uint8_t cond_code = map_cond_code ( tokens[0] );
    
    char label_name[MAX_LABEL_LENGTH];
    strcpy( label_name, tokens[1] );

    uint32_t label_address;
    getSymbolEntry( state->symbolTable, label_name, &label_address );

    int32_t offset = (int32_t)label_address - (int32_t)state->currAddress;
    int32_t simm19 = offset / ADDR_INCREMENT;

    uint32_t instr = ((uint32_t) (CONST_COND) << 24) | //put the constant part of the conditional instr
            (uint32_t)((simm19 & 0x7FFFF) << 5) | //Add offset value and mask with 0x7FFFF to unsure only 19 bits are used
            (cond_code & 0xF) << 0; //Add condition code and mask with oxF

    return instr;
}

uint32_t parse_b_unconditional( char **tokens, int num_tokens, ARM_STATE *state ) {
    char label_name[MAX_LABEL_LENGTH];
    strcpy( label_name, tokens[1] );

    uint32_t label_address;
    getSymbolEntry( state->symbolTable, label_name, &label_address );

    int32_t offset = (int32_t)label_address - (int32_t)state->currAddress;
    int32_t simm26 = offset / ADDR_INCREMENT;

    uint32_t instr = (uint32_t)(CONST_UNCOND << 26)              |
            (uint32_t)((simm26 & 0x3FFFFFF) << 0); //Mask with 0x3FFFFF so only 26 bits are used

    return instr;
}

uint32_t parse_br_register( char **tokens, int num_tokens, ARM_STATE *state ) {
    uint32_t sf = 0;
    uint32_t reg_num = parse_register_token(tokens[1], &sf);

    uint32_t instr = (uint32_t)(CONST_REG << 10) | (uint32_t)((reg_num & 0x1F) << 5); //Shift the preset bits

    return instr;

}
