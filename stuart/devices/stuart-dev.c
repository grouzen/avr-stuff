#define F_CPU 1000000UL
#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <util/delay.h>

#include "atmega8.h"

int main(void)
{
    uint8_t buf[256], byte;

    DDRB = _BV(DDB1) | _BV(DDB2);
        
    usart_init(STUART_UBRR);

    for(;;) {
        volatile int i, j;

        PORTB = _BV(PB1);
        
        for(i = 0; (byte = usart_recieve_byte()) != '\0'; i++) {            
            buf[i] = byte;
            PORTB = ~PORTB;
        }
        buf[i++] = '1';
        buf[i++] = '2';
        buf[i++] = '3';
        buf[i++] = '\0';

        for(j = 0; j < i; j++) {
            usart_send_byte(buf[j]);
            PORTB = ~PORTB;
        }

        PORTB = 0;
        _delay_ms(1000);
    }

    return 0;
}
