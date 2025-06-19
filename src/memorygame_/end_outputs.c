#include "end_outputs.h"

void flash_led(int led_num, game_state_t *game_state, int sleep_time) {
    gpiod_line_set_value(game_state->led_lines[led_num], 1);
    usleep(sleep_time);
    gpiod_line_set_value(game_state->led_lines[led_num], 0);
    usleep(sleep_time);
}

void display_success(game_state_t *game_state) {
    for (int i = 0; i < 10; i++) {
        flash_led(SUCCESS_LED_NUM, game_state, FLASH_TIME/4);
    }
}

void display_failure(game_state_t *game_state) {
    for (int i = 0; i < 10; i++) {
        gpiod_line_set_value(game_state->led_lines[0], 1);
        gpiod_line_set_value(game_state->led_lines[5], 1);
        usleep(FLASH_TIME/4);
        gpiod_line_set_value(game_state->led_lines[0], 0);
        gpiod_line_set_value(game_state->led_lines[5], 0);
        usleep(FLASH_TIME/4);
    }
    printf("Your score is %d! Have a great day!\n", game_state->seq_len - 1);
}


void flash_led_seq(game_state_t *game_state) {
    for (int i = 0; i < game_state->seq_len; i++) {
        int led_num = game_state->led_sequence[i];
        flash_led(led_num, game_state, FLASH_TIME);
    }
}