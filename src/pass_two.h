#ifndef PASS_TWO_H
#define PASS_TWO_H

extern bool is_directive(char *s);
extern uint32_t parse_directive(char *line);
extern void run_pass_two( const char *filename, SymbolTable_t st);

#endif