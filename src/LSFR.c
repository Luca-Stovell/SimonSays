#include "LSFR.h"
#include "initialisation.h"
#include <stdio.h>

#include "initialisation.h"

uint32_t state = 0;
uint32_t lfsr_seed = 0;
uint32_t saved_state = 0;


void next_LSFR(void)
{
    uint8_t lsb = state & 0x1;
    state >>= 1;
    if (lsb) {
        state ^= 0xE2025CAB;
    }
}

uint8_t get_step(void)
{       
    return state & 0x03;  // Get a number between 0–3
}

void reset_LSFR(void)
{
    state = saved_state;
}

void advance_LSFR(uint16_t steps)
{
    for (uint16_t i = 0; i < steps; i++) {
        next_LSFR();
    }
    saved_state = state;  // Save the new state after advancing
}
