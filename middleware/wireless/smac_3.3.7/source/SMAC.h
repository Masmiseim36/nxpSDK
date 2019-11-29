/*! *********************************************************************************
* Copyright (c) 2004 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

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
#define gSmacVerPatch_c   7
#define gSmacBuildNo_c    0

#define gSmacModuleId_c  0xD1

#define gSmacVerString_c "SMAC v" \
                        QUH(gSmacVerMajor_c) "." \
                        QUH(gSmacVerMinor_c) "." \
                        QUH(gSmacVerPatch_c) "." \
                        QUH(gSmacBuildNo_c)

#endif /* SMAC_H_ */
