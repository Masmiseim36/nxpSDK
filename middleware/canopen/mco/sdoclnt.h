/**************************************************************************
MODULE:    SDOCLNT
CONTAINS:  MicroCANopen Plus implementation, CANopen SDO Clients
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS MANAGER
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#ifndef _SDOCLNT_H
#define _SDOCLNT_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
AUTOMATIC INCLUDES
**************************************************************************/

#include "mco.h"

/**************************************************************************
SELECT DEFAULTS FOR DEFINES, IF NOT YET SET
**************************************************************************/
#ifndef NR_OF_SDO_CLIENTS
// Number of SDO channels implemented
#define NR_OF_SDO_CLIENTS 2
#endif
#ifndef USE_BLOCKED_SDO_CLIENT
// SDO block mode is used, if set to 1
#define USE_BLOCKED_SDO_CLIENT 0
#endif

/**************************************************************************
Further "#define" controls used in comgr.c

__SIMULATION__
Must be defined, if code is compiled for CANopen Magic Ultimate simulation

CHECK_PARAMETERS
Additional parameter check, if parameters passed are in legal range

MGR_MONITOR_ALL_NODES
If set, Manager is enabled maintaining a node list

RTOS_SLEEP
This is a macro to be used in RTOS evironments. Called in blocking calls
while waiting, typically set to a delay of a few system ticks
**************************************************************************/

#if (NR_OF_SDO_CLIENTS > 0)

/**************************************************************************
GLOBAL DEFINITIONS
**************************************************************************/

// SDO Client Status Flags (in SDOCLIENT)
#define SDOCL_FREE      0x00 // Channel unused
#define SDOCL_READY     0x01 // Channel initialized
#define SDOCL_WAIT_RES  0x02 // Waiting for expedited SDO response
#define SDOCL_NEXT_UPL  0x04 // Next SDO Upload Request queued
#define SDOCL_NEXT_DWN  0x08 // Next SDO Download Request due
#define SDOCL_LAST_SEG  0x10 // Last segment of a segmented transfer
#define SDOCL_CUSTOM    0x20 // Custom handling
#define SDOCL_SEGMENTED 0x80 // Segmented transfer allowed

// For block transfer
#define SDOCL_BLOCK_INITWR 0x20 // Block Write Init
#define SDOCL_BLOCK_WRITE  0x30 // Block Write in progress
#define SDOCL_BLOCK_WRCONF 0x40 // Block Write over
#define SDOCL_BLOCK_WRFINA 0x41 // Block Write over, final & call-back
#define SDOCL_BLOCK_INITRD 0x50 // Block Read Init
#define SDOCL_BLOCK_READ   0x60 // Block Read in progress
#define SDOCL_BLOCK_RDCONF 0x70 // Block Read over
#define SDOCL_BLOCK_RDFINA 0x71 // Block Read over, final & call-back

// SDO Response Status (in SDOCLNTCB_SDOComplete, SDOCLNT_IsTransferComplete)
#define SDOERR_FATAL   0x00 // Illegal pointer passed
#define SDOERR_OK      0x01 // Confirmation, last access was sucess
#define SDOERR_ABORT   0x80 // Abort received
#define SDOERR_TIMEOUT 0x81 // Request timed out
#define SDOERR_TOGGLE  0x82 // Toggle error
#define SDOERR_BUFSIZE 0x84 // Out of Memory
#define SDOERR_PARAM   0x85 // Wrong parameter
#define SDOERR_UNKNOWN 0x88 // No transfer possible
#define SDOERR_RUNNING 0xFF // SDO Transfer still running, not complete
#endif                      // NR_OF_SDO_CLIENTS > 0

// Scanning of nodes: states
#define SCAN_NONE      0x00 // node is not present
#define SCAN_DELAY     0x02 // started a delay
#define SCAN_RUN       0x80 // scanning in progress
#define SCAN_WAITREPLY 0x81 // waiting for reply
#define SCAN_ABORT     0xC0 // scan aborted
#define SCAN_DONE      0xFE // scan complete
#define SCAN_OVER      0xFF // scan is over

/**************************************************************************
GLOBAL TYPES AND STRUCTURES
**************************************************************************/

