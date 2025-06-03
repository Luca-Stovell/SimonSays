#ifndef INITIALISATION_H
#define INITIALISATION_H

extern uint32_t state;
extern uint32_t lfsr_seed ;
extern uint32_t saved_state;

void buzzer_init(void);

void buttons_init(void);

void init_LSFR(void);

void timer_init(void);

void uart_init(void);

void adc_init();

#endif
