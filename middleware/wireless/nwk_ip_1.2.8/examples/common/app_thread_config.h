/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!
**  @addtogroup TAC
**  @{
*/
#ifndef _APP_THREAD_CONFIG_H
#define _APP_THREAD_CONFIG_H

/*!
\file       app_thread_config.h
\brief      This is a header file for the Thread stack configuration.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "app_stack_config.h"
#include "stack_config.h"

#include "stack_manager_if.h"
#include "network_utils.h"
#include "app_init.h"

#if STACK_THREAD

#include "version.h"
/*==================================================================================================
Public macros
==================================================================================================*/

/*******************************/
/* THREAD NON VOLATILE MEMORY RESTORE */
/*******************************/

/*! Enable restore after reset of Thread Stack persistent data */
#ifndef  THR_NVMRESTORE_ENABLE
    #define THR_NVMRESTORE_ENABLE                       TRUE
#endif

/*******************************/
/* THREAD Device capabilities */
/*******************************/
/*! The device can create a new network */
#ifndef THR_DEFAULT_CAN_CREATE_NEW_NETWORK
    #define THR_DEFAULT_CAN_CREATE_NEW_NETWORK          TRUE
#endif

/*! The device can become an active router. If this capability bit is set FALSE, the device can not promote
 * itself as a router */
#ifndef THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER
    #define THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER        TRUE
#endif

/*! The device is a polling end device (sleepy end device) */
#ifndef THR_DEFAULT_IS_POLLING_END_DEVICE
    #define THR_DEFAULT_IS_POLLING_END_DEVICE           FALSE
#endif

/*! The device is a full end device (can perform address querry but does not act as active router) */
#ifndef THR_DEFAULT_IS_FULL_END_DEVICE
    #define THR_DEFAULT_IS_FULL_END_DEVICE              FALSE
#endif

/*! Enable/disable the thread event monitoring */
#ifndef THR_ENABLE_EVENT_MONITORING
    #define THR_ENABLE_EVENT_MONITORING                 FALSE
#endif

/*! Enable/disable SERIAL_TUN router */
#ifndef SERIAL_TUN_IF
    #define SERIAL_TUN_IF                               FALSE
#endif

/*! Enable/disable UDP echo protocol */
#ifndef UDP_ECHO_PROTOCOL
    #define UDP_ECHO_PROTOCOL                           FALSE
#endif

/*! The device is out-of-band commissioned (the node is pre-configured). This means that the device
 * has all network parameters to directly attach to that network (e.g. master key, PSKc, mesh-local ULA,
 * extended PAN ID, Network name) */
#ifndef THR_DEV_IS_OUT_OF_BAND_CONFIGURED
    #define THR_DEV_IS_OUT_OF_BAND_CONFIGURED           FALSE
#endif

/*! Enable/disable Thread network diagnostics commands */
#ifndef THR_ENABLE_MGMT_DIAGNOSTICS
    #define THR_ENABLE_MGMT_DIAGNOSTICS                 FALSE
#endif

/*! Enable/disable DHCP server */
#ifndef THR_USE_DHCP6_SERVER
    #define THR_USE_DHCP6_SERVER                        FALSE
#endif

/*! Enable/disable DHCP client */
#ifndef THR_USE_DHCP6_CLIENT
    #define THR_USE_DHCP6_CLIENT                        TRUE
#endif

/*! The Network capabilities */
#ifndef THR_NWK_CAPABILITIES_BITMASK
    #define THR_NWK_CAPABILITIES_BITMASK (  (THR_DEFAULT_CAN_CREATE_NEW_NETWORK << 0) | \
                                            (THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER << 1) | \
                                            (THR_DEFAULT_IS_POLLING_END_DEVICE << 2) | \
                                            (THR_DEFAULT_IS_FULL_END_DEVICE) << 3 )
#endif

/**********************************/
/* 802.15.4 default configuration */
/**********************************/
/*! The channel mask used when a network scanning is performed (energy scan, active scan or both);
   0x07FFF800 sets all 2.4GHz 16 channels are used (from channel id 11 to 26).
   This channel mask is used during network creation to select the best channel or during network discovery
   to find the available networks on that channels.
   To set the scan mask to a single channel, set the mask to 0x00000001 shifted with the channel ID
   e.g.: to set mask for channel 25, set THR_SCANCHANNEL_MASK to: (0x00000001 << 25) */
