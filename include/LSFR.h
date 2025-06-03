#ifndef LSFR_H
#define LSFR_H

#include <stdint.h>

extern uint32_t state;

void init_LSFR(void);       // Initialize LFSR with student number seed
void next_LSFR(void);       // Advance LFSR one step
void reset_LSFR(void);      // Reset LFSR to initial state
void advance_LSFR(uint16_t steps); // Advance LFSR by the number of sequence steps
uint8_t get_step(void);

                                    

#endif // LSFR_H
