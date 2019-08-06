/******************************************************************************
 * MCP2515 driver for Arduino mini pro (based on ATmega328).                  *
 * mcp2515.h                                                                  *
 *                                                                            *
 * Guillaume Communie - guillaume.communie@gmail.com                          *
 ******************************************************************************/

#ifndef MCP2515_H
#define MCP2515_H

#define CAN_DATA_MAX_LENGTH 8

#include <stdint.h>

//! Initialize the MCP2515 driver.
void mcp2515_init(void);

//! Send a CAN frame.
//! @param[in] isExtended True if the CAN frame is an extended frame.
//! @param[in] isRtr      True is the frame is a Request to Response frame.
//! @param[in] id         Frame id.
//! @apram[in] data       Frame data.
//! @param[in] length     Data length.
//! @param[in] priority   Sending priority (0-3)
//! return                Buffer used for tranmission, -1 if no buffer available.
int mcp2515_sendFrame(uint8_t isExtended, uint8_t isRtr, uint32_t id,
                      uint8_t *data, uint8_t length, uint8_t priority);

//! Read a CAN frame from one of the two RX buffers.
//! @param[out] id       Read frame id.
//! @param[out] data     Read frame data.
//! @param[in]  rxBuffer The RX buffer to read (0 or 1).
//! @return              The number of data byte readn -1 if an error occured.
int mcp2515_readFrame(uint32_t *id, uint8_t data[CAN_DATA_MAX_LENGTH],
                      uint8_t rxBuffer);

#endif /* MCP2515_H */

