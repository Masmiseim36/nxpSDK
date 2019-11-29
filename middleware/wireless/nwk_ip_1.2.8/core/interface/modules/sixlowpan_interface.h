/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SIXLOWPAN_INTERFACE_H
#define _SIXLOWPAN_INTERFACE_H
/*!=================================================================================================
\file       sixlowpan_interface.h
\brief      This is a header file for the SixlowPan interface.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "mac_abs_types.h"
#include "sixlowpan_ib.h"
#include "network_utils.h"
#include "event_manager.h"

/*==================================================================================================
Public macros
==================================================================================================*/
/*! Maximum number of hops */
#define SLWPCFG_IB_MAX_HOPS             10U

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! Enumeration of ADP statuses */
typedef enum adpStatus_tag
{
    gAdpStatusSuccess_c               = 0x00U,
    gAdpStatusInvalidIPv6Frame_c      = 0x01U,
    gAdpStatusInvalidRequest_c        = 0x02U,
    gAdpStatusNokey_c                 = 0x03U,
    gAdpStatusBadCCMOutput_c          = 0x04U,
    gAdpStatusRouteError_c            = 0x05U,
    gAdpStatusBTTableFull_c           = 0x06U,
    gAdpStatusFrameNotBuffered_c      = 0x07U,
    gAdpStatusCFARequestExceeded_c    = 0x08U,
    gAdpStatusNotPermitted_c          = 0x09U
} adpStatus_t;

typedef enum
{
    /* G3 Adaptation Layer Attributes IDs */
    gAdpIbGmk_c                         = 0x00U,
    gAdpIbIPv6Address_c                 = 0x01U,
    gAdpIbBroadcastLogTableEntryTTL_c   = 0x02U,
    gAdpIbMaxDiscoveryPerHour_c         = 0x04U,
    gAdpIbNumDiscoveryAttempts_c        = 0x05U,
    gAdpIbDiscoveryAttemptsSpeed_c      = 0x06U,
    gAdpIbContextInformationTable_c     = 0x07U,
    gAdpIbBroadcastLogTable_c           = 0x0BU,
    gAdpIbRoutingTable_c                = 0x0CU,
    gAdpIbUnicastRREQGenEnable_c        = 0x0DU,
    gAdpIbGroupTable_c                  = 0x0EU,
    gAdpIbToneMask_c                    = 0x0FU,
    gAdpIbMaxHops_c                     = 0x10U,
    gAdpIbDeviceType_c                  = 0x11U,
    gAdpIbNetTraversalTime_c            = 0x12U,
    gAdpRoutingTableEntryTTL_c          = 0x13U,
    gAdpIbKr_c                          = 0x14U,
    gAdpIbKm_c                          = 0x15U,
    gAdpIbKc_c                          = 0x16U,
    gAdpIbKq_c                          = 0x17U,
    gAdpIbKh_c                          = 0x18U,
    gAdpIbRREQRetries_c                 = 0x19U,
    gAdpIbRREQRERRWait_c                = 0x1AU,
    gAdpIbWeakLQIValue_c                = 0x1BU,
    gAdpIbKrt_c                         = 0x1CU,
    gAdpIbSoftVersion_c                 = 0x1DU,
    gAdpIbSnifferMode_c                 = 0x1EU,
    gAdpIbMaxConsecutiveRequest_c       = 0x1FU,
    gAdpIbMaxConsecutiveCfaTime_c       = 0x20U,
    gAdpIbMaxJoinWaitTime_c             = 0x21U,
    gAdpIbPathDiscoveryTime_c           = 0x22U,
    gAdpIbUseNewGMKTime_c               = 0x23U,
    gAdpIbBlacklistedNeighborSet_c      = 0x25U,
    gAdpIbExpPrecGMKTime_c              = 0x24U,
    gAdpIbBlacklistedNeighborSetTTL_c   = 0x28U,
    gAdpIbSecurityLevel                 = 0x29U,

    /* Custom IDs */
    gAdpIbCustomAdpStateDeviceJoined_c  = 0xA1U,

    gAdpIbCustomEnableHC_c              = 0xD1U,
    gAdpIbCustomBCSequence_c            = 0xD2U,
    gAdpIbCustomFragSequence_c          = 0xD3U,

    gAdpIbCustomLBPTransactionId_c      = 0xE1U,

    gAdpIbCustomPSK_c                   = 0xF1U,
    gAdpIbCustomEAPRand_c               = 0xF2U,
    gAdpIbCustomEAXCtr_c                = 0xF3U,
    gAdpIbCustomEAPRandDis_c            = 0xF4U,
    gAdpIbCustomAK_c                    = 0xF5U,
    gAdpIbCustomKDK_c                   = 0xF6U,
    gAdpIbCustomPrefixTable_c           = 0xF7U,
    gAdpIbCustomEnableLoadNG_c          = 0xF8U,
    gAdpCustomAggregateRouteReplies_c   = 0xF9U
} adpIbId_t;

