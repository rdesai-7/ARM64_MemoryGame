#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

// Define our pins
#define L0_PIN 26
#define B0_PIN 4
#define L1_PIN 19
#define B1_PIN 3
#define L2_PIN 13
#define B2_PIN 2

int main(int argc, char **argv) {
    const char *chipname = "gpiochip0";
    struct gpiod_chip *chip;
    struct gpiod_line *l0, *b0, *l1, *b1, *l2, *b2;
    int b0_val, b1_val, b2_val;
    int l0_state = 0;
    int l1_state = 0;
    int l2_state = 0;

    // Open the GPIO chip
    chip = gpiod_chip_open_by_name(chipname);
    assert(chip != NULL);

    // Get the line for the LEDs and buttons
    l0 = gpiod_chip_get_line(chip, L0_PIN);
    assert(l0 != NULL);

    l1 = gpiod_chip_get_line(chip, L1_PIN);
    assert(l1 != NULL);

    l2 = gpiod_chip_get_line(chip, L2_PIN);
    assert(l2 != NULL);

    b0 = gpiod_chip_get_line(chip, B0_PIN);
    assert(b0 != NULL);

    b1 = gpiod_chip_get_line(chip, B1_PIN);
    assert(b1 != NULL);

    b2 = gpiod_chip_get_line(chip, B2_PIN);
    assert(b2 != NULL);

    // Request the LED line as an output
    gpiod_line_request_output(l0, "led_button_x3", l0_state);
    gpiod_line_request_output(l1, "led_button_x3", l1_state);
    gpiod_line_request_output(l2, "led_button_x3", l2_state);


    // Request the Button line as an input with pull-up
    gpiod_line_request_input_flags(b0, "led_button_x3", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
    gpiod_line_request_input_flags(b1, "led_button_x3", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
    gpiod_line_request_input_flags(b2, "led_button_x3", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);

    printf("Program running. Press the button to toggle the LED. Press Ctrl+C to exit.\n");

    // Main loop
    while (1) {
        b0_val = gpiod_line_get_value(b0);
        b1_val = gpiod_line_get_value(b1);
        b2_val = gpiod_line_get_value(b2);

        // Check for a button press (pin goes from HIGH to LOW)
        if (b0_val == 0) {
            // Toggle the state
            l0_state = !l0_state;

            // Set the LED to the new state
            gpiod_line_set_value(l0, l0_state);
            
            printf("Button Pressed! LED 0 (green) is now %s\n", l0_state ? "ON" : "OFF");

            // VERY IMPORTANT: Debounce delay.
            // Wait for the button contact to stabilize to avoid multiple toggles.
            usleep(200000); // 200ms
        }

        if (b1_val == 0) {
            l1_state = !l1_state;
            gpiod_line_set_value(l1, l1_state);
            printf("Button Pressed! LED 1 (amber) is now %s\n", l1_state ? "ON" : "OFF");
            usleep(200000); // 200ms
        }

        if (b2_val == 0) {
            l2_state = !l2_state;
            gpiod_line_set_value(l2, l2_state);
            printf("Button Pressed! LED 2 (red) is now %s\n", l2_state ? "ON" : "OFF");
            usleep(200000); // 200ms
        }

        
        // Small delay to prevent high CPU usage
        usleep(10000); // 10ms
    }

    // Release lines and close chip (never reached in this loop, but good practice)
    gpiod_line_release(l0);
    gpiod_line_release(l1);
    gpiod_line_release(l2);
    gpiod_line_release(b0);
    gpiod_line_release(b1);
    gpiod_line_release(b2);
    gpiod_chip_close(chip);

    return 0;
}
