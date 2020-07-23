/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
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

#include "FsciCommunication.h"
#include "FsciInterface.h"
#include "FsciCommands.h"
#include "FunctionLib.h"

#include "MemManager.h"
#include "Panic.h"
#include "TimersManager.h"

#include "fsl_os_abstraction.h"
#include <assert.h>

#if gNvStorageIncluded_d
#include "NVM_Interface.h"
#endif

#if gFsciIncluded_c
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define gFsciUseBlockingTx_c 1
#define MIN_VALID_PACKET_LEN (sizeof(clientPacketHdr_t))
#define FSCI_txCallback MEM_BufferFree
#define FSCI_rxCallback FSCI_receivePacket

#ifndef mFsciRxAckTimeoutMs_c
#define mFsciRxAckTimeoutMs_c 100U /* milliseconds */
#endif

#ifndef mFsciTxRetryCnt_c
#define mFsciTxRetryCnt_c     4
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
static void FSCI_Task(osaTaskParam_t argument);
#endif

fsci_packetStatus_t FSCI_checkPacket( clientPacket_t *pData, uint16_t bytes, uint8_t* pVIntf );

#if gFsciRxAck_c && gFsciRxAckTimeoutUseTmr_c
static void FSCI_RxAckExpireCb(void *param);
#endif

#if gFsciRxTimeout_c
static void FSCI_RxRxTimeoutCb(void *param);
#endif

#if mFsciRxTimeoutUsePolling_c
static void FSCI_CheckRxTimeoutPolling(void* param);
#endif

static void FSCI_SendPacketToSerialManager(uint32_t fsciInterface, uint8_t *pPacket, uint16_t packetLen);

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
typedef struct {
    clientPacket_t*     pFsciPacketToProcess;
    uint8_t             fsciInterface;
} fsciClientPacketInfo_t;
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
bool_t gFsciTxBlocking = FALSE;
bool_t gFsciTxDisable  = FALSE;

/* Holds the SMGR interface Id associated with the FSCI interface (index) */
uint8_t gFsciSerialInterfaces[gFsciMaxInterfaces_c];

#if (gFsciMaxVirtualInterfaces_c > 0)
/* Holds the virtual interface Id for the FSCI interface (index) */
static uint8_t gFsciVirtualInterfaces[gFsciMaxInterfaces_c];
#endif

#if gFsciHostSupport_c
    #if gFsciHostSyncUseEvent_c && !USE_RTOS
    #undef gFsciHostSyncUseEvent_c
    #endif

    #if gFsciHostSyncUseEvent_c
    osaEventId_t gFsciHostSyncRspEventId;
    #endif

clientPacket_t *pFsciHostSyncRsp = NULL;
bool_t          gFsciHostWaitingSyncRsp = FALSE;
opGroup_t       gFsciHostWaitingOpGroup = 0;
opCode_t        gFsciHostWaitingOpCode = 0;
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static fsciComm_t mFsciCommData[gFsciMaxInterfaces_c];

static uint8_t mFsciSrcInterface = mFsciInvalidInterface_c;

#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
static OSA_TASK_DEFINE( FSCI_Task, gFsciTaskPriority_c, 1, gFsciTaskStackSize_c, FALSE );
static osaTaskId_t      gFsciTaskId;
static osaEventId_t     mFsciTaskEventId;

static fsciClientPacketInfo_t mFsciClientPacketInfo;
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#if gFsciHostSupport_c
void FSCI_HostSyncLock(uint32_t fsciInstance, opGroup_t OG, opCode_t OC)
{
    OSA_MutexLock(mFsciCommData[fsciInstance].syncHostMutexId, osaWaitForever_c);
    gFsciHostWaitingSyncRsp = TRUE;
    gFsciHostWaitingOpGroup = OG;
    gFsciHostWaitingOpCode = OC;
    pFsciHostSyncRsp = NULL;
}

void FSCI_HostSyncUnlock(uint32_t fsciInstance)
{
    gFsciHostWaitingSyncRsp = FALSE;
    pFsciHostSyncRsp = NULL;
    OSA_MutexUnlock(mFsciCommData[fsciInstance].syncHostMutexId);
}
#endif