/*! Enumeration of ADPD Data Request QoS options */
typedef enum adpQos_tag
{
    gAdpQoSNormalPriority_c           = 0x00U,        /*!< Normal priority */
    gAdpQoSHighPriority_c             = 0x01U,        /*!< High priority */
    gAdpQoSContentionFreeAccess_c     = 0x02U,        /*!< Contention free (optional) */
    gAdpQoSInvalidPriority_c          = 0xFFU         /*!< Invalid priority */
} adpQos_t;

/*! Enumeration of statuses returned by ADPM Reset Confirm */
typedef enum adpmResetStatus_tag
{
    gAdpmResetStatusSuccess_c           = 0x00U,
    gAdpmResetStatusDisableTrxFailure_c = 0x01U
} adpmResetStatus_t;

typedef uint8_t adpmDiscoveryStatus_t;

/*! Enumeration of statuses returned by ADPM Route Discovery Confirm */
typedef enum adpmDiscoveryStatus_tag
{
    gAdpmDiscStatusSuccess_c        = 0x00U,
    gAdpmDiscStatusNoBeacons_c      = 0x01U,
    gAdpmDiscStatusFailed_c         = 0x02U
} adpmDiscoveryStatus_e;

typedef uint8_t adpmNwkStartStatus_t;

/*! Enumeration of statuses returned by ADPM Reset Confirm */
typedef enum adpmNwkStartStatus_tag
{
    gAdpmNwkStartStatusSuccess_c        = 0x00U,
    gAdpmNwkStartStatusInvalidReq_c     = 0x01U,
    gAdpmNwkStartStatusStartupFailure_c = 0x02U
} adpmNwkStartStatus_e;

typedef uint32_t adpmGetSetStatus_t;

/*! Enumeration of statuses returned by ADPM Get Confirm */
typedef enum adpmGetSetStatus_tag
{
    gAdpmGetStatusSuccess_c             = 0x00U,
    gAdpmGetStatusUnsupportedAttr_c     = 0x01U,
    gAdpmGetStatusInvalidIndex_c        = 0x02U,
    gAdpmGetStatusReadOnly_c            = 0x03U,
    gAdpmGetStatusInvalidParameter_c    = 0x04U,
} adpmGetSetStatus_e;

typedef uint32_t adpmRouteDiscoveryStatus_t;

/*! Enumeration of statuses returned by ADPM Route Discovery Confirm */
typedef enum adpmRouteDiscoveryStatus_tag
{
    gAdpmRouteDiscStatusSuccess_c        = 0x00U,
    gAdpmRouteDiscStatusInvalidReq_c     = 0x01U,
    gAdpmRouteDiscStatusRouteError_c     = 0x02U
} adpmRouteDiscoveryStatus_e;

typedef uint32_t adpAddrModeType_t;

typedef enum
{
    gAdpAddrModeNoAddress_c             = 0x00000000U,  /*!< No address (addressing fields omitted)*/
    gAdpAddrModeReserved_c              = 0x00000001U,  /*!< Reserved value*/
    gAdpAddrModeShortAddress_c          = 0x00000002U,  /*!< 16-bit short address*/
    gAdpAddrModeExtendedAddress_c       = 0x00000003U   /*!< 64-bit extended address*/
} adpAddrModeType_e;

typedef enum
{
    gAdpNsduTypeAny_c                   = 0x00,
    gAdpNsduTypeUdp_c                   = 0x01,
    gAdpNsduTypeIcmp_c                  = 0x02,
    gAdpNsduTypeTcp_c                   = 0x03,
} adpNsduType_t;

