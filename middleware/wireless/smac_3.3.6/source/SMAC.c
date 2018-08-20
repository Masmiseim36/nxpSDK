/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "SMAC.h"
#include "PhyInterface.h"
#include "EmbeddedTypes.h"

#include "SMAC_Config.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "Panic.h"


/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/          

/*!   
 * \brief The version string of SMAC
 */
char* mSmacVersionString = (char*)gSmacVerString_c;

/*!   
 * \brief Register SMAC module information
 */
RegisterModuleInfo(SMAC, /* DO NOT MODIFY */
                   mSmacVersionString, /* DO NOT MODIFY */
                   gSmacModuleId_c, /* DO NOT MODIFY, EDIT in SMAC.h */
                   gSmacVerMajor_c, /* DO NOT MODIFY, EDIT in SMAC.h */
                   gSmacVerMinor_c, /* DO NOT MODIFY, EDIT in SMAC.h */
                   gSmacVerPatch_c, /* DO NOT MODIFY, EDIT in SMAC.h */
                   gSmacBuildNo_c); /* DO NOT MODIFY, EDIT in SMAC.h */

uint8_t gTotalChannels;
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static smacInternalAttrib_t maSmacAttributes[gSmacMaxPan_c];
static smacMultiPanInstances_t mSmacActivePan;
static uint8_t mSmacInitialized;
//Sap Handlers Called by PHY
phyStatus_t PD_SMAC_SapHandler(void* pMsg, instanceId_t instance);
phyStatus_t PLME_SMAC_SapHandler(void* pMsg, instanceId_t instance);

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static bool_t SMACPacketCheck(pdDataToMacMessage_t* pMsgFromPhy, 
                              smacMultiPanInstances_t instance);
static void BackoffTimeElapsed(void* param);    

#if gSmacUseSecurity_c
#define ENC_BLOCK_SIZE (16)
static void SMAC_Encrypt(uint8_t* pIn, uint8_t* pOut, uint8_t *len, smacMultiPanInstances_t panID);
static void SMAC_Decrypt(uint8_t* pIn, uint8_t* pOut, uint8_t *len, smacMultiPanInstances_t panID);
#endif

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/


/***********************************************************************************/
/******************************** SMAC Data primitives *****************************/
/***********************************************************************************/

/************************************************************************************
* MCPSDataRequest
* 
* This data primitive is used to send an over the air packet. This is an asynchronous 
* function,  it means it asks SMAC to transmit one OTA packet,  but when the function 
* returns it is not sent already.
*
************************************************************************************/
smacErrors_t MCPSDataRequest
(
txPacket_t *psTxPacket        //IN:Pointer to the packet to be transmitted
)
{  
  macToPdDataMessage_t *pMsg;
  phyStatus_t u8PhyRes = gPhySuccess_c; 
  
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif      /* TRUE == smacInitializationValidation_d */
  
#if(TRUE == smacParametersValidation_d)

  if((NULL == psTxPacket) || (gMaxSmacSDULength_c < psTxPacket->u8DataLength))
  {
    return gErrorOutOfRange_c;
  }  
#endif         /* TRUE == smacParametersValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  
#if !gSmacUseSecurity_c
  pMsg = MEM_BufferAlloc( sizeof(macToPdDataMessage_t) +
                          psTxPacket->u8DataLength+ gSmacHeaderBytes_c);
#else
  pMsg = MEM_BufferAlloc( sizeof(macToPdDataMessage_t) + 
                          psTxPacket->u8DataLength + gSmacHeaderBytes_c + ENC_BLOCK_SIZE );
#endif
  if(pMsg == NULL )
  {
    return gErrorNoResourcesAvailable_c;
  }
  
  maSmacAttributes[mSmacActivePan].u8SmacSeqNo++;
  maSmacAttributes[mSmacActivePan].u8AckRetryCounter = 0;
  maSmacAttributes[mSmacActivePan].u8CCARetryCounter = 0;
  
  /* Fill with Phy related data */
  pMsg->macInstance = mSmacActivePan;
  pMsg->msgType = gPdDataReq_c;
  //SMAC doesn't use slotted mode
  pMsg->msgData.dataReq.slottedTx = gPhyUnslottedMode_c;
  //start transmission immediately
  pMsg->msgData.dataReq.startTime = gPhySeqStartAsap_c;
#ifdef gPHY_802_15_4g_d
  //for sub-Gig phy handles duration in case of ACK
  pMsg->msgData.dataReq.txDuration = 0xFFFFFFFF;
#else
  
#if !gUseSMACLegacy_c
  if(maSmacAttributes[mSmacActivePan].txConfigurator.autoAck && 
     psTxPacket->smacHeader.destAddr != 0xFFFF &&
       psTxPacket->smacHeader.panId != 0xFFFF)
  {
                                    //Turn@       +       phy payload(symbols)+ Turn@ + ACK
    pMsg->msgData.dataReq.txDuration = 12 + (6 + psTxPacket->u8DataLength + 2)*2 + 12 + 42; 
    if(maSmacAttributes[mSmacActivePan].txConfigurator.ccaBeforeTx)
    {
      pMsg->msgData.dataReq.txDuration += 0x08; //CCA Duration: 8 symbols
    }
#if gSmacUseSecurity_c
	/*if security is used take padding into account for tx duration*/
    pMsg->msgData.dataReq.txDuration += ( ENC_BLOCK_SIZE - ((psTxPacket->u8DataLength - gSmacHeaderBytes_c
                                                            +ENC_BLOCK_SIZE)&(ENC_BLOCK_SIZE-1))
                                         )*2;
#endif
  }
  else
#endif  
  {
    pMsg->msgData.dataReq.txDuration = 0xFFFFFFFF;
  }
#endif
  pMsg->msgData.dataReq.psduLength = psTxPacket->u8DataLength + gSmacHeaderBytes_c;
  pMsg->msgData.dataReq.pPsdu = (uint8_t*)&pMsg->msgData.dataReq.pPsdu +
    sizeof(pMsg->msgData.dataReq.pPsdu);
  
  FLib_MemCpy(pMsg->msgData.dataReq.pPsdu, &(psTxPacket->smacHeader), gSmacHeaderBytes_c);
  FLib_MemCpy(pMsg->msgData.dataReq.pPsdu + gSmacHeaderBytes_c, 
              &(psTxPacket->smacPdu), 
              psTxPacket->u8DataLength);

  if(maSmacAttributes[mSmacActivePan].txConfigurator.ccaBeforeTx)
  {
    //tell phy to perform CCA before transmission
    pMsg->msgData.dataReq.CCABeforeTx = gPhyCCAMode1_c;
  }
  else
  {
    pMsg->msgData.dataReq.CCABeforeTx = gPhyNoCCABeforeTx_c;
  }
#if !gUseSMACLegacy_c  
  if(maSmacAttributes[mSmacActivePan].txConfigurator.autoAck && 
     psTxPacket->smacHeader.destAddr != 0xFFFF &&
       psTxPacket->smacHeader.panId != 0xFFFF)
  {
    //set frame control option: ACK.
    pMsg->msgData.dataReq.pPsdu[0] |=   gFrameCtrlAckReqMsk_c;
    pMsg->msgData.dataReq.ackRequired = gPhyRxAckRqd_c;
  }
  else
#endif
  {
    pMsg->msgData.dataReq.ackRequired = gPhyNoAckRqd_c;
  }
#if gSmacUseSecurity_c
  uint8_t inputLen = pMsg->msgData.dataReq.psduLength - gSmacHeaderBytes_c;
  SMAC_Encrypt(pMsg->msgData.dataReq.pPsdu + gSmacHeaderBytes_c, pMsg->msgData.dataReq.pPsdu + gSmacHeaderBytes_c, 
               &(inputLen),
               mSmacActivePan);
  pMsg->msgData.dataReq.psduLength = inputLen + gSmacHeaderBytes_c;
#endif
  //set sequence number;
#if !gUseSMACLegacy_c
  pMsg->msgData.dataReq.pPsdu[2] = maSmacAttributes[mSmacActivePan].u8SmacSeqNo;
#endif
  maSmacAttributes[mSmacActivePan].gSmacDataMessage = pMsg;      //Store pointer for freeing later 
  
  OSA_InterruptDisable();
  maSmacAttributes[mSmacActivePan].smacState = mSmacStateTransmitting_c; 
  OSA_InterruptEnable();
  u8PhyRes = MAC_PD_SapHandler(pMsg, 0);

  if(u8PhyRes == gPhySuccess_c)
  {
    return gErrorNoError_c;
  }
  else
  {
    MEM_BufferFree(maSmacAttributes[mSmacActivePan].gSmacDataMessage);
    maSmacAttributes[mSmacActivePan].gSmacDataMessage = NULL;
    
    OSA_InterruptDisable();
    maSmacAttributes[mSmacActivePan].smacState = mSmacStateIdle_c; 
    OSA_InterruptEnable();
    return gErrorNoResourcesAvailable_c;
  }
}


