/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     


#ifndef _APP_ZPS_CFG_H
#define _APP_ZPS_CFG_H

/*!
\file       app_zps_cfg.h
\brief      Application Zigbee Protocol Stack Configuration File header
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define ZPS_NWK_OPT_ALL
#define ZPS_ROUTER
#define ZPS_NODE_DIMMABLELIGHT

/*****************************************************************************/
/* ZPS ZDO */
/*****************************************************************************/

#define ZPS_ZDO_DEVICE_TYPE                              ZPS_ZDO_DEVICE_ROUTER

/*! Configure ZDO Servers */
/*! Default Server - Mandatory. Replies to any unimplemented server requests.*/
#define ZPS_ZDO_ENABLE_ZDO_DEFAULT_SERVER                TRUE

/*! ZDO Client - Mandatory. Processes ZDO client messages. */ 
#define ZPS_ZDO_ENABLE_ZDO_ZDO_CLIENT                    TRUE

/*! Device Annce Server - Mandatory. Processes device announcements. */
#define ZPS_ZDO_ENABLE_ZDO_DEVICE_ANNCE_SERVER           TRUE  

/*! Active Ep Server - Mandatory. Processes active endpoint requests */ 
#define ZPS_ZDO_ENABLE_ZDO_ACTIVE_EP_SERVER              TRUE

/*! Nwk Addr Server - Mandatory. Processes network address discovery requests. */
#define ZPS_ZDO_ENABLE_ZDO_NWK_ADDR_SERVER               TRUE

/*! IEEE Address Server - Mandatory. Processes IEEE address discovery requests.*/
#define ZPS_ZDO_ENABLE_ZDO_IEEE_ADDR_SERVER              TRUE

/*! System Server Discovery Server - Mandatory. Processes system server discovery requests. */
#define ZPS_ZDO_ENABLE_ZDO_SYSTEM_SERVER_DISCOVERY_SERVER TRUE

/*! Node Descriptor Server - Mandatory. Processes Node descriptor requests. */
#define ZPS_ZDO_ENABLE_ZDO_NODE_DESC_SERVER              TRUE

/*! Power Descriptor Server - Mandatory. Processes Node Power descriptor requests*/
#define ZPS_ZDO_ENABLE_ZDO_POWER_DESC_SERVER             TRUE

/*! Match Descriptor Server - Mandatory. Processes Match descriptor requests.*/ 
#define ZPS_ZDO_ENABLE_ZDO_MATCH_DESC_SERVER             TRUE

/*! Simple Descriptor Server - Mandatory. Processes simple descriptor requests.*/
#define ZPS_ZDO_ENABLE_ZDO_SIMPLE_DESC_SERVER            TRUE

/*! Mgmt Lqi Server - Mandatory. Processes management LQI requests. */
#define ZPS_ZDO_ENABLE_ZDO_MGMT_LQI_SERVER               TRUE

/*! Mgmt Leave Server - Mandatory. Processes management leave requests. */
#define ZPS_ZDO_ENABLE_MGMT_LEAVE_SERVER                 TRUE

/*! Mgmt NWK Update Server - Mandatory. Processes management network update requests.*/
#define ZPS_ZDO_ENABLE_ZDO_MGMT_NWK_UPDATE_SERVER        TRUE

/*! Bind Unbind Server - Mandatory. Processes both bind and unbind requests. */
#define ZPS_ZDO_ENABLE_ZDO_BIND_UNBIND_SERVER            TRUE

/*! Extended Active Ep Server - Mandatory. Processes extended active endpoint discovery requests.*/
#define ZPS_ZDO_ENABLE_ZDO_EXTENDED_ACTIVE_EP_SERVER     FALSE

/*! Extended Simple Descriptor Server - Mandatory. Processes extended Simple 
    descriptor discovery requests. */
#define ZPS_ZDO_ENABLE_ZDO_EXTENDED_SIMPLE_DESC_SERVER   FALSE

/*! Bind request server */
#define ZPS_ZDO_ENABLE_ZDO_BIND_REQUEST_SERVER           TRUE

/*! Mgmt Bind Server */
#define ZPS_ZDO_ENABLE_ZDO_MGMT_BIND_SERVER              TRUE

/*! Permit Joining Server - Mandatory. Processes 'permit joining' requests.*/
#define ZPS_ZDO_ENABLE_ZDO_PERMIT_JOINING_SERVER         TRUE

