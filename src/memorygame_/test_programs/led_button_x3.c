#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

// Define our pins
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

int main(int argc, char **argv) {
    const char *chipname = "gpiochip0";
    struct gpiod_chip *chip;

    int led_pins[] = {L0_PIN, L1_PIN, L2_PIN, L3_PIN, L4_PIN, L5_PIN};
    int button_pins[] = {B0_PIN, B1_PIN, B2_PIN, B3_PIN, B4_PIN, B5_PIN};

    struct gpiod_line *led_lines[6];
    struct gpiod_line *button_lines[6];

    int b_vals[6];
    int l_states[] = {0,0,0,0,0,0};

    // Open the GPIO chip
    chip = gpiod_chip_open_by_name(chipname);
    assert(chip != NULL);
    
    for (int i = 0; i < 6; i++) {
        button_lines[i] =  gpiod_chip_get_line(chip, button_pins[i]);
        assert(button_lines[i] != NULL);

        led_lines[i] =  gpiod_chip_get_line(chip, led_pins[i]);
        assert(led_lines[i] != NULL);

        gpiod_line_request_output(led_lines[i], "test", 0); 
        gpiod_line_request_input_flags(button_lines[i], "test", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
    }


    printf("Program running. Press the button to toggle the LED. Press Ctrl+C to exit.\n");

    // Main loop
    while (1) {

        for (int i = 0; i < 6; i++) {
            b_vals[i] = gpiod_line_get_value(button_lines[i]);
        }

        for (int i = 0; i < 6; i++) {
            if (b_vals[i] == 0) {
                l_states[i] = !l_states[i];
                gpiod_line_set_value(led_lines[i], l_states[i]);
                printf("Button Pressed! LED %d (amber) is now %s\n",i ,l_states[i] ? "ON" : "OFF");
                usleep(200000); // 200ms
            }
        }
        
        // Small delay to prevent high CPU usage
        usleep(10000); // 10ms
    }

    // Release lines and close chip (never reached in this loop, but good practice)
    // gpiod_line_release(l0);
    // gpiod_line_release(l1);
    // gpiod_line_release(l2);
    // gpiod_line_release(b0);
    // gpiod_line_release(b1);
    // gpiod_line_release(b2);
    // gpiod_chip_close(chip);

    return 0;
}