/*! *********************************************************************************
* \brief  Initialize the serial interface.
*
* \param[in]  initStruct pointer to a gFsciSerialConfig_t structure
*
********************************************************************************** */
void FSCI_commInit( const gFsciSerialConfig_t* pSerCfg )
{
    uint32_t i;
#if (gFsciMaxVirtualInterfaces_c > 0)
    uint32_t j;
#endif

    assert(pSerCfg);
    FLib_MemSet( mFsciCommData, 0x00, sizeof(mFsciCommData) );

#if gFsciHostSupport_c && gFsciHostSyncUseEvent_c
    if( (gFsciHostSyncRspEventId = OSA_EventCreate(TRUE)) == NULL )
    {
        panic( ID_PANIC(0,0), (uint32_t)FSCI_commInit, 0, 0 );
        return;
    }
#endif
#if gFsciHostSupport_c && !gFsciHostSyncUseEvent_c
    TMR_TimeStampInit();
#endif
    for ( i = 0; i < gFsciMaxInterfaces_c; i++ )
    {
        gFsciSerialInterfaces[i] = gSerialMgrInvalidIdx_c;

#if (gFsciMaxVirtualInterfaces_c > 0)
        gFsciVirtualInterfaces[i] = pSerCfg[i].virtualInterface;

        if( pSerCfg[i].virtualInterface >= gFsciMaxInterfaces_c )
        {
            panic( ID_PANIC(0,0), (uint32_t)FSCI_commInit, 0, 0 );
            break;
        }

        /* Check if the serial interface was allready initialized */
        for( j = 0; j < i; j++ )
        {
            if( (pSerCfg[i].interfaceType == pSerCfg[j].interfaceType) &&
                (pSerCfg[i].interfaceChannel == pSerCfg[j].interfaceChannel) )
            {
                gFsciSerialInterfaces[i] = gFsciSerialInterfaces[j];
            }
        }

        if( gFsciSerialInterfaces[i] == (uint8_t)gSerialMgrInvalidIdx_c )
#endif
        {
            (void)Serial_InitInterface(&gFsciSerialInterfaces[i],
                                        pSerCfg[i].interfaceType,
                                        pSerCfg[i].interfaceChannel );

            (void)Serial_SetRxCallBack( gFsciSerialInterfaces[i], FSCI_rxCallback, (uint8_t*)i);
            (void)Serial_SetBaudRate  ( gFsciSerialInterfaces[i], pSerCfg[i].baudrate );

#if gFsciHostSupport_c
            if( (mFsciCommData[i].syncHostMutexId = OSA_MutexCreate()) == NULL )
            {
                panic( ID_PANIC(0,0), (uint32_t)FSCI_commInit, 0, 0 );
                break;
            }
#endif

#if gFsciRxAck_c
            mFsciCommData[i].syncTxRxAckMutexId = OSA_MutexCreate();
            assert(mFsciCommData[i].syncTxRxAckMutexId);

#if gFsciRxAckTimeoutUseTmr_c
            mFsciCommData[i].ackWaitTmr = TMR_AllocateTimer();
            assert(gTmrInvalidTimerID_c != mFsciCommData[i].ackWaitTmr);
#else
            TMR_TimeStampInit();
#endif

            mFsciCommData[i].txRetryCnt = mFsciTxRetryCnt_c;
            mFsciCommData[i].ackReceived = FALSE;
            mFsciCommData[i].ackWaitOngoing = FALSE;
#endif

#if gFsciRxTimeout_c
#if mFsciRxTimeoutUsePolling_c
            TMR_TimeStampInit();
#else
            mFsciCommData[i].rxRestartTmr = TMR_AllocateTimer();
            if( gTmrInvalidTimerID_c == mFsciCommData[i].rxRestartTmr )
            {
                panic( ID_PANIC(0,0), (uint32_t)FSCI_commInit, 0, 0 );
                break;
            }
#endif

            mFsciCommData[i].rxOngoing = FALSE;
#endif
        }
    }

#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)

    mFsciClientPacketInfo.pFsciPacketToProcess = NULL;
    mFsciClientPacketInfo.fsciInterface = mFsciInvalidInterface_c;

    /* Init Fsci task */
    mFsciTaskEventId = OSA_EventCreate(TRUE);
    gFsciTaskId = OSA_TaskCreate(OSA_TASK(FSCI_Task), NULL);

    assert(mFsciTaskEventId);
    assert(gFsciTaskId);
    NOT_USED(gFsciTaskId);
#endif
}