/*! Mgmt Rtg Server - Mandatory. Processes management routing requests.*/
#define ZPS_ZDO_ENABLE_ZDO_MGMT_RTG_SERVER               TRUE

/*! PARENT ANNCE SERVER */
#define ZPS_ZDO_ENABLE_ZDO_PARENT_ANNCE_SERVER           TRUE

/*! END DEVICE BIND SERVER */
#define ZPS_ZDO_ENABLE_ZDO_END_DEVICE_BIND_SERVER        FALSE


/*! ZDO SERVERS CONTEXT SIZES */

#define ZPS_DEVICE_ANNCE_SERVER_CONTEXT_SIZE             4

#define ZPS_BIND_REQUEST_SERVER_ACKS_DCFM_CONTEXT_SIZE   3
#define ZPS_BIND_REQUEST_SERVER_TIME_INTERVAL            1

/*! Specify a type of security */
/*! ZPS_ZDO_NO_NETWORK_KEY/ZPS_ZDO_PRECONFIGURED_LINK_KEY/
    ZPS_ZDO_DISTRIBUTED_LINK_KEY/ZPS_ZDO_PRCONFIGURED_INSTALLATION_CODE */
#define ZPS_ZDO_NWK_KEY_STATE    ZPS_ZDO_NO_NETWORK_KEY
/*****************************************************************************/
/* ZPS APS AIB Parameters  DEFAULT VALUES */
/*****************************************************************************/

/*! APS TRUST CENTER ADDRESS then node_desc_req/resp is check for r21 Trust center if is not 0xFFFFFFFFFFFFFFFFULL */
#define ZPS_APS_AIB_INIT_TRUST_CENTER_ADDRESS            0        

/*! NWK EXTENDED PANID (EPID) that the device will use.*/
#define ZPS_APS_AIB_INIT_USE_EXTENDED_PANID              0x0000000000000000ULL

/*! CHANNEL MASK */
#define ZPS_APS_AIB_INIT_CHANNEL_MASK                    0x07fff800UL

/*! On start-up the node should assume
    the Co-ordinator role within the network. */
#define ZPS_APS_AIB_INIT_DESIGNATED_COORDINATOR          FALSE

/*! APS Use Insecure Join */
/*! If true, a join using the MAC layer association procedure is performed
    when a secure rejoin fails. */
#define ZPS_APS_AIB_INIT_USE_INSECURE_JOIN               FALSE

#define ZPS_APS_AIB_INIT_DECRYPT_INSTALL_CODE            FALSE

#define ZPS_APS_AIB_INIT_KEY_TYPE                        0

/* volatile */ /* not used */

/*! Multicast non-member radius size. */
/*! Defines the number of hops away from the core multi-cast members that a
    multi-cast transmission can be received.*/
#define ZPS_APS_AIB_INIT_NON_MEMBER_RADIUS               0x02

/*! Number of milliseconds between APS data frames */
#define ZPS_APS_AIB_INIT_INTER_FRAME_DELAY               0x0a

#define ZPS_APS_AIB_INIT_LAST_CHANNEL_ENERGY             0
 
#define ZPS_APS_AIB_INIT_LAST_CHANNEL_FAILURE_RATE       0

#define ZPS_APS_AIB_INIT_CHANNEL_TIMER                   0

/*! APS fragmented data window size */
/*! how many fragments are sent before an acknowledgement is expected */
#define ZPS_APS_AIB_INIT_MAX_WINDOW_SIZE                 0x08

#define ZPS_APS_AIB_INIT_PARENT_ANNOUNCE_PENDING         FALSE

#define ZPS_APS_AIB_INIT_USE_INSTALL_CODE                FALSE

/*! APS Security Timeout Period*/
/*! Authentication timeout period in milliseconds for nodes joining the network.*/
#define ZPS_APS_AIB_INIT_SECURITY_TIMEOUT_PERIOD         0x0bb8 
/*****************************************************************************/


/*****************************************************************************/
/* ZPS Fragmentation Configuration Parameters */
/*****************************************************************************/

/*! Maximum Number of Received Simultaneous Fragmented Messages
    - zero will disable the rx fragmentation feature */
#define ZPS_FRAGMENTATION_MAX_NUM_SIMUL_RX    0 


/*! Maximum Number of Transmitted Simultaneous Fragmented Messages 
    - zero will disable the tx fragmentation feature */
