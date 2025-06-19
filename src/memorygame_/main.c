#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <gpiod.h>
#include "game_state.h"
#include "end_outputs.h"
#include "button_inputs.h"

int led_pins[] = {L0_PIN, L1_PIN, L2_PIN};
int button_pins[] = {B0_PIN, B1_PIN, B2_PIN};
const char *chipname = "gpiochip0";

void reset(game_state_t *game_state) {
    game_state->mode = IDLE;
    game_state->seq_len = 0;
    game_state->user_seq_len = 0;
}

void initialise(game_state_t *game_state) {
    reset(&game_state);

    struct gpiod_chip *chip = chip = gpiod_chip_open_by_name(chipname);

    for (int i = 0; i < NUM_BUTTONS; i++) {
        game_state->button_lines[i] =  gpiod_chip_get_line(chip, button_pins[i]);
        assert(game_state->button_lines[i] != NULL);

        game_state->led_lines[i] =  gpiod_chip_get_line(chip, led_pins[i]);
        assert(game_state->led_lines[i] != NULL);

        gpiod_line_request_output(game_state->led_lines[i], "memory game", 0);
        gpiod_line_request_input_flags(game_state->button_lines[i], "memory game", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
    }

}

void success(game_state_t *game_state) {
    display_success(&game_state);
    game_state->mode = LED_FLASH;
    game_state->user_seq_len = 0;
}

void failure(game_state_t *game_state) {
    display_failure(&game_state);
    reset(&game_state);
}

bool check_seq(game_state_t *game_state) {
    // PRE: seq_len == user_seq_len

    // check if led and user sequences match
    for (int i = 0; i < game_state->seq_len; i++) {
        if (game_state->led_sequence[i] != game_state->user_sequence[i]) {
            return false;
        }
    }
    return true;
}

void append_to_sequence(game_state_t *game_state) {
    if (game_state->seq_len == MAX_SEQ_LEN) {
        printf("LED sequence has reached maximum length.\n");
        return;
    }
    // random number 0 to NUM_BUTTONS-1
    int new_val = rand() % NUM_BUTTONS;  
    game_state->led_sequence[game_state->seq_len] = new_val;
    game_state->seq_len++;
}

void flash_led_seq(game_state_t *game_state) {
    for (int i = 0; i < game_state->seq_len; i++) {
        int led_num = game_state->led_sequence[i];
        flash_led(led_num, &game_state);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // initialise state
    game_state_t game_state;
    initialise(&game_state);

    while(true) {
        switch(game_state.mode){
            case IDLE:
                int b0_inp, b2_inp;
                while(true) {
                    int b0_inp = gpiod_line_get_value(game_state.button_lines[0]);
                    int b2_inp = gpiod_line_get_value(game_state.button_lines[2]);

                    if (b0_inp == 0 && b2_inp == 0) {
                        game_state.mode == LED_FLASH;
                        break;
                    }
                    
                    usleep(INPUT_READ_DELAY * 2);
                }
                break;
            case LED_FLASH:
                // appends a random number 0..NUM_BUTTONS-1 to the sequence, incrementing seq_len
                append_to_sequence(&game_state);
                flash_led_seq(&game_state);
                game_state.mode = PLAYER_TURN;
                break;
            case PLAYER_TURN:
                // optional: user can time-out if no input for X seconds

                get_user_sequence_input(&game_state); // get seq_len button inputs from user, store this in user_sequence

                bool success_flag = check_seq(&game_state);
                if (success_flag) {
                    // handle success
                    success();
                } else {
                    // handle failure
                    failure();
                }
                break;
        }
    }
    return 0;
}