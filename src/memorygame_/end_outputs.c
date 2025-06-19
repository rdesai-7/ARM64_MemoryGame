#include "end_outputs.h"

void flash_led(int led_num, game_state_t *game_state) {
    gpiod_line_set_value(game_state->led_lines[led_num], 1);
    usleep(FLASH_TIME);
    gpiod_line_set_value(game_state->led_lines[led_num], 0);
}

void display_success(game_state_t *game_state) {
    for (int i = 0; i < 3; i++) {
        flash_led(SUCCESS_LED_NUM, &game_state);
    }
}

void display_failure(game_state_t *game_state) {
    for (int i = 0; i < 3; i++) {
        flash_led(FAILURE_LED_NUM, &game_state);
    }
}  