#ifndef THR_SCANCHANNEL_MASK
    #define THR_SCANCHANNEL_MASK                        (0x00000001 << 25)
#endif


/*! The scan duration time. This is an exponential scale, as seen in the 802.15.4 specification.
   Range:   1 - 14 */
#ifndef THR_SCAN_DURATION
    #define THR_SCAN_DURATION                           2
#endif

/*! Network creation channel. If different form 0, the router which creates a new
   network by calling THR_NwkCreate will OVERRIDE the SCAN channel and
   only use this channel. Range: 0, 11-26 */
#ifndef THR_NWK_CREATE_CHANNEL
    #define THR_NWK_CREATE_CHANNEL                      0
#endif

/*! The PAN identifier.
   If this value is 0xFFFF a random PAN ID will be generated on network creation */
#ifndef THR_PAN_ID
    #define THR_PAN_ID                                  THR_ALL_FFs16
#endif

/*! The extended Pan ID.
   If this is set to all FFs, a random extended PAN ID will be generated on network creation  */
#ifndef THR_EXTENDED_PAN_ID
    #define THR_EXTENDED_PAN_ID                         {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#endif

/*! Node Addresses - if TRUE, unique device ID is USED, otherwise will have THR_EXT_ADDR */
#ifndef THR_UNIQUE_EUI64_ADDR_ENABLED
    #define THR_UNIQUE_EUI64_ADDR_ENABLED               TRUE
#endif

/*! Default MAC extended address (IEEE address, long address or 64-bit MAC address) */
#ifndef THR_EUI64_ADDR
    #define THR_EUI64_ADDR                              0x146E0A0000000001
#endif

/*! Rx on when Idle */
#if THR_DEFAULT_IS_POLLING_END_DEVICE
    #define RX_ON_IDLE                                  FALSE
#else
    #define RX_ON_IDLE                                  TRUE
#endif

/*! The default value for sleepy end device (SED) polling interval */
#ifndef THR_SED_POLLING_INTERVAL_MS
    #define THR_SED_POLLING_INTERVAL_MS                 3000     /* Milliseconds */
#endif

/*! Sleepy End Device data fast polling interval */
#ifndef THR_FAST_POLLING_INTERVAL_MS
    #define THR_FAST_POLLING_INTERVAL_MS                100      /* Miliseconds */
#endif

/**************************************/
/*  Thread - default configuration:   */
/**************************************/

/*! The default Device Passphrase (PSKd) used in commissioning procedure. The minimum length is 6
    and the maximum length is 32. It has to be encoded in base32-thread format:
    (0-9,A-Y excluding I,O,Q, and Z)
    !!!WARNING!!! USE PRESET VALUE FOR TESTING ONLY!!!
    For production devices, the application MUST use THR_SetAttr to set UNIQUE PSKd:
    THR_SetAttr(0, gNwkAttrId_PSKd_c, 0, sizeof(uniquePSKd), uniquePSKd); */
#ifndef THR_PSK_D
    #define THR_PSK_D                                  {6, "THREAD"}
#endif

/*! If the THR_PSK_C have all bytes zero, the PKSc is computed using the PSKd as
   commissioning credential for PBKDF2() (see "Derivation of PSKc" section) */
#ifndef THR_PSK_C
    #define THR_PSK_C                                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#endif

/*! The default network name */
#ifndef THR_NETWORK_NAME
    #define THR_NETWORK_NAME                           {14, "Kinetis_Thread"}
#endif

/*! The default vendor name */
#ifndef THR_VENDOR_NAME
    #define THR_VENDOR_NAME                             {3,  "NXP"}
#endif

/*! The default Model Name */
#ifndef THR_MODEL_NAME
    #define THR_MODEL_NAME                              {12, "Kinetis_Demo"}
#endif

/*! The default software version */
#ifndef THR_VENDOR_SW_VERSION
    #define THR_VENDOR_SW_VERSION                       {SizeOfString(gNXPVendorSwVersion_c), gNXPVendorSwVersion_c}
#endif

/*! The default thread stack version */
#ifndef THR_VENDOR_STACK_VERSION
    #define THR_VENDOR_STACK_VERSION                    gNXPThreadVendorStackVersion_c