/************************************************************************************
* MLMESetActivePan
* 
* This management primitive switches between pans
*
************************************************************************************/
smacErrors_t MLMESetActivePan(smacMultiPanInstances_t panID)
{
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif      /* TRUE == smacInitializationValidation_d */
  if(panID >= gSmacMaxPan_c)
  {
    return gErrorOutOfRange_c;
  }
  if(panID == mSmacActivePan)
  {
    return gErrorNoError_c;
  }
  OSA_InterruptDisable();
  mSmacActivePan = panID;
  OSA_InterruptEnable();
  
  return gErrorNoError_c;
}

/************************************************************************************
* MLMEConfigureDualPanSettings
* 
* This management primitive configures the multipan settings
*
************************************************************************************/
smacErrors_t MLMEConfigureDualPanSettings(
                                          bool_t bUseAutoMode,
                                          bool_t bModifyDwell,
                                          uint8_t u8Prescaler, 
                                          uint8_t u8Scale
                                            )
{
#if gMpmMaxPANs_c != 2
  return gErrorNoValidCondition_c;
#else
    
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif      /* TRUE == smacInitializationValidation_d */
  if(mSmacStateIdle_c != maSmacAttributes[gSmacPan0_c].smacState ||
     mSmacStateIdle_c != maSmacAttributes[gSmacPan1_c].smacState)
  {
    return gErrorBusy_c;
  }
  if((bModifyDwell == TRUE) && (u8Prescaler > 3 || u8Scale > 63))
  {
    return gErrorOutOfRange_c;
  }
  
  mpmConfig_t lMpmConfig;
  MPM_GetConfig(&lMpmConfig);
  
  if(bModifyDwell)
  {
    lMpmConfig.dwellTime = ((u8Prescaler << mDualPanDwellPrescallerShift_c) |
                            u8Scale << mDualPanDwellTimeShift_c);
  }
  lMpmConfig.autoMode = bUseAutoMode;
  MPM_SetConfig(&lMpmConfig);
  
  return gErrorNoError_c;
#endif
}
/************************************************************************************
* MLMEConfigureTxContext
* 
* This management primitive is used to configure the conditions under which SMAC will
* perform a transmission OTA.
*
************************************************************************************/
smacErrors_t MLMEConfigureTxContext(txContextConfig_t* pTxConfig)
{
  if( (pTxConfig->autoAck == FALSE && pTxConfig->retryCountAckFail !=0) || 
      (pTxConfig->ccaBeforeTx == FALSE && pTxConfig->retryCountCCAFail !=0) )
  {
    return gErrorNoValidCondition_c;
  }
  if( pTxConfig->retryCountAckFail > gMaxRetriesAllowed_c || 
     pTxConfig->retryCountCCAFail > gMaxRetriesAllowed_c)
  {
    return gErrorOutOfRange_c;
  }
#if gUseSMACLegacy_c
  if(pTxConfig->autoAck)
  {
    return gErrorOutOfRange_c;
  }
#endif
  smacInternalAttrib_t* p = &(maSmacAttributes[mSmacActivePan]);
  
  p->txConfigurator.autoAck           = pTxConfig->autoAck;
  p->txConfigurator.ccaBeforeTx       = pTxConfig->ccaBeforeTx;
  p->txConfigurator.retryCountAckFail = pTxConfig->retryCountAckFail;
  p->txConfigurator.retryCountCCAFail = pTxConfig->retryCountCCAFail;
  
  return gErrorNoError_c;
}

/************************************************************************************
* MLMERXEnableRequest
* 
* Function used to place the radio into receive mode 
*
************************************************************************************/
smacErrors_t MLMERXEnableRequest
(
rxPacket_t *gsRxPacket, //OUT: Pointer to the structure where the reception results 
//     will be stored.
smacTime_t stTimeout     //IN:  64-bit timeout value, absolute value in symbols
)
{
  
  uint8_t u8PhyRes = 0; 
  macToPlmeMessage_t lMsg;
  
#if(TRUE == smacParametersValidation_d)
#if gSmacUseSecurity_c
  if((NULL == gsRxPacket) || (gMaxSmacSDULength_c + 16 < gsRxPacket->u8MaxDataLength))
#else
  if((NULL == gsRxPacket) || (gMaxSmacSDULength_c < gsRxPacket->u8MaxDataLength))
#endif
  {
    return gErrorOutOfRange_c;
  }
#endif     /* TRUE == smacParametersValidation_d */
  
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  lMsg.macInstance = mSmacActivePan;
  if(stTimeout)
  {
    lMsg.msgType = gPlmeSetTRxStateReq_c;
    lMsg.msgData.setTRxStateReq.startTime = gPhySeqStartAsap_c;
    lMsg.macInstance = mSmacActivePan;
    lMsg.msgData.setTRxStateReq.state = gPhySetRxOn_c;
    lMsg.msgData.setTRxStateReq.rxDuration = stTimeout;
  }
  else
  {
    lMsg.msgType = gPlmeSetReq_c;
    lMsg.msgData.setReq.PibAttribute = gPhyPibRxOnWhenIdle;
    lMsg.msgData.setReq.PibAttributeValue = (uint64_t)1;
  }
  maSmacAttributes[mSmacActivePan].mSmacTimeoutAsked = (stTimeout > 0);
  
  gsRxPacket->rxStatus = rxProcessingReceptionStatus_c;
  maSmacAttributes[mSmacActivePan].smacProccesPacketPtr.smacRxPacketPointer  
    = gsRxPacket;
    
  OSA_InterruptDisable();
  maSmacAttributes[mSmacActivePan].smacState = mSmacStateReceiving_c; 
  OSA_InterruptEnable();
  u8PhyRes = MAC_PLME_SapHandler(&lMsg, 0);
  
  if(u8PhyRes == gPhySuccess_c)
  { 
    return gErrorNoError_c;
  }
  else
  {
    OSA_InterruptDisable();
    maSmacAttributes[mSmacActivePan].smacState = mSmacStateIdle_c; 
    OSA_InterruptEnable();
    return gErrorNoResourcesAvailable_c;
  }
}
#if defined (gPHY_802_15_4g_d)
/************************************************************************************
* MLMESetPreambleLength
* 
* Function used to change the preamble size in the OTA Packet 
*
************************************************************************************/
smacErrors_t MLMESetPreambleLength
(
uint16_t u16preambleLength
)
{
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  gPhyPib.mPIBphyFSKPreambleRepetitions = u16preambleLength;
  PhyPib_RFUpdatePreambleLength();
  
  return gErrorNoError_c;
  
}

