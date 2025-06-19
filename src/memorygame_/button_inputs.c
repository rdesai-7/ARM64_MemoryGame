#include "button_inputs.h"

static int num_zeros(int arr[], int len) {
    int n = 0;
    for (int i = 0; i < len; i++) {
        if (arr[i] == 0) {
            n++;
        }
    }
    return n;
}

void print_int_arr(int arr[], int len, char *starting_str) {
    printf("%s",starting_str);
    for (int i = 0; i < len; i++) {
        printf("%d, ",arr[i]);
    }
    printf("\n");
}

// read seq_len button inputs from user, and store this in user_sequence
// returns a bool, false if user gives bad input
bool get_user_sequence_input(game_state_t *game_state) {
    int expected_n = game_state->seq_len;
    int prev_button_inps[NUM_BUTTONS];
    for (int i = 0; i < NUM_BUTTONS; i++) {
        prev_button_inps[i] = 1;
    }
    int button_inps[NUM_BUTTONS];
    struct timeval start, end;
    gettimeofday(&start, NULL);
    while(game_state->user_seq_len < expected_n) {

        gettimeofday(&end, NULL);
        double time_elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        if (time_elapsed > TIMEOUT) {
            print_int_arr(game_state->user_sequence,game_state->user_seq_len,"USER sequence: ");
            printf("No input for %d seconds, timed out. \n",TIMEOUT);
            return false;
        }

        // get inputs from each button
        for (int i = 0; i < NUM_BUTTONS; i++) {
            // get line for the button, and store input in array
            button_inps[i] = gpiod_line_get_value(game_state->button_lines[i]);
        }

        if (num_zeros(button_inps,NUM_BUTTONS) > 1) {
            print_int_arr(game_state->user_sequence,game_state->user_seq_len,"USER sequence: ");
            printf("Bad input - Multiple buttons pressed at once.\n");
            return false;
        }
        // we are now guaranteed to have either 0 or 1 button pressed.

        // process inputs received
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (button_inps[i] == 0 && prev_button_inps[i] == 1){
                // button i was pressed!

                gettimeofday(&start, NULL); //reset timer
                
                // NOTE: no need to check array is full, since we check if led_sequence is full in append_to_sequence()
                game_state->user_sequence[game_state->user_seq_len++] = i;
                
                usleep(DEBOUNCE_DELAY);
            }
        }

        // update previous state
        for (int i = 0; i < NUM_BUTTONS; i++) {
            prev_button_inps[i] = button_inps[i];
        }

        usleep(INPUT_READ_DELAY);
    }
    print_int_arr(game_state->user_sequence,game_state->user_seq_len,"USER sequence: ");
    return true;
}