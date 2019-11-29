/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
*/



/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"

#include "PhyInterface.h"
#include "Phy.h"
#include "MemManager.h"
#include "Messaging.h"
#include "FunctionLib.h"

#include "AspInterface.h"
#include "MpmInterface.h"

#ifndef gMWS_Enabled_d
#define gMWS_Enabled_d 0
#endif

#ifndef gMWS_UseCoexistence_d
#define gMWS_UseCoexistence_d 0
#endif

#if (gMWS_Enabled_d) || (gMWS_UseCoexistence_d)
#include "MWS.h"
#include "fsl_xcvr.h"
#include "gpio_pins.h"
#endif

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define mPhyMaxIdleRxDuration_c         (0xF00000) /* [sym] */
#define mPhyOverhead_d                  (60)       /* [sym] */
#define mPhyMaxFrameDuration_d (gPhySHRDuration_c + (1 + gMaxPHYPacketSize_c) * gPhySymbolsPerOctet_c + gPhyTurnaroundTime_c + 54)
//#define mPhyMinRxDuration_d mPhyMaxFrameDuration_d
#define mPhyMinRxDuration_d (gPhyTurnaroundTime_c + 22)

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static void Phy24Task(Phy_PhyLocalStruct_t *pPhyStruct);

static phyStatus_t Phy_HandlePdDataReq( Phy_PhyLocalStruct_t *pPhyData, macToPdDataMessage_t * pMsg );

static void Phy_EnterIdle( Phy_PhyLocalStruct_t *pPhyData );

static void PLME_SendMessage(Phy_PhyLocalStruct_t *pPhyStruct, phyMessageId_t msgType);

static void PD_SendMessage(Phy_PhyLocalStruct_t *pPhyStruct, phyMessageId_t msgType);

static void Phy_SendLatePD( uint32_t param );
static void Phy_SendLatePLME( uint32_t param );

#if (gMWS_Enabled_d) || (gMWS_UseCoexistence_d)
static uint32_t MWS_802_15_4_Callback ( mwsEvents_t event );
#endif

#if (gMWS_Enabled_d)
static uint32_t Phy_GetSeqDuration(phyMessageHeader_t * pMsg);
#endif


/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
Phy_PhyLocalStruct_t phyLocal;
uint8_t mXcvrDisallowSleep = 0;
const uint8_t gPhyPoolId = gPhyPoolId_d;

#if gMWS_Enabled_d
uint8_t mXcvrAcquired = 0;
#endif

#if gPhyUseNeighborTable_d
extern const uint8_t gPhyHwIndQueueSize_c;
#endif


/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  This function creates the PHY task
*
********************************************************************************** */
void Phy_Init(void)
{
    PhyHwInit();
    PhyTime_TimerInit(NULL);
    ASP_Init( 0 );
    MPM_Init();

    phyLocal.flags = gPhyFlagDeferTx_c;
    phyLocal.rxParams.pRxData = NULL;
    
    /* Prepare input queues.*/
    MSG_InitQueue( &phyLocal.macPhyInputQueue );

    PhyIsrPassRxParams( NULL );
    PhyPlmeSetPwrState( gPhyDefaultIdlePwrMode_c );
#if gMWS_Enabled_d
    MWS_Register(gMWS_802_15_4_c, MWS_802_15_4_Callback);
#endif
#if gMWS_UseCoexistence_d
    MWS_CoexistenceInit(&gCoexistence_RfDeny, &gCoexistence_RfActive, &gCoexistence_RfStatus);
    MWS_CoexistenceRegister(gMWS_802_15_4_c, MWS_802_15_4_Callback);
#endif
}

/*! *********************************************************************************
* \brief  This function binds a MAC instance to a PHY instance
*
* \param[in]  instanceId The instance of the MAC
*
* \return  The instance of the PHY.
*
********************************************************************************** */
instanceId_t BindToPHY( instanceId_t macInstance )
{
    return 0;
}

/*! *********************************************************************************
* \brief  This function registers the MAC PD and PLME SAP handlers
*
* \param[in]  pPD_MAC_SapHandler   Pointer to the MAC PD handler function
* \param[in]  pPLME_MAC_SapHandler Pointer to the MAC PLME handler function
* \param[in]  instanceId           The instance of the PHY
*
* \return  The status of the operation.
*
********************************************************************************** */
void Phy_RegisterSapHandlers( PD_MAC_SapHandler_t pPD_MAC_SapHandler,
                              PLME_MAC_SapHandler_t pPLME_MAC_SapHandler,
                              instanceId_t instanceId )
{
    instanceId = instanceId;
    phyLocal.PD_MAC_SapHandler = pPD_MAC_SapHandler;
    phyLocal.PLME_MAC_SapHandler = pPLME_MAC_SapHandler;
}