typedef struct
{
    CAN_MSG sdomsg;          // Message buffer for send and receive
    uint32_t canid_request;  // CAN ID used for SDO requests
    uint32_t canid_response; // CAN ID used for SDO responses
    uint32_t bufmax;         // Maximum length of buffer
    uint32_t buflen;         // Length of expected transfer
    uint32_t curlen;         // Current length of buffer
    uint32_t last_abort;     // last abort code if any
    MEM_FAR uint8_t *pBuf;   // Pointer to buffer for transfer
    uint16_t timeout;        // SDO Timeout current timestamp
    uint16_t timeout_reload; // SDO Timeout re-load value
    uint16_t b2btimeout;     // Back-to-Back Timestamp
    uint16_t index;          // Index of current request
    uint8_t subindex;        // Subindex of current request
#if USE_BLOCKED_SDO_CLIENT
    uint8_t blksize; // Size of block (in messages)
    uint8_t n;       // in last segment, number of unused bytes
#endif
    uint8_t toggle;  // Last toggle value used, or block counter
    uint8_t status;  // Channel status info
    uint8_t channel; // Channel number from 1 to NR_OF_SDO_CLIENTS
} SDOCLIENT;

#if NR_OF_SDO_CLIENTS > 0
// data records for each client
extern SDOCLIENT MEM_FAR gSDOClientList[NR_OF_SDO_CLIENTS];
#endif

/**************************************************************************
PUBLIC FUNCTIONS
**************************************************************************/

/**************************************************************************
DOES:    This function resets all SDO Client channels
RETURNS: nothing
**************************************************************************/
void SDOCLNT_ResetChannels(void);

/**************************************************************************
DOES:    (Re-)initializes an SDO client channel.
RETURNS: NULL-Pointer, if channel initialization failed.
         Pointer to SDOCLIENT structure used, if init success
**************************************************************************/
MEM_FAR SDOCLIENT *SDOCLNT_Init(uint8_t channel,         // SDO channel number in range of 1 to NR_OF_SDO_CLIENTS
                                uint32_t canid_request,  // CAN message ID used for the SDO request
                                uint32_t canid_response, // CAN message ID used for the SDO response
                                MEM_FAR uint8_t *p_buf,  // data buffer pointer for data exchanged
                                uint32_t buf_size        // max length of data buffer
);

/**************************************************************************
DOES:    Transmits an SDO Write (download) request.
RETURNS: TRUE, if request was queued
         FALSE, if transmit queue full
GLOBALS: Uses the last data buffer assigned from last call to SDOCLNT_Init,
         SDOCLNT_ReadXtd or SDOCLNT_WriteXtd
NOTE:    Non blocking transfer, use functions SDOCLNT_GetStatus,
         SDOCLNT_BlockUntilCompleted or SDOCLNTCB_SDOComplete to determine
         when transfer completed
**************************************************************************/
uint8_t SDOCLNT_Write(MEM_FAR SDOCLIENT *p_client, // Pointer to initialized SDO client structure
                      uint16_t index,              // Object Dictionary Index to write
                      uint8_t subindex             // Object Dictionary Subindex to write
);

/**************************************************************************
DOES:    Transmits an SDO Write (download) request with extended parameters
RETURNS: TRUE, if request was queued
         FALSE, if transmit queue full
NOTE:    Non blocking transfer, use functions SDOCLNT_GetStatus,
         SDOCLNT_BlockUntilCompleted or SDOCLNTCB_SDOComplete to determine
         when transfer completed
**************************************************************************/
uint8_t SDOCLNT_WriteXtd(MEM_FAR SDOCLIENT *p_client, // Pointer to initialized SDO client structure
                         uint16_t index,              // Object Dictionary Index to write
                         uint8_t subindex,            // Object Dictionary Subindex to write
                         MEM_FAR uint8_t *pSrc,       // Pointer to data source
                         uint32_t len,                // Length of data
                         uint16_t timeout             // Timeout for this transfer in milliseconds
);

