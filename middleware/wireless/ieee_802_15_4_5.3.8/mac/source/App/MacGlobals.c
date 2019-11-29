/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file contains various global variables definitions needed by the 802.15.4 MAC
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"

#include "MacGlobals.h"
#include "MacInterface.h"

#if !gFsciHost_802_15_4_c
#include "PhyInterface.h"
#include "MpmInterface.h"
#endif

#include "ModuleInfo.h"
#include "Panic.h"
#if gFsciIncluded_c && gFsciHost_802_15_4_c
  #include "FsciMacCommands.h"
#endif

/************************************************************************************
*************************************************************************************
* Public memory definitions
*************************************************************************************
************************************************************************************/
extern void Mac_Task( osaTaskParam_t argument );
extern void Mac_InitializeData( void );

#if !gFsciHost_802_15_4_c
/* The following definitions are required by the VERSION_TAGS. DO NOT MODIFY or REMOVE */
extern const moduleInfo_t MAC_VERSION;
#if defined ( __IAR_SYSTEMS_ICC__ )
#pragma required=MAC_VERSION /* force the linker to keep the symbol in the current compilation unit */
uint8_t mac_dummy; /* symbol suppressed by the linker as it is unused in the compilation unit, but necessary because 
                             to avoid warnings related to #pragma required */
#elif defined(__GNUC__)
static const moduleInfo_t *const dummy __attribute__((__used__)) = &MAC_VERSION;
#endif /* __IAR_SYSTEMS_ICC__ */
#endif /* !gFsciHost_802_15_4_c */

#if gFsciIncluded_c && gFsciHost_802_15_4_c
extern FsciHostMacInterface_t fsciHostMacInterfaces[gMacInstancesCnt_c];
extern uint8_t                fsciToMacBinding[gMacInstancesCnt_c];

extern resultType_t Dumy_MCPS_NWK_SapHandler (mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);
extern resultType_t Dumy_MLME_NWK_SapHandler (nwkMessage_t* pMsg, instanceId_t instanceId);
#endif

/* MAC RTOS objects */
osaTaskId_t gMacTaskHandler;

OSA_TASK_DEFINE( Mac_Task, gMacTaskPriority_c, 1, gMacTaskStackSize_c, FALSE );

/* The maximum number of Indirect transactions */
uint8_t gMacMaxIndirectTransactions;

/* The maximum number of pending MAC requests */
uint8_t gMacMaxPendingReq = gMacInpuQueueLimit_d;

/* The ID of the MEM pools used by the MAC */
const uint8_t gMacPoolId = gMacPoolId_d;

#if gCslSupport_d
/* The maximum number of CSL transactions */    
const uint8_t gMacMaxCslTransactions = gMacCslMaxSequencesCnt_c;

/* MAC CSL table used for sync */
#if gMacCslTableSize_c > 0
    macCslEntry_t macCslTable[gMacCslTableSize_c];
    const uint8_t gMacCslTableSize = gMacCslTableSize_c;
    #endif
#endif

#if gRitSupport_d
/* The maximum number of RIT transactions */    
const uint8_t gMacMaxRitTransactions = gMacRitMaxSequencesCnt_c;

/* MAC RIT table used for sync */
#if gMacRitTableSize_c > 0
    macRitEntry_t macRitTable[gMacRitTableSize_c];
    const uint8_t gMacRitTableSize = gMacRitTableSize_c;
    #endif
#endif

#if gTschSupport_d    
/* MAC TSCH tables */
#if gMacHoppingSequenceTableEntries_c > 0
    uint8_t gMacHoppingSequenceTable[gMacHoppingSequenceTableEntries_c];
    const uint16_t gMacHoppingSequenceTableSize = gMacHoppingSequenceTableEntries_c;
    #endif
#if gMacSlotframeTableEntries_c > 0
    macSlotframe_t gMacSlotframeTable[gMacSlotframeTableEntries_c];
    const uint8_t gMacSlotframeTableSize = gMacSlotframeTableEntries_c;
    #endif    
#if gMacLinkTableEntries_c > 0
    macLink_t gMacLinkTable[gMacLinkTableEntries_c];
    const uint8_t gMacLinkTableSize = gMacLinkTableEntries_c;
    #endif
#if gMacTschNeighborTableEntries_c > 0
    macTschNeighbor_t gMacTschNeighborTable[gMacTschNeighborTableEntries_c];
    const uint8_t gMacTschNeighborTableSize = gMacTschNeighborTableEntries_c;
    #endif
/* MAC TSCH PAN join dynamically allocated buffer size */    
const uint8_t gMacTschMaxPanCoordSync = gMacTschMaxPanCoordSync_c;    
/* MAC TSCH max sequences in Tx queue */
const uint8_t gMacTschMaxTransactions = gMacTschMaxTransactions_c;
#endif

/* The maximum number MAC instances */
const uint8_t gMacNoOfInstances = gMacInstancesCnt_c;

