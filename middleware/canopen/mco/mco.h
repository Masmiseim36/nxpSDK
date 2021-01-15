/**************************************************************************
MODULE:    MCO
CONTAINS:  Main MicroCANopen implementation
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
           $LastChangedDate: 2020-09-14 15:26:54 +0200 (Mon, 14 Sep 2020) $
           $LastChangedRevision: 5045 $
***************************************************************************/

#ifndef _MCO_H
#define _MCO_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
Version Information, here V7.10
**************************************************************************/
#define _MCOPVERSION_    7
#define _MCOSUBPVERSION_ 10

/**************************************************************************
CANopen FD Base Specification Information, here
CiA 1301 Part 0, Sub-part 0, Version 1, Sub-version 0, Class 0
**************************************************************************/
#define COFD_BASE_SPEC 0x51500100ul

// The following configuration files need to be supplied by the application
#include "nodecfg.h"
#include "mco_types.h"
#include "procimg.h"

/**************************************************************************
ENSURE THAT IN REGARDS TO OPTIMIZATION THE DEFAULT CONFIGURATION IS USED,
IF NOT SELECTED OTHERWISE in nodecfg.h
**************************************************************************/

#if (ENFORCE_DEFAULT_CONFIGURATION == 1)

// 1 or more TPDOs use the event timer
#define USE_EVENT_TIME 1
// 1 or more TPDOs are change-of-state and use the inhibit timer
#define USE_INHIBIT_TIME 1
// 1 or more PDOs use the SYNC signal
#define USE_SYNC 1
// 1 to allow PDO mapping to be configurable
#ifndef USE_DYNAMIC_PDO_MAPPING
#define USE_DYNAMIC_PDO_MAPPING 0
#endif
// Do not use static PDO comunication parameters
#define USE_STATIC_PDO 0
// Heartbeat consumer is dynamic / configurable
#define DYNAMIC_HB_CONSUMER 1
// Use MicroCANopen Plus functionality
#define USE_MCOP 1
// Produce emergencies when required
#define USE_EMCY 1
// Size of error field history [1003h]
#define ERROR_FIELD_SIZE 4
// Support segmented SDOs
#define USE_EXTENDED_SDO 1
// Do not use Node Guarding
#define USE_NODE_GUARDING 0
// If enabled, the call-back function MCOUSER_SDORequest() is called every
// time the CANopen stack receives an SDO Request that it cannot handle
#define USECB_SDOREQ 0
// If enabled, use extended call backs MCOUSER_XSDOInitWrite()
// and MCOUSER_XSDOWriteSegment() to enable custom segmented SDO handling
// NOTE: This requires a complete custom SDO handler!
#define USE_XSDOCB_WRITE 0
// Custom SDO channel handling
#define USE_CiA447  0
#define USE_SDOMESH 0

#endif

// Manager functionality, if not defined, disable
#ifndef MGR_MONITOR_ALL_NODES
#define MGR_MONITOR_ALL_NODES 0
#endif

/**************************************************************************
CANopen NMT (Network Management) Master Msg and Slave States
**************************************************************************/
#define NMTMSG_OP       1
#define NMTMSG_STOP     2
#define NMTMSG_PREOP    128
#define NMTMSG_RESETAPP 129
#define NMTMSG_RESETCOM 130

#define NMTSTATE_BOOT  0
#define NMTSTATE_STOP  4
#define NMTSTATE_OP    5
#define NMTSTATE_PREOP 127

/**************************************************************************
EMCY error codes used when calling MCOP_PushEMCY
**************************************************************************/
#define EMCY_NO_ERROR  0x0000 // No Error / Reset
#define EMCY_INTERN_SW 0x6100 // Internal software - generic (fatal error)
#define EMCY_DATA_SET  0x6300 // Data set - generic (fatal error)
#define EMCY_HB_ERR    0x8130 // Life guard error or heartbeat error
#define EMCY_PROT_ERR  0x8200 // Protocol error - generic
#define EMCY_PDO_LEN   0x8210 // PDO not processed due to length error
#define EMCY_SYNC_LEN  0x8240 // SYNC not processed due to length error
// EMCY active system bits
#define EMCYSBIT_PROT    (1 << 1) // Protocol
#define EMCYSBIT_SYNCLEN (1 << 2) // SYNC received has wrong lenth

/**************************************************************************
CANopen FD error status bits
**************************************************************************/
#define ERST_PRIO(prio) (prio & 0x07) // Priority, 0=highest, 7=lowest
#define ERST_CLASS_NREC 0x08          // Error class, 0=recoverable, 1=non-recoverable
#define ERST_STATE_OCC  0x10          // Error state, 0=removed, 1=occurred
#define ERST_STATE_MOCC 0x20          // Error occurred multiple times

/**************************************************************************
Error codes used when calling MCOUSER_FatalError
**************************************************************************/
#define ERR_WARN     0x4000 // Warning only, continue execution
#define ERROFL_EMCY  0x4810 // Transmit buffer overflow, TPDO message lost
#define ERROFL_PDO   0x4820 // Transmit buffer overflow, TPDO message lost
#define ERROFL_SDO   0x4830 // Transmit buffer overflow, SDO message lost
#define ERROFL_HBT   0x4840 // Transmit buffer overflow, Heartbeat message lost
#define ERR_FATAL    0x8000 // Fatal error, should abort/reset
#define ERRFT_INIT   0x8010 // MCOHW Init failed
#define ERRFT_PARA   0x8011 // MCO essential parameters error
#define ERRFT_RXFLTN 0x8021 // Out of CAN receive filters, NMT
#define ERRFT_RXFLTP 0x8022 // Out of CAN receive filters, PDO
#define ERRFT_RXFLTS 0x8023 // Out of CAN receive filters, SDO
#define ERRFT_IPP    0x8031 // Init PDO Parameters out of range
#define ERRFT_PIR    0x8032 // Process Image access out of range
#define ERRFT_TPDOR  0x8041 // Out of TPDOs
#define ERRFT_RPDOR  0x8042 // Out of RPDOs
#define ERRFT_RPMAP  0x8043 // Found no RPDO Mapping

/**************************************************************************
DEFINES FOR ACCESS TYPE TO OD ENRIES
Readable, Writable, Read-Mapable, Write-Mapable, Call-Back
**************************************************************************/
#define ODRD 0x10
#define ODWR 0x20
#define RMAP 0x40
#define WMAP 0x80
#define COFD 0x08 // flag for entry that is only used with CANopen FD

