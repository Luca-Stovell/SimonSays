#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "display.h"
#include "uart.h"
#include "buzzer.h"

static int stdio_putchar(char c, FILE *stream);
static int stdio_getchar(FILE *stream);

static FILE stdio = FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);
    
uint8_t uart_getc(void)
{
     while (!(USART0.STATUS & USART_RXCIF_bm));   //wait for receive complete
     return USART0.RXDATAL;
}

void uart_putc(uint8_t c)
{
    while (!(USART0.STATUS & USART_DREIF_bm)); // wait for data register empty
    USART0.TXDATAL = c;
}

static int stdio_putchar(char c, FILE *stream)
{
    uart_putc(c);
    return c; // the putchar function must return the character written to the stream
}

static int stdio_getchar(FILE *stream)
{
    return uart_getc();
}

void uart_init()
{
    PORTB.DIRSET = PIN2_bm; // enable PB2 as an output (UART TX)

    USART0.BAUD = 1389;                           // 9600 BAUD @ 3.33MHz
    
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // enable TX/RX

    USART0.CTRLA = USART_RXCIE_bm; // Enable receive complete interrupt

    stdout = &stdio;
    stdin = &stdio;
}//uart_init

// -------------------------  SERIAL PARSER  -------------------------

ISR(USART0_RXC_vect)
{
    char rx_data = (char)USART0.RXDATAL;

    // Echo back immediately
    while (!(USART0.STATUS & USART_DREIF_bm)) { }
    USART0.TXDATAL = (uint8_t)rx_data;

    // Handle each key in one‐byte fashion:
    if (rx_data == '1' || rx_data == 'q') {
        display_pattern_1();
        play_tone(0);
        current_button = 1;
        elapsed_time   = 0;
        pb_released    = 1;
        current_state  = HANDLE_INPUT;
    }
    else if (rx_data == '2' || rx_data == 'w') {
        display_pattern_2();
        play_tone(1);
        current_button = 2;
        elapsed_time   = 0;
        pb_released    = 1;
        current_state  = HANDLE_INPUT;
    }
    else if (rx_data == '3' || rx_data == 'e') {
        display_pattern_3();
        play_tone(2);
        current_button = 3;
        elapsed_time   = 0;
        pb_released    = 1;
        current_state  = HANDLE_INPUT;
    }
    else if (rx_data == '4' || rx_data == 'r') {
        display_pattern_4();
        play_tone(3);
        current_button = 4;
        elapsed_time   = 0;
        pb_released    = 1;
        current_state  = HANDLE_INPUT;
    }
    else if (rx_data == ',' || rx_data == 'k') {
        increase_octave();
    }
    else if (rx_data == '.' || rx_data == 'l') {
        decrease_octave();
    }
    else if (rx_data == '0' || rx_data == 'p') {
        // perform reset logic in main
    }
    else if (rx_data == '9' || rx_data == 'o') {
        // perform seed‐load logic in main
    }
}