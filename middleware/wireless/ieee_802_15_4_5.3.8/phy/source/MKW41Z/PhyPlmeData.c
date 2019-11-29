/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "Phy.h"
#include "PhyInterface.h"
#include "EmbeddedTypes.h"
#include "FunctionLib.h"

#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"

#ifndef gMWS_UseCoexistence_d
#define gMWS_UseCoexistence_d 0
#endif

#if gMWS_UseCoexistence_d
#include "MWS.h"
#endif


/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define PHY_PARAMETERS_VALIDATION 1


/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
extern Phy_PhyLocalStruct_t     phyLocal;
extern uint8_t mXcvrDisallowSleep;
#if gPhyRxRetryInterval_c
uint8_t gRxRetryTimer = gInvalidTimerId_c;
#endif

uint8_t gPhyChannelTxPowerLimits[] = gChannelTxPowerLimit_c;


/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
#if gPhyRxRetryInterval_c
static void PhyRxRetry( uint32_t param );
#endif


/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  This function will start a TX sequence. The packet will be sent OTA
*
* \param[in]  pTxPacket   pointer to the TX packet structure
* \param[in]  pRxParams   pointer to RX parameters
* \param[in]  pTxParams   pointer to TX parameters
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPdDataRequest( pdDataReq_t *pTxPacket,
                              volatile phyRxParams_t *pRxParams,
                              volatile phyTxParams_t *pTxParams )
{
    phyStatus_t status = gPhySuccess_c;
    uint32_t irqSts;
    uint32_t i;
    uint8_t xcvseq;
    uint8_t *pPB;

#ifdef PHY_PARAMETERS_VALIDATION
    if(NULL == pTxPacket)
    {
        status = gPhyInvalidParameter_c;
    }
    /* if CCA required ... */
    else if( (pTxPacket->CCABeforeTx > gPhyNoCCABeforeTx_c) || (pTxPacket->CCABeforeTx == gPhyEnergyDetectMode_c) )
    {
        status = gPhyInvalidParameter_c;
    }
    else
#endif /* PHY_PARAMETERS_VALIDATION */

    if( gIdle_c != PhyGetSeqState() )
    {
        status = gPhyBusy_c;
    }
    else
    {
        /* Load data into Packet Buffer byte by byte to avoid memory acces issues */
        pPB = (uint8_t*)ZLL->PKT_BUFFER_TX;
        *pPB++ = pTxPacket->psduLength + 2; /* including 2 bytes of FCS */

        for (i=0; i<pTxPacket->psduLength; i++)
        {
            *pPB++ = pTxPacket->pPsdu[i];
        }

        /* Perform CCA before TX if required */
        if( pTxPacket->CCABeforeTx != gPhyNoCCABeforeTx_c )
        {
            ZLL->PHY_CTRL |= ZLL_PHY_CTRL_CCABFRTX_MASK;
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_CCATYPE_MASK;
            ZLL->PHY_CTRL |= ZLL_PHY_CTRL_CCATYPE(pTxPacket->CCABeforeTx);
        }
        else
        {
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_CCABFRTX_MASK;
        }
        
        /* Slotted operation */
        if( pTxPacket->slottedTx == gPhySlottedMode_c )
        {
            ZLL->PHY_CTRL |= ZLL_PHY_CTRL_SLOTTED_MASK;
        }
        else
        {
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_SLOTTED_MASK;
        }
        
        /* Perform TxRxAck sequence if required by phyTxMode */
        if(pTxPacket->ackRequired == gPhyRxAckRqd_c)
        {
            PhyIsrPassRxParams(pRxParams);
            ZLL->PHY_CTRL |= ZLL_PHY_CTRL_RXACKRQD_MASK;
            xcvseq = gTR_c;
        }
        else
        {
            PhyIsrPassRxParams(NULL);
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_RXACKRQD_MASK;
            xcvseq = gTX_c;
        }
        
        /* Ensure that no spurious interrupts are raised(do not change TMR1 and TMR4 IRQ status) */
        irqSts = ZLL->IRQSTS;
        irqSts &= ~(ZLL_IRQSTS_TMR1IRQ_MASK | ZLL_IRQSTS_TMR4IRQ_MASK);
        irqSts |= ZLL_IRQSTS_TMR3MSK_MASK;
        ZLL->IRQSTS = irqSts;
        
        /* Start the TX / TRX / CCA sequence */
        ZLL->PHY_CTRL |= xcvseq;
        /* Unmask SEQ interrupt */
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_SEQMSK_MASK;
#if gMWS_UseCoexistence_d
        if(gMWS_Success_c != MWS_CoexistenceRequestAccess(gMWS_TxState_c))
        {
            PhyAbort();
            status = gPhyBusy_c;
        }
#endif
        if( (status == gPhySuccess_c) && !mXcvrDisallowSleep )
        {
            mXcvrDisallowSleep = 1;
            PWR_DisallowXcvrToSleep();
        }
    }

    return status;
}

