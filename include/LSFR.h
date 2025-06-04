#ifndef LSFR_H
#define LSFR_H

#include <stdint.h>

/**
 * @brief The current state of the Linear Feedback Shift Register (LFSR).
 * 
 * Used for pseudo-random sequence generation.
 */
extern uint32_t state;

/**
 * @brief Advances the LFSR by one step.
 * 
 * Updates the state based on the feedback function to generate the next pseudo-random value.
 */
void next_LSFR(void);

/**
 * @brief Resets the LFSR to its initial saved state.
 * 
 * Useful for replaying a previously generated sequence.
 */
void reset_LSFR(void);

/**
 * @brief Advances the LFSR by a specified number of steps.
 * 
 * Repeatedly applies the feedback function to skip ahead in the pseudo-random sequence.
 * 
 * @param steps The number of steps to advance.
 */
void advance_LSFR(uint16_t steps);

/**
 * @brief Retrieves the next 2-bit value from the current LFSR state.
 * 
 * Used to select the next step in the game sequence (value between 0 and 3).
 * 
 * @return A 2-bit value (0–3) representing the next sequence step.
 */
uint8_t get_step(void);

#endif // LSFR_H
