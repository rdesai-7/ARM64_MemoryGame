#include <stdint.h>
#include "emulator_state.h" //NEED TO ADD GUARDS LATER

#define SHIFT_CONSTANT 16

extern void execute(ARM_STATE *state);

extern void executeDPImmediate( ARM_STATE *state);
extern void executeDPRegister( ARM_STATE *state);
extern void executeLoadStore( ARM_STATE *state);
extern void executeBranch( ARM_STATE *state);
