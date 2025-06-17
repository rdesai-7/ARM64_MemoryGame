#ifndef PARSE_LS_H
#define PARSE_LS_H

#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include "defns.h"
#include "symbol_table.h"
#include "parse_helpers.h"
#include "assembler_state.h"

// Addressing Modes
typedef enum {
    U_OFFSET,
    PRE_INDEXED,
    POST_INDEXED,
    REG_OFFSET
} addrmode_t;

extern uint32_t parse_loadstore(char **tokens, int num_toks, ARM_STATE *state);

// initial binary encodings
#define LOADLIT_ENCODING 0x18000000
#define PREPOST_ENCODING 0x1
#define REGOFFSET_ENCODING 0x81A
#define SDT_ENCODING 0xB8000000
#define SIMM19_MASK 0x7FFFF


#endif