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

#ifndef _THREAD_STACK_CFG_H
#define _THREAD_STACK_CFG_H
/*!
\file       thread_cfg.h
\brief      This configuration header file for the Thread stack parameters. It contains default values for
            module configuration defines.
\details    These values are for library configuration only and cannot  be modified by application
*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
/*==================================================================================================
Private macros
==================================================================================================*/

/* THREAD 1.1 Configs */

/*!  The message pool ID used for thread stack */
//#ifndef ThrPoolId_d
//    #define ThrPoolId_d 0
//#endif

/*! The size of the massage queue used by Thread task */
#ifndef THREAD_TASK_MSG_QUEUE_SIZE
    #define THREAD_TASK_MSG_QUEUE_SIZE 30
#endif

/*! The stack size of Thread task */
#ifndef THREAD_TASK_STACK_SIZE
   #define THREAD_TASK_STACK_SIZE 3400U
#endif

/*! The maximum number of Thread Interfaces. MUST not be greater that IP_IF_NB */
#ifndef THR_MAX_INSTANCES
    #define THR_MAX_INSTANCES 1
#endif

/*! Debug flag for auto promote */
#ifndef DEBUG_REED_AUTO_PROMOTE
    #define DEBUG_REED_AUTO_PROMOTE 0
#endif

/*! The size of the Server Data prefix table */
#ifndef THR_SERVER_DATA_PREFIX_TBL_SIZE
    #define THR_SERVER_DATA_PREFIX_TBL_SIZE 5
#endif

/*! The size of the Server Data DNS server IP address table */
#ifndef THR_SERVER_DATA_DNS_SRV_TBL_SIZE
    #define THR_SERVER_DATA_DNS_SRV_TBL_SIZE 1
#endif

/*! The size of Border Route table for local Server Data */
#ifndef THR_SERVER_DATA_BR_SET_TBL_SIZE
    #define THR_SERVER_DATA_BR_SET_TBL_SIZE 5
#endif

/*! The size of Has Route table for local Server Data */
#ifndef THR_SERVER_DATA_HAS_ROUTE_TBL_SIZE
   #define THR_SERVER_DATA_HAS_ROUTE_TBL_SIZE 5
#endif

/*! The size of local BR service set */
#ifndef THR_LOCAL_SERVICE_SET_TBL_SIZE
    #define THR_LOCAL_SERVICE_SET_TBL_SIZE 2
#endif

/*! The size of Nwk Data Service Set table */
#ifndef THR_NWK_DATA_SERVICE_SET_TBL_SIZE
    #define THR_NWK_DATA_SERVICE_SET_TBL_SIZE 3
#endif

/*! The maximum number of Server Sub-TLVs in a Service Set */
#ifndef THR_SERVICE_DATA_MAX_SERVER_SUBTLVS
    #define THR_SERVICE_DATA_MAX_SERVER_SUBTLVS 5
#endif

/*! The size of Thread Slaac temporary address table - stored in NVM (NotMirroredInRam) */
#ifndef THR_SLAAC_TEMP_ADDR_TABLE_SIZE
    #define THR_SLAAC_TEMP_ADDR_TABLE_SIZE 6
#endif
/*! The size of NWK Data prefix table */
#ifndef THR_NWK_DATA_PREFIX_TBL_SIZE
    #define THR_NWK_DATA_PREFIX_TBL_SIZE 15
#endif

/*! The size of NWK Data context table */
#ifndef THR_NWK_DATA_CTX_TBL_SIZE
    #define THR_NWK_DATA_CTX_TBL_SIZE 16
#endif

/*! The size of NWK Data Border Set table */
#ifndef THR_NWK_DATA_BR_SET_TBL_SIZE
    #define THR_NWK_DATA_BR_SET_TBL_SIZE 15
#endif

/*! The size of NWK Data Has Route table */
#ifndef THR_NWK_DATA_HAS_ROUTE_TBL_SIZE
   #define THR_NWK_DATA_HAS_ROUTE_TBL_SIZE 15
#endif