/*! Enumeration of 6LoWPAN packet types */
typedef enum adpPacketType_tag
{
    gAdpPacketIPv6_c,                                  /*!< IPv6 Packet */
    gAdpPacketLbpcMsg_c,                               /*!< LBP Packet send by client */
    gAdpPacketLbpsMsg_c,                               /*!< LBP Packet send by server */
    gAdpPacketMeshMsg_c,                               /*!< MESH Packet */
    gAdpPacketRoutedPacket_c                           /*!< Routed packet */
} adpPacketType_t;

/*! 6LoWPAN Event Sets */
typedef enum
{
    gSlwpEvSet_Security_c    = 0x00010000 | gEvmModule6LoWPAN_c,
} slwpEvSets_t;

/*! 6LoWPAN Events */
typedef enum
{
    gSlwpEv_SecInd_RcvdMACDiffKeyIdx_c = gSlwpEvSet_Security_c + 1,                 /* 6lowpan security indication - received different key index*/
} slwpEvCodesSecurity_t;

/*! Structure which defines the ADPD Data Request parameters */
typedef struct adpdDataReq_tag
{
    instanceId_t    slwpInstanceId;         /*!< 6LoWPAN instance id */
    nwkBuffer_t     pNsduNwkBuff;           /*!< Pointer to the IPv6 datagram in network buffers */
    ipAddr_t        ipDstAddr;              /*!< Pointer to the Destination (Next Hop) IP Address
                                                 If Not NULL then the IP stack provides the next hop IP address.
                                                 If NULL then 6LoWPAN computes the next hop address from the IP header.*/
    uint8_t         nsduHandle;             /*!< Identifier for the data request */
    adpQos_t        qualityOfService;       /*!< Priority of the transmission */
    bool_t          discoverRoute;          /*!< Enable/disable route discovery */
    uint8_t         securityLevel;          /*!< MAC Security level */
    uint8_t         macSecKeyIdMode;
    uint8_t         channel;
    uint16_t        destPanId;
} adpdDataReq_t;

/*! Structure which defines the ADP Data Confirm parameters */
typedef struct adpdDataCnf_tag
{
    instanceId_t    ifInstanceId;           /*!< upper layer instance id */
    adpStatus_t     status;                 /*!< Status of the data request */
    uint8_t         nsduHandle;             /*!< Identifier for the data request */
} adpdDataCnf_t;

/*! Structure which defines the ADPD Data Indication parameters */
typedef struct adpdDataInd_tag
{
    instanceId_t    ifInstanceId;           /*!< upper layer instance id */
    uint8_t *       pNsdu;                  /*!< Pointer to the IPv6 datagram */
    uint16_t        nsduLength;             /*!< Length of the IPv6 datagram. Maximum value is 1280 */
    uint8_t         securityLevel;          /*!< MAC security level */
    uint8_t         linkQualityIndicator;   /*!< Link quality during reception of the frame */
    llAddr_t        srcAddrInfo;
    llAddr_t        dstAddrInfo;
    uint16_t        srcPanId;               /* MAC source panID */
    uint8_t         channel;                /* MAC channel */
    uint8_t         keyIdMode;
} adpdDataInd_t;

/*! Structure which defines the ADPM Discovery Request parameters */
typedef struct adpmDiscoveryReq_tag
{
    uint8_t  duration;                 /*!< Number of seconds the active scan shall last */
} adpmDiscoveryReq_t;

/*! Structure which defines the PAN descriptor */
typedef struct adpPanDescriptor_tag
{
    uint16_t panID;                    /*!< PAN identifier */
    uint16_t lbaAdress;                /*!< Short address of the device to be used as the LBA */
    uint8_t  linkQuality;              /*!< Link quality of LBA */
    uint8_t  rcCoord;                  /*!< Estimated route cost from LBA to the coordinator */
} adpPanDescriptor_t;

/*! Structure which defines the ADPM Discovery Confirm parameters */
typedef struct adpmDiscoveryCnf_tag
{
    adpPanDescriptor_t *   panDescriptor;/*!< PAN operating in the POS of the device */
    adpmDiscoveryStatus_t  status;       /*!< Status of the discovery request */
    uint8_t                panCount;     /*!< Number of PANs operating in the POS of the device */
} adpmDiscoveryCnf_t;

#if (SLWPCFG_SERVER_ENABLED)

/*! Structure which defines the ADPM Network Join Request parameters */
typedef struct adpmNwkStartReq_tag
{
    uint16_t  panId;                   /*!< PAN Identifier of the network to start */
} adpmNwkStartReq_t;