/**************************************************************************
MACROS FOR PLATFORM-ENDIANNESS-INDEPENDENT LITTLE-ENDIAN MEMORY ACCESS
**************************************************************************/
#define GEN_RD32(ptr)                                                                               \
    (((uint32_t)(*((uint8_t *)ptr + 3)) << 24) | ((uint32_t)(*((uint8_t *)ptr + 2) & 0xFF) << 16) | \
     ((uint32_t)(*((uint8_t *)ptr + 1) & 0xFF) << 8) | (uint32_t)(*((uint8_t *)ptr) & 0xFF))

#define GEN_RD24(ptr)                                                                                     \
    (((uint32_t)(*((uint8_t *)ptr + 2) & 0xFF) << 16) | ((uint32_t)(*((uint8_t *)ptr + 1) & 0xFF) << 8) | \
     (uint32_t)(*((uint8_t *)ptr) & 0xFF))

#define GEN_RD16(ptr) (((uint16_t)(*((uint8_t *)ptr + 1)) << 8) | (uint16_t)(*((uint8_t *)ptr) & 0xFF))

#define GEN_RD8(ptr) (*((uint8_t *)ptr) & 0xFF)

#define GEN_WR32(ptr, value)                                                 \
    do                                                                       \
    {                                                                        \
        *((uint8_t *)ptr + 3) = (uint8_t)(((uint32_t)(value) >> 24) & 0xFF); \
        *((uint8_t *)ptr + 2) = (uint8_t)(((uint32_t)(value) >> 16) & 0xFF); \
        *((uint8_t *)ptr + 1) = (uint8_t)(((uint32_t)(value) >> 8) & 0xFF);  \
        *((uint8_t *)ptr)     = (uint8_t)((uint32_t)(value)&0xFF);           \
    } while (0)

#define GEN_WR24(ptr, value)                                                 \
    do                                                                       \
    {                                                                        \
        *((uint8_t *)ptr + 2) = (uint8_t)(((uint32_t)(value) >> 16) & 0xFF); \
        *((uint8_t *)ptr + 1) = (uint8_t)(((uint32_t)(value) >> 8) & 0xFF);  \
        *((uint8_t *)ptr)     = (uint8_t)((uint32_t)(value)&0xFF);           \
    } while (0)

#define GEN_WR16(ptr, value)                                                \
    do                                                                      \
    {                                                                       \
        *((uint8_t *)ptr + 1) = (uint8_t)(((uint16_t)(value) >> 8) & 0xFF); \
        *((uint8_t *)ptr)     = (uint8_t)((uint16_t)(value)&0xFF);          \
    } while (0)

#define GEN_WR8(ptr, value)                          \
    do                                               \
    {                                                \
        *((uint8_t *)ptr) = (uint8_t)(value & 0xFF); \
    } while (0)

/**************************************************************************
MACROS FOR OBJECT DICTIONARY ENTRIES
**************************************************************************/
#define GETBYTE(val, pos) (((val) >> (pos)) & 0xFF)
#define GETBYTES16(val)   GETBYTE(val, 0), GETBYTE(val, 8)
#define GETBYTES32(val)   GETBYTE(val, 0), GETBYTE(val, 8), GETBYTE(val, 16), GETBYTE(val, 24)

#define SDOREPLY(index, sub, len, val)             0x43 | ((4 - (len)) << 2), GETBYTES16(index), sub, GETBYTES32(val)
#define SDOREPLY4(index, sub, len, d1, d2, d3, d4) 0x43 | ((4 - (len)) << 2), GETBYTES16(index), sub, d1, d2, d3, d4

#define ODENTRY(index, sub, len, offset)                \
    {                                                   \
        GETBYTES16(index), sub, len, GETBYTES16(offset) \
    }

#if USE_EXTENDED_SDO
#if USE_GENOD_PTR
#define ODGENTRYC(index, sub, acc, len, ptr)                         \
    {                                                                \
        GETBYTES16(index), sub, acc, GETBYTES16(len), (uint8_t *)ptr \
    }
#define ODGENTRYP(index, sub, acc, len, off)                           \
    {                                                                  \
        GETBYTES16(index), sub, acc, GETBYTES16(len), &(gProcImg[off]) \
    }
#else
#define ODGENTRYP(index, sub, acc, len, off)                          \
    {                                                                 \
        GETBYTES16(index), sub, acc, GETBYTES16(len), GETBYTES16(off) \
    }
#endif
#endif // USE_EXTENDED_SDO

// invalid process image offset
#define INVALID_PI_OFFSET 0xFFFF

/**************************************************************************
DO NOT USE THESE PIxxACC MACROS!
USE PI_READ AND PI_WRITE INSTEAD!
NEW VERSIONS SUPPORT RTOS, DATA CONSITENCY, SIMULATION
MACROS FOR PROCESS IMAGE ACCESS 8bit, 16bit and 32bit access
**************************************************************************/
/*
#define PI8ACC(offset) (gProcImg[offset])
#define PI16ACC(offset) (*((uint16_t *) &(gProcImg[offset])))
#define PI32ACC(offset) (*((uint32_t *) &(gProcImg[offset])))
*/

/**************************************************************************
CAN SETTINGS DEPENDING ON THE VERSION OF CAN USED
**************************************************************************/
// With CAN FD, there are up to 64 bytes per message, otherwise 8
#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
#define CAN_MAX_DATA_SIZE 64
#define EMCY_DATA_SIZE    20
#else
#define CAN_MAX_DATA_SIZE 8
#define EMCY_DATA_SIZE    8
#endif

/**************************************************************************
LIMITS FOR ESSENTIAL SETTINGS, PROJECT CAN OVERRIDE
**************************************************************************/
#if !defined(NODE_ID_MIN)
#define NODE_ID_MIN 1
#endif
#if !defined(NODE_ID_MAX)
#define NODE_ID_MAX 127
#endif

/**************************************************************************
MACROS FOR ACCESS TO INTERNAL VARIABLES SOMETIMES NEEDED BY APPLICATION
**************************************************************************/
#define MY_NODE_ID   (gMCOConfig.Node_ID)
#define MY_NMT_STATE (gMCOConfig.heartbeat_msg.BUF[0])

/**************************************************************************
MACROS FOR CAN-ID DEFINITION
**************************************************************************/

#if CAN_ID_SIZE == 16
typedef uint16_t COBID_TYPE;
#define COBID_DISABLED    0x8000U
#define COBID_RTR         0x4000U
#define COBID_EXT         0x2000U
#define COBID_OPT_MASK    0xE000U
#define COBID_PDO_DEFAULT 0x0000U
#elif CAN_ID_SIZE == 32
typedef uint32_t COBID_TYPE;
#define COBID_DISABLED    0x80000000UL
#define COBID_RTR         0x40000000UL
#define COBID_EXT         0x20000000UL
#define COBID_OPT_MASK    0xE0000000UL
#define COBID_PDO_DEFAULT 0x00000000UL
#else
#error Only CAN_ID_SIZE 16 or 32 is possible
#endif

