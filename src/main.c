#include "display.h"
#include "initialisation.h"
#include "uart.h"
#include "buzzer.h"
#include "LSFR.h"
#include "pushbutton.h"
#include "timer.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

// Define min/max playback delays for potentiometer scaling
#define MIN_PLAYBACK_DELAY 250
#define MAX_PLAYBACK_DELAY 2000

#define MAX_SEQUENCE_LENGTH 128 //Limited by the available memory.

void state_machine(void);

// Define all game states using enum for state machine
typedef enum {
    SIMON_GENERATE,
    AWAIT_INPUT,
    HANDLE_INPUT,
    EVALUATE_INPUT,
    FAIL,
    SUCCESS,
    DISP_SCORE, 
    SIMON_PLAY_ON,
    SIMON_PLAY_OFF
} GameState;

// Global state machine variables
GameState current_state = SIMON_GENERATE;

uint8_t current_button = 0;
uint16_t sequence[MAX_SEQUENCE_LENGTH];   // Holds generated Simon sequence
uint16_t elapsed_time = 0;       // Tracks time since state transition
uint16_t sequence_length = 1;    // Current sequence length
uint16_t sequence_index = 0;     // Current index inside sequence
uint16_t playback_delay = MIN_PLAYBACK_DELAY;

// Button debounce states
uint8_t pb_state_prev = 0xFF;  
uint8_t pb_state_curr = 0xFF;

// Button edge detection flags
uint8_t pb_falling_edge, pb_rising_edge, pb_released = 0;

// UART control flags
volatile uint8_t uart_play = 0;
volatile uint8_t uart_stop = 0;
volatile uint8_t uart_requested_button = 0;

// Reads ADC, calculates scaled playback delay
static uint16_t prepare_delay(void)
{
    // Start single 8-bit ADC conversion
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;

    // Wait for ADC conversion complete
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)) { ; }

    uint8_t val = ADC0.RESULT;
    ADC0.INTFLAGS = ADC_RESRDY_bm;  // Clear ADC flag

    // Scale ADC value to calculate playback delay
    return playback_delay = (((uint16_t)(MAX_PLAYBACK_DELAY - MIN_PLAYBACK_DELAY) * val) >> 8) + MIN_PLAYBACK_DELAY;
}

int main(void)
{
    cli(); // Disable interrupts globally during initialisation

    // Hardware peripheral initialisation
    display_init();
    buzzer_init();
    buttons_init();
    init_LSFR();
    timer_init();
    adc_init();
    uart_init();

    sei(); // Enable interrupts globally

    state_machine();  // Enter main state machine loop

    while (1); // Should never reach here
}