#endif

/*! The default provisioning URL */
#ifndef THR_PROV_URL
    #define THR_PROV_URL                                {14, "nxp.com/thread"}
#endif

/*! The default vendor data */
#ifndef THR_VENDOR_DATA
    #define THR_VENDOR_DATA                             {7,  "Kinetis"}
#endif

/*! The Commissioner ID is the human-readable identifier for the active Commissioner */
#ifndef THR_COMMISSIONER_ID
    #define THR_COMMISSIONER_ID                         {16, "NXP Commissioner"}
#endif

/*! Minimum delay time before a pending dataset can be installed */
#ifndef THR_DELAY_TIMER_DEFAULT
    #define THR_DELAY_TIMER_DEFAULT                     (5 * 60) /* seconds */
#endif

/*! Default master key set if the current device creates a new network
    !!!WARNING!!! USE PRESET VALUE FOR TESTING ONLY!!!
    For Production Devices set to all FFs for the stack to auto-generate random key
    OR set to unique application define key at runtime:
    THR_SetAttr(0, NwkAttrId_NwkMasterKey_c, 0, sizeof(appMasterKey), appMasterKey); */
#ifndef THR_MASTER_KEY
    #define THR_MASTER_KEY                             {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, \
                                                        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}
#endif

/*! Enable to use the default Mesh Local Prefix value specified below */
#ifndef THR_USE_RANDOM_MESH_LOCAL_PREFIX
    #define THR_USE_RANDOM_MESH_LOCAL_PREFIX             1
#endif

/*! Default Mesh Local Prefix - if value is different from 0 the value specified below will be used */
#ifndef THR_DEFAULT_MESH_LOCAL_PREFIX

#if THR_USE_RANDOM_MESH_LOCAL_PREFIX
    #define THR_DEFAULT_MESH_LOCAL_PREFIX             {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, \
                                                        00}
#else
    #define THR_DEFAULT_MESH_LOCAL_PREFIX             {{0xFD, 0x00, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, \
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, \
                                                        64}
#endif
#endif

/*! The network key sequence number */
#ifndef THR_KEY_SEQ_NUMBER
    #define THR_KEY_SEQ_NUMBER                          0
#endif

/*! Key rotation interval in hours. Max 1193 hours. THR_KEY_ROTATION_INTERVAL_HOURS must be higher
    than THR_KEY_SWITCH_GUARD_TIME_HOURS. */
#ifndef THR_KEY_ROTATION_INTERVAL_HOURS
    #define THR_KEY_ROTATION_INTERVAL_HOURS             672
#endif

/*! Key switching guard time to prevent inadvertent key switching.
    Typically set to a value of (THR_KEY_ROTATION_INTERVAL_HOURS * 7)/8. Zero means it is not used */
#ifndef THR_KEY_SWITCH_GUARD_TIME_HOURS
    #define THR_KEY_SWITCH_GUARD_TIME_HOURS             624
#endif

/*! The Timeout period used by the parent to consider a sleepy end device (SED) disconnected
   (in seconds ).
   NOTE: This interval should be more than 3 seconds */
#ifndef THR_SED_TIMEOUT_PERIOD_SEC
    #define THR_SED_TIMEOUT_PERIOD_SEC                 (4 * (THR_SED_POLLING_INTERVAL_MS)/1000 + 3)
#endif

/*! The Timeout period used by the parent to consider an end device
 *  (REED or RxOn ED) disconnected */
#ifndef THR_CHILD_ED_TIMEOUT_PERIOD_SEC
    #define THR_CHILD_ED_TIMEOUT_PERIOD_SEC             240
#endif

/*! The joining LQI threshold used to select a potential parent during joining procedure */
#ifndef THR_SELECT_PARENT_LQI_THRESHOLD
    #define THR_SELECT_PARENT_LQI_THRESHOLD             0
#endif

/*! The energy channel threshold to select the best channel when
    more channels are scan (energy detect scan) to form the network.
    Note that this is used only if the scanChannelMask attribute includes more than one channel */
#ifndef THR_SEL_BEST_CHANNEL_ED_THRESHOLD
    #define THR_SEL_BEST_CHANNEL_ED_THRESHOLD           0x55