/************************************************************************************
* MLMESetSyncWordSize
* 
* Function used to change the synchronization word size. Values from 0 to 8 required. 
* IMPORTANT-> Use below arguments only (indicating a direct value from 1-8 will not work) 
* Inputs      : 
* 	SyncConfig_SyncSize_1             
*	SyncConfig_SyncSize_2              
*	SyncConfig_SyncSize_3               
*	SyncConfig_SyncSize_4               
* 	SyncConfig_SyncSize_5                
* 	SyncConfig_SyncSize_6                
*	SyncConfig_SyncSize_7              
*  	SyncConfig_SyncSize_8    
*
************************************************************************************/
smacErrors_t MLMESetSyncWordSize
(
uint8_t u8syncWordSize
)
{
  
  phyStatus_t status;
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  
  status = (phyStatus_t)Phy_SetSyncWordSize(u8syncWordSize);
  if(status == gPhyInvalidParameter_c)
    return gErrorOutOfRange_c;
  
  maSmacAttributes[mSmacActivePan].u8SyncWordSize = u8syncWordSize;
  
  return gErrorNoError_c;
  
}

/************************************************************************************
* MLMESetSyncWordValue
* 
* Function used to change the synchronization word value. 
*     
*
************************************************************************************/
smacErrors_t MLMESetSyncWordValue
(
uint8_t *u8syncWordValue
)
{
  uint8_t syncWordSizeTemp = maSmacAttributes[mSmacActivePan].u8SyncWordSize;
  uint8_t syncValueRegIndex = 0;
  
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  } 
  
  while (syncWordSizeTemp--)
  {
    Phy_SetSyncWordValue(syncValueRegIndex, (uint8_t)*u8syncWordValue);
    syncValueRegIndex++;
    u8syncWordValue++;
  }
  while(syncValueRegIndex < 8)
  {
    Phy_SetSyncWordValue(syncValueRegIndex, 0x00);
    syncValueRegIndex++;
  }
  
  return gErrorNoError_c;
  
}

/************************************************************************************
* MLMEPacketConfig
* 
*  
*
************************************************************************************/
smacErrors_t MLMEPacketConfig
(
packetConfig_t *pPacketCfg
)
{
  smacErrors_t err = gErrorNoError_c;
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }	
  err  = MLMESetSyncWordSize(pPacketCfg->u8SyncWordSize);
  err |=  MLMESetSyncWordValue(pPacketCfg->pu8SyncWord);
  err |= MLMESetPreambleLength(pPacketCfg->u16PreambleSize);
  if(err != gErrorNoError_c)
    return gErrorOutOfRange_c;
  
  return gErrorNoError_c;	
}

#endif

/************************************************************************************
* MLMESetChannelRequest
* 
*  
*
************************************************************************************/
smacErrors_t MLMESetChannelRequest
(
channels_t newChannel
)
{
  uint8_t errorVal;	
  macToPlmeMessage_t lMsg;
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }	
  lMsg.msgType = gPlmeSetReq_c;
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgData.setReq.PibAttribute = gPhyPibCurrentChannel_c;
  lMsg.msgData.setReq.PibAttributeValue = (uint64_t) newChannel;
  
  errorVal = MAC_PLME_SapHandler(&lMsg, 0);
  switch (errorVal)
  {
  case  gPhyBusy_c:
    return gErrorBusy_c;
    break;
    
  case gPhyInvalidParameter_c:	 
    return gErrorOutOfRange_c;
    break;
    
  case gPhySuccess_c: 
    return gErrorNoError_c;
    break;
    
  default:
    return gErrorOutOfRange_c;
    break;
  }
}


/************************************************************************************
* MLMESetAdditionalRFOffset
*
*************************************************************************************/
smacErrors_t MLMESetAdditionalRFOffset (uint32_t additionalRFOffset)
{
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */

#if CT_Feature_Calibration
  gPhyPib.mPIBAdditionalRFFrequencyOffset = additionalRFOffset;
  return gErrorNoError_c;
#else
  return gErrorNoResourcesAvailable_c;
#endif
}

/************************************************************************************
* MLMESetAdditionalEDOffset
*
*************************************************************************************/
smacErrors_t MLMESetAdditionalEDOffset (uint8_t additionalEDOffset)
{
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */

#if CT_Feature_Calibration
  gPhyPib.mPIBAdditionalEDOffset = additionalEDOffset;
  return gErrorNoError_c;
#else
  return gErrorNoResourcesAvailable_c;
#endif
}

/************************************************************************************
* MLMEGetAdditionalRFOffset
*
*************************************************************************************/
uint32_t MLMEGetAdditionalRFOffset( void )
{
#if CT_Feature_Calibration
  return gPhyPib.mPIBAdditionalRFFrequencyOffset;
#else
  return 0;
#endif
}

/************************************************************************************
* MLMEGetAdditionalEDOffset
*
*************************************************************************************/
uint8_t MLMEGetAdditionalEDOffset( void )
{
#if CT_Feature_Calibration
  return gPhyPib.mPIBAdditionalEDOffset;
#else
  return 0;
#endif
}

#if defined (gPHY_802_15_4g_d)
/************************************************************************************
* MLMESetFreqBand
*
************************************************************************************/
smacErrors_t MLMESetFreqBand
(
smacFrequencyBands_t freqBand,
smacRFModes_t phyMode
)
{	          	
  return gErrorNoResourcesAvailable_c;
}

