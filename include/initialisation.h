#ifndef INITIALISATION_H
#define INITIALISATION_H

typedef enum {
    NO_BUTTON_PRESSED = -1,
    BUTTON_1 = 0,
    BUTTON_2 = 1,
    BUTTON_3 = 2,
    BUTTON_4 = 3
} Button;


void buttons_init(void);

void led_init(void);

#endif