// CAN-ID used for NMT Master message
#define NMT_MASTER_ID 0

// CAN-ID for LSS reception (slave) or transmission (master)
#define LSS_MASTER_ID 2021ul

// CAN-ID for LSS transmission (slave) or reception (master)
#define LSS_SLAVE_ID 2020ul

/**************************************************************************
MACROS FOR CAN-ID USAGE
**************************************************************************/
#define IS_CANID_LSS_REQUEST(canid) ((canid) == LSS_MASTER_ID)

#define IS_CANID_LSS_RESPONSE(canid) (((canid) == LSS_SLAVE_ID) || (((canid) >= 0x07D0) && ((canid) < 0x07E0)))

#define IS_CAN_ID_MY_HB(canid) ((canid) == 0x700 + MY_NODE_ID)

#define IS_CANID_RESTRICTED(canid)                                                               \
    (((canid) < 0x0080) || (((canid) > 0x0100) && ((canid) <= 0x0180)) ||                        \
     (((canid) > 0x0580) && ((canid) < 0x0600)) || (((canid) > 0x0600) && ((canid) < 0x0680)) || \
     (((canid) >= 0x06E0) && ((canid) < 0x0700)) || (((canid) > 0x0700) && ((canid) < 0x0800)))

#if (USE_CiA447 == 1)

#define IS_NODE_ID_VALID(nodeid) ((nodeid >= 2) && (nodeid <= 16))

#define IS_CAN_ID_GWHB(canid) ((canid) == 0x701)

// SDO Transfers: Server = j, Client = i
// Request from client node i to server node j
// (0x240 + ((i-1) & 0xC) << 6) + (((i-1) & 0x03) << 4) + j-1)
// Response from server node j to client node i
// (0x1C0 + ((i-1) & 0xC) << 6) + (((i-1) & 0x03) << 4) + j-1)
#define IS_CAN_ID_EMERGENCY(canid) (((canid) >= 0x081) && ((canid) <= 0x090))
#define IS_CAN_ID_HEARTBEAT(canid) (((canid) >= 0x701) && ((canid) <= 0x710))
#define IS_CAN_ID_ISO_TP(canid)                                                                                    \
    (((canid) == 0x251) || ((canid) == 0x262) || ((canid) == 0x273) || ((canid) == 0x344) || ((canid) == 0x355) || \
     ((canid) == 0x366) || ((canid) == 0x377) || ((canid) == 0x448) || ((canid) == 0x459) || ((canid) == 0x46A) || \
     ((canid) == 0x47B) || ((canid) == 0x54C) || ((canid) == 0x55D) || ((canid) == 0x56D) || ((canid) == 0x56E) || \
     ((canid) == 0x57F))

#define IS_CAN_ID_SDOREQUEST(canid)     \
    (((canid) == 0x600 + MY_NODE_ID) || \
     (((canid) > 0x0240) && ((canid) < 0x0580) && (((canid)&0x00CF) == (0x0040 + MY_NODE_ID - 1))))

#define IS_CAN_ID_SDOREQUEST_TX(canid)            \
    (((canid) >= 0x0240) && ((canid) < 0x0580) && \
     (((canid)&0x0730) == (((MY_NODE_ID - 1) & 0x0C) << 6) + (((MY_NODE_ID - 1) & 0x03) << 4)))

#define IS_CAN_ID_SDORESPONSE(canid)             \
    (((canid) > 0x01C0) && ((canid) < 0x04FF) && \
     (((canid)&0x07F0) == 0x1C0 + (((MY_NODE_ID - 1) & 0x0C) << 6) + (((MY_NODE_ID - 1) & 0x03) << 4)))

#define IS_CAN_ID_SDORESPONSE_TX(canid)                                                                             \
    (((canid) == 0x580 + MY_NODE_ID) || (((canid) > 0x01C0) && ((canid) < 0x04FF) && (((canid)&0x00C0) == 0x0C0) && \
                                         (((canid)&0x000F) == (MY_NODE_ID - 1))))

#define CAN_ID_SDORESPONSE_FROM_RXID(canid) ((canid)-0x80)
#define CAN_ID_SDOREQUEST(client_nodeid, server_nodeid) \
    (0x240 + (((client_nodeid - 1) & 0x0C) << 6) + (((client_nodeid - 1) & 0x03) << 4) + server_nodeid - 1)
#define CAN_ID_SDORESPONSE(client_nodeid, server_nodeid) \
    (0x1C0 + (((client_nodeid - 1) & 0x0C) << 6) + (((client_nodeid - 1) & 0x03) << 4) + server_nodeid - 1)
#define SDOSERVER(tx_canid) ( \
      ((tx_canid) == 0x580 + MY_NODE_ID) \
        ? (MY_NODE_ID-1) \
        : ((((tx_canid)-0x100) >> 6) & 0x0C) + (((tx_canid) >> 4) & 0x03)) \
    )

#define CAN_ID_PDO_CiA447(node) \
    (0x40000180ul + (((uint16_t)((node - 1) & 7) << 7)) + (((uint16_t)((node - 1) & 8)) << 2))

#define IS_NODE_CiA301_CLIENT() (MY_NODE_ID == 1)

#define IS_CAN_ID_MINE(canid)                                                                                    \
    (((canid) == 0x080 + MY_NODE_ID) || ((canid) == 0x580 + MY_NODE_ID) || (IS_CAN_ID_SDOREQUEST_TX((canid))) || \
     (IS_CAN_ID_SDORESPONSE_TX((canid))) || ((canid) == CAN_ID_PDO_CiA447(MY_NODE_ID)))

#elif (USE_SDOMESH == 1)

#define IS_NODE_ID_VALID(nodeid) ((nodeid >= 1) && (nodeid <= 16))

#define IS_CAN_ID_EMERGENCY(canid) (((canid) >= 0x081) && ((canid) <= 0x0FF))
#define IS_CAN_ID_HEARTBEAT(canid) (((canid) >= 0x701) && ((canid) <= 0x77F))

#define IS_CAN_ID_SDOREQUEST(canid) ((((canid)&0x700) == 0x600) && (((canid)&0x00F) == (MY_NODE_ID - 1)))
#define IS_CAN_ID_SDORESPONSE(canid)                                                                     \
    (((MY_NODE_ID < 9) && (((canid)&0x700) == 0x500) && (((canid)&0x0F0) == ((MY_NODE_ID + 7) << 4))) || \
     ((MY_NODE_ID > 8) && (((canid)&0x700) == 0x100) && (((canid)&0x0F0) == ((MY_NODE_ID - 9) << 4))))
