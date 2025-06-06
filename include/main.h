#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <stdint.h>


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

extern GameState current_state;
extern uint8_t   current_button;    // 1..4 for S1..S4
extern uint16_t  elapsed_time;      // timestamp since last tone or PB
extern uint8_t   pb_released;       // has the physical PB been released?
extern uint8_t   pb_falling_edge;   // set each ms by button debounce
extern uint8_t   pb_rising_edge;    // set each ms by button debounce
extern uint16_t sequence_length;   // Current sequence length
extern uint16_t sequence_index;    // Current index inside sequence
extern volatile uint8_t uart_requested_button;
extern volatile uint8_t uart_play;
extern volatile uint8_t uart_stop;

#endif