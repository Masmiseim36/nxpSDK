/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _DHCP6_H
#define _DHCP6_H

/*!
\file       dhcp6.h
\brief      This is a public header file for the DHCPv6 module. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "stdint.h"   
#include "network_utils.h"
#include "ip.h"
#include "ip_if_management.h"
/*--------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
  INFORMATIVE MACROS THAT SHOULD NOT BE CHANGED BY THE USER
  (THESE ARE USED BY THREAD LIBRARY AT COMPILE TIME)
----------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------*/
#ifndef DHCP6_CLIENT_ENABLED
    #define DHCP6_CLIENT_ENABLED                    0
#endif

#ifndef DHCP6_SERVER_ENABLED
    #define DHCP6_SERVER_ENABLED                    0
#endif

#ifndef DHCP6_RELAY_AGENT_ENABLED
    #define DHCP6_RELAY_AGENT_ENABLED               0
#endif

#ifndef DHCP6_LEASEQUERY_ENABLED
    #define  DHCP6_LEASEQUERY_ENABLED               0
#endif

#ifndef DHCP6_RAPID_COMMIT
    #define DHCP6_RAPID_COMMIT                      1
#endif

#ifndef DHCP6_CLIENT_RETRANSMISSION_ENABLED
    #define DHCP6_CLIENT_RETRANSMISSION_ENABLED     1
#endif


/*==================================================================================================
Public macros
==================================================================================================*/
#define DHCP6_CLIENT_PORT     (546)   
#define DHCP6_SERVER_PORT     (547)      

#define DHCP6_HOST              1
#define DHCP6_ROUTER            2
#define DHCP6_BORDER_ROUTER     3
#define DHCP6_HW_TYPE_EUI64     27


/*! Message Type Codes */
#define DHCP6_SOLICIT                1
#define DHCP6_ADVERTISE              2
#define DHCP6_REQUEST                3
#define DHCP6_CONFIRM                4
#define DHCP6_RENEW                  5
#define DHCP6_REBIND                 6
#define DHCP6_REPLY                  7
#define DHCP6_RELEASE                8
#define DHCP6_DECLINE                9
#define DHCP6_RECONFIGURE            10
#define DHCP6_INFORMATION_REQUEST    11
#define DHCP6_RELAY_FW               12
#define DHCP6_RELAY_REPLY            13
#define DHCP6_LEASEQUERY             14
#define DHCP6_LEASEQUERY_REPLY       15

/*!< Option Codes */
#define DHCP6_OPTION_CLIENTID       1U 
#define DHCP6_OPTION_SERVERID       2U
#define DHCP6_OPTION_IA_NA          3U
#define DHCP6_OPTION_IA_TA          4U
#define DHCP6_OPTION_IAADDR         5U
#define DHCP6_OPTION_ORO            6U
#define DHCP6_OPTION_PREFERENCE     7U
#define DHCP6_OPTION_ELAPSED_TIME   8U
#define DHCP6_OPTION_RELAY_MSG      9U
#define DHCP6_OPTION_AUTH          11U
#define DHCP6_OPTION_UNICAST       12U
#define DHCP6_OPTION_STATUS_CODE   13U
#define DHCP6_OPTION_RAPID_COMMIT  14U
#define DHCP6_OPTION_USER_CLASS    15U
#define DHCP6_OPTION_VENDOR_CLASS  16U
#define DHCP6_OPTION_VENDOR_OPTS   17U
#define DHCP6_OPTION_INTERFACE_ID  18U
#define DHCP6_OPTION_RECONF_MSG    19U
#define DHCP6_OPTION_RECONF_ACCEPT 20U
#define DHCP6_OPTION_DNS_SERVERS   23U
#define DHCP6_OPTION_DOMAIN_LIST   24U
#define DHCP6_OPTION_IA_PD         25U
#define DHCP6_OPTION_IAPREFIX      26U
#define DHCP6_OPTION_LQ_QUERY      44U
#define DHCP6_OPTION_CLIENT_DATA   45U 
#define DHCP6_OPTION_CLT_TIME      46U

