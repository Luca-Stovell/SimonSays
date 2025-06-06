#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

extern volatile uint8_t is_playing;  
extern volatile uint8_t selected_tone;  
extern volatile uint8_t octave;  

/**
 * @brief Plays the required tone on the buzzer.
 * 
 * Sets the top values for each of the required frequencies
 * Loads the top
 * 
 */
void play_tone(uint8_t tone);

/**
 * @brief Stops tone playback by disabling the buzzer or PWM output.
 * 
 * Silences any currently playing tone.
 */
void stop_tone(void);

void recompute_all_periods(void);

void increase_octave(void);
void decrease_octave(void);
void update_tone(uint8_t new_tone);
void play_selected_tone(void);

#endif

