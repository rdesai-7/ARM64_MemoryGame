#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MAX_SEQ_LEN 100
#define NUM_BUTTONS 6
#define NUM_LEDS NUM_BUTTONS
#define FLASH_TIME 300000
#define DEBOUNCE_DELAY 50000  
#define INPUT_READ_DELAY 10000
#define TIMEOUT 3

#define L0_PIN 26 //red
#define B0_PIN 21
#define L1_PIN 19
#define B1_PIN 20
#define L2_PIN 13
#define B2_PIN 16
#define L3_PIN 15
#define B3_PIN 18
#define L4_PIN 4
#define B4_PIN 14
#define L5_PIN 3 //red
#define B5_PIN 2



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