#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>


void display_init(void);

void set_display_segments(uint8_t segs_l, uint8_t segs_r);
void swap_display_digit(void);
void display_off(void);
void display_pattern_1(void);
void display_pattern_2(void);
void display_pattern_3(void);
void display_pattern_4(void);
void display_success_pattern(void);
void display_fail_pattern(void);
void display_score(uint16_t score);

#endif

