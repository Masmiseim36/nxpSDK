/**************************************************************************
MODULE:    USDOCLNT
CONTAINS:  MicroCANopen Plus implementation, CANopen USDO Clients
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

#ifndef _UUSDOCLNT_H
#define _UUSDOCLNT_H

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
// Number of SDO/USDO channels implemented
#define NR_OF_SDO_CLIENTS 2
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

/**************************************************************************
GLOBAL DEFINITIONS
**************************************************************************/

#if (NR_OF_SDO_CLIENTS > 0)
// USDO Client Status Flags (in USDOCLIENT)
#define USDOCL_READY      0x00 // Channel idle
#define USDOCL_EXP_WRCONF 0x02 // Waiting for expedited USDO write response
#define USDOCL_RDCONF     0x03 // Waiting for (any) USDO read response
#define USDOCL_EXP_RDCONF 0x04 // Expedited USDO read confirmed

// For segmented transfers
#define USDOCL_SEG_INITWR 0x10 // Segmented Write Init
#define USDOCL_SEG_WRITE  0x11 // Segmented Write in progress
#define USDOCL_SEG_WRCONF 0x12 // Segmented Write over
#define USDOCL_SEG_WRNEXT 0x13 // Segmented Write over
#define USDOCL_SEG_WRFINA 0x14 // Segmented Write over, final & call-back
#define USDOCL_SEG_READ   0x21 // Segmented Read in progress
#define USDOCL_SEG_RDCONF 0x22 // Segmented Read over
#define USDOCL_SEG_RDFINA 0x23 // Segmented Read over, final & call-back

// For block transfer
#define USDOCL_BLOCK_INITWR 0x30 // Block Write Init
#define USDOCL_BLOCK_WRITE  0x31 // Block Write in progress
#define USDOCL_BLOCK_WRCONF 0x32 // Block Write over
#define USDOCL_BLOCK_WRFINA 0x33 // Block Write over, final & call-back
#define USDOCL_BLOCK_READ   0x41 // Block Read in progress
#define USDOCL_BLOCK_RDCONF 0x42 // Block Read over
#define USDOCL_BLOCK_RDFINA 0x43 // Block Read over, final & call-back

// USDO Response Status (in USDOCLNT_GetStatus)
#define USDOERR_FATAL   0x00 // Illegal pointer passed
#define USDOERR_OK      0x01 // Confirmation, last access was sucess
#define USDOERR_ABORT   0x80 // Abort received
#define USDOERR_TIMEOUT 0x81 // Request timed out
#define USDOERR_SEQ     0x82 // Sequence error
#define USDOERR_BUFSIZE 0x84 // Out of Memory
#define USDOERR_PARAM   0x85 // Wrong parameter
#define USDOERR_UNKNOWN 0x88 // No transfer possible
#define USDOERR_RUNNING 0xFF // USDO Transfer still running, not complete
#endif                       // NR_OF_SDO_CLIENTS > 0

// Scanning of nodes: states
#define SCAN_NONE      0x00 // node is not present
#define SCAN_DELAY     0x02 // started a delay
#define SCAN_RUN       0x80 // scanning in progress
#define SCAN_WAITREPLY 0x81 // waiting for reply
#define SCAN_ABORT     0xC0 // scan aborted
#define SCAN_DONE      0xFE // scan complete
#define SCAN_OVER      0xFF // scan is over

// Definitions for broadcast/multicast

#if USDOCLNT_FLAGTYPE_64
#define USDOCLNT_FLAGTYPE uint64_t
#define SETBIT(position)  (1ULL << position)
#define ALLBITS           0xFFFFFFFFFFFFFFFFULL
#else
#define USDOCLNT_FLAGTYPE uint32_t
#define SETBIT(position)  (1UL << position)
#define ALLBITS           0xFFFFFFFFUL
#endif

#define USDO_CLIENT_RESPONSE_ACK(client_ptr, ind) \
    {                                             \
        client_ptr->responses_ack |= SETBIT(ind); \
    }
#define USDO_CLIENT_RESPONSE_NAK(client_ptr, ind)  \
    {                                              \
        client_ptr->responses_nak |= SETBIT(ind);  \
        client_ptr->responses_cmp &= ~SETBIT(ind); \
    }
#define USDO_CLIENT_RESPONSES_ALL(client_ptr) ((client_ptr->responses_ack == client_ptr->responses_cmp))

/**************************************************************************
GLOBAL TYPES AND STRUCTURES
**************************************************************************/