/*! The lifetime for stable NWK Data */
#ifndef THR_NWK_DATA_MIN_STABLE_LIFETIME_SEC
    #define THR_NWK_DATA_MIN_STABLE_LIFETIME_SEC 604800 /* 168 h*/
#endif

/*! The maximum interval between increments of ID_sequence_number by the Leader */
#ifndef THR_LEADER_ID_SEQUENCE_PERIOD_SEC
    #define THR_LEADER_ID_SEQUENCE_PERIOD_SEC 10
#endif

/*! The number of entries in the Address registration table per RFD child */
#ifndef THR_CHILD_ADDR_REG_ENTIRES
    #define THR_CHILD_ADDR_REG_ENTIRES 4
#endif

/*! The number of entries in the Multicast Address registration table per RFD child */
#ifndef THR_CHILD_MCAST_ADDR_REG_ENTIRES
    #define THR_CHILD_MCAST_ADDR_REG_ENTIRES 3
#endif

/*! The max number of neighbors to do a link sync */
#ifndef THR_MAX_LINK_SYNC_NEIGHBORS
    #define THR_MAX_LINK_SYNC_NEIGHBORS     4
#endif

/*! The maximum number of parents selected to attach with */
#ifndef THR_MAX_NWK_ATTACH_PARENT_ENTRIES
    #define THR_MAX_NWK_ATTACH_PARENT_ENTRIES   3
#endif

/*! The minimum jitter time for generating the random period used at re-attaching the device */
#ifndef THR_REATTACH_JITTER_MIN_MS
    #define THR_REATTACH_JITTER_MIN_MS 0
#endif
/*! The maximum jitter time for generating the random period used at re-attaching the device */
#ifndef THR_REATTACH_JITTER_MAX_MS
    #define THR_REATTACH_JITTER_MAX_MS 1000
#endif

/*! The maximum number of seconds for a Router to get disconnected from the Leader if no ID_sequence_number is received from a neighbor.
    If a Router goes for THR_LEADER_TIMEOUT_SEC seconds without receiving a new ID_sequence_number from a neighbor, it MUST consider
    itself disconnected from the Leader and stop using its current Router ID */
#ifndef THR_LEADER_TIMEOUT_SEC
    #define THR_LEADER_TIMEOUT_SEC 120
#endif

/*! The maximum number of allowed Routers in the Thread network. Maximum value can be 32 */
#ifndef THR_MAX_ROUTERS
    #define THR_MAX_ROUTERS              32
#endif

/*! The number of active Routers on the Thread Network below which
    a REED may decide to become a Router */
#ifndef THR_ROUTER_UPGRADE_THRESHOLD
    #define THR_ROUTER_UPGRADE_THRESHOLD 16
#endif

/*! The number of active Routers on the Thread Network above
    which an active Router may decide to become a Child */
#ifndef THR_ROUTER_DOWNGRADE_THRESHOLD
    #define THR_ROUTER_DOWNGRADE_THRESHOLD 23
#endif

/*! The minimum number of neighbors with link quality 2 or better that a
    Router must have to downgrade to a REED. It should be less than 32 */
#ifndef THR_MIN_DOWNGRADE_NEIGHBORS
    #define THR_MIN_DOWNGRADE_NEIGHBORS 7
#endif

/*! The maximum jitter time when soliciting a router ID */
#ifndef THR_ROUTER_SELECTION_JITTER_SEC
    #define THR_ROUTER_SELECTION_JITTER_SEC 120
#endif

/*! The max number of cache entries for address query */
#ifndef THR_MAX_DEV_ADDR_QUERY_CACHE_ENTRIES
    #define THR_MAX_DEV_ADDR_QUERY_CACHE_ENTRIES  5
#endif

/*! The time needed for an address query to complete */
#ifndef THR_ADDRESS_QUERY_TIMEOUT_SEC
    #define THR_ADDRESS_QUERY_TIMEOUT_SEC   3
#endif

/*! The minimum delay between 2 address queries */
#ifndef THR_ADDRESS_QUERY_INITIAL_RETRY_DELAY_SEC
    #define THR_ADDRESS_QUERY_INITIAL_RETRY_DELAY_SEC    15
