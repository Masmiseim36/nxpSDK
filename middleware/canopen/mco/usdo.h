/**************************************************************************
MODULE:    USDO
CONTAINS:  MicroCANopen Plus, Universal SDO implementation
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

#ifndef _USDO_H
#define _USDO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mco.h"

#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)

/**************************************************************************
DEFINITIONS
***************************************************************************/

// USDO COB-IDs from node IDs and vice versa
#define USDO_REQUEST_ID(clnid)          (0x600 + clnid)
#define USDO_RESPONSE_ID(srvnid)        (0x580 + srvnid)
#define USDO_CLNID_FROM_REQID(can_id)   (can_id & 0x7F)
#define USDO_SRVNID_FROM_RESPID(can_id) (can_id & 0x7F)

// USDO abort codes
#define USDO_ABORT_NONE              0x00 // No error/success
#define USDO_ABORT_NOTSET            0xFF // Pseudo code: No abort status set
#define USDO_ABORT_UNKNOWN           0x10 // Unknown error
#define USDO_ABORT_PROTOCOL          0x11 // USDO protocol error detected
#define USDO_ABORT_TIMEOUT           0x12 // USDO time out error
#define USDO_ABORT_UNKNOWN_COMMAND   0x13 // Unknown command specifier
#define USDO_ABORT_INVALID_SEQ       0x14 // Invalid value in counter byte (segmented protocol)
#define USDO_ABORT_OUT_OF_MEM        0x16 // Out of memory
#define USDO_ABORT_GEN_ROUTING       0x19 // USDO general routing error
#define USDO_ABORT_DEST_NET_UNKNOWN  0x1A // USDO destination network unknown
#define USDO_ABORT_DEST_NODE_UNKNOWN 0x1B // USDO destination node unknown
#define USDO_ABORT_SRC_NET_UNKNOWN   0x1C // USDO source network unknown
#define USDO_ABORT_SRC_NODE_UNKNOWN  0x1D // USDO source node unknown
#define USDO_ABORT_CAN_LEN           0x1E // Unexpected CAN DLC
#define USDO_ABORT_MULIPLX           0x1F // Unexpected multiplexer
#define USDO_ABORT_UNSUPPORTED       0x30 // Unsupported access to an object
#define USDO_ABORT_WRITEONLY         0x31 // Attempt to read a write only object
#define USDO_ABORT_READONLY          0x32 // Attempt to write a read only object
#define USDO_ABORT_NOT_EXISTS        0x33 // Object does not exist in the object dictionary
#define USDO_ABORT_UNKNOWNSUB        0x34 // Sub-index does not exist
#define USDO_ABORT_HARDWARE          0x35 // Access failed due to a hardware error
#define USDO_ABORT_LEN_MISMATCH      0x36 // Data type does not match, length of service parameter does not match
#define USDO_ABORT_LEN_TOOHIGH       0x37 // Data type does not match, length of service parameter too high
#define USDO_ABORT_LEN_TOOLOW        0x38 // Data type does not match, length of service parameter too low
#define USDO_ABORT_DL_PARAM_INVALID  0x40 // Invalid value for parameter (download only)
#define USDO_ABORT_DL_PARAM_TOOHIGH  0x41 // Value of parameter written too high (download only)
#define USDO_ABORT_DL_PARAM_TOOLOW   0x42 // Value of parameter written too low (download only)
#define USDO_ABORT_MAX_TOOLOW        0x43 // Maximum value is less than minimum value
#define USDO_ABORT_MIN_TOOHIGH       0x44 // Minimum value is higher than maximum value
#define USDO_ABORT_CAN_READONLY      0x45 // CAN-ID and CAN frame format are of access type read-only
#define USDO_ABORT_PDO_CANTMAP       0x50 // Object cannot be mapped to the PDO
#define USDO_ABORT_PDO_MAPEXCEED     0x51 // The number and length of the objects to be mapped would exceed PDO length
#define USDO_ABORT_PDO_NONMAP        0x52 // Attempt to map a non mappable parameter to a PDO
#define USDO_ABORT_RPDO_MAP_TPDO     0x53 // Attempt to map a parameter of PDO access type RPDO to a TPDO
#define USDO_ABORT_TPDO_MAP_RPDO     0x54 // Attempt to map a parameter of PDO access type TPDO toa RPDO
#define USDO_ABORT_PDOCOM_NOTDIS     0x55 // Attempt to write to PDO parameters, while PDO is valid
#define USDO_ABORT_PDOMAP_NOTDIS     0x56 // Attempt to write to PDO mapping parameters, while PDO mapping is valid
#define USDO_ABORT_NOTRANS           0x60 // Data cannot be transferred or stored to the application
#define USDO_ABORT_NOTRANS_LOCAL \
    0x61 // Data cannot be transferred or stored to the application because of local control
#define USDO_ABORT_NOTRANS_DEVICE \
    0x62 // Data cannot be transferred or stored to the application because of the present device state
#define USDO_ABORT_ODCRREATE 0x63 // Object dictionary dynamic generation fails or no object dictionary is present
#define USDO_ABORT_NODATA    0x64 // No data available
#define USDO_ABORT_BUSY      0x70 // USDO server busy

