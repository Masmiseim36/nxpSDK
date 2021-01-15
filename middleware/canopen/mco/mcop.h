/**************************************************************************
MODULE:    MCOP
CONTAINS:  MicroCANopen Plus implementation
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-17 16:12:10 +0200 (Thu, 17 Sep 2020) $
           $LastChangedRevision: 5047 $
***************************************************************************/

#ifndef _MCOP_H
#define _MCOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mco.h"

/**************************************************************************
Defines for SLEEP modes
**************************************************************************/
#if USE_SLEEP
#define SLEEP_MASTER_QUERY_OBJECTION 0x01
#define SLEEP_SLAVE_SLEEP_OBJECTION  0x81
#define SLEEP_MASTER_SET_SLEEP       0x02
#if USE_LSS_SLAVE
#define SLEEP_WAKEUP 0x81
#else
#define SLEEP_WAKEUP 0x82
#endif
#define SLEEP_SLAVE_REQUEST 0x03
#define SLEEP_REASON_NONE   0xFE
#endif // USE_SLEEP

#if USE_EMCY
// This structure holds all emergency configuration
typedef struct
{
    CAN_MSG emcy_msg;
    uint16_t emcy_inhibit;   // Emergency inhibit time in ms
    uint16_t emcy_timestamp; // Timestamp of next allowed emergency
    uint16_t active_sys;     // active system EMCYs
#if NR_OF_RPDOS > 0
                         // active RDO len EMCY
    uint16_t active_rpdo[(NR_OF_RPDOS + 15) / 16];
#endif
#if MGR_MONITOR_ALL_NODES
    uint16_t active_hbcons[(MAX_NR_OF_NODES + 15) / 16];
#elif NR_OF_HB_CONSUMER > 0
                         // active HB loss EMCY
    uint16_t active_hbcons[(NR_OF_HB_CONSUMER + 15) / 16];
#endif

#if ERROR_FIELD_SIZE > 0
    uint32_t Field[ERROR_FIELD_SIZE];
    uint8_t InPtr;
    uint8_t NrOfRec;
#if defined(USE_CANOPEN_FD) && USE_CANOPEN_FD // CANopen FD
    uint8_t status[ERROR_FIELD_SIZE];         // status esp. recoverable/non-recoverable
#endif                                        // CANopen FD
#endif
} EMCY_CONFIG; // emergency handling data record

// Macros that set, clr or check a bit in 16 bit array active_hbcons and active_rpdo
#define ARRAY16_SETBIT(array, bit) (array[(bit) / 16] |= (0x0001 << ((bit) % 16)))
#define ARRAY16_CLRBIT(array, bit) (array[(bit) / 16] &= ~(0x0001 << ((bit) % 16)))
#define ARRAY16_GETBIT(array, bit) ((array[(bit) / 16] >> ((bit) % 16)) & 0x0001)

// Macro to generate the 32bit err codes used in error field
#define MAKE_ERRCODE32(code, em1, em2) ((uint32_t)(code) | ((uint32_t)(em1) << 16) | ((uint32_t)(em2) << 24))

#endif

#if (NR_OF_HB_CONSUMER > 0)
// This structure holds all node specific configuration
typedef struct
{
    uint16_t time;       // Heartbeat Consumer time in ms
    uint16_t timestamp;  // Timestamp of last heartbeat consumed
    uint16_t can_id;     // CAN ID to monitor 0x700 + node ID
    HBCONS_STATE status; // state of this consumer: off, init, active, lost
} HBCONS_CONFIG;
#endif // (NR_OF_HB_CONSUMER > 0)

/**************************************************************************
DOES:    This function reads data from the process image and copies it
         to an OUTPUT location
RETURNS: Number of bytes that were copied
**************************************************************************/
uint8_t MCO_ReadProcessData(uint8_t MEM_PROC *pDest, // Destination pointer
                            uint8_t length,          // Number of bytes to copy
                            uint16_t offset          // Offset of source data in process image
);

/**************************************************************************
DOES:    This function writes data from an INPUT location to the process
         image
RETURNS: Number of bytes that were copied
**************************************************************************/
uint8_t MCO_WriteProcessData(uint16_t offset,       // Offset of destination data in process image
                             uint8_t length,        // Number of bytes to copy
                             uint8_t MEM_PROC *pSrc // Source pointer
);

