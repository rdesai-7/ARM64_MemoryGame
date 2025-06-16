#ifndef PARSE_DP_H
#define PARSE_DP_H

#include <stdint.h>
#include <stddef.h>
#include "assembler_state.h"
#include "parse_loadstore.h"

#define ZERO_REGISTER 31
#define STACK_POINTER 31

#define MUL_INITIAL_STATE (((uint32_t)0b0011011000) << 21)
#define BIT_LOGIC_INITIAL_STATE (((uint32_t)0b01010) << 24) 
#define ADD_SUB_REGISTER_INITIAL_STATE (((uint32_t)0b01011) << 24)
#define ADD_SUB_IMMEDIATE_INITIAL_STATE (((uint32_t)0b100010) << 23)
#define MOV_INITIAL_STATE (((uint32_t)0b100101) << 23)


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

extern uint32_t parse_imm(char* token);
extern uint32_t parse_shift_type(const char* shift_type_str);

extern uint32_t multiply_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t bit_logic_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t assemble_add_sub_instruction(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t mov_wide_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);

extern uint32_t cmp_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t cmn_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t neg_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t negs_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t tst_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t mvn_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t mov_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t mul_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);
extern uint32_t mneg_assembly(char** tokens, uint32_t token_count, ARM_STATE *state);

extern uint32_t get_logic_opcode(const char* mnemonic, uint32_t *N);
extern int get_add_sub_opcode(const char* mnemonic, uint32_t* op_out, uint32_t* S_out);
extern int get_mov_opcode(const char* mnemonic, uint32_t* opc_out);

#endif