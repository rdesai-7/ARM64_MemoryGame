#ifndef PASS_TWO_H
#define PASS_TWO_H

#include "assembler_state.h"

extern bool is_directive(char *s);
extern uint32_t parse_directive(char *line);
extern bool run_pass_two(const char *filename, ARM_STATE *state);

typedef uint32_t (*parse_func)(char **tokens, int num_toks, ARM_STATE *state);

typedef struct {
    const char *mnemonic;
    parse_func parser;
} instruction_entry_t;

#endif