/*! *********************************************************************************
* \brief  This function represents the PHY's task
*
* \param[in]  taskParam The instance of the PHY
*
********************************************************************************** */
static void Phy24Task(Phy_PhyLocalStruct_t *pPhyStruct)
{
    uint8_t state;
    phyMessageHeader_t * pMsgIn = NULL;
    phyStatus_t status;

    //ProtectFromXcvrInterrupt();
    OSA_InterruptDisable();
    
    /* Handling messages from upper layer */
    while( MSG_Pending(&pPhyStruct->macPhyInputQueue) )
    {
        status = gPhySuccess_c;
        state = PhyGetSeqState();
        
        /* Check if PHY is busy */
        if ((state != gIdle_c) && (state != gRX_c))
        {
            status = gPhyBusy_c;
            break;
        }
        else
        {
            uint8_t seq_state = ZLL->SEQ_STATE & 0x1F;

            /* PHY doesn't free dynamic alocated messages! */
            pMsgIn = MSG_DeQueue( &pPhyStruct->macPhyInputQueue );
            pPhyStruct->currentMacInstance = pMsgIn->macInstance;

#if gMWS_Enabled_d
            /* Dual Mode */
            if( (Phy_GetSeqDuration(pMsgIn) + mPhyOverhead_d) <= (MWS_GetInactivityDuration(gMWS_802_15_4_c) / 16) )
            {
                if( !mXcvrAcquired )
                {
                    if( gMWS_Success_c == MWS_Acquire(gMWS_802_15_4_c, FALSE) )
                    {
                        mXcvrAcquired = 1;
                    }
                    else
                    {
                        status = gPhyBusy_c;
                    }
                }
            }
            else
            {
                status = gPhyBusy_c;
                
                if( mXcvrAcquired && (state == gIdle_c) )
                {
                    mXcvrAcquired = 0;
                    MWS_Release(gMWS_802_15_4_c);
                }
            }
#endif

            /* Check if Radio is busy */
            if (seq_state && ((seq_state <= 0x06) || (seq_state == 0x15) || (seq_state == 0x16)))
            {
                if (pMsgIn->msgType == gPdDataReq_c)
                {
                    macToPdDataMessage_t *pPD = (macToPdDataMessage_t*)pMsgIn;
                    
                    if ((pPD->msgData.dataReq.CCABeforeTx != gPhyNoCCABeforeTx_c) && 
                        (pPD->msgData.dataReq.startTime == gPhySeqStartAsap_c))
                    {
                        if (pPhyStruct->flags & gPhyFlagDeferTx_c)
                        {
                            /* Postpone TX until the Rx has finished */
                            pPhyStruct->flags |= gPhyFlaqReqPostponed_c;
                            status = gPhyBusy_c;
                        }
                        else
                        {
                            phyLocal.channelParams.channelStatus = gPhyChannelBusy_c;
                            PLME_SendMessage(&phyLocal, gPlmeCcaCnf_c);
                            status = gPhyBusyRx_c;
                        }
                    }
                }
                else if (pMsgIn->msgType == gPlmeCcaReq_c)
                {
                    phyLocal.channelParams.channelStatus = gPhyChannelBusy_c;
                    PLME_SendMessage(&phyLocal, gPlmeCcaCnf_c);
                    status = gPhyBusyRx_c;
                }
            }
        }

        if (gPhyBusy_c == status)
        {
            /* Will be triggered on the next event */
            MSG_QueueHead (&pPhyStruct->macPhyInputQueue, pMsgIn);
            break;
        }
        else if (status == gPhySuccess_c)
        {
            if (gRX_c == state)
            {        
                state = gIdle_c;
                PhyPlmeForceTrxOffRequest();
            }
            
#if gMpmIncluded_d
            status = MPM_PrepareForTx( pMsgIn->macInstance );
#endif
            
            if (status == gPhySuccess_c)
            {
                pPhyStruct->flags &= ~(gPhyFlagIdleRx_c | gPhyFlaqReqPostponed_c);
                
                switch( pMsgIn->msgType )
                {
                case gPdDataReq_c:
                    status = Phy_HandlePdDataReq( pPhyStruct, (macToPdDataMessage_t *)pMsgIn );
                    break;
                case gPlmeCcaReq_c:
                    status = PhyPlmeCcaEdRequest(gPhyCCAMode1_c, gPhyContCcaDisabled);
                    break;
                case gPlmeEdReq_c:
                    status = PhyPlmeCcaEdRequest(gPhyEnergyDetectMode_c, gPhyContCcaDisabled);
                    break;
                default:
                    status = gPhyInvalidPrimitive_c;
                    break;
                }
            }
            
            /* Check status */
            if( gPhySuccess_c != status )
            {
                switch( pMsgIn->msgType )
                {
                case gPdDataReq_c:
                    pPhyStruct->channelParams.channelStatus = gPhyChannelBusy_c;
                    PLME_SendMessage(pPhyStruct, gPlmeCcaCnf_c);
                    //PD_SendMessage(pPhyStruct, gPdDataCnf_c);
                    break;
                    /* Fallthorough */
                case gPlmeCcaReq_c:
                    pPhyStruct->channelParams.channelStatus = gPhyChannelBusy_c;
                    PLME_SendMessage(pPhyStruct, gPlmeCcaCnf_c);
                    break;
                case gPlmeEdReq_c:
                    pPhyStruct->channelParams.energyLeveldB = 0;
                    PLME_SendMessage(pPhyStruct, gPlmeEdCnf_c);
                    break;
                default:
                    PLME_SendMessage(pPhyStruct, gPlmeTimeoutInd_c);
                    break;
                }
            }
        }
    }/* while( MSG_Pending(&pPhyStruct->macPhyInputQueue) ) */
    //UnprotectFromXcvrInterrupt();
    OSA_InterruptEnable();
    /* Check if PHY can enter Idle state */
    if( gIdle_c == PhyGetSeqState() )
    {
        Phy_EnterIdle( pPhyStruct );
    }

}

