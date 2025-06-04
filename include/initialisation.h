#ifndef INITIALISATION_H
#define INITIALISATION_H

extern uint32_t state;
extern uint32_t lfsr_seed ;
extern uint32_t saved_state;

void buzzer_init(void);
/**
 * @brief Initializes the buttons on PORTA pins 4-7.
 * 
 * Configures pull-up resistors and falling edge interrupts for pushbuttons S1-S4.
 */

void buttons_init(void);

/**
 * @brief Initializes the Linear Feedback Shift Register (LFSR) used for pseudo-random number generation.
 * 
 * Loads the LFSR with the initial seed value based on the student number.
 */
void init_LSFR(void);

/**
 * @brief 
 */

void timer_init(void);

void potentionmeter_init(void);

void uart_init(void);

void adc_init();

#endif
