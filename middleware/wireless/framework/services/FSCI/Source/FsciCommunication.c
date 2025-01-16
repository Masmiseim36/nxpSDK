/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017,2020 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is a source file for the FSCI communication.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include <stdbool.h>

#include "FsciCommunication.h"
#include "FsciInterface.h"
#include "FsciCommands.h"
#include "FunctionLib.h"

#include "fsl_component_mem_manager.h"
#include "fsl_component_timer_manager.h"

#include "fsl_os_abstraction.h"
#include <assert.h>

#if defined(gNvStorageIncluded_d) && (gNvStorageIncluded_d == 1)
#include "NVM_Interface.h"
#endif

#if (defined gFsciOverRpmsgBridge_c) && (gFsciOverRpmsgBridge_c == 1)
#include "fwk_platform_ble.h"
#endif

#if defined gFsciIncluded_c && (gFsciIncluded_c != 0)
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define gFsciUseBlockingTx_c 1
#define MIN_VALID_PACKET_LEN (sizeof(clientPacketHdr_t))
#define FSCI_txCallback      MEM_BufferFree

#ifndef mFsciRxAckTimeoutMs_c
#define mFsciRxAckTimeoutMs_c 100U /* milliseconds */
#endif

#ifndef mFsciTxRetryCnt_c
#define mFsciTxRetryCnt_c 4
#endif

#ifndef mFsciRxRestartTimeoutMs_c
#define mFsciRxRestartTimeoutMs_c 50u /* milliseconds */
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
static void FSCI_Task(osa_task_param_t argument);
#endif

fsci_packetStatus_t FSCI_checkPacket(clientPacket_t *pData, uint16_t bytes, uint8_t *pVIntf);

#if defined gFsciRxAckTimeoutUseTmr_c && (gFsciRxAckTimeoutUseTmr_c != 0)
static void FSCI_RxAckExpireCb(void *param);
#endif

#if defined gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
static void FSCI_RxRxTimeoutCb(void *param);
#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
#endif

#if defined mFsciRxTimeoutUsePolling_c && (mFsciRxTimeoutUsePolling_c != 0)
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
static void FSCI_CheckRxTimeoutPolling(void *param);
#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
#endif

static void FSCI_SendPacketToSerialManager(uint32_t fsciInterface, uint8_t *pPacket, uint16_t packetLen);
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
inline static void FSCI_rxCallback(void                              *pData,
                                   serial_manager_callback_message_t *message,
                                   serial_manager_status_t            status);
#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
typedef struct
{
    clientPacket_t *pFsciPacketToProcess;
    uint8_t         fsciInterface;
} fsciClientPacketInfo_t;
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
bool_t gFsciTxBlocking = FALSE;
bool_t gFsciTxDisable  = FALSE;