/*! *********************************************************************************
* \brief  This is the PD SAP message handler
*
* \param[in]  pMsg Pointer to the PD request message
* \param[in]  instanceId The instance of the PHY
*
* \return  The status of the operation.
*
********************************************************************************** */
phyStatus_t MAC_PD_SapHandler(macToPdDataMessage_t *pMsg, instanceId_t phyInstance)
{
    phyStatus_t result = gPhySuccess_c;
    uint8_t baseIndex = 0;

    phyInstance = phyInstance;

    if( NULL == pMsg )
    {
        result = gPhyInvalidParameter_c;
    }
    else
    {
#if gMpmIncluded_d
        if( pMsg->msgType == gPdIndQueueInsertReq_c || pMsg->msgType == gPdIndQueueRemoveReq_c )
        {
            baseIndex = MPM_GetRegSet( MPM_GetPanIndex(pMsg->macInstance) );
            baseIndex = 
#if gPhyUseNeighborTable_d
                baseIndex * (gPhyHwIndQueueSize_c/gMpmPhyPanRegSets_c) +
#endif
                baseIndex * (gPhyIndirectQueueSize_c/gMpmPhyPanRegSets_c);
        }
#endif
        
        switch( pMsg->msgType )
        {
        case gPdIndQueueInsertReq_c:
            result = PhyPp_IndirectQueueInsert(baseIndex + pMsg->msgData.indQueueInsertReq.index,
                                               pMsg->msgData.indQueueInsertReq.checksum,
                                               phyInstance);
            break;
            
        case gPdIndQueueRemoveReq_c:
            result = PhyPp_RemoveFromIndirect(baseIndex + pMsg->msgData.indQueueRemoveReq.index,
                                              phyInstance);
            break;
            
        case gPdDataReq_c:
            MSG_Queue(&phyLocal.macPhyInputQueue, pMsg);
            //Phy24Task( &phyLocal );
            PHY_ForceIrqPending();
            break;
            
        default:
            result = gPhyInvalidPrimitive_c;
            break;
        }
    }
    
    return result;
}

