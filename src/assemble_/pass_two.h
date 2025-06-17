#ifndef PASS_TWO_H
#define PASS_TWO_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "defns.h"
#include "parse_helpers.h"
#include "assembler_state.h"
#include "parse_branch.h"
#include "parse_loadstore.h"
#include "parse_DP.h"

extern bool run_pass_two(const char *filename, ARM_STATE *state);

typedef uint32_t (*parse_func)(char **tokens, int num_toks, ARM_STATE *state);

typedef struct {
    const char *mnemonic;
    parse_func parser;
} instruction_entry_t;

#endif