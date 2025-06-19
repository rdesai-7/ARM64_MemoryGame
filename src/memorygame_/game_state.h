#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MAX_SEQ_LEN 100
#define NUM_BUTTONS 3
#define NUM_LEDS NUM_BUTTONS
#define FLASH_TIME 500000
#define DEBOUNCE_DELAY 200000
#define INPUT_READ_DELAY 10000
#define SUCCESS_LED_NUM 0
#define FAILURE_LED_NUM 2

#define L0_PIN 26
#define B0_PIN 4
#define L1_PIN 19
#define B1_PIN 3
#define L2_PIN 13
#define B2_PIN 2 


// game mode
typedef enum {
    IDLE,
    LED_FLASH,
    PLAYER_TURN
} game_mode_t;

// State struct
typedef struct {
    game_mode_t mode;
    int led_sequence[MAX_SEQ_LEN];
    int seq_len;
    int user_sequence[MAX_SEQ_LEN];
    int user_seq_len; 
    struct gpiod_line *led_lines[NUM_LEDS];
    struct gpiod_line *button_lines[NUM_BUTTONS];
} game_state_t;

#endif