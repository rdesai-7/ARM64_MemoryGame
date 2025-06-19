#ifndef BUTTON_INPUTS_H
#define BUTTON_INPUTS_H

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "game_state.h"
#include <gpiod.h>

extern void print_int_arr(int arr[], int len, char *starting_str);
extern bool get_user_sequence_input(game_state_t *game_state);

#endif