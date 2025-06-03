#include <avr/io.h>

#include "buzzer.h"

void buzzer_init(void) {

    // TCA0 will control the buzzer (PB0), so we need to enable it as an output
    PORTB.OUTCLR = PIN0_bm; // buzzer off initially
    PORTB.DIRSET = PIN0_bm; // Enable PB0 as output

     

    // Single-slope PWM mode, WO2 enable (PB5, LED DISP DP)    
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;

    // Enable overflow interrupt (interrupt at TOP)
    // TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;

    // PWM initially off
    TCA0.SINGLE.PER = 1;      
    TCA0.SINGLE.CMP0 = 0;    

    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;      // Enable TCA0 
}//buzzer_init


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

