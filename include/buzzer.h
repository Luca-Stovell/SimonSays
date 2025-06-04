#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

/**
 * @brief Plays the required tone on the buzzer.
 * 
 * Sets the top values for each of the required frequencies
 * Loads the top
 * 
 */
void play_tone(uint8_t tone);

/**
 * @brief 
 */
void stop_tone(void);

void increase_octave(void);
void decrease_octave(void);

#endif