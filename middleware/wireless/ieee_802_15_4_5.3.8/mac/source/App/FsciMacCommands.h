/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the private header file for the MAC FSCI cmmands.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


#ifndef _FSCI_MAC_COMMANDS_H_
#define _FSCI_MAC_COMMANDS_H_


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "FsciInterface.h"
#include "MacInterface.h"

#if gFsciIncluded_c && gFsciHost_802_15_4_c
#include "fsl_os_abstraction.h"
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
#if gFsciIncluded_c && gFsciHost_802_15_4_c
typedef struct FsciHost_MacInterface_tag
{               
    MCPS_NWK_SapHandler_t pfMCPS_NWK_SapHandler;
    MLME_NWK_SapHandler_t pfMLME_NWK_SapHandler;
    instanceId_t          upperLayerId;
    uint8_t               fsciInterfaceId;
}FsciHostMacInterface_t;
#endif

/*
 * Operation Codes used in packets sent/received from the Serial Interface
 */
enum { /* NWK_MLME_SapHandler() OG = 0x85*/
    mFsciNwkMlmeAssociateReq_c    = 0x00,
    mFsciNwkMlmeAssociateRes_c    = 0x01,
    mFsciNwkMlmeDisassociateReq_c = 0x02,
    mFsciNwkMlmeGetReq_c          = 0x03,
    mFsciNwkMlmeGtsReq_c          = 0x04,
    mFsciNwkMlmeOrphanRes_c       = 0x05,
    mFsciNwkMlmeResetReq_c        = 0x06,
    mFsciNwkMlmeRxEnableReq_c     = 0x07,
    mFsciNwkMlmeScanReq_c         = 0x08,
    mFsciNwkMlmeSetReq_c          = 0x09,
    mFsciNwkMlmeStartReq_c        = 0x0A,
    mFsciNwkMlmeSyncReq_c         = 0x0B,
    mFsciNwkMlmePollReq_c         = 0x0C,
    mFsciNwkMlmeSetSlotframeReq_c = 0x0D,
    mFsciNwkMlmeSetLinkReq_c      = 0x0E,
    mFsciNwkMlmeTschModeReq_c     = 0x0F,
    mFsciNwkMlmeKeepAliveReq_c    = 0x10,
    mFsciNwkMlmeBeaconReq_c       = 0x11,
};

enum { /* MLME_NWK_SapHandler() OG = 0x84 */
    mFsciMlmeNwkAssociateInd_c       = 0x00,
    mFsciMlmeNwkAssociateCnf_c       = 0x01,
    mFsciMlmeNwkDisassociateInd_c    = 0x02,
    mFsciMlmeNwkDisassociateCnf_c    = 0x03,
    mFsciMlmeNwkBeaconNotifyInd_c    = 0x04,
    mFsciMlmeNwkGetCnf_c             = 0x05,
    mFsciMlmeNwkGtsInd_c             = 0x06,
    mFsciMlmeNwkGtsCnf_c             = 0x07,
    mFsciMlmeNwkOrphanInd_c          = 0x08,
    mFsciMlmwNwkResetCnf_c           = 0x09,
    mFsciMlmeNwkRxEnableCnf_c        = 0x0A,
    mFsciMlmeNwkScanCnf_c            = 0x0B,
    mFsciMlmeNwkCommStatusInd_c      = 0x0C,
    mFsciMlmeNwkSetCnf_c             = 0x0D,
    mFsciMlmeNwkStartCnf_c           = 0x0E,
    mFsciMlmeNwkSyncLossInd_c        = 0x0F,
    mFsciMlmeNwkPollCnf_c            = 0x10,
    mFsciMlmeNwkErrorCnf_c           = 0x11,
    mFsciMlmeNwkBeaconStartInd_c     = 0x12,
    mFsciMlmeNwkMaintenanceScanCnf_c = 0x13,
    mFsciMlmeNwkPollNotifyInd_c      = 0x14,
    mFsciMlmeNwkSetSlotframeCnf_c    = 0x15,
    mFsciMlmeNwkSetLinkCnf_c         = 0x16,
    mFsciMlmeNwkTschModeCnf_c        = 0x17,
    mFsciMlmeNwkKeepAliveCnf_c       = 0x18,
    mFsciMlmeNwkBeaconCnf_c          = 0x19,
};

enum { /* NWK_MCPS_SapHandler() OG = 0x87 */
    mFsciNwkMcpsDataReq_c  = 0x00,
    mFsciNwkMcpsPurgeReq_c = 0x01,
};

enum { /* MCPS_NWK_SapHandler() OG = 0x86 */
    mFsciMcpsNwkDataCnf_c  = 0x00,
    mFsciMcpsNwkDataInd_c  = 0x01,
    mFsciMcpsNwkPurgeCnf_c = 0x02,
    mFsciMcpsNwkPromInd_c  = 0x03,
};

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define gFSCI_MlmeNwkOpcodeGroup_c       0x84    /* MLME_NWK_SapHandler          */
#define gFSCI_NwkMlmeOpcodeGroup_c       0x85    /* NWK_MLME_SapHandler          */
#define gFSCI_McpsNwkOpcodeGroup_c       0x86    /* MCPS_NWK_SapHandler          */
#define gFSCI_NwkMcpsOpcodeGroup_c       0x87    /* NWK_MCPS_SapHandler          */

#if gFsciHost_802_15_4_c
#ifndef gMaxPHYPacketSize_c
#ifdef gPHY_802_15_4g_d
#define gMaxPHYPacketSize_c 254
#else
#define gMaxPHYPacketSize_c 125
#endif
#endif
#endif

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void fsciRegisterMac( instanceId_t macInterface, uint32_t fsciInterfaceId );
void fsciRegisterMacToHost( instanceId_t macInterface, uint32_t fsciInterfaceId );

uint32_t fsciGetMacInterfaceId( instanceId_t macInstance );
uint32_t fsciHostGetMacInterfaceId( instanceId_t macInstance );

instanceId_t fsciGetMacInstanceId( uint32_t interfaceId );
instanceId_t fsciHostGetMacInstanceId( uint32_t interfaceId );

void MAC_Monitor(uint8_t sapId, void *pMsg, void* param, uint8_t instanceId );

void fsciMcpsReqHandler(void *pData, void* param, uint32_t interfaceId);
void fsciMlmeReqHandler(void *pData, void* param, uint32_t interfaceId);

void fsciMcpsCnfHandler(void *pData, void* param, uint32_t interfaceId);
void fsciMlmeCnfHandler(void *pData, void* param, uint32_t interfaceId);

void MlmeSapMonitor(void *pData, void* param, uint32_t interfaceId);
void McpsSapMonitor(void *pData, void* param, uint32_t interfaceId);

#endif /* _FSCI_MAC_COMMANDS_H_ */