/**************************************************************************
DOES:    Obtain PI offset and length for short (1-4 bytes) OD entries by
         index/subindex.
RETURNS: 0: Entry exists, results valid
         1: Error - entry not found in PI, or is no short entry
**************************************************************************/
uint8_t MCO_GetODProcPIOffsLenByIndex(uint16_t index,   // OD index of entry in PI
                                      uint8_t subindex, // OD subindex of entry in PI
                                      uint16_t *offset, // out: offset of OD entry in PI
                                      uint8_t *len      // out: length of OD entry in PI
);

/**************************************************************************
DOES:    Obtain PI offset and length for any PI-based OD entries by
         index/subindex.
RETURNS: 0: Entry exists, results valid
         1: Error - entry not found in PI, or is no short entry
**************************************************************************/
uint8_t MCO_GetPIOffsLenByIndex(uint16_t index,   // OD index of entry in PI
                                uint8_t subindex, // OD subindex of entry in PI
                                uint16_t *offset, // out: offset of OD entry in PI
                                uint16_t *len     // out: length of OD entry in PI
);

/**************************************************************************
DOES:    Read OD entries by index/subindex.
         Short entries up to 4 bytes read into variables and obey byte order,
         (potentially) longer entries we read into a byte array.
NOTE:    This function executes a search for the index/subindex selected.
         For better performance data access, use MCO_ReadProcessData()
RETURNS: 0: Entry read ok
         1: Error - entry not found in PI
         2: Error - data length mismatch
         3: Error - data length exceeded maximum
         4: Error - parameter error
**************************************************************************/
uint8_t MCO_ReadValByIndex(uint16_t index,     // OD index of entry in PI
                           uint8_t subindex,   // OD subindex of entry in PI
                           uint8_t len_type,   // 1-4: read into uint8_t..uint32_t, 0: read into byte array
                           uint16_t *len_read, // in: if len_type==0 give max. read length
                                               // out: must be !=NULL to return read length for larger entries
                           void *data          // pointer to write data (variable or byte array)
);

/**************************************************************************
DOES:    Write OD entries to PI by index/subindex.
         Short entries up to 4 bytes write from variables and obey byte order,
         longer entries we write from a byte array.
NOTE:    This function executes a search for the index/subindex selected.
         For better performance data access, use MCO_WriteProcessData()
RETURNS: 0: Entry write ok
         1: Error - entry not found in PI
         2: Error - data length mismatch
         3: Error - data length exceeded maximum
         4: Error - parameter error
**************************************************************************/
uint8_t MCO_WriteValByIndex(uint16_t index,     // OD index of entry in PI
                            uint8_t subindex,   // OD subindex of entry in PI
                            uint8_t len_type,   // 1-4: write from uint8_t..uint32_t, 0: write from byte array
                            uint16_t len_write, // if len_type==0 give write length, otherwise 0
                            void *data          // pointer to write data (variable or byte array)
);

#if USE_EMCY
/**************************************************************************
DOES:    Transmits an Emergency Message
RETURNS: TRUE - If msg was considered for transmit
         FALSE - If message was not sent due to duplicate
**************************************************************************/
uint8_t MCOP_PushEMCY(uint16_t emcy_code, // 16 bit error code
                      uint8_t em_1,       // 5 byte manufacturer specific error code
                      uint8_t em_2,
                      uint8_t em_3,
                      uint8_t em_4,
                      uint8_t em_5
#if defined(USE_CANOPEN_FD) && USE_CANOPEN_FD // CANopen FD
                      ,
                      uint8_t dev_num,   // logical device number
                      uint16_t spec_num, // CiA specification number
                      uint8_t status,    // status
                      uint32_t time_lo,  // timestamp bits 0-31
                      uint16_t time_hi   // timestamp bits 32-47
#endif                                   // CANopen FD
);

#if ERROR_FIELD_SIZE > 0
/**************************************************************************
DOES:    This function clears all entries of the error history [1003h] for
         CANopen or the active error list [1032h] for CANopen FD.
RETURNS: Nothing
**************************************************************************/
void MCOP_ErrField_Flush(void);

