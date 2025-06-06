#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "initialisation.h"
#include "pushbutton.h"
#include "display.h"
#include "timer.h"
#include "main.h"

// periodic interrupt every 1ms
ISR(TCB0_INT_vect) { 
    elapsed_time++;
    TCB0.INTFLAGS = TCB_CAPT_bm;
}//TCB0_INT_vect

// periodic interrupt every 5ms
ISR(TCB1_INT_vect) { 
    pb_debounce();
    swap_display_digit();    
    TCB1.INTFLAGS = TCB_CAPT_bm;
}//TCB1_INT_vect