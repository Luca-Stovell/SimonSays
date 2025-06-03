#include <avr/io.h>
#include "initialisation.h"
#include "buzzer.h"


void play_tone(uint8_t tone)
{
    // Frequencies: 341, 287, 455, 176
    static const uint16_t per_values[4] = {9674, 11524, 7264, 18750};

    TCA0.SINGLE.PERBUF = per_values[tone];;
    TCA0.SINGLE.CMP0BUF = per_values[tone] >> 1;
}//play_tone

void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
}//stop_tone

