// header file for the state of the ARMV8 Machine

#ifndef EMULATOR_STATE_H
#define EMULATOR_STATE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Define constants for state
#define MEM_SIZE 2097152 // 2 * 2^20
#define NUM_GPR 31
#define HALT_INSTRUCTION 0x8a000000
#define PC_INCREMENT 0x4

// Struct for PSTATE register
typedef struct {
    bool N; // Negative
    bool Z; // Zero
    bool C; // Carry
    bool V; // Overflow
} PSTATE_Flags;

//Instruction Types
typedef enum {
    DP_IMMEDIATE,
    DP_REGISTER,
    LOAD_STORE,
    BRANCH
} instr_type;

//Branch Types
typedef enum {
    UNCOND,
    REG,
    COND
} branch_types;

// Load Store Types
typedef enum {
    SDT,
    LOADLITERAL
} loadstore_types;

// Addressing Modes
typedef enum {
    U_OFFSET,
    PRE_INDEXED,
    POST_INDEXED,
    REG_OFFSET
} addr_modes;

//Decoded Instruction
typedef struct {
    bool sf;
    uint8_t opc;
    uint8_t opi;
    uint8_t rd;

    bool sh;
    uint32_t imm12;
    uint8_t rn;

    uint8_t hw;
    uint32_t imm16;

    bool M;
    uint8_t opr;
    uint8_t rm;

    uint8_t shift; //arithmetic+logical
    bool N; //logical only
    
    uint8_t operand;
    bool x; //for multiply
    uint8_t ra;

    bool L;
    bool U;
    uint16_t offset;
    uint32_t simm19;
    uint8_t xn;
    uint8_t rt;

    //for addressing modes 
    uint8_t xm;
    uint16_t simm9;
    bool I;

    uint32_t simm26;
    uint8_t cond;

    branch_types branch_type;
    loadstore_types loadstore_type;
    addr_modes addr_mode;
    
} DECODED_INSTR; 

//Struct for ARMV8 state
typedef struct {
    uint8_t memory[MEM_SIZE];
    uint64_t registers[NUM_GPR];
    uint64_t pc;
    uint32_t instruction;
    DECODED_INSTR decoded;
    instr_type instruction_type;
    PSTATE_Flags pstate;
    bool halt_flag;
    FILE *output;
} ARM_STATE;

//Function definitions
uint32_t get_bits( uint32_t source, int start_bit, int end_bit );
uint64_t get_reg_val(ARM_STATE *state, uint8_t reg_id, bool is_64_bit);
void set_reg_val(ARM_STATE *state, uint8_t reg_id, uint64_t value, bool is_64_bit);
instr_type getInstructionType (uint32_t instr);
void storeMemory(ARM_STATE *state, uint32_t addr, bool is_64_bit, uint64_t value);
uint64_t loadMemory(ARM_STATE *state, uint32_t addr, bool is_64_bit) ;

#endif