typedef struct
{
    CAN_MSG sdomsg;                    // Message buffer for send and receive
    uint8_t init;                      // TRUE if client initialized
    uint8_t srvnid;                    // server node ID or USDO_NID_BCAST for broadcast
    uint8_t curnid;                    // last server node ID , for broadcast receive
    uint8_t cnt;                       // segment counter
    uint32_t bufmax;                   // Maximum length of buffer
    uint32_t buflen;                   // Length of expected transfer
    uint32_t curlen;                   // Current length of buffer
    MEM_FAR uint8_t *pBuf;             // Pointer to buffer for transfer
    uint16_t timeout;                  // USDO Timeout current timestamp
    uint16_t timeout_reload;           // USDO Timeout re-load value
    uint16_t b2btimeout;               // Back-to-Back Timestamp
    uint16_t tproc;                    // Back-to-Back processing time
    uint16_t index;                    // Index of current request
    uint8_t subindex;                  // Subindex of current request
    uint8_t datatype;                  // Data type of current request
    uint8_t last_abort;                // last abort code if any
    uint8_t blocked;                   // TRUE, if blocked mode shall be used when length is greater than 56 bytes
    uint8_t msg_pending;               // message couldn't be transmit, try again
    uint8_t session;                   // Session ID
    uint8_t status;                    // Channel status info
    uint8_t channel;                   // Channel number from 1 to NR_OF_SDO_CLIENTS
    MEM_FAR uint8_t *num_servers;      // Pointer to maximum number of USDO servers, up to 32/64
    MEM_FAR uint8_t *server_list;      // Pointer to list of responding USDO servers
    USDOCLNT_FLAGTYPE responses_ack;   // Flags for successful USDO response arrived (one bit each)
    USDOCLNT_FLAGTYPE responses_nak;   // Flags for USDO Abort response arrived or USDO timeout (one bit each)
    USDOCLNT_FLAGTYPE responses_cmp;   // Saved _ack value for subsequent cycles
    uint8_t bcresp[CAN_MAX_DATA_SIZE]; // Buffer for the last received USDO response
} USDOCLIENT;

#if NR_OF_SDO_CLIENTS > 0
// data records for each client
extern USDOCLIENT MEM_FAR gUSDOClientList[NR_OF_SDO_CLIENTS];
#endif

/**************************************************************************
PUBLIC FUNCTIONS
**************************************************************************/

/**************************************************************************
DOES:    This function resets all USDO Client channels
RETURNS: nothing
**************************************************************************/
void USDOCLNT_ResetChannels(void);

/**************************************************************************
DOES:    (Re-)initializes an USDO client channel.
RETURNS: NULL-Pointer, if channel initialization failed.
         Pointer to USDOCLIENT structure used, if init success
**************************************************************************/
MEM_FAR USDOCLIENT *USDOCLNT_Init(
    uint8_t channel,        // USDO channel number in range of 1 to NR_OF_SDO_CLIENTS
    uint8_t srvnid,         // USDO server node ID
    MEM_FAR uint8_t *p_buf, // data buffer pointer for data exchanged
    uint32_t buf_size,      // max length of data buffer
    uint8_t blocked         // TRUE, if blocked mode shall be used when length is greater than 56 bytes
);

/**************************************************************************
DOES:    (Re-)initializes a broadcast USDO client channel.
RETURNS: NULL-Pointer, if channel initialization failed.
         Pointer to USDOCLIENT structure used if init success
***************************************************************************/
MEM_FAR USDOCLIENT *USDOCLNT_Init_Bcast(
    uint8_t channel,              // USDO channel number in range of 1 to NR_OF_SDO_CLIENTS
    MEM_FAR uint8_t *num_servers, // Pointer to maximum number of USDO servers, up to 31/63, or 0 for auto-detect (needs
                                  // maximum server_list size)
    MEM_FAR uint8_t *server_list, // Pointer to list of server node IDs
    MEM_FAR uint8_t *p_buf,       // data buffer pointer for data exchanged
    uint32_t buf_size,            // max length of data buffer
    uint8_t blocked               // TRUE, if blocked mode shall be used when length is greater than 56 bytes
);

/**************************************************************************
DOES:    Transmits a USDO Write (download) request.
RETURNS: TRUE, if request was queued
         FALSE, if transmit queue full
GLOBALS: Uses the last data buffer assigned from last call to USDOCLNT_Init,
         USDOCLNT_ReadXtd or USDOCLNT_WriteXtd
NOTE:    Non blocking transfer, use functions USDOCLNT_GetStatus,
         USDOCLNT_BlockUntilCompleted or SDOCLNTCB_SDOComplete to determine
         when transfer completed
**************************************************************************/
uint8_t USDOCLNT_Write(MEM_FAR USDOCLIENT *p_client, // Pointer to initialized USDO client structure
                       uint16_t index,               // Object Dictionary Index to write
                       uint8_t subindex,             // Object Dictionary Subindex to write
                       uint8_t datatype              // Data type of entry to write
);

