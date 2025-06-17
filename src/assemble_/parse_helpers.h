#ifndef PARSE_HELPERS_H
#define PARSE_HELPERS_H

#include <stdint.h>
#include <ctype.h>  
#include <string.h>   
#include <stdio.h>    
#include <stdbool.h> 
#include <assert.h>
#include "defns.h"
#include "assembler_state.h"

extern char* trim_whitespace(char *s);
extern bool is_line_empty(const char *line);
extern bool is_label(const char *line, char *label_name_out);
extern bool is_directive(char *s);
extern uint32_t parse_directive(char *line);
extern uint32_t parse_imm(char *token);
extern uint32_t parse_simm(char *token, int num_bits);
extern uint32_t parse_shift_type(const char* shift_type_str);
extern uint32_t parse_register_token(char* reg_token, uint32_t* sf_bit);

#endif 