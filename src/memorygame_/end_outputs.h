#ifndef END_OUTPUTS_H
#define END_OUTPUTS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <gpiod.h>
#include <unistd.h>
#include "game_state.h"

extern void flash_led(int led_num, game_state_t *game_state, int sleep_time, bool end_sleep);
extern void display_failure(game_state_t *game_state);
extern void flash_led_seq(game_state_t *game_state);

#endif