/******************************************************************************
 * MCP2515 driver for Arduino mini pro (based on ATmega328).                  *
 * mcp2515_defines.c                                                          *
 *                                                                            *
 * Guillaume Communie - guillaume.communie@gmail.com                          *
 ******************************************************************************/


/******************************************************************************
 * Registers                                                                  *
 ******************************************************************************/

/* 0Xh registers and associated bit numbers */
#define RXF0SIDH  0b00000000
#define RXF0SIDL  0b00000001
#define EXIDE     3
#define RXF0EID8  0b00000010
#define RXF0EID0  0b00000011
#define RXF1SIDH  0b00000100
#define RXF1SIDL  0b00000101
#define RXF1EID8  0b00000110
#define RXF1EID0  0b00000111
#define RXF2SIDH  0b00001000
#define RXF2SIDL  0b00001001
#define RXF2EID8  0b00001010
#define RXF2EID0  0b00001011
#define BFPCTRL   0b00001100
#define B1BFS     5
#define B0BFS     4
#define B1BFE     3
#define B0BFE     2
#define B1BFM     1
#define B0BFM     0
#define TXRTSCTRL 0b00001101
#define B2RTS     5
#define B1RTS     4
#define B0RTS     3
#define B2RTSM    2
#define B1RTSM    1
#define B0RTSM    0
#define CANSTAT   0b00001110
#define OPMOD2    7
#define OPMOD1    6
#define OPMOD0    5
#define ICOD2     3
#define ICOD1     2
#define ICOD0     1
#define CANCTRL   0b00001111
#define REQOP2    7
#define REQOP1    6
#define REQOP0    5
#define ABAT      4
#define OSM       3
#define CLKEN     2
#define CLKPRE1   1
#define CLKPRE0   0

/* 1Xh registers and associated bit numbers */
#define RXF3SIDH  0b00010000
#define RXF3SIDL  0b00010001
#define RXF3EID8  0b00010010
#define RXF3EID0  0b00010011
#define RXF4SIDH  0b00010100
#define RXF4SIDL  0b00010101
#define RXF4EID8  0b00010110
#define RXF4EID0  0b00010111
#define RXF5SIDH  0b00011000
#define RXF5SIDL  0b00011001
#define RXF5EID8  0b00011010
#define RXF5EID0  0b00011011
#define TEC       0b00011100
#define REC       0b00011101

/* 2Xh registers and associated bit numbers */
#define RXM0SIDH  0b00100000
#define RXM0SIDL  0b00100001
#define RXM0EID8  0b00100010
#define RXM0EID0  0b00100011
#define RXM1SIDH  0b00100100
#define RXM1SIDL  0b00100101
#define RXM1EID8  0b00100110
#define RXM1EID0  0b00100111
#define CNF3      0b00101000
#define SOF       7
#define WAKFIL    6
#define PHSEG22   2
#define PHSEG21   1
#define PHSEG20   0
#define CNF2      0b00101001
#define BTLMODE   7
#define SAM       6
#define PHSEG12   5
#define PHSEG11   4
#define PHSEG10   3
#define PRSEG2    2
#define PRSEG1    1
#define PRSEG0    0
#define CNF1      0b00101010
#define SJW1      7
#define SJW0      6
#define BRP5      5
#define BRP4      4
#define BRP3      3
#define BRP2      2
#define BRP1      1
#define BRP0      0
#define CANINTE   0b00101011
#define MERRE     7
#define WAKIE     6
#define ERRIE     5
#define TX2IE     4
#define TX1IE     3
#define TX0IE     2
#define RX1IE     1
#define RX0IE     0
#define CANINTF   0b00101100
#define MERRF     7
#define WAKIF     6
#define ERRIF     5
#define TX2IF     4
#define TX1IF     3
#define TX0IF     2
#define RX1IF     1
#define RX0IF     0
#define EFLG      0b00101101
#define RX1OVR    7
#define RX0OVR    6
#define TXBO      5
#define TXEP      4
#define RXEP      3
#define TXWAR     2
#define RXWAR     1
#define EWARN     0

/* 3Xh registers and associated bit numbers */
#define TXB0CTRL  0b00110000
#define ABTF      6
#define MLOA      5
#define TXERR     4
#define TXREQ     3
#define TXP1      1
#define TXP0      0
#define TXB0SIDH  0b00110001
#define TXB0SIDL  0b00110010
#define TXB0EID8  0b00110011
#define TXB0EID0  0b00110100
#define TXB0DLC   0b00110101
#define RTR       6
#define TXB0D0    0b00110110
#define TXB0D1    0b00110111
#define TXB0D2    0b00111000
#define TXB0D3    0b00111001
#define TXB0D4    0b00111010
#define TXB0D5    0b00111011
#define TXB0D6    0b00111100
#define TXB0D7    0b00111101