/*! Structure which defines the ADPM Network Start Confirm parameters */
typedef struct adpmNwkStartCnf_tag
{
    adpmNwkStartStatus_t  status;      /*!< The result of the attempt to start the network */
} adpmNwkStartCnf_t;

#endif /* ADPCFG_DEVICE_SERVER_ENABLED */

#if (SLWPCFG_NODE_ENABLED)

/*! Structure which defines the ADPM Network Join Request parameters */
typedef struct adpmNwkJoinReq_tag
{
    uint16_t    panId;                   /*!< PAN Identifier of the network to join */
    uint16_t    lbaAddress;              /*!< Short address of the device acting as a LoWPAN Bootstrap Agent */
} adpmNwkJoinReq_t;

/*! Structure which defines the ADPM Network Join Confirm parameters */
typedef struct adpmNwkJoinCnf_tag
{
    uint16_t    nwkAddress;           /*!< The 16-bit network address that was allocated to the device */
    uint16_t    panId;                /*!< The 16-bit PAN identifier of which the device is now a member */
    adpStatus_t status;               /*!< The result of the attempt to join the network */
} adpmNwkJoinCnf_t;

/*! Structure which defines the ADPM Network Leave Confirm parameters */
typedef struct adpmNwkLeaveCnf_tag
{
    uint64_t    extendedAddress;      /*!< Extended address */
    adpStatus_t status;               /*!< Status of the Leave request */
} adpmNwkLeaveCnf_t;

#endif /* ADPCFG_DEVICE_NODE_ENABLED */

/*! Structure which defines the ADPM Network Leave Indication parameters */
typedef struct adpmNwkLeaveInd_tag
{
    uint64_t    extendedAddress;         /*!< Extended address */
} adpmNwkLeaveInd_t;

/*! Structure which defines the ADPM Get Request parameters */
typedef struct adpmGetReq_tag
{
    uint32_t    attributeIndex;           /*!< The index within the table of the IB attribute to read */
    adpIbId_t   attributeId;              /*!< The identifier of the IB attribute */
} adpmGetReq_t;

/*! Structure which defines the ADPM Get Confirm parameters */
typedef struct adpmGetCnf_tag
{
    void *              attributeValue;   /*!< The value of the attribute read from the IB */
    uint32_t            attributeIndex;   /*!< The index within the table of the IB attribute */
    adpmGetSetStatus_t  status;           /*!< The status of the reading */
    adpIbId_t           attributeId;      /*!< The identifier of the IB attribute */
} adpmGetCnf_t;

/*! Structure which defines the ADPM Set Request parameters */
typedef struct adpmSetReq_tag
{
    void *    attributeValue;          /*!< The value of the attribute to set in the IB */
    uint32_t  attributeIndex;          /*!< The index within the table of the IB attribute */
    adpIbId_t attributeId;             /*!< The identifier of the IB attribute */
} adpmSetReq_t;

/*! Typedef for the ADPM Set Confirm parameters */
typedef adpmGetSetStatus_t adpmSetCnf_t;

/*! Structure which defines the ADPM Network Reset Confirm parameters */
typedef struct adpmResetCnf_tag
{
    adpmResetStatus_t  status;         /*!< The result of the ADPM reset operation */
} adpmResetCnf_t;

typedef struct addrInfo_tag
{
    union
    {
        uint64_t extendedAddr;
        uint16_t shortAddr;
    } addr;                            /*!< Destination address: short/extended */
    adpAddrModeType_t  addrType;       /*!< Destination address type: short/extended */
} addrInfo_t;

/*! Structure which defines the ADPM Route Discovery Request parameters */
typedef struct adpmRouteDiscoveryReq_tag
{
    uint16_t dstShortAddr; /*!< The short unicast destination address of the route discovery */
    uint8_t  maxHops;      /*!< The maximum number of hops allowed for the route discovery */
} adpmRouteDiscoveryReq_t;

/*! Structure which defines the ADPM Route Discovery Confirm parameters */
typedef struct adpmRouteDiscoveryCnf_tag
{
    adpmRouteDiscoveryStatus_t status; /*!< The result of the ADPM Route Discovery operation */
} adpmRouteDiscoveryCnf_t;

/*! Structure which defines the ADPM Path Discovery Request parameters */
typedef struct adpmPathDiscoveryReq_tag
{
    uint16_t dstShortAddr; /*!< The short unicast destination address of the path discovery */
} adpmPathDiscoveryReq_t;

