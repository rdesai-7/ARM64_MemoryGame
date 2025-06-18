#include "game_state.h"

void initialise(game_state_t *game_state) {
    game_state->mode = IDLE;
    game_state->seq_len = 0;
    game_state->user_seq_len = 0;
}

void check_seq(game_state_t *game_state) {
    // PRE: seq_len == user_seq_len

    // check if led and user sequences match
    bool success_flag = true;
    for (int i = 0; i < seq_len; i++) {
        if (game_state->led_sequence[i] != game_state->user_sequence[i]) {
            success_flag = false;
        }
    }

    if (success_flag) {
        // handle success
        display_success();
        game_state->mode = LED_FLASH;
    } else {
        // handle failure
        display_failure();
        game_state->mode = IDLE;
    }

}

int main(int argc, char *argv[]) {

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
                flash_leds(&game_state);
                game_state.mode = PLAYER_TURN;
                break;
            case PLAYER_TURN:
                // sort out timeout.... !!
                // get seq_len button inputs from user, store this in user_sequence
                check_seq(&game_state);
                break;
        }
    }
    return 0;
}