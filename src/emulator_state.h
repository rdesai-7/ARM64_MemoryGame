// header file for the state of the ARMV8 Machine

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

//Decode function pointers
typedef void (*func_decode) ( *DECODED_INSTR, uint32_t );

//Execute function pointers
typedef void (*func_execute) ( *ARM_STATE ) ;

//Instruction Types
typedef enum {
    DP_IMMEDIATE,
    DP_REGISTER,
    LOAD_STORE,
    BRANCH
} instr_type;

//Decoded Instruction
typedef struct {
    bool sf;
    uint8_t opc;
    uint8_t opi;
    uint8_t rd;

    bool sh;
    uint16_t imm12;
    uint8_t rn;

    uint8_t hw;
    uint32_t imm16;

    uint8_t opr;
    uint8_t rm;

    uint8_t shift; //arithmetic+logical
    bool N; //logical only

    bool x;
    uint8_t ra;

    bool L;
    bool U;
    uint16_t offset;
    uint32_t simm19;
    uint8_t xn;
    uint8_t rt;

    uint8_t xm;
    uint16_t simm9;
    bool I;

    uint32_t simm26;
    uint8_t cond;
    
} DECODED_INSTR; 