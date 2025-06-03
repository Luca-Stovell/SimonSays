#include <avr/io.h>

#include "initialisation.h"

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

void buttons_init(void)
{
    // Enable pull-up resistors for PBs (no interrupts required)
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;  // S1
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;  // S2
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;  // S3
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;  // S4
}


void potentionmeter_init(void){
    PORTA.DIRCLR = (1 << 2);  // Makes PA2 an input pin
}

void init_LSFR(void)
{
    lfsr_seed = 0x11079355; 
    state = lfsr_seed;
    saved_state = state;
}

void timer_init(void) {

    // configure TCB0 for a periodic interrupt every 1ms
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;    // Configure TCB0 in periodic interrupt mode
    TCB0.CCMP = 3333;                   // Set interval for 1ms (3333 clocks @ 3.3 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm;         // CAPT interrupt enable
    TCB0.CTRLA = TCB_ENABLE_bm;         // Enable TCB0

    // configure TCB1 for a periodic interrupt every 5ms
    TCB1.CTRLB = TCB_CNTMODE_INT_gc;    // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 16667;                  // Set interval for 5ms (16667 clocks @ 3.3 MHz)
    TCB1.INTCTRL = TCB_CAPT_bm;         // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm;         // Enable TCB1

}//timer_init


void adc_init(){
    ADC0.CTRLA = ADC_ENABLE_bm;

    ADC0.CTRLB = ADC_PRESC_DIV2_gc;

    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;

    ADC0.CTRLE = 64;

    ADC0.CTRLF = ADC_FREERUN_bm;

    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;

    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;
}

void uart_init(void)
{
    // 9600 baud
    USART0.BAUD = 1389;

    // Enable receive complete interrupt
    USART0.CTRLA = USART_RXCIE_bm;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
}