/* FSCI serial manager handle list */
serial_handle_t gFsciSerialInterfaces[gFsciMaxInterfaces_c];
/* FSCI serial manager read handle buffer */
uint32_t gFsciSerialReadHandle[gFsciMaxInterfaces_c]
                              [((SERIAL_MANAGER_READ_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
/* FSCI serial manager write handle buffer */
uint32_t gFsciSerialWriteHandle[gFsciMaxInterfaces_c]
                               [((SERIAL_MANAGER_WRITE_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];

#if (gFsciMaxVirtualInterfaces_c > 0)
/* Holds the virtual interface Id for the FSCI interface (index) */
static uint8_t gFsciVirtualInterfaces[gFsciMaxInterfaces_c];
#endif

#if defined gFsciHostSupport_c && (gFsciHostSupport_c != 0)
#if defined gFsciHostSyncUseEvent_c && (gFsciHostSyncUseEvent_c != 0) && !USE_RTOS
#undef gFsciHostSyncUseEvent_c
#endif

#if defined  gFsciHostSyncUseEvent_c && (gFsciHostSyncUseEvent_c != 0)
osaEventId_t gFsciHostSyncRspEventId;
#endif

clientPacket_t *pFsciHostSyncRsp        = NULL;
bool_t          gFsciHostWaitingSyncRsp = FALSE;
opGroup_t       gFsciHostWaitingOpGroup = 0;
opCode_t        gFsciHostWaitingOpCode  = 0;
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static fsciComm_t mFsciCommData[gFsciMaxInterfaces_c];

static uint8_t mFsciSrcInterface = mFsciInvalidInterface_c;

#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
static OSA_TASK_DEFINE(FSCI_Task, gFsciTaskPriority_c, 1, gFsciTaskStackSize_c, FALSE);
static OSA_TASK_HANDLE_DEFINE(gFsciTaskId);
static OSA_EVENT_HANDLE_DEFINE(mFsciTaskEventId);
static fsciClientPacketInfo_t mFsciClientPacketInfo;
#endif /* gFsciUseDedicatedTask_c */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#if defined gFsciHostSupport_c && (gFsciHostSupport_c != 0)
void        FSCI_HostSyncLock(uint32_t fsciInstance, opGroup_t OG, opCode_t OC)
{
    OSA_MutexLock(mFsciCommData[fsciInstance].syncHostMutexId, osaWaitForever_c);
    gFsciHostWaitingSyncRsp = TRUE;
    gFsciHostWaitingOpGroup = OG;
    gFsciHostWaitingOpCode  = OC;
    pFsciHostSyncRsp        = NULL;
}

void FSCI_HostSyncUnlock(uint32_t fsciInstance)
{
    gFsciHostWaitingSyncRsp = FALSE;
    pFsciHostSyncRsp        = NULL;
    OSA_MutexUnlock(mFsciCommData[fsciInstance].syncHostMutexId);
}
#endif /* gFsciHostSupport_c */

/*! *********************************************************************************
 * \brief  Initialize the serial interface.
 *
 * \param[in]  initStruct pointer to a gFsciSerialConfig_t structure
 *
 ********************************************************************************** */
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
void FSCI_commInit(serial_handle_t *pSerCfg)
{
    int      ret = kStatus_Success;
    uint32_t i;
#if (gFsciMaxVirtualInterfaces_c > 0)
    uint32_t j;
#endif
    osa_status_t status;
    do
    {
        assert(pSerCfg);
        FLib_MemSet(mFsciCommData, 0x00, sizeof(mFsciCommData));

#if defined gFsciHostSupport_c && (gFsciHostSupport_c != 0)
#if defined gFsciHostSyncUseEvent_c && (gFsciHostSyncUseEvent_c != 0)
        if ((gFsciHostSyncRspEventId = OSA_EventCreate(TRUE)) == NULL)
        {
            ret = kStatus_Fail;
            break;
        }
#else  /* gFsciHostSyncUseEvent_c */
        TMR_TimeStampInit();
#endif /* gFsciHostSyncUseEvent_c */
#endif /* gFsciHostSupport_c */

        for (i = 0; i < gFsciMaxInterfaces_c; i++)
        {
            serial_handle_t ser_h;
            ser_h = gFsciSerialInterfaces[i] = (serial_handle_t)pSerCfg[i];

            /*open read handle*/
            (void)SerialManager_OpenReadHandle(ser_h, (serial_read_handle_t)gFsciSerialReadHandle[i]);
            (void)SerialManager_InstallRxCallback((serial_read_handle_t)gFsciSerialReadHandle[i], FSCI_rxCallback,
                                                  (uint8_t *)i);

            /*open write handle*/
            (void)SerialManager_OpenWriteHandle(ser_h, (serial_write_handle_t)gFsciSerialWriteHandle[i]);

#if defined gFsciHostSupport_c && (gFsciHostSupport_c != 0)
            if (KOSA_StatusSuccess != OSA_MutexCreate((osa_mutex_handle_t)mFsciCommData[i].syncHostMutexId))
            {
                ret = kStatus_Fail;
                break;
            }
#endif /* gFsciHostSupport_c */

#if defined gFsciRxAck_c && (gFsciRxAck_c != 0)
            if (KOSA_StatusSuccess != OSA_MutexCreate((osa_mutex_handle_t)mFsciCommData[i].syncTxRxAckMutexId))
            {
                ret = kStatus_Fail;
                break;
            }
#if defined gFsciRxAckTimeoutUseTmr_c && (gFsciRxAckTimeoutUseTmr_c != 0)
            if (kStatus_TimerSuccess != TM_Open((timer_handle_t)mFsciCommData[i].ackWaitTmr))
            {
                ret = kStatus_Fail;
                break;
            }
#else  /* gFsciRxAckTimeoutUseTmr_c */
            /* Initialization of the timestamp counter done by previous call to TM_Init */
#endif /* gFsciRxAckTimeoutUseTmr_c */

            mFsciCommData[i].txRetryCnt     = mFsciTxRetryCnt_c;
            mFsciCommData[i].ackReceived    = FALSE;
            mFsciCommData[i].ackWaitOngoing = FALSE;
#endif

#if defined  gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
#if !defined mFsciRxTimeoutUsePolling_c || (mFsciRxTimeoutUsePolling_c == 0)
            mFsciCommData[i].rxRestartTmr = MEM_BufferAlloc(TIMER_HANDLE_SIZE);
            if (mFsciCommData[i].rxRestartTmr == NULL)
            {
                ret = (int)kStatus_MemAllocError;
                break;
            }

            if (kStatus_TimerSuccess != TM_Open(mFsciCommData[i].rxRestartTmr))
            {
                ret = (int)kStatus_TimerSuccess;
                break;
            }
#endif    /* !mFsciRxTimeoutUsePolling_c */
            mFsciCommData[i].rxOngoing = FALSE;
#endif    /* gFsciRxTimeout_c */

        } /* for */
        if (i != gFsciMaxInterfaces_c)
        {
            ret = kStatus_Fail;
            break;
        }
#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
        mFsciClientPacketInfo.pFsciPacketToProcess = NULL;
        mFsciClientPacketInfo.fsciInterface        = mFsciInvalidInterface_c;

        /* Init Fsci task */
        status = OSA_EventCreate((osa_event_handle_t)mFsciTaskEventId, TRUE);
        assert(KOSA_StatusSuccess == status);
        status = OSA_TaskCreate((osa_task_handle_t)gFsciTaskId, OSA_TASK(FSCI_Task), NULL);
        assert(KOSA_StatusSuccess == status);
        (void)status;
#endif /* gFsciUseDedicatedTask_c */
        ret = kStatus_Success;
    } while (false);
    assert(ret == kStatus_Success);
    NOT_USED(ret);
}
#else  /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */

void FSCI_commInit(serial_handle_t *pSerCfg)
{
    assert(pSerCfg);
    FLib_MemSet(mFsciCommData, 0x00, sizeof(mFsciCommData));
    for (uint32_t i = 0; i < gFsciMaxInterfaces_c; i++)
    {
        gFsciSerialInterfaces[i] = (serial_handle_t)pSerCfg[i];
    }
#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
    osa_status_t status;
    mFsciClientPacketInfo.pFsciPacketToProcess = NULL;
    mFsciClientPacketInfo.fsciInterface        = mFsciInvalidInterface_c;

    /* Init Fsci task */
    status = OSA_EventCreate((osa_event_handle_t)mFsciTaskEventId, TRUE);
    assert(KOSA_StatusSuccess == status);
    status = OSA_TaskCreate((osa_task_handle_t)gFsciTaskId, OSA_TASK(FSCI_Task), NULL);
    assert(KOSA_StatusSuccess == status);
    (void)status;
#endif /* gFsciUseDedicatedTask_c */
}

#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
/*! *********************************************************************************
 * \brief   The main task of the FSCI module, maily used to process a packet
 *          after it has been fully received
 *
 * \param[in] argument unused
 *
 ********************************************************************************** */
#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
static void FSCI_Task(osa_task_param_t argument)
{
    osa_event_flags_t mFsciTaskEventFlags = 0;

#if USE_RTOS
    while (true)
#endif
    {
        (void)OSA_EventWait((osa_event_handle_t)mFsciTaskEventId, osaEventFlagsAll_c, FALSE, osaWaitForever_c,
                            &mFsciTaskEventFlags);

        if (mFsciTaskEventFlags == (uint32_t)gFSCI_ClientPacketReady_c)
        {
            /* a client packet is ready to be processed */
            assert(mFsciClientPacketInfo.pFsciPacketToProcess != NULL);
            assert(mFsciClientPacketInfo.fsciInterface != mFsciInvalidInterface_c);
            /* Process the client packet */
            (void)FSCI_ProcessRxPkt(mFsciClientPacketInfo.pFsciPacketToProcess, mFsciClientPacketInfo.fsciInterface);
        }
    }
}
#endif /* gFsciUseDedicatedTask_c */

/*! *********************************************************************************
 * \brief  Receives data from the serial interface and checks to see if we have a valid packet.
 *
 * \param[in]  param the fsciInterface on which the data has been received
 *
 ********************************************************************************** */
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
void FSCI_receivePacket(void *param)
{
    fsciComm_t         *pCommData = &mFsciCommData[(uint32_t)(uint32_t *)param];
    uint32_t            readBytes;
    fsci_packetStatus_t status;
    uint8_t             virtualInterfaceId;
    uint8_t             c;

#if defined gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
    bool_t timerRestartEn = FALSE;
#if defined mFsciRxTimeoutUsePolling_c && (mFsciRxTimeoutUsePolling_c != 0)
    FSCI_CheckRxTimeoutPolling(param);
#endif /* mFsciRxTimeoutUsePolling_c */
#endif /*gFsciRxTimeout_c*/

    if (kStatus_SerialManager_Success ==
        SerialManager_TryRead((serial_read_handle_t)gFsciSerialReadHandle[(uint32_t)(uint32_t *)param], &c, 1,
                              &readBytes))
    {
        while (readBytes != 0u)
        {
#if defined gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
            timerRestartEn = TRUE;
#endif /* gFsciRxTimeout_c */
            /* It is a new packet if pointer is NULL */
            if (NULL == pCommData->pPacketFromClient)
            {
                pCommData->bytesReceived = 0;
                if (c == gFSCI_StartMarker_c)
                {
                    pCommData->pPacketFromClient = (void *)&pCommData->pktHeader;
                    pCommData->bytesReceived++;
#if defined gNvStorageIncluded_d && (gNvStorageIncluded_d != 0)
                    NvSetCriticalSection();
#endif
#if defined gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
                    pCommData->rxOngoing = TRUE;
#endif /* gFsciRxTimeout_c */
                }
            }
            else
            {
                pCommData->pPacketFromClient->raw[pCommData->bytesReceived++] = c;

#if defined gFsciUseEscapeSeq_c && (gFsciUseEscapeSeq_c != 0)
                FSCI_decodeEscapeSeq(pCommData->pPacketFromClient->raw, pCommData->bytesReceived);
#endif

                /* call the check packet function to see if we have a valid packet */
                status = FSCI_checkPacket(pCommData->pPacketFromClient, pCommData->bytesReceived, &virtualInterfaceId);

                if ((pCommData->bytesReceived == sizeof(clientPacketHdr_t)) && (status == PACKET_IS_TO_SHORT))
                {
                    pCommData->pPacketFromClient = MEM_BufferAlloc(
                        sizeof(clientPacketHdr_t) + (uint32_t)pCommData->pktHeader.len + gFsci_TailBytes_c);
                    if (NULL != pCommData->pPacketFromClient)
                    {
                        FLib_MemCpy(pCommData->pPacketFromClient, &pCommData->pktHeader, sizeof(clientPacketHdr_t));
                    }
                    else
                    {
#if defined gNvStorageIncluded_d && (gNvStorageIncluded_d != 0)
                        NvClearCriticalSection();
#endif
#if defined  gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
#if !defined mFsciRxTimeoutUsePolling_c || (mFsciRxTimeoutUsePolling_c == 0)
                        (void)TM_Stop(pCommData->rxRestartTmr);
#endif /* !mFsciRxTimeoutUsePolling_c */
                        pCommData->rxOngoing = FALSE;
#endif /* gFsciRxTimeout_c */
                    }
                }

                if (status == PACKET_IS_VALID)
                {
#if defined gNvStorageIncluded_d && (gNvStorageIncluded_d != 0)
                    NvClearCriticalSection();
#endif
#if defined  gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
#if !defined mFsciRxTimeoutUsePolling_c || (mFsciRxTimeoutUsePolling_c == 0)
                    (void)TM_Stop(pCommData->rxRestartTmr);
#endif
                    pCommData->rxOngoing = FALSE;
#endif /* gFsciRxTimeout_c */
#if defined gFsciRxAck_c && (gFsciRxAck_c != 0)
                    /* Check for ACK packet */
                    if ((gFSCI_CnfOpcodeGroup_c == pCommData->pktHeader.opGroup) &&
                        ((opCode_t)mFsciMsgAck_c == pCommData->pktHeader.opCode))
                    {
                        pCommData->ackReceived = TRUE;
                        (void)MEM_BufferFree(pCommData->pPacketFromClient);
                        pCommData->pPacketFromClient = NULL;
                        /* Do not process any other packets for now */
                        break;
                    }
                    else
#endif /* gFsciRxAck_c */
                    {
#if (gFsciMaxVirtualInterfaces_c > 0)
                        mFsciSrcInterface = mFsciInvalidInterface_c;

                        for (c = 0; c < gFsciMaxInterfaces_c; c++)
                        {
                            if ((virtualInterfaceId == gFsciVirtualInterfaces[c]) &&
                                (gFsciSerialInterfaces[(uint32_t)(uint32_t *)param] == gFsciSerialInterfaces[c]))
                            {
                                mFsciSrcInterface = c;
                                break;
                            }
                        }
#else
                        mFsciSrcInterface = (uint32_t)param;
#endif /* gFsciMaxVirtualInterfaces_c > 0*/

#if defined gFsciTxAck_c && (gFsciTxAck_c != 0)
                        FSCI_Ack(c, mFsciSrcInterface);
#endif
#if defined gFsciHostSupport_c && (gFsciHostSupport_c != 0)
                        if (gFsciHostWaitingSyncRsp &&
                            (gFsciHostWaitingOpGroup == pCommData->pPacketFromClient->structured.header.opGroup) &&
                            (gFsciHostWaitingOpCode == pCommData->pPacketFromClient->structured.header.opCode))
                        {
                            /* Save packet to be processed by caller */
                            pFsciHostSyncRsp = pCommData->pPacketFromClient;
#if defined gFsciHostSyncUseEvent_c && (gFsciHostSyncUseEvent_c != 0)
                            OSA_EventSet(gFsciHostSyncRspEventId, gFSCIHost_RspReady_c);
#endif
                        }
                        else
#endif /* gFsciHostSupport_c */
                        {
                            if (mFsciSrcInterface < gFsciMaxInterfaces_c)
                            {
#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
                                /* store client packet information */
                                mFsciClientPacketInfo.pFsciPacketToProcess = pCommData->pPacketFromClient;
                                mFsciClientPacketInfo.fsciInterface        = mFsciSrcInterface;
                                /* schedule FSCI_Task by raising gFSCI_ClientPacketReady_c event
                                   FSCI_Task will process the client packet */
                                (void)OSA_EventSet((osa_event_handle_t)mFsciTaskEventId,
                                                   (uint32_t)gFSCI_ClientPacketReady_c);
#else
                                (void)FSCI_ProcessRxPkt(pCommData->pPacketFromClient, mFsciSrcInterface);
#endif
                            }
                        }
                    }
                    pCommData->pPacketFromClient = NULL;
                }
                else if (status == FRAMING_ERROR)
                {
#if 0
                    uint16_t i;

                    /* If there appears to be a framing error, search the data received for */
                    /* the next STX and try again. */
                    c = 0;
                    for (i = 0; i < pCommData->bytesReceived; ++i)
                    {
                        if (pCommData->pPacketFromClient->raw[i] == gFSCI_StartMarker_c)
                        {
                            c = gFSCI_StartMarker_c;
                            pCommData->bytesReceived -= i;
                            FLib_MemCpy( pCommData->pPacketFromClient->raw,
                                        pCommData->pPacketFromClient->raw + i,
                                        pCommData->bytesReceived);
#if 0
                            /* If a payload buffer was alocated, and the new payload is larger than the buffer's size,
                            * then a new buffer must be allocated. The received data is copied into the new buffer,
                            * and the old one is freed.
                            */
                            if( (void*)pCommData->pPacketFromClient != (void*)&pCommData->pktHeader &&
                                pCommData->bytesReceived >= sizeof(clientPacketHdr_t) &&
                                pCommData->pPacketFromClient->structured.header.len > MEM_BufferGetSize(pCommData->pPacketFromClient)
                              )
                            {
                                clientPacket_t *p;
                                p = MEM_BufferAlloc( sizeof(clientPacketHdr_t) + pCommData->pPacketFromClient->structured.header.len + gFsci_TailBytes_c );

                                if( NULL != p )
                                {
                                    FLib_MemCpy(p->raw,
                                                pCommData->pPacketFromClient->raw,
                                                pCommData->bytesReceived);
                                }

                                (void)MEM_BufferFree( pCommData->pPacketFromClient );
                                pCommData->pPacketFromClient = p;
                            }
#endif
                            break;
                        }
                    }

                    if( c != gFSCI_StartMarker_c )
#endif
                    {
                        if (pCommData->pPacketFromClient != (void *)&pCommData->pktHeader)
                        {
                            (void)MEM_BufferFree(pCommData->pPacketFromClient);
                        }

                        pCommData->pPacketFromClient = NULL;

#if defined gNvStorageIncluded_d && (gNvStorageIncluded_d != 0)
                        NvClearCriticalSection();
#endif
#if defined  gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
#if !defined mFsciRxTimeoutUsePolling_c || (mFsciRxTimeoutUsePolling_c == 0)
                        (void)TM_Stop(pCommData->rxRestartTmr);
#endif /* !mFsciRxTimeoutUsePolling_c */
                        pCommData->rxOngoing = FALSE;
#endif /* gFsciRxTimeout_c */
                    }

                } /* if (status == FRAMING_ERROR) */
                else
                {
                    /* fix MISRA-C 2004 error */
                }

            } /* if (!startOfFrameSeen) */
            if (kStatus_SerialManager_Success !=
                SerialManager_TryRead((serial_read_handle_t)gFsciSerialReadHandle[(uint32_t)(uint32_t *)param], &c, 1,
                                      &readBytes))
            {
                break;
            }

        } /* while (j < size) */

#if defined gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
        if (timerRestartEn && pCommData->rxOngoing)
        {
#if defined mFsciRxTimeoutUsePolling_c && (mFsciRxTimeoutUsePolling_c != 0)
            pCommData->lastRxByteTs = TM_GetTimestamp();
#else
            (void)TM_InstallCallback(pCommData->rxRestartTmr, FSCI_RxRxTimeoutCb, param);
            (void)TM_Start(pCommData->rxRestartTmr, kTimerModeSingleShot, mFsciRxRestartTimeoutMs_c);
#endif /* mFsciRxTimeoutUsePolling_c */
        }
#endif /* gFsciRxTimeout_c */
    }
}
#else /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
void FSCI_receivePacket(void *param)
{
    mFsciClientPacketInfo.pFsciPacketToProcess = (clientPacket_t *)param;
    mFsciSrcInterface                          = 0;
    mFsciClientPacketInfo.fsciInterface        = mFsciSrcInterface;

#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
    /* store client packet information */

    /* schedule FSCI_Task by raising gFSCI_ClientPacketReady_c event
    FSCI_Task will process the client packet */
    (void)OSA_EventSet((osa_event_handle_t)mFsciTaskEventId, (uint32_t)gFSCI_ClientPacketReady_c);
#else
    (void)FSCI_ProcessRxPkt(mFsciClientPacketInfo.pFsciPacketToProcess, mFsciClientPacketInfo.fsciInterface);
#endif
}
#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)*/
/*! *********************************************************************************
 * \brief  Send packet over the serial interface, after computing Checksum.
 *
 * \param[in] pPacket pointer to the packet to be sent over the serial interface
 * \param[in] fsciInterface the interface on which the packet should be sent
 *
 ********************************************************************************** */
void FSCI_transmitFormatedPacket(clientPacket_t *pPkt, uint32_t fsciInterface)
{
    uint32_t size;
    uint8_t  virtInterface = FSCI_GetVirtualInterface(fsciInterface);
    uint8_t  checksum;

    pPkt->structured.header.startMarker = gFSCI_StartMarker_c;
    size = (uint32_t)sizeof(clientPacketHdr_t) + pPkt->structured.header.len + 1u /* CRC */;

    /* Compute Checksum */
    checksum = FSCI_computeChecksum(pPkt->raw + 1, (uint16_t)(size - 2u));

    pPkt->structured.payload[pPkt->structured.header.len] = checksum;

    if (virtInterface != 0u)
    {
#if (gFsciMaxVirtualInterfaces_c > 0)
        pPkt->structured.payload[pPkt->structured.header.len] += virtInterface;
        pPkt->structured.payload[pPkt->structured.header.len + 1u] = checksum ^ (checksum + virtInterface);
        size += sizeof(checksum);
#else
        (void)virtInterface;
#endif
    }

    /* send message to Serial Manager */
    FSCI_SendPacketToSerialManager(fsciInterface, pPkt->raw, (uint16_t)size);
}

/*! *********************************************************************************
 * \brief  Encode and send messages over the serial interface
 *
 * \param[in] OG operation Group
 * \param[in] OC operation Code
 * \param[in] pMsg pointer to payload
 * \param[in] msgLen length of the payload
 * \param[in] fsciInterface the interface on which the packet should be sent
 *
 ********************************************************************************** */
void FSCI_transmitPayload(uint8_t OG, uint8_t OC, const uint8_t *pMsg, uint16_t msgLen, uint32_t fsciInterface)
{
    uint8_t          *buffer_ptr = NULL;
    uint16_t          buffer_size, index;
    uint8_t           checksum;
    clientPacketHdr_t header;
#if (gFsciMaxVirtualInterfaces_c > 0)
    uint8_t checksum2;
    uint8_t virtInterface = FSCI_GetVirtualInterface(fsciInterface);
#endif

    assert(msgLen <= gFsciMaxPayloadLen_c);
    if (FALSE == gFsciTxDisable)
    {
        /* Compute size */
        buffer_size = sizeof(clientPacketHdr_t) + msgLen + gFsci_TailBytes_c;

#if gFsciUseEscapeSeq_c
        buffer_size = buffer_size * 2u;
#endif

        /* Allocate buffer */
        buffer_ptr = MEM_BufferAlloc(buffer_size);
        if (NULL != buffer_ptr)
        {
            /* Message header */
            header.startMarker = gFSCI_StartMarker_c;
            header.opGroup     = OG;
            header.opCode      = OC;
            header.len         = msgLen;

            /* Compute CRC for TX packet, on opcode group, opcode, payload length, and payload fields */
            checksum = FSCI_computeChecksum((uint8_t *)&header + 1, sizeof(header) - 1u);
            checksum ^= FSCI_computeChecksum(pMsg, msgLen);
#if (gFsciMaxVirtualInterfaces_c > 0)
            if (virtInterface != 0u)
            {
                checksum2 = checksum ^ (checksum + virtInterface);
                checksum += virtInterface;
            }
#endif

            index = 0;
#if gFsciUseEscapeSeq_c
            index += (uint16_t)FSCI_encodeEscapeSeq((const uint8_t *)&header, sizeof(header), &buffer_ptr[index]);
            index += (uint16_t)FSCI_encodeEscapeSeq(pMsg, msgLen, &buffer_ptr[index]);
            /* Store the Checksum*/
            index += (uint16_t)FSCI_encodeEscapeSeq((const uint8_t *)&checksum, sizeof(checksum), &buffer_ptr[index]);
#if (gFsciMaxVirtualInterfaces_c > 0)
            if (virtInterface != 0u)
            {
                index +=
                    (uint16_t)FSCI_encodeEscapeSeq((const uint8_t *)&checksum2, sizeof(checksum2), &buffer_ptr[index]);
            }
#endif /* gFsciMaxVirtualInterfaces_c */
            buffer_ptr[index++] = gFSCI_EndMarker_c;

#else  /* gFsciUseEscapeSeq_c */
            FLib_MemCpy(&buffer_ptr[index], &header, sizeof(header));
            index += sizeof(header);
            FLib_MemCpy(&buffer_ptr[index], pMsg, msgLen);
            index += msgLen;
            /* Store the Checksum */
            buffer_ptr[index++] = checksum;
#if (gFsciMaxVirtualInterfaces_c > 0)
            if (virtInterface)
            {
                buffer_ptr[index++] = checksum2;
            }
#endif /* gFsciMaxVirtualInterfaces_c */
#endif /* gFsciUseEscapeSeq_c */

#if (defined gFsciOverRpmsgBridge_c) && (gFsciOverRpmsgBridge_c == 1)
            (void)PLATFORM_SendHciMessage(buffer_ptr, index);
#else
            /* send message to Serial Manager */
            FSCI_SendPacketToSerialManager(fsciInterface, buffer_ptr, index);
#endif
        }
    }
}

/*! *********************************************************************************
 * \brief  Get a FSCI formatted packet from a payload message
 *
 * \param[in] OG operation Group
 * \param[in] OC operation Code
 * \param[in] pMsg pointer to payload
 * \param[in] msgLen length of the payload
 * \param[out] pOutLen the actual length of the formatted packet
 *
 ********************************************************************************** */
uint8_t *FSCI_GetFormattedPacket(uint8_t OG, uint8_t OC, void *pMsg, uint16_t msgLen, uint16_t *pOutLen)
{
    uint8_t          *pBuff = NULL;
    clientPacketHdr_t header;
    uint16_t          index    = 0;
    uint8_t           checksum = 0;

    assert(pMsg);
    assert(msgLen <= gFsciMaxPayloadLen_c);
    assert(pOutLen);

    pBuff = MEM_BufferAlloc(sizeof(header) + msgLen + sizeof(checksum));
    if (pBuff != NULL)
    {
        /* Fill the message Header */
        header.startMarker = gFSCI_StartMarker_c;
        header.opGroup     = OG;
        header.opCode      = OC;
        header.len         = msgLen;

        /* Copy message Header */
        FLib_MemCpy(pBuff, &header, sizeof(header));
        index += (uint16_t)sizeof(header);
        /* Copy message payload */
        FLib_MemCpy(pBuff + index, pMsg, msgLen);
        index += msgLen;
        checksum       = FSCI_computeChecksum(pBuff + 1, sizeof(header) - 1u + msgLen);
        pBuff[index++] = checksum;

        *pOutLen = index;
    }

    return pBuff;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief  Returnd the virtual interface associated with the specified fsciInterface.
 *
 * \param[in] fsciInterface the interface on which the packet should be sent
 *
 * \return the Virtual Interface Id
 *
 ********************************************************************************** */
uint8_t FSCI_GetVirtualInterface(uint32_t fsciInterface)
{
#if (gFsciMaxVirtualInterfaces_c > 0)
    return gFsciVirtualInterfaces[fsciInterface];
#else
    return 0;
#endif
}

/*! *********************************************************************************
 * \brief  Checks to see if we have a valid packet
 *
 * \param[in] pData The message containing the incoming data packet to be handled.
 * \param[in] bytes the number of bytes inside the buffer
 * \param[Out] pVIntf pointer to the location where the virtual interface Id will be stored
 *
 * \return the status of the packet
 *
 ********************************************************************************** */
fsci_packetStatus_t FSCI_checkPacket(clientPacket_t *pData, uint16_t bytes, uint8_t *pVIntf)
{
    uint8_t             checksum = 0;
    uint16_t            len;
    fsci_packetStatus_t status = FRAMING_ERROR;

    assert(pData);
    if (bytes < MIN_VALID_PACKET_LEN)
    {
        status = PACKET_IS_TO_SHORT; /* Too short to be valid. */
    }
    else if (bytes >= sizeof(clientPacket_t))
    {
        status = FRAMING_ERROR;
    }
    else
    {
        /* The packet's len field does not count the STX, the opcode group, the */
        /* opcode, the len field, or the checksum. */
        len = pData->structured.header.len;

        /* If the length appears to be too long, it might be because the external */
        /* client is sending a packet that is too long, or it might be that we're */
        /* out of sync with the external client. Assume we're out of sync. */
        if (len > gFsciMaxPayloadLen_c)
        {
            status = FRAMING_ERROR;
        }
        else
        {
            if (bytes < (len + sizeof(clientPacketHdr_t) + (uint16_t)sizeof(checksum)))
            {
                status = PACKET_IS_TO_SHORT;
            }
            else /* If the length looks right, make sure that the checksum is correct. */
            {
                checksum = FSCI_computeChecksum(pData->raw + 1, (uint16_t)(len + sizeof(clientPacketHdr_t) - 1u));
                *pVIntf  = pData->structured.payload[len] - checksum;

                if (bytes == (uint16_t)(len + sizeof(clientPacketHdr_t) + (uint16_t)sizeof(checksum)))
                {
                    if (0u == *pVIntf)
                    {
                        status = PACKET_IS_VALID;
                    }
#if (gFsciMaxVirtualInterfaces_c > 0)
                    else
                    {
                        if (*pVIntf < (uint8_t)gFsciMaxVirtualInterfaces_c)
                        {
                            status = PACKET_IS_TO_SHORT;
                        }
                    }
#endif
                }

#if (gFsciMaxVirtualInterfaces_c > 0)
                if (FRAMING_ERROR == status)
                {
                    /* Check virtual interface */
                    if (bytes == len + sizeof(clientPacketHdr_t) + 2u * sizeof(checksum))
                    {
                        checksum ^= checksum + *pVIntf;
                        if (pData->structured.payload[len + 1u] == checksum)
                        {
                            status = PACKET_IS_VALID;
                        }
                    }
                }
#endif
            }
        }
    }
    return status;
}

/*! *********************************************************************************
 * \brief  This function performs a XOR over the message to compute the CRC
 *
 * \param[in]  pBuffer - pointer to the messae
 * \param[in]  size - the length of the message
 *
 * \return  the CRC of the message
 *
 ********************************************************************************** */
uint8_t FSCI_computeChecksum(const void *pBuffer, uint16_t size)
{
    uint16_t index;
    uint8_t  checksum = 0;

    /* Compute the CRC from Opcode byte */
    for (index = 0; index < size; index++)
    {
        checksum ^= ((const uint8_t *)pBuffer)[index];
    }

    return checksum;
}

/*! *********************************************************************************
 * \brief  This function performs the encoding of a message, using the Escape Sequence
 *
 * \param[in]  pDataIn, pointer to the messae to be encoded
 * \param[in]  len, the length of the message
 * \param[out]  pDataOut, pointer to the encoded message
 *
 * \return  The number of bytes added in the new buffer
 *
 ********************************************************************************** */
#if gFsciUseEscapeSeq_c
uint32_t FSCI_encodeEscapeSeq(const uint8_t *pDataIn, uint32_t len, uint8_t *pDataOut)
{
    uint32_t index, new_index = 0;

    if (NULL != pDataOut)
    {
        for (index = 0; index < len; index++)
        {
            if ((pDataIn[index] == gFSCI_StartMarker_c) || (pDataIn[index] == gFSCI_EndMarker_c) ||
                (pDataIn[index] == gFSCI_EscapeChar_c))
            {
                pDataOut[new_index++] = gFSCI_EscapeChar_c;
                pDataOut[new_index++] = pDataIn[index] ^ gFSCI_EscapeChar_c;
            }
            else
            {
                pDataOut[new_index++] = pDataIn[index];
            }
        }
    }

    return new_index;
}
#endif

/*! *********************************************************************************
 * \brief  This function performs the decoding of a message, using the Escape Sequence
 *
 * \param[in]  pData pointer to the messae to be encoded
 * \param[in]  len the length of the message
 *
 *
 ********************************************************************************** */
#if gFsciUseEscapeSeq_c
void FSCI_decodeEscapeSeq(uint8_t *pData, uint32_t len)
{
    uint32_t index, new_index;

    /* Find the first gFSCI_EscapeChar_c */
    for (index = 0; index < len; index++)
    {
        if (pData[index] == gFSCI_EscapeChar_c)
        {
            break;
        }
    }

    new_index = index;

    /* If a gFSCI_EscapeChar_c was found, decode the packet in place */
    while (index < len)
    {
        if (pData[index] == gFSCI_EscapeChar_c)
        {
            index++; /* skip over the gFSCI_EscapeChar_c */

            if (index < len)
            {
                pData[new_index++] = pData[index++] ^ gFSCI_EscapeChar_c;
            }
        }
        else if (new_index != index)
        {
            pData[new_index++] = pData[index++];
        }
        else
        {
            /*MISRA rule 15.7*/
        }
    }
}
#endif
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
inline static void FSCI_rxCallback(void                              *pData,
                                   serial_manager_callback_message_t *message,
                                   serial_manager_status_t            status)
{
    FSCI_receivePacket(pData);
}
#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
#if defined gFsciRxAckTimeoutUseTmr_c && (gFsciRxAckTimeoutUseTmr_c != 0)
/*! *********************************************************************************
 * \brief  This function is the callback of an Ack wait expire for a fsci interface
 *
 * \param[in]  param fsci interface on which the Ack was expected
 *
 ********************************************************************************** */
static void FSCI_RxAckExpireCb(void *param)
{
    if (mFsciCommData[(uint32_t)(uint32_t *)param].txRetryCnt)
    {
        mFsciCommData[(uint32_t)(uint32_t *)param].txRetryCnt--;
    }

    /* Allow retransmission */
    mFsciCommData[(uint32_t)(uint32_t *)param].ackWaitOngoing = FALSE;
}
#endif /* gFsciRxAckTimeoutUseTmr_c */

#if defined gFsciRxTimeout_c && (gFsciRxTimeout_c != 0)
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
/*! *********************************************************************************
 * \brief  This function is the callback of an Rx timeout expired for a fsci interface
 *
 * \param[in]  param fsci interface on which no bytes were received
 *
 ********************************************************************************** */
static void FSCI_RxRxTimeoutCb(void *param)
{
    /* Do the cleanup in case no other character is received */
    fsciComm_t *pCommData = &mFsciCommData[(uint32_t)(uint32_t *)param];

    OSA_InterruptDisable();
#if defined gNvStorageIncluded_d && (gNvStorageIncluded_d != 0)
    NvClearCriticalSection();
#endif
    pCommData->rxOngoing = FALSE;
    /* Check if free buffer size for timeout */
    if (pCommData->pPacketFromClient != (void *)&pCommData->pktHeader)
    {
        (void)MEM_BufferFree(pCommData->pPacketFromClient);
    }
    pCommData->pPacketFromClient = NULL;
    OSA_InterruptEnable();
}
#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
#endif

/*! *********************************************************************************
 * \brief  This function is used to check the RX timeout with polling
 *
 * \param[in]  param fsci interface
 *
 ********************************************************************************** */
#if defined mFsciRxTimeoutUsePolling_c && (mFsciRxTimeoutUsePolling_c != 0)
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
static void FSCI_CheckRxTimeoutPolling(void *param)
{
    fsciComm_t *pCommData = &mFsciCommData[(uint32_t)(uint32_t *)param];
    uint64_t    currentTs = 0U;

    if (pCommData->rxOngoing)
    {
        currentTs = TM_GetTimestamp();

        /* Check for timeout */
        if (((currentTs - pCommData->lastRxByteTs) / 1000u) > mFsciRxRestartTimeoutMs_c)
        {
            /* call the RX timeout callback to do the cleanup */
            FSCI_RxRxTimeoutCb(param);
        }
    }
}
#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
#endif

#if !defined gFsciRxAck_c || (gFsciRxAck_c == 0)
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
static void fsci_serial_manager_tx_callback(void                              *callbackParam,
                                            serial_manager_callback_message_t *message,
                                            serial_manager_status_t            status)
{
    fsci_serial_manager_tx_t *tx = (fsci_serial_manager_tx_t *)callbackParam;

    FSCI_txCallback(tx->txBuffer);
    (void)MEM_BufferFree(tx);
}
#endif /* defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
#endif /* !gFsciRxAck_c */

/*! *********************************************************************************
 * \brief  This function is used to send a FSCI packet to the serial manager
 *
 * \param[in]  fsciInterface fsci interface on which the packet is to be sent
 * \param[in]  pPacket serial packet to be sent
 * \param[in]  packetLen lenght of the serial packet in bytes
 *
 ********************************************************************************** */
#if !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0)
static void FSCI_SendPacketToSerialManager(uint32_t fsciInterface, uint8_t *pPacket, uint16_t packetLen)
{
#if defined  gFsciRxAck_c && (gFsciRxAck_c != 0)

    fsciComm_t *pCommData = &mFsciCommData[fsciInterface];
#if !defined gFsciRxAckTimeoutUseTmr_c || (gFsciRxAckTimeoutUseTmr_c == 0)
    uint64_t ackWaitStartTs;
    uint64_t currentTs;
#endif /* gFsciRxAckTimeoutUseTmr_c */
    (void)OSA_MutexLock(pCommData->syncTxRxAckMutexId, osaWaitForever_c);

    pCommData->ackReceived = FALSE;
    pCommData->txRetryCnt  = mFsciTxRetryCnt_c;

    while (pCommData->txRetryCnt != 0u)
    {
        (void)SerialManager_WriteBlocking((serial_write_handle_t)gFsciSerialWriteHandle[fsciInterface], pPacket,
                                          packetLen);
        pCommData->ackWaitOngoing = TRUE;

        /* Allow the FSCI interface to receive ACK packet,
           if last packet was received on the same interface */
        if (mFsciSrcInterface == fsciInterface)
        {
            pCommData->pPacketFromClient = NULL;
        }

#if defined gFsciRxAckTimeoutUseTmr_c && (gFsciRxAckTimeoutUseTmr_c != 0)
        /* Start timer for ACK wait */
        (void)TM_InstallCallback(pCommData->ackWaitTmr, FSCI_RxAckExpireCb, (void *)fsciInterface);
        (void)TM_Start(pCommData->ackWaitTmr, kTimerModeSingleShot, mFsciRxAckTimeoutMs_c);

        /* Wait for timer to expire or Ack to be received */
        while (pCommData->ackWaitOngoing && !pCommData->ackReceived)
        {
            FSCI_receivePacket((uint32_t *)fsciInterface);
        }

        if (pCommData->ackReceived)
        {
            (void)TM_Stop(pCommData->ackWaitTmr);
            pCommData->ackWaitOngoing = FALSE;
            break;
        }
#else  /* gFsciRxAckTimeoutUseTmr_c */
        /* Get initial timestamp and poll until mFsciRxAckTimeoutMs_c has passed */
        ackWaitStartTs = TM_GetTimestamp();

        while (!pCommData->ackReceived)
        {
            FSCI_receivePacket((uint32_t *)fsciInterface);
            currentTs = TM_GetTimestamp();
            if (((currentTs - ackWaitStartTs) / 1000u) > mFsciRxAckTimeoutMs_c)
            {
                pCommData->ackWaitOngoing = FALSE;
                if (pCommData->txRetryCnt != 0u)
                {
                    pCommData->txRetryCnt--;
                }
                break; /* Timeout expired */
            }
        }

        if (pCommData->ackReceived)
        {
            pCommData->ackWaitOngoing = FALSE;
            break; /* Success */
        }
#endif /* gFsciRxAckTimeoutUseTmr_c */
    }

    (void)MEM_BufferFree(pPacket);

    (void)OSA_MutexUnlock(pCommData->syncTxRxAckMutexId);
#else  /* gFsciRxAck_c */
#if defined gFsciUseBlockingTx_c && (gFsciUseBlockingTx_c != 0)
    if (gFsciTxBlocking)
    {
        (void)SerialManager_WriteBlocking((serial_write_handle_t)gFsciSerialWriteHandle[fsciInterface], pPacket,
                                          packetLen);
        (void)MEM_BufferFree(pPacket);
    }
    else
#endif /* gFsciUseBlockingTx_c */
    {
        fsci_serial_manager_tx_t *tx = MEM_BufferAllocWithId(sizeof(fsci_serial_manager_tx_t), 0U);

        if (NULL != tx)
        {
            tx->txBuffer = pPacket;

            (void)SerialManager_OpenWriteHandle((serial_handle_t)gFsciSerialInterfaces[fsciInterface],
                                                (serial_write_handle_t)tx->fsciSerialWriteHandle);
            (void)SerialManager_InstallTxCallback((serial_write_handle_t)tx->fsciSerialWriteHandle,
                                                  (serial_manager_callback_t)fsci_serial_manager_tx_callback, tx);
            if (kStatus_SerialManager_Success !=
                SerialManager_WriteNonBlocking((serial_write_handle_t)tx->fsciSerialWriteHandle, tx->txBuffer,
                                               packetLen))
            {
                (void)MEM_BufferFree(pPacket);
                (void)MEM_BufferFree(tx);
            }
        }
        else
        {
            (void)MEM_BufferFree(pPacket);
        }
    }
#endif /* gFsciRxAck_c */
}
#else  /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
static void FSCI_SendPacketToSerialManager(uint32_t fsciInterface, uint8_t *pPacket, uint16_t packetLen)
{
    union
    {
        void         *pVoid;
        pfFSCI_Send_t pfFSCI_Send;
    } fsciHandle;
    fsciHandle.pVoid = gFsciSerialInterfaces[fsciInterface];
    fsciHandle.pfFSCI_Send(pPacket, packetLen, TRUE);
}
#endif /* !defined(gFsciOverRpmsg_c) || (gFsciOverRpmsg_c == 0) */
#endif /* gFsciIncluded_c */
