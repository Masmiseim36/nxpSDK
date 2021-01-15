/**************************************************************************
MODULE:    Remote Access Handler
CONTAINS:  Implementation of serial communication with a host interface
           as specified by Serial CANopen Slave Control Interface
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

#ifndef _REMOTE_ACCESS_H
#define _REMOTE_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
DOES:    Initializes the serial interface to the host,
         also flushes all transmit/receive buffers
RETURNS: nothing
**************************************************************************/
void MCORA_Init(void);

/**************************************************************************
DOES:    Main Processing for Host Interface interaction
RETURNS: TRUE for command received, otherwise FALSE
**************************************************************************/
uint8_t MCORA_ProcessHost(void);

/**************************************************************************
DOES:    Sends a write local response to the host
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
uint8_t MCORA_SendWriteLocalResponse(uint16_t index,   // od index
                                     uint8_t subindex, // od subindex
                                     uint16_t err      // error code or zero for no error
);

/**************************************************************************
DOES:    Sends a write remote response to the host
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
uint8_t MCORA_SendWriteRemoteResponse(uint8_t sdo,      // sdo channel number
                                      uint16_t index,   // od index
                                      uint8_t subindex, // od subindex
                                      uint16_t err      // error code or zero for no error
);

/**************************************************************************
DOES:    Sends a read local response to the host
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
uint8_t MCORA_SendReadLocalResponse(uint16_t index,   // od index
                                    uint8_t subindex, // od subindex
                                    uint16_t err,     // error code or zero for no error
                                    uint16_t length,  // length of data from od entry
                                    uint8_t *pdata    // data from od entry
);

/**************************************************************************
DOES:    Sends a read remote response to the host
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
uint8_t MCORA_SendReadRemoteResponse(uint8_t sdo,      // sdo channel number
                                     uint16_t index,   // od index
                                     uint8_t subindex, // od subindex
                                     uint16_t err,     // error code or zero for no error
                                     uint16_t length,  // length of data from od entry
                                     uint8_t *pdata    // data from od entry
);

/**************************************************************************
DOES:    Sends a new process data local write notification to the host
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
uint8_t MCORA_SendLocalProcessDataWrite(uint8_t nodeid,   // node id
                                        uint16_t index,   // od index
                                        uint8_t subindex, // od subindex
                                        uint16_t length,  // length of data written to od entry
                                        uint8_t *pdata    // data written to od entry
);

/**************************************************************************
DOES:    Sends a new process data local read notification to the host
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
uint8_t MCORA_SendLocalProcessDataRead(uint8_t nodeid,  // node id
                                       uint16_t index,  // od index
                                       uint8_t subindex // od subindex
);

/**************************************************************************
DOES:    Command handler for 'G' response to a SDO segment previously
         received
RETURNS: nothing
**************************************************************************/
void MCORA_Response_ServerSDO(uint8_t length, // length of data
                              uint8_t *pData  // node id and 8 SDO bytes
);

/**************************************************************************
DOES:    Sends a SDO segment to host (request received for local server)
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
uint8_t MCORA_SendXSDOServer(uint8_t nodeid, // node id
                             uint8_t *pdata  // data written to od entry
);

/**************************************************************************
DOES:    Sends a SDO segment to host (response received for local client)
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
uint8_t MCORA_SendXSDOClient(uint8_t nodeid, // node id
                             uint8_t *pdata  // data written to od entry
);

/**************************************************************************
DOES:    Call Back to Remote Access module, when SDO transfer completed
RETURNS: Nothing
**************************************************************************/
void MCORACB_SDOComplete(uint8_t channel,    // SDO channel number in range of 1 to NR_OF_SDO_CLIENTS
                         uint32_t abort_code // status, error, abort code
);

/**************************************************************************
DOES:    Checks if a requested Remote Read access was already scanned and
         is available locally. If it is, get the value
RETURNS: Number of bytes copied to buffer
**************************************************************************/
uint16_t MGRRA_ReadRemoteCache(uint8_t nodeid, uint16_t index, uint8_t subindex, uint8_t *pDest);

#ifdef __cplusplus
}
#endif

#endif // _REMOTE_ACCESS_H
/**************************************************************************
END OF FILE
**************************************************************************/
