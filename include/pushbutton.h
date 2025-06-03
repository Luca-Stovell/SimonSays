#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <stdint.h>

void pb_debounce(void);
void buttons_init(void);

// Add this extern declaration:
extern volatile uint8_t pb_debounced_state;

#endif