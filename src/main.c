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


#define MIN_PLAYBACK_DELAY 250
#define MAX_PLAYBACK_DELAY 2000


void state_machine(void);

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

GameState current_state = SIMON_GENERATE;

uint8_t current_button = 0;
uint16_t sequence[32];
uint16_t elapsed_time;
uint16_t sequence_length = 1;
uint16_t sequence_index = 0;
uint16_t playback_delay = MIN_PLAYBACK_DELAY;

// Pushbutton states
uint8_t pb_state_prev = 0xFF;  
uint8_t pb_state_curr = 0xFF;

// Pushbutton flags
uint8_t pb_falling_edge, pb_rising_edge, pb_released = 0;

int main(void)
{
    cli();
    display_init();
    buzzer_init();
    buttons_init();
    init_LSFR();
    timer_init();
    adc_init();
    sei();

    state_machine();

    while (1); // Should never reach here
}

void state_machine(void)
{
    while (1)
    {
        // Save state from previous iteration
        pb_state_prev = pb_state_curr;
        pb_state_curr = pb_debounced_state;

        // Find edges
        pb_falling_edge = (pb_state_prev ^ pb_state_curr) & pb_state_prev;
        pb_rising_edge  = (pb_state_prev ^ pb_state_curr) & pb_state_curr;

        playback_delay = (((uint16_t) (MAX_PLAYBACK_DELAY - MIN_PLAYBACK_DELAY) * ADC0.RESULT) >> 8) + MIN_PLAYBACK_DELAY;

        switch (current_state)
        {
            case SIMON_GENERATE:
                if (sequence_index == sequence_length) {
                    sequence_index = 0;
                    reset_LSFR();
                    current_state = AWAIT_INPUT;
                } else {
                    next_LSFR();
                    uint8_t step = get_step() + 1;
                    sequence[sequence_index] = step;  // store step into sequence
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
                        }
                    sequence_index++;
                    elapsed_time = 0;
                    current_state = SIMON_PLAY_ON;
                }
                break;

            case SIMON_PLAY_ON:
                if (elapsed_time >= (playback_delay >> 1)) {
                    stop_tone();
                    display_off();
                    current_state = SIMON_PLAY_OFF;
                }
                break;

            case SIMON_PLAY_OFF:
                if (elapsed_time >= playback_delay) {
                    current_state = SIMON_GENERATE;
                }
                break;

            case AWAIT_INPUT:
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
                if (!pb_released) {
                    if (pb_rising_edge & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)) {
                        pb_released = 1;        
                    }
                }
                else {                
                    if (elapsed_time >= (playback_delay >> 1)) {
                        stop_tone();
                        display_off();
                        elapsed_time = 0;
                        current_state = EVALUATE_INPUT;
                    }
                }
                break;

            case EVALUATE_INPUT:
                if (current_button == sequence[sequence_index]) {
                    sequence_index++;
                    if (sequence_index == sequence_length) {
                        display_success_pattern();
                        elapsed_time = 0;
                        sequence_index = 0;
                        sequence_length++;
                        current_state = SUCCESS;
                    } else {
                        current_state = AWAIT_INPUT;
                    }
                } else {
                    display_fail_pattern();
                    elapsed_time = 0;
                    current_state = FAIL;
                }
                break;

            case FAIL:
                if (elapsed_time >= playback_delay) {
                    display_score(sequence_length);
                    elapsed_time = 0;
                    current_state = DISP_SCORE;
                }
                break;

            case SUCCESS:
                if (elapsed_time >= playback_delay) {
                    display_off();
                    current_state = SIMON_GENERATE;
                }
                break;

            case DISP_SCORE:
                if (elapsed_time >= (playback_delay >> 1)) {
                    display_off();
                    advance_LSFR(sequence_length);
                    sequence_index = 0;
                    sequence_length = 1;
                    current_state = SIMON_GENERATE;
                }
                break;
        }
    }
}