/**************************************************************************
DOES:    This function adds or updates an entry to/in the error history
         [1003h] for CANopen or the active error list [1032h] for
         CANopen FD.
RETURNS: Nothing
**************************************************************************/
void MCOP_ErrField_AddUpdate(uint32_t err_value // the 32bit error code used in the last EMCY
#if defined(USE_CANOPEN_FD) && USE_CANOPEN_FD
                             ,
                             uint8_t status // status esp. recoverable/non-recoverable
#endif                                      // CANopen FD
);

/**************************************************************************
DOES:    This function removes an entry from the error history [1003h]
         for CANopen or the active error list [1032h] for CANopen FD.
RETURNS: The subindex value that was removed, 0 if there was no matching
         error found.
**************************************************************************/
uint8_t MCOP_ErrField_Remove(uint32_t err_value // the 32-bit error code used in the last EMCY
);

/**************************************************************************
DOES:    This function retrieves an entry from the error history [1003h]
         for CANopen or the active error list [1032h] for CANopen FD, based
         on the subindex
RETURNS: 32-bit error code for OD at subindex, or raw stored value
**************************************************************************/
uint32_t MCOP_ErrField_Get(uint8_t subindex, // Subindex number of [1003h]/[1032h]
                           uint8_t od_val,   // TRUE: Value for OD, FALSE: raw stored value
                           uint8_t *ef_index // if !=NULL, return internal index to entry
);

/**************************************************************************
DOES:    This function finds an entry in the error history [1003h]
         for CANopen or the active error list [1032h] for CANopen FD, based
         on the 32-bit error code used in the EMCY.
RETURNS: The subindex value for the entry or 0 if none found.
**************************************************************************/
uint8_t MCOP_ErrField_Find(uint32_t err_value, // the 32bit error code used in the last EMCY
                           uint8_t *ef_index   // if !=NULL, return internal index to entry
);
#endif // ERROR_FIELD_SIZE > 0

/**************************************************************************
DOES:    Checks if all EMCY active bits are cleared.
         Used to determine if now an EMCY no error / reset can be sent.
RETURNS: TRUE - No more EMCY pending
         FALSE - Still some EMCY pending
**************************************************************************/
uint8_t MCOP_IsNoEMCYactive(void);
#endif // USE_EMCY

#if (MGR_MONITOR_ALL_NODES == 0)
/* Device version, for SELECTED heartbeats, Manager uses comgr.h*/
#if (NR_OF_HB_CONSUMER > 0)
/**************************************************************************
DOES:    Initializes Heartbeat Consumer
GLOBALS: Inits gHBCons[consumer_channel-1]
**************************************************************************/
void MCOP_InitHBConsumer(uint8_t consumer_channel, // HB Consumer channel
                         uint8_t node_id,          // Node ID to monitor
                         uint16_t hb_time          // Timeout ot use (in ms)
);

/**************************************************************************
DOES:    Checks if a message received contains a heartbeat to be consumed
GLOBALS: Updates gHBCons[consumer_channel-1]
RETURNS: one, if message received was a heartbeat monitored, else zero
**************************************************************************/
uint8_t MCOP_ConsumeHB(CAN_MSG MEM_FAR *pRxCAN // CAN message received
);

/**************************************************************************
DOES:    Checks if a heartbeat consumer timeout occured
RETURNS:
          HBCONS_OFF,    // Disabled
          HBCONS_INIT,   // Initialized, waiting for first 2 heartbeats
          HBCONS_ACTIVE, // Consumer active and OK
          HBCONS_LOST    // Heartbeat lost
**************************************************************************/
HBCONS_STATE MCOP_ProcessHBCheck(uint8_t consumer_channel);

/**************************************************************************
DOES:    Checks if a node ID is already used, needed for conformance test
RETURNS: TRUE, if node_id is already used
**************************************************************************/
uint8_t MCOP_IsHBMonitored(uint8_t channel, uint8_t node_id);
#endif // (NR_OF_HB_CONSUMER > 0)
#endif // ! MGR_MONITOR_ALL_NODES

#if USECB_NMTCHANGE
/**************************************************************************
DOES:    This function is called before a change to the NMT Slave state
         machine is applied. Values passed are one of the following:
         NMTSTATE_BOOT, NMTSTATE_STOP, NMTSTATE_OP or NMTSTATE_PREOP
RETURNS: nothing
**************************************************************************/
void MCOUSER_NMTChange(uint8_t NMTState);
#endif // USECB_NMTCHANGE