smacErrors_t MLMESetPhyMode(smacRFModes_t phyMode)
{
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  phyStatus_t err;
  macToPlmeMessage_t lMsg;
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgType = gPlmeSetReq_c;
  lMsg.msgData.setReq.PibAttribute = gPhyPibCurrentMode_c;
  lMsg.msgData.setReq.PibAttributeValue = (uint64_t)phyMode;
  
  err = MAC_PLME_SapHandler(&lMsg, 0);
  if(err == gPhyInvalidParameter_c)
  {
    return gErrorOutOfRange_c;
  }
  if(err == gPhyBusy_c)
  {
    return gErrorBusy_c;
  }
  
  gTotalChannels = gPhyPib.pPIBphyRfConstants->totalNumChannels;
  
  return gErrorNoError_c;
}
#endif
/************************************************************************************
* MLMEGetChannelRequest
* 
*  
*
************************************************************************************/
channels_t MLMEGetChannelRequest
(
void 
)
{
  channels_t currentChannel;
  macToPlmeMessage_t lMsg;	
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gChannelInvalid_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */	
  lMsg.msgType = gPlmeGetReq_c;
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgData.getReq.PibAttribute = gPhyPibCurrentChannel_c;
  lMsg.msgData.getReq.pPibAttributeValue = (uint64_t*)&currentChannel;
  
  MAC_PLME_SapHandler(&lMsg, 0);
  
  return currentChannel;	
}

#if defined (gPHY_802_15_4g_d)
/************************************************************************************
* MLMERssi
* 
*  
* 
************************************************************************************/
uint8_t MLMERssi(void )
{
  uint8_t rssiVal;
  
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  
  rssiVal = Phy_GetRssi();
  
  return rssiVal;
}

/************************************************************************************
* MLMESetCCADuration
* 
*  
* 
************************************************************************************/
smacErrors_t MLMESetCCADuration(uint64_t usCCADuration )
{
  macToPlmeMessage_t lMsg;
  phyStatus_t status;
  
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  
  usCCADuration = TIME_US_TO_TICKS(usCCADuration);
  Phy_TimeDivider((phyTime_t*)&usCCADuration);
  
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgType = gPlmeSetReq_c;
  lMsg.msgData.setReq.PibAttribute = gPhyPibCCADuration_c;
  lMsg.msgData.setReq.PibAttributeValue = usCCADuration;
  status = MAC_PLME_SapHandler(&lMsg, 0);
  
  if(status == gPhySuccess_c)
    return gErrorNoError_c;
  else
    return gErrorNoResourcesAvailable_c;
}

/************************************************************************************
* MLMESetInterPacketRxDelay
* 
* IMPORTANT-> Use below arguments only (indicating a direct value from 1-12 will not work)
* Inputs      :
*
* InterPacketRxDelay_0
* InterPacketRxDelay_1
* InterPacketRxDelay_2
* InterPacketRxDelay_3
* InterPacketRxDelay_4
* InterPacketRxDelay_5
* InterPacketRxDelay_6
* InterPacketRxDelay_7
* InterPacketRxDelay_8
* InterPacketRxDelay_9
* InterPacketRxDelay_A
* InterPacketRxDelay_B
************************************************************************************/
smacErrors_t MLMESetInterPacketRxDelay
(
uint8_t u8InterPacketRxDelay
)
{	
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif     /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  
  if (gPhySuccess_c != Phy_SetInterPacketRxDelay(u8InterPacketRxDelay))
  {
    return gErrorOutOfRange_c;
  }
  return gErrorNoError_c;
}

#endif
/************************************************************************************
* MLMERXDisableRequest
* 
* Returns the radio to idle mode from receive mode.
*
************************************************************************************/
smacErrors_t MLMERXDisableRequest(void)
{
  macToPlmeMessage_t lMsg;
  phyStatus_t err;
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif
  if((mSmacStateReceiving_c != maSmacAttributes[mSmacActivePan].smacState) && 
     (mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState))
  {
    return gErrorNoValidCondition_c;
  }
  
  lMsg.macInstance = mSmacActivePan;
  
  OSA_InterruptDisable();
  smacStates_t lState = maSmacAttributes[mSmacActivePan].smacState;
  maSmacAttributes[mSmacActivePan].smacState = mSmacStateIdle_c;
  OSA_InterruptEnable();
  if(!maSmacAttributes[mSmacActivePan].mSmacTimeoutAsked)
  {
    lMsg.msgType                          = gPlmeSetReq_c;
    lMsg.msgData.setReq.PibAttribute      = gPhyPibRxOnWhenIdle;
    lMsg.msgData.setReq.PibAttributeValue = (uint64_t)0;
    err = MAC_PLME_SapHandler(&lMsg, 0);
    if(err != gPhySuccess_c)
    {
        OSA_InterruptDisable();
        maSmacAttributes[mSmacActivePan].smacState = lState;
        OSA_InterruptEnable();
        return gErrorBusy_c;
    }
  }
  else
  {
    lMsg.msgType = gPlmeSetTRxStateReq_c;
    lMsg.msgData.setTRxStateReq.state = gPhyForceTRxOff_c;
    maSmacAttributes[mSmacActivePan].mSmacTimeoutAsked = FALSE;
    (void)MAC_PLME_SapHandler(&lMsg, 0);
  }

  
  return gErrorNoError_c;
  
}

/*@CMA, Conn Test Added*/
/************************************************************************************
* MLMETXDisableRequest
* 
* Returns the radio to idle mode from Tx mode.
*
************************************************************************************/
void MLMETXDisableRequest(void)
{
  macToPlmeMessage_t lMsg;
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgType     = gPlmeSetTRxStateReq_c;
  lMsg.msgData.setTRxStateReq.state = gPhyForceTRxOff_c;
  (void)MAC_PLME_SapHandler(&lMsg, 0);
  if(maSmacAttributes[mSmacActivePan].gSmacDataMessage != NULL)
  {
    (void)MEM_BufferFree(maSmacAttributes[mSmacActivePan].gSmacDataMessage);
    maSmacAttributes[mSmacActivePan].gSmacDataMessage = NULL;
  }
  OSA_InterruptDisable();
  maSmacAttributes[mSmacActivePan].smacState = mSmacStateIdle_c;
  OSA_InterruptEnable();
}

/************************************************************************************
* MLMELinkQuality
* 
* This  function  returns  an  integer  value  that is the link quality from the last 
* received packet of the form:  dBm = (-Link Quality/2).
*
************************************************************************************/
uint8_t MLMELinkQuality(void)
{
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return 0;
  }
#endif
  return maSmacAttributes[mSmacActivePan].smacLastDataRxParams.linkQuality;
}

/************************************************************************************
* MLMEPAOutputAdjust
* 
*
************************************************************************************/
smacErrors_t MLMEPAOutputAdjust
( 
uint8_t u8PaValue
)
{
  macToPlmeMessage_t lMsg;
  uint8_t errorVal;		
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgType     = gPlmeSetReq_c;
  lMsg.msgData.setReq.PibAttribute      =  gPhyPibTransmitPower_c;
  lMsg.msgData.setReq.PibAttributeValue =  (uint64_t) u8PaValue;
  errorVal = MAC_PLME_SapHandler(&lMsg, 0);
  switch (errorVal)
  {
  case  gPhyBusy_c:
    return gErrorBusy_c;
    break;
    
  case gPhyInvalidParameter_c:	 
    return gErrorOutOfRange_c;
    break;
    
  case gPhySuccess_c: 
    return gErrorNoError_c;
    break;
    
  default:
    return gErrorOutOfRange_c; 
    break;
  }
}