/**************************************************************************
DOES:    Transmits an SDO Read (upload) request.
RETURNS: TRUE, if request was queued
         FALSE, if transmit queue full
GLOBALS: Uses the last data buffer assigned from last call to SDOCLNT_Init,
         SDOCLNT_ReadXtd or SDOCLNT_WriteXtd
NOTE:    Non blocking transfer, use functions SDOCLNT_GetStatus,
         SDOCLNT_BlockUntilCompleted or SDOCLNTCB_SDOComplete to determine
         when transfer completed
**************************************************************************/
uint8_t SDOCLNT_Read(MEM_FAR SDOCLIENT *p_client, // Pointer to initialized SDO client structure
                     uint16_t index,              // Object Dictionary Index to read
                     uint8_t subindex             // Object Dictionary Subindex to read
);

/**************************************************************************
DOES:    Transmits an SDO Read (upload) request with extended parameters
RETURNS: TRUE, if request was queued
         FALSE, if transmit queue full
NOTE:    Non blocking transfer, use functions SDOCLNT_GetStatus,
         SDOCLNT_BlockUntilCompleted or SDOCLNTCB_SDOComplete to determine
         when transfer completed
**************************************************************************/
uint8_t SDOCLNT_ReadXtd(MEM_FAR SDOCLIENT *p_client, // Pointer to initialized SDO client structure
                        uint16_t index,              // Object Dictionary Index to read
                        uint8_t subindex,            // Object Dictionary Subindex to read
                        MEM_FAR uint8_t *pDest,      // Pointer to data destination
                        uint32_t len,                // Maximum length of data destination
                        uint16_t timeout             // Timeout for this transfer in milliseconds
);

/**************************************************************************
DOES:    Checks the current status of the SDO client
RETURNS: SDOERR_xxx codes
**************************************************************************/
uint8_t SDOCLNT_GetStatus(MEM_FAR SDOCLIENT *p_client // Pointer to initialized SDO client structure
);

/**************************************************************************
DOES:    Checks if an SDO client is busy or not
RETURNS: TRUE if busy
**************************************************************************/
uint8_t SDOCLNT_IsBusy(MEM_FAR SDOCLIENT *p_client // Pointer to initialized SDO client structure
);

/**************************************************************************
DOES:    Returns the last abort code of the SDO client, if any
RETURNS: Last SDO client Abort code or 0
**************************************************************************/
uint32_t SDOCLNT_GetLastAbort(MEM_FAR SDOCLIENT *p_client // Pointer to initialized SDO client structure
);

/**************************************************************************
DOES:    Blocking call to wait until previous SDO transfer completed.
RETURNS: SDOERR_xxx status codes
**************************************************************************/
uint8_t SDOCLNT_BlockUntilCompleted(MEM_FAR SDOCLIENT *p_client // Pointer to initialized SDO client structure
);

/**************************************************************************
DOES:    Call-Back to application when an SDO client transfer is completed
RETURNS: nothing
**************************************************************************/
void SDOCLNTCB_SDOComplete(uint8_t channel,    // SDO channel number in range of 1 to NR_OF_SDO_CLIENTS
                           uint32_t abort_code // SDOERR_xxx codes or SDO abort code
);

/**************************************************************************
DOES:    Transmits an SDO read request and waits for the response
         or an abort.
RETURNS: 0 if access aborted, else length of data received
**************************************************************************/
uint32_t SDOCLNT_ReadCycle(uint8_t channel,        // SDO channel number in range of 1 to NR_OF_SDO_CLIENTS
                           uint8_t node_id,        // CANopen node ID
                           uint16_t index,         // Object Dictionary Index to read
                           uint8_t subindex,       // Object Dictionary Subindex to read
                           MEM_FAR uint8_t *p_buf, // data buffer pointer for data received
                           uint32_t buf_size,      // max length of data buffer
                           uint16_t timeout        // Timeout for this transfer in milliseconds, 0 for default
);

/**************************************************************************
DOES:    Transmits an SDO Write request and waits for the response
         or an abort.
RETURNS: 0 if access aborted, else length of data received
**************************************************************************/
uint8_t SDOCLNT_WriteCycle(uint8_t channel,        // SDO channel number in range of 1 to NR_OF_SDO_CLIENTS
                           uint8_t node_id,        // CANopen node ID
                           uint16_t index,         // Object Dictionary Index to read
                           uint8_t subindex,       // Object Dictionary Subindex to read
                           MEM_FAR uint8_t *p_buf, // data buffer pointer for data transmitted
                           uint32_t len,           // length of data transmitted
                           uint16_t timeout        // Timeout for this transfer in milliseconds, 0 for default
);

