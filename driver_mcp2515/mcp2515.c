/******************************************************************************
 * MCP2515 driver for Arduino mini pro (based on ATmega328).                  *
 * mcp2515.c                                                                  *
 *                                                                            *
 * Guillaume Communie - guillaume.communie@gmail.com                          *
 ******************************************************************************/

#include "spi.h"
#include "usart.h"

#include "mcp2515.h"
#include "mcp2515_defines.h"

//! Read a register and return the value.
//! @param[in] add Register address.
//! @return        Register value.
static uint8_t readRegister(uint8_t add) {
    spi_slaveActivate();
    spi_write(READ_INSTR);
    spi_write(add);
    uint8_t reg = spi_read();
    spi_slaveInactivate();

    return reg;
}

//! Write a complete register.
//! @param[in] add Register address.
//! @param[in] val Value to write.
static void writeRegister(uint8_t add, uint8_t val) {
    spi_slaveActivate();
    spi_write(WRITE_INSTR);
    spi_write(add);
    spi_write(val);
    spi_slaveInactivate();
}

//! Write any bits of a register.
//! @param[in] add  Register address.
//! @param[in] mask Bitmask to specify which bit(s) to write.
//! @param[in] bits Value to write.
static void writeRegisterBits(uint8_t add, uint8_t mask, uint8_t bits) {
    spi_slaveActivate();
    spi_write(BIT_MODIFY_INSTR);
    spi_write(add);
    spi_write(mask);
    spi_write(bits);
    spi_slaveInactivate();
}

//! Read the mcp2515 status.
//! @return Status: 7 6 5 4 3 2 1 0
//!                 | | | | | | | |
//!                 | | | | | | | RX0IF (CANINTF[0])
//!                 | | | | | | RX1IF (CANINTF[1])
//!                 | | | | | TXREQ (TXB0CNTRL[3])
//!                 | | | | TX0IF (CANINTF[2])
//!                 | | | TXREQ (TXB1CNTRL[3])
//!                 | | TX1IF (CANINTF[3])
//!                 | TXREQ (TXB2CNTRL[3])
//!                 TX2IF (CANINTF[4])
static uint8_t readStatus(void) {
    spi_slaveActivate();
    spi_write(READ_STATUS_INSTR);
    uint8_t status = spi_read();
    spi_slaveInactivate();

    return status;
}

void mcp2515_init(void) {
    spi_init(SPI_MODE_3);
    spi_slaveActivate();
    spi_write(RESET_INSTR);
    spi_slaveInactivate();

    // Interrupt
    writeRegisterBits(CANINTE, ((1<<MERRE) | (1<<RX0IE) | (1<<RX1IE)),
                               ((0<<MERRE) | (1<<RX0IE) | (1<<RX1IE)));
    writeRegister(CANINTF, 0x00);

    // No filters on RXB0 and RXB1
    writeRegisterBits(RXB0CTRL, (1<<RXM1) | (1<<RXM0), (1<<RXM1) | (1<<RXM0));
    writeRegisterBits(RXB1CTRL, (1<<RXM1) | (1<<RXM0), (1<<RXM1) | (1<<RXM0));

    // Loopback mode
    writeRegisterBits(CANCTRL, ((1<<REQOP2) | (1<<REQOP1) | (1<<REQOP0)),
                               ((0<<REQOP2) | (1<<REQOP1) | (0<<REQOP0)));
    // Normal mode
    //writeRegMask(CANCTRL, (1<<REQOP2) | (1<<REQOP1) | (1<<REQOP0), 0x00);
}

