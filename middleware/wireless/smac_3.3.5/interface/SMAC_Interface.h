/*
* The Clear BSD License
* Copyright (c) 2004 - 2015, Freescale Semiconductor, Inc.
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

#ifndef SMAC_INTERFACE_H_
#define SMAC_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"                   
#include "PhyTypes.h"
/************************************************************************************
*************************************************************************************
* Interface macro definitions 
*************************************************************************************
************************************************************************************/
#ifndef gUseSMACLegacy_c
#define gUseSMACLegacy_c           (0)
#endif

#ifndef gSmacUseSecurity_c
#define gSmacUseSecurity_c         (0)
#endif

#define gSmacHeaderBytes_c	   ( sizeof(smacHeader_t) )

#if !gSmacUseSecurity_c
#define gMaxSmacSDULength_c        (gMaxPHYPacketSize_c -(sizeof(smacHeader_t) + 2) )
#else
#define gMaxSmacSDULength_c        (gMaxPHYPacketSize_c -(sizeof(smacHeader_t) + 2) - 16)
#endif

#define gMinSmacSDULength_c	   (0)

#if !gUseSMACLegacy_c
 #define gNodeAddress_c            (0xBEAD)
 #define gDefaultPanID_c           (0xFACE)
 #define gBroadcastAddress_c	   (0xFFFF)
 #define gSmacDefaultFrameCtrl_c   (0x8841)
#else
 #define gNodeAddress_c            (0xAA)
 #define gDefaultPanID_c           (0xBB)
 #define gBroadcastAddress_c	   (0xFF)
 #define gSmacDefaultFrameCtrl_c   (0xFF7E)
#endif

#define gSmacDefaultSeqNo_c        (0xAC)
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
// extern bool_t smacStandalone;          
extern uint8_t gTotalChannels;

/************************************************************************************
*************************************************************************************
* Interface Type definitions
*************************************************************************************
************************************************************************************/
#if !gUseSMACLegacy_c
typedef uint16_t address_size_t;
#else
typedef uint8_t address_size_t;
#endif

typedef enum smacMultiPanInstances_tag
{
  gSmacPan0_c = 0,
#if gMpmMaxPANs_c == 2
  gSmacPan1_c,
#endif
  gSmacMaxPan_c
}smacMultiPanInstances_t;

typedef enum smacMessageDefs_tag 
{
  gMcpsDataCnf_c,
  gMcpsDataInd_c,
 
  gMlmeCcaCnf_c,
  
  gMlmeEdCnf_c,
   
  gMlmeSetReq_c,
  gMlmeSetCnf_c,
  
  gMlmeTimeoutInd_c,
  
  gMlme_UnexpectedRadioResetInd_c
}smacMessageDefs_t;

typedef uint64_t smacTime_t;

typedef struct smacPdu_tag
{
  uint8_t smacPdu[1];
}smacPdu_t;

typedef PACKED_STRUCT smacHeader_tag
{
  uint16_t frameControl;
#if !gUseSMACLegacy_c
  uint8_t seqNo;
  address_size_t       panId;
  address_size_t       destAddr;
  address_size_t       srcAddr;
#else
  address_size_t destAddr;
#endif
}smacHeader_t;

typedef struct txPacket_tag
{
  uint8_t u8DataLength;
  smacHeader_t smacHeader;
  smacPdu_t smacPdu;
}txPacket_t;

typedef struct txContextConfig_tag
{
  bool_t ccaBeforeTx;
  bool_t autoAck;
  uint8_t retryCountCCAFail;
  uint8_t retryCountAckFail;
}txContextConfig_t;

typedef enum rxStatus_tag
{
  rxInitStatus,
  rxProcessingReceptionStatus_c,
  rxSuccessStatus_c, 
  rxTimeOutStatus_c,
  rxAbortedStatus_c,
  rxMaxStatus_c
} rxStatus_t;

typedef struct rxPacket_tag
{
  uint8_t    u8MaxDataLength;
  uint8_t    u8DataLength;
  rxStatus_t rxStatus;
  smacMultiPanInstances_t instanceId;
  smacHeader_t smacHeader;
  smacPdu_t  smacPdu;
}rxPacket_t;

typedef enum smacErrors_tag
{
  gErrorNoError_c = 0,
  gErrorBusy_c,
  gErrorChannelBusy_c,
  gErrorNoAck_c,
  gErrorOutOfRange_c,
  gErrorNoResourcesAvailable_c,
  gErrorNoValidCondition_c,
  gErrorCorrupted_c,
  gErrorMaxError_c
} smacErrors_t;