/**************************************************************************
DOES:    Transmits a USDO Write (download) request with extended parameters
RETURNS: TRUE, if request was queued
         FALSE, if transmit queue full
NOTE:    Non blocking transfer, use functions USDOCLNT_GetStatus,
         USDOCLNT_BlockUntilCompleted or SDOCLNTCB_SDOComplete to determine
         when transfer completed
**************************************************************************/
uint8_t USDOCLNT_WriteXtd(MEM_FAR USDOCLIENT *p_client, // Pointer to initialized USDO client structure
                          uint16_t index,               // Object Dictionary Index to write
                          uint8_t subindex,             // Object Dictionary Subindex to write
                          uint8_t datatype,             // Data type of entry to write
                          MEM_FAR uint8_t *pSrc,        // Pointer to data source
                          uint32_t len,                 // Length of data
                          uint16_t timeout,             // Timeout for this transfer in milliseconds
                          uint8_t blocked // TRUE, if blocked mode shall be used when length is greater than 56 bytes
);

/**************************************************************************
DOES:    Transmits a USDO Read (upload) request.
RETURNS: TRUE, if request was queued
         FALSE, if transmit queue full
GLOBALS: Uses the last data buffer assigned from last call to USDOCLNT_Init,
         USDOCLNT_ReadXtd or USDOCLNT_WriteXtd
NOTE:    Non blocking transfer, use functions USDOCLNT_GetStatus,
         USDOCLNT_BlockUntilCompleted or USDOCLNTCB_USDOComplete to determine
         when transfer completed
**************************************************************************/
uint8_t USDOCLNT_Read(MEM_FAR USDOCLIENT *p_client, // Pointer to initialized USDO client structure
                      uint16_t index,               // Object Dictionary Index to read
                      uint8_t subindex              // Object Dictionary Subindex to read
);

/**************************************************************************
DOES:    Transmits a USDO Read (upload) request with extended parameters
RETURNS: TRUE, if request was queued
         FALSE, if transmit queue full
NOTE:    Non blocking transfer, use functions USDOCLNT_GetStatus,
         USDOCLNT_BlockUntilCompleted or USDOCLNTCB_USDOComplete to determine
         when transfer completed
**************************************************************************/
uint8_t USDOCLNT_ReadXtd(MEM_FAR USDOCLIENT *p_client, // Pointer to initialized USDO client structure
                         uint16_t index,               // Object Dictionary Index to read
                         uint8_t subindex,             // Object Dictionary Subindex to read
                         MEM_FAR uint8_t *pDest,       // Pointer to data destination
                         uint32_t len,                 // Maximum length of data destination
                         uint16_t timeout              // Timeout for this transfer in milliseconds
);

/**************************************************************************
DOES:    Checks the current status of the USDO client
RETURNS: USDOERR_xxx codes
**************************************************************************/
uint8_t USDOCLNT_GetStatus(MEM_FAR USDOCLIENT *p_client // Pointer to initialized USDO client structure
);

/**************************************************************************
DOES:    Checks if a USDO client is busy or not
RETURNS: TRUE if busy
**************************************************************************/
uint8_t USDOCLNT_IsBusy(MEM_FAR USDOCLIENT *p_client // Pointer to initialized USDO client structure
);

/**************************************************************************
DOES:    Returns the last abort code of the USDO client, if any
RETURNS: Last USDO client Abort code or 0
**************************************************************************/
uint8_t USDOCLNT_GetLastAbort(MEM_FAR USDOCLIENT *p_client // Pointer to initialized USDO client structure
);

/**************************************************************************
DOES:    Blocking call to wait until previous USDO transfer completed.
RETURNS: USDOERR_xxx status codes
**************************************************************************/
uint8_t USDOCLNT_BlockUntilCompleted(MEM_FAR USDOCLIENT *p_client // Pointer to initialized USDO client structure
);

/**************************************************************************
DOES:    Call-Back to application when an USDO client transfer is completed
RETURNS: nothing
**************************************************************************/
void USDOCLNTCB_USDOComplete(uint8_t channel,    // USDO channel number in range of 1 to NR_OF_SDO_CLIENTS
                             uint32_t abort_code // USDOERR_xxx codes or USDO abort code
);