/*! Vendor-Specific Information Sub-Option Codes */
#define DHCP6_SUBOPTION_ROUTER_ID_MASK  0U
#define DHCP6_SUBOPTION_MLEID            1U
#define DHCP6_SUBOPTION_ATTACH_TIME     2U

#define DHCP6_DUID_LL              3U

#define DHCP6_HW_TYPE_ETH          1U

/*! Option lengths */
#define DHCP6_OPTION_ELAPSED_TIME_LEN       2U
#define DHCP6_OPTION_IAADR_LEN              24U  
#define DHCP6_OPTION_IA_NA_LEN              12U
#define DHCP6_OPTION_IA_PD_LEN              12U
#define DHCP6_OPTION_IA_PREFIX_LEN          25U
#define DHCP6_OPTION_STATUS_CODE_LEN        2U
#define DHCP6_OPTION_DNS_SERVERS_LEN        16U
#define DHCP6_OPTION_LQ_QUERY_LEN           45U
#define DHCP6_OPTION_RAPID_COMMIT_LEN       0U
#define DHCP6_OPTION_LAST_TRANSACTION_LEN   4U
#define DHCP6_OPTION_MLEID_LEN              8U

#define DHCP6_PKT_MAX_SIZE                  256U    //500U

/*! Transmission and Retransmission Parameters */
#define DHCP6_SOL_MAX_DELAY             1        /*Max delay of first Solicit*/ 
#define DHCP6_SOL_TIMEOUT               1        /*Initial Solicit timeout*/
#define DHCP6_SOL_MAX_RT                120      /*Max Solicit timeout value*/

#define DHCP6_REQ_TIMEOUT               1        /*Initial Request timeout*/
#define DHCP6_REQ_MAX_RT                30       /*Max Request timeout value*/
#define DHCP6_REQ_MAX_RC                10       /*Max Request retry attempts*/

#define DHCP6_CNF_MAX_DELAY             1        /*Max delay of first Confirm */
#define DHCP6_CNF_TIMEOUT               1        /*Initial Confirm timeout*/
#define DHCP6_CNF_MAX_RT                4        /*Max Confirm timeout */
#define DHCP6_CNF_MAX_RD                10       /*Max Confirm duration*/

#define DHCP6_REN_TIMEOUT               10       /*Initial Renew timeout  */
#define DHCP6_REN_MAX_RT                600      /*Max Renew timeout value*/

#define DHCP6_REB_TIMEOUT               10       /*Initial Rebind timeout  */
#define DHCP6_REB_MAX_RT                600      /*Max Rebind timeout value*/

#define DHCP6_INF_MAX_DELAY             1        /*Max delay of first Information-request*/
#define DHCP6_INF_TIMEOUT               1        /*Initial Information-request timeout*/
#define DHCP6_INF_MAX_RT                120      /*Max Information-request timeout value*/

#define DHCP6_REL_TIMEOUT               1        /*Initial Release timeout */
#define DHCP6_REL_MAX_RC                5        /*MAX Release attempts */

#define DHCP6_DEC_TIMEOUT               1        /*Initial Decline timeout*/ 
#define DHCP6_DEC_MAX_RC                5        /*Max Decline attempts */

#define DHCP6_REC_TIMEOUT               2        /*Initial Reconfigure timeout*/ 
#define DHCP6_REC_MAX_RC                8        /*Max Reconfigure attempts */

#define DHCP6_LQ_TIMEOUT                1        /*Initial LEASEQUERY timeout  */
#define DHCP6_LQ_MAX_RT                 10       /*Max LEASEQUERY timeout value*/
#define DHCP6_LQ_MAX_RC                 5        /*  Max LEASEQUERY retry attempts */

#define DHCP6_QUERY_BY_ADDRESS          1
#define DHCP6_QUERY_BY_CLIENTID         2

