#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gpiod.h>
#include "game_state.h"

void display_success() {
    for (i = 0; i < 3; i++) {
        flash_led(SUCCESS_LED_NUM);
    }
}

void display_failure() {
    for (i = 0; i < 3; i++) {
        flash_led(FAILURE_LED_NUM);
    }
}  

void flash_led(int led_num, game_state_t *game_state) {
    gpiod_line_set_value(game_state->led_lines[led_num], 1);
    usleep(FLASH_TIME);
    gpiod_line_set_value(game_state->led_lines[led_num], 0);
}