/*! *********************************************************************************
* \brief   The main task of the FSCI module, maily used to process a packet
*          after it has been fully received
*
* \param[in] argument unused
*
********************************************************************************** */
#if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
static void FSCI_Task(osaTaskParam_t argument)
{
    osaEventFlags_t  mFsciTaskEventFlags = 0;

#if USE_RTOS
    while( 1 )
#endif
    {
        (void)OSA_EventWait(mFsciTaskEventId, osaEventFlagsAll_c, FALSE, osaWaitForever_c ,&mFsciTaskEventFlags);

        if(mFsciTaskEventFlags == (uint32_t)gFSCI_ClientPacketReady_c)
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
* \brief  Receives data from the serial interface and checks to see if we have a valid pachet.
*
* \param[in]  param the fsciInterface on which the data has been received
*
********************************************************************************** */
void FSCI_receivePacket( void* param )
{
    fsciComm_t          *pCommData = &mFsciCommData[(uint32_t)(uint32_t*)param];
    uint16_t            readBytes;
    fsci_packetStatus_t status;
    uint8_t             virtualInterfaceId;
    uint8_t             c;

    #if gFsciRxTimeout_c
    bool_t timerRestartEn = FALSE;
    #if mFsciRxTimeoutUsePolling_c
    FSCI_CheckRxTimeoutPolling(param);
    #endif
    #endif /*gFsciRxTimeout_c*/

    if( gSerial_Success_c == Serial_GetByteFromRxBuffer( gFsciSerialInterfaces[(uint32_t)(uint32_t*)param], &c, &readBytes ) )
    {
        while( readBytes != 0u)
        {
            #if gFsciRxTimeout_c
            timerRestartEn = TRUE;
            #endif
            /* It is a new packet if pointer is NULL */
            if( NULL == pCommData->pPacketFromClient )
            {
                pCommData->bytesReceived = 0;
                if( c == gFSCI_StartMarker_c )
                {
                    pCommData->pPacketFromClient = (void*)&pCommData->pktHeader;
                    pCommData->bytesReceived++;
                    #if gNvStorageIncluded_d
                    NvSetCriticalSection();
                    #endif
                    #if gFsciRxTimeout_c
                    pCommData->rxOngoing = TRUE;
                    #endif
                }
            }
            else
            {
                pCommData->pPacketFromClient->raw[pCommData->bytesReceived++] = c;

                #if gFsciUseEscapeSeq_c
                FSCI_decodeEscapeSeq( pCommData->pPacketFromClient->raw, pCommData->bytesReceived );
                #endif

                /* call the check pachet function to see if we have a valid packet */
                status = FSCI_checkPacket( pCommData->pPacketFromClient, pCommData->bytesReceived, &virtualInterfaceId );

                if( (pCommData->bytesReceived == sizeof(clientPacketHdr_t)) && (status == PACKET_IS_TO_SHORT) )
                {
                    pCommData->pPacketFromClient = MEM_BufferAlloc( sizeof(clientPacketHdr_t) + (uint32_t)pCommData->pktHeader.len + gFsci_TailBytes_c );
                    if( NULL != pCommData->pPacketFromClient )
                    {
                        FLib_MemCpy(pCommData->pPacketFromClient, &pCommData->pktHeader, sizeof(clientPacketHdr_t));
                    }
                    else
                    {
                        #if gNvStorageIncluded_d
                        NvClearCriticalSection();
                        #endif
                        #if gFsciRxTimeout_c
                        #if !mFsciRxTimeoutUsePolling_c
                        (void)TMR_StopTimer(pCommData->rxRestartTmr);
                        #endif
                        pCommData->rxOngoing = FALSE;
                        #endif
                    }
                }

                if( status == PACKET_IS_VALID )
                {
                    #if gNvStorageIncluded_d
                    NvClearCriticalSection();
                    #endif
                    #if gFsciRxTimeout_c
                    #if !mFsciRxTimeoutUsePolling_c
                    (void)TMR_StopTimer(pCommData->rxRestartTmr);
                    #endif
                    pCommData->rxOngoing = FALSE;
                    #endif
                    #if gFsciRxAck_c
                    /* Check for ACK packet */
                    if( ( gFSCI_CnfOpcodeGroup_c == pCommData->pktHeader.opGroup ) &&
                        ( (opCode_t)mFsciMsgAck_c == pCommData->pktHeader.opCode ) )
                    {
                        pCommData->ackReceived = TRUE;
                        (void)MEM_BufferFree(pCommData->pPacketFromClient);
                        pCommData->pPacketFromClient = NULL;
                        /* Do not process any other packets for now */
                        break;
                    }
                    else
                    #endif
                    {
                        #if (gFsciMaxVirtualInterfaces_c > 0)
                        mFsciSrcInterface = mFsciInvalidInterface_c;

                        for ( c = 0; c < gFsciMaxInterfaces_c; c++)
                        {
                            if ( (virtualInterfaceId == gFsciVirtualInterfaces[c]) &&
                                 (gFsciSerialInterfaces[(uint32_t)(uint32_t*)param] == gFsciSerialInterfaces[c]) )
                            {
                                mFsciSrcInterface = c;
                                break;
                            }
                        }
                        #else
                        mFsciSrcInterface = (uint32_t)param;
                        #endif

                        #if gFsciTxAck_c
                        FSCI_Ack(c, mFsciSrcInterface);
                        #endif
                        #if gFsciHostSupport_c
                        if( gFsciHostWaitingSyncRsp &&
                          ( gFsciHostWaitingOpGroup == pCommData->pPacketFromClient->structured.header.opGroup ) &&
                          ( gFsciHostWaitingOpCode == pCommData->pPacketFromClient->structured.header.opCode ) )
                        {
                            /* Save packet to be processed by caller */
                            pFsciHostSyncRsp = pCommData->pPacketFromClient;
                            #if gFsciHostSyncUseEvent_c
                            OSA_EventSet(gFsciHostSyncRspEventId, gFSCIHost_RspReady_c);
                            #endif
                        }
                        else
                        #endif
                        {
                            if(mFsciSrcInterface < gFsciMaxInterfaces_c)
                            {
                                #if defined(gFsciUseDedicatedTask_c) && (gFsciUseDedicatedTask_c == 1)
                                /* store client packet information */
                                mFsciClientPacketInfo.pFsciPacketToProcess = pCommData->pPacketFromClient;
                                mFsciClientPacketInfo.fsciInterface = mFsciSrcInterface;
                                /* schedule FSCI_Task by raising gFSCI_ClientPacketReady_c event
                                   FSCI_Task will process the client packet */
                                (void)OSA_EventSet(mFsciTaskEventId, (uint32_t)gFSCI_ClientPacketReady_c);
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
                        if( pCommData->pPacketFromClient != (void*)&pCommData->pktHeader )
                        {
                            (void)MEM_BufferFree(pCommData->pPacketFromClient);
                        }

                        pCommData->pPacketFromClient = NULL;

                        #if gNvStorageIncluded_d
                        NvClearCriticalSection();
                        #endif
                        #if gFsciRxTimeout_c
                        #if !mFsciRxTimeoutUsePolling_c
                        (void)TMR_StopTimer(pCommData->rxRestartTmr);
                        #endif
                        pCommData->rxOngoing = FALSE;
                        #endif
                    }

                } /* if (status == FRAMING_ERROR) */
                else
                {
                    /* fix MISRA-C 2004 error */
                }

            }  /* if (!startOfFrameSeen) */

            if ( gSerial_Success_c != Serial_GetByteFromRxBuffer( gFsciSerialInterfaces[(uint32_t)(uint32_t*)param], &c, &readBytes ) )
            {
                break;
            }

        } /* while (j < size) */

        #if gFsciRxTimeout_c
        if( timerRestartEn && pCommData->rxOngoing )
        {
            #if mFsciRxTimeoutUsePolling_c
            pCommData->lastRxByteTs = TMR_GetTimestamp();
            #else
            (void)TMR_StartSingleShotTimer(pCommData->rxRestartTmr,
                                     mFsciRxRestartTimeoutMs_c,
                                     FSCI_RxRxTimeoutCb,
                                     param);
            #endif
        }
        #endif
    }
}

/*! *********************************************************************************
* \brief  Send packet over the serial interface, after computing Checksum.
*
* \param[in] pPacket pointer to the packet to be sent over the serial interface
* \param[in] fsciInterface the interface on which the packet should be sent
*
********************************************************************************** */
void FSCI_transmitFormatedPacket( clientPacket_t *pPkt, uint32_t fsciInterface )
{
    uint32_t        size;
    uint8_t         virtInterface = FSCI_GetVirtualInterface(fsciInterface);
    uint8_t         checksum;

    pPkt->structured.header.startMarker = gFSCI_StartMarker_c;
    size = (uint32_t)sizeof(clientPacketHdr_t) + pPkt->structured.header.len + 1u /* CRC */;

    /* Compute Checksum */
    checksum = FSCI_computeChecksum( pPkt->raw+1, (uint16_t)(size - 2u));

    pPkt->structured.payload[pPkt->structured.header.len] = checksum;

    if( virtInterface != 0u )
    {
#if (gFsciMaxVirtualInterfaces_c > 0)
        pPkt->structured.payload[pPkt->structured.header.len] += virtInterface;
        pPkt->structured.payload[pPkt->structured.header.len+1u] = checksum^(checksum + virtInterface);
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
void FSCI_transmitPayload( uint8_t OG, uint8_t OC, const uint8_t *pMsg, uint16_t msgLen, uint32_t fsciInterface )
{
    uint8_t* buffer_ptr = NULL;
    uint16_t buffer_size, index;
    uint8_t checksum;
    clientPacketHdr_t header;
    #if (gFsciMaxVirtualInterfaces_c > 0)
    uint8_t checksum2;
    uint8_t virtInterface = FSCI_GetVirtualInterface(fsciInterface);
    #endif

    assert(msgLen <= gFsciMaxPayloadLen_c);
    if( FALSE == gFsciTxDisable )
    {
        /* Compute size */
        buffer_size = sizeof(clientPacketHdr_t) + msgLen + gFsci_TailBytes_c;

        #if gFsciUseEscapeSeq_c
        buffer_size = buffer_size*2u;
        #endif

        /* Allocate buffer */
        buffer_ptr = MEM_BufferAlloc( buffer_size );
        if( NULL != buffer_ptr )
        {
             /* Message header */
            header.startMarker = gFSCI_StartMarker_c;
            header.opGroup = OG;
            header.opCode = OC;
            header.len = msgLen;

            /* Compute CRC for TX packet, on opcode group, opcode, payload length, and payload fields */
            checksum = FSCI_computeChecksum((uint8_t*)&header + 1, sizeof(header) - 1u);
            checksum ^= FSCI_computeChecksum(pMsg, msgLen);
            #if (gFsciMaxVirtualInterfaces_c > 0)
            if( virtInterface != 0u )
            {
                checksum2 = checksum^(checksum + virtInterface);
                checksum += virtInterface;
            }
            #endif

            index = 0;
            #if gFsciUseEscapeSeq_c
            index += (uint16_t)FSCI_encodeEscapeSeq( (const uint8_t*)&header, sizeof(header), &buffer_ptr[index] );
            index += (uint16_t)FSCI_encodeEscapeSeq( pMsg, msgLen, &buffer_ptr[index]);
            /* Store the Checksum*/
            index += (uint16_t)FSCI_encodeEscapeSeq( (const uint8_t*)&checksum, sizeof(checksum), &buffer_ptr[index] );
            #if (gFsciMaxVirtualInterfaces_c > 0)
            if( virtInterface != 0u )
            {
                index += (uint16_t)FSCI_encodeEscapeSeq( (const uint8_t*)&checksum2, sizeof(checksum2), &buffer_ptr[index] );
            }
            #endif /* gFsciMaxVirtualInterfaces_c */
            buffer_ptr[index++] = gFSCI_EndMarker_c;

            #else /* gFsciUseEscapeSeq_c */
            FLib_MemCpy( &buffer_ptr[index], &header, sizeof(header) );
            index += sizeof(header);
            FLib_MemCpy( &buffer_ptr[index], pMsg, msgLen );
            index += msgLen;
            /* Store the Checksum */
            buffer_ptr[index++] = checksum;
            #if (gFsciMaxVirtualInterfaces_c > 0)
            if( virtInterface )
            {
                buffer_ptr[index++] = checksum2;
            }
            #endif /* gFsciMaxVirtualInterfaces_c */
            #endif /* gFsciUseEscapeSeq_c */

            /* send message to Serial Manager */
            FSCI_SendPacketToSerialManager(fsciInterface, buffer_ptr, index);
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
uint8_t* FSCI_GetFormattedPacket(uint8_t OG, uint8_t OC, void *pMsg, uint16_t msgLen, uint16_t *pOutLen)
{
    uint8_t* pBuff = NULL;
    clientPacketHdr_t header;
    uint16_t index = 0;
    uint8_t checksum = 0;

    assert(pMsg);
    assert(msgLen <= gFsciMaxPayloadLen_c);
    assert(pOutLen);

    pBuff = MEM_BufferAlloc(sizeof(header) + msgLen + sizeof(checksum));
    if( pBuff != NULL )
    {
        /* Fill the message Header */
        header.startMarker = gFSCI_StartMarker_c;
        header.opGroup = OG;
        header.opCode = OC;
        header.len = msgLen;

        /* Copy message Header */
        FLib_MemCpy(pBuff, &header, sizeof(header));
        index += (uint16_t)sizeof(header);
        /* Copy message payload */
        FLib_MemCpy(pBuff+index, pMsg, msgLen);
        index += msgLen;
        checksum = FSCI_computeChecksum(pBuff+1, sizeof(header)-1u + msgLen);
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
fsci_packetStatus_t FSCI_checkPacket( clientPacket_t *pData, uint16_t bytes, uint8_t* pVIntf )
{
    uint8_t checksum = 0;
    uint16_t len;
    fsci_packetStatus_t status = FRAMING_ERROR;

    assert(pData);
    if ( bytes < MIN_VALID_PACKET_LEN )
    {
        status = PACKET_IS_TO_SHORT;            /* Too short to be valid. */
    }
    else if ( bytes >= sizeof(clientPacket_t) )
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
        if ( len > gFsciMaxPayloadLen_c )
        {
            status = FRAMING_ERROR;
        }
        else
        {
            if ( bytes < (len + sizeof(clientPacketHdr_t) + (uint16_t)sizeof(checksum)) )
            {
                status = PACKET_IS_TO_SHORT;
            }
            else /* If the length looks right, make sure that the checksum is correct. */
            {
                checksum = FSCI_computeChecksum(pData->raw+1, (uint16_t)(len + sizeof(clientPacketHdr_t)-1u));
                *pVIntf = pData->structured.payload[len] - checksum;

                if( bytes == (uint16_t)(len + sizeof(clientPacketHdr_t) + (uint16_t)sizeof(checksum)) )
                {
                    if( 0u == *pVIntf )
                    {
                        status = PACKET_IS_VALID;
                    }
                    #if (gFsciMaxVirtualInterfaces_c > 0)
                    else
                    {
                        if( *pVIntf < (uint8_t)gFsciMaxVirtualInterfaces_c )
                        {
                            status = PACKET_IS_TO_SHORT;
                        }
                    }
                    #endif
                }

                #if (gFsciMaxVirtualInterfaces_c > 0)
                if(FRAMING_ERROR == status)
                {
                    /* Check virtual interface */
                    if( bytes == len + sizeof(clientPacketHdr_t) + 2u*sizeof(checksum) )
                    {
                        checksum ^= checksum + *pVIntf;
                        if( pData->structured.payload[len+1u] == checksum )
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
uint8_t FSCI_computeChecksum( const void *pBuffer, uint16_t size )
{
    uint16_t index;
    uint8_t  checksum = 0;

    /* Compute the CRC from Opcode byte */
    for ( index = 0; index < size; index++ )
    {
        checksum ^= ((const uint8_t*)pBuffer)[index];
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
uint32_t FSCI_encodeEscapeSeq(const uint8_t* pDataIn, uint32_t len, uint8_t* pDataOut )
{
    uint32_t index, new_index = 0;

    if( NULL != pDataOut )
    {
        for ( index = 0; index < len; index++ )
        {
            if( (pDataIn[index] == gFSCI_StartMarker_c) ||
                (pDataIn[index] == gFSCI_EndMarker_c)   ||
                (pDataIn[index] == gFSCI_EscapeChar_c) )
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
void FSCI_decodeEscapeSeq( uint8_t* pData, uint32_t len )
{
    uint32_t index, new_index;

    /* Find the first gFSCI_EscapeChar_c */
    for ( index = 0; index < len; index++ )
    {
        if ( pData[index] == gFSCI_EscapeChar_c )
        {
            break;
        }
    }

    new_index = index;

    /* If a gFSCI_EscapeChar_c was found, decode the packet in place */
    while ( index < len )
    {
        if ( pData[index] == gFSCI_EscapeChar_c )
        {
            index++; /* skip over the gFSCI_EscapeChar_c */

            if ( index < len )
            {
                pData[new_index++] = pData[index++] ^ gFSCI_EscapeChar_c;
            }
        }
        else if ( new_index != index )
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

#if gFsciRxAck_c && gFsciRxAckTimeoutUseTmr_c
/*! *********************************************************************************
* \brief  This function is the callback of an Ack wait expire for a fsci interface
*
* \param[in]  param fsci interface on which the Ack was expected
*
********************************************************************************** */
static void FSCI_RxAckExpireCb(void *param)
{
    if( mFsciCommData[(uint32_t)(uint32_t*)param].txRetryCnt )
    {
        mFsciCommData[(uint32_t)(uint32_t*)param].txRetryCnt--;
    }

    /* Allow retransmission */
    mFsciCommData[(uint32_t)(uint32_t*)param].ackWaitOngoing = FALSE;
}
#endif

#if gFsciRxTimeout_c
/*! *********************************************************************************
* \brief  This function is the callback of an Rx timeout expired for a fsci interface
*
* \param[in]  param fsci interface on which no bytes were received
*
********************************************************************************** */
static void FSCI_RxRxTimeoutCb(void *param)
{
    /* Do the cleanup in case no other character is received */
    fsciComm_t *pCommData = &mFsciCommData[(uint32_t)(uint32_t*)param];

    OSA_InterruptDisable();
#if gNvStorageIncluded_d
    NvClearCriticalSection();
#endif
    pCommData->rxOngoing = FALSE;
    /* Check if free buffer size for timeout */
    if( pCommData->pPacketFromClient != (void*)&pCommData->pktHeader )
    {
        (void)MEM_BufferFree(pCommData->pPacketFromClient);
    }
    pCommData->pPacketFromClient = NULL;
    OSA_InterruptEnable();
}
#endif

/*! *********************************************************************************
* \brief  This function is used to check the RX timeout with polling
*
* \param[in]  param fsci interface
*
********************************************************************************** */
#if mFsciRxTimeoutUsePolling_c
static void FSCI_CheckRxTimeoutPolling(void* param)
{
    fsciComm_t *pCommData = &mFsciCommData[(uint32_t)(uint32_t*)param];
    uint64_t currentTs = 0U;

    if( pCommData->rxOngoing )
    {
        currentTs = TMR_GetTimestamp();
        /* Check for timeout */
        if( ((currentTs - pCommData->lastRxByteTs) / 1000u)  > mFsciRxRestartTimeoutMs_c )
        {
            /* call the RX timeout callback to do the cleanup */
            FSCI_RxRxTimeoutCb(param);
        }
    }
}
#endif

/*! *********************************************************************************
* \brief  This function is used to send a FSCI packet to the serial manager
*
* \param[in]  fsciInterface fsci interface on which the packet is to be sent
* \param[in]  pPacket serial packet to be sent
* \param[in]  packetLen lenght of the serial packet in bytes
*
********************************************************************************** */
static void FSCI_SendPacketToSerialManager(uint32_t fsciInterface, uint8_t *pPacket, uint16_t packetLen)
{
#if gFsciRxAck_c
    fsciComm_t     *pCommData = &mFsciCommData[fsciInterface];
#if !gFsciRxAckTimeoutUseTmr_c
    uint64_t        ackWaitStartTs;
    uint64_t        currentTs;
#endif
#endif

#if gFsciRxAck_c

    (void)OSA_MutexLock(pCommData->syncTxRxAckMutexId, osaWaitForever_c);

    pCommData->ackReceived = FALSE;
    pCommData->txRetryCnt = mFsciTxRetryCnt_c;

    while( pCommData->txRetryCnt != 0u )
    {
        (void)Serial_SyncWrite(gFsciSerialInterfaces[fsciInterface], pPacket, packetLen);
        pCommData->ackWaitOngoing = TRUE;

        /* Allow the FSCI interface to receive ACK packet,
           if last packet was received on the same interface */
        if( mFsciSrcInterface == fsciInterface )
        {
            pCommData->pPacketFromClient = NULL;
        }

#if gFsciRxAckTimeoutUseTmr_c
        /* Start timer for ACK wait */
        TMR_StartSingleShotTimer(pCommData->ackWaitTmr, mFsciRxAckTimeoutMs_c, FSCI_RxAckExpireCb, (void*)fsciInterface);

        /* Wait for timer to expire or Ack to be received */
        while( pCommData->ackWaitOngoing && !pCommData->ackReceived )
        {
            FSCI_receivePacket((uint32_t*)fsciInterface);
        }

        if( pCommData->ackReceived )
        {
            (void)TMR_StopTimer(pCommData->ackWaitTmr);
            pCommData->ackWaitOngoing = FALSE;
            break;
        }
#else
        /* Get initial timestamp and poll until mFsciRxAckTimeoutMs_c has passed */
        ackWaitStartTs = TMR_GetTimestamp();

        while( !pCommData->ackReceived )
        {
            FSCI_receivePacket((uint32_t*)fsciInterface);
            currentTs = TMR_GetTimestamp();
            if( ((currentTs - ackWaitStartTs) / 1000u)  > mFsciRxAckTimeoutMs_c )
            {
                pCommData->ackWaitOngoing = FALSE;
                if( pCommData->txRetryCnt != 0u )
                {
                    pCommData->txRetryCnt--;
                }
                break; /* Timeout expired */
            }
        }

        if( pCommData->ackReceived )
        {
            pCommData->ackWaitOngoing = FALSE;
            break; /* Success */
        }
#endif
    }

    (void)MEM_BufferFree(pPacket);

    (void)OSA_MutexUnlock(pCommData->syncTxRxAckMutexId);
#else /* gFsciRxAck_c */
#if gFsciUseBlockingTx_c
    if( gFsciTxBlocking )
    {
        (void)Serial_SyncWrite(gFsciSerialInterfaces[fsciInterface], pPacket, packetLen);
        (void)MEM_BufferFree(pPacket);
    }
    else
#endif /* gFsciUseBlockingTx_c */
    {
        if(gSerial_Success_c != Serial_AsyncWrite( gFsciSerialInterfaces[fsciInterface], pPacket, packetLen, (pSerialCallBack_t)FSCI_txCallback, pPacket))
        {
            (void)MEM_BufferFree(pPacket);
        }
    }
#endif /* gFsciRxAck_c */
}

#endif /* gFsciIncluded_c */
