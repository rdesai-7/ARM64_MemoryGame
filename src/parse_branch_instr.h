#ifndef PARSE_BRANCH_INSTR_H
#define PARSE_BRANCH_INSTR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define COND_EQ 0x0
#define COND_NE 0x1
#define COND_GE 0xA
#define COND_LT 0xB
#define COND_GT 0xC
#define COND_LE 0xD
#define COND_AL 0xE
#define CONST_COND 0x54U
#define CONST_UNCOND 0x5U
#define CONST_REG 0x3587C0U

extern uint32_t parse_b_conditional( char **tokens, int num_tokens, ARM_STATE *state );
extern uint32_t parse_b_unconditional( char **tokens, int num_tokens, ARM_STATE *state );
extern uint32_t parse_br_register( char **tokens, int num_tokens, ARM_STATE *state );


#endif