/************************************************************************************
* MLMEScanRequest
* 
* This  function  returns  the RSSI value of the channel passes as a parameter  
* 
*
************************************************************************************/
smacErrors_t MLMEScanRequest(channels_t u8ChannelToScan)
{
  smacErrors_t err = gErrorNoError_c;
  phyStatus_t u8PhyRes;
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  if(u8ChannelToScan != MLMEGetChannelRequest())
  {
    err = MLMESetChannelRequest(u8ChannelToScan);
  }
  if(err != gErrorNoError_c)
  {
    return err;
  }
  
  macToPlmeMessage_t* pMsg = (macToPlmeMessage_t*)MEM_BufferAlloc(sizeof(macToPlmeMessage_t));
  
  pMsg->macInstance = mSmacActivePan;
  pMsg->msgType = gPlmeEdReq_c;
  pMsg->msgData.edReq.startTime = gPhySeqStartAsap_c;
  maSmacAttributes[mSmacActivePan].gSmacMlmeMessage = pMsg;
  
  OSA_InterruptDisable();
  maSmacAttributes[mSmacActivePan].smacState = mSmacStateScanningChannels_c;
  OSA_InterruptEnable();
  u8PhyRes = MAC_PLME_SapHandler(pMsg,0);
  if(u8PhyRes != gPhySuccess_c)
  {
    OSA_InterruptDisable();
    maSmacAttributes[mSmacActivePan].smacState = mSmacStateIdle_c;
    OSA_InterruptEnable();
    MEM_BufferFree(maSmacAttributes[mSmacActivePan].gSmacMlmeMessage);
    maSmacAttributes[mSmacActivePan].gSmacMlmeMessage = NULL;
    return gErrorBusy_c;
  }

  return gErrorNoError_c;
}


/************************************************************************************
* MLMECcaRequest
* 
* This  function  performs Clear Channel Assessment on the active channel  
* 
*
************************************************************************************/
smacErrors_t MLMECcaRequest()
{
  phyStatus_t u8PhyRes;
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif /* TRUE == smacInitializationValidation_d */
  
  if(mSmacStateIdle_c != maSmacAttributes[mSmacActivePan].smacState)
  {
    return gErrorBusy_c;
  }
  macToPlmeMessage_t* pMsg = (macToPlmeMessage_t*)MEM_BufferAlloc(sizeof(macToPlmeMessage_t));
  
  pMsg->macInstance = mSmacActivePan;
  pMsg->msgType = gPlmeCcaReq_c;
  pMsg->msgData.ccaReq.ccaType = gPhyCCAMode1_c;
  pMsg->msgData.ccaReq.contCcaMode = gPhyContCcaDisabled;
  
  maSmacAttributes[mSmacActivePan].gSmacMlmeMessage = pMsg;
  OSA_InterruptDisable();
  maSmacAttributes[mSmacActivePan].smacState = mSmacStatePerformingCca_c;
  OSA_InterruptEnable();
  u8PhyRes = MAC_PLME_SapHandler(pMsg,0);
  if(u8PhyRes != gPhySuccess_c)
  {
    OSA_InterruptDisable();
    maSmacAttributes[mSmacActivePan].smacState = mSmacStateIdle_c;
    OSA_InterruptEnable();
    MEM_BufferFree(maSmacAttributes[mSmacActivePan].gSmacMlmeMessage);
    maSmacAttributes[mSmacActivePan].gSmacMlmeMessage = NULL;
    return gErrorBusy_c;
  }
  return gErrorNoError_c;
}
/************************************************************************************
* MLMEPhySoftReset
* 
* This function performs a software reset on the radio, PHY and SMAC state machines.
* 
*
************************************************************************************/
smacErrors_t MLMEPhySoftReset
(
void	
)
{	
  macToPlmeMessage_t lMsg;
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif /* TRUE == smacInitializationValidation_d */
//  
//  if(mSmacStateIdle_c != smacState)
//  {
//    return gErrorBusy_c;
//  }
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgType     = gPlmeSetTRxStateReq_c;
  lMsg.msgData.setTRxStateReq.state = gPhyForceTRxOff_c;
  (void)MAC_PLME_SapHandler(&lMsg, 0);
  
  OSA_InterruptDisable();
  maSmacAttributes[mSmacActivePan].smacState= mSmacStateIdle_c; 
  OSA_InterruptEnable();
  
  if(maSmacAttributes[mSmacActivePan].gSmacDataMessage != NULL)
  {
    MEM_BufferFree(maSmacAttributes[mSmacActivePan].gSmacDataMessage);
    maSmacAttributes[mSmacActivePan].gSmacDataMessage = NULL;
  }
  
  if(maSmacAttributes[mSmacActivePan].gSmacMlmeMessage != NULL)
  {
    MEM_BufferFree(maSmacAttributes[mSmacActivePan].gSmacMlmeMessage);
    maSmacAttributes[mSmacActivePan].gSmacMlmeMessage = NULL;
  }
  
  return gErrorNoError_c;
}
/************************************************************************************
* PD_SMAC_SapHandler
* 
* This SAP handles data confirm and data indication from PHY.
* 
************************************************************************************/
phyStatus_t PD_SMAC_SapHandler(void* pMsg, instanceId_t instance)
{
  phyStatus_t status;
  smacToAppDataMessage_t* pSmacMsg;
  smacMultiPanInstances_t lSmacInstanceBackup;
  
  pdDataToMacMessage_t* pDataMsg = (pdDataToMacMessage_t*)pMsg;
  
  switch(pDataMsg->msgType)
  {
  case gPdDataCnf_c:
    //no data request was fired
    if(NULL == maSmacAttributes[instance].gSmacDataMessage)
    {
      status = gPhySuccess_c;
    }
    else
    {
      //phy finished work with the data request packet so it can be freed
      MEM_BufferFree(maSmacAttributes[instance].gSmacDataMessage);
      maSmacAttributes[instance].gSmacDataMessage = NULL;
      
      pSmacMsg = MEM_BufferAlloc(sizeof(smacToAppDataMessage_t));
      if(pSmacMsg == NULL)
      {
        status = gPhySuccess_c;
      }
      else
      {
        pSmacMsg->msgType = gMcpsDataCnf_c;
        pSmacMsg->msgData.dataCnf.status = gErrorNoError_c;
        // call App Sap
        maSmacAttributes[instance].gSMAC_APP_MCPS_SapHandler(pSmacMsg, instance); 
      }
      OSA_InterruptDisable();
      maSmacAttributes[instance].smacState = mSmacStateIdle_c;
      OSA_InterruptEnable();
    }
    break;
  case gPdDataInd_c:
    if(FALSE == SMACPacketCheck(pDataMsg, (smacMultiPanInstances_t)instance))
    {
      MEM_BufferFree(pDataMsg);
      //if timeout is asked and packet fails the check, send message with abort status
      if(maSmacAttributes[instance].mSmacTimeoutAsked)
      {
        pSmacMsg = MEM_BufferAlloc(sizeof(smacToAppDataMessage_t));
        pSmacMsg->msgType = gMcpsDataInd_c;
        pSmacMsg->msgData.dataInd.pRxPacket = 
          maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer;
        pSmacMsg->msgData.dataInd.pRxPacket->rxStatus = rxAbortedStatus_c;
        maSmacAttributes[instance].gSMAC_APP_MCPS_SapHandler(pSmacMsg, instance);
        
        OSA_InterruptDisable();
        maSmacAttributes[instance].smacState = mSmacStateIdle_c;
        OSA_InterruptEnable();
      }
      status = gPhySuccess_c;
    }
    else
    {
      maSmacAttributes[instance].smacLastDataRxParams.linkQuality = 
        ((pdDataToMacMessage_t*)pMsg)->msgData.dataInd.ppduLinkQuality;
      maSmacAttributes[instance].smacLastDataRxParams.timeStamp = 
        (phyTime_t)((pdDataToMacMessage_t*)pMsg)->msgData.dataInd.timeStamp;
      maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer->rxStatus = 
        rxSuccessStatus_c;
#if gSmacUseSecurity_c
      uint8_t len = pDataMsg->msgData.dataInd.psduLength - gSmacHeaderBytes_c;
      SMAC_Decrypt(pDataMsg->msgData.dataInd.pPsdu + gSmacHeaderBytes_c, pDataMsg->msgData.dataInd.pPsdu + gSmacHeaderBytes_c,
                   &len,
                   (smacMultiPanInstances_t)instance);
      pDataMsg->msgData.dataInd.psduLength = len + gSmacHeaderBytes_c;
#endif
      // in case no timeout was asked we need to unset RXOnWhenIdle Pib.
      if(!maSmacAttributes[instance].mSmacTimeoutAsked) 
      {
        lSmacInstanceBackup = mSmacActivePan;
        MLMESetActivePan((smacMultiPanInstances_t)instance);
        (void)MLMERXDisableRequest();
        MLMESetActivePan(lSmacInstanceBackup);
      }
      maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer->u8DataLength 
        = pDataMsg->msgData.dataInd.psduLength - gSmacHeaderBytes_c;
      FLib_MemCpy(&maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer->smacHeader, 
                  ((smacHeader_t*)pDataMsg->msgData.dataInd.pPsdu), 
                  gSmacHeaderBytes_c);
      FLib_MemCpy(&maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer->smacPdu, 
                  ((smacPdu_t*)(pDataMsg->msgData.dataInd.pPsdu + gSmacHeaderBytes_c)), 
                  maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer->u8DataLength);
      
      pSmacMsg = MEM_BufferAlloc(sizeof(smacToAppDataMessage_t));
      if(pSmacMsg == NULL)
      {
        status = gPhySuccess_c;
      }
      else
      {
        pSmacMsg->msgType = gMcpsDataInd_c;
        pSmacMsg->msgData.dataInd.pRxPacket = maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer;
#if gMpmMaxPANs_c == 2
        pSmacMsg->msgData.dataInd.pRxPacket->instanceId = 
          (smacMultiPanInstances_t)instance;
#else
        pSmacMsg->msgData.dataInd.pRxPacket->instanceId = 
          (smacMultiPanInstances_t)0;
#endif
        pSmacMsg->msgData.dataInd.u8LastRxRssi = PhyGetLastRxRssiValue();
        maSmacAttributes[instance].gSMAC_APP_MCPS_SapHandler(pSmacMsg, instance); 
      }
      OSA_InterruptDisable();
      maSmacAttributes[instance].smacState = mSmacStateIdle_c;
      OSA_InterruptEnable();
    }
    break;
  default:
    break;
  }
  MEM_BufferFree(pMsg);
  return status;
}

