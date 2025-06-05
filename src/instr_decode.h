#include <stdint.h>

//Decode function pointers
typedef void (*func_decode) ( DECODED_INSTR *decoded, uint32_t );

extern void decodeDPImmediate ( DECODED_INSTR *decoded, uint32_t instr );
extern void decodeDPRegister ( DECODED_INSTR *decoded, uint32_t instruction );
  //check m for multiply
  //check bit 24 for arithmetic/logic
extern void decodeLoadStore ( DECODED_INSTR *decoded, uint32_t instruction );
extern void decodeBranch ( DECODED_INSTR *decoded, uint32_t instruction );