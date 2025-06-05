#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

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

// -------------------------  SERIAL PARSER  -------------------------

typedef enum
{
    AWAITING_COMMAND,
    AWAITING_PAYLOAD
} Serial_State;

volatile uint8_t uart_play = 0;
volatile uint8_t uart_stop = 0;

static uint8_t hexchar_to_int(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0'; 
    else if ('a' <= c && c <= 'f')
        return 10 + c - 'a';
    else
        return 16; // Invalid
}

ISR(USART0_RXC_vect)
{
    static Serial_State SERIAL_STATE = AWAITING_COMMAND;

    // Payload parsing
    static uint8_t chars_received = 0;
    static uint16_t payload = 0;
    static uint8_t payload_valid = 1;

    char rx_data = USART0.RXDATAL;
    while (!(USART0.STATUS & USART_DREIF_bm));  // Wait for transmit ready
    USART0.TXDATAL = rx_data;

    switch (SERIAL_STATE)
    {
    case AWAITING_COMMAND:
        if (rx_data == '1' || rx_data == '2')
            update_tone(hexchar_to_int(rx_data) - 1);
        else if (rx_data == 'p')
        {
            // Only if stopped
            if (is_playing == 0)
                uart_play = 1;
        }
        else if (rx_data == 's')
        {
            // Only if playing
            if (is_playing == 1)
                uart_stop = 1;
        }
        else if (rx_data == ',')
            decrease_octave();
        else if (rx_data == '.')
            increase_octave();
        else if (rx_data == 'd')
        {
            // Reset flags
            payload_valid = 1;
            chars_received = 0;
            payload = 0;

            // State transition
            SERIAL_STATE = AWAITING_PAYLOAD;
        }
        break;
    case AWAITING_PAYLOAD:
    {
        // Parse payload
        uint8_t parsed_result = hexchar_to_int(rx_data);

        // Valid
        if (parsed_result != 16)
            payload = (payload << 4) | parsed_result;
        // Invalid
        else
            payload_valid = 0;

        // Check if payload is complete
        if (++chars_received == 4)
        {
            // Update playback delay if valid
            if (payload_valid)
                new_playback_delay = payload;

            // State transition
            SERIAL_STATE = AWAITING_COMMAND;
        }
        break;
    }
    default:
        SERIAL_STATE = AWAITING_COMMAND;
        break;
    }
}