/************************************************************************************
* PLME_SMAC_SapHandler
* 
* This SAP handles management for confirms and indications from PHY.
* 
************************************************************************************/

phyStatus_t PLME_SMAC_SapHandler(void* pMsg, instanceId_t instance)
{
  uint32_t backOffTime = 0;
  smacMultiPanInstances_t lSmacInstanceBackup;
  smacToAppMlmeMessage_t* pSmacToApp;
  smacToAppDataMessage_t* pSmacMsg;
  plmeToMacMessage_t* pPlmeMsg = (plmeToMacMessage_t*)pMsg;
  
  MEM_BufferFree(maSmacAttributes[instance].gSmacMlmeMessage);
  maSmacAttributes[instance].gSmacMlmeMessage = NULL;

  switch(pPlmeMsg->msgType)
  {
  case gPlmeCcaCnf_c:
    if(pPlmeMsg->msgData.ccaCnf.status == gPhyChannelBusy_c && 
       maSmacAttributes[instance].smacState == mSmacStateTransmitting_c)
    {
      if(maSmacAttributes[instance].txConfigurator.ccaBeforeTx)
      { 
          if(maSmacAttributes[instance].txConfigurator.retryCountCCAFail 
             > maSmacAttributes[instance].u8CCARetryCounter)
          {
            //increment cca fail counter
            maSmacAttributes[instance].u8CCARetryCounter++;
            //get random number for backoff time.
            RNG_GetRandomNo(&backOffTime);
            //start event timer. After time elapses, Data request will be fired.
            TMR_StartSingleShotTimer(maSmacAttributes[instance].u8BackoffTimerId, 
                                     ((backOffTime & gMaxBackoffTime_c) + gMinBackoffTime_c), 
                                     BackoffTimeElapsed, (void*)instance);
          }
          else
          {
            MEM_BufferFree(maSmacAttributes[instance].gSmacDataMessage);
            maSmacAttributes[instance].gSmacDataMessage = NULL;
            
            //retries failed so create message for the application
            pSmacMsg = MEM_BufferAlloc(sizeof(smacToAppDataMessage_t));
            if(pSmacMsg != NULL)
            {
              //error type : Channel Busy
              pSmacMsg->msgData.dataCnf.status = gErrorChannelBusy_c;
              //type is Data Confirm
              pSmacMsg->msgType = gMcpsDataCnf_c;
              maSmacAttributes[instance].gSMAC_APP_MCPS_SapHandler(pSmacMsg, instance);
            }
            //place SMAC into idle state
            OSA_InterruptDisable();
            maSmacAttributes[instance].smacState = mSmacStateIdle_c;
            OSA_InterruptEnable();
          }
      }
      MEM_BufferFree(pMsg);
      return gPhySuccess_c;
    }
    //if SMAC isn't in TX then definitely it is a CCA confirm
    //allocate a message for the application
    pSmacToApp = MEM_BufferAlloc(sizeof(smacToAppMlmeMessage_t));
    if(pSmacToApp != NULL)
    {
      //type is CCA Confirm
      pSmacToApp->msgType = gMlmeCcaCnf_c;
      //Channel status translated into SMAC messages: idle channel means no error.
      if(pPlmeMsg->msgData.ccaCnf.status == gPhyChannelIdle_c)
      {
        pSmacToApp->msgData.ccaCnf.status = gErrorNoError_c;
      }
      else
      {
        pSmacToApp->msgData.ccaCnf.status = gErrorChannelBusy_c;
      }
    }
    break;
  case gPlmeEdCnf_c:
    //allocate a message for the application
    pSmacToApp = MEM_BufferAlloc(sizeof(smacToAppMlmeMessage_t));
    if(pSmacToApp != NULL)
    {
      //message type is ED Confirm
      pSmacToApp->msgType = gMlmeEdCnf_c;
      if(pPlmeMsg->msgData.edCnf.status == gPhySuccess_c)
      {
        pSmacToApp->msgData.edCnf.status = gErrorNoError_c;
        pSmacToApp->msgData.edCnf.energyLevel = pPlmeMsg->msgData.edCnf.energyLevel;
        pSmacToApp->msgData.edCnf.energyLeveldB = pPlmeMsg->msgData.edCnf.energyLeveldB;
        
        lSmacInstanceBackup = mSmacActivePan;
        MLMESetActivePan((smacMultiPanInstances_t)instance);
        pSmacToApp->msgData.edCnf.scannedChannel = MLMEGetChannelRequest();
        MLMESetActivePan(lSmacInstanceBackup);
      }
      else
      {
        pSmacToApp->msgData.edCnf.status = gErrorBusy_c;
      }
    }
    break;
  case gPlmeTimeoutInd_c:
    if(maSmacAttributes[instance].smacState == mSmacStateTransmitting_c)
    {
      if(maSmacAttributes[instance].txConfigurator.autoAck)
      {
        //re-arm retries for channel busy at retransmission.
        maSmacAttributes[instance].u8CCARetryCounter = 0;
        
        if(maSmacAttributes[instance].txConfigurator.retryCountAckFail > 
             maSmacAttributes[instance].u8AckRetryCounter)
        {
          maSmacAttributes[instance].u8AckRetryCounter++;
          
          RNG_GetRandomNo(&backOffTime);
          //start event timer. After time elapses, Data request will be fired.
          TMR_StartSingleShotTimer(maSmacAttributes[instance].u8BackoffTimerId, 
                                   ((backOffTime & gMaxBackoffTime_c) + gMinBackoffTime_c), 
                                   BackoffTimeElapsed, (void*)instance);
        }
        else
        {
          (void)MEM_BufferFree(maSmacAttributes[instance].gSmacDataMessage);
          maSmacAttributes[instance].gSmacDataMessage = NULL;
          
          //retries failed so create message for the application
          pSmacMsg = MEM_BufferAlloc(sizeof(smacToAppDataMessage_t));
          if(pSmacMsg != NULL)
          {
            //set error code: No Ack
            pSmacMsg->msgData.dataCnf.status = gErrorNoAck_c;
            //type is Data Confirm
            pSmacMsg->msgType = gMcpsDataCnf_c;
            
            maSmacAttributes[instance].gSMAC_APP_MCPS_SapHandler(pSmacMsg, instance);
          }
          //place SMAC into idle state
          OSA_InterruptDisable();
          maSmacAttributes[instance].smacState = mSmacStateIdle_c;
          OSA_InterruptEnable();
        }
      }
      MEM_BufferFree(pMsg);
      return gPhySuccess_c;
    }
    //if no ack timeout was received then it is definitely a RX timeout
    pSmacToApp = MEM_BufferAlloc(sizeof(smacToAppMlmeMessage_t));
    if(pSmacToApp != NULL)
    {
      if(maSmacAttributes[instance].smacState == mSmacStateReceiving_c)
      {
        maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer->rxStatus 
          = rxTimeOutStatus_c;
      }
      pSmacToApp->msgType = gMlmeTimeoutInd_c;
    }
    break;
  case gPlme_UnexpectedRadioResetInd_c:
    pSmacToApp = MEM_BufferAlloc(sizeof(smacToAppMlmeMessage_t));
    if(pSmacToApp != NULL)
    {
      pSmacToApp->msgType = gMlme_UnexpectedRadioResetInd_c;
    }
    break;
  default:
    //MEM_BufferFree(pSmacToApp);
    MEM_BufferFree(pMsg);
    return gPhySuccess_c;
    break;
  }
  OSA_InterruptDisable();
  maSmacAttributes[instance].smacState = mSmacStateIdle_c;
  OSA_InterruptEnable();
  //send message to upper layer if it is not NULL
  if(pSmacToApp != NULL)
  {
    maSmacAttributes[instance].gSMAC_APP_MLME_SapHandler(pSmacToApp,instance);
  }
  MEM_BufferFree(pMsg);
  return gPhySuccess_c;
}

