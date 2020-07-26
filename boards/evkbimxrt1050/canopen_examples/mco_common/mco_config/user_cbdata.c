/**************************************************************************
MODULE:    USER_CBDATA
CONTAINS:  Default functions for user call-backs accessing process data
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2019
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.01, EmSA 02-APR-20
           $LastChangedDate: 2020-04-02 17:30:41 +0200 (Thu, 02 Apr 2020) $
           $LastChangedRevision: 4909 $
***************************************************************************/

#include "mcop_inc.h"

#ifdef MCOUSER_MINMAX
// SET THIS DEFINE MANUALLY TO ENABLE A CUSTOM MIN/MAX CHECK
// Also make sure to set USECB_SDO_WR_PI
// User example: Min Max control of SDO acces
uint8_t MEM_CONST gProcMin[PROCIMG_SIZE] = PIMGMINS;
uint8_t MEM_CONST gProcMax[PROCIMG_SIZE] = PIMGMAXS;
#endif

#if USECB_RPDORECEIVE
/**************************************************************************
DOES:    This function is called after an RPDO has been received and stored
         into the Process Image.
RETURNS: nothing
**************************************************************************/
void MCOUSER_RPDOReceived(uint16_t RPDONr, // RPDO Number
                          uint16_t offset, // Offset to RPDO data in Process Image
                          uint8_t len      // Length of RPDO
)
{
}
#endif // USECB_RPDORECEIVE

#if USECB_ODDATARECEIVED
/**************************************************************************
DOES:    This function is called after Object Dictionary data was received
         (works for SDO/USDO and PDO).
RETURNS: nothing
**************************************************************************/
void MCOUSER_ODData(uint8_t client_nid,     // node ID from where data arrived (0 if unknown)
                    uint16_t idx,           // Index
                    uint8_t subidx,         // Subindex
                    uint8_t MEM_PROC *pDat, // pointer to data
                    uint16_t len            // length of data
)
{
}
#endif // USECB_ODDATARECEIVED

#if USECB_TPDORDY
/**************************************************************************
DOES:    This function is called before a TPDO is sent. For triggering
         modes that are outside of the application's doing (Event Timer,
         SYNC), it is called before the sent data is retrieved from the
         Process Image. This allows the application to update the TPDO
         data if necessary.
NOTE:    This function is also called before a change-of-state or
         application-triggered TPDO is sent, but updating the Process Image
         will not have any effect on the TPDO data in this case.
RETURNS: TRUE to allow the PDO to be sent, FALSE to stop PDO transmission
**************************************************************************/
uint8_t MCOUSER_TPDOReady(uint16_t TPDONr,    // TPDO Number
                          uint8_t TPDOTrigger // Trigger for this TPDO's transmission:
                                              // 0: Event Timer
                                              // 1: SYNC
                                              // 2: SYNC+COS
                                              // 3: COS or application trigger
)
{
    // always transmit if event timer or SYNC is being used
    if (TPDOTrigger < 2)
        return TRUE;

    // customize for application-specific TPDO send conditions
    return TRUE;
}
#endif // USECB_TPDORDY

#if USECB_SYNCRECEIVE
/**************************************************************************
DOES:    This function is called with every SYNC message received.
         It allows the application to now apply all sync-triggered TPDO
         data to be applied to the application
RETURNS: nothing
**************************************************************************/
void MCOUSER_SYNCReceived(void)
{
}
#endif // USECB_SYNCRECEIVE

#if USECB_SDO_RD_PI
/**************************************************************************
DOES:    This function is called before an SDO or USDO read request is
         executed reading from the process image. The application can
         use this function to either update the data or to deny access
         (by returning an SDO or USDO Abort code).
RETURNS: 0, if access is granted, data can be copied and returned or
         CANopen SDO or USDO Abort Code - in which case the (U)SDO
         transfer is aborted
**************************************************************************/
uint32_t MCOUSER_SDORdPI(uint8_t client_nid, // node ID from where the request came (0 if unknown)
                         uint16_t index,     // Index of Object Dictionary entry
                         uint8_t subindex,   // Subindex of Object Dictionary entry
                         uint16_t offset,    // Offset to data in process image
                         uint16_t len        // Length of data
)
{
    return 0;
}
#endif // USECB_SDO_RD_PI

#if USECB_SDO_RD_AFTER
/**************************************************************************
DOES:    This function is called after an SDO or USDO read request was
         executed. The application can use this to clear the data or
         mark it as read.
RETURNS: Nothing
**************************************************************************/
void MCOUSER_SDORdAft(uint8_t client_nid, // node ID from where the request came (0 if unknown)
                      uint16_t index,     // Index of Object Dictionary entry
                      uint8_t subindex,   // Subindex of Object Dictionary entry
                      uint16_t offset,    // Offset to data in process image
                      uint16_t len        // Length of data
)
{
}
#endif // USECB_SDO_RD_AFTER

