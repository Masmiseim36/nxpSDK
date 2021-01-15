/**************************************************************************
PROJECT:   Packet Based Serial Protocol Handler
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020.
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#ifndef _SERIALPROTOCOL_H
#define _SERIALPROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
 DEFINITIONS AND TYPES
***************************************************************************/

// maximum number of bytes of data in a packet
// here: 28 is 4 SDO segments of 7 data bytes, plus 7 for longest possible header
// typical threshhold before using block transfer
#define MAX_PACKET_DATA 28 + 7

// transmit and receive buffer sizes
// here: from UART, only a minimal number of back-2-back messages expected
// but 447izer can produce multiple back-2-back indications, worst case 8 single PDO mapped bytes
#define SER_RX_BUFFER_SIZE 32
#define SER_TX_BUFFER_SIZE 128

// Maximum number of SerialProcess calls per cycle
#if (MGR_MONITOR_ALL_NODES == 1)
#define MAX_NR_OF_SERIAL_PER_CYCLE 32
#else
#define MAX_NR_OF_SERIAL_PER_CYCLE 16
#endif

// Definitions for protocol codes
#define PROT_SOH 0x11 // Start-of-header byte, here: network 1, use CRC

// error flags
#define SERIALPROTOCOL_ERROR_UART 0x01UL

/***************************************************************************
 EXPORTED GLOBALS
***************************************************************************/

/***************************************************************************
 EXPORTED FUNCTIONS
***************************************************************************/

/**************************************************************************
DOES:    Initialization of protocol handler
RETURNS: -
**************************************************************************/
extern void SerialProtocol_Init(void);

/**************************************************************************
DOES:    Checks if any communication errors occured and returns the
         error status. Only TRUE/FALSE value for the global UART error
         status. This could be made more specific if needed at the host.
   Clears error flags and counters.
RETURNS: Current error flags (SERIALPROTOCOL_ERROR_xxx)
GLOBALS: SerialProtocol_ReceiveErrorCounter,
         SerialProtocol_TransmitErrorCounter
**************************************************************************/
extern uint16_t SerialProtocol_CheckError(void);

/**************************************************************************
DOES:    Transmits all packets in transmit buffer
RETURNS: Nothing
**************************************************************************/
void SerialProtocol_CompleteTransmits(void);

/**************************************************************************
DOES:    Executes state machines - must be called periodically
RETURNS: Nothing
**************************************************************************/
extern void SerialProtocol_Process(void);

/**************************************************************************
DOES:    Queues a message for transmission
RETURNS: TRUE if message was queued, FALSE if there was an error
**************************************************************************/
extern uint8_t SerialProtocol_PushMessage(uint8_t length1, // length of data chunk 1
                                          uint8_t *pTx1,   // pointer to data chunk 1
                                          uint8_t length2, // length of data chunk 2
                                          uint8_t *pTx2    // pointer to data chunk 2
);

/**************************************************************************
DOES:    Gets the next message from the receive buffer
RETURNS: Number of data bytes copied or zero if no messages
**************************************************************************/
extern uint8_t SerialProtocol_PullMessage(uint8_t *pRx // pointer to location to store data
);

#ifdef __cplusplus
}
#endif

#endif // _SERIALPROTOCOL_H
/**************************************************************************
END OF FILE
**************************************************************************/
