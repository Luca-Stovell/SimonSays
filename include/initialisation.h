#ifndef INITIALISATION_H
#define INITIALISATION_H

extern uint32_t state;
extern uint32_t lfsr_seed ;
extern uint32_t saved_state;

/**
 * @brief Initializes the 7-segment display interface.
 *
 * Configures SPI peripheral and related I/O pins for communication 
 * with the 7-segment display driver. Sets up SPI routing, output directions, 
 * DISP_LATCH control, and enables SPI interrupts.
 */
void display_init(void);

/**
 * @brief Initializes the buzzer output using TCA0 timer.
 *
 * Configures TCA0 timer peripheral in single-slope PWM mode to control 
 * the buzzer output on PB0. Sets initial PWM parameters and enables 
 * the timer for future buzzer control.
 */
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
 * @brief Initialise the 
 */

void timer_init(void);

/**
 * @brief Initializes the potentionmeter as an input on the Quty.
 */

void potentionmeter_init(void);

/**
 * @brief()
 */

void uart_init(void);

/**
 * @brief Initializes the ADC peripheral for analog-to-digital conversion.
 * 
 * Configures ADC0 to use VDD as the reference voltage, sets prescaler and timebase,
 * enables free-running mode, selects AIN2 as the input channel, and starts 
 * 8-bit single conversion mode.
 */

void adc_init();

#endif