#define CAN_ID_SDORESPONSE_FROM_RXID(canid)                                                         \
    (((((canid)&0x0F0) >> 4) < 8) ? (0x500 + (((((canid)&0x0F0) >> 4) + 8) << 4) + ((canid)&0xF)) : \
                                    (0x100 + (((((canid)&0x0F0) >> 4) - 8) << 4) + ((canid)&0xF)))
#define CAN_ID_SDOREQUEST(client_nodeid, server_nodeid) \
    (0x600 + (((client_nodeid - 1) & 0xF) << 4) + ((server_nodeid - 1) & 0xF))
#define CAN_ID_SDORESPONSE(client_nodeid, server_nodeid)                                                    \
    ((client_nodeid < 9) ? 0x500 + ((((client_nodeid - 1) & 0xF) + 8) << 4) + ((server_nodeid - 1) & 0xF) : \
                           0x100 + ((((client_nodeid - 1) & 0xF) - 8) << 4) + ((server_nodeid - 1) & 0xF))
#define SDOSERVER(tx_canid) \
    (((gTxSDO.ID & 0x700) == 0x500) ? (((gTxSDO.ID & 0x0F0) >> 4) - 8) : (((gTxSDO.ID & 0x0F0) >> 4) + 8))

#define IS_NODE_CiA301_CLIENT() (TRUE)

#else // standard CANopen

#define IS_NODE_ID_VALID(node_id)  ((node_id >= NODE_ID_MIN) && (node_id <= NODE_ID_MAX))
#define IS_CAN_ID_EMERGENCY(canid) (((canid) >= 0x081) && ((canid) <= 0x0FF))
#define IS_CAN_ID_HEARTBEAT(canid) (((canid) >= 0x701) && ((canid) <= 0x77F))

#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
#define IS_CAN_ID_SDOREQUEST(canid)                      (((canid) >= 0x601) && ((canid) <= 0x67F))
#define IS_CAN_ID_SDORESPONSE(canid)                     (((canid) >= 0x581) && ((canid) <= 0x5FF))
#define CAN_ID_SDORESPONSE_FROM_RXID(canid)              (0x580 + MY_NODE_ID)
#define CAN_ID_SDOREQUEST(client_nodeid, server_nodeid)  (0x600 + client_nodeid)
#define CAN_ID_SDORESPONSE(client_nodeid, server_nodeid) (0x580 + server_nodeid)
#define SDOSERVER(tx_canid)                              0
#else
#define IS_CAN_ID_SDOREQUEST(canid)                      ((canid) == 0x600 + MY_NODE_ID)
#define IS_CAN_ID_SDORESPONSE(canid)                     (((canid) >= 0x581) && ((canid) <= 0x5FF))
#define CAN_ID_SDORESPONSE_FROM_RXID(canid)              ((canid)-0x80)
#define CAN_ID_SDOREQUEST(client_nodeid, server_nodeid)  (0x600 + server_nodeid)
#define CAN_ID_SDORESPONSE(client_nodeid, server_nodeid) (0x580 + server_nodeid)
#define SDOSERVER(tx_canid)                              0
#endif

#if MGR_MONITOR_ALL_NODES == 1
#define IS_NODE_CiA301_CLIENT() (TRUE)
#else
#define IS_NODE_CiA301_CLIENT() (FALSE)
#endif

#endif

/**************************************************************************
SDO ABORT MESSAGES
**************************************************************************/
#define SDO_ABORT_TOGGLE          0x05030000UL
#define SDO_ABORT_UNKNOWN_COMMAND 0x05040001UL
#define SDO_ABORT_INVALID_SEQ     0x05040003UL
#define SDO_ABORT_CRC             0x05040004UL
#define SDO_ABORT_UNSUPPORTED     0x06010000UL
#define SDO_ABORT_WRITEONLY       0x06010001UL
#define SDO_ABORT_READONLY        0x06010002UL
#define SDO_ABORT_NOT_EXISTS      0x06020000UL
#define SDO_ABORT_PARAMETER       0x06040043UL
#define SDO_ABORT_TYPEMISMATCH    0x06070010UL
#define SDO_ABORT_DATATOBIG       0x06070012UL
#define SDO_ABORT_UNKNOWNSUB      0x06090011UL
#define SDO_ABORT_VALUE_RANGE     0x06090030UL
#define SDO_ABORT_VALUE_HIGH      0x06090031UL
#define SDO_ABORT_VALUE_LOW       0x06090032UL
#define SDO_ABORT_GENERAL         0x08000000UL
#define SDO_ABORT_TRANSFER        0x08000020UL
#define SDO_ABORT_NOTRANSFERCTRL  0x08000021UL
#define SDO_ABORT_NOTMAPPED       0x06040041UL
#define SDO_ABORT_MAPLENGTH       0x06040042UL
#define SDO_ABORT_TIMEOUT         0x05040000UL
#define SDO_ABORT_NOTRANSFER      0x08040020UL
#define SDO_ABORT_NOMEM           0x05040005UL

/**************************************************************************
Status bits for function MCOHW_GetStatus
**************************************************************************/
#define HW_INIT  0x01
#define HW_CERR  0x02
#define HW_ERPA  0x04
#define HW_RXOR  0x08
#define HW_TXOR  0x10
#define HW_TXBSY 0x40
#define HW_BOFF  0x80

/**************************************************************************
Defines for LED control
**************************************************************************/
#if USE_LEDS
// LED Flash Patterns
// Flickering directly implemented in lss.c
#define LED_OFF   0
#define LED_ON    0xFF
#define LED_BLINK 0x7F
// Single, double, triple and quadruple flashes
#define LED_FLASH1 1
#define LED_FLASH2 2
#define LED_FLASH3 3
#define LED_FLASH4 4
#endif // USE_LEDS

/**************************************************************************
Utility macro to print preprocessor define value with
#pragma message(VAR_NAME_VALUE(SOMEDEFINE))
**************************************************************************/
#define VALUE_TO_STRING(x)  #x
#define VALUE(x)            VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "=" VALUE(var)

/**************************************************************************
States for Heartbeat Consumer handling
**************************************************************************/
typedef enum
{
    HBCONS_OFF,    // Disabled
    HBCONS_INIT,   // Initialized, waiting for second heartbeat
    HBCONS_ACTIVE, // Consumer active and OK
    HBCONS_LOST,   // Heartbeat lost
    HBCONS_SLEEP   // Device in sleep
} HBCONS_STATE;

