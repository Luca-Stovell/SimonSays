#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "initialisation.h"
/**
 * @brief Sets the segment patterns for the left and right 7-segment display digits.
 * 
 * Updates the global variables digit_l and digit_r with the provided segment patterns.
 * 
 * @param segs_l Segment pattern for the left digit.
 * @param segs_r Segment pattern for the right digit.
 */
void set_display_segments(uint8_t segs_l, uint8_t segs_r);

/**
 * @brief Performs multiplexing to alternate between the left and right 7-segment display digits.
 * 
 * Updates which digit is currently active to create the appearance of both digits being displayed simultaneously.
 */
void swap_display_digit(void);
/**
 * @brief Turns off both digits on the 7-segment display.
 */
void display_off(void);

/**
 * @brief Displays pattern 1 on the 7-segment display.
 * 
 * Used to indicate button 1 or step 1 in the game sequence.
 */
void display_pattern_1(void);

/**
 * @brief Displays pattern 2 on the 7-segment display.
 * 
 * Used to indicate button 2 or step 2 in the game sequence.
 */
void display_pattern_2(void);

/**
 * @brief Displays pattern 3 on the 7-segment display.
 * 
 * Used to indicate button 3 or step 3 in the game sequence.
 */
void display_pattern_3(void);

/**
 * @brief Displays pattern 4 on the 7-segment display.
 * 
 * Used to indicate button 4 or step 4 in the game sequence.
 */
void display_pattern_4(void);

/**
 * @brief Displays the success pattern on the 7-segment display.
 * 
 * Used to indicate correct user input or successful completion of the sequence.
 */
void display_success_pattern(void);

/**
 * @brief Displays the fail pattern on the 7-segment display.
 * 
 * Used to indicate incorrect user input or failure in the game sequence.
 */
void display_fail_pattern(void);

/**
 * @brief Displays the current score on the 7-segment display.
 * 
 * @param score The score value to be displayed. Within the SimonSays it refelcts the sequence length.
 */
void display_score(uint16_t score);


#endif