#endif

#ifndef THR_DEFAULT_SECURITY_POLICY
    #define THR_DEFAULT_SECURITY_POLICY                (MESHCOP_O_MASK | /* Allow obtaining the Master Key */\
                                                        MESHCOP_N_MASK | /* Allow Native Commissioners */\
                                                        MESHCOP_R_MASK | /* Allow Thread 1.1 routers */\
                                                        MESHCOP_C_MASK | /* Allow External Commissioner */\
                                                        MESHCOP_B_MASK | /* Thread Beacons enabled */\
                                                        0x7)             /* reserved bits */
#endif

/*! The default commissioner active timestamp */
#ifndef THR_DEFAULT_ACTIVE_TIMESTAMP
    #define THR_DEFAULT_ACTIVE_TIMESTAMP                {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

/*! The maximum number of thread network that can be discovered during network create
 *  (to select the best channel and an unique PanID) */
#ifndef THR_MAX_DISCNWK_TO_FORM_NWK
    #define THR_MAX_DISCNWK_TO_FORM_NWK                30
#endif

/*! The default reset MCU timeout in milliseconds */
#ifndef THR_RESET_MCU_TIMEOUT_MS
    #define THR_RESET_MCU_TIMEOUT_MS                   500 /* milliseconds */
#endif

/*! The default device type */
#ifndef THR_DEVICE_TYPE
    #define THR_DEVICE_TYPE                            gThrDevType_ComboNode_c
#endif

/*! On Border Router use the below global /64 on-Mesh Prefix */
#ifndef THR_BR_GLOBAL_ONMESH_PREFIX
    #define THR_BR_GLOBAL_ONMESH_PREFIX               {{0xFD, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3E, 0xAD, \
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, \
                                                        64}
#endif
/*! On Border Router, use default route for the above /64 on-Mesh Prefix */
#ifndef THR_BR_DEFAULT_ROUTE_ON_MESH_PREFIX
    #define THR_BR_DEFAULT_ROUTE_ON_MESH_PREFIX      FALSE
#endif

/*! If the end device had lost its parent, specify how many times to retry the attaching procedure.
 * OxFFFF means that the end device will retry to attach forever
   Ox0000 means no retry (the app will decide what to do) */
#ifndef THR_ED_LOST_PARENT_ATTACH_RETRIES
    #define THR_ED_LOST_PARENT_ATTACH_RETRIES        THR_ALL_FFs16
#endif

/*! When the end device (sleepy or powered ED) loses its parent, the end device
    will attempt to attach after THR_ED_ATTACH_RETRY_INITIAL_TIMEOUT milliseconds.
    If the attachment still fails, for each retry, the timeout to trigger the next retry
    will be doubled, and so on, until it reaches to THR_ED_ATTACH_RETRY_MAX_TIMEOUT */
#ifndef THR_ED_ATTACH_RETRY_INITIAL_TIMEOUT
    #define THR_ED_ATTACH_RETRY_INITIAL_TIMEOUT       2000  /* milliseconds */
#endif

/*!  The maximum retry timeout for the end device attachment */
#ifndef THR_ED_ATTACH_RETRY_MAX_TIMEOUT
    #define THR_ED_ATTACH_RETRY_MAX_TIMEOUT           64000   /* milliseconds */
#endif

/*!  Specify how many times to retry the MAC active scan if no beacon is received */
#ifndef THR_ACTIVE_SCAN_RETRIES
    #define THR_ACTIVE_SCAN_RETRIES                   2
#endif

/*! 0xFFFFFFFF -> randomize partition ID when becoming Leader, otherwise use this value */
#ifndef THR_DEFAULT_PARTITION_ID
    #define THR_DEFAULT_PARTITION_ID                    (THR_ALL_FFs32)
#endif

/*! The default MAC Tx options for Discovery Request */
#ifndef THR_DISCOVERYREQ_MAC_TX_OPTIONS
    #define THR_DISCOVERYREQ_MAC_TX_OPTIONS                    gThrNoSecurityAtMacLevel_c
#endif

/*! The default Discovery method used by APP to join */
#ifndef THR_APP_JOIN_DISCOVERY_METHOD
    #define THR_APP_JOIN_DISCOVERY_METHOD                      gUseThreadDiscovery_c
