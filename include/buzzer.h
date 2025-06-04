#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>


void play_tone(uint8_t button);  // <-- Add this
void stop_tone(void);

void increase_octave(void);
void decrease_octave(void);

#endif