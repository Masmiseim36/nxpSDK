/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017,2024 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the private header file for the FSCI communication module.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#ifndef _FSCI_COMMUNICATION_H_
#define _FSCI_COMMUNICATION_H_

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_component_serial_manager.h"
#include "fsl_component_timer_manager.h"
#include "FsciInterface.h"

#include "fsl_os_abstraction.h"

#if defined gFsciRxAck_c && (gFsciRxAck_c != 0)
#ifndef gFsciRxAckTimeoutUseTmr_c
#define gFsciRxAckTimeoutUseTmr_c (USE_RTOS)
#endif
#endif

#ifndef mFsciRxTimeoutUsePolling_c
#define mFsciRxTimeoutUsePolling_c 0
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum
{
    PACKET_IS_VALID,
    PACKET_IS_TO_SHORT,
    FRAMING_ERROR,
    INTERNAL_ERROR
} fsci_packetStatus_t;

typedef struct fsciComm_tag
{
    clientPacket_t   *pPacketFromClient;
    clientPacketHdr_t pktHeader;
    uint16_t          bytesReceived;
#if gFsciHostSupport_c
    OSA_MUTEX_HANDLE_DEFINE(syncHostMutexId);
#endif
#if gFsciRxAck_c
    OSA_MUTEX_HANDLE_DEFINE(syncTxRxAckMutexId);
#if gFsciRxAckTimeoutUseTmr_c
    TIMER_MANAGER_HANDLE_DEFINE(ackWaitTmr);
#endif
    uint8_t         txRetryCnt;
    volatile bool_t ackReceived;
    volatile bool_t ackWaitOngoing;
#endif
#if gFsciRxTimeout_c
#if mFsciRxTimeoutUsePolling_c
    uint64_t lastRxByteTs;
#else
    timer_handle_t rxRestartTmr;
#endif
    bool_t rxOngoing;
#endif
} fsciComm_t;

typedef enum
{
    gFSCIHost_RspReady_c = (1 << 0),
} fsciHostEventType_t;

#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
typedef enum
{
    gFSCI_ClientPacketReady_c = (1 << 0),
} fsciEventType_t;
#endif

typedef struct fsci_serial_manager_tx
{
    uint32_t fsciSerialWriteHandle[((SERIAL_MANAGER_WRITE_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
    uint8_t *txBuffer;
} fsci_serial_manager_tx_t;

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern serial_handle_t gFsciSerialInterfaces[];
extern uint32_t        gFsciSerialWriteHandle[]
                                      [((SERIAL_MANAGER_WRITE_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
extern bool_t gFsciTxDisable;

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define gFSCI_StartMarker_c 0x02U
#define gFSCI_EndMarker_c   0x03U
#define gFSCI_EscapeChar_c  0x7FU

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void     FSCI_commInit(serial_handle_t *pSerCfg);
void     FSCI_receivePacket(void *param);
uint8_t  FSCI_GetVirtualInterface(uint32_t fsciInterface);
uint32_t FSCI_encodeEscapeSeq(const uint8_t *pDataIn, uint32_t len, uint8_t *pDataOut);
void     FSCI_decodeEscapeSeq(uint8_t *pData, uint32_t len);
uint8_t  FSCI_computeChecksum(const void *pBuffer, uint16_t size);

#if gFsciHostSupport_c
void FSCI_HostSyncLock(uint32_t fsciInstance, opGroup_t OG, opCode_t OC);
void FSCI_HostSyncUnlock(uint32_t fsciInstance);
#endif

#endif /* _FSCI_COMMUNICATION_H_ */