#if defined (gPHY_802_15_4g_d)

typedef enum smacRFModes_tag
{
  gRFMode1_c = gPhyMode1_c,
  gRFMode2_c = gPhyMode2_c,
  gRFMode3_c = gPhyMode3_c,
  gRFMode4_c = gPhyMode4_c,
  gRFMode5_c = gPhyMode1ARIB_c, /*ARIB mode 1*/
  gRFMode6_c = gPhyMode2ARIB_c, /*ARIB mode 2*/
  gRFMaxMode_c
} smacRFModes_t;

typedef enum smacFrequencyBands_tag
{
  gSMAC_863_870MHz_c = gFreq863__870MHz_c, /* 863-870   (Europe) */ 
  gSMAC_902_928MHz_c = gFreq902__928MHz_c, /* 902-928   (US)     */ 
  gSMAC_920_928MHz_c = gFreq920__928MHz_c, /* 920-928 (Japan)    */
  gSMAC_169_400__169_475MHz_c = gFreq169_400__169_475MHz_c /* 169-475 */
}smacFrequencyBands_t;

#endif

#if defined (gPHY_802_15_4g_d)

typedef struct packetConfig_tag
{	
  uint16_t u16PreambleSize;
  uint8_t u8SyncWordSize;
  uint8_t *pu8SyncWord;
} packetConfig_t;

#endif

typedef enum channels_tag
{
#include "SMAC_Channels.h"
} channels_t;

typedef enum smacTestMode_tag  
{
  gTestModeForceIdle_c = 0,
  gTestModeContinuousTxModulated_c,
  gTestModeContinuousTxUnmodulated_c,
  gTestModePRBS9_c,
  gTestModeContinuousRxBER_c,
  gMaxTestMode_c
} smacTestMode_t;

typedef  struct smacDataCnf_tag
{
  smacErrors_t         status;
} smacDataCnf_t;

typedef  struct smacDataInd_tag
{
  uint8_t                 u8LastRxRssi;
  rxPacket_t *            pRxPacket;
} smacDataInd_t;

typedef  struct smacCcaCnf_tag
{
  smacErrors_t       status;
} smacCcaCnf_t;

typedef  struct smacEdCnf_tag
{
  smacErrors_t            status;
  uint8_t                 energyLevel;
  uint8_t                 energyLeveldB;
  channels_t              scannedChannel;
} smacEdCnf_t;

typedef  struct smacToAppMlmeMessage_tag
{
  smacMessageDefs_t          msgType;
  uint8_t                    appInstanceId;
  union
  {
    smacCcaCnf_t       ccaCnf;
    smacEdCnf_t             edCnf;
  }msgData;
} smacToAppMlmeMessage_t;

typedef  struct smacToAppDataMessage_tag
{
  smacMessageDefs_t            msgType;
  uint8_t                    appInstanceId;
  union
  {
    smacDataCnf_t             dataCnf;
    smacDataInd_t             dataInd;
  }msgData;
} smacToAppDataMessage_t;

typedef smacErrors_t ( * SMAC_APP_MCPS_SapHandler_t)(smacToAppDataMessage_t * pMsg, instanceId_t instanceId);

typedef smacErrors_t ( * SMAC_APP_MLME_SapHandler_t)(smacToAppMlmeMessage_t * pMsg, instanceId_t instanceId);

#if gSmacUseSecurity_c

#include "SecLib.h"
typedef struct smacEncryptionKeyIV_tag
{
  uint8_t IV[16];
  uint8_t KEY[16];
}smacEncryptionKeyIV_t;

#endif
/************************************************************************************
*************************************************************************************
* External Prototypes
*************************************************************************************
************************************************************************************/
/***********************************************************************************
* Smac_RegisterSapHandlers
* This function registers the data and management components callbacks to the application
* After calling this function and providing two function pointers, SMAC will call 
* one of these two, for each async request, based on request type (data or management)
*
* Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used. 
*
* Return value:  
*   None
************************************************************************************/
extern void Smac_RegisterSapHandlers(SMAC_APP_MCPS_SapHandler_t pSMAC_APP_MCPS_SapHandler,
                                     SMAC_APP_MLME_SapHandler_t pSMAC_APP_MLME_SapHandler,
                                     instanceId_t smacInstanceId);