#endif


/*=================================================================================================
   6LoWPAN Common Configuration
==================================================================================================*/

/*! The default configuration for SLWP fragmentation */
#if (SLWPCFG_FRAGMENTATION_ENABLED)
    #define SLP_FRAGMENTATION_DEFAULT_CONFIGURATION \
                                        .datagramTag = 0,
#else
    #define SLP_FRAGMENTATION_DEFAULT_CONFIGURATION
#endif


/*=================================================================================================
   MPL Common Configuration
==================================================================================================*/

/*! The default configuration for MPL SEED ID */
#if MPL_MULTIPLE_SEED_ID_LENGTH_ENABLED
    #define THR_MPL_CFG_SEED_ID      .seedIdLength = 2
#else
    #define THR_MPL_CFG_SEED_ID
#endif

/*! The default stack MPL configuration for router */
#define THR_STACK_MPL_CFG_ROUTER        .seedLifetime = 2000,           /* ms */ \
                                        .Imin = 64,                 /* ms */ \
                                        .Imax = 64,                 /* ms */ \
                                        .k = 0x00,                   /* infinite */ \
                                        .nbOfTimerExpirations = 2, \
                                        .useFullInterval = TRUE,   /* full Trickle interval */ \
                                        THR_MPL_CFG_SEED_ID

/*! The default stack MPL configuration for end device */
#define THR_STACK_MPL_CFG_ENDDEVICE     .seedLifetime = 2000,           /* ms */ \
                                        .Imin = 64,                 /* ms */ \
                                        .Imax = 64,                 /* ms */ \
                                        .k = 0x00,                   /* infinite */ \
                                        .nbOfTimerExpirations = 0, \
                                        .useFullInterval = TRUE,    /* full Trickle interval */ \
                                        THR_MPL_CFG_SEED_ID

/*================================================================================================*/


/*==================================================================================================
 Private macros
==================================================================================================*/

/*! The default configuration for SLWP mesh */
#if (SLWPCFG_MESH_ENABLED)
    #define THR_SLP_MESH_CONFIGURATION \
                                        .adpMeshEnable = gAdpMeshOnly_c, \
                                        .adpMeshIb.adpMaxHops = SLWPIB_ADP_MAX_HOPS, \
                                        .adpFwdIb.init = NULL, \
                                        .adpFwdIb.isMeshNeeded = adpThreadIsMeshNeeded, \
                                        .adpFwdIb.ucastGetNextHop = NULL, \
                                        .adpFwdIb.mcastForward = NULL, \
                                        .adpRouteTbl = NULL
#else
    #define THR_SLP_MESH_CONFIGURATION
#endif

/*! The default configuration for SWLP information base attributes */
#define THR_ADP_IB                      .adpDeviceType = gAdpDeviceTypeNone_c, \
                                        .adpHcEnable = gAdpHcRFC6282_c, \
                                        .adpIIDType = gAdpIIDWithoutPanId_c, \
                                        .adpActiveKeyIndex = 0, \
                                        .msduHandle = SLWPCFG_UNFRAG_SED_TRACK_NB * SLWPCFG_UNFRAG_SED_TRACK_PKT_NB, \
                                        .sedTrackTblEntries = SLWPCFG_UNFRAG_SED_TRACK_NB, \
                                        .sedTrackTblPktNb = SLWPCFG_UNFRAG_SED_TRACK_PKT_NB, \
                                        .sedIndQueueSize = SLWPCFG_SED_IND_QUEUE_SIZE, \
                                        SLP_FRAGMENTATION_DEFAULT_CONFIGURATION \
                                        THR_SLP_MESH_CONFIGURATION

