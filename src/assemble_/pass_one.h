#ifndef PASS_ONE_H
#define PASS_ONE_H

#include <stdio.h>     
#include <stdint.h>    
#include <stdbool.h>    
#include <assert.h>    
#include "defns.h" 
#include "symbol_table.h"
#include "assembler_state.h"
#include "parse_helpers.h"

extern bool run_pass_one( const char *filename, SymbolTable_t st, uint32_t *program_size );

#endif