/************************************************************************************
* Smac_RegisterSapHandlers
* 
* This function helps the user register the handlers for the messages that come from 
* SMAC.
* 
************************************************************************************/

void Smac_RegisterSapHandlers(
                              SMAC_APP_MCPS_SapHandler_t pSMAC_APP_MCPS_SapHandler,
                              SMAC_APP_MLME_SapHandler_t pSMAC_APP_MLME_SapHandler,
                              instanceId_t smacInstanceId
                                )
{
  maSmacAttributes[smacInstanceId].gSMAC_APP_MCPS_SapHandler = pSMAC_APP_MCPS_SapHandler;
  maSmacAttributes[smacInstanceId].gSMAC_APP_MLME_SapHandler = pSMAC_APP_MLME_SapHandler;
}

/************************************************************************************
* SMACFillHeader
* 
* This function helps the user fill the SMAC header(short hardcoded MAC header) with
* the desired short destination address.
* 
************************************************************************************/

void SMACFillHeader(smacHeader_t* pSmacHeader, address_size_t destAddr)
{
  pSmacHeader->frameControl = gSmacDefaultFrameCtrl_c;
#if !gUseSMACLegacy_c
  pSmacHeader->panId        = maSmacAttributes[mSmacActivePan].u16PanID;
  pSmacHeader->seqNo        = gSmacDefaultSeqNo_c;
  pSmacHeader->srcAddr      = maSmacAttributes[mSmacActivePan].u16ShortSrcAddress;
#endif
  pSmacHeader->destAddr     = destAddr;
}

/************************************************************************************
* InitSmac
* 
* Basic SMAC initialisation.
* 
************************************************************************************/
void InitSmac(void)
{
  uint32_t u32RandomNo = 0;
  RNG_Init();
#if defined(gPHY_802_15_4g_d)
  gTotalChannels = gPhyPib.pPIBphyRfConstants->totalNumChannels;
#else
  gTotalChannels = 26;
#endif
#if(TRUE == smacInitializationValidation_d)
    mSmacInitialized = TRUE;  
#endif
  /* SMAC Initialization */
  mSmacActivePan = gSmacPan0_c;
  while(mSmacActivePan < gSmacMaxPan_c)
  {
    maSmacAttributes[mSmacActivePan].smacState = mSmacStateIdle_c;
    maSmacAttributes[mSmacActivePan].smacLastDataRxParams.linkQuality = 0;
    maSmacAttributes[mSmacActivePan].smacLastDataRxParams.timeStamp = 0;
    
    /*clear defer tx flag*/
    macToPlmeMessage_t lMsg;
    lMsg.macInstance = mSmacActivePan;
    lMsg.msgType = gPlmeSetReq_c;
    lMsg.msgData.setReq.PibAttribute = gPhyPibDeferTxIfRxBusy_c;
    lMsg.msgData.setReq.PibAttributeValue = (uint64_t)FALSE;
    (void)MAC_PLME_SapHandler(&lMsg, 0);
    
    maSmacAttributes[mSmacActivePan].txConfigurator.autoAck = FALSE;
    maSmacAttributes[mSmacActivePan].txConfigurator.ccaBeforeTx = FALSE;
    maSmacAttributes[mSmacActivePan].txConfigurator.retryCountAckFail = 0;
    maSmacAttributes[mSmacActivePan].txConfigurator.retryCountCCAFail = 0;
    maSmacAttributes[mSmacActivePan].u8BackoffTimerId = (uint8_t)TMR_AllocateTimer();
    
    (void)SMACSetShortSrcAddress(gNodeAddress_c);
    (void)SMACSetPanID(gDefaultPanID_c);
    
    RNG_GetRandomNo(&u32RandomNo);
    maSmacAttributes[mSmacActivePan].u8SmacSeqNo = (uint8_t)u32RandomNo;
    mSmacActivePan = (smacMultiPanInstances_t)(mSmacActivePan + 1);
  }
  mSmacActivePan = gSmacPan0_c;
#if gUseSMACLegacy_c
  PhyPpSetPromiscuous(TRUE);
#endif
  //Notify the PHY what function to call for communicating with SMAC  
  Phy_RegisterSapHandlers((PD_MAC_SapHandler_t)PD_SMAC_SapHandler, (PLME_MAC_SapHandler_t)PLME_SMAC_SapHandler, 0);
}

