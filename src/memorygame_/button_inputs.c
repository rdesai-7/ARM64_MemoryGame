#include "button_inputs.h"

// read seq_len button inputs from user, and store this in user_sequence
void get_user_sequence_input(game_state_t *game_state) {
    int expected_n = game_state->seq_len;
    int prev_button_inps[NUM_BUTTONS] = {1};
    int button_inps[NUM_BUTTONS];
    while(game_state->user_seq_len < expected_n) {

        // get inputs from each button
        for (int i = 0; i < NUM_BUTTONS; i++) {
            // get line for the button, and store input in array
            button_inps[i] = gpiod_line_get_value(game_state->button_lines[i]);
        }

        // process inputs received
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (button_inps[i] == 0 && prev_button_inps[i] == 1){
                // button i was pressed!
                
                // NOTE: no need to check array is full, since we check if led_sequence is full in append_to_sequence()
                game_state->user_sequence[game_state->user_seq_len++] = i;
                
                usleep(DEBOUNCE_DELAY);
            }
            prev_button_inps[i] = button_inps[i];
        }

        usleep(INPUT_READ_DELAY);
    }
}