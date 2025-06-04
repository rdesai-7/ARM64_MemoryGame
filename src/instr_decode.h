#include <stdint.h>

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

//Decode function pointers
typedef void (*func_decode) ( DECODED_INSTR *decoded, uint32_t );

extern void decodeDPImmediate ( DECODED_INSTR *decoded, uint32_t instr );
extern void decodeDPRegister ( DECODED_INSTR *decoded, uint32_t instruction );
  //check m for multiply
  //check bit 24 for arithmetic/logic
extern void decodeLoadStore ( DECODED_INSTR *decoded, uint32_t instruction );
extern void decodeBranch ( DECODED_INSTR *decoded, uint32_t instruction );