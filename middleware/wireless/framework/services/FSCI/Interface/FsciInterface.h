/**********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018,2021,2024 NXP
 * All rights reserved.
 *
 *
 * This file is the interface file for the FSCI module
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef FSCI_INTERFACE_H
#define FSCI_INTERFACE_H

/*!
 * @addtogroup FSCI
 * The Framework Serial Communication Interface module
 *
 * INFORMATION
 *
 * The Framework Serial Communication Interface (FSCI) is both a software module and
 * a protocol that allows monitoring and extensive testing of the protocol layers.
 * It also allows separation of the protocol stack between two protocol layers in a two
 * processing entities setup, the host processor (typically running the upper layers of
 * a protocol stack) and the Black Box application (typically containing the lower layers
 * of the stack, serving as a modem). The Test Tool software is an example of a host
 * processor, which can interact with FSCI Black Boxes at various layers. In this setup,
 * the user can run numerous commands to test the Black Box application services and
 * interfaces.
 *
 * The FSCI enables common service features for each device enables monitoring of specific
 * interfaces and API calls. Additionally, the FSCI injects or calls specific events and
 * commands into the interfaces between layers.
 *
 * An entity which needs to be interfaced to the FSCI module can use the API to register
 * opcodes to specific interfaces. After doing so, any packet coming from that interface
 * with the same opcode triggers a callback execution. Two or more entities cannot
 * register the same opcode on the same interface, but they can do so on different
 * interfaces. For example, two MAC instances can register the same opcodes, one over
 * UARTA, and the other over UARTB. This way, Test Tool can communicate with each MAC
 * layer over two UART interfaces.
 *
 * The FSCI module executes in the context of the Serial Manager task.
 *
 * @{
 */

/**********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"

/**********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/*
 * FSCI Configuration
 */
#if !defined(gFsciIncluded_c)
#define gFsciIncluded_c 0 /* Enable/Disable FSCI module */
#endif

#ifndef gFsciUseDedicatedTask_c
#define gFsciUseDedicatedTask_c 1
#endif

#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
/* Fsci Task specification */
#ifndef gFsciTaskStackSize_c
#define gFsciTaskStackSize_c (1024) /* bytes */
#endif
#ifndef gFsciTaskPriority_c
#define gFsciTaskPriority_c (3)
#endif

#endif /* gFsciUseDedicatedTask_c */

#ifndef gFsciMaxOpGroups_c
#define gFsciMaxOpGroups_c 9
#endif

#ifndef gFsciMaxInterfaces_c
#define gFsciMaxInterfaces_c 1
#endif

#ifndef gFsciMaxVirtualInterfaces_c
#define gFsciMaxVirtualInterfaces_c 0
#endif

#ifndef gFsciMaxPayloadLen_c
#define gFsciMaxPayloadLen_c 245 /* bytes */
#endif

#ifndef gFsciLenHas2Bytes_c
#define gFsciLenHas2Bytes_c 0 /* boolean */
#endif

#ifndef gFsciUseEscapeSeq_c
#define gFsciUseEscapeSeq_c 0 /* boolean */
#endif

#ifndef gFsciUseFmtLog_c
#define gFsciUseFmtLog_c 0 /* boolean */
#endif

#ifndef gFsciUseFileDataLog_c
#define gFsciUseFileDataLog_c 0 /* boolean */
#endif

#ifndef gFsciTimestampSize_c
#define gFsciTimestampSize_c 0 /* bytes */
#endif

#ifndef gFsciLoggingInterface_c
#define gFsciLoggingInterface_c 0 /* [0..gFsciMaxInterfaces_c) */
#endif

#ifndef gFsciHostSupport_c
#define gFsciHostSupport_c 0 /* boolean */
#endif

#ifndef gFsciHostSyncUseEvent_c
#define gFsciHostSyncUseEvent_c 0 /* boolean */
#endif

#ifndef gFsciTxAck_c
#define gFsciTxAck_c 0 /* boolean */
#endif

#ifndef gFsciRxAck_c
#define gFsciRxAck_c 0 /* boolean */
#endif

#ifndef gFsciRxTimeout_c
#define gFsciRxTimeout_c 1 /* boolean */
#endif