/**************************************************************************
GLOBAL TYPE DEFINITIONS
**************************************************************************/

// Data structure for a single CAN message
typedef struct
{                                   // order optimized for allignment
    uint8_t BUF[CAN_MAX_DATA_SIZE]; // Data buffer
    COBID_TYPE ID;                  // Message Identifier
    uint8_t LEN;                    // Data length (0-CAN_MAX_DATA_SIZE)
} CAN_MSG;

// This structure holds all node specific configuration
typedef struct
{
    CAN_MSG heartbeat_msg; // Heartbeat message contents
    uint16_t Baudrate;     // Current baudrate in kbit
#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
    uint16_t BRSBaudrate; // For CAN FD: Data (BRS) baudrate in kbit
#endif
    uint16_t heartbeat_time;      // Heartbeat time in ms
    uint16_t heartbeat_timestamp; // Timestamp of last heartbeat
    uint16_t last_fatal;          // Last Fatal Error code
    uint16_t last_rxtime;         // Timestamp of last CAN receive
#if USE_SYNC
#if USE_SYNC_PRODUCER
    uint32_t SYNC_cycle; // SYNC producer cycle time (OD 1006h)
#endif
    COBID_TYPE SYNC_id;  // CAN ID used for SYNC (OD 1005h)
    uint8_t SYNC_cntovr; // Synchronous counter overflow (OD 1019h) 0=off
#endif
#if NR_OF_TPDOS > 255
    uint16_t nrTPDOs;
#else
#if NR_OF_TPDOS > 0
    uint16_t nrTPDOs;
#endif
#endif
#if NR_OF_RPDOS > 255
    uint16_t nrRPDOs;
#else
#if NR_OF_RPDOS > 0
    uint16_t nrRPDOs;
#endif
#endif
#if USE_LEDS
    uint16_t LED_timestamp; // LED control timestamp
    uint8_t LEDtoggle;      // Toggler for blinking or flickering pattern
    uint8_t LEDRun;         // Current pattern on run led
    uint8_t LEDcntR;        // Current flash counter on run led
    uint8_t LEDErr;         // Current pattern on error led
    uint8_t LEDcntE;        // Current flash counter on error led
#endif                      // USE_LEDS
    uint8_t Node_ID;        // Current Node ID (1-126)
    uint8_t error_code;     // Bits: 0=RxQueue 1=TxQueue 3=CAN 7=first_op
    uint8_t error_register; // Error regiter for OD entry [1001,00]
#if USE_NODE_GUARDING
    uint8_t NGtoggle; // Toggle value for node guarding
#endif
    uint8_t HWStatus; // CAN HW status
} MCO_CONFIG;

// Enumeration type for inhibit status
typedef enum
{
    INHITIM_EXPIRED,            // Inhibit timer not started or expired
    INHITIM_RUNNING_NO_TRIGGER, // Inhibit timer started, not yet triggered for transmission
    INHITIM_RUNNING_TRIGGERED   // Transmit msg waiting for expiration of inhibit
} INHITIM_TYPE;

// This structure holds all the TPDO configuration data for one TPDO
typedef struct
{
    CAN_MSG CANmsg;  // Current/last CAN message to be transmitted
    uint16_t offset; // Offest to application data in process image
    uint16_t PDONr;  // PDO number (1-512)
#if USE_EVENT_TIME
    uint16_t event_time;      // Event timer in ms (0 for COS only operation)
    uint16_t event_timestamp; // If event timer is used, this is the
                              // timestamp for the next transmission
#endif
#if USE_INHIBIT_TIME
    uint16_t inhibit_time;       // Inhibit timer in ms (0 if COS not used)
    uint16_t inhibit_timestamp;  // If inhibit timer is used, this is the
                                 // timestamp for the next transmission
    INHITIM_TYPE inhibit_status; // Status of inhibit timer
#endif
#if USE_SYNC
    uint8_t SYNCcnt;   // SYNC counter for counting SYNC signals
    uint8_t SYNCmatch; // If SYNC with counter is used, trigger on this match
#endif
    uint8_t TType; // Transmission Type
} TPDO_CONFIG;

// This structure holds all the RPDO configuration data for one RPDO
typedef struct
{
#if USE_SYNC
    uint8_t BUF[8]; // SYNC data buffer
#endif
    COBID_TYPE CANID; // Message Identifier
    uint16_t offset;  // Pointer to destination of data
    uint16_t PDONr;   // PDO number (1-512)
    uint8_t len;      // Data length (0-8)
    uint8_t TType;    // Transmission Type
#if USECB_ODDATARECEIVED
    uint16_t map; // offset to RPDO mapping in SDOResponseTable
#endif            // USECB_ODDATARECEIVED
} RPDO_CONFIG;

/**************************************************************************
GLOBAL TYPES: Object Dictionary tables
**************************************************************************/
// This structure holds all data for one process data entry in the OD
typedef struct
{
    uint8_t idx_lo; // Index of OD entry
    uint8_t idx_hi;
    uint8_t subidx; // Subindex of OD entry
    uint8_t len;    // Data length in bytes (1-4), plus bits ODRD, ODWR, RMAP/WMAP
    uint8_t off_lo; // Offset to process data in process image
    uint8_t off_hi;
} OD_PROCESS_DATA_ENTRY;

#if USE_EXTENDED_SDO
// This structure holds all data for generic entries into the OD
typedef struct
{
    uint8_t idx_lo; // Index of OD entry
    uint8_t idx_hi;
    uint8_t subidx; // Subindex of OD entry
    uint8_t access; // Bits ODRD, ODWR
    uint8_t len_lo; // Length of data
    uint8_t len_hi;
#if USE_GENOD_PTR
    uint8_t *pDat;
#else
    uint8_t off_lo; // Offset to process data in process image
    uint8_t off_hi;
#endif
} OD_GENERIC_DATA_ENTRY;
#endif // USE_EXTENDED_SDO

#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)

// defines for OD entry location
typedef enum
{
    ODLOC_UNKNOWN,
    ODLOC_SDOREPLY,
    ODLOC_ODENTRY,
    ODLOC_ODGENTRY
} ODLocationType;

#endif // USE_CANOPEN_FD

