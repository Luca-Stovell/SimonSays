#include <avr/io.h>
#include "pushbutton.h"

// Declare the global debounced state variable
volatile uint8_t pb_debounced_state = 0xFF;

void pb_debounce(void) {
    static uint8_t vcount1 = 0;  // vertical counter MSB
    static uint8_t vcount0 = 0;  // vertical counter LSB
     
    uint8_t pb_sample = PORTA.IN;
    uint8_t pb_changed = pb_sample ^ pb_debounced_state;

    // Increment vertical counter
    vcount1 = (vcount1 ^ vcount0) & pb_changed;
    vcount0 = ~vcount0 & pb_changed;

    pb_debounced_state ^= (vcount0 & vcount1);
}