/**************************************************************************
DOES:    Transmits a USDO read request and waits for the response
         or an abort.
RETURNS: 0 if access aborted, else length of data received
**************************************************************************/
uint32_t USDOCLNT_ReadCycle(uint8_t channel,        // USDO channel number in range of 1 to NR_OF_SDO_CLIENTS
                            uint8_t srvnid,         // USDO server node ID
                            uint16_t index,         // Object Dictionary Index to read
                            uint8_t subindex,       // Object Dictionary Subindex to read
                            MEM_FAR uint8_t *p_buf, // data buffer pointer for data received
                            uint32_t buf_size,      // max length of data buffer
                            uint16_t timeout        // Timeout for this transfer in milliseconds, 0 for default
);

/**************************************************************************
DOES:    Transmits a USDO Write request and waits for the response
         or an abort.
RETURNS: TRUE if write transfer was successful
**************************************************************************/
uint8_t USDOCLNT_WriteCycle(uint8_t channel,        // USDO channel number in range of 1 to NR_OF_SDO_CLIENTS
                            uint8_t srvnid,         // USDO server node ID
                            uint16_t index,         // Object Dictionary Index to read
                            uint8_t subindex,       // Object Dictionary Subindex to read
                            uint8_t datatype,       // Data type of entry to write
                            MEM_FAR uint8_t *p_buf, // data buffer pointer for data transmitted
                            uint32_t len,           // length of data transmitted
                            uint8_t blocked, // TRUE, if blocked mode shall be used when length is greater than 56 bytes
                            uint16_t timeout // Timeout for this transfer in milliseconds, 0 for default
);

/**************************************************************************
DOES:    Aborts a transfer currently in progress.
RETURNS: nothing
**************************************************************************/
void USDOCLNT_AbortTransfer(MEM_FAR USDOCLIENT *p_client, // Pointer to initialized USDO client structure
                            uint8_t abort_code            // Abort code to transmit
);

/**************************************************************************
DOES:    Gets executed if a CAN message was received that is the response
         to a USDO request
RETURNS: TRUE if message was handled
***************************************************************************/
uint8_t USDOCLNT_HandleUSDOClientResponse(MEM_FAR CAN_MSG *p_rxusdo // Pointer to received USDO response
);

/**************************************************************************
DOES: USDO Client Handler, needs to be executed frequently
RETURNS: TRUE, if a message was generated
***************************************************************************/
uint8_t USDOCLNT_USDOHandleClient(void);

/**************************************************************************
DOES:    Initiates multiple USDO read requests to be sent, results read
         go directly into an array of uint32_t. Limited to expedited
         transfers of up to 4 byte.
         Format of the ScanList is 4 bytes per entry
         0,1: Index (set to 0xFFFF to mark end of list)
         2: Subindex
         3: Length
         When the scan completed, MGRCB_NodeStatusChanged() is called
RETURNS: nothing
**************************************************************************/
void MGRSCAN_Init(uint8_t sdo_clnt,           // USDO client number from 1 to NR_OF_SDO_CLIENTS
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

#if USDOCLNTCB_APPSDO_WRITE
/*******************************************************************************
DOES:    Call Back function to allow custom USDO client writes/downloads of
         longer data than local buffer size.
RETURNS: 0x00 - No custom handling, or no data
         0x01 - Custom handling, data buffer filled
*******************************************************************************/
uint8_t USDOCLNTCB_USDOWriteInit(
    MEM_FAR USDOCLIENT *p_client, // Pointer to initialized USDO client structure
    uint16_t idx,                 // Index of OD entry
    uint8_t subidx,               // Subindex of OD entry
    uint32_t MEM_FAR *size,       // Size of USDO client data buffer, RETURN: length of data filled
    uint32_t MEM_FAR *totalsize,  // RETURN: total size of data, only set if >*size
    uint8_t MEM_FAR *pDat         // Pointer to data buffer to fill, RETURN: data up to *size
);

/*******************************************************************************
DOES:    Call Back function to allow custom USDO client writes/downloads of
         longer data than local buffer size. Provides next data block to write,
         or signals the last block.
RETURNS: 0x00 - No custom handling, or no data
         0x01 - Custom handling, data buffer filled
*******************************************************************************/
uint8_t USDOCLNTCB_USDOWriteComplete(
    MEM_FAR USDOCLIENT *p_client, // Pointer to initialized USDO client structure
    uint16_t idx,                 // Index of OD entry
    uint8_t subidx,               // Subindex of OD entry
    uint32_t MEM_FAR *size,       // Size of USDO client data buffer, RETURN: length of data filled
    uint32_t more,                // Number of bytes still needed (of total transfer)
    uint8_t MEM_FAR *pDat         // Pointer to data buffer to fill, RETURN: data up to *size
);
#endif

#ifdef __cplusplus
}
#endif

#endif // _UUSDOCLNT_H
/**************************************************************************
END OF FILE
**************************************************************************/