int mcp2515_sendFrame(uint8_t isExtended, uint8_t isRtr, uint32_t id,
                      uint8_t *data, uint8_t length, uint8_t priority) {
    // Identify a free buffer
    uint8_t buffer;
    uint8_t ctrl;
    uint8_t loadInstr;
    uint8_t status = readStatus();
    if (!(status & (1<<TXREQ_BUF0_STATUS))) {
        buffer = 0;
        ctrl = TXB0CTRL;
        loadInstr = LOAD_TX_BUFFER_0_ID_INSTR;
    } else if (!(status & (1<<TXREQ_BUF1_STATUS))) {
        buffer = 1;
        ctrl = TXB1CTRL;
        loadInstr = LOAD_TX_BUFFER_1_ID_INSTR;
    } else if (!(status & (1<<TXREQ_BUF2_STATUS))) {
        buffer = 2;
        ctrl = TXB2CTRL;
        loadInstr = LOAD_TX_BUFFER_2_ID_INSTR;
    } else {
        return -1;
    }

    // Frame ID
    writeRegisterBits(ctrl, ((1<<TXP1) | (1<<TXP0)),
                            ((1<<TXP1) | (1<<TXP0)));
    spi_slaveActivate();
    spi_write(loadInstr);
    uint8_t sIdH = 0;
    uint8_t sIdL = 0;
    uint8_t eId8 = 0;
    uint8_t eId0 = 0;
    if (isExtended) {
        id = id & 0x1fffffff;  // 29 bits ID
        sIdH = id >> 21;
        sIdL = ((id & 0x001c0000) >> 13) | (1<<EXIDE) | ((id & 0x00030000) >> 16);
        eId8 = (id & 0x0000FF00 >> 8);
        eId0 = id & 0x000000FF;
    } else {
        id = id & 0x000007FF;  // 11 bits ID
        sIdH = id >> 3;
        sIdL = (id & 0x00000007) << 5;
    }
    spi_write(sIdH);
    spi_write(sIdL);
    spi_write(eId8);
    spi_write(eId0);

    // Frame Data
    if (isRtr)
        spi_write(1<<RTR);
    else
        spi_write(length);
    for (int i = 0 ; i < CAN_DATA_MAX_LENGTH ; i++)
        spi_write(data[i]);
    spi_slaveInactivate();

    // Sending
    switch (priority) {
        case 0:
            writeRegisterBits(ctrl, ((1<<TXREQ) | (1<<TXP1) | (1<<TXP0)),
                                    ((1<<TXREQ) | (0<<TXP1) | (0<<TXP0)));
            break;
        case 1:
            writeRegisterBits(ctrl, ((1<<TXREQ) | (1<<TXP1) | (1<<TXP0)),
                                    ((1<<TXREQ) | (0<<TXP1) | (1<<TXP0)));
            break;
        case 2:
            writeRegisterBits(ctrl, ((1<<TXREQ) | (1<<TXP1) | (1<<TXP0)),
                                    ((1<<TXREQ) | (1<<TXP1) | (0<<TXP0)));
            break;
        case 3:
        default:
            writeRegisterBits(ctrl, ((1<<TXREQ) | (1<<TXP1) | (1<<TXP0)),
                                    ((1<<TXREQ) | (1<<TXP1) | (1<<TXP0)));
            break;
    }

    return buffer;
}

int mcp2515_readFrame(uint32_t *id, uint8_t data[CAN_DATA_MAX_LENGTH],
                      uint8_t rxBuffer) {
    spi_slaveActivate();
    if (rxBuffer == 0)
        spi_write(READ_RX_BUFFER_0_ID_INSTR);
    else if (rxBuffer == 1)
        spi_write(READ_RX_BUFFER_1_ID_INSTR);
    else {
        spi_slaveInactivate();
        return -1;
    }

    // ID
    uint32_t sIdH;
    uint32_t sIdL;
    uint32_t eId8;
    uint32_t eId0;
    sIdH = spi_read();
    sIdL = spi_read();
    eId8 = spi_read();
    eId0 = spi_read();
    if (sIdL & (1<<IDE))
        *id = (sIdH << 21) | ((sIdL & 0xE0) << 18) | ((sIdL & 0x03) << 16)
            | (eId8 << 8) | eId0;
    else
        *id = (sIdH << 3) | ((sIdL & 0xE0) >> 5);

    // Data
    uint8_t length = spi_read() & 0x0F;
    int i = 0;
    while (i < length) {
        data[i] = spi_read();
        i++;
    }
    while (i < CAN_DATA_MAX_LENGTH) {
        data[i] = 0;
        i++;
    }
    spi_slaveInactivate();

    return length;
}