#define ZPS_FRAGMENTATION_MAX_NUM_SIMUL_TX    0

/*****************************************************************************/
/* ZPS AF Layer Configuration Parameters */
/*****************************************************************************/

#define AF_SIMPLE_DESCRIPTOR_TABLE_SIZE    2

#define AF_ENABLE_USER_DESCRIPTOR          FALSE

/*****************************************************************************/
/* ZPS APS Layer Configuration Parameters */
/*****************************************************************************/
/*! APS Duplicate Table Size */
/*! The size of the APS layer duplicate rejection table. 
    This removes duplicated APS packets. */
#define APS_DUPLICATE_TABLE_SIZE          3

/*! The maximum number of simultaneous APSDE 
   data requests without APS acknowledgements. */
#define APSME_SIMULTANEOUS_COMMANDS       4

/*! APSDE sync msg pool size*/
#define APS_SYNC_MSG_POOL_SIZE            3

/*! APSDE dcfm record pool size*/
#define APS_DCFM_RECORD_POOL_SIZE         5

/*! The polling period, in milliseconds, of a 
    sleeping End Device collecting data of any kind */
#define APS_POLL_INTERVAL  100


#define ZPS_INIT_APL_DEFAULT_GLOBAL_APS_LINK_KEY TRUE
/*****************************************************************************/
/*! ZPS NWK AND APS PERSIST FRAME COUNTER */
/*****************************************************************************/

/*! PERSIST APS FRAME COUNTER EVERY ROLL OVER OF 1024 */
#define  APS_FC_SAVE_COUNT_BIT_SHIFT  10

/* PERSIST NWK FRAME COUNTER EVERY ROLL OVER OF 1024 */
#define  NWK_FC_SAVE_COUNT_BIT_SHIFT 10
/*****************************************************************************/

/*****************************************************************************/
/*! ZPS NWK CONFIG VALUES */
/*****************************************************************************/

/*! Stack Profile of the network */
/*! 0 - manufacturer-specific, 1 - ZigBee, 2 - ZigBee PRO, other values reserved */
#define ZPS_STACK_PROFILE  2

/*! ZigBee version of the network */
#define ZPS_ZIGBEE_VERSION 2

/*! The length of time to scan the selected RF
channels when searching for a netwok to
join. */
/*! [aBaseSuperframeDuration x (2n + 1)] symbols*/
#define ZPS_SCAN_DURATION 3

/*! Default ZDO Context */
/*! Default permit joining time seconds */
/*! 255 means permanently on 
    0 means permanently off */
#define ZPS_DEFAULT_PERMIT_JOINING_TIME       0

/*! Maximum number of AplZdoRequestKeyReq */
#define ZPS_MAX_NUM_SIMULTANEOUS_REQUEST_KEY_REQS 1


#define ZPS_PRECONFIGURED_TRUST_CENTER_LINK_KEY TRUE

#define ZPS_INTER_PAN TRUE
/*****************************************************************************/
/*! ZPS NWK NIB DEFAULT VALUES */
/*****************************************************************************/

/*! Vendor specific - Period of old route garbage collection */
#define ZPS_NWK_NIB_INIT_VSOLD_ROUTE_EXPIRY_TIME  600

/*! Network Max Routers */
#define ZPS_NWK_NIB_INIT_MAX_ROUTERS 5

/*! Network Max Children */
#define ZPS_NWK_NIB_INIT_MAX_CHILDREN 7

/*! Network Max Depth */
#define ZPS_NWK_NIB_INIT_MAX_DEPTH  15

/*! Network PassiveAckTimeout (in half-seconds) */ 
#define ZPS_NWK_NIB_INIT_PASSIVE_ACK_TIMEOUT  1

/*! Network Max Broadcast Retries */ 
#define ZPS_NWK_NIB_INIT_MAX_BROADCAST_RETRIES 2

/*! Network Max Source Route */
#define ZPS_NWK_NIB_INIT_MAX_SOURCE_ROUTE 12

/*! Network Broadcast Delivery Time */
#define ZPS_NWK_NIB_INIT_NETWORK_BROADCAST_DELIVERY_TIME 18

/*! Network Unique Addr */
#define ZPS_NWK_NIB_INIT_UNIQUE_ADDR 0

/*! Network Addr Alloc */
#define ZPS_NWK_NIB_INIT_ADDR_ALLOC 2