/* Use RPMSG instead of Serial Manager for FSCI communication on the application core */
#ifndef gFsciOverRpmsgBridge_c
#define gFsciOverRpmsgBridge_c 0 /* boolean */
#endif

/* Use RPMSG instead of Serial Manager for FSCI communication on the NBU */
#ifndef gFsciOverRpmsg_c
#define gFsciOverRpmsg_c 0 /* boolean */
#endif

#define mFsciInvalidInterface_c (0xFF)

/* Used for maintaining backward compatibility */
#define gFSCI_McpsSapId_c  1
#define gFSCI_MlmeSapId_c  2
#define gFSCI_AspSapId_c   3
#define gFSCI_NldeSapId_c  4
#define gFSCI_NlmeSapId_c  5
#define gFSCI_AspdeSapId_c 6
#define gFSCI_AfdeSapId_c  7
#define gFSCI_ApsmeSapId_c 8
#define gFSCI_ZdpSapId_c   9

/*
 * The Test Tool uses an Opcode Group to specify which SAP Handler a packet
 * in intended for or received from.
 */
#define gFSCI_LoggingOpcodeGroup_c 0xB0  /* FSCI data logging utility    */
#define gFSCI_ReqOpcodeGroup_c     0xA3  /* FSCI utility Requests         */
#define gFSCI_CnfOpcodeGroup_c     0xA4U /* FSCI utility Confirmations/Indications    */
#define gFSCI_ReservedOpGroup_c    0x52

/* Additional bytes added by FSCI to a packet */
#if (gFsciMaxVirtualInterfaces_c > 0)
#define gFsci_TailBytes_c (2U)
#else
#define gFsci_TailBytes_c (1U)
#endif

/**********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/*!
 * \brief  Data type definition for FSCI response status code
 */
typedef uint8_t clientPacketStatus_t;

/*!
 * \brief  Data type definition for the operation group
 */
typedef uint8_t opGroup_t;

/*!
 * \brief  Data type definition for the operation code
 */
typedef uint8_t opCode_t;

/*!
 * \brief  Data type definition for the payload length
 */
#if gFsciLenHas2Bytes_c
typedef uint16_t fsciLen_t;
#else
typedef uint8_t fsciLen_t;
#endif

/*!
 * \brief  Enumerated data type definition for FSCI status codes.
 */
typedef enum
{
    gFsciSuccess_c               = 0x00, /*!< operation successful */
    gFsciSAPHook_c               = 0xEF, /*!< SAP operates in Hook mode */
    gFsciSAPDisabled_c           = 0xF0, /*!< SAP disabled */
    gFsciSAPInfoNotFound_c       = 0xF1, /*!< SAP information not found*/
    gFsciUnknownPIB_c            = 0xF2, /*!< unknown PIB */
    gFsciAppMsgTooBig_c          = 0xF3, /*!< message too long */
    gFsciOutOfMessages_c         = 0xF4, /*!< out of memory */
    gFsciEndPointTableIsFull_c   = 0xF5, /*!< table is full*/
    gFsciEndPointNotFound_c      = 0xF6, /*!< end point not found */
    gFsciUnknownOpcodeGroup_c    = 0xF7, /*!< unknown opCode/opGroup */
    gFsciOpcodeGroupIsDisabled_c = 0xF8, /*!< opCode/opGroup disabled  */
    gFsciDebugPrintFailed_c      = 0xF9, /*!< debug print failure */
    gFsciReadOnly_c              = 0xFA, /*!< read only  */
    gFsciUnknownIBIdentifier_c   = 0xFB, /*!<  unknown IB identifier */
    gFsciRequestIsDisabled_c     = 0xFC, /*!< request disabled */
    gFsciUnknownOpcode_c         = 0xFD, /*!< unknown opCode */
    gFsciTooBig_c                = 0xFE, /*!< too big */
    gFsciError_c                 = 0xFF  /*!< general catchall error */
} gFsciStatus_t;

/*!
 * \brief Data type definition for format of packet header exchanged between the external client and FSCI
 */
typedef PACKED_STRUCT clientPacketHdr_tag
{
    uint8_t   startMarker; /*!< start marker */
    opGroup_t opGroup;     /*!< operation group */
    opCode_t  opCode;      /*!< operation code */
    fsciLen_t len;         /*!< actual length of payload[] */
}
clientPacketHdr_t;

