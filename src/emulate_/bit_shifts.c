#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulator_state.h"
#include "instr_decode.h"
#include "instr_execute.h"

// Logical shift Left
uint64_t lsl( uint64_t operand, uint8_t sh_amt, bool is_64_bit ) {
    uint64_t val = 0;
    if (is_64_bit) {
        val = operand << sh_amt;
        return val;
    } else {
        val = (uint32_t)operand << sh_amt;
        return val;
    }
}

// Logical shift right
uint64_t lsr( uint64_t operand, uint8_t sh_amt, bool is_64_bit ) {
    uint64_t val = 0;
    if (is_64_bit) {
        val = operand >> sh_amt;
        return val;
    } else {
        val = (uint32_t)operand >> sh_amt;
        return val;
    }
}

// Arithmetic shift right
uint64_t asr( uint64_t operand, uint8_t sh_amt, bool is_64_bit ) {
    uint64_t val = 0;
    bool msb = false;
    if (sh_amt == 0) {
            return operand;
        }

    if (is_64_bit) {
        msb = get_msb(operand, is_64_bit); // true for 1 false for 0
        val = operand >> sh_amt;

        if (msb) {
            uint64_t mask = 0;
            mask = UINT64_MAX << (64 - sh_amt); // UINT64_MAX GIVES 64 BIT 1s
            val |= mask;
        }
        return val;
    } else {// 32 bit register
        msb = get_msb(operand, is_64_bit);
        val = (uint32_t)operand >> sh_amt;

        if (msb) {
            uint32_t mask = 0;
            mask = UINT32_MAX << (32 - sh_amt);
            val |= mask;
        }
        return val;
    }
}

//Rotate right
uint64_t ror(uint64_t operand, uint8_t sh_amt, bool is_64_bit) {
    uint8_t width = is_64_bit ? 64 : 32;
    uint8_t ra = sh_amt % width;

    if (ra == 0) {
        return operand; // No rotation needed
    }

    if (is_64_bit) {
        uint64_t val64 = operand;
        uint64_t right_part = val64 >> ra;
        uint64_t wrapped_part = val64 << (width - ra);
        return right_part | wrapped_part;
    } else {// 32 bit
        uint32_t val32 = (uint32_t)operand;

        uint32_t right_part = val32 >> ra;

        uint32_t wrapped_part = val32 << (width - ra);
        return (uint64_t)(right_part | wrapped_part); 
    }
}
