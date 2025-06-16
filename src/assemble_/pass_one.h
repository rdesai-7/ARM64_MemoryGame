#ifndef PASS_ONE_H
#define PASS_ONE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "symbol_table.h"
#include "pass_two.h"

#define LINE_BUFFER 256
#define ADDR_INCREMENT 4
#define MAX_LABEL_LENGTH 128 //potentially edit size

extern char* trim_whitespace(char *s);
extern bool is_line_empty(const char *line);
extern bool is_label(const char *line, char *label_name_out);
extern bool run_pass_one( const char *filename, SymbolTable_t st, uint32_t *program_size );

#endif