/***********************************************************************************
* App to Smac SAP HANDLERS
************************************************************************************/
//smacErrors_t AppToSmac_Data_SapHandler(AppToSmacDataMessage_t* pMsg, uint8_t MACInstance);
//smacErrors_t AppToSmac_Mlme_SapHandler(AppToSmacMlmeMessage_t* pMsg, uint8_t MACInstance);
/***********************************************************************************/

/******************************** SMAC Data primitives *****************************/
/***********************************************************************************/

/************************************************************************************
* MCPSDataRequest
* 
* This data primitive is used to send an over the air packet. This is an asyncronous 
* function,  it means it ask SMAC to transmit one OTA packet,  but when the function 
* returns it is not sent already.
*
* Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used. 
*
* Return value:  
*   gErrorNoError_c: Everything is ok and the transmission will be performed.
*   gErrorOutOfRange_c: One of the members in the pTxMessage structure is out of 
*                      range (no valid bufer size or data buffer pointer is NULL)
*   gErrorNoResourcesAvailable_c: the radio is performing another action.
*   gErrorNoValidCondition_c: The SMAC has not been initialized 
*
************************************************************************************/
extern smacErrors_t MCPSDataRequest(txPacket_t *psTxPacket);

/***********************************************************************************/
/******************************** SMAC Radio primitives ****************************/
/***********************************************************************************/

/************************************************************************************
* MLMEConfigureTxContext
* 
* This management primitive sets up the transmission conditions used by MCPSDataRequest
*
* Interface assumptions:
*   SMAC is initialized 
*
* Return value:  
*   gErrorNoError_c: Everything is set accordingly.
*   gErrorOutOfRange_c: More than gMaxRetriesAllowed_c are required
*   gErrorNoValidCondition_c: Retries are required but neither Ack nor CCA are requested 
*
************************************************************************************/
extern smacErrors_t MLMEConfigureTxContext(txContextConfig_t* pTxConfig);

/************************************************************************************
* MLMESetActivePan
* 
* This management primitive switches between pans
*
* Interface assumptions:
*   SMAC is initialized 
* 
* Parameters: panID the ID of the pan to switch to.
*
* Return value:  
*   gErrorNoError_c: Everything is set accordingly.
*   gErrorOutOfRange_c: Maximum available pans is exceeded
*   gErrorNoValidCondition_c: Not initialized
*
************************************************************************************/
extern smacErrors_t MLMESetActivePan(smacMultiPanInstances_t panID);

/************************************************************************************
* MLMEConfigureDualPanSettings
* 
* This management primitive configures the multipan settings
*
* Interface assumptions:
*   SMAC is initialized 
* 
* Parameters: bUseAutoMode specifies if automatical switch between pans 
*                          in rx is enabled
*             bModifyDwell specifies if next parameters should be taken
*                          into consideration
*             u8Prescaler  can be 0,1,2,3. Check MpmInterface.h for its meaning
*             u8Scale      can be between 0 and 63. Check MpmInterface for its meaning
*
* Return value:  
*   gErrorNoError_c: Everything is set accordingly.
*   gErrorOutOfRange_c: Parameters exceed range
*   gErrorNoValidCondition_c: Not initialized
*   gErrorBusy_c SMAC is busy on (at least) one of the pans
*
************************************************************************************/
extern smacErrors_t MLMEConfigureDualPanSettings
(
bool_t bUseAutoMode,
bool_t bModifyDwell,
uint8_t u8Prescaler, 
uint8_t u8Scale
);

/************************************************************************************
* MLMERXEnableRequest
* 
* Function used to place the radio into receive mode
* 
* Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
*    
* Arguments:
* 
*        rxPacket_t *gsRxPacket: Pointer to the structure where the reception results will be stored.
*        smacTime_t stTimeout: 64-bit timeout value, absolute time in symbols
*        
*  Return Value:
*		gErrorNoError_c: Everything is ok and the reception will be performed.
*		gErrorOutOfRange_c: One of the members in the pRxMessage structure is out of range (no valid bufer size or data buffer pointer is NULL).
*		gErrorBusy_c: the radio is performing another action.
*		gErrorNoValidCondition_c: The SMAC has not been initialized.
*************************************************************************************/
extern smacErrors_t MLMERXEnableRequest(rxPacket_t *gsRxPacket, smacTime_t stTimeout);


/************************************************************************************
* MLMERXDisableRequest
* 
* Returns the radio to idle mode from receive mode.
* 
*   Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*   Arguments: 
*   None 
*   
*   Return Value:
*      gErrorNoValidCondition_c If the Radio is not in Rx state.
*      gErrorNoError_c When the message was aborted or disabled.
*************************************************************************************/
extern smacErrors_t MLMERXDisableRequest(void);

