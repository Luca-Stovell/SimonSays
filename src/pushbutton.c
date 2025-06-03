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

void buttons_init(void)
{
    // Enable pull-up resistors for PBs (no interrupts required)
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;  // S1
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;  // S2
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;  // S3
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;  // S4
}