#if (INDEX_FOR_DIAGNOSTICS != 0)
/**************************************************************************
GLOBAL TYPES: Diagnostic Data Record
**************************************************************************/
typedef struct
{                    // Subindexes provided at Index INDEX_FOR_DIAGNOSTICS
                     //  0: 22
                     //  1: Identification (hard coded)
                     //  2: Version (hard coded)
                     //  3: Functionailty (hard coded)
    uint32_t Status; //  4
#if (TXFIFOSIZE > 0)
    uint32_t TxFIFOStatus; //  5: from lo to hi byte: FIFO size, max use, ovrflow count, 81 on ovrflow
#endif
#if (RXFIFOSIZE > 0)
    uint32_t RxFIFOStatus; //  6: from lo to hi byte: FIFO size, max use, ovrflow count, 81 on ovrflow
#endif
#if (MGRFIFOSIZE > 0)
    uint32_t RxMgrFIFOStatus; //  7: from lo to hi byte: FIFO size, max use, ovrflow count, 81 on ovrflow
#endif
    uint16_t ProcTickPerSecCur; //  8
    uint16_t ProcTickPerSecMin; //  9
    uint16_t ProcTickPerSecMax; // 10
    uint16_t ProcTickBurstMax;  // 11
    uint16_t ProcRxPerSecCur;   // 12
    uint16_t ProcRxPerSecMin;   // 13
    uint16_t ProcRxPerSecMax;   // 14
                                // 15: Reserved
#if MGR_MONITOR_ALL_NODES
    uint16_t ProcMgrTickPerSecCur; // 16
    uint16_t ProcMgrTickPerSecMin; // 17
    uint16_t ProcMgrTickPerSecMax; // 18
    uint16_t ProcMgrTickBurstMax;  // 19
    uint16_t ProcMgrRxPerSecCur;   // 20
    uint16_t ProcMgrRxPerSecMin;   // 21
    uint16_t ProcMgrRxPerSecMax;   // 22
#endif
    // working variables
    uint16_t TickCnt;    // Counting calls to MCO_ProcessStackTick()
    uint16_t RxCnt;      // Counting calls to MCO_ProcessStackRx()
    uint16_t BurstCnt;   // Counting retrun values of TRUE for MCO_ProcessStackTick()
    uint16_t NextSecond; // Analyse diagnostics once per second
#if MGR_MONITOR_ALL_NODES
    uint16_t MgrTickCnt;
    uint16_t MgrRxCnt;
    uint16_t MgrBurstCnt;
    uint16_t MgrNextSecond;
#endif
} MCO_DIAGNOSTICS;
#endif

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
// this structure holds all node specific configuration
extern MCO_CONFIG MEM_FAR gMCOConfig;

#if (INDEX_FOR_DIAGNOSTICS != 0)
// MCO diagnostics record
extern MCO_DIAGNOSTICS MEM_FAR gMCODiag;
#endif

// Global timer/conter variable, incremented every millisecond
extern uint16_t volatile gTimCnt;

// The process image
extern uint8_t MEM_PROC gProcImg[];

// table with SDO Responses for read requests to OD - defined in user_xxx.c
extern uint8_t MEM_CONST gSDOResponseTable[];

// table with Object Dictionary entries to process Data - defined in user_xxx.c
extern OD_PROCESS_DATA_ENTRY MEM_CONST gODProcTable[];

#if USE_EXTENDED_SDO
// table with generic OD entries
extern OD_GENERIC_DATA_ENTRY MEM_CONST gODGenericTable[];
#endif

#if NR_OF_TPDOS > 0
// this structure holds all the TPDO configuration data
extern TPDO_CONFIG MEM_FAR gTPDOConfig[NR_OF_TPDOS];
#endif

#if NR_OF_RPDOS > 0
// this structure holds all the RPDO configuration data for up to 4 RPDOs
extern RPDO_CONFIG MEM_FAR gRPDOConfig[NR_OF_RPDOS];
#endif

/**************************************************************************
GLOBAL FUNCTIONS
**************************************************************************/

/**************************************************************************
DOES:    Initializes the MicroCANopen stack
         It must be called from within MCOUSER_ResetCommunication
RETURNS: TRUE, if init OK, else FALSE (also when unconfigured and in LSS)
**************************************************************************/
uint8_t MCO_Init(uint16_t Baudrate, // CAN baudrate in kbit(1000,800,500,250,125,50,25 or 10)
#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
                 uint16_t BRSBaudRate,
#endif
                 uint8_t Node_ID,   // CANopen node ID (1-126)
                 uint16_t Heartbeat // Heartbeat time in ms (0 for none)
);

/**************************************************************************
DOES:    (Re-)Initializes CANopen system entries by reading them from the
         Object Dictionary currently in use. Call after MCO_Init() or
         whenever loading/activating a new Object Dictionary.
         Uses globals gSDOResponseTable, gODProcTable, gProcImg
RETURNS: nothing
**************************************************************************/
void MCO_UpdateSystemFromOD(void);

/**************************************************************************
DOES:    This function initializes a transmit PDO. Once initialized, the
         MicroCANopen stack automatically handles transmitting the PDO.
         The application can directly change the data at any time.
NOTE:    For data consistency, the application should not write to the data
         while function MCO_ProcessStack executes.
RETURNS: nothing
**************************************************************************/
void MCO_InitTPDO(uint16_t PDO_NR,      // TPDO number (1-512)
                  uint32_t CAN_ID,      // CAN identifier to be used (set to 0 to use default)
                  uint16_t event_tim,   // Transmitted every event_tim ms
                                        // (set to 0 if ONLY inhibit_tim should be used)
                  uint16_t inhibit_tim, // Inhibit time in ms for change-of-state transmit
                                        // (set to 0 if ONLY event_tim should be used)
                  uint8_t len,          // Number of data bytes in TPDO
                  uint16_t offset       // Offset to data location in process image
);

/**************************************************************************
DOES:    This function initializes a transmit PDO. Once initialized, the
         MicroCANopen stack automatically handles transmitting the PDO.
         The application can directly change the data at any time.
NOTE:    For data consistency, the application should not write to the data
         while function MCO_ProcessStack executes.
         This is an extended version of MCO_InitTPDO() that includes the
         transmission type. MCO_InitTPDO() is still available for backward-
         compatibility.
RETURNS: nothing
**************************************************************************/
void MCO_InitTPDOFull(uint16_t PDO_NR,       // TPDO number (1-512)
                      uint32_t CAN_ID,       // CAN identifier to be used (set to 0 to use default)
                      uint16_t event_time,   // Transmitted every event_tim ms
                      uint16_t inhibit_time, // Inhibit time in ms for change-of-state transmit
                                             // (set to 0 if ONLY event_tim should be used)
                      uint8_t trans_type,    // Transmission type of the TPDO
                      uint8_t len,           // Number of data bytes in TPDO
                      uint16_t offset        // Offset to data location in process image
);

/**************************************************************************
DOES:    This function changes the COBID of a TPDO
RETURNS: nothing
**************************************************************************/
void MCO_ChangeTPDOID(uint16_t PDO_NR, // RPDO number (1-512)
                      uint32_t CAN_ID  // CAN identifier to be used
);