/************************************************************************************
* MLMELinkQuality
* 
*  This function returns an integer value that is the link quality from the last received 
*  packet.
* 
*   Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*   Arguments: 
*   None 
*   
*   Return Value:
*   uint8_t: 8 bit value representing the link quality value in dBm.
*   Returns the result in smacLastDataRxParams.linkQuality.
*************************************************************************************/
extern uint8_t  MLMELinkQuality(void);

/************************************************************************************
* MLMESetInterPacketRxDelay
* 
* This sets the inter packet delay for the packet handler 
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*   uint8_t u8InterPacketRxDelay: interpacket delay in ms
*   
*  Return Value:
*      gErrorNoValidCondition_c If the Radio is not in Rx state.
*      gErrorNoError_c When the message was aborted or disabled.
*
*************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMESetInterPacketRxDelay(uint8_t u8InterPacketRxDelay);
#endif

/************************************************************************************
* MLMESetChannelRequest
*
* This sets the frequency on which the radio will transmit or receive on.
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*    channels_t newChannel: channel to be set
*   
*  Return Value:
*   gErrorNoError_c: The channel set has been performed
*   gErrorOutOfRange_c : channel requested is not valid
*   gErrorBusy_c: when SMAC is busy in other radio activity as transmitting or receiving data. Or performing a channel scan.
*************************************************************************************/
extern smacErrors_t MLMESetChannelRequest (channels_t newChannel);

#if CT_Feature_Calibration
/************************************************************************************
* MLMESetAdditionalRFOffset
*
* This sets the frequency offset in respect to the current channel. Used for calibration.
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*    uint32_t additionalRFOffset: offset used in frequency Calculation
*   
*  Return Value:
*   gErrorNoError_c: The PIB is set
*   gErrorNoValidCondition_c: SMAC is not initialized
*************************************************************************************/
extern smacErrors_t MLMESetAdditionalRFOffset (uint32_t additionalRFOffset);

/************************************************************************************
* MLMEGetAdditionalRFOffset
*
* This gets the frequency offset in respect to the current channel. Used for calibration.
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*    None
*   
*  Return Value:
*   calibration offset
*************************************************************************************/
extern uint32_t MLMEGetAdditionalRFOffset( void );

/************************************************************************************
* MLMESetAdditionalRFOffset
*
* This sets the energy detect offset for the following ED requests. Used for calibration.
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*    uint8_t additionalEDOffset: offset used in ED Calculation
*   
*  Return Value:
*   gErrorNoError_c: The PIB is set
*   gErrorNoValidCondition_c: SMAC is not initialized
*************************************************************************************/
extern smacErrors_t MLMESetAdditionalEDOffset (uint8_t additionalEDOffset);

/************************************************************************************
* MLMEGetAdditionalEDOffset
*
* This gets the energy detect offset. Used for calibration.
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*    None
*   
*  Return Value:
*   ED calibration offset
*************************************************************************************/
extern uint8_t MLMEGetAdditionalEDOffset( void );
#endif
/************************************************************************************
* MLMEGetChannelRequest
* 
* This function returns the current channel, if an error is detected it returns gChannelOutOfRange_c.
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*   None 
*   
*  Return Value:
*  channels_t (uint8_t): The current RF channel.
*  gChannelOutOfRange_c: If current channel could not be detected
*************************************************************************************/
extern channels_t MLMEGetChannelRequest(void);


/************************************************************************************
* MLMERssi
*
* This call starts an energy detect (ED) cycle and returns the energy value (-power/2) 
* via the returned argument. For example, if the Energy Detect returns 80 then the 
* interpreted value is -80/2 or -40 dBm
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*   None 
*   
*  Return Value:
*  uint8_t: An unsigned 8-bit value representing the energy on the current channel.
*************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern uint8_t MLMERssi(void);
#endif

/************************************************************************************
* MLMESetCCADuration
*
* This call sets the amount of time necessary to perform CCA or ED
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*   uint64_t usCCADuration: duration in microseconds
*   
*  Return Value:
*  gErrorNoError_c in case of success, error code otherwise.
*************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMESetCCADuration(uint64_t usCCADuration );
#endif
/************************************************************************************
* MLMEPacketConfig
*
* This function sets the following parameters for OTA packets in radio’s registers:
* Preamble size, synchronization word size, and synchronization word value.
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*   packetConfig_t *pPacketCfg
*   
*  Return Value:
*  gErrorBusy_c: when SMAC is busy in other radio activity as transmitting or receiving data. 
*  Or performing a channel scan.
*  gErrorNoError_c: the packet has been configured 
*************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMEPacketConfig(packetConfig_t *pPacketCfg);
#endif
/************************************************************************************
* MLMERadioInit
* 
* This function initializes the Radio parameters.
* 
*  Interface assumptions:
*   None
* 
*  Arguments: 
*   None 
*   
*  Return Value:
*  gErrorNoError_c: the Radio initialization has been done successfully
*************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMERadioInit(void);
#endif
/************************************************************************************
* MLMEPhySoftReset
*
* This function performs a software reset on the radio, PHY and SMAC state machines.
* 
*  Interface assumptions:
*   None
* 
*  Arguments: 
*   None 
*   
*  Return Value:
*  gErrorNoError_c: If the action is performed.
*************************************************************************************/
extern smacErrors_t MLMEPhySoftReset(void);