/* 4Xh registers and associated bit numbers */
#define TXB1CTRL  0b01000000
#define TXB1SIDH  0b01000001
#define TXB1SIDL  0b01000010
#define TXB1EID8  0b01000011
#define TXB1EID0  0b01000100
#define TXB1DLC   0b01000101
#define TXB1D0    0b01000110
#define TXB1D1    0b01000111
#define TXB1D2    0b01001000
#define TXB1D3    0b01001001
#define TXB1D4    0b01001010
#define TXB1D5    0b01001011
#define TXB1D6    0b01001100
#define TXB1D7    0b01001101

/* 5Xh registers and associated bit numbers */
#define TXB2CTRL  0b01010000
#define TXB2SIDH  0b01010001
#define TXB2SIDL  0b01010010
#define TXB2EID8  0b01010011
#define TXB2EID0  0b01010100
#define TXB2DLC   0b01010101
#define TXB2D0    0b01010110
#define TXB2D1    0b01010111
#define TXB2D2    0b01011000
#define TXB2D3    0b01011001
#define TXB2D4    0b01011010
#define TXB2D5    0b01011011
#define TXB2D6    0b01011100
#define TXB2D7    0b01011101

/* 6Xh registers and associated bit numbers */
#define RXB0CTRL  0b01100000
#define RXM1      6
#define RXM0      5
#define RXRTR     3
#define BUKT      2
#define BUKT1     1
#define FILHIT0   0
#define RXB0SIDH  0b01100001
#define RXB0SIDL  0b01100010
#define SRR       4
#define IDE       3
#define RXB0EID8  0b01100011
#define RXB0EID0  0b01100100
#define RXB0DLC   0b01100101
#define RXB0D0    0b01100110
#define RXB0D1    0b01100111
#define RXB0D2    0b01101000
#define RXB0D3    0b01101001
#define RXB0D4    0b01101010
#define RXB0D5    0b01101011
#define RXB0D6    0b01101100
#define RXB0D7    0b01101101

/* 7Xh registers and associated bit numbers */
#define RXB1CTRL  0b01110000
#define FILHIT2   2
#define FILHIT1   1
#define RXB1SIDH  0b01110001
#define RXB1SIDL  0b01110010
#define RXB1EID8  0b01110011
#define RXB1EID0  0b01110100
#define RXB1DLC   0b01110101
#define RXB1D0    0b01110110
#define RXB1D1    0b01110111
#define RXB1D2    0b01111000
#define RXB1D3    0b01111001
#define RXB1D4    0b01111010
#define RXB1D5    0b01111011
#define RXB1D6    0b01111100
#define RXB1D7    0b01111101


/******************************************************************************
 * SPI instructions                                                           *
 ******************************************************************************/

/* Basic instructions */
#define RESET_INSTR        0b11000000
#define READ_INSTR         0b00000011
#define WRITE_INSTR        0b00000010
#define BIT_MODIFY_INSTR   0b00000101

/* Read status instruction*/
#define READ_STATUS_INSTR  0b10100000
#define RX0IF_STATUS       0
#define RX1IF_STATUS       1
#define TXREQ_BUF0_STATUS  2
#define TX0IF_STATUS       3
#define TXREQ_BUF1_STATUS  4
#define TX1IF_STATUS       5
#define TXREQ_BUF2_STATUS  6
#define TX2IF_STATUS       7

/* Read reception buffers instructions */
#define READ_RX_BUFFER_0_ID_INSTR    0b10010000
#define READ_RX_BUFFER_0_DATA_INSTR  0b10010010
#define READ_RX_BUFFER_1_ID_INSTR    0b10010100
#define READ_RX_BUFFER_1_DATA_INSTR  0b10010110

/* Load transmission buffers instructions */
#define LOAD_TX_BUFFER_0_ID_INSTR    0b01000000
#define LOAD_TX_BUFFER_0_DATA_INSTR  0b01000001
#define LOAD_TX_BUFFER_1_ID_INSTR    0b01000010
#define LOAD_TX_BUFFER_1_DATA_INSTR  0b01000011
#define LOAD_TX_BUFFER_2_ID_INSTR    0b01000100
#define LOAD_TX_BUFFER_2_DATA_INSTR  0b01000101

/* Request to send instructions */
#define RTS_BUFFER_0_INSTR           0b10000001
#define RTS_BUFFER_1_INSTR           0b10000010
#define RTS_BUFFER_2_INSTR           0b10000100