/*!
 * \brief Data type definition for format of packets exchanged between the external client and FSCI.
 *
 * \details The terminal checksum is actually stored at payload[len].
 *          The additional gFsci_TailBytes_c bytes insures that there is
 *          always space for it, even if the payload is full
 */
typedef PACKED_STRUCT clientPacketStructured_tag
{
    clientPacketHdr_t header;                                            /*!<  header */
    uint8_t           payload[gFsciMaxPayloadLen_c + gFsci_TailBytes_c]; /*!<  payload */
}
clientPacketStructured_t;

/*!
 * \brief Data type definition for format of working packets exchanged between the external client and FSCI.
 */
typedef PACKED_UNION clientPacket_tag
{
    uint8_t                  raw[sizeof(clientPacketStructured_t)]; /*!<  entire packet as unformatted data */
    clientPacketStructured_t structured;                            /*!<  packet as header + payload */
    PACKED_STRUCT                    /*!<  minimal packet with only a status value as payload */
    {
        clientPacketHdr_t    header; /*!<  header */
        clientPacketStatus_t status; /*!<  only a status as payload */
    }
    headerAndStatus;
}
clientPacket_t;

/*!
 * \brief  Data type definition for the FSCI OpGroup operating mode
 */
typedef enum
{
    gFsciDisableMode_c = 0,   /*!<  SAP disabled */
    gFsciHookMode_c    = 1,   /*!<  Hook mode */
    gFsciMonitorMode_c = 2,   /*!<  monitor mode */
    gFsciInvalidMode   = 0xFF /*!<  invalid mode */
} gFsciMode_t;

/*!
 * \brief Data type definition for the message handler function
 */
typedef void (*pfMsgHandler_t)(void *pData, void *param, uint32_t fsciInterface);

typedef void (*pfFSCI_Send_t)(uint8_t *pPacket, uint16_t packetLen, bool_t freePacket);

/*!
 * \brief Data type definition for the message handler function
 */
typedef gFsciStatus_t (*pfMonitor_t)(opGroup_t opGroup, void *pData, void *param, uint32_t fsciInterface);

/* defines the Operation Group table entry */

/*!
 * \struct gFsciOpGroup_t
 * \brief Data type definition for the Operation Group table entry
 */
typedef struct gFsciOpGroup_tag
{
    pfMsgHandler_t pfOpGroupHandler; /*!<  opGroup handler*/
    void          *param;            /*!<  handler parameter */
    opGroup_t      opGroup;          /*!<  operation group */
    gFsciMode_t    mode;             /*!<  FSCI mode*/
    uint8_t        fsciInterfaceId;  /*!<  FSCI interface Id*/
} gFsciOpGroup_t;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
#if gFsciIncluded_c
extern bool_t gFsciTxBlocking;
extern bool_t (*pfFSCI_OtaSupportCalback)(clientPacket_t *pPacket);
#endif

/**********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/*!*********************************************************************************
 * \brief Registers a message handler function for the specified Operation Group.
 *
 * \param[in] opGroup       the Operation Group
 * \param[in] mode          the operating mode
 * \param[in] pfHandler     pointer to a function that handles the received message
 * \param[in] param         pointer to a parameter that is provided inside the OG Handler function
 * \param[in] fsciInterface the interface ID on which the callback must be registered
 *
 * \return gFsciStatus_t: \n
 *         gFsciSuccess_c: if the operation was successful\n
 *         gFsciError_c  : if there is no more space in the table or the OG specified already exists
 ********************************************************************************* */
gFsciStatus_t FSCI_RegisterOpGroup(
    opGroup_t opGroup, gFsciMode_t mode, pfMsgHandler_t pfHandler, void *param, uint32_t fsciInterface);

/*!*********************************************************************************
 * \brief Monitoring SAPs
 *
 * \param[in] opGroup       the Operation Group
 * \param[in] pData         pointer to data location
 * \param[in] param         pointer to a parameter that is provided inside the OG Handler function
 * \param[in] fsciInterface interface on which the data must be printed
 *
 * \return gFsciStatus_t: status of the call process.
 ********************************************************************************* */