/*! Thread default attributes */
#define THR_DEFAULT_ATTR                .ieeeAddr = THR_EUI64_ADDR, \
                                        .devRole = gThrDevRole_Disconnected, \
                                        .scanDuration = THR_SCAN_DURATION,\
                                        .rxOnWhenIdle = RX_ON_IDLE, \
                                        .sedPollInterval = THR_SED_POLLING_INTERVAL_MS, \
                                        .uniqueExtAddr = THR_UNIQUE_EUI64_ADDR_ENABLED, \
                                        .shortAddr = THR_ALL_FFs16, \
                                        .thrNwkCapabilitiesBitMap = THR_NWK_CAPABILITIES_BITMASK, \
                                        .nwkKeySeq = THR_KEY_SEQ_NUMBER, \
                                        .deviceType = THR_DEVICE_TYPE, \
                                        .childEDReqFullNwkData = FALSE, \
                                        .sedTimeoutPeriod = THR_SED_TIMEOUT_PERIOD_SEC, \
                                        .edTimeoutPeriod = THR_CHILD_ED_TIMEOUT_PERIOD_SEC, \
                                        .sedFastPollInterval = THR_FAST_POLLING_INTERVAL_MS, \
                                        .devIsCommissioned = THR_DEV_IS_OUT_OF_BAND_CONFIGURED, \
                                        .joinLqiThreshold = THR_SELECT_PARENT_LQI_THRESHOLD, \
                                        .selBestChannelEDThreshold = THR_SEL_BEST_CHANNEL_ED_THRESHOLD, \
                                        .keySwitchGuardTime = THR_KEY_SWITCH_GUARD_TIME_HOURS, \
                                        .parentHoldTime = 90,\
                                        .nvmRestoreAutoStart = TRUE, \
                                        .nvmRestore = THR_NVMRESTORE_ENABLE, \
                                        .nvmRestoreData = FALSE, \
                                        .slaacPolicy = gThrSlaacRandom_c, \
                                        .brGlobalOnMeshPrefix = THR_BR_GLOBAL_ONMESH_PREFIX, \
                                        .brDefaultRoute = THR_BR_DEFAULT_ROUTE_ON_MESH_PREFIX, \
                                        .partitionId = THR_DEFAULT_PARTITION_ID, \
                                        .weighting = 64, \
                                        .joinerUdpPort = 49153, \
                                        .commissionerUdpPort = 19779U, \
                                        .minDelayTimer = THR_DELAY_TIMER_DEFAULT, \
                                        .discoveryReqMacTxOptions = THR_DISCOVERYREQ_MAC_TX_OPTIONS, \
                                        .minDelayTimer = THR_DELAY_TIMER_DEFAULT

/*! Thread string attributes */
#define THR_STRING_ATTR                 .vendorName = THR_VENDOR_NAME, \
                                        .modelName = THR_MODEL_NAME, \
                                        .swVersion = THR_VENDOR_SW_VERSION, \
                                        .pskD = THR_PSK_D, \
                                        .stackVersion = THR_VENDOR_STACK_VERSION, \
                                        .provisioningURL = THR_PROV_URL, \
                                        .vendorData = THR_VENDOR_DATA, \
                                        .commissionerId = THR_COMMISSIONER_ID

/*! Thread Active dataset attributes */
#define THR_DEFAULT_ACTIVE_ATTR         .channel = THR_NWK_CREATE_CHANNEL, \
                                        .scanChannelMask = THR_SCANCHANNEL_MASK, \
                                        .xPanId = THR_EXTENDED_PAN_ID, \
                                        .nwkMasterKey = THR_MASTER_KEY, \
                                        .nwkName = THR_NETWORK_NAME, \
                                        .panId = THR_PAN_ID, \
                                        .nwkKeyRotationInterval = THR_KEY_ROTATION_INTERVAL_HOURS, \
                                        .securityPolicy = THR_DEFAULT_SECURITY_POLICY, \
                                        .timestamp = THR_DEFAULT_ACTIVE_TIMESTAMP,\
                                        .pskC = THR_PSK_C\

/*! Thread Configuration */
#define THR_CONFIGURATION               .stackType = gThread_c, \
                                        .ifUniqueId = gIpIfSlp0_c, \
                                        .deviceRole = gThrDevType_ComboNode_c, \
                                        .pDefaultStackAttr = (void*)&mThrDefaultAttr, \
                                        .pStackStringAttr = (void*)&mThrStringAttr,\
                                        .pDefaultActiveAttr = (void*)&mThrDefaultActiveAttr,\
                                        /* 802.15.4 */ \
                                        .pAdpIb = (adpIb_t *)&mThreadAdpIb, \


/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern CONST stackConfig_t* pStackCfg[];

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

/*================================================================================================*/

#endif /* STACK_THREAD */

#endif  /* _APP_THREAD_CONFIG_H */
/*!
** @}
*/
