#include <avr/io.h>
#include "initialisation.h"
#include "buzzer.h"

volatile uint8_t is_playing = 0;
volatile uint8_t selected_tone = 0;
volatile uint8_t octave = 0;

#define MIN_OCTAVE -3
#define MAX_OCTAVE 3

void increase_octave(void)
{
    if (octave < MAX_OCTAVE)
    {
        octave++;

        if (is_playing)
            play_tone(selected_tone);
    }
}

void decrease_octave(void)
{
    if (octave > MIN_OCTAVE)
    {
        octave--;

        if (is_playing)
            play_tone(selected_tone);
    }
}

void update_tone(uint8_t new_tone)
{
    // Update the tone if already active
    if (is_playing)
        play_tone(new_tone);
    else
        // otherwise, select a new tone for the next time a tone is played
        selected_tone = new_tone;
}

void play_selected_tone(void)
{
    play_tone(selected_tone);
}

void play_tone(uint8_t tone)
{
    // Frequencies: 341, 287, 455, 170
    static const uint16_t per_values[4] = {9775, 11616, 7324, 19607};

    TCA0.SINGLE.PERBUF = per_values[tone];
    TCA0.SINGLE.CMP0BUF = per_values[tone] >> 1;
}//play_tone

void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
    is_playing = 0;
}//stop_tone

