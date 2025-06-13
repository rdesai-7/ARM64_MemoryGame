#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"
#include "instr_decode.h"
#include "instr_execute.h"

extern uint64_t lsl( uint64_t operand, uint8_t sh_amt, bool is_64_bit );

extern uint64_t lsr( uint64_t operand, uint8_t sh_amt, bool is_64_bit );

extern uint64_t asr( uint64_t operand, uint8_t sh_amt, bool is_64_bit );

extern uint64_t ror(uint64_t operand, uint8_t sh_amt, bool is_64_bit);

typedef uint64_t (*shift_func_t) (uint64_t, uint8_t, bool);