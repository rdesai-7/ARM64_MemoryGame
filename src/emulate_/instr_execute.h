#ifndef INSTR_EXECUTE_H
#define INSTR_EXECUTE_H

#include <stdint.h>
#include "emulator_state.h" //NEED TO ADD GUARDS LATER

#define SHIFT_CONSTANT 16

//Execute function pointers
typedef void (*func_execute) ( ARM_STATE *state ) ;

//Shift function pointers
typedef uint64_t (*func_shift) ( uint64_t operand, uint8_t sh_amt, bool is_64_bit );

//Helper functions
bool get_msb(uint64_t value, bool is_64bit);
void update_pstate_arith( ARM_STATE *state, uint64_t operand1, uint64_t operand2, uint64_t result, bool is_sub, bool is_64_bit );
void update_pstate_logical( ARM_STATE *state, uint64_t result, bool is_64_bit );
extern bool check_branch_cond(uint8_t cond, PSTATE_Flags pstate);


//Execute functions
extern void execute(ARM_STATE *state);
extern void executeDPImmediate( ARM_STATE *state);
extern void executeDPRegister( ARM_STATE *state);
extern void executeLoadStore( ARM_STATE *state);
extern void executeBranch( ARM_STATE *state);
#endif