#if USECB_SDO_WR_PI
/**************************************************************************
DOES:    This function is called before an SDO or USDO write request is
         executed writing to the process image. The application can use
         this function to check the data (e.g. range check) BEFORE it
         gets written to the process image.
RETURNS: 0, if access is granted, data can be copied to process image or
         CANopen SDO or USDO Abort Code - in which case the (U)SDO
         transfer is aborted
**************************************************************************/
uint32_t MCOUSER_SDOWrPI(uint8_t client_nid, // node ID from where the request came (0 if unknown)
                         uint16_t index,     // Index of Object Dictionary entry
                         uint8_t subindex,   // Subindex of Object Dictionary entry
                         uint16_t offset,    // Offset to data in process image
                         uint8_t *pDat,      // Pointer to data received
                         uint16_t len        // Length of data
)
{
    return 0;
}
#endif // USECB_SDO_WR_PI

#if USECB_SDO_WR_AFTER
/**************************************************************************
DOES:    This function is called after an SDO or USDO write request was
         executed. Data is now in the process image and can be processed.
RETURNS: Nothing
**************************************************************************/
void MCOUSER_SDOWrAft(uint8_t client_nid, // node ID from where the request came (0 if unknown)
                      uint16_t index,     // Index of Object Dictionary entry
                      uint8_t subindex,   // Subindex of Object Dictionary entry
                      uint16_t offset,    // Offset to data in process image
                      uint16_t len        // Length of data
)
{
}
#endif // USECB_SDO_WR_AFTER

#if USECB_APPSDO_READ
/*******************************************************************************
DOES:    Call Back function to allow implementation of custom, application
         specific OD Read entries
         Here: Alternating between 2 different strings
RETURNS: 0x00 - OD entry not handled by this function
         0x01 - OD entry handled by this function
         0x05 - Abort with "attempting to read a write-only object"
         0x06 - Abort with "entry does not exist"
         0x08 - Abort with "data type doesn't match" (CANopen FD only)
*******************************************************************************/
uint8_t MCOUSER_AppSDOReadInit(uint8_t sdoserver_client_nid, // CANopen: The SDO server number on which
                                                             // the request came in.
                                                             // CANopen FD: The USDO client node ID
                                                             // from which the request came in.
                               uint16_t idx,                 // Index of OD entry
                               uint8_t subidx,               // Subindex of OD entry
                               uint32_t MEM_FAR *totalsize,  // RETURN: total size of data, only set if >*size
                               uint32_t MEM_FAR *size,       // RETURN: size of data buffer
                               uint8_t *MEM_FAR *pDat,       // RETURN: pointer to data buffer
                               uint8_t MEM_FAR *type         // RETURN: data type (CANopen FD only)
)
{
    return 0x00u;
}

/*******************************************************************************
DOES:    Call Back function to allow implementation of custom, application
         specific OD Read entries, called at end of transfer with the option
         to add more data.
RETURNS: Nothing
*******************************************************************************/
void MCOUSER_AppSDOReadComplete(uint8_t sdoserver_client_nid, // CANopen: The SDO server number on which
                                                              // the request came in.
                                                              // CANopen FD: The USDO client node ID
                                                              // from which the request came in.
                                uint16_t idx,                 // Index of OD entry
                                uint8_t subidx,               // Subindex of OD entry
                                uint32_t MEM_FAR *size // RETURN: size of next block of data, 0 for no further data
)
{
    return;
}
#endif // USECB_APPSDO_READ

#if USECB_APPSDO_WRITE
/*******************************************************************************
DOES:    Call Back function to allow implementation of custom, application
         specific OD Read entries
         Here: Simply receive data
RETURNS: 0x00 - OD entry not handled by this function
         0x01 - OD entry handled by this function
         0x04 - Abort with "attempting to write a read-only object"
         0x06 - Abort with "entry does not exist"
         0x08 - Abort with "data type doesn't match" (CANopen FD only)
*******************************************************************************/
uint8_t MCOUSER_AppSDOWriteInit(uint8_t sdoserver_client_nid, // CANopen: The SDO server number on which
                                                              // the request came in.
                                                              // CANopen FD: The USDO client node ID
                                                              // from which the request came in.
                                uint16_t idx,                 // Index of OD entry
                                uint8_t subidx,               // Subindex of OD entry
                                uint32_t MEM_FAR *totalsize,  // RETURN: total maximum size of data, only set if >*size
                                uint32_t MEM_FAR *size,       // Data size, if known. RETURN: max size of data buffer
                                uint8_t *MEM_FAR *pDat,       // RETURN: pointer to data buffer
                                uint8_t MEM_FAR *type         // RETURN: data type (CANopen FD only)
)
{
    return 0x00u;
}

/*******************************************************************************
DOES:    Call Back function to allow implementation of custom, application
         specific OD Write entries, call at end of transfer of a block. For
         multiple blocks per transfer, the same buffer is used for all blocks.
RETURNS: 0x00 - OD entry not handled by this function
         0x01 - OD entry handled by this function
         0x04 - Abort with "attempting to write a read-only object"
*******************************************************************************/
uint8_t MCOUSER_AppSDOWriteComplete(uint8_t sdoserver_client_nid, // CANopen: The SDO server number on which
                                                                  // the request came in.
                                                                  // CANopen FD: The USDO client node ID
                                                                  // from which the request came in.
                                    uint16_t idx,                 // Index of OD entry
                                    uint8_t subidx,               // Subindex of OD entry
                                    uint32_t size,                // Number of bytes written (of last block)
                                    uint32_t more                 // number of bytes still to come (of total transfer)
)
{
    return 0x00u;
}
#endif // USECB_APPSDO_WRITE

/**************************************************************************
END-OF-FILE
***************************************************************************/
