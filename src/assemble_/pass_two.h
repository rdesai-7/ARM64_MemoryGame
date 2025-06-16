#ifndef PASS_TWO_H
#define PASS_TWO_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"
#include "pass_one.h"
#include "assembler_state.h"
#include "parse_branch.h"
#include "parse_loadstore.h"
#include "parse_DP.h"

extern bool is_directive(char *s);
extern uint32_t parse_directive(char *line);
extern bool run_pass_two(const char *filename, ARM_STATE *state);

typedef uint32_t (*parse_func)(char **tokens, int num_toks, ARM_STATE *state);

typedef struct {
    const char *mnemonic;
    parse_func parser;
} instruction_entry_t;

#endif