gFsciStatus_t FSCI_Monitor(opGroup_t opGroup, void *pData, void *param, uint32_t fsciInterface);

#if gFsciIncluded_c
/*!*********************************************************************************
 * \brief Sends a message to the FSCI module.
 *
 * \param[in] pPacket       pointer to the message payload
 * \param[in] fsciInterface interface on which the data was received
 *
 * \return gFsciStatus_t: status of the operation.
 ********************************************************************************* */
gFsciStatus_t FSCI_ProcessRxPkt(clientPacket_t *pPacket, uint32_t fsciInterface);

/*!*********************************************************************************
 * \brief Sends a packet over the serial interface after computing the checksum.
 *
 * \param[in] pPkt           pointer to the packet to be sent over the serial interface
 * \param[in] fsciInterface  interface on which the packet must be sent
 ********************************************************************************* */
void FSCI_transmitFormatedPacket(clientPacket_t *pPkt, uint32_t fsciInterface);

/*!*********************************************************************************
 * \brief Encodes and sends messages over the serial interface.
 *
 * \param[in] OG             operation group
 * \param[in] OC             operation code
 * \param[in] pMsg           pointer to payload
 * \param[in] msgLen         length of the payload
 * \param[in] fsciInterface  interface on which the packet must be sent
 ********************************************************************************* */
void FSCI_transmitPayload(uint8_t OG, uint8_t OC, const uint8_t *pMsg, uint16_t msgLen, uint32_t fsciInterface);

/*!*********************************************************************************
 * \brief Sends a packet over the serial interface with the specified error code.
 *
 * \details This function does not use dynamic memory. The packet is sent in blocking mode.
 *
 * \param[in] errorCode      FSCI error code to be transmitted
 * \param[in] fsciInterface  interface on which the packet must be sent
 ********************************************************************************* */
void FSCI_Error(uint8_t errorCode, uint32_t fsciInterface);

uint8_t *FSCI_GetFormattedPacket(uint8_t OG, uint8_t OC, void *pMsg, uint16_t msgLen, uint16_t *pOutLen);
#endif

#if gFsciTxAck_c
/*!*********************************************************************************
 * \brief Sends an ACK packet over the serial interface with an optional checksum identifier.
 *
 * \details This function does not use dynamic memory. The packet is sent in blocking mode.
 *          It is recommended to enable the ACK functionality through the gFsciTxAck_c macro definition.
 *
 * \param[in] checksum       packet identifier to be included in the ACK, currently optional
 * \param[in] fsciInterface  interface on which the packet must be sent
 ********************************************************************************* */
void FSCI_Ack(uint8_t checksum, uint32_t fsciInterface);
#endif

#if gFsciHostSupport_c
gFsciStatus_t FSCI_ResetReq(uint32_t fsciInterface);
#endif

/*!*********************************************************************************
 * \brief Sends bytes over the serial interface.
 *
 * \param[in] readyToSend  specifies whether the data should be transmitted asap
 * \param[in] pSrc         pointer to the data location
 * \param[in] len          size of the data
 ********************************************************************************* */
#if gFsciIncluded_c
void FSCI_Print(uint8_t readyToSend, void *pSrc, fsciLen_t len);
#endif

/*!*********************************************************************************
 * \brief Sends a formatted text string to the host.
 *
 * \param[in] fmt string and format specifiers to output to the data log
 * \param[in] ...
 ********************************************************************************* */
#if gFsciUseFmtLog_c
void FSCI_LogFormatedText(const char *fmt, ...);
#endif

/*!*********************************************************************************
 * \brief Sends binary data to a specific file.
 *
 * \param[in] fileName   name of the file in which the data is stored
 * \param[in] pData      pointer to the data to be written
 * \param[in] dataSize   size of the data to be written
 * \param[in] mode       mode in which the file is accessed
 ********************************************************************************* */
#if gFsciUseFileDataLog_c
void FSCI_LogToFile(char *fileName, uint8_t *pData, uint16_t dataSize, uint8_t mode);
#endif

/*!
 * @}  end of FSCI addtogroup
 */

#ifdef __cplusplus
}
#endif

#endif /* FSCI_INTERFACE_H */
