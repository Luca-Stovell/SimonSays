#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <stdint.h>

/**
 * @brief Debounces the pushbutton inputs.
 * 
 * Filters out spurious transitions caused by mechanical bounce, 
 * ensuring stable button state detection.
 */
void pb_debounce(void);

/**
 * @brief Stores the current debounced state of the pushbuttons.
 * 
 * Each bit represents the state of a button (pressed or released).
 */
extern volatile uint8_t pb_debounced_state;

#endif
