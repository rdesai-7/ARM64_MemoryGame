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
    PSTATE_Flags pstate;
    bool halt_flag;
    FILE *output;
} ARM_STATE;