/*! *********************************************************************************
* \brief  This function will start a RX sequence
*
* \param[in]  phyRxMode   slotted/unslotted
* \param[in]  pRxParams   pointer to RX parameters
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPlmeRxRequest( phySlottedMode_t phyRxMode, phyRxParams_t *  pRxParams )
{
    uint32_t irqSts;
    phyStatus_t status = gPhySuccess_c;

#ifdef PHY_PARAMETERS_VALIDATION
    if(NULL == pRxParams)
    {
        status = gPhyInvalidParameter_c;
    }
    else
#endif /* PHY_PARAMETERS_VALIDATION */

    if( gIdle_c != PhyGetSeqState() )
    {
        status = gPhyBusy_c;
    }
    else
    {
        
        pRxParams->phyRxMode = phyRxMode;
        
        if( NULL == pRxParams->pRxData )
        {
            pRxParams->pRxData = Phy_BufferAllocForever(sizeof(pdDataToMacMessage_t) + gMaxPHYPacketSize_c);
        }
        
        if( NULL == pRxParams->pRxData )
        {
#if gPhyRxRetryInterval_c
            if( gRxRetryTimer == gInvalidTimerId_c )
            {
                phyTimeEvent_t event;

                event.timestamp = PhyTime_GetTimestamp() + gPhyRxRetryInterval_c;
                event.parameter = 0;
                event.callback  = PhyRxRetry;
                gRxRetryTimer = PhyTime_ScheduleEvent( &event );
            }
#endif
            status = gPhyTRxOff_c;   
        }
        else
        {
            PhyIsrPassRxParams(pRxParams);
            
            pRxParams->pRxData->msgData.dataInd.pPsdu = 
                (uint8_t*)&pRxParams->pRxData->msgData.dataInd.pPsdu +
                    sizeof(pRxParams->pRxData->msgData.dataInd.pPsdu);
            
            /* Slotted operation */
            if(gPhySlottedMode_c == phyRxMode)
            {
                ZLL->PHY_CTRL |= ZLL_PHY_CTRL_SLOTTED_MASK;
            }
            else
            {
                ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_SLOTTED_MASK;
            }
            
            /* Ensure that no spurious interrupts are raised, but do not change TMR1 and TMR4 IRQ status */
            irqSts = ZLL->IRQSTS;
            irqSts &= ~(ZLL_IRQSTS_TMR1IRQ_MASK | ZLL_IRQSTS_TMR4IRQ_MASK);
            irqSts |= ZLL_IRQSTS_TMR3MSK_MASK;
            ZLL->IRQSTS = irqSts;
            
            /* Start the RX sequence */
            ZLL->PHY_CTRL |= gRX_c ;
            /* unmask SEQ interrupt */
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_SEQMSK_MASK;
            if( (status == gPhySuccess_c) && !mXcvrDisallowSleep )
            {
                mXcvrDisallowSleep = 1;
                PWR_DisallowXcvrToSleep();
            }
        }
    }

    return status;
}

/*! *********************************************************************************
* \brief  This function will start a CCA / CCCA sequence
*
* \param[in]  ccaParam   the type of CCA
* \param[in]  cccaMode   continuous or single CCA
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPlmeCcaEdRequest( phyCCAType_t ccaParam, phyContCCAMode_t cccaMode )
{
    uint32_t irqSts;
    phyStatus_t status = gPhySuccess_c;

#ifdef PHY_PARAMETERS_VALIDATION
    /* Check for illegal CCA type */
    if( (ccaParam != gPhyCCAMode1_c) && (ccaParam != gPhyCCAMode2_c) && (ccaParam != gPhyCCAMode3_c) && (ccaParam != gPhyEnergyDetectMode_c))
    {
        status = gPhyInvalidParameter_c;
    }
    /* Cannot perform Continuous CCA using ED type */
    else if( (ccaParam == gPhyEnergyDetectMode_c) && (cccaMode == gPhyContCcaEnabled) )
    {
        status = gPhyInvalidParameter_c;
    }
    else