/**************************************************************************
DOES:    Aborts a transfer currently in progress.
RETURNS: nothing
**************************************************************************/
void SDOCLNT_AbortTransfer(MEM_FAR SDOCLIENT *p_client, // Pointer to initialized SDO client structure
                           uint32_t abort_code          // Abort code to transmit
);

/**************************************************************************
DOES:    Gets executed if a CAN message was received that is the response
         to a SDO request
RETURNS: Nothing
***************************************************************************/
void SDOCLNT_HandleSDOClientResponse(MEM_FAR SDOCLIENT *p_client, // Pointer to SDO client structure for this response
                                     MEM_FAR uint8_t *pDat        // Pointer to 8 bytes of SDO data received
);

/**************************************************************************
DOES: SDO Client Handler, needs to be executed frequently
RETURNS: TRUE, if a message was generated
****************************************************************************/
uint8_t SDOCLNT_SDOHandleClient(void);

#if SDOCLNTCB_APPSDO_WRITE
/*******************************************************************************
DOES:    Call Back function to allow allow custom SDO client writes/downloads of
         longer data than local buffer size.
RETURNS: 0x00 - No custom handling, or no data
         0x01 - Custom handling, data buffer filled
*******************************************************************************/
uint8_t SDOCLNTCB_SDOWriteInit(MEM_FAR SDOCLIENT *p_client, // Pointer to initialized SDO client structure
                               uint16_t idx,                // Index of OD entry
                               uint8_t subidx,              // Subindex of OD entry
                               uint32_t MEM_FAR *size, // Size of SDO client data buffer, RETURN: length of data filled
                               uint32_t MEM_FAR *totalsize, // RETURN: total size of data, only set if >*size
                               uint8_t MEM_FAR *pDat        // Pointer to data buffer to fill, RETURN: data up to *size
);

/*******************************************************************************
DOES:    Call Back function to allow allow custom SDO client writes/downloads of
         longer data than local buffer size. Provides next data block to write,
         or signals the last block.
RETURNS: 0x00 - No custom handling, or no data
         0x01 - Custom handling, data buffer filled
*******************************************************************************/
uint8_t SDOCLNTCB_SDOWriteComplete(
    MEM_FAR SDOCLIENT *p_client, // Pointer to initialized SDO client structure
    uint16_t idx,                // Index of OD entry
    uint8_t subidx,              // Subindex of OD entry
    uint32_t MEM_FAR *size,      // Size of SDO client data buffer, RETURN: length of data filled
    uint32_t more,               // Number of bytes still needed (of total transfer)
    uint8_t MEM_FAR *pDat        // Pointer to data buffer to fill, RETURN: data up to *size
);
#endif

/**************************************************************************
DOES:    Initiates multiple SDO read requests to be sent, results read
         go directly into an array of uint32_t. Limited to expedited
         transfers of up to 4 byte.
         Format of the ScanList is 4 bytes per entry
         0,1: Index (set to 0xFFFF to mark end of list)
         2: Subindex
         3: Length
         When the scan completed, MGRCB_NodeStatusChanged() is called
RETURNS: nothing
**************************************************************************/
void MGRSCAN_Init(uint8_t sdo_clnt,           // SDO client number from 1 to NR_OF_SDO_CLIENTS
                  uint8_t node_id,            // Node ID of node to read data from
                  MEM_FAR uint8_t *pScanList, // Pointer to list with OD entries to be read
                  MEM_FAR uint8_t *pScanData, // Pointer to array, must be as long as number
                                              // of entries times four (to read/write up to four bytes)
                                              // in list above
                  uint16_t Delay              // Delay in ms between read requests
);

/**************************************************************************
DOES:    Can be used to check if a previously started scan is still running
RETURNS: TRUE if scan is still running, else FALSE
**************************************************************************/
uint8_t MGRSCAN_GetStatus(uint8_t node_id // Node ID of node whic is auto-scanned
);

#ifdef __cplusplus
}
#endif

#endif // _SDOCLNT_H
/**************************************************************************
END OF FILE
**************************************************************************/