#if USECB_SDOREQ
/**************************************************************************
DOES:    This call-back function is called by the SDO Handler for unknown
         SDO Requests. This allows the application to add own, custom
         Object Dictionary entries. When using this call-back function,
         the function is responsible for generating the right response or
         abort. The function MCO_ReplyWith can be used to generate a
         response, the function MCO_SendSDOAbort to generate an abort.
RETURNS: 0 - An SDO Abort was generated by the call-back function
         1 - An SDO Response was generated by the call-back function
         2 - The call-back function did not do anything with Request
**************************************************************************/
#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
uint8_t MCOUSER_SDORequest(CAN_MSG *USDO_Req);
#else
uint8_t MCOUSER_SDORequest(uint8_t SDO_Data[8]);
#endif
#endif // USECB_SDOREQ

/**************************************************************************
INTERNAL FUNCTIONS FROM MCO.C AND MCOP.C
**************************************************************************/

/**************************************************************************
DOES:    Generates an SDO Abort Response
RETURNS: nothing
**************************************************************************/
void MCO_SendSDOAbort(uint32_t ErrorCode // 4 byte SDO abort error code
);

/**************************************************************************
DOES:    Common exit routine for SDO_Handler.
         Send SDO response with variable length (1-4 bytes).
         Assumes that gTxCAN.ID, LEN and BUF[1-3] are already set.
         Requires data to be in little endian format.
RETURNS: 1 if response transmitted
**************************************************************************/
uint8_t MCO_ReplyWith(uint8_t *pDat, // pointer to sdo data
                      uint8_t len    // number of bytes of data in SDO
);

#if (NR_OF_RPDOS > 0) || (NR_OF_TPDOS > 0)
/**************************************************************************
DOES: Common exit routine for SDO_Handler.
      Send SDO response with write confirmation.
      Assumes that gTxSDO.ID, LEN and BUF[1-3] are already set
**************************************************************************/
void MCOP_WriteConfirm(void);

/**************************************************************************
DOES:    Handles incoming SDO Request for accesses to PDO Communication
         Parameters
RETURNS: 0: No action, SDO request is not for PDO Communication
         1: Access was made, SDO Response sent
         2: Wrong access, SDO Abort sent
GLOBALS: Various global variables with configuration information
**************************************************************************/
uint8_t SDO_HandlePDOComParam(uint8_t PDOType, // 0 for TPDO, 1 for RPDO
                              uint16_t index,  // OD index
                              uint8_t *pData   // pointer to SDO Request message
);
#endif // (NR_OF_RPDOS > 0) || (NR_OF_TPDOS > 0)

#if USE_DYNAMIC_PDO_MAPPING
/**************************************************************************
DOES:    Handles incoming SDO Request for accesses to PDO Mapping
         Parameters
RETURNS: 0: No action, SDO request is not for PDO Mapping
         1: Access was made, SDO Response sent
         2: Wrong access, SDO Abort sent
GLOBALS: Various global variables with configuration information
**************************************************************************/
uint8_t SDO_HandlePDOMapParam(uint8_t PDOType, // 0 for TPDO, 1 for RPDO
                              uint16_t index,  // OD index
                              uint8_t *pData   // pointer to SDO Request message
);
#endif

#if USE_SYNC
/**************************************************************************
DOES:    Processes reception of the SYNC message
RETURNS: 0: No messages processed
         Bit 0 set: SYNC TPDOs transmitted
         Bit 1 set: SYNC RPDOs received
**************************************************************************/
uint8_t MCOP_HandleSYNC(uint8_t len, // length of SYNC (0 or 1)
                        uint8_t scnt // counter value, if len = 1
);
#endif

/**************************************************************************
DOES:    Called when a TPDO needs to be transmitted
RETURNS: nothing
**************************************************************************/
void MCO_TransmitPDO(uint16_t PDONr // TPDO number to transmit
);

#if USE_INHIBIT_TIME
/**************************************************************************
DOES:    Called by application when a TPDO should be transmitted
RETURNS: nothing
**************************************************************************/
void MCO_TriggerTPDO(uint16_t TPDONr // TPDO number to transmit, as gTPDOConfig[] index
);
#endif

