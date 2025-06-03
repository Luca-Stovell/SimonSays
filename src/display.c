#include <avr/io.h>
#include "display_macros.h"

#include <avr/interrupt.h>

volatile uint8_t digit_l = DISP_OFF;
volatile uint8_t digit_r = DISP_OFF;
volatile uint8_t dp_enable = 0;

void display_init(void) {
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;  // SPI pins on PC0-3

    // SPI SCK and MOSI
    PORTC.DIRSET = (PIN0_bm | PIN2_bm);   // SCK (PC0) and MOSI (PC2) output

    // DISP_LATCH
    PORTA.OUTSET = PIN1_bm;        // DISP_LATCH initial high
    PORTA.DIRSET = PIN1_bm;        // set DISP_LATCH pin as output

    PORTB.DIRSET = PIN5_bm;        //Set DISP_DP to output 
    PORTB.OUTSET = PIN5_bm;        //Set DISP_DP to off. 

    SPI0.CTRLA = SPI_MASTER_bm;    // Master, /4 prescaler, MSB first
    SPI0.CTRLB = SPI_SSD_bm;       // Mode 0, client select disable, unbuffered
    SPI0.INTCTRL = SPI_IE_bm;      // Interrupt enable
    SPI0.CTRLA |= SPI_ENABLE_bm;   // Enable
}

// Set the display segments (this replaces set_display_segments)
void set_display_segments(uint8_t segs_l, uint8_t segs_r) {
    digit_l = segs_l;
    digit_r = segs_r;
}

// Timer-driven multiplexing: swap digits
void swap_display_digit(void) {
    static int digit = 0;

    if (digit) {
        // LEFT digit pass
        if (dp_enable)
            PORTB.OUTCLR = PIN5_bm;  // DP ON if enabled
        else
            PORTB.OUTSET = PIN5_bm;  // DP OFF if not enabled

        SPI0.DATA = digit_l | (1 << 7);
    } else {
        // RIGHT digit pass — always DP OFF
        PORTB.OUTSET = PIN5_bm;
        SPI0.DATA = digit_r;
    }

    digit = !digit;
}



void display_off(void)
{
    set_display_segments(DISP_OFF, DISP_OFF);
    dp_enable = 0;
}

void display_pattern_1(){
    set_display_segments(DISP_BAR_LEFT, DISP_OFF);
}

void display_pattern_2(){
    set_display_segments(DISP_BAR_RIGHT, DISP_OFF);
}

void display_pattern_3(){
    set_display_segments(DISP_OFF, DISP_BAR_LEFT);
}

void display_pattern_4(){
    set_display_segments(DISP_OFF, DISP_BAR_RIGHT);
}

void display_success_pattern(){
    set_display_segments(DISP_ON, DISP_ON);
}

void display_fail_pattern(){
    set_display_segments(DISP_SEG_G, DISP_SEG_G);
}

uint8_t seg_decoder(uint8_t num)
{
    switch (num)
    {                  // FABGCDE 
        case 0: return 0b00001000;
        case 1: return 0b01101011;
        case 2: return 0b01000100;
        case 3: return 0b01000001;
        case 4: return 0b00100011;
        case 5: return 0b00010001;
        case 6: return 0b00010000;
        case 7: return 0b01001011;
        case 8: return 0b00000000;
        case 9: return 0b00000001;
        default: return DISP_OFF;
    }
}


void display_score(uint16_t score)
{
    uint8_t tens, ones;

    // Get two least significant digits
    uint8_t last_two_digits = score % 100;
    tens = last_two_digits / 10;
    ones = last_two_digits % 10;

    uint8_t left_display;

    if (score < 10)
    {
        left_display = DISP_OFF;  // blank left digit for 0-9
    }
    else
    {
        left_display = seg_decoder(tens);  // normal tens digit
    }

    // DP control: ON for scores < 100, OFF otherwise
    if (score < 100)
    {
        dp_enable = 1;
    }
    else
    {
        dp_enable = 0;
    }

    digit_l = left_display;
    digit_r = seg_decoder(ones);
}



ISR(SPI0_INT_vect) {
    PORTA.OUTCLR = PIN1_bm;
    PORTA.OUTSET = PIN1_bm;
    SPI0.INTFLAGS = SPI_IF_bm;
}
