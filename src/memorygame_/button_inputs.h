#ifndef BUTTON_INPUTS_H
#define BUTTON_INPUTS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "game_state.h"
#include <gpiod.h>

extern void get_user_sequence_input(game_state_t *game_state);

#endif