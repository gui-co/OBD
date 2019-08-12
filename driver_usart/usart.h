/******************************************************************************
 * USART driver for Arduino min pro (based on ATmega328).                     *
 * usart.h                                                                    *
 *                                                                            *
 * Guillaume Communie - guillaume.communie@gmail.com                          *
 ******************************************************************************/

#ifndef USART_H
#define USART_H

#include <stdint.h>

//! Init the USART driver with a specific baudrate.
//! @param[in] baudRate Baudrate of USART data transmission.
void usart_init(unsigned int baudRate);

//! Write 1 byte on USART.
//! @param[in] data Byte to write on the USART.
void usart_write(uint8_t data);

//! Read data from the USART.
//! @return Read data.
uint8_t usart_read(void);

#endif /* USART_H */