/*! *********************************************************************************
* \brief  This is the PLME SAP message handler
*
* \param[in]  pMsg Pointer to the PLME request message
* \param[in]  instanceId The instance of the PHY
*
* \return  phyStatus_t The status of the operation.
*
********************************************************************************** */
phyStatus_t MAC_PLME_SapHandler(macToPlmeMessage_t * pMsg, instanceId_t phyInstance)
{
    Phy_PhyLocalStruct_t *pPhyStruct = &phyLocal;
    phyStatus_t result = gPhySuccess_c;
    uint8_t phyRegSet = 0;
#if gMpmIncluded_d
    int32_t panIdx;
#endif

    if( NULL == pMsg )
    {
        result = gPhyInvalidParameter_c;
    }
    else
    {
#if gMpmIncluded_d
        panIdx = MPM_GetPanIndex( pMsg->macInstance );
        phyRegSet = MPM_GetRegSet( panIdx );
#endif
        switch( pMsg->msgType )
        {
        case gPlmeEdReq_c:
        case gPlmeCcaReq_c:
            MSG_Queue(&phyLocal.macPhyInputQueue, pMsg);
            //Phy24Task( &phyLocal );
            PHY_ForceIrqPending();
            break;
            
        case gPlmeSetReq_c:
#if gMpmIncluded_d
            result = MPM_SetPIB(pMsg->msgData.setReq.PibAttribute,
                                &pMsg->msgData.setReq.PibAttributeValue,
                                panIdx );
            if( !MPM_isPanActive(panIdx) )
            {
                break;
            }
#endif
            result = PhyPlmeSetPIBRequest(pMsg->msgData.setReq.PibAttribute, pMsg->msgData.setReq.PibAttributeValue, phyRegSet, phyInstance);
            break;
            
        case gPlmeGetReq_c:
#if gMpmIncluded_d
            if( gPhySuccess_c == MPM_GetPIB(pMsg->msgData.getReq.PibAttribute, pMsg->msgData.getReq.pPibAttributeValue, panIdx) )
            {
                break;
            }
#endif
            result = PhyPlmeGetPIBRequest( pMsg->msgData.getReq.PibAttribute, pMsg->msgData.getReq.pPibAttributeValue, phyRegSet, phyInstance);
            break;
            
        case gPlmeSetTRxStateReq_c:
            if(gPhySetRxOn_c == pMsg->msgData.setTRxStateReq.state)
            {
                /* Compensate Rx warmup time */
                if( pMsg->msgData.setTRxStateReq.startTime != gPhySeqStartAsap_c )
                {
                    pMsg->msgData.setTRxStateReq.startTime -= ((XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT) >> 4; /* /16 */
                }

                pMsg->msgData.setTRxStateReq.rxDuration += ((XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT) >> 4; /* /16 */
                
#if gMWS_Enabled_d
                if( (MWS_GetInactivityDuration(gMWS_802_15_4_c) / 16) < (pMsg->msgData.setTRxStateReq.rxDuration + mPhyOverhead_d) )
                {
                    result = gPhyBusy_c;
                    break;
                }
#endif
                if( PhyIsIdleRx(phyInstance) )
                {
                    PhyPlmeForceTrxOffRequest();
                }
                else 
                {
                    if( gIdle_c != PhyGetSeqState() )
                    {
                        result = gPhyBusy_c;
                        break;
                    }
                }
#if gMpmIncluded_d
                /* If another PAN has the RxOnWhenIdle PIB set, enable the DualPan Auto mode */
                if( gPhySuccess_c != MPM_PrepareForRx( pMsg->macInstance ) )
                {
                    result = gPhyBusy_c;
                    break;
                }
#endif
                pPhyStruct->flags &= ~(gPhyFlagIdleRx_c);
                
                Phy_SetSequenceTiming(pMsg->msgData.setTRxStateReq.startTime,
                                      pMsg->msgData.setTRxStateReq.rxDuration);
                
                result = PhyPlmeRxRequest(pMsg->msgData.setTRxStateReq.slottedMode, (phyRxParams_t *) &pPhyStruct->rxParams);
                break;
            }
            else
            {
                if (gPhyForceTRxOff_c == pMsg->msgData.setTRxStateReq.state)
                {
#if gMpmIncluded_d
                    if( !MPM_isPanActive(panIdx) )
                    {
                        break;
                    }
#endif
                    pPhyStruct->flags &= ~(gPhyFlagIdleRx_c);
                    PhyPlmeForceTrxOffRequest();
#if gMWS_Enabled_d
                    mXcvrAcquired = 0;
                    MWS_Release(gMWS_802_15_4_c);
#endif
                }
            }
            break;
            
        default:
            result = gPhyInvalidPrimitive_c;
            break;
        }
    }
    return result;
}

/*! *********************************************************************************
* \brief  This function programs a new TX sequence
*
* \param[in]  pMsg Pointer to the PD request message
* \param[in]  pPhyData pointer to PHY data
*
* \return  The status of the operation.
*
********************************************************************************** */
static phyStatus_t Phy_HandlePdDataReq( Phy_PhyLocalStruct_t *pPhyData, macToPdDataMessage_t * pMsg )
{
    phyStatus_t status;
    phyTime_t startTime = gPhySeqStartAsap_c;
    phyTime_t t;
    
    if( NULL == pMsg->msgData.dataReq.pPsdu )
    {
        status = gPhyInvalidParameter_c;
    }
    else
    {
        ProtectFromXcvrInterrupt();
        
        /* Compensate XCVR Tx warmup time */
        if( pMsg->msgData.dataReq.startTime != gPhySeqStartAsap_c )
        {
            startTime = pMsg->msgData.dataReq.startTime - (((XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT) >> 4); /* /16 */
            PhyTimeSetEventTrigger( startTime );
        }

        status = PhyPdDataRequest(&pMsg->msgData.dataReq , &pPhyData->rxParams, &pPhyData->txParams);
        
        t = PhyTime_GetTimestamp();
        
        if( t > startTime )
        {
            status = gPhyTRxOff_c;
        }
        else 
        {
            if( pMsg->msgData.dataReq.txDuration != 0xFFFFFFFFU )
            {
                if( startTime != gPhySeqStartAsap_c )
                {
                    t = startTime + pMsg->msgData.dataReq.txDuration;
                }
                else
                {
                    t += pMsg->msgData.dataReq.txDuration;
                }
                /* Compensate PHY overhead */
                t += ((XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT) >> 4; /* /16 */
                t += 10;
                PhyTimeSetEventTimeout( &t );
            }
        }
        
        UnprotectFromXcvrInterrupt();
        
        if( gPhySuccess_c != status )
        {
            PhyPlmeForceTrxOffRequest();
        }
    }
    return status;
}

/*! *********************************************************************************
* \brief  This function sets the start time and the timeout value for a sequence.
*
* \param[in]  startTime The absolute start time for the sequence.
*             If startTime is gPhySeqStartAsap_c, the start timer is disabled.
* \param[in]  seqDuration The duration of the sequence.
*             If seqDuration is 0xFFFFFFFF, the timeout is disabled.
*
********************************************************************************** */
void Phy_SetSequenceTiming(phyTime_t startTime, uint32_t seqDuration)
{
    phyTime_t endTime;

    OSA_InterruptDisable();

    if( gPhySeqStartAsap_c == startTime )
    {
        PhyTimeReadClock( &endTime );
    }
    else
    {
        PhyTimeSetEventTrigger( startTime );
        endTime = startTime & gPhyTimeMask_c;
    }

    if( 0xFFFFFFFFU != seqDuration )
    {
        endTime += seqDuration;
        endTime = endTime & gPhyTimeMask_c;

        PhyTimeSetEventTimeout( &(endTime) );
    }

    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  This function starts the IdleRX if the PhyRxOnWhenIdle PIB is set
*
* \param[in]  pPhyData pointer to PHY data
*
********************************************************************************** */
static void Phy_EnterIdle( Phy_PhyLocalStruct_t *pPhyData )
{
    uint32_t t = mPhyMaxIdleRxDuration_c;
    
    if( (pPhyData->flags & gPhyFlagRxOnWhenIdle_c)
#if gMpmIncluded_d
       /* Prepare the Active PAN/PANs */
       && (gPhySuccess_c == MPM_PrepareForRx(gInvalidInstanceId_c))
#endif
      )
    {
#if gMWS_Enabled_d
        t = MWS_GetInactivityDuration(gMWS_802_15_4_c) / 16; /* convert to symbols */

        if( t < (mPhyMinRxDuration_d + mPhyOverhead_d) )
        {
            pPhyData->flags &= ~(gPhyFlagIdleRx_c);
            if( mXcvrAcquired )
            {
                mXcvrAcquired = 0;
                MWS_Release(gMWS_802_15_4_c);
            }
        }
        else 
        {
            if( t > (mPhyMaxIdleRxDuration_c + mPhyOverhead_d ) ) 
            {
                t = mPhyMaxIdleRxDuration_c;
            }
            else
            {
                t -= mPhyOverhead_d;
            }
            
            if( !mXcvrAcquired )
            {
                mXcvrAcquired = (gMWS_Success_c == MWS_Acquire(gMWS_802_15_4_c, FALSE));
            }
        }

        if( mXcvrAcquired )
#endif
        {
            pPhyData->flags |= gPhyFlagIdleRx_c;
            Phy_SetSequenceTiming( gPhySeqStartAsap_c, t );
            PhyPlmeRxRequest( gPhyUnslottedMode_c, &pPhyData->rxParams );
        }
    }
    else
    {
        pPhyData->flags &= ~(gPhyFlagIdleRx_c);
#if gMWS_Enabled_d
        t = MWS_GetInactivityDuration(gMWS_802_15_4_c) / 16; /* convert to symbols */

        if (t > mPhyOverhead_d)
        {
            uint32_t time = (uint32_t)(PhyTime_GetTimestamp() + t - mPhyOverhead_d);
            PhyTimeSetWakeUpTime(&time);
        }
        else if( mXcvrAcquired )
        {
            mXcvrAcquired = 0;
            MWS_Release(gMWS_802_15_4_c);
        }
        else
        {
            PhyTime_TMR4Callback();
        }
#endif
        if( mXcvrDisallowSleep && (gIdle_c == PhyGetSeqState()) )
        {
            mXcvrDisallowSleep = 0;
            PWR_AllowXcvrToSleep();
        }
    }
}

/*! *********************************************************************************
* \brief  This function sets the value of the maxFrameWaitTime PIB
*
* \param[in]  instanceId The instance of the PHY
* \param[in]  maxTime The maxFrameWaitTime value
*
********************************************************************************** */
void PhyPlmeSetFrameWaitTime( uint32_t maxTime, instanceId_t instanceId )
{
    phyLocal.maxFrameWaitTime = maxTime;
}

/*! *********************************************************************************
* \brief  This function sets the state of the PhyRxOnWhenIdle PIB
*
* \param[in]  instanceId The instance of the PHY
* \param[in]  state The PhyRxOnWhenIdle value
*
********************************************************************************** */
void PhyPlmeSetRxOnWhenIdle( bool_t state, instanceId_t instanceId )
{
    uint8_t radioState = PhyGetSeqState();
#if gMpmIncluded_d
    /* Check if at least one PAN has RxOnWhenIdle set */
    if( FALSE == state )
    {
        uint32_t i;

        for( i=0; i<gMpmMaxPANs_c; i++ )
        {
            MPM_GetPIB( gPhyPibRxOnWhenIdle, &state, i );
            if( state )
                break;
        }
    }
#endif

    if( state )
    {
        phyLocal.flags |= gPhyFlagRxOnWhenIdle_c;
#if gMpmIncluded_d
        if( (radioState == gRX_c) && (phyLocal.flags & gPhyFlagIdleRx_c) )
        {
            PhyPlmeForceTrxOffRequest();
        }
#endif
    }
    else
    {
        phyLocal.flags &= ~gPhyFlagRxOnWhenIdle_c;
        if( (phyLocal.flags & gPhyFlagIdleRx_c) && (radioState == gRX_c) )
        {
            PhyPlmeForceTrxOffRequest();
        }
    }

    if( gIdle_c == PhyGetSeqState() )
    {
        Phy_EnterIdle( &phyLocal );
    }
}

/*! *********************************************************************************
* \brief  This function starts the IdleRX if the PhyRxOnWhenIdle PIB is set
*
* \param[in]  instanceId The instance of the PHY
*
********************************************************************************** */
bool_t PhyIsIdleRx( instanceId_t instanceId )
{
    bool_t status = FALSE;
    uint8_t state = PhyGetSeqState();

    if( (phyLocal.flags & gPhyFlagIdleRx_c) && (gRX_c == state) )
    {
        status = TRUE;
    }

    return status;
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that a TX operation completed successfully.
*         If the received ACK has FP=1, then the radio will enter RX state for
*         maxFrameWaitTime duration.
*
* \param[in]  instanceId The instance of the PHY
* \param[in]  framePending The value of the framePending bit for the received ACK
*
********************************************************************************** */
void Radio_Phy_PdDataConfirm(instanceId_t instanceId, bool_t framePending)
{
    PhyTimeDisableEventTimeout();

    if( framePending )
    {
        phyLocal.flags |= gPhyFlagRxFP_c;

        if( phyLocal.maxFrameWaitTime > 0 )
        {
            /* Restart Rx asap if an ACK with FP=1 is received */
            phyLocal.flags &= ~(gPhyFlagIdleRx_c);
            Phy_SetSequenceTiming( gPhySeqStartAsap_c, phyLocal.maxFrameWaitTime );
            PhyPlmeRxRequest( gPhyUnslottedMode_c, &phyLocal.rxParams );
        }
    }
    else
    {
        phyLocal.flags &= ~gPhyFlagRxFP_c;
    }

    PD_SendMessage(&phyLocal, gPdDataCnf_c);
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that new data has been received
*
* \param[in]  instanceId The instance of the PHY
*
********************************************************************************** */
void Radio_Phy_PdDataIndication(instanceId_t instanceId)
{
    PhyTimeDisableEventTimeout();

    PD_SendMessage(&phyLocal, gPdDataInd_c);
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that timer1 compare match occured
*
* \param[in]  instanceId The instance of the PHY
*
********************************************************************************** */
void Radio_Phy_TimeWaitTimeoutIndication(instanceId_t instanceId)
{
    PhyTime_ISR();
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that a CCA sequence has finished
*
* \param[in]  instanceId The instance of the PHY
* \param[in]  phyChannelStatus The status of the channel: Idle/Busy
*
* \return  None.
*
********************************************************************************** */
void Radio_Phy_PlmeCcaConfirm(phyStatus_t phyChannelStatus, instanceId_t instanceId)
{
    PhyTimeDisableEventTimeout();

    phyLocal.channelParams.channelStatus = phyChannelStatus;

    PLME_SendMessage(&phyLocal, gPlmeCcaCnf_c);
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that a ED sequence has finished
*
* \param[in]  instanceId The instance of the PHY
* \param[in]  energyLevel The enetgy level on the channel.
* \param[in]  energyLeveldB The energy level in DB
*
********************************************************************************** */
void Radio_Phy_PlmeEdConfirm(uint8_t energyLeveldB, instanceId_t instanceId)
{
    PhyTimeDisableEventTimeout();

    phyLocal.channelParams.energyLeveldB = energyLeveldB;

    PLME_SendMessage(&phyLocal, gPlmeEdCnf_c);
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that the programmed sequence has timed out
*         The Radio is forced to Idle.
*
* \param[in]  instanceId The instance of the PHY
*
********************************************************************************** */
void Radio_Phy_TimeRxTimeoutIndication(instanceId_t instanceId)
{
    if( (phyLocal.flags & gPhyFlagIdleRx_c) != gPhyFlagIdleRx_c )
    {
        PLME_SendMessage(&phyLocal, gPlmeTimeoutInd_c);
    }
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that the programmed sequence has started
*
* \param[in]  instanceId The instance of the PHY
*
* \return  None.
*
********************************************************************************** */
void Radio_Phy_TimeStartEventIndication(instanceId_t instanceId)
{
#ifdef MAC_PHY_DEBUG
    PLME_SendMessage(&phyLocal, gPlme_StartEventInd_c);
#endif
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that the specified Rx watermark has been reached.
*         Also, if there is not enough time to receive the entire packet, the
*         RX timeout will be extended.
*
* \param[in]  instanceId The instance of the PHY
* \param[in]  frameLen the length of the PSDU
*
********************************************************************************** */
void Radio_Phy_PlmeRxWatermark(instanceId_t instanceId, uint32_t frameLength)
{
    /* In DualMode operation, sequence timeouts are strict, and cannot be extended. */
    if( ((phyLocal.flags & gPhyFlagDeferTx_c) == gPhyFlagDeferTx_c) && (frameLength <= gMaxPHYPacketSize_c) )
    {
        phyTime_t currentTime, t;

#if (gMWS_Enabled_d) || (gMWS_UseCoexistence_d)
        uint32_t inactiveTime = MWS_GetInactivityDuration(gMWS_802_15_4_c) / 16; /* [symbols] */
#endif

        OSA_InterruptDisable();

        /* Read currentTime and Timeout values [sym] */
        PhyTimeReadClock(&currentTime);
        /* Convert to symbols and add IFS and ACK duration */
        frameLength = frameLength * 2 + 54;

        t = (PhyTimeGetEventTimeout() - currentTime) & gPhyTimeMask_c;

#if (gMWS_Enabled_d) || (gMWS_UseCoexistence_d)
        if ((t > 1) && (inactiveTime > (frameLength + mPhyOverhead_d)))
#else
        if (t > 1)
#endif
        {
            /* Disable TMR3 compare */
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR3CMP_EN_MASK;
            /* Write new TMR3 compare value */
            currentTime += frameLength;
            ZLL->T3CMP = currentTime;
            /* Enable TMR3 compare */
            ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TMR3CMP_EN_MASK;
        }

        OSA_InterruptEnable();
    }

#ifdef MAC_PHY_DEBUG
    PLME_SendMessage(&phyLocal, gPlme_RxSfdDetectInd_c);
#endif
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that a Sync Loss occured (PLL unlock)
*         The Radio is forced to Idle.
*
* \param[in]  instanceId The instance of the PHY
*
********************************************************************************** */
void Radio_Phy_PlmeSyncLossIndication(instanceId_t instanceId)
{
    PhyPlmeForceTrxOffRequest();
#ifdef MAC_PHY_DEBUG
    PLME_SendMessage(&phyLocal, gPlme_SyncLossInd_c);
#endif
    Radio_Phy_TimeRxTimeoutIndication(instanceId);
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that a Filter Fail occured
*
* \param[in]  instanceId The instance of the PHY
*
********************************************************************************** */
void Radio_Phy_PlmeFilterFailRx(instanceId_t instanceId)
{
    if (phyLocal.flags & gPhyFlaqReqPostponed_c)
    {
        /* The Rx packet is not intended for the current device.
         * Signal a Channel Busy event, and discard the Tx request */
        phyLocal.flags &= ~(gPhyFlaqReqPostponed_c);
        if (MSG_DeQueue(&phyLocal.macPhyInputQueue))
        {
            Radio_Phy_PlmeCcaConfirm(gPhyChannelBusy_c, instanceId);
        }
    }
#ifdef MAC_PHY_DEBUG
    PLME_SendMessage(&phyLocal, gPlme_FilterFailInd_c);
#endif
}

/*! *********************************************************************************
* \brief  This function signals the PHY task that an unexpected Transceiver Reset
*          occured and force the TRX to Off
*
* \param[in]  instanceId The instance of the PHY
*
********************************************************************************** */
void Radio_Phy_UnexpectedTransceiverReset(instanceId_t instanceId)
{
    PhyPlmeForceTrxOffRequest();
#ifdef MAC_PHY_DEBUG
    PLME_SendMessage(&phyLocal, gPlme_UnexpectedRadioResetInd_c);
#endif
    Radio_Phy_TimeRxTimeoutIndication(instanceId);
}

/*! *********************************************************************************
* \brief  Senf a PLME message to upper layer
*
* \param[in]  instanceId The instance of the PHY
* \param[in]  msgType    The type of message to be sent
*
********************************************************************************** */
static void PLME_SendMessage(Phy_PhyLocalStruct_t *pPhyStruct, phyMessageId_t msgType)
{
    plmeToMacMessage_t * pMsg;
    
    pPhyStruct->flags &= ~(gPhyFlaqReqPostponed_c);
    
    if( pPhyStruct->PLME_MAC_SapHandler )
    {
        pMsg = Phy_BufferAlloc(sizeof(plmeToMacMessage_t));
        
        if(NULL == pMsg)
        {
            phyTimeEvent_t ev;
            
            ev.parameter = (uint32_t)msgType;
            ev.callback = Phy_SendLatePLME;
            ev.timestamp = gPhyRxRetryInterval_c + PhyTime_GetTimestamp();
            PhyTime_ScheduleEvent(&ev);
        }
        else
        {
            pMsg->msgType = msgType;
            
            switch(msgType)
            {
            case gPlmeCcaCnf_c:
                pMsg->msgData.ccaCnf.status = pPhyStruct->channelParams.channelStatus;
                break;
                
            case gPlmeEdCnf_c:
                pMsg->msgData.edCnf.status        = gPhySuccess_c;
                pMsg->msgData.edCnf.energyLeveldB = pPhyStruct->channelParams.energyLeveldB;
                pMsg->msgData.edCnf.energyLevel   = Phy_GetEnergyLevel(pPhyStruct->channelParams.energyLeveldB);
                break;
                
            default:
                /* No aditional info needs to be filled */
                break;
            }
            
            pPhyStruct->PLME_MAC_SapHandler(pMsg, pPhyStruct->currentMacInstance);
        }
    }
}

/*! *********************************************************************************
* \brief  Senf a PD message to upper layer
*
* \param[in]  instanceId The instance of the PHY
* \param[in]  msgType    The type of message to be sent
*
********************************************************************************** */
static void PD_SendMessage(Phy_PhyLocalStruct_t *pPhyStruct, phyMessageId_t msgType)
{
    pdDataToMacMessage_t *pMsg;
    
    if( pPhyStruct->PD_MAC_SapHandler )
    {
        if( msgType == gPdDataInd_c )
        {
            uint32_t i;
            uint16_t len = pPhyStruct->rxParams.psduLength - 2; /* Excluding FCS (2 bytes) */
            
            pMsg = pPhyStruct->rxParams.pRxData;
            pPhyStruct->rxParams.pRxData = NULL;

            /* Copy PB data byte by byte to avoid memory access issues */
            for (i = 0; i < len; i++)
            {
                pMsg->msgData.dataInd.pPsdu[i] = ((uint8_t*)ZLL->PKT_BUFFER_RX)[i];
            }

            if (pPhyStruct->flags & gPhyFlaqReqPostponed_c)
            {
                uint8_t *pPsdu = pMsg->msgData.dataInd.pPsdu;
                uint8_t  dstAddrMode = (pPsdu[mFrameCtrlHi_d] & mDstAddrModeMask_d) >> mDstAddrModeShift_d;

                /* Skip over FrameControl, SeqNo and PanID */
                pPsdu += mAddressingFields_d + 2;
                    
                if ((dstAddrMode == mShortAddr_d) && (pPsdu[0] == 0xFF) && (pPsdu[1] == 0xFF))
                {
                    /* The Rx packet is a broadcast message.
                     * Signal a Channel Busy event, and discard the Tx request */
                    pPhyStruct->flags &= ~(gPhyFlaqReqPostponed_c);
                    if (MSG_DeQueue(&pPhyStruct->macPhyInputQueue))
                    {
                        pPhyStruct->channelParams.channelStatus = gPhyChannelBusy_c;
                        PLME_SendMessage(pPhyStruct, gPlmeCcaCnf_c);
                    }
                }
            }

            pMsg->msgType = gPdDataInd_c;
            pMsg->msgData.dataInd.ppduLinkQuality = pPhyStruct->rxParams.linkQuality;
            pMsg->msgData.dataInd.psduLength = len;

            pMsg->msgData.dataInd.timeStamp = PhyTime_GetTimestamp();            /* current timestamp (64bit) */
            i = (uint32_t)(pMsg->msgData.dataInd.timeStamp & gPhyTimeMask_c); /* convert to 24bit */
            pMsg->msgData.dataInd.timeStamp -= (i - pPhyStruct->rxParams.timeStamp) & gPhyTimeMask_c;
#if !(gMpmIncluded_d)
            pPhyStruct->PD_MAC_SapHandler(pMsg, pPhyStruct->currentMacInstance);
#else
            {
                uint32_t i, bitMask = PhyPpGetPanOfRxPacket();
                
                for( i=0; i<gMpmPhyPanRegSets_c; i++ )
                {
                    if( bitMask & (1 << i) )
                    {
                        bitMask &= ~(1 << i);
                        pPhyStruct->currentMacInstance = MPM_GetMacInstanceFromRegSet(i);
                        
                        /* If the packet passed filtering on muliple PANs, send a copy to each one */
                        if( bitMask )
                        {
                            pdDataToMacMessage_t *pDataIndCopy;
                            
                            pDataIndCopy = Phy_BufferAlloc(sizeof(pdDataToMacMessage_t) + len);
                            if( pDataIndCopy )
                            {
                                FLib_MemCpy(pDataIndCopy, pMsg, sizeof(pdDataToMacMessage_t) + len);
                                pPhyStruct->PD_MAC_SapHandler(pDataIndCopy, pPhyStruct->currentMacInstance);
                            }
                        }
                        else
                        {
                            pPhyStruct->PD_MAC_SapHandler(pMsg, pPhyStruct->currentMacInstance);
                            break;
                        }
                    }
                }
            }
#endif
        }
        else
        {
            phyStatus_t status;
            
            if( (pPhyStruct->flags & gPhyFlagRxFP_c) == gPhyFlagRxFP_c )
            {
                pPhyStruct->flags &= ~(gPhyFlagRxFP_c);
                status = gPhyFramePending_c;
            }
            else
            {
                status = gPhySuccess_c;
            }
            
            pPhyStruct->flags &= ~(gPhyFlaqReqPostponed_c);
            pMsg = Phy_BufferAlloc(GetRelAddr(pdDataToMacMessage_t, msgData) + sizeof(pdDataCnf_t));

            if (NULL == pMsg)
            {
                phyTimeEvent_t ev;
                
                ev.callback = Phy_SendLatePD;
                ev.parameter = (uint32_t)msgType;
                ev.timestamp = gPhyRxRetryInterval_c + PhyTime_GetTimestamp();
                PhyTime_ScheduleEvent(&ev);
            }
            else
            {
                pMsg->msgType = gPdDataCnf_c;
                pMsg->msgData.dataCnf.status = status;
                pPhyStruct->PD_MAC_SapHandler(pMsg, pPhyStruct->currentMacInstance);
            }
        }
    }
}


static void Phy_SendLatePLME( uint32_t param )
{
    PLME_SendMessage(&phyLocal, (phyMessageId_t)param);
}

static void Phy_SendLatePD( uint32_t param )
{
    PD_SendMessage(&phyLocal, (phyMessageId_t)param);
}

void Radio_Phy_Notify(void)
{
    Phy24Task(&phyLocal);
}

#if (gMWS_Enabled_d) || (gMWS_UseCoexistence_d)
/*! *********************************************************************************
* \brief  This function represents the callback used by the MWS module to signal
*         events to the 802.15.4 PHY
*
* \param[in]  event - the MWS event
*
* \return  status
*
********************************************************************************** */
static uint32_t MWS_802_15_4_Callback ( mwsEvents_t event )
{
    uint32_t status = 0;
    uint8_t xcvrState;

    switch(event)
    {
    case gMWS_Init_c:
#if gMWS_Enabled_d
        mXcvrAcquired = 0;
#endif
        break;

    case gMWS_Active_c:
        XCVR_ChangeMode(ZIGBEE_MODE, DR_500KBPS);
        XCVR_MISC->BLE_ARB_CTRL |= XCVR_CTRL_BLE_ARB_CTRL_BLE_RELINQUISH_MASK;
#if gMWS_UseCoexistence_d && gMWS_Enabled_d
        /* Restore Pin settings */
        MWS_CoexistenceInit(&gCoexistence_RfDeny, &gCoexistence_RfActive, &gCoexistence_RfStatus);
#endif
        break;

    case gMWS_Idle_c:
        // Phy24Task(&phyLocal);
        PHY_ForceIrqPending();
        break;

    case gMWS_Abort_c:
#if gMWS_Enabled_d
        mXcvrAcquired = 0;
#endif
        xcvrState = PhyGetSeqState();
        if( xcvrState != gIdle_c )
        {
            PhyAbort();
            switch(xcvrState)
            {
            case gCCA_c:
                if( gCcaED_c == (ZLL->PHY_CTRL & ZLL_PHY_CTRL_CCATYPE_MASK) >> ZLL_PHY_CTRL_CCATYPE_SHIFT )
                {
                    phyLocal.channelParams.energyLeveldB = (ZLL->LQI_AND_RSSI & ZLL_LQI_AND_RSSI_CCA1_ED_FNL_MASK) >> ZLL_LQI_AND_RSSI_CCA1_ED_FNL_SHIFT;
                    PLME_SendMessage(&phyLocal, gPlmeEdCnf_c);
                    break;
                }
            case gTX_c:
            case gTR_c:
                phyLocal.channelParams.channelStatus = gPhyChannelBusy_c;
                PLME_SendMessage(&phyLocal, gPlmeCcaCnf_c);
                break;
            case gRX_c:
                break;
            default:
                PLME_SendMessage(&phyLocal, gPlmeTimeoutInd_c);
                break;
            }
            PHY_ForceIrqPending();
        }
        break;

    case gMWS_GetInactivityDuration_c:
        /* Default status is 0 (Busy)  */
        if( gIdle_c == PhyGetSeqState() )
        {
            status = 0xFFFFFFFF;
        }
        break;

    default:
        status = gMWS_InvalidParameter_c;
        break;
    }

    return status;
}
#endif

#if gMWS_Enabled_d
/*! *********************************************************************************
* \brief  This function returns the duration of the PHY request in symbols
*
* \param[in]  pMsg Pointer to the PHY request message
*
* \return  seq duration in symbols
*
********************************************************************************** */
static uint32_t Phy_GetSeqDuration(phyMessageHeader_t * pMsg)
{
    uint32_t duration;
    
    /* Compute the duration of the sequence */
    switch( pMsg->msgType )
    {
    case gPdDataReq_c:
        duration = ((macToPdDataMessage_t *)pMsg)->msgData.dataReq.txDuration;
        break;
    case gPlmeCcaReq_c:
    case gPlmeEdReq_c:
        duration = gCCATime_c + gPhyTurnaroundTime_c;
        break;
    default:
        duration = 0;
    }
    
    return duration;
}
#endif /* gMWS_Enabled_d */

/*! *********************************************************************************
* \brief  This function will signal other protocols that the XCVR is unused.
*
* \param[in]  param unused
*
********************************************************************************** */
#if gMWS_Enabled_d
void PhyTime_TMR4Callback (void)
{
    if (MWS_GetActiveProtocol() == gMWS_None_c)
    {
        MWS_SignalIdle(gMWS_802_15_4_c);
    }
}
#endif /* gMWS_Enabled_d */