/*! Structure which defines the ADPM Path Discovery Confirm parameters */
typedef struct adpmPathDiscoveryCnf_tag
{
    uint16_t dstShortAddr;
    uint16_t routeCost;
    uint16_t pathAddress[SLWPCFG_IB_MAX_HOPS];
    uint8_t  hopCount;
    uint8_t  weakLinkCount;
    bool_t   bResult;
} adpmPathDiscoveryCnf_t;

/*! Structure which defines the ADPD Data Request parameters */
typedef struct adpmLbpReq_tag
{
    uint8_t *       pNsdu;             /*!< Pointer to the NSDU */
    uint16_t        nsduLength;        /*!< Length of the NSDU. Maximum value is 1280 */
    uint8_t         nsduHandle;        /*!< Identifier for the lbp request */
    adpQos_t        qualityOfService;  /*!< Priority of the transmission */
    bool_t          discoverRoute;     /*!< Enable/disable route discovery */
    bool_t          securityEnabled;   /*!< Enable/disable MAC security */
    adpNsduType_t   nsduType;          /*!< Enable/disable MAC security */
    uint8_t         maxHops;           /*!< The number of times the frame will be repeated by network routers. */
    addrInfo_t      destAddrInfo;      /*!< Contains destination address type and value */
} adpmLbpReq_t;

/*! Structure which defines the ADPM LBP Confirm parameters */
typedef struct adpmLbpCnf_tag
{
    adpStatus_t     status;                /*!< Status of the lbp request */
    uint8_t         nsduHandle;            /*!< Identifier for the lbp request */
} adpmLbpCnf_t;

/*! Structure which describes the LBP indication parameters */
typedef struct adpmLbpInd_tag
{
    uint8_t *       pNsdu;
    uint16_t        dstAddr;
    uint16_t        nsduLength;
    bool_t          bSecurityEnabled;
    uint8_t         nsduType;
    uint8_t         lqi;
    addrInfo_t      srcAddrInfo;
} adpmLbpInd_t;

typedef struct adpmSleepyPollInd_tag
{
    uint64_t                srcAddr;
    instanceId_t            instanceId;
    uint16_t                srcPanId;
    macAbsAddrModeType_t    srcAddrMode;
} adpmSleepyPollInd_t;

typedef struct adpdCb_tag
{
    void (* adpdDataCnf)            (adpdDataCnf_t * pParam);
    void (* adpdDataInd)            (adpdDataInd_t * pParam);
} adpdCb_t;

typedef struct adpmCb_tag
{
    void (* adpmDiscoveryCnf)       (adpmDiscoveryCnf_t * pParam);
#if (SLWPCFG_SERVER_ENABLED)
    void (* adpmNwkStartCnf)        (adpmNwkStartCnf_t * pParam);
#endif
#if (SLWPCFG_NODE_ENABLED)
    void (* adpmNwkJoinCnf)         (adpmNwkJoinCnf_t * pParam);
    void (* adpmNwkLeaveCnf)        (adpmNwkLeaveCnf_t * pParam);
#endif
    void (* adpmNwkLeaveInd)        (adpmNwkLeaveInd_t * pParam);
    void (* adpmResetCnf)           (adpmResetCnf_t * pParam);
#if (SLWPCFG_ROUTING_ENABLED)
    void (* adpmRouteDiscoveryCnf)  (adpmRouteDiscoveryCnf_t * pParam);
    void (* adpmPathDiscoveryCnf)   (adpmPathDiscoveryCnf_t * pParam);
#endif
    void (* adpmSleepyPollInd)      (adpmSleepyPollInd_t * pParam);
    void (* adpmDataReq)            (instanceId_t macInstanceId, macAbsMcpsDataReq_t * pParam);
    void (* adpmDataCnf)            (macAbsMcpsDataCnf_t * pParam);
} adpmCb_t;

typedef macAbsRfdType_t (* getRfdType_t) (macAbsAddrModeType_t* addrMode, uint64_t* address, bool_t *bEhFp);
typedef bool_t (* getNextReducedDev_t) (uint16_t* address);
typedef bool_t (* checkRfdMcast_t)      (instanceId_t instanceId, uint16_t shortAddress, ipAddr_t *pIpAddr,
                                           ipAddr_t *pIpSrcAddr);
typedef bool_t (* checkAddrCompression) (instanceId_t instanceId, ipAddr_t *pDestAddr,
                                         ipAddr_t *pTargetAddr);

