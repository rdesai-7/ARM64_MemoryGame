#include <stdint.h>

//Decode function pointers
typedef void (*func_decode) ( DECODED_INSTR *decoded, uint32_t );

extern void decode (ARM_STATE *state);
extern void decodeDPImmediate ( DECODED_INSTR *decoded, uint32_t instr );
extern void decodeDPRegister ( DECODED_INSTR *decoded, uint32_t instr );
extern void decodeLoadStore ( DECODED_INSTR *decoded, uint32_t instr );
extern void decodeBranch ( DECODED_INSTR *decoded, uint32_t instr );