#endif /* PHY_PARAMETERS_VALIDATION */

    if( gIdle_c != PhyGetSeqState() )
    {
        status = gPhyBusy_c;
    }
    else
    {
        /* Write in PHY CTRL the desired type of CCA */
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_CCATYPE_MASK;
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_CCATYPE(ccaParam);
        
        /* Ensure that no spurious interrupts are raised(do not change TMR1 and TMR4 IRQ status) */
        irqSts = ZLL->IRQSTS;
        irqSts &= ~(ZLL_IRQSTS_TMR1IRQ_MASK | ZLL_IRQSTS_TMR4IRQ_MASK);
        irqSts |= ZLL_IRQSTS_TMR3MSK_MASK;
        ZLL->IRQSTS = irqSts;
        /* Unmask SEQ interrupt */
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_SEQMSK_MASK;
        
        /* continuous CCA */
        if(cccaMode == gPhyContCcaEnabled)
        {
            /* start the continuous CCA sequence
            immediately or by TC2', depending on a previous PhyTimeSetEventTrigger() call) */
            ZLL->PHY_CTRL |= gCCCA_c;
        }
        /* normal CCA (not continuous) */
        else
        {
            /* start the CCA or ED sequence (this depends on CcaType used)
            immediately or by TC2', depending on a previous PhyTimeSetEventTrigger() call) */
            ZLL->PHY_CTRL |= gCCA_c;
        }
        /* At the end of the scheduled sequence, an interrupt will occur:
        CCA , SEQ or TMR3 */
        if( (status == gPhySuccess_c) && !mXcvrDisallowSleep )
        {
            mXcvrDisallowSleep = 1;
            PWR_DisallowXcvrToSleep();
        }
    }

    return status;
}

/*! *********************************************************************************
* \brief  This function will set the channel number for the specified PAN
*
* \param[in]   channel   new channel number
* \param[in]   pan       the PAN registers (0/1)
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPlmeSetCurrentChannelRequest
(
  uint8_t channel,
  uint8_t pan
)
{
    phyStatus_t status = gPhySuccess_c;
#ifdef PHY_PARAMETERS_VALIDATION
    if((channel < 11) || (channel > 26))
    {
        status = gPhyInvalidParameter_c;
    }
    else
#endif /* PHY_PARAMETERS_VALIDATION */
    {
        if( !pan )
        {
            ZLL->CHANNEL_NUM0 = channel;
        }
        else
        {
            ZLL->CHANNEL_NUM1 = channel;
        }
        
        /* Make sure the current Tx power doesn't exceed the Tx power limit for the new channel */
        if( PhyPlmeGetPwrLevelRequest() > gPhyChannelTxPowerLimits[channel - 11] )
        {
            PhyPlmeSetPwrLevelRequest(gPhyChannelTxPowerLimits[channel - 11]);
        }
    }
    
    return status;
}

/*! *********************************************************************************
* \brief  This function will return the current channel for a specified PAN
*
* \param[in]   pan   the PAN registers (0/1)
*
* \return  uint8_t  current channel number
*
********************************************************************************** */
uint8_t PhyPlmeGetCurrentChannelRequest
(
  uint8_t pan
)
{
    uint8_t channel;
    
    if( !pan )
    {
        channel = ZLL->CHANNEL_NUM0;
    }
    else
    {
        channel = ZLL->CHANNEL_NUM1;
    }

    return channel;
}

/*! *********************************************************************************
* \brief  This function will set the radio Tx power
*
* \param[in]   pwrStep   the Tx power
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPlmeSetPwrLevelRequest
(
  uint8_t pwrStep
)
{
    phyStatus_t status = gPhySuccess_c;
#ifdef PHY_PARAMETERS_VALIDATION
    if( pwrStep > 32 )
    {
        status = gPhyInvalidParameter_c;
    }
    else
#endif /* PHY_PARAMETERS_VALIDATION */
    {
        /* Do not exceed the Tx power limit for the current channel */
        if( pwrStep > gPhyChannelTxPowerLimits[ZLL->CHANNEL_NUM0 - 11] )
        {
            pwrStep = gPhyChannelTxPowerLimits[ZLL->CHANNEL_NUM0 - 11];
        }
#if gMpmIncluded_d
        if( pwrStep > gPhyChannelTxPowerLimits[ZLL->CHANNEL_NUM1 - 11] )
        {
            pwrStep = gPhyChannelTxPowerLimits[ZLL->CHANNEL_NUM1 - 11];
        }
#endif
        if( pwrStep > 2 )
        {
            pwrStep = (pwrStep << 1) - 2;
        }
        
        ZLL->PA_PWR = pwrStep;
    }
    return status;
}

