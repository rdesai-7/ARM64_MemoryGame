#ifndef PARSE_LS_H
#define PARSE_LS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "symbol_table.h"
#include "pass_one.h"
#include "pass_two.h"
#include "emulate_/emulator_state.h"
#include "parse_dp.h"

extern uint32_t parse_imm(char *token);
extern addrmode_t set_addrmode(char **tokens, int num_toks);
extern uint32_t parse_loadstore(char **tokens, int num_toks, ARM_STATE *state);

// initial binary encodings
#define LOADLIT_ENCODING 0b00011000000000000000000000000000
#define PREPOST_ENCODING 1
#define REGOFFSET_ENCODING 0b100000011010
#define SDT_ENCODING 0b10111000000000000000000000000000


#endif