// USDO command specifiers
#define UCS_CCS_DWNEXP_REQ      0x01 // Download expedited request
#define UCS_CCS_DWNSEGINI_REQ   0x02 // Download segmented initial request
#define UCS_CSCS_DWNSEGINT_REQ  0x03 // Download segmented intermeditate request (segment)
#define UCS_CECS_DWNSEGEND_REQ  0x04 // Download segmented last request (end)
#define UCS_TBD_DWNMULSUB_REQ   0x05 // Download multiple sub-indices request
#define UCS_CCS_DWNBLKINI_REQ   0x06 // Download segmented block initial request
#define UCS_CSCS_DWNBLKINT_REQ  0x07 // Download segmented block intermeditate request (segment)
#define UCS_CECS_DWNBLKEND_REQ  0x08 // Download segmented block last request (end)
#define UCS_CCS_UP_REQ          0x11 // Upload request
#define UCS_CSCS_UPSEGINT_REQ   0x13 // Upload segmented intermediate request (segment)
#define UCS_TBD_UPMULSUB_REQ    0x15 // Upload multiple sub-indices request
#define UCS_SCS_DWNEXP_RESP     0x21 // Download expedited response
#define UCS_SCS_DWNSEGINI_RESP  0x22 // Download segmented initial response
#define UCS_SSCS_DWMSEGINT_RESP 0x23 // Download segmented intermeditate response (segment)
#define UCS_SECS_DWNSEGEND_RESP 0x24 // Download segmented last response (end)
#define UCS_SCS_DWNBLKINI_RESP  0x26 // Download segmented block initial response
#define UCS_SECS_DWNBLKEND_RESP 0x28 // Download segmented block last response (end)
#define UCS_TBD_DWNMULSUB_RESP  0x25 // Download multiple sub-indices response
#define UCS_SCS_UPEXP_RESP      0x31 // Upload expedited response
#define UCS_SCS_UPSEGINI_RESP   0x32 // Upload segmented initial response
#define UCS_SSCS_UPSEGINT_RESP  0x33 // Upload segmented intermeditate response (segment)
#define UCS_SECS_UPSEGEND_RESP  0x34 // Upload segmented last response (end)
#define UCS_TBD_UPMULSUB_RESP   0x35 // Upload multiple sub-indices response
#define UCS_CS_ABORT            0x7F // USDO abort service
#define UCS_REMOTE_MASK         0x80 // bit set for remote accesses

#define USDO_SIZE_UNKNOWN 0xFFFFFFFFUL // 'unknown' size

#define USDO_NID_BCAST 0 // node ID value indicating broadcast transfer

// Data types
#define TYPE_BOOLEAN                       0x01
#define TYPE_INTEGER8                      0x02
#define TYPE_INTEGER16                     0x03
#define TYPE_INTEGER32                     0x04
#define TYPE_UNSIGNED8                     0x05
#define TYPE_UNSIGNED16                    0x06
#define TYPE_UNSIGNED32                    0x07
#define TYPE_REAL32                        0x08
#define TYPE_VISIBLE_STRING                0x09
#define TYPE_OCTET_STRING                  0x0A
#define TYPE_UNICODE_STRING                0x0B
#define TYPE_TIME_OF_DAY                   0x0C
#define TYPE_TIME_DIFFERENCE               0x0D
#define TYPE_DOMAIN                        0x0F
#define TYPE_INTEGER24                     0x10
#define TYPE_REAL64                        0x11
#define TYPE_INTEGER40                     0x12
#define TYPE_INTEGER48                     0x13
#define TYPE_INTEGER56                     0x14
#define TYPE_INTEGER64                     0x15
#define TYPE_UNSIGNED24                    0x16
#define TYPE_UNSIGNED40                    0x18
#define TYPE_UNSIGNED48                    0x19
#define TYPE_UNSIGNED56                    0x1A
#define TYPE_UNSIGNED64                    0x1B
#define STRUCT_PDO_COMMUNICATION_PARAMETER 0x20
#define STRUCT_PDO_MAPPING_PARAMETER       0x21
#define STRUCT_IDENTITY                    0x23
#define STRUCT_OS_DEBUG                    0x24
#define STRUCT_OS_COMMAND                  0x25
#define STRUCT_ACTIVE_ERROR_HISTORY        0x26

/**************************************************************************
PUBLIC FUNCTIONS
***************************************************************************/

/**************************************************************************
DOES:    Handle an incoimg USDO request.
RETURNS: returns 1 if USDO access success, returns 0 if USDO abort generated
**************************************************************************/
uint8_t USDO_HandleUSDORequest(CAN_MSG *pRxUSDO // Pointer to USDO request
);

/**************************************************************************
DOES:    Handles incoming USDO Request for accesses to PDO Communication
         Parameters
RETURNS: 0: Wrong access, USDO Abort sent
         1: Access was made, USDO Response sent
GLOBALS: Various global variables with configuration information
**************************************************************************/
uint8_t USDO_HandlePDOComParam(uint8_t PDOType, // 0 for TPDO, 1 for RPDO
                               uint16_t index,  // OD index
                               uint8_t *pData,  // pointer to USDO Request message
                               CAN_MSG *pTxUSDO // Pointer to USDO response
);

#if USE_DYNAMIC_PDO_MAPPING
/**************************************************************************
DOES:    Handles incoming USDO Request for accesses to PDO Mapping
Parameters
RETURNS: 0: Wrong access, USDO Abort sent
1: Access was made, USDO Response sent
GLOBALS: Various global variables with configuration information
**************************************************************************/
uint8_t USDO_HandlePDOMapParam(uint8_t PDOType, // 0 for TPDO, 1 for RPDO
                               uint16_t index,  // OD index
                               uint8_t *pData,  // pointer to USDO request message
                               CAN_MSG *pTxUSDO // Pointer to USDO response
);
#endif

/**************************************************************************
DOES:    Called from ProcessStackTick
         Checks timed actions for UDSO servers
RETURNS: FALSE, nothing done
         TRUE, transfer in progress, message generated
**************************************************************************/
uint8_t USDO_CheckProgress(void);

#endif // USE_CANOPEN_FD

#ifdef __cplusplus
}
#endif

#endif // _USDO_H
/**************************************************************************
END OF FILE
**************************************************************************/