/************************************************************************************
* SMACSetShortSrcAddress
* 
* This function sets the short source address so that PHY can perform filtering
* 
************************************************************************************/

smacErrors_t SMACSetShortSrcAddress(address_size_t nwShortAddress)
{
#if !gUseSMACLegacy_c
  macToPlmeMessage_t lMsg;
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgType = gPlmeSetReq_c;
  lMsg.msgData.setReq.PibAttribute = gPhyPibShortAddress_c;
  lMsg.msgData.setReq.PibAttributeValue = (uint64_t)nwShortAddress;
  
  phyStatus_t u8PhyRes = MAC_PLME_SapHandler(&lMsg,0);
  if(u8PhyRes == gPhyBusy_c || u8PhyRes == gPhyBusyTx_c || u8PhyRes == gPhyBusyRx_c)
  {
    return gErrorBusy_c;
  }
  if(u8PhyRes != gPhySuccess_c)
  {
    return gErrorNoResourcesAvailable_c;
  }
#endif
  maSmacAttributes[mSmacActivePan].u16ShortSrcAddress = nwShortAddress;
  return gErrorNoError_c;
}

/************************************************************************************
* SMACSetPanID
* 
* This function sets the pan ID so that PHY can perform filtering
* 
************************************************************************************/

smacErrors_t SMACSetPanID(address_size_t nwShortPanID)
{
#if !gUseSMACLegacy_c
  macToPlmeMessage_t lMsg;
  lMsg.macInstance = mSmacActivePan;
  lMsg.msgType = gPlmeSetReq_c;
  lMsg.msgData.setReq.PibAttribute = gPhyPibPanId_c;
  lMsg.msgData.setReq.PibAttributeValue = (uint64_t)nwShortPanID;
  
  phyStatus_t u8PhyRes = MAC_PLME_SapHandler(&lMsg,0);
  if(u8PhyRes == gPhyBusy_c || u8PhyRes == gPhyBusyTx_c || u8PhyRes == gPhyBusyRx_c)
  {
    return gErrorBusy_c;
  }
  if(u8PhyRes != gPhySuccess_c)
  {
    return gErrorNoResourcesAvailable_c;
  }
#endif
  maSmacAttributes[mSmacActivePan].u16PanID = nwShortPanID;
  return gErrorNoError_c;
}

/************************************************************************************
* BackoffTimeElapsed
* 
* Callback run after backoff time expires.
* 
************************************************************************************/
void BackoffTimeElapsed(void* param)
{
  uint32_t lsmacInstance = 0;
  lsmacInstance = (uint32_t)param;
  uint8_t u8PhyRes = MAC_PD_SapHandler(maSmacAttributes[lsmacInstance].gSmacDataMessage, 0);
  if(u8PhyRes != gPhySuccess_c)
  {
    OSA_InterruptDisable();
    maSmacAttributes[lsmacInstance].smacState = mSmacStateIdle_c;
    OSA_InterruptEnable();
    
    MEM_BufferFree(maSmacAttributes[lsmacInstance].gSmacDataMessage);
    maSmacAttributes[lsmacInstance].gSmacDataMessage = NULL;
  }
}

/************************************************************************************
* SMACPacketCheck
* 
* This function returns TRUE if Phy payload can be of SMAC packet type and if the SDU
* size is smaller than the configured maximum size;
* 
************************************************************************************/

static bool_t SMACPacketCheck
(
pdDataToMacMessage_t* pMsgFromPhy, 
smacMultiPanInstances_t instance
)
{
#if !gUseSMACLegacy_c
  //check if packet is of type Data
  if( (pMsgFromPhy->msgData.dataInd.pPsdu[0] & 0x07) != 0x01 )
    return FALSE;
#else
  if( (pMsgFromPhy->msgData.dataInd.pPsdu[0] != 0x7E) || 
       (pMsgFromPhy->msgData.dataInd.pPsdu[1] != 0xFF))
    return FALSE;
  if( (pMsgFromPhy->msgData.dataInd.pPsdu[2] != gBroadcastAddress_c)
        && (pMsgFromPhy->msgData.dataInd.pPsdu[2] != u16ShortSrcAddress))
    return FALSE;
#endif
  //check if PSDU length is at least of SMAC header size.
  if( (pMsgFromPhy->msgData.dataInd.psduLength < gSmacHeaderBytes_c) )
    return FALSE;
  //check if PSDU length is greater than the maximum configured SMAC packet size.
  if( pMsgFromPhy->msgData.dataInd.psduLength > 
       (maSmacAttributes[instance].smacProccesPacketPtr.smacRxPacketPointer->u8MaxDataLength + gSmacHeaderBytes_c))
    return FALSE;
  
  return TRUE;
}

#if gSmacUseSecurity_c
/************************************************************************************
* SMAC Security primitives
* 
* These primitives allow to:
* - Set IV and KEY parameters;
* - Pad and Encrypt message using AES_128_CBC
* - Decrypt using AES_128_CBC and Depad
************************************************************************************/
void SMAC_SetIVKey(uint8_t* KEY, uint8_t* IV)
{
  FLib_MemCpy(maSmacAttributes[mSmacActivePan].secInit.KEY, KEY, ENC_BLOCK_SIZE);
  FLib_MemCpy(maSmacAttributes[mSmacActivePan].secInit.IV, IV, ENC_BLOCK_SIZE);
}
/************************************************************************************/
static void SMAC_Encrypt(uint8_t* pIn, uint8_t* pOut, uint8_t *len, smacMultiPanInstances_t panID)
{
  *len = AES_128_CBC_Encrypt_And_Pad(pIn, 
                                     (uint32_t)(*len), 
                                     maSmacAttributes[panID].secInit.IV, 
                                     maSmacAttributes[panID].secInit.KEY, 
                                     pOut);  
}
/************************************************************************************/
static void SMAC_Decrypt(uint8_t* pIn, uint8_t* pOut, uint8_t *len, smacMultiPanInstances_t panID)
{
  *len = AES_128_CBC_Decrypt_And_Depad(pIn, 
                                       (uint32_t)(*len), 
                                       maSmacAttributes[panID].secInit.IV, 
                                       maSmacAttributes[panID].secInit.KEY, 
                                       pOut);
}
#endif