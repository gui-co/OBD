/******************************************************************************
 * USART driver for Arduino min pro (based on ATmega328).                     *
 * usart.c                                                                    *
 *                                                                            *
 * Guillaume Communie - guillaume.communie@gmail.com                          *
 ******************************************************************************/

#include <avr/io.h>
#include <stdint.h>

#include "usart.h"

void usart_init(unsigned int baudrate) {
    unsigned int ubrr = F_CPU / 16 / baudrate - 1;
    UBRR0H = (uint8_t) (ubrr>>8);
    UBRR0L = (uint8_t) ubrr;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) |  (1<<RXCIE0);
    UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

void usart_write(uint8_t data) {
    while(!((UCSR0A) & (1<<UDRE0)))
        ;
    UDR0 = data;
}

void usart_writeString(char *data) {
    while (*data)
        usart_write(*data++);
    usart_write('\r');
    usart_write('\n');
}

uint8_t usart_read(void) {
    while(!((UCSR0A) & (1<<UDRE0)))
        ;
    return UDR0;
}