/**************************************************************************
DOES:    This function initializes a receive PDO. Once initialized, the
         MicroCANopen stack automatically updates the data at offset.
NOTE:    For data consistency, the application should not read the data
         while function MCO_ProcessStack executes.
RETURNS: nothing
**************************************************************************/
void MCO_InitRPDO(uint16_t PDO_NR, // RPDO number (1-512)
                  uint32_t CAN_ID, // CAN identifier to be used (set to 0 to use default)
                  uint8_t len,     // Number of data bytes in RPDO
                  uint16_t offset  // Offset to data location in process image
);

/**************************************************************************
DOES:    This function changes the COBID of a RPDO
RETURNS: nothing
**************************************************************************/
void MCO_ChangeRPDOID(uint16_t PDO_NR, // RPDO number (1-512)
                      uint32_t CAN_ID  // CAN identifier to be used
);

/**************************************************************************
DOES:    This function implements the main MicroCANopen protocol stack.
         It must be called frequently to ensure proper operation of the
         communication stack.
         Typically it is called from the while(1) loop in main.
RETURNS: 0 if nothing was done, 1 if a CAN message was sent or received
**************************************************************************/
uint8_t MCO_ProcessStack(void);

/**************************************************************************
         ALTERNATE PROCESS FUNCTION TO BE USED WITH RTOS INTEGRATION
DOES:    This function processes the next CAN message from the CAN receive
         queue. When using an RTOS, this can be turned into a task
         triggered by a CAN receive event.
RETURNS: FALSE, if no message was processed,
         TRUE, if a CAN message received was processed
**************************************************************************/
uint8_t MCO_ProcessStackRx(void);

/**************************************************************************
         ALTERNATE PROCESS FUNCTION TO BE USED WITH RTOS INTEGRATION
DOES:    This function executes all sub functions required to keep the
         CANopen stack operating. It should be called frequently. When used
         in an RTOS it should be called repeatedly every RTOS time tick
         until it returns zero.
RETURNS: FALSE, if there was nothing to process
         TRUE, if stack functions were executed
**************************************************************************/
uint8_t MCO_ProcessStackTick(void);

/**************************************************************************
USER CALL-BACK FUNCTIONS
These must be implemented by the application.
**************************************************************************/

/**************************************************************************
DOES:    Call-back function for reset application.
         Starts the watchdog and waits until watchdog causes a reset.
RETURNS: nothing
**************************************************************************/
void MCOUSER_ResetApplication(void);

/**************************************************************************
DOES:    This function both resets and initializes both the CAN interface
         and the CANopen protocol stack. It is called from within the
         CANopen protocol stack, if a NMT master message was received that
         demanded "Reset Communication".
         This function should call MCO_Init and MCO_InitTPDO/MCO_InitRPDO.
RETURNS: nothing
**************************************************************************/
void MCOUSER_ResetCommunication(void);

/**************************************************************************
DOES:    This function is called if a fatal error occurred.
         Error codes of mcohwxxx.c are in the range of 0x8000 to 0x87FF.
         Error codes of mco.c are in the range of 0x8800 to 0x8FFF.
         All other error codes may be used by the application.
RETURNS: nothing
**************************************************************************/
void MCOUSER_FatalError(uint16_t ErrCode // To debug, search source code for the ErrCode encountered
);

/**************************************************************************
OPTIONAL USER CALL-BACK FUNCTIONS
These must be implemented by the application, if enabled by nodecfg.h
Default implementation in user_callback.c
**************************************************************************/

/**************************************************************************
DOES:    Call-back function, heartbeat lost, timeout occured.
         Gets called when a heartbeat timeout occured for a node.
RETURNS: Nothing
**************************************************************************/
void MCOUSER_HeartbeatLost(uint8_t node_id);

#if USECB_TIMEOFDAY
/**************************************************************************
DOES:    This function is called if the message with the time object has
         been received.
RETURNS: nothing
**************************************************************************/
void MCOUSER_TimeOfDay(uint32_t millis, // Milliseconds since midnight
                       uint16_t days    // Number of days since January 1st, 1984
);
#endif // USECB_TIMEOFDAY

#if USECB_RPDORECEIVE
/**************************************************************************
DOES:    This function is called after an RPDO has been received and stored
         into the Process Image.
RETURNS: nothing
**************************************************************************/
void MCOUSER_RPDOReceived(uint16_t RPDONr, // RPDO Number
                          uint16_t offset, // Offset to RPDO data in Process Image
                          uint8_t len      // Length of RPDO
);
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
);
#endif // USECB_ODDATARECEIVED

#if USECB_TPDORDY
/**************************************************************************
DOES:    This function is called before a TPDO is sent. For triggering
         modes that are outside of the application's doing (Event Timer,
         SYNC), it is called before the send data is retrieved from the
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
);
#endif // USECB_TPDORDY

#if USECB_SYNCRECEIVE
/**************************************************************************
DOES:    This function is called with every SYNC message received.
         It allows the application to now apply all sync-triggered TPDO
         data to be applied to the application.
RETURNS: nothing
**************************************************************************/
void MCOUSER_SYNCReceived(void);

/**************************************************************************
DOES:    This function is called with every SYNC message received.
         VERSION for SYNC messages WITH counter value.
         It allows the application to now apply all sync-triggered TPDO
         data to be applied to the application.
RETURNS: nothing
**************************************************************************/
void MCOUSER_SYNCCNTReceived(uint8_t counter_value);
#endif

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
);
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
);
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
);
#endif // USECB_SDO_WR_PI

#if USECB_SDO_WR_AFTER
/**************************************************************************
DOES:    This function is called after an SDO write request was executed.
         Data is now in the process image and can be processed.
RETURNS: Nothing
**************************************************************************/

void MCOUSER_SDOWrAft(uint8_t client_nid, // node ID from where the request came (0 if unknown)
                      uint16_t index,     // Index of Object Dictionary entry
                      uint8_t subindex,   // Subindex of Object Dictionary entry
                      uint16_t offset,    // Offset to data in process image
                      uint16_t len        // Length of data
);
#endif // USECB_SDO_WR_AFTER