#define DHCP6_HOP_COUNT_LIMIT           32       /*Max hop count in a Relay-forward message*/ 


#define DEFAULT_LLADDR_IDX              0 

#define INFINITE_LIFETIME               THR_ALL_FFs32

/*! Enterprise number needed in Vendor Option */
#define DHCP6_ENTERPRISE_NUMBER         0x0000AFAA//44970 //Thread    
//#define DHCP6_ENTERPRISE_NUMBER         33118 //Freescale

#define DHCP6_MAX_REQUESTED_OPTION_LEN  10
#define DHCP6_MAX_DEVICEID_LEN          14


#define DHCP6_TIMER_PERIOD_MS           1000U     /*!< Timer interval in milliseconds */
#define DHCP6_TIMER_PERIOD_SEC          1U


#define DHCP6_LINK_LOCAL_ADDRESS_INDEX  0U

#define DHCP6_EXTRA_TIME_SPAN           10U   /* keep the client for another DHCP6_EXTRA_TIME_SPAN 
                                                    seconds after its address expires */


#define DHCP6_SERVER_NB_ADDR_RANGES     1
#define DHCP6_SERVER_NB_DNS_SERVERS     1
 
#define DHCP6_MAX_IA_ADDRESSES          2
#define DHCP6_NOPREFIX_RETRY_SEC        3600        
/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    gDhcp6AddressAssignment_c,
    gDhcp6DnsAddrAssign_c,
    gDhcp6Stateless_c,
    gDhcp6Pd_c,
    gDhcp6LeaseQuery_c,
    gDhcp6DnsPd_c
} dhcp6RequestedOptions_t;

typedef enum
{
    gDhcp6StateInit_c             = 0U, 
    gDhcp6StateSolicit_c          = 1U,
    gDhcp6StateSelecting_c        = 2U, /*!< Before receiving DHCP6 Advertise */
    gDhcp6StateRequesting_c       = 3U, /*!< After sending DHCP6 Request */    
    gDhcp6StateBound_c            = 4U, /*!< Connection with DHCPv6 server established */
    gDhcp6StateRenewing_c         = 5U, /*!< Timer 1 expired, sent DHCP6 Solicit to leasing server */
    gDhcp6StateRebinding_c        = 6U, /*!< Timer 2 expired, sent broadcast DHCPREQUEST*/
    gDhcp6StateRelease_c          = 7U,
    gDhcp6StateDecline_c          = 8U,
    gDhcp6StateConfirm_c          = 9U  
} dhcp6State_t;


typedef enum 
{
    gDhcp6Success_c           = 0,
    gDhcp6UnspecFail_c        = 1,
    gDhcp6NoAddrsAvail_c      = 2,
    gDhcp6NoBinding_c         = 3,
    gDhcp6NotOnLink_c         = 4,
    gDhcp6UseMulticast_c      = 5,
    gDhcp6NoPrefixAvail_c     = 6,
    gDhcp6AddrNotRegistered_c = 10,
    gDhcp6NoStatus_c          = 11,
    gDhcp6RelayedMsg_c        = 12  
} dhcp6StatusCode;


typedef struct dhcp6Packet_tag
{
    uint8_t msgType;
    uint8_t xid[3];
} dhcp6Packet_t;

typedef struct dhcp6RelayAgPkt_tag
{
    uint8_t msgType;
    uint8_t hopCount;
    uint8_t linkAddress[16];
    uint8_t peerAddress[16];
}dhcp6RelayAgPkt_t;

typedef struct dhcp6DUID_tag
{
    uint16_t duidType;
    uint16_t hardwareType;    
    uint8_t lladdr[8];
} dhcp6DUID_t;

typedef struct dhcp6StatusCode_tag
{
    uint16_t opNo;
    uint16_t opLen;
    uint16_t statusCode;

} dhcp6StatusCode_t;

