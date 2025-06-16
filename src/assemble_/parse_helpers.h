#ifndef PARSE_HELPERS_H
#define PARSE_HELPERS_H

#include <stdint.h>
#include "assembler_state.h"

extern uint32_t parse_imm(char *token);
extern uint32_t parse_simm(char *token, int num_bits);
extern uint32_t parse_shift_type(const char* shift_type_str);
extern uint32_t parse_register_token(char* reg_token, uint32_t* sf_bit);

#endif 