/**************************************************************************
DOES:    Checks if message received is guarding request
RETURNS: 0: message received is not guarding request
         1: guarding request received, response sent
**************************************************************************/
uint8_t MCOP_HandleGuarding(uint16_t can_id);

/**************************************************************************
DOES:    Function allowing application access to NMT changes
**************************************************************************/
void MCO_HandleNMTRequest(uint8_t NMTReq);

#if USE_STORE_PARAMETERS
/**************************************************************************
DOES:    Determines offset values for the the different regions
         in non-volatile memory.
         1. LSS + XOD binary eds checksum
         2. Save Parameters 1XXX
         3. Save Parameters 6XXX
         4. Save Parameters 2XXX
         5. First free space
RETURNS: Info on start offsets is copied into the string (5 values)
**************************************************************************/
void MCOSP_GetNVOLUsage(uint16_t pLoc[5] // info is written into this string
);

/**************************************************************************
DOES:    Checks the non-volatile memory for saved parameters and retrives
         them all
RETURNS: Nothing.
**************************************************************************/
void MCOSP_GetStoredParameters(void);

/**************************************************************************
DOES:    Implements the Store Parameters and Restore Parameters
         functionaility.
RETURNS: TRUE, if data was restored
         FALSE. if no valid data was found
**************************************************************************/
uint8_t MCOSP_StoreParameters(uint16_t idx, // set to 0x1010 for store, to 0x1011 for restore
                              uint8_t sub   // subindex
);

/**************************************************************************
DOES:    Initializes access to non-volatile memory.
**************************************************************************/
void NVOL_Init(void);

/**************************************************************************
DOES:    Reads a data byte from non-volatile memory.
         Must be provided by application driver.
NOTE:    The address is relative, an offset to NVOL_STORE_START
RETURNS: The data read from memory
**************************************************************************/
uint8_t NVOL_ReadByte(uint16_t address // location of byte in NVOL memory
);

/**************************************************************************
DOES:    Writes a data byte to non-volatile memory
         Must be provided by application driver.
NOTE:    The address is relative, an offset to NVOL_STORE_START
RETURNS: nothing
**************************************************************************/
void NVOL_WriteByte(uint16_t address, // location of byte in NVOL memory
                    uint8_t data);

/**************************************************************************
DOES:    Is called when a consecutive block of write cycles is complete.
         The driver may buffer the data from calls to NVOL_WriteByte with
         consecutive destination addresses in RAM and then write the entire
         buffer to non-volatile memory upon a call to this function.
**************************************************************************/
void NVOL_WriteComplete(void);
#endif // USE_STORE_PARAMETERS

#if USECB_ODSERIAL
/**************************************************************************
DOES:    Requests the device's serial number from the application.
         Must be passed in little endian format.
RETURNS: Serial number
**************************************************************************/
uint32_t MCOUSER_GetSerial(void);
#endif // USECB_ODSERIAL

#if USE_SLEEP
/**************************************************************************
DOES:    DRIVER LEVEL: Sets the processor into sleep or power down mode.
         Called when a sleep request was received and confirmed.
         Wakeup MUST be through a reset.
**************************************************************************/
void MCOHW_Sleep(void);

/*******************************************************************************
DOES:    APPLICATION LEVEL: Call back function for sleep/wakeup messages
         received as specified in CiA447 V2.
RETURNS: nothing
*******************************************************************************/
void MCOUSER_Sleep(uint8_t node,    // node ID of the node who sent the message
                   uint8_t command, // command byte of the message
                   uint8_t reason   // reason byte of the message
);

/*******************************************************************************
DOES:    Function to transmit wakeup/sleep messages as defined by CiA 447 V2
         SLEEP_MASTER_QUERY_OBJECTION 0x01
         SLEEP_SLAVE_SLEEP_OBJECTION 0x81
         SLEEP_MASTER_SET_SLEEP 0x02
         SLEEP_WAKEUP 0x82
         SLEEP_SLAVE_REQUEST 0x03
         SLEEP_REASON_NONE 0xFE
RETURNS: TRUE, if message was succesfully queued
*******************************************************************************/
uint8_t MCOP_TransmitWakeupSleep(uint8_t statcmd, uint8_t reason);
#endif // USE_SLEEP

#ifdef __cplusplus
}
#endif

#endif // _MCOP_H
/**************************************************************************
END OF FILE
**************************************************************************/
