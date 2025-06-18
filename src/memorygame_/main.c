#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_state.h"


/* functions to make

func to get input from buttons

void display_success()
void display_failure()
void flash_led(int button_num);

void get_user_sequence_input(game_state_t *game_state)
^ read seq_len button inputs from user, and store this in user_sequence
*/

void initialise(game_state_t *game_state) {
    game_state->mode = IDLE;
    game_state->seq_len = 0;
    game_state->user_seq_len = 0;
}

void success(game_state_t *game_state) {
    display_success();
    game_state->mode = LED_FLASH;
    game_state->user_seq_len = 0;
}

void failure(game_state_t *game_state) {
    display_failure();
    initialise(&game_state);
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
        int button_num = game_state->led_sequence[i];
        flash_led(button_num,&game_state);
        usleep(FLASH_TIME); // sleep for 0.5s
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // initialise state
    game_state_t game_state;
    initialise(&game_state);

    while(true) {
        switch(game_state){
            case IDLE:
                while(true) {
                    // get input from buttons... waiting to start the game
                    // if (specific input from button) {
                    //     game_state.mode == LED_FLASH;
                    //     break; 
                    // }
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