#if USECB_APPSDO_READ
/*******************************************************************************
DOES:    Call Back function to allow implementation of custom, application
         specific OD Read entries
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
);

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
);
#endif // USECB_APPSDO_READ

#if USECB_APPSDO_WRITE
/*******************************************************************************
DOES:    Call Back function to allow implementation of custom, application
         specific OD Write entries
RETURNS: 0x00 - OD entry not handled by this function
         0x01 - OD entry handled by this function
         0x04 - Abort with "attempting to write a read-only object"
         0x06 - Abort with "entry does not exist"
         0x08 - Abort with "data typ doesn't match" (CANopen FD only)
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
);

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
);
#endif // USECB_APPSDO_WRITE

#if USE_XSDOCB_WRITE
/**************************************************************************
DOES:    This function is called before a segmented SDO write access is
         made. The application can use this to implement custom, segmented
       SDO write transfers.
RETURNS: TRUE, if this access is supported by the application
**************************************************************************/
uint8_t MCOUSER_XSDOInitWrite(uint16_t index,
                              uint8_t subindex,
                              uint32_t size // size in bytes
);

/*******************************************************************************
DOES:    After an access was approved by the application by the
         MCOUSER_XSDOInitWrite function, this function is called with each
         new segment received.
RETURNS: TRUE if no error occured and the data was processed
         FALSE if a major error occured and the transfer needs to be aborted
*******************************************************************************/
uint8_t MCOUSER_XSDOWriteSegment(uint8_t last, // Is set to 1 if this is the last segment
                                 uint8_t len,  // length of segment (0-7)
                                 uint8_t *pDat // pointer to 'len' data bytes
);
#endif // USE_XSDOCB_WRITE

#ifdef USECB_RX_UNKNOWN
/*******************************************************************************
DOES:    CAN message received that was not processed by Micro CANpopen.
         (unknown message)
RETURNS: TRUE if message was processed
         FALSE if message was ignored
*******************************************************************************/
uint8_t MCOUSER_UnknownReceived(CAN_MSG *pRxCAN // Pointer to CAN message received
);
#endif

#if USE_DYNAMIC_PDO_MAPPING
/**************************************************************************
DOES:     RESETS SINGLE PDO MAPPING ENTRY TO DEFAULT
RETURNS:  TRUE, if PDO found and reset,
          FALSE, if PDO not implemented
GLOBALS:  Sets gRPDOmap and gTPDOmap to values defined in gSDOResponseTable
***************************************************************************/
uint8_t XPDO_ResetPDOMapEntry(uint8_t TxRx,  // Set to 0 for TPDO, 1 for RPDO
                              uint16_t PDONr // Number of PDO, 1 to 512
);
#endif

#if defined(USECB_EMCY) && USECB_EMCY
/**************************************************************************
DOES:    Process pending or clearing Emergency events (set or release).
USE:     Use this to implement an active error list and to keep track
         of application specific error codes.
RETURNS: 0 - No objection from application to transmit EMCY message
         !=0  - Application requests, that EMCY message is NOT generated
**************************************************************************/
uint8_t MCOUSER_EMCY(uint8_t ev_clr,     // set to TRUE if this is to clear a previous EMCY event
                     uint16_t emcy_code, // 16 bit error code
                     uint8_t em_1,       // 5 byte manufacturer specific error code
                     uint8_t em_2,
                     uint8_t em_3,
                     uint8_t em_4,
                     uint8_t em_5
#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
                     ,
                     uint8_t dev_num,   // logical device number
                     uint16_t spec_num, // CiA specification number
                     uint8_t status,    // status
                     uint32_t time_lo,  // timestamp bits 0-31
                     uint16_t time_hi   // timestamp bits 32-47
#endif                                  // USE_CANOPEN_FD
);
#endif // USECB_EMCY

/*******************************************************************************
DOES:    Internal functions of MCOP that get used in multiple modules
         Search tables with OD definitions for a specific index/subindex
RETURNS: 0xFFFF if not found, else offset in tabel
*******************************************************************************/
uint16_t MCO_SearchOD(uint16_t index,  // Index of OD entry searched
                      uint8_t subindex // Subindex of OD entry searched
);

uint16_t MCO_SearchODProcTable(uint16_t index,  // Index of OD entry searched
                               uint8_t subindex // Subindex of OD entry searched
);

uint8_t XSDO_SearchODGenTable(uint16_t index,   // Index of OD entry searched
                              uint8_t subindex, // Subindex of OD entry searched
                              uint8_t *access,
                              uint32_t *len,
                              uint8_t **pDat);

/**************************************************************************
DOES:    Internal function, checks if this OD entry is a system entry and
         if it is applies the data to the system variable(s)
RETURNS: 0xFFFFFFFF if access success, else SDO abort code
**************************************************************************/
uint32_t MCO_ApplySystemEntry(uint16_t index, uint8_t subindex, uint32_t dat);

#if USE_CiA447
/**************************************************************************
DOES:    Advanced call-back function for expedited SDO write accesses made.
         Version that includes node ID number of node making the request
RETURNS: nothing
**************************************************************************/
void MCOUSER_NodeSpecificSDOWrite(uint8_t node, // node id of node that made the request (if known, else zero)
                                  uint16_t idx,
                                  uint8_t sub,
                                  uint16_t offset,
                                  uint8_t len);

/**************************************************************************
DOES:    Working on CiA447 background tasks and timeouts
RETURNS: nothing
**************************************************************************/
void PROFILE_Process447(void);
void CiA447_Process(void);

#endif

/**************************************************************************
Plausability check for settings
**************************************************************************/

#if (NR_OF_TPDOS > 0)
#if !USE_EVENT_TIME
#if !USE_INHIBIT_TIME
#error At least one, USE_EVENT_TIME or USE_INHIBIT_TIME must be defined!
#endif
#endif
#endif

#if USE_STORE_PARAMETERS
#if !USE_EVENT_TIME
#error When using Save Parameters, USE_EVENT_TIME must be used, too
#endif
#if !USE_INHIBIT_TIME
#error When using Save Parameters, USE_INHIBIT_TIME must be used, too
#endif
#endif

#if ((NR_OF_TPDOS > 512) || (NR_OF_RPDOS > 512))
#error Illegal number of PDOs
#endif

#if ERROR_FIELD_SIZE > 253
#error Illegal size of error field
#endif

#if USE_XOD_ACCESS == 1
#if USE_DYNAMIC_PDO_MAPPING == 1
#error USE_XOD_ACCESS can not be combined with USE_DYNAMIC_PDO_MAPPING
#endif
#if USE_GENOD_PTR == 1
#error USE_XOD_ACCESS can not be combined with USE_GENOD_PTR
#endif
#if CAN_ID_SIZE != 32
#error USE_XOD_ACCESS requires CAN_ID_SIZE to be 32
#endif
#endif

#if USE_29BIT_LSSFEEDBACK == 1
#if CAN_ID_SIZE != 32
#error USE_29BIT_LSSFEEDBACK requires CAN_ID_SIZE to be 32
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif // _MCO_H
/**************************************************************************
END OF FILE
**************************************************************************/