/*! *********************************************************************************
* \brief  This function will return the radio Tx power
*
* \return  Power level
*
********************************************************************************** */
uint8_t PhyPlmeGetPwrLevelRequest(void)
{
    uint8_t pwrStep = (uint8_t)ZLL->PA_PWR;

    if( pwrStep > 2 )
    {
        pwrStep = (pwrStep + 2) >> 1;
    }
    
    return pwrStep;
}

/*! *********************************************************************************
* \brief  This function will set the value of PHY PIBs
*
* \param[in]   pibId            the Id of the PIB
* \param[in]   pibValue         the new value of the PIB
* \param[in]   phyRegistrySet   the PAN registers (0/1)
* \param[in]   instanceId       the instance of the PHY
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPlmeSetPIBRequest(phyPibId_t pibId, uint64_t pibValue, uint8_t phyRegistrySet, instanceId_t instanceId)
{
  phyStatus_t result = gPhySuccess_c;

  switch(pibId)
  {
    case gPhyPibCurrentChannel_c:
    {
        bool_t value = !!(phyLocal.flags & gPhyFlagRxOnWhenIdle_c);

        if( gRX_c == PhyGetSeqState() )
        {
            PhyAbort();
        }
        result = PhyPlmeSetCurrentChannelRequest((uint8_t) pibValue, phyRegistrySet);
        PhyPlmeSetRxOnWhenIdle(value, instanceId);
    }
    break;
    case gPhyPibTransmitPower_c:
    {
        result = PhyPlmeSetPwrLevelRequest((uint8_t) pibValue);
    }
    break;
    case gPhyPibLongAddress_c:
    {
        uint64_t longAddr = pibValue;
        result = PhyPpSetLongAddr((uint8_t *) &longAddr, phyRegistrySet);
    }
    break;
    case gPhyPibShortAddress_c:
    {
        uint16_t shortAddr = (uint16_t) pibValue;
        result = PhyPpSetShortAddr((uint8_t *) &shortAddr, phyRegistrySet);
    }
    break;
    case gPhyPibPanId_c:
    {
        uint16_t panId = (uint16_t) pibValue;
        result = PhyPpSetPanId((uint8_t *) &panId, phyRegistrySet);
    }
    break;
    case gPhyPibPanCoordinator_c:
    {
        bool_t macRole = (bool_t) pibValue;
        result = PhyPpSetMacRole(macRole, phyRegistrySet);
    }
    break;
    case gPhyPibCurrentPage_c:
    {
        /* Nothinh to do... */
    }
    break;
    case gPhyPibPromiscuousMode_c:
    {
        PhyPpSetPromiscuous((uint8_t)pibValue);
    }
    break;
    case gPhyPibRxOnWhenIdle:
    {
        PhyPlmeSetRxOnWhenIdle( (bool_t)pibValue, instanceId );
    }
    break;
    case gPhyPibFrameWaitTime_c:
    {
        PhyPlmeSetFrameWaitTime( (uint32_t)pibValue, instanceId );
    }
    break;
    case gPhyPibDeferTxIfRxBusy_c:
    {
        if( pibValue )
        {
            phyLocal.flags |= gPhyFlagDeferTx_c;
        }
        else
        {
            phyLocal.flags &= ~gPhyFlagDeferTx_c;
        }
    }
    break;
    case gPhyPibLastTxAckFP_c:
    {
        result = gPhyReadOnly_c;
    }
    break;
    default:
    {
        result = gPhyUnsupportedAttribute_c;
    }
    break;
  }

  return result;
}