void state_machine(void)
{
    while (1)
    {
        // Handle button edge detection
        pb_state_prev = pb_state_curr;
        pb_state_curr = pb_debounced_state;

        pb_falling_edge = (pb_state_prev ^ pb_state_curr) & pb_state_prev;
        pb_rising_edge  = (pb_state_prev ^ pb_state_curr) & pb_state_curr;

        switch (current_state)
        {
            case SIMON_GENERATE: {
                // Read potentiometer and calculate playback delay
                prepare_delay();
                // If sequence complete, transition to await input
                if (sequence_index == sequence_length) {
                    sequence_index = 0;
                    reset_LSFR();
                    current_state = AWAIT_INPUT;
                } else {
                    // Generate next random step using LFSR
                    next_LSFR(); 
                    uint8_t step = get_step() + 1;
                    sequence[sequence_index] = step;

                    // Output tone and display pattern based on step
                    switch (step)
                    {
                        case 1:
                            display_pattern_1();
                            play_tone(0);
                            break;
                        case 2:
                            display_pattern_2();
                            play_tone(1);
                            break;
                        case 3:
                            display_pattern_3();
                            play_tone(2);
                            break;
                        case 4:
                            display_pattern_4();
                            play_tone(3);
                            break;
                        default:
                            display_off();
                            stop_tone();
                    }
                    sequence_index++;
                    elapsed_time = 0;
                    current_state = SIMON_PLAY_ON;
                }
            }
            break;

            case SIMON_PLAY_ON:
                // Wait for tone duration (half delay)
                if (elapsed_time >= (playback_delay >> 1)) {
                    stop_tone();
                    display_off();
                    current_state = SIMON_PLAY_OFF;
                }
                break;

            case SIMON_PLAY_OFF:
                // Wait for full delay before next sequence step
                if (elapsed_time >= playback_delay) {
                    current_state = SIMON_GENERATE;
                }
                break;

            case AWAIT_INPUT:
                // Check for UART button press (simulated button input)
                if (uart_requested_button != 0)
                {
                    uint8_t b = uart_requested_button;
                    uart_requested_button = 0;

                    if (b == 1) {
                        display_pattern_1();
                        play_tone(0);
                        current_button = 1;
                    }
                    else if (b == 2) {
                        display_pattern_2();
                        play_tone(1);
                        current_button = 2;
                    }
                    else if (b == 3) {
                        display_pattern_3();
                        play_tone(2);
                        current_button = 3;
                    }
                    else if (b == 4) {
                        display_pattern_4();
                        play_tone(3);
                        current_button = 4;
                    }

                    elapsed_time = 0;
                    pb_released  = 0;
                    current_state = HANDLE_INPUT;
                }

                // Check for physical button press (PB4-PB7)
                if (pb_falling_edge & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)) {
                    if (pb_falling_edge & PIN4_bm) {
                        display_pattern_1();
                        play_tone(0);
                        current_button = 1;
                    }
                    else if (pb_falling_edge & PIN5_bm) {
                        display_pattern_2();
                        play_tone(1);
                        current_button = 2;
                    }
                    else if (pb_falling_edge & PIN6_bm) {
                        display_pattern_3();
                        play_tone(2);
                        current_button = 3;
                    }
                    else if (pb_falling_edge & PIN7_bm) {
                        display_pattern_4();
                        play_tone(3);
                        current_button = 4;
                    }

                    elapsed_time = 0;
                    pb_released = 0;
                    current_state = HANDLE_INPUT;
                }
                break;

            case HANDLE_INPUT:
                // Wait for button release
                if (!pb_released)
                {
                    if (pb_rising_edge & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm))
                    {
                        pb_released = 1;
                    }
                }
                if (pb_released)
                {
                    // Once released, wait half delay before evaluating input
                    if (elapsed_time >= (playback_delay >> 1))
                    {
                        stop_tone();
                        display_off();
                        current_state = EVALUATE_INPUT;
                    }
                }
                break;

            case EVALUATE_INPUT:
                // Check if player input matches sequence
                if (current_button == sequence[sequence_index]) {
                    sequence_index++;
                    if (sequence_index == sequence_length) {
                        display_success_pattern();
                        elapsed_time = 0;
                        sequence_index = 0;
                        
                        // Check if max sequence length reached
                        if (sequence_length < MAX_SEQUENCE_LENGTH) {
                            sequence_length++;
                            current_state = SUCCESS;
                        } else {
                            // End game if max reached
                            current_state = DISP_SCORE;
                        }
                    } else {
                        // Still more steps to evaluate
                        current_state = AWAIT_INPUT;
                    }
                } else {
                    // Player failed: show fail pattern
                    display_fail_pattern();
                    elapsed_time = 0;
                    current_state = FAIL;
                }  
                break;

            case FAIL:
                // After fail pattern delay, display score
                if (elapsed_time >= playback_delay) {
                    display_score(sequence_length);
                    elapsed_time = 0;
                    current_state = DISP_SCORE;
                }
                break;

            case SUCCESS:
                // After success pattern, start next round
                if (elapsed_time >= playback_delay) {
                    display_off();
                    current_state = SIMON_GENERATE;
                }
                break;

            case DISP_SCORE:
                // After score shown, reset game state
                if (elapsed_time >= (playback_delay >> 1)) {
                    display_off();
                    advance_LSFR(sequence_length);
                    sequence_index = 0;
                    sequence_length = 1;
                    current_state = SIMON_GENERATE;
                }
                break;

            default:
                // Emergency fallback for undefined states
                current_state = SIMON_GENERATE;
                sequence_index = 0;
                sequence_length = 1;
                elapsed_time = 0;
                display_off();
                stop_tone();
        }
    }
}