#endif

/*! The maximum delay betwenn 2 address queries */
#ifndef THR_ADDRESS_QUERY_MAX_RETRY_DELAY_SEC
    #define THR_ADDRESS_QUERY_MAX_RETRY_DELAY_SEC        120//28800
#endif

/*! On power on, during the network start with NVM, a router will perform a Link Sync
 *  after a random period between [THR_POWERON_ROUTER_MIN_JITTER_MS, THR_POWERON_ROUTER_MAX_JITTER_MS] */
#ifndef THR_POWERON_ROUTER_MIN_JITTER_MS
    #define THR_POWERON_ROUTER_MIN_JITTER_MS 5000
#endif

/*! On power on, during the network start with NVM, a router will perform a Link Sync
 *  after a random period between [THR_POWERON_ROUTER_MIN_JITTER_MS, THR_POWERON_ROUTER_MAX_JITTER_MS] */
#ifndef THR_POWERON_ROUTER_MAX_JITTER_MS
    #define THR_POWERON_ROUTER_MAX_JITTER_MS 7000
#endif

/*! On power on, during the network start with NVM, an end device will perform a Child Update
 *  after a random period between [gThrPowerOnRouterMaxJitterMs,
 *                                 gThrPowerOnRouterMaxJitterMs+gThrPowerOnEDMaxJitterMs] */
#ifndef THR_POWERON_ED_MAX_JITTER_MS
    #define THR_POWERON_ED_MAX_JITTER_MS 9000
#endif

/*! The number of seconds a Child waits prior to reattaching in the event its Parent advertises an
 *  infinite cost to the Leader */
#ifndef THR_PARENT_ROUTE_TO_LEADER_TIMEOUT_MS
    #define THR_PARENT_ROUTE_TO_LEADER_TIMEOUT_MS  20000
#endif

/*! The default periodic interval for REED or End DeviceRxOn to send ChildUpdateRequest.
 * These values should be less than THR_CHILD_ED_TIMEOUT_PERIOD_SEC */
#ifndef THR_CHILD_ED_KEEP_ALIVE_INTERVAL_MIN_MS
    #define THR_CHILD_ED_KEEP_ALIVE_INTERVAL_MIN_MS   200000
#endif

/*! The default periodic interval for REED or End DeviceRxOn to send ChildUpdateRequest.
 * These values should be less than THR_CHILD_ED_TIMEOUT_PERIOD_SEC */
#ifndef THR_CHILD_ED_KEEP_ALIVE_INTERVAL_MAX_MS
    #define THR_CHILD_ED_KEEP_ALIVE_INTERVAL_MAX_MS   230000
#endif

/*! Network Data Context ID reuse delay */
#ifndef THR_CONTEXT_REUSE_DELAY_SEC
    #define THR_CONTEXT_REUSE_DELAY_SEC 60
#endif

/*! Maximum number of address queries that can be performed at the same time */
#ifndef THR_ADDR_QUERY_LIST_SIZE
    #define THR_ADDR_QUERY_LIST_SIZE    10
#endif

/*! The extended address used for discovery */
#define THR_DISCOVERY_EXT_ADDR   {0x35, 0x06, 0xfe, 0xb8, 0x23, 0xd4, 0x87, 0x12}

/*! The key used for discovery */
#define THR_DISCOVERY_KEY        {0x78, 0x58, 0x16, 0x86, 0xfd, 0xb4, 0x58, 0x0f, \
                                  0xb0, 0x92, 0x54, 0x6a, 0xec, 0xbd, 0x15, 0x66}

/*! The frame counter used for discovery */
#define THR_DISCOVERY_FRAME_COUNTER   0

/*! Time an originator of a Discovery Request should wait
 * for incoming Discovery Responses on a channel */
#define THR_DISCOVERY_TIME                  (500)

/*! Maximum jitter time used to delay generation of Discovery Responses */
#define THR_DISCOVERY_MAX_JITTER            (250)

#endif  /* THREAD_STACK_CFG_H */
