#ifndef PARSE_DP_H
#define PARSE_DP_H

#include <stdint.h>
#include <stddef.h>
#include "defns.h"
#include "assembler_state.h"
#include "parse_helpers.h"

#define MUL_INITIAL_STATE (((uint32_t)0xD8) << 21)
#define BIT_LOGIC_INITIAL_STATE (((uint32_t)0xA) << 24) 
#define ADD_SUB_REGISTER_INITIAL_STATE (((uint32_t)0xB) << 24)
#define ADD_SUB_IMMEDIATE_INITIAL_STATE (((uint32_t)0x22) << 23)
#define MOV_INITIAL_STATE (((uint32_t)0x25) << 23)

typedef struct {
    const char* mnemonic;
    uint32_t opc;
    uint32_t N;
} InstructionOpcodeData;

extern const InstructionOpcodeData opcode_lookup_table[];
extern const size_t NUM_OPCODE_ENTRIES;

typedef struct {
    const char* mnemonic;
    uint32_t opc;
} AddSubOpcodeData;

extern const AddSubOpcodeData add_sub_opcode_table[];
extern const size_t NUM_ADD_SUB_ENTRIES;

typedef struct {
    const char* mnemonic;
    uint32_t opc;
} MovWOpcData;

extern const MovWOpcData mov_w_opcode_table[];
extern const size_t NUM_MOV_W_ENTRIES;

extern uint32_t multiply_assembly(char** tokens, int token_count, ARM_STATE *state);
extern uint32_t bit_logic_assembly(char** tokens, int token_count, ARM_STATE *state);
extern uint32_t assemble_add_sub_instruction(char** tokens, int token_count, ARM_STATE *state);
extern uint32_t mov_wide_assembly(char** tokens, int token_count, ARM_STATE *state);

extern uint32_t parse_add_sub_alias(char** tokens, int token_count, ARM_STATE *state);
extern uint32_t parse_compare(char** tokens, int token_count, ARM_STATE *state);
extern uint32_t parse_mov_alias(char** tokens, int token_count, ARM_STATE *state);
extern uint32_t parse_mul_alias(char** tokens, int token_count, ARM_STATE *state);

#endif