/* Storage for MAC's internal data */
uint32_t gMacData[gMacInstancesCnt_c][(gMacInternalDataSize_c + 3)/sizeof(uint32_t)];

/* MAC internal data size. Used for sanity check */
extern const uint16_t gMacLocalDataSize;

#if gMacSecurityEnable_d

const bool_t gMacWipeSecurityTables = TRUE;

/* Allocate memory for the MAC KeyTable and sub-tables */
#if gNumKeyTableEntries_c > 0
    keyDescriptor_t            gPIBKeyTable[gMacInstancesCnt_c][gNumKeyTableEntries_c];
    uint8_t                    gNumKeyTableEntries = gNumKeyTableEntries_c;
    
    #if gNumKeyIdLookupListEntries_c > 0
    keyIdLookupDescriptor_t    gPIBKeyIdLookupDescriptorTable[gMacInstancesCnt_c][gNumKeyIdLookupListEntries_c * gNumKeyTableEntries_c];
    uint8_t                    gNumKeyIdLookupListEntries = gNumKeyIdLookupListEntries_c;
    #endif

#ifndef gMAC2011_d    
    #if gNumKeyDeviceListEntries_c > 0
    keyDeviceDescriptor_t      gPIBKeyDeviceDescriptorTable[gMacInstancesCnt_c][gNumKeyDeviceListEntries_c * gNumKeyTableEntries_c];
    uint8_t                    gNumKeyDeviceListEntries = gNumKeyDeviceListEntries_c;
    #endif
#else /* gMAC2011_d */    
    #if gNumDeviceDescriptorHandleListEntries_c > 0
    uint8_t                    gPIBDeviceDescriptorHandleTable[gMacInstancesCnt_c][gNumDeviceDescriptorHandleListEntries_c * gNumKeyTableEntries_c];
    uint8_t                    gNumDeviceDescriptorHandleListEntries = gNumDeviceDescriptorHandleListEntries_c;
    #endif
#endif /* gMAC2011_d */
    
    #if gNumKeyUsageListEntries_c > 0
    keyUsageDescriptor_t       gPIBKeyUsageDescriptorTable[gMacInstancesCnt_c][gNumKeyUsageListEntries_c * gNumKeyTableEntries_c];
    uint8_t                    gNumKeyUsageListEntries = gNumKeyUsageListEntries_c;
    #endif
#endif

/* Allocate memory for the MAC DeviceTable */
#if gNumDeviceTableEntries_c > 0
    deviceAddrDescriptor_t     gPIBDeviceAddrTable[gMacInstancesCnt_c][gNumDeviceAddrTableEntries_c];
    uint8_t                    gNumDeviceAddrTableEntries = gNumDeviceAddrTableEntries_c;

    deviceDescriptor_t         gPIBDeviceTable[gMacInstancesCnt_c][gNumDeviceTableEntries_c];
    uint8_t                    gNumDeviceTableEntries = gNumDeviceTableEntries_c;
#endif

/* Allocate memory for the MAC SecurityLevelTable */
#if gNumSecurityLevelTableEntries_c > 0
    securityLevelDescriptor_t  gPIBSecurityLevelTable[gMacInstancesCnt_c][gNumSecurityLevelTableEntries_c];
    uint8_t                    gNumSecurityLevelTableEntries = gNumSecurityLevelTableEntries_c;
#endif

#endif /*gMacSecurityEnable_d*/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This function will create the MAC task(s)
*
********************************************************************************** */
void MAC_Init( void )
{
#if gFsciIncluded_c && gFsciHost_802_15_4_c
    for( uint32_t i=0; i<gMacInstancesCnt_c; i++)
    {
        fsciHostMacInterfaces[i].upperLayerId = gInvalidInstanceId_c;
        fsciHostMacInterfaces[i].pfMCPS_NWK_SapHandler = Dumy_MCPS_NWK_SapHandler;
        fsciHostMacInterfaces[i].pfMLME_NWK_SapHandler = Dumy_MLME_NWK_SapHandler;
        fsciHostMacInterfaces[i].fsciInterfaceId = mFsciInvalidInterface_c;
        fsciToMacBinding[i] = mFsciInvalidInterface_c;
    }
   
    /* Nothing else to do if this is a FSCI host */
    return;
#else

    gMacMaxIndirectTransactions =
#if gMpmIncluded_d
    gPhyIndirectQueueSize_c/gMpmPhyPanRegSets_c;
#else
    gPhyIndirectQueueSize_c;
#endif
    
    if( gMacLocalDataSize > gMacInternalDataSize_c )
    {
        /* The value of gMacInternalDataSize_c define must be increased */
        panic(0,0,0,0);
    }
    else
    {
        Mac_InitializeData();
        
        /* The instance of the MAC is passed at task creaton */
        gMacTaskHandler = OSA_TaskCreate(OSA_TASK(Mac_Task), NULL);
        if( NULL == gMacTaskHandler )
        {
            panic(0,0,0,0);
        }
    }
#endif
}