typedef struct dhcp6IaAddr_tag
{
    uint16_t opNo;
    uint16_t opLen;
    ipAddr_t ip6Addr;
    uint32_t preferredLifetime;
    uint32_t validLifetime;
    dhcp6StatusCode_t statusCode;
    
} dhcp6IaAddr_t;

typedef struct dhcp6IaNa_tag
{
    uint8_t iaid[4];
    uint8_t T1[4];
    uint8_t T2[4];
    uint8_t iaAddrOpCode[2];
    uint8_t iaAddrOpLen[2];
    uint8_t ipAddr[16];
    uint8_t preferredLifetime[4];
    uint8_t validLifetime[4];
    uint8_t statusCodeOpCode[2];
    uint8_t statusCodeOpLen[2];
    uint8_t statusCode[2];
} dhcp6IaNa_t;

typedef struct dhcp6VendorOpt_tag
{
    uint8_t* pRouterIdMask;
    uint8_t* pMlEidIid;
    uint8_t* pAttachTime;    
}dhcp6VendorOpt_t;

typedef struct dhcp6LeasequeryInfo_tag
{
    uint32_t lastTransactionTime;
    uint32_t attachTime;
    ipAddr_t* pIpAddr;
    uint8_t* pClientId;
}dhcp6LeasequeryInfo_t;

typedef struct dhcp6ClientTempBindingTbl_tag
{
    uint8_t msgType;
    uint8_t state;
    uint8_t xid[3];
    uint8_t status;
    bool_t rapidCommit;
    uint16_t clientIDlen;
    uint16_t len;
    uint16_t deviceType;
    uint32_t mode;
    uint32_t iaId;
    uint8_t nbIaNaOp;
    uint8_t nbIaAddrOp;
    ipIfUniqueId_t ipIfId;
    ipAddr_t clientOfferedIpAddr[DHCP6_MAX_IA_ADDRESSES];
    ipAddr_t clientLLAddr; 
    uint8_t clientID[DHCP6_MAX_DEVICEID_LEN];     
    uint8_t paramReqList[DHCP6_MAX_REQUESTED_OPTION_LEN];

} dhcp6ClientTempBindingTbl_t;

typedef void (*dhcp6RelayAgCb_t)(uint8_t msgType,void* info,dhcp6Packet_t * pRxDhcp6Pkt,
                                    uint32_t dhcp6PktSize);

typedef struct dhcp6LqNotifyData_tag
{
    ipAddr_t ipAddr;                        /*!< ULA16 */
    uint8_t eui[gLlayerAddrEui64_c];        /*!< extended address */
    uint32_t clientTime;                    /*!< client last transaction time */
    uint32_t attachTime;                    /*!< seconds since the device attached to the Thread partition*/

}dhcp6LqNotifyData_t;

typedef struct dhcp6ServerIpCfg_tag
{
    ipAddr_t startIpAddr;
    ipAddr_t endIpAddr;
    uint64_t addrIncrFactor;
    uint16_t clientType;
    uint16_t prefixLen;
}dhcp6ServerIpCfg_t;
typedef struct dhcp6ServerVars_tag
{
    dhcp6RelayAgCb_t pfRelayAgCb;
}dhcp6ServerVars_t;

typedef struct dhcp6ServerCfg_tag
{
    ipAddr_t aLastAssignedIpAddr[DHCP6_SERVER_NB_ADDR_RANGES];
    dhcp6ServerIpCfg_t aIpCfg[DHCP6_SERVER_NB_ADDR_RANGES];
    ipAddr_t aDnsAddrCfg[DHCP6_SERVER_NB_DNS_SERVERS];  
    ipIfUniqueId_t ipIfId;
    dhcp6DUID_t serverDUID;
    dhcp6ServerVars_t serverVars;
}dhcp6ServerCfg_t;

typedef void (* dhcp6LqNotifyCb_t)(dhcp6LqNotifyData_t* pNotifyData);

/*==================================================================================================
Public global variables declarations
==================================================================================================*/



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
#endif  /* _FILENAME_PUBLIC_H */