/*! Network Use Tree Routing: bool */
#define ZPS_NWK_NIB_INIT_USE_TREE_ROUTING 0

/*! Network SymLink: bool */
#define ZPS_NWK_NIB_INIT_SYMLINK 1

/*! Network Use Multicast: bool */
#define ZPS_NWK_NIB_INIT_USE_MULTICAST 0

/*! Network Link Status Period */
#define ZPS_NWK_NIB_INIT_LINK_STATUS_PERIOD 15

/*! Network Router Age Limit */
#define ZPS_NWK_NIB_INIT_ROUTER_AGE_LIMIT 3

/*! Network Route Discovery Retries Permitted */
#define ZPS_NWK_NIB_INIT_ROUTE_DISCOVERY_RETRIES_PERMITTED 3

/*!  Vendor specific - energy detect threshold for network forming */
#define ZPS_NWK_NIB_INIT_VS_FORMED_THRESHOLD 255

/*! Network Security Level */
#define ZPS_NWK_NIB_INIT_SECURITY_LEVEL 5

/*! Network All Fresh */
#define ZPS_NWK_NIB_INIT_ALL_FRESH TRUE

/*! Network Secure All Frames */
#define ZPS_NWK_NIB_INIT_SECURE_ALL_FRAMES TRUE

/*! Vendor specific - transmit fail threshold when RT nxt hop gets cleared */
#define ZPS_NWK_NIB_INIT_VS_TX_FAIL_THRESHOLD 5

/*! Vendor specific - only uses links with cost >= than this */
#define ZPS_NWK_NIB_INIT_VS_MAX_OUTGOING_COST 4

/*! Vendor specific - leave reajoin flags - flags: leave_allowed 1, leave_and_rejoin 2, leave_without rejoin 4 */
#define ZPS_NWK_NIB_INIT_VS_LEAVE_REJOIN 1

/*! Network End Device Timeout */
#define ZPS_NWK_NIB_INIT_ZED_TIMEOUT  2

/*! Network End Device Timeout Default */
#define ZPS_NWK_NIB_INIT_ZED_TIMEOUT_DEFAULT 8
/*****************************************************************************/

/*****************************************************************************/
/* NETWORK TABLES SIZES */
/*****************************************************************************/
/*! Active neighbour table size */
#define ZPS_NEIGHBOUR_TABLE_SIZE       26

/*! Neighbour Discovery Table Size */
/*! keeps a list of the neighbouring devices associated with the node */
#define ZPS_NEIGHBOUR_DISCOVERY_TABLE_SIZE  16

/*! Network Address Map Table  size */
/*! Size of the address map, which maps 64-bit 
   IEEE addresses to 16-bit network (short) addresses. */
#define ZPS_ADDRESS_MAP_TABLE_SIZE    10

/*! Network Security Material Set size */
#define ZPS_SECURITY_MATERIAL_SETS    2

/*! Broadcast Transaction Table size */
/*! the broadcast transaction records, which are records
    of the broadcast messages received by the node.*/
#define ZPS_BROADCAST_TRANSACTION_TABLE_SIZE 25

/*! Route Record Table size */
/*! Each route record contains the destination network address, 
    a count of the number of relay nodes to reach the destination,
    and a list of the network addresses of the relay nodes*/
#define ZPS_ROUTE_RECORD_TABLE_SIZE    1

/*! Routing Table Size */
/*! Information required for the node to participate 
    in the routing of message packets */
#define ZPS_ROUTING_TABLE_SIZE         70

/*! Route discovery table size */
/*! used by the node to store temporary information used
    during route discovery. */
#define ZPS_ROUTE_DISCOVERY_TABLE_SIZE 2

/* MAC address table size */
#define ZPS_MAC_ADDRESS_TABLE_SIZE    36

/*! Binding table Size */
#define ZPS_BINDING_TABLE_SIZE        16

/*! Group table Size */
#define ZPS_GROUP_TABLE_SIZE          16

/*! Security Material Sets */
/*! Number of supported network keys. */
#define ZPS_KEY_TABLE_SIZE            1

/*! CHILD TABLE SIZE */
/*! Size of the persisted sub-table of the active Neighbour table. */
/*! Entries for the node’s parent and immediate children */
#define ZPS_CHILD_TABLE_SIZE          5

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern void *g_pvApl;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void* ZPS_vGetGpContext(void);
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif
