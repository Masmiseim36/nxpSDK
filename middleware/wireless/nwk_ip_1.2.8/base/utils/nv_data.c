/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

   /*!=================================================================================================
\file       nv_data.c
\brief      This is the source file for the  stack non volatile data module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"

#include "app_stack_config.h"

#include "NVM_Interface.h"
#include "nv_data.h"

#include "stack_config.h"
#include "ip.h"
#include "ip_cfg.h"
#include "ip6.h"
#include "network_utils.h"
#include "sixlowpan_cfg.h"
#include "sixlowpan_interface.h"
#include "sixlowpan_tbl.h"
#include "sixlowpan.h"
#include "mac_abs_types.h"

#include "dhcp.h"
#include "dhcp_cfg.h"
#include "dhcp6.h"

#include "dhcp_client.h"
#include "dhcp_server.h"

#include "dhcp6_client.h"
#include "dhcp6_server.h"

#include "thread_meshcop.h"

#include "mpl_cfg.h"
#include "mpl.h"

#include "nd.h"
#include "nd_cfg.h"

#include "trickle.h"
#include "mac_filtering.h"

#include "event_manager.h"
#include "thread_attributes.h"
#include "thread_cfg.h"
#include "thread_mgmt.h"

#include "MemManager.h"
#include "FunctionLib.h"
#include "nvm_adapter.h"
#include "mle_security.h"
#include "debug_log.h"


/* gSIXLOWPAN_DATA_SET_FOR_NVM */
extern ndContextEntry_t            *aContextTable[];

#if DHCP6_SERVER_ENABLED
/* gDhcp6Server_DATA_SET_FOR_NVM */
extern dhcp6ServerBindingTbl_t     *mDhcp6ServerBindingTbl[];
extern dhcp6ServerCfg_t            *aDhcp6ServerCfgStruct[];
#endif

#if DHCP6_CLIENT_ENABLED
/* gDhcp6Client_DATA_SET_FOR_NVM */
extern dhcp6ClientData_t           *aDhcp6ClientParams[];
#endif

#if DHCP4_SERVER_ENABLED && IP_IP4_ENABLE
extern dhcpServerData_t            *aServerBindingTbl[];
#endif

#if DHCP4_CLIENT_ENABLED && IP_IP4_ENABLE
/* gDhcp4Client_DATA_SET_FOR_NVM */
extern dhcpClientData_t             *aClientParamsTbl[];
#endif

#if IP_IP4_ENABLE
extern ip4IfAddrData_t              *aGlobalAddrTable4[];
#endif

/* IP_IP6_DATA_SET_FOR_NVM */
extern ip6IfAddrData_t             *aGlobalAddrTable6[];
extern ip6MulticastAddrData_t      *aMulticastAddrTable[];

#if TRICKLE_ENABLED && MPL_ENABLED
/* MPL_DATA_SET_FOR_NVM */
extern mplInstanceSetEntry_t       *mMplInstanceSet[];
extern uint8_t                      mMplSequenceNumber;

#endif

/* IP_IP6_ROUTING_DATA_SET_FOR_NVM */
extern ip6RoutingTblEntry_t        *aIp6RoutingTable[];

/* IP_IP6_FIREWALL_DATA_SET_FOR_NVM*/
extern ip6FirewallTblEntry_t       *aIp6FirewallTbl[];

/* MAC_FILTERING_DATA_SET_FOR_NVM */
extern macFilteringNeighborData_t*  gaMacFilteringTable[];
extern macFilteringDefault_t mbDefaultSetting;

/* ND_DATA_SET_FOR_NVM */
extern ndCfg_t* aNdCfg[];
extern ndPrefixEntry_t* aPrefixList[];

/* THREAD_LEADER_ID_ASIGN_SET_FOR_NVM */
extern thrIdAssignSet_t* gpaThreadIdAssignSet[][(THR_MAX_ROUTER_ID + 1)];

/* THREAD_NEIGHBORS_DATA_SET_FOR_NVM */
extern thrNeighbor_t gpaThrNeighbors[];

/* THREAD_CHILD_ADDR_REG_TABLE_SET_FOR_NVM */
extern thrChildAddrRegEntry_t gaThrAddrRegTbl[];

/* THREAD_CHILD_VERS_TABLE_SET_FOR_NVM */
extern childVersNbSet_t gaChildVersNbTbl[];

/* Thread Slaac temporary address table - stored in NVM (NotMirroredInRam) */
extern ipAddr_t *gpaThrSlaacTempAddrTbl[];

/* THREAD_ROUTER_PROMOTION_JITTER */
extern uint32_t gThrRouterSelJitterSec;
#if IP_IP6_MCAST_FW_ENABLE
/* Multicast forwarding table */
extern ip6McastFwTblEntry_t aIp6McastFwTable[];
#endif

/*==================================================================================================
Private macros
==================================================================================================*/
#ifndef gMGMT_DIAGNVMEnabled_d
  #define gMGMT_DIAGNVMEnabled_d 0
#endif

#define gMaxNVDataTableEntries_c gNvTableEntriesCountMax_c

/* for diagnostic nvm module */
#ifndef gDiagnosticTlv_FslMac6LowPanNvmCount_c
#define gDiagnosticTlv_FslMac6LowPanNvmCount_c           0xA0    /* MAC, 6lowpan (Mac Filtering, 6LowPan) NVM data save count */
#endif
#ifndef gDiagnosticTlv_FslNetworkNvmCount_c
#define gDiagnosticTlv_FslNetworkNvmCount_c              0xA1    /* Network (IP, DHCP, Leader Id Assignment, Trickle, MPL, ND) NVM data save count */
#endif
#ifndef gDiagnosticTlv_FslSecurityNvmCount_c
#define gDiagnosticTlv_FslSecurityNvmCount_c             0xA2    /* Security NVM data save count */
#endif
#ifndef gDiagnosticTlv_FslFunctionalNvmCount_c
#define gDiagnosticTlv_FslFunctionalNvmCount_c           0xA3    /* attributes, events NVM data save count */
#endif

/*==================================================================================================
Private type definitions
==================================================================================================*/
 /* network diagnostic mac/6lowpan NVM count structure */
typedef struct mgmtDiagMacSlwPanNvmSaveCount_tag
{
  uint16_t contextTblCount;
  uint16_t macFilteringCount;
}mgmtDiagMacSlwPanNvmSaveCount_t;

/* network diagnostic network NVM  count structure */
typedef struct mgmtDiagNwkNvmSaveCount_tag
{
  /* dhcp6*/
  uint16_t dhcp6ClientParamCount;
  uint16_t dhcp6ServerBindingCount;
  uint16_t dhcp6ServerCfgCount;

#if DHCP4_CLIENT_ENABLED
  /* dhcp4*/
  uint16_t dhcp4ClientParamCount;
#endif

  /* ip6 */
  uint16_t interfaceTableCount;
  uint16_t globalAddr6Count;
  uint16_t multicastAddrCount;
  uint16_t ip6RoutingTblCount;
  /* ip4 */
  uint16_t globalAddr4Count;

#if TRICKLE_ENABLED && MPL_ENABLED
  /* mpl */
  uint16_t mplInstCount;
#endif

#if ND_ENABLED
  /* ND */
  uint16_t ndCfgDataCount;
  uint16_t ndPrefixListCount;
#endif

  /* thr assign Id */
  uint16_t thrIdAssignTblCount;
}mgmtDiagNwkNvmSaveCount_t;

/* network diagnostic security NVM  count structure */
typedef struct mgmtDiagSecurityNvmSaveCount_tag
{
  uint16_t mleSecInfoCount;
  uint16_t mleActiveKeyIndexCount;
  uint16_t macOutgoingSecFrameCCount;
}mgmtDiagSecurityNvmSaveCount_t;

/* network diagnostic functional NVM  count structure */
typedef struct mgmtDiagFunctionalNvmSaveCount_tag
{
  uint16_t thrAttrCount;
  uint16_t thrStringAttrCount;
  uint16_t brPrefixSetCount;
  uint16_t eventTlbCount;
}mgmtDiagFunctionalNvmSaveCount_t;
/*==================================================================================================
Private functions
==================================================================================================*/

/* None */

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

#if gNvStorageIncluded_d
/*
 * Name: NvDataTable
 * Description: NVM data table. Contains entries of datasets.
 *              Defined by application.
 */

    NVM_RegisterDataSet(aContextTable, SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE, sizeof(ndContextEntry_t), nvmId_ContextTable_c, gNVM_NotMirroredInRamAutoRestore_c);

#if DHCP4_SERVER_ENABLED && IP_IP4_ENABLE
    NVM_RegisterDataSet(aServerBindingTbl, DHCP4_SERVER_MAX_CLIENTS, sizeof(dhcpServerData_t), nvmId_mDhcp4ServerBindingTbl_c, gNVM_NotMirroredInRamAutoRestore_c);
#endif

#if (!THREAD_ED_CONFIG && DHCP6_SERVER_ENABLED)
    NVM_RegisterDataSet(mDhcp6ServerBindingTbl, DHCP6_SERVER_MAX_CLIENTS, sizeof(dhcp6ServerBindingTbl_t), nvmId_mDhcp6ServerBindingTbl_c, gNVM_NotMirroredInRamAutoRestore_c);
    NVM_RegisterDataSet(aDhcp6ServerCfgStruct, DHCP6_SERVER_MAX_INSTANCES, sizeof(dhcp6ServerCfg_t), nvmId_Dhcp6ServerCfg_c, gNVM_NotMirroredInRamAutoRestore_c);
#endif

#if DHCP6_CLIENT_ENABLED
    NVM_RegisterDataSet(aDhcp6ClientParams, DHCP6_CLIENT_MAX_INSTANCES, sizeof(dhcp6ClientData_t), nvmId_Dhcp6ClientParams_c, gNVM_NotMirroredInRamAutoRestore_c);
#endif

#if DHCP4_CLIENT_ENABLED && IP_IP4_ENABLE
    NVM_RegisterDataSet(aClientParamsTbl, IP_IF_NB, sizeof(dhcpClientData_t), nvmId_Dhcp4ClientParams_c, gNVM_NotMirroredInRam_c);
#endif

#if IP_IP4_ENABLE
    NVM_RegisterDataSet(aGlobalAddrTable4, IP_IF_IP4_ADDR_NB, sizeof(ip4IfAddrData_t), nvmId_GlobalAddrTable4_c, gNVM_NotMirroredInRam_c);
#endif

    NVM_RegisterDataSet(aGlobalAddrTable6, IP_IF_IP6_ADDR_NB+1, sizeof(ip6IfAddrData_t), nvmId_GlobalAddrTable6_c, gNVM_NotMirroredInRamAutoRestore_c);
    NVM_RegisterDataSet(aMulticastAddrTable, IP_IF_IP6_MULTICAST_ADDR_NB, sizeof(ip6MulticastAddrData_t), nvmId_MulticastAddrTable_c, gNVM_NotMirroredInRamAutoRestore_c);

#if IP_IP6_FIREWALL_ENABLE
    NVM_RegisterDataSet(aIp6FirewallTbl, IP_IP6_FIREWALL_TBL_SIZE, sizeof(ip6FirewallTblEntry_t), nvmId_Ip6FirewallTbl_c, gNVM_NotMirroredInRam_c);
#endif

#if IP_IP6_ROUTING_ENABLE
    NVM_RegisterDataSet(aIp6RoutingTable, IP_IP6_ROUTING_TBL_SIZE+1, sizeof(ip6RoutingTblEntry_t ), nvmId_ip6RoutingTblEntry_c, gNVM_NotMirroredInRamAutoRestore_c);
#endif

#if MPL_ENABLED
    NVM_RegisterDataSet(mMplInstanceSet, MPL_INSTANCE_SET_SIZE, sizeof(mplInstanceSetEntry_t), nvmId_mMplInstanceSet_c, gNVM_NotMirroredInRam_c);
    NVM_RegisterDataSet(&mMplSequenceNumber, 1, sizeof(uint8_t), nvmId_mMplSequenceNumber, gNVM_MirroredInRam_c);
#endif

#if MAC_FILTERING_ENABLED
    NVM_RegisterDataSet(gaMacFilteringTable, MAC_FILTERING_TABLE_SIZE, sizeof(macFilteringNeighborData_t), nvmId_macFilteringTable_c, gNVM_NotMirroredInRamAutoRestore_c);
    NVM_RegisterDataSet(&mbDefaultSetting, 1, sizeof(macFilteringDefault_t), nvmId_macFilteringPolicy_c, gNVM_MirroredInRam_c);
#endif

#if ND_ENABLED
    NVM_RegisterDataSet(aNdCfg, IP_IF_NB, sizeof(ndCfg_t), nvmId_ndCfgData_c, gNVM_NotMirroredInRam_c);
    NVM_RegisterDataSet(aPrefixList, ND_PREFIX_LIST_SIZE, sizeof(ndPrefixEntry_t), nvmId_ndPrefixList_c, gNVM_NotMirroredInRam_c);
#endif

    NVM_RegisterDataSet(gpaThrAttr, THR_MAX_INSTANCES, sizeof(thrAttr_t), nvmId_ThreadAttr_c, gNVM_NotMirroredInRamAutoRestore_c);
    NVM_RegisterDataSet(gpaThrStringAttr, THR_MAX_INSTANCES, sizeof(thrStringAttr_t), nvmId_ThreadStringAttr_c, gNVM_NotMirroredInRamAutoRestore_c);
    NVM_RegisterDataSet(gpaThrActiveAttr, THR_MAX_INSTANCES, sizeof(thrActiveAttr_t), nvmId_ThreadActiveAttr_c, gNVM_NotMirroredInRamAutoRestore_c);
    NVM_RegisterDataSet(gpaThrPendingAttr, THR_MAX_INSTANCES, sizeof(thrPendingAttr_t), nvmId_ThreadPendingAttr_c, gNVM_NotMirroredInRamAutoRestore_c);

#if !THREAD_ED_CONFIG
    NVM_RegisterDataSet(gaServerDataPrefixTbl,    THR_SERVER_DATA_PREFIX_TBL_SIZE,    sizeof(ipAddr_t),             nvmId_ServerDataPrefixAttr_c,       gNVM_MirroredInRam_c);
    NVM_RegisterDataSet(gaServerDataPrefixLenTbl, THR_SERVER_DATA_PREFIX_TBL_SIZE,    sizeof(uint8_t),              nvmId_ServerDataPrefixLenAttr_c,    gNVM_MirroredInRam_c);
    NVM_RegisterDataSet(gaThrServerDataBrSetTbl,  THR_SERVER_DATA_BR_SET_TBL_SIZE,    sizeof(borderRouterSet_t),    nvmId_ServerDataBrSetAttr_c,        gNVM_MirroredInRam_c);
    NVM_RegisterDataSet(gaServerDataExtRouteTbl,  THR_SERVER_DATA_HAS_ROUTE_TBL_SIZE, sizeof(externalRouteSet_t),   nvmId_ServerDataExtRouteSetAttr_c,  gNVM_MirroredInRam_c);
    NVM_RegisterDataSet(gpaLocalServiceSetTbl,    THR_LOCAL_SERVICE_SET_TBL_SIZE,     sizeof(thrLocalServiceSet_t), nvmId_ServerDataServiceSetAttr_c,   gNVM_NotMirroredInRamAutoRestore_c);
    NVM_RegisterDataSet(&gThrRouterSelJitterSec,  1,                                  sizeof(uint32_t),             nvmId_ThreadRouterPromotionJitter_c,gNVM_MirroredInRam_c);
#endif

    NVM_RegisterDataSet(&mMleOutgoingSecFrameCounter, 1, sizeof(uint32_t), nvmId_MleOutgoingSecFrameCounter_c, gNVM_MirroredInRam_c);
    NVM_RegisterDataSet(&mMacOutgoingSecFrameCounter,1, sizeof(uint32_t), nvmId_MacOutgoingSecFrameCounter_c, gNVM_MirroredInRam_c);

#if !THREAD_ED_CONFIG
    NVM_RegisterDataSet(gpaThreadIdAssignSet, (THR_MAX_INSTANCES*(THR_MAX_ROUTER_ID + 1)), sizeof(thrIdAssignSet_t), nvmId_ThrIdAssignTbl_c, gNVM_NotMirroredInRamAutoRestore_c);
#endif

    NVM_RegisterDataSet(gpaThrNeighbors, THR_MAX_NEIGHBORS, sizeof(thrNeighbor_t), nvmId_ThrNeighborsTbl_c, gNVM_MirroredInRam_c);

#if !THREAD_ED_CONFIG
    NVM_RegisterDataSet(gaThrAddrRegTbl, THR_MAX_SLEEPY_ED_NEIGHBORS, sizeof(thrChildAddrRegEntry_t), nvmId_ThrChildAddrRegTbl_c, gNVM_MirroredInRam_c);
    NVM_RegisterDataSet(gaChildVersNbTbl, THR_MAX_SLEEPY_ED_NEIGHBORS, sizeof(childVersNbSet_t), nvmId_ThrChildVersionTbl_c, gNVM_MirroredInRam_c);
#endif

#if IP_IP6_MCAST_FW_ENABLE
    NVM_RegisterDataSet(aIp6McastFwTable, IP_IP6_MCASTFW_TBL_SIZE, sizeof(ip6McastFwTblEntry_t), nvmId_ip6McastFwTblEntry_c, gNVM_MirroredInRam_c);
#endif

    NVM_RegisterDataSet(gpaThrSlaacTempAddrTbl, THR_SLAAC_TEMP_ADDR_TABLE_SIZE, sizeof(ipAddr_t), nvmId_ThrSlaacTempAddrTbl_c, gNVM_NotMirroredInRamAutoRestore_c);

#if DEBUG_LOG
dbgNVMSavings_t gDbgNVMSavings[gNvTableEntriesCountMax_c];
const uint16_t gDbgNVMSavingsSize = sizeof(gDbgNVMSavings)/sizeof(dbgNVMSavings_t);
#endif
#endif /* gNvStorageIncluded_d */

#if (gMGMT_DIAGNVMEnabled_d && gNvStorageIncluded_d)

mgmtDiagNwkNvmSaveCount_t gMgmtDiagNwkNvmSaveCount;
mgmtDiagSecurityNvmSaveCount_t gMgmtDiagSecurityNvmSaveCount;
mgmtDiagMacSlwPanNvmSaveCount_t gMgmtDiagMacSlwPanNvmSaveCount;
mgmtDiagFunctionalNvmSaveCount_t gMgmtDiagFunctionaNvmSaveCount;

