#include "parse_loadstore.h"

uint32_t parse_imm(char *token) {
    // PRE: token is of the form "#imm"
    // trailing chars ignored
    long val = strtol(token + 1, NULL, 0);
    return (uint32_t) val;
}

addrmode_t set_addrmode(char **tokens, int num_toks) {
    if (num_toks == 3) {
        // zero unsigned offset
        return U_OFFSET;
    } else if (tokens[2][strlen(tokens[2]) - 1] == ']') {
        return POST_INDEXED;
    } else if (tokens[3][0] != '#') {
        return REG_OFFSET;
    } else if (tokens[3][strlen(tokens[3]) - 1] == '!') {
        return PRE_INDEXED;
    } else {
        return U_OFFSET;
    }
}


uint32_t parse_loadstore(char **tokens, int num_toks, ARM_STATE *state) {
    assert(tokens != NULL);
    assert(state != NULL);

    char *rt_tok = tokens[1];
    uint32_t sf;
    uint32_t rt = parse_register_token(rt_tok,&sf);
    uint32_t L = (strcmp(tokens[0], "ldr") == 0) ? 1 : 0;
    uint32_t encoding;

    if (L == 1 && tokens[2][0] != '[') {
        // 'load literal' instruction
        assert(num_toks == 3);
        uint32_t address;
        if (tokens[2][0] == '#') {
            address = parse_imm(tokens[2]); 
        } else {
            getSymbolEntry(state->symbolTable, tokens[2], &address);
        }

        // ** DIV BY 4 MIGHT BE A MISTAKE **
        uint32_t simm19 = (address - state->curr_address) / ADDR_INCREMENT;
        
        encoding = LOADLIT_ENCODING | rt | (simm19 << 5) | (sf << 30);
    } else {
        // 'single data transfer' instruction
        addrmode_t addrmode = set_addrmode(tokens, num_toks);
        uint32_t U = 0;
        uint32_t xn = parse_register_token(tokens[2] + 1, NULL);
        uint32_t offset;
        
        switch (addrmode) {
            case U_OFFSET:
                U = 1;
                if (num_toks == 3) {
                    // zero unsigned offset
                    offset = 0;
                } else {
                    assert(num_toks == 4);
                    if (sf == 1) {
                        offset = parse_imm(tokens[3]) >> 3;
                    } else {
                        offset = parse_imm(tokens[3]) >> 2;
                    }
                }
                break;
            case REG_OFFSET:
                assert(num_toks == 4);
                uint32_t xm = parse_register_token(tokens[3], NULL);
                offset = REGOFFSET_ENCODING | (xm << 6);
                break;
            default:
                // pre or post indexed
                assert(num_toks == 4);
                uint32_t simm9 = parse_imm(tokens[3]);
                uint32_t I = (addrmode == PRE_INDEXED) ? 1 : 0;
                offset = PREPOST_ENCODING | (I << 1) | (simm9 << 2);
                break;
        }

        encoding = SDT_ENCODING | rt | (xn << 5) | (offset << 10) |
         (L << 22) | (U << 24) | (sf << 30);
    }
    return encoding;
}