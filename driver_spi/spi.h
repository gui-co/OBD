/******************************************************************************
 * SPI driver for Arduino mini pro (based on ATmega328).                      *
 * spi.h                                                                      *
 *                                                                            *
 * Guillaume Communie - guillaume.communie@gmail.com                          *
 ******************************************************************************/

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>

//! SPI Mode.
enum spi_dataMode {
    SPI_MODE_0,  // Clock polarity: 0 - Clock phase: 0
    SPI_MODE_1,  // Clock polarity: 0 - Clock phase: 1
    SPI_MODE_2,  // Clock polarity: 1 - Clock phase: 0
    SPI_MODE_3   // Clock polarity: 1 - Clock phase: 1
};

//! Init the SPI driver.
//! @apram[in] mode SPI mode to use.
void spi_init(enum spi_dataMode mode);

//! Activate the slave by lowering the SS pin.
void spi_slaveActivate(void);

//! Inactivate the slave.
void spi_slaveInactivate(void);

//! Write data on the SPI bus.
//! @param[in] data Byte to write.
//! @return Data received during the write operation.
uint8_t spi_write(uint8_t data);

//! Read data on SPI bus.
//! @return Read data.
uint8_t spi_read(void);

#endif /* SPI_H */

