#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "game_state.h"
#include "end_outputs.h"
#include "button_inputs.h"

int led_pins[] = {L0_PIN, L1_PIN, L2_PIN, L3_PIN, L4_PIN, L5_PIN};
int button_pins[] = {B0_PIN, B1_PIN, B2_PIN, B3_PIN, B4_PIN, B5_PIN};
const char *chipname = "gpiochip0";

void reset(game_state_t *game_state) {
    game_state->mode = IDLE;
    game_state->seq_len = 0;
    game_state->user_seq_len = 0;
}

void initialise(game_state_t *game_state) {
    reset(game_state);

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
    printf("success!\n");
    game_state->mode = LED_FLASH;
    game_state->user_seq_len = 0;
}

void failure(game_state_t *game_state) {
    printf("failure :( \n");
    display_failure(game_state);
    reset(game_state);
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

// TODO: return bool for error checking
void append_to_sequence(game_state_t *game_state) {
    if (game_state->seq_len == MAX_SEQ_LEN) {
        printf("LED sequence has reached maximum length.\n");
        display_success(game_state);
        reset(game_state);
        game_state->mode = IDLE;
        return;
    }
    // random number 0 to NUM_BUTTONS-1
    int new_val = rand() % NUM_BUTTONS;  
    //printf("I JUST MADE A RANDOM NUMBER, HERE IT IS %d\n", new_val);
    game_state->led_sequence[game_state->seq_len] = new_val;
    game_state->seq_len++;
}

int main(int argc, char *argv[]) {
    printf("lets play MEMSET! Press Ctrl+C to exit.\n");
    //srand(time(NULL));

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand(ts.tv_nsec ^ ts.tv_sec);


    // initialise state
    game_state_t game_state;
    initialise(&game_state);

    while(true) {
        switch(game_state.mode){
            case IDLE: {
                int b0_inp, b5_inp;
                printf("mode: IDLE\n");
                while(true) {
                    b0_inp = gpiod_line_get_value(game_state.button_lines[0]);
                    b5_inp = gpiod_line_get_value(game_state.button_lines[5]);

                    if (b0_inp == 0 && b5_inp == 0) {
                        game_state.mode = LED_FLASH;
                        break;
                    }
                    
                    usleep(INPUT_READ_DELAY * 2);
                }
                break;
            }
            case LED_FLASH:
                printf("mode: LED_FLASH\n");
                usleep(FLASH_TIME * 2);
                // appends a random number 0..NUM_BUTTONS-1 to the sequence, incrementing seq_len
                append_to_sequence(&game_state);

                print_int_arr(game_state.led_sequence,game_state.seq_len,"LED sequence: ");
                
                flash_led_seq(&game_state);
                game_state.mode = PLAYER_TURN;
                break;
            case PLAYER_TURN:
                printf("mode: PLAYER_TURN\n");

                if (get_user_sequence_input(&game_state) && check_seq(&game_state)) {
                    success(&game_state);
                } else {
                    failure(&game_state);
                }
                break;
        }
    }
    return 0;
}