#if (SLWPCFG_SERVER_ENABLED) && (SLWPCFG_LBP_ENABLED)
typedef struct lbpCb_tag
{
    void (* adpmLbpCnf)             (adpmLbpCnf_t * pParam);
    void (* adpmLbpInd)             (adpmLbpInd_t * pParam);
} lbpCb_t;
#endif

/*==================================================================================================
Public global variables declarations
==================================================================================================*/


/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void SLWP_PurgeDevice(instanceId_t slwpInstanceId, uint16_t shortAddr, uint64_t extendedAddr)
\brief  Purge all queued indirect packets for the requested device

\param  [in]    slwpInstanceId      6LoWPAN instance pointer
\param  [in]    shortAddr           device short address
\param  [in]    extendedAddr        device extended address
***************************************************************************************************/
void SLWP_PurgeDevice( instanceId_t slwpInstanceId, uint16_t shortAddr, uint64_t extendedAddr);

/*!*************************************************************************************************
\fn     instanceId_t SLWP_Init(uint32_t instanceNb, adpIb_t * defaultAdpIb)
\brief  6LoWPAN Instance initialization function.

\param  [in]    instanceNb      Instance number.
\param  [in]    defaultAdpIb   Pointer to the ADP IB structure.

\retval         instanceId_t    Instance ID. Pointer to the 6LoWPAN instance structure.
***************************************************************************************************/
instanceId_t SLWP_Init(uint32_t instanceNb, adpIb_t *defaultAdpIb);

/*!*************************************************************************************************
\fn     void SWLP_RegisterMacInstance(instanceId_t slwpInstanceId, instanceId_t macAbsInstanceId,
                                      instanceId_t macInstanceId)
\brief  Interface function for the 6LoWPAN module. It registers a MAC instance to a 6LoWPAN instance.

\param  [in]    slwpInstanceId      6LoWPAN instance ID
\param  [in]    pMacAbsReq          pointer MAC Abstraction Requests
\param  [in]    macInstanceId       MAC Layer instance ID
***************************************************************************************************/
void SWLP_RegisterMacInstance(instanceId_t slwpInstanceId, macAbsRequests_t* pMacAbsReq, 
                                            instanceId_t macInstanceId);

/*!*************************************************************************************************
\fn     void SWLP_RegisterIfInstance(instanceId_t slwpInstanceId, instanceId_t ifInstanceId)
\brief  Interface function for the 6LoWPAN module. It registers the upper layer interface instance.

\param  [in]    slwpInstanceId      6LoWPAN instance ID
\param  [in]    ifInstanceId        Interface instance ID
***************************************************************************************************/
void SWLP_RegisterIfInstance(instanceId_t slwpInstanceId, instanceId_t ifInstanceId);

/*!*************************************************************************************************
\fn     void SWLP_RegisterAdpdCallbacks(instanceId_t slwpInstanceId, adpdCb_t * pAdpdCb)
\brief  Interface function for the 6LoWPAN module. It registers the callbacks for the 6LoWPAN.

\param  [in]    slwpInstanceId      6LoWPAN instance ID
\param  [in]    pAdpdCb             Pointer to callback structure
***************************************************************************************************/
void SWLP_RegisterAdpdCallbacks(instanceId_t slwpInstanceId, adpdCb_t * pAdpdCb);

/*!*************************************************************************************************
\fn     void SWLP_RegisterAdpmCallbacks(instanceId_t slwpInstanceId, adpmCb_t * pAdpmCb)
\brief  Interface function for the 6LoWPAN module. It registers the callbacks for the 6LoWPAN.

\param  [in]    slwpInstanceId      6LoWPAN instance ID
\param  [in]    pAdpmCb             Pointer to callback structure
***************************************************************************************************/
void SWLP_RegisterAdpmCallbacks(instanceId_t slwpInstanceId, adpmCb_t * pAdpmCb);

/*!*************************************************************************************************
\fn     void SLWP_RegisterMgmtFunc(instanceId_t slwpInstanceId, getRfdType_t pfGetRfdType,
                  getNextReducedDev_t pfGetNextReducedEndDev, checkRfdMcast_t pfCheckRfdMcast,
                  checkAddrCompression pfCheckAddrCompression)
\brief  Interface function for the 6LoWPAN module. It registers the callbacks for the 6LoWPAN.

\param  [in]    slwpInstanceId          6LoWPAN instance ID
\param  [in]    pfGetRfdType            Pointer to IsSleepyDevice function
\param  [in]    pfGetNextReducedEndDev  Pointer to GetNextReducedDevice function
\param  [in]    pfCheckRfdMcast         Pointer to CheckRfdMcast function
\param  [in]    pfCheckAddrCompression  Pointer to CheckAddrCompression function
***************************************************************************************************/
void SLWP_RegisterMgmtFunc(instanceId_t slwpInstanceId, getRfdType_t pfGetRfdType,
                   getNextReducedDev_t pfGetNextReducedEndDev, checkRfdMcast_t pfCheckRfdMcast,
                   checkAddrCompression pfCheckAddrCompression);

#if (SLWPCFG_LBP_ENABLED)
/*!*************************************************************************************************
\fn     void SWLP_RegisterAdpdCallbacks(instanceId_t slwpInstanceId, lbpCb_t * pLbpCb)
\brief  Interface function for the 6LoWPAN module. It registers the callbacks for the 6LoWPAN.

\param  [in]    slwpInstanceId      6LoWPAN instance ID
\param  [in]    pAdpdCb             Pointer to callback structure
***************************************************************************************************/
void SWLP_RegisterLbpCallbacks(instanceId_t slwpInstanceId, lbpCb_t * pLbpCb);
#endif

/*!*************************************************************************************************
\fn     void SLWP_DataRequest(adpdDataReq_t * pAdpdDataReq)
\brief  Interface function for the 6LoWPAN module. It sends a 6LoWPAN Data Packet.

\param  [in]    pAdpdDataReq    Pointer to the ADPD Data Request structure.
***************************************************************************************************/
void SLWP_DataRequest(adpdDataReq_t * pAdpdDataReq);

/*!*************************************************************************************************
\fn     void SLWP_McpsDataCnfCB(macAbsMcpsDataCnf_t * pMcpsDataCnf, instanceId_t instanceId)
\brief  Interface function for the 6LoWPAN module. Callback used by MAC to signal a MCPS Data Confirm.

\param  [in]    pMcpsDataCnf    Pointer to the MCPS Data Confirmation structure
***************************************************************************************************/
void SLWP_McpsDataCnfCB(macAbsMcpsDataCnf_t * pMcpsDataCnf);

/*!*************************************************************************************************
\fn     void SLWP_McpsDataCnfCB(macAbsMcpsDataInd_t * pMcpsDataInd, instanceId_t instanceId)
\brief  Interface function for the 6LoWPAN module. Callback used by MAC to signal a MCPS Data Indication.

\param  [in]    pMcpsDataInd    Pointer to the MCPS Data Indication structure
\param  [in]    srcAddrMode     The source address mode used.
\param  [in]    pSrcAddr        The source address of the sender.
***************************************************************************************************/
void SLWP_McpsDataIndCB(macAbsMcpsDataInd_t * pMcpsDataInd, macAbsAddrModeType_t srcAddrMode, 
                                  uint64_t *pSrcAddr);

/*!*************************************************************************************************
\fn     void SLWP_MlmePollIndCB(macAbsMlmePollNotifyInd_t *pMlmePollInd, instanceId_t instanceId)
\brief  Interface function for the 6LoWPAN module. Callback used by MAC to signal a MLME Poll Indication.

\param  [in]    pMlmePollInd    Pointer to the MLME Poll Indication structure
\param  [in]    instanceId      Instance ID
***************************************************************************************************/
void SLWP_MlmePollIndCB(macAbsMlmePollNotifyInd_t *pMlmePollInd, instanceId_t instanceId);

/*!*************************************************************************************************
\fn     void SLWP_GetLLADDRFromIID(uint8_t * pIID, llAddr_t * pLlAddr, uint16_t * pPanId, instanceId_t instanceId)
\brief  Interface function for the 6LoWPAN module. It returns the Link-Layer address from the IID.

\param  [in]    pIID            Pointer to the IID
\param  [out]   pLlAddr         Pointer to the variable which will hold the Link-Layer address
\param  [out]   pPanId           Pointer to the variable which will hold the PAN ID
\param  [in]    instanceId      Instance ID
***************************************************************************************************/
void SLWP_GetLLADDRFromIID(uint8_t * pIID, llAddr_t * pLlAddr, uint16_t * pPanId, instanceId_t instanceId);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/


#endif  /*_SIXLOWPAN_INTERFACE_H */