/*! *********************************************************************************
* \brief  This function will return the value of PHY PIBs
*
* \param[in]   pibId            the Id of the PIB
* \param[out]  pibValue         pointer to a location where the value will be stored
* \param[in]   phyRegistrySet   the PAN registers (0/1)
* \param[in]   instanceId       the instance of the PHY
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPlmeGetPIBRequest(phyPibId_t pibId, uint64_t * pibValue, uint8_t phyRegistrySet, instanceId_t instanceId)
{
    phyStatus_t result = gPhySuccess_c;
    uint64_t value;
    uint8_t size = 1;
    
    switch(pibId)
    {
      case gPhyPibCurrentChannel_c:
      {
          value = (uint64_t) PhyPlmeGetCurrentChannelRequest(phyRegistrySet);
      }
      break;
      case gPhyPibTransmitPower_c:
      {
          value = PhyPlmeGetPwrLevelRequest();
      }
      break;
      case gPhyPibLongAddress_c:
      {
          size = 8;
          if( !phyRegistrySet )
          {
              value   = ZLL->MACLONGADDRS0_MSB;
              value <<= 32;
              value  |= ZLL->MACLONGADDRS0_LSB;
          }
          else
          {
              value   = ZLL->MACLONGADDRS1_MSB;
              value <<= 32;
              value  |= ZLL->MACLONGADDRS1_LSB;
          }
      }
      break;
      case gPhyPibShortAddress_c:
      {
          size = 2;
          if( !phyRegistrySet )
          {
              value = (ZLL->MACSHORTADDRS0 & ZLL_MACSHORTADDRS0_MACSHORTADDRS0_MASK) >> ZLL_MACSHORTADDRS0_MACSHORTADDRS0_SHIFT;
          }
          else
          {
              value = (ZLL->MACSHORTADDRS1 & ZLL_MACSHORTADDRS1_MACSHORTADDRS1_MASK) >> ZLL_MACSHORTADDRS1_MACSHORTADDRS1_SHIFT;
          }
      }
      break;
      case gPhyPibPanId_c:
      {
          size = 2;
          if( !phyRegistrySet )
          {
              value = (ZLL->MACSHORTADDRS0 & ZLL_MACSHORTADDRS0_MACPANID0_MASK) >> ZLL_MACSHORTADDRS0_MACPANID0_SHIFT;
          }
          else
          {
              value = (ZLL->MACSHORTADDRS1 & ZLL_MACSHORTADDRS1_MACPANID1_MASK) >> ZLL_MACSHORTADDRS1_MACPANID1_SHIFT;
          }
      }
      break;
      case gPhyPibPanCoordinator_c:
      {
          if( !phyRegistrySet )
          {
              value = !!(ZLL->PHY_CTRL & ZLL_PHY_CTRL_PANCORDNTR0_MASK);
          }
          else
          {
              value = !!(ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_PANCORDNTR1_MASK);
          }
      }
      break;
      case gPhyPibRxOnWhenIdle:
      {
          value = !!(phyLocal.flags & gPhyFlagRxOnWhenIdle_c);
      }
      break;
      case gPhyPibFrameWaitTime_c:
      {
          size = sizeof(phyLocal.maxFrameWaitTime);
          value = phyLocal.maxFrameWaitTime;
      }
      break;
      case gPhyPibDeferTxIfRxBusy_c:
      {
          value = !!(phyLocal.flags & gPhyFlagDeferTx_c);
      }
      break;
      case gPhyPibLastTxAckFP_c:
      {
          value = !!(phyLocal.flags & gPhyFlagTxAckFP_c);
      }
      break;
      default:
      {
          size = 0;
          result = gPhyUnsupportedAttribute_c;
      }
      break;
    }

    /* Avoid unaligned memory access issues */
    FLib_MemCpy(pibValue, &value, size);
    return result;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This function try to restart the Rx
*
* \param[in]   param  phy Rx params
*
********************************************************************************** */
#if gPhyRxRetryInterval_c
static void PhyRxRetry( uint32_t param )
{
    phyRxParams_t *pRxParams = &phyLocal.rxParams;
    phyTime_t absEndTime = pRxParams->timeStamp;
    absEndTime += pRxParams->duration;

    gRxRetryTimer = gInvalidTimerId_c;
    if( PhyTime_GetTimestamp() < absEndTime )
    {
        PhyPlmeRxRequest( pRxParams->phyRxMode, pRxParams );
    }
    else
    {
        Radio_Phy_TimeRxTimeoutIndication(param);
    }
}
#endif