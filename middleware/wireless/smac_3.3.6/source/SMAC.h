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

#ifndef SMAC_H_
#define SMAC_H_

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/

#include "Phy.h"
#include "PhyInterface.h"
#include "MpmInterface.h"
#include "SMAC_Interface.h"
#include "TimersManager.h"
#include "RNG_Interface.h"

#if defined (gPHY_802_15_4g_d)
#include "PhyPib.h"
#include "PhyExtended.h"
#include "PhyTime.h"
#endif

#include "ModuleInfo.h"

/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/
void SmacSetRxTimeout(smacTime_t timeoutSymbols);
extern void InitSmac(void);

#if defined (gPHY_802_15_4g_d)

#define smacPreambleSizeOf16_c			(16)
#define smacPreambleSizeOf3_c			(3)

#endif
/*smacParametersValidation_d:
TRUE :  SMAC primitives validate their incoming parameters.
FALSE:  SMAC primitives do their stuff without validating their incoming 
parameters
*Note: Setting this as FALSE will compile a smaller foot print SMAC.
*/
#define smacParametersValidation_d      	TRUE

/*smacInitializationValidation_d:
TRUE :  SMAC primitives validate whether SMAC is initialized or not.
FALSE:  SMAC primitives don't care about SMAC's initialization.
*Note: Setting this as FALSE will compile a smaller foot print SMAC.
*/
#define smacInitializationValidation_d  	TRUE

#define gFrameCtrlAckReqMsk_c                  (1 << 5)
/************************************************************************************
*************************************************************************************
* Module Type definitions
*************************************************************************************
************************************************************************************/
typedef enum smacStates_tag {
  mSmacStateIdle_c,
  mSmacStateTransmitting_c,
  mSmacStateReceiving_c,
  mSmacStateScanningChannels_c,
  mSmacStatePerformingCca_c,
  mSmacStatePerformingEd_c,
  mSmacStatePerformingTest_c,
  mSmacStateHibernate_c, 
  mSmacStateDoze_c    
} smacStates_t;

typedef union prssPacketPtr_tag
{
  uint8_t*    smacScanResultsPointer;     
  rxPacket_t  *smacRxPacketPointer;
  pdDataReq_t *smacTxPacketPointer;
}prssPacketPtr_t;

/***********************************************************************************
* Phy to SMAC SAP prototype
************************************************************************************/
typedef phyStatus_t ( * PD_SMAC_SapHandler_t)(pdDataToMacMessage_t * pMsg, instanceId_t instanceId);

typedef phyStatus_t ( * PLME_SMAC_SapHandler_t)(plmeToMacMessage_t * pMsg, instanceId_t instanceId);

/***********************************************************************************
* SMAC internal attributes
************************************************************************************/
typedef struct smacInternalAttrib_tag
{
  smacStates_t smacState;
  prssPacketPtr_t smacProccesPacketPtr;
  phyRxParams_t   smacLastDataRxParams;
  txContextConfig_t txConfigurator;
#if (gPHY_802_15_4g_d)
  uint8_t u8SyncWordSize;
#endif
  macToPdDataMessage_t * gSmacDataMessage;
  macToPlmeMessage_t *   gSmacMlmeMessage;
  SMAC_APP_MCPS_SapHandler_t gSMAC_APP_MCPS_SapHandler;
  SMAC_APP_MLME_SapHandler_t gSMAC_APP_MLME_SapHandler;
  
  address_size_t u16PanID;
  address_size_t u16ShortSrcAddress;
  
  uint8_t u8AckRetryCounter;
  uint8_t u8CCARetryCounter;
  uint8_t mSmacTimeoutAsked;
  
  uint8_t u8BackoffTimerId;
  uint8_t u8SmacSeqNo;
#if (gSmacUseSecurity_c)
  smacEncryptionKeyIV_t secInit;
#endif
} smacInternalAttrib_t;
/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

/*! @brief SMAC build version  */
#define gSmacVerMajor_c   3
#define gSmacVerMinor_c   3
#define gSmacVerPatch_c   6
#define gSmacBuildNo_c    0

#define gSmacModuleId_c  0xD1

#define gSmacVerString_c "SMAC v" \
                        QUH(gSmacVerMajor_c) "." \
                        QUH(gSmacVerMinor_c) "." \
                        QUH(gSmacVerPatch_c) "." \
                        QUH(gSmacBuildNo_c)

#endif /* SMAC_H_ */
