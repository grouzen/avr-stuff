#include <inttypes.h>
#include <avr/io.h>

void usart_init(uint16_t ubrr)
{
    UBRRL = ubrr;
    UBRRH = (ubrr >> 8);

    /* Use UCSRC instead of UBRRH. Set format to 8N1. */
    UCSRC = _BV(URSEL) | _BV(UCSZ0) | _BV(UCSZ1);

    UCSRB = _BV(RXEN) | _BV(TXEN);

    return;
}

void usart_send_byte(uint8_t byte)
{
    while(!(UCSRA & (1 << UDRE)));
    UDR = byte;

    return;
}

uint8_t usart_recieve_byte(void)
{
    while(!(UCSRA & (1 << RXC)));

    return UDR;
}