const mgmtDiagnostic_NvmData_t gaMgmtDiagNvmData[] =
{
   /* mac/ 6lowpan */
   {nvmId_ContextTable_c,               gDiagnosticTlv_FslMac6LowPanNvmCount_c,  &gMgmtDiagMacSlwPanNvmSaveCount.contextTblCount},
   {nvmId_macFilteringTable_c,          gDiagnosticTlv_FslMac6LowPanNvmCount_c,  &gMgmtDiagMacSlwPanNvmSaveCount.macFilteringCount},
   /* network */
   {nvmId_Dhcp6ClientParams_c,          gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.dhcp6ClientParamCount},
   {nvmId_mDhcp6ServerBindingTbl_c,     gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.dhcp6ServerBindingCount},
   {nvmId_Dhcp6ServerCfg_c,             gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.dhcp6ServerCfgCount},
   {nvmId_Dhcp6ServerCfg_c,             gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.dhcp6ServerCfgCount},
#if DHCP4_CLIENT_ENABLED
   {nvmId_Dhcp4ClientParams_c,          gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.dhcp4ClientParamCount},
#endif
   {nvmId_InterfaceTable_c,             gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.interfaceTableCount},
   {nvmId_GlobalAddrTable6_c,           gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.globalAddr6Count},
   {nvmId_MulticastAddrTable_c,         gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.multicastAddrCount},
   {nvmId_ip6RoutingTblEntry_c,         gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.ip6RoutingTblCount},
   {nvmId_GlobalAddrTable4_c,           gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.globalAddr4Count},
#if TRICKLE_ENABLED && MPL_ENABLED
   {nvmId_mMplInstanceSet_c,            gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.mplInstCount},
#endif
#if ND_ENABLED
   {nvmId_ndCfgData_c,                  gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.ndCfgDataCount},
   {nvmId_ndPrefixList_c,               gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.ndPrefixListCount},
#endif
   {nvmId_ThrIdAssignTbl_c,             gDiagnosticTlv_FslNetworkNvmCount_c,     &gMgmtDiagNwkNvmSaveCount.thrIdAssignTblCount},
   /* security */
   {nvmId_MleOutgoingSecFrameCounter_c, gDiagnosticTlv_FslSecurityNvmCount_c,    &gMgmtDiagSecurityNvmSaveCount.mleSecInfoCount},
   {nvmId_MacOutgoingSecFrameCounter_c, gDiagnosticTlv_FslSecurityNvmCount_c,    &gMgmtDiagSecurityNvmSaveCount.macOutgoingSecFrameCCount},
   /* functional */
   {nvmId_ThreadAttr_c,                 gDiagnosticTlv_FslFunctionalNvmCount_c,  &gMgmtDiagFunctionaNvmSaveCount.thrAttrCount},
   {nvmId_EventsTbl_c,                  gDiagnosticTlv_FslFunctionalNvmCount_c,  &gMgmtDiagFunctionaNvmSaveCount.thrStringAttrCount},
   {nvmId_ThreadStringAttr_c,           gDiagnosticTlv_FslFunctionalNvmCount_c,  &gMgmtDiagFunctionaNvmSaveCount.brPrefixSetCount},
};

const uint8_t gMgmtDiagNvmDataSize = NumberOfElements(gaMgmtDiagNvmData);
#else
/*! Nwk Diagnostics NVM Data */
const mgmtDiagnostic_NvmData_t gaMgmtDiagNvmData[]= {0xFF, 0xFF, NULL};
uint8_t const gMgmtDiagNvmDataSize = 0;
#endif /* (gMGMT_DIAGNVMEnabled_d && gNvStorageIncluded_d) */

/*==================================================================================================
Public functions
==================================================================================================*/
#if gNvStorageIncluded_d
#if _DEBUG
/*!*************************************************************************************************
\public
\fn     void NVM_DebugCountNvSavings(uint16_t nvmDataSetId)
\brief  This function is used to send a Network Diagnostic Get Request

\param  [in]    nvmDataSetId - nvm data set id

\return         void
***************************************************************************************************/
void NVM_DebugCountNvSavings(uint16_t nvmDataSetId)
{
    /* get entry */
    for(uint8_t i = 0; i<gMgmtDiagNvmDataSize; i++)
    {
        if(gaMgmtDiagNvmData[i].nvmId == nvmDataSetId)
        {
            uint16_t *pValue = gaMgmtDiagNvmData[i].pData;
            pValue[0]++;
            break;
        }
    }
}
#endif
#endif /*gNvStorageIncluded_d */