/************************************************************************************
* MLMEPAOutputAdjust
*
*
*************************************************************************************/
extern smacErrors_t MLMEPAOutputAdjust(uint8_t u8PaValue);

/************************************************************************************
* MLMESetPreambleLength
*
*
*************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMESetPreambleLength(uint16_t u16preambleLength);
#endif
/************************************************************************************
* MLMESetSyncWordValue
*
*
*************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMESetSyncWordValue(uint8_t *u8syncWordValue);
#endif
/************************************************************************************
* MLMESetSyncWordSize
*
*
*************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMESetSyncWordSize(uint8_t u8syncWordSize);
#endif
/************************************************************************************
* MLMESetFreqBand
* 
*  
*
************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMESetFreqBand(smacFrequencyBands_t freqBand, smacRFModes_t phyMode);
#endif
/************************************************************************************
* MLMESetPhyMode
* 
*  
*
************************************************************************************/
#if defined(gPHY_802_15_4g_d)
extern smacErrors_t MLMESetPhyMode(smacRFModes_t phyMode);
#endif
/***********************************************************************************/
/***************************** SMAC Management primitives **************************/
/***********************************************************************************/

/************************************************************************************
* MLMEScanRequest
*
*  This function scans the channel passed as parameter using CCA or ED mode
*  returns the RSSI in that channel.
* 
*  Interface assumptions:
*   The SMAC and radio driver have been initialized and are ready to be used.
* 
*  Arguments: 
*   channels_t u8ChannelToScan: channel to scan 
*  uint8_t *u8ChannelScanResult: to return the RSSI value 
*   
*  Return Value:
*   gErrorNoError_c:  If the action was performed correctly.
*   gErrorBusy_c:     If SMAC is busy. 
*************************************************************************************/
extern smacErrors_t MLMEScanRequest(channels_t u8ChannelToScan);

/*@CMA, Conn Test Added*/
/************************************************************************************
* MLMECcaRequest
* 
* This  function  performs Clear Channel Assessment on the active channel  
* 
* Return value:
* gErrorNoError_c: SMAC will perform Cca 
* gErrorBusy_c:    SMAC is busy
************************************************************************************/
extern smacErrors_t MLMECcaRequest();

/************************************************************************************
* MLMETXDisableRequest
* 
* Returns the radio to idle mode from Tx mode.
*
************************************************************************************/
extern void MLMETXDisableRequest(void);

/***********************************************************************************
******************************** SMAC MCU primitives *******************************
***********************************************************************************/

/************************************************************************************
*SMAC auxiliary functions
*************************************************************************************/

/************************************************************************************
*SMACFillHeader
*This is a replacement for the u8DestAddress in the txPacket_t structure
*To set the destination address simply call this function with a pointer to 
*txPacket->smacHeader and a uint16_t variable resembling the address.
*Also, to change the default source address and panID, modify gDefaultPanID_c and
*gNodeAddress_c from SMAC_Config.h or call SMACSetShortSrcAddress and SMACSetPanID.
*************************************************************************************/
extern void SMACFillHeader(smacHeader_t* pSmacHeader, address_size_t destAddr);

/***********************************************************************************/
extern smacErrors_t SMACSetShortSrcAddress(address_size_t nwShortAddress);

/***********************************************************************************/
extern smacErrors_t SMACSetPanID(address_size_t nwShortPanID);

/***********************************************************************************/
#if gSmacUseSecurity_c
extern void SMAC_SetIVKey(uint8_t* KEY, uint8_t* IV );
#endif
/***********************************************************************************/


#endif /* SMAC_INTERFACE_H_ */
