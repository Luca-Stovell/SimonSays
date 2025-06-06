#include "buzzer.h"
#include <stdint.h>
#include <avr/io.h>

// -----------------------------  BUZZER  -----------------------------

// Maximum and minimum octave offsets
#define MAX_OCTAVE   3
#define MIN_OCTAVE  -3

// “Base” periods for octave = 0 (reference).
//   per_base[0] = 9775   → Tone 0 at 341 Hz when octave = 0
//   per_base[1] = 11616  → Tone 1 at 287 Hz when octave = 0
//   per_base[2] = 7324   → Tone 2 at 455 Hz when octave = 0
//   per_base[3] = 19607  → Tone 3 at 170 Hz when octave = 0
static const uint16_t per_base[4] = {
    9775,
    11616,
    7324,
    19607
};

// “Current” periods, after applying the current octave shift.
// We will recompute these every time we call increase_ or decrease_octave().
 uint16_t cur_periods[4];

// Which tone (0..3) is currently “selected” for playback.
volatile uint8_t selected_tone = 0;

// Whether a tone is actively playing right now (1=yes, 0=no).
volatile uint8_t is_playing = 0;

// Signed octave offset in range [MIN_OCTAVE…MAX_OCTAVE].
//   octave = 0 → use per_base[] exactly.
//   octave = +1 → one octave up  → period = per_base[] >> 1.
//   octave = –1 → one octave down → period = per_base[] << 1, etc.
volatile uint8_t octave = 0;

/*
 * Call this at startup (e.g. inside buzzer_init()) to fill cur_periods[]
 * with the period values corresponding to octave = 0.
 */
void recompute_all_periods(void)
{
    // For each of the four base periods, shift according to (octave):
    for (uint8_t i = 0; i < 4; i++)
    {
        if (octave >= 0) {
            // For octave >= 0: shift right to halve period each octave up
            cur_periods[i] = (uint16_t)(per_base[i] >> octave);
        }
        else {
            // For octave < 0: shift left to double period each octave down
            cur_periods[i] = (uint16_t)(per_base[i] << (uint8_t)(octave));
        }
    }
}

/*
 * Increase octave by +1, up to MAX_OCTAVE.
 * Then recompute all four cur_periods[]. If a tone is currently playing,
 * immediately re‐play the selected_tone at its new period.
 */
void increase_octave(void)
{
    if (octave < MAX_OCTAVE)
    {
        octave++;
        recompute_all_periods();
        if (is_playing) {
            // Immediately update the PWM to the new period for selected_tone
            play_tone(selected_tone);
        }
    }
}

/*
 * Decrease octave by –1, down to MIN_OCTAVE.
 * Then recompute all four cur_periods[]. If a tone is currently playing,
 * immediately re‐play the selected_tone at its new period.
 */
void decrease_octave(void)
{
    if (octave > MIN_OCTAVE)
    {
        octave--;
        recompute_all_periods();
        if (is_playing) {
            play_tone(selected_tone);
        }
    }
}

/*
 * Called when the Simon state machine (or UART) wants to switch which
 * of the four tones (0..3) is “selected.” If a tone is already playing,
 * we immediately re‐play the new tone at the same octave; otherwise,
 * we only remember the new selection (and play it later).
 */
void update_tones(uint8_t new_tone)
{
    if (is_playing) {
        play_tone(new_tone);
    }
    else {
        selected_tone = new_tone;
    }
}

// Convenience wrapper: play whatever tone index was last “selected.”
void play_selected_tone(void)
{
    play_tone(selected_tone);
}

/*
 * Actually start (or re‐start) the buzzer at a particular tone index (0..3).
 * We look up cur_periods[tone], write that into the TCA0 PERBUF/CMP0BUF
 * registers, and mark is_playing = 1. That immediately produces a square wave
 * on the buzzer pin at the “current octave” pitch.
 */
void play_tone(uint8_t tone)
{
    uint16_t period = cur_periods[tone];    // Already shifted for current octave

    // Write into buffered registers so TCA0 updates on next cycle:
    TCA0.SINGLE.PERBUF  = period;
    // 50% duty cycle:
    TCA0.SINGLE.CMP0BUF = (period >> 1);

    selected_tone = tone;
    is_playing = 1;
}

// Silence the buzzer immediately (0% duty) and mark is_playing = 0.
void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
    is_playing = 0;
}
