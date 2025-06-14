#ifndef PASS_TWO_H
#define PASS_TWO_H

#include "assembler_state.h"

extern bool is_directive(char *s);
extern uint32_t parse_directive(char *line);
extern bool run_pass_two(const char *filename, ARM_STATE *state);

#endif