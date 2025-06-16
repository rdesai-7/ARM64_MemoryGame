#ifndef PARSE_LS_H
#define PARSE_LS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "symbol_table.h"
#include "pass_one.h"
#include "pass_two.h"
#include "parse_DP.h"

// Addressing Modes
typedef enum {
    U_OFFSET,
    PRE_INDEXED,
    POST_INDEXED,
    REG_OFFSET
} addrmode_t;

extern uint32_t parse_imm(char *token);
extern addrmode_t set_addrmode(char **tokens, int num_toks);
extern uint32_t parse_loadstore(char **tokens, int num_toks, ARM_STATE *state);

// initial binary encodings
#define LOADLIT_ENCODING 0x18000000
#define PREPOST_ENCODING 0x1
#define REGOFFSET_ENCODING 0x81A
#define SDT_ENCODING 0xB8000000


#endif