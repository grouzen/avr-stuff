#ifndef __ATMEGA8_H__
#define __ATMEGA8_H__

#define STUART_UBRR 12

void usart_init(uint16_t);
void usart_send_byte(uint8_t);
uint8_t usart_recieve_byte(void);

#endif /* __ATMEGA8_H__ */
