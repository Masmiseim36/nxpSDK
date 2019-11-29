/*! *********************************************************************************
 * \defgroup TTY  Thread Types Interface
 * @{
 ***********************************************************************************/
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _THREAD_TYPES_H
#define _THREAD_TYPES_H

/*!
\file       thread_types.h
\brief      This is a header file for the Thread Types module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* Framework */
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "mac_abs_types.h"
#include "nvm_adapter.h"
#include "stack_manager_if.h"
#include "thread_cfg.h"
#include "event_manager.h"
#include "ip6.h"

/*==================================================================================================
Public macros
==================================================================================================*/
/*! Thread protocol version */

#define THR_PROTOCOL_VERSION_1_1            2
#define THR_PROTOCOL_VERSION                THR_PROTOCOL_VERSION_1_1

/*! Thread Enterprise number */
#define THREAD_ENTERPRISE_NUMBER            44970U

/*! Thread Enterprise number */
#define THREAD_ENTERPRISE_NUMBER_ARRAY      {0x00, 0x00, 0xAF, 0xAA}

/*! Thread Enterprise number */
#define NXP_ENTERPRISE_NUMBER               28137U

/*! NXP Enterprise number */
#define NXP_ENTERPRISE_NUMBER_ARRAY         {0x00, 0x00, 0x6D, 0xE9}

#define THREAD_SERVICE_DATA_BBR             0x01

/*! Supported Service Type IDs */
#define THREAD_DNS_SERVICE_TYPE_ID          0
#define THREAD_OTA_SERVICE_TYPE_ID          1

/*! Maximum Router ID */
#define THR_MAX_ROUTER_ID                   62

/* update bit usage accordingly */
#define THR_ROUTER_BITS_SIZE                6
#define THR_CHILD_BITS_SIZE                 9
#define THR_RSV_BITS_SIZE                   1
#define THR_MAX_ADV_ROUTE_COST              15

/*! Thread Six Low Pan context IDs */
#define SLWP_CID_MLEID              0

/*! Maximum number of thread routers */
#define THR_MAX_POSSIBLE_ROUTERS          (THR_MAX_ROUTER_ID + 1)
/*! The maximum bytes of the router mask */
#define THR_ROUTER_MASK_BYTES    (THR_MAX_POSSIBLE_ROUTERS/8) + ((THR_MAX_POSSIBLE_ROUTERS%8) != 0)
/*! The maximum child ID */
#define THR_MAX_CHILD_IDS         (1<<THR_CHILD_BITS_SIZE) / 32

/*! Thread Router Id <-> Short address conversion */
#define THR_R_ID_ADDR_SHIFT         (16U-THR_ROUTER_BITS_SIZE)
#define THR_R_ID_TO_SHORT_ADDR(x)   (x << THR_R_ID_ADDR_SHIFT)
#define THR_SHORT_ADDR_TO_R_ID(x)   (x >> THR_R_ID_ADDR_SHIFT)

#define ROUTER_ID_MASK_BYTE         (THR_ALL_FFs8 <<(8 - THR_ROUTER_BITS_SIZE))
#define ROUTER_ID_MASK              ((uint16_t)(0xFF00 << (8 - THR_ROUTER_BITS_SIZE)))
#define CHILD_ID_MASK               (THR_ALL_FFs16 >> (THR_ROUTER_BITS_SIZE + THR_RSV_BITS_SIZE))

/*! Macro for determining the address of a parent based on the child short. Input: child short
    address (uin16_t) */
#define THR_GET_MY_PARENT(chidShortAddr) ((chidShortAddr) & (~CHILD_ID_MASK))

#define THR_IS_ROUTER(x) (0U == ((x) &  (~ROUTER_ID_MASK)))
#define THR_IS_END_DEVICE(x) (FALSE == THR_IS_ROUTER(x))

/*! Macro for determining if an node is the devices child. Input: node short address (uin16_t)
    parent short address (uint16_t) */
#define THR_IS_MY_CHILD(childShortAddr, parentShortAddr) ((THR_IS_END_DEVICE(childShortAddr)) && ((parentShortAddr) == THR_GET_MY_PARENT(childShortAddr)))

#define THR_IS_MY_PARENT(childShortAddr, parentShortAddr) THR_IS_MY_CHILD(childShortAddr, parentShortAddr)

#define THR_IS_ROUTER(x) (0U == ((x) &  (~ROUTER_ID_MASK)))

/*! Check if the router ID is set in the mask */
#define THR_R_ID_IS_SET_IN_MASK(mask,rId)    ((mask[(rId)/8]) & (0x80 >> ((rId)%8)))

/*! The Network key size */
#define THR_NWK_KEY_SIZE                    16

#define THR_BEACON_XPAN_DEFAULT_VALUE       THR_ALL_FFs64

/*! Permit join flag mask and offset */
#define THR_BEACON_J_FLAG_MASK              0x01U
#define THR_BEACON_J_FLAG_OFFSET            0U
/*! Native commissioner flag mask and offset */
#define THR_BEACON_N_FLAG_MASK              0x08U
#define THR_BEACON_N_FLAG_OFFSET            3U
/*! Beacon Version mask and offset */
#define THR_BEACON_VERSION_MASK             0xF0U
#define THR_BEACON_VERSION_OFFSET           4U

/*! Thread Beacon Permit Join Flag Macros */
#define THR_BEACON_J_FLAG_GET(byte)         (((byte) & THR_BEACON_J_FLAG_MASK) >> THR_BEACON_J_FLAG_OFFSET)
#define THR_BEACON_J_FLAG_SET(byte, flag)   ((byte) = (((byte) & (~THR_BEACON_J_FLAG_MASK)) | ((flag) << THR_BEACON_J_FLAG_OFFSET )))

/*! Thread Beacon Native Commissioner Flag Macros */
#define THR_BEACON_N_FLAG_GET(byte)         (((byte) & THR_BEACON_N_FLAG_MASK) >> THR_BEACON_N_FLAG_OFFSET)
#define THR_BEACON_N_FLAG_SET(byte, flag)   ((byte) = (((byte) & (~THR_BEACON_N_FLAG_MASK)) | ((flag) << THR_BEACON_N_FLAG_OFFSET )))

/*! Thread Beacon Permit Join Flag Macros */
#define THR_BEACON_VERSION_GET(byte)        (((byte) & THR_BEACON_VERSION_MASK) >> THR_BEACON_VERSION_OFFSET)
#define THR_BEACON_VERSION_SET(byte, flag)  ((byte) = (((byte) & (~THR_BEACON_VERSION_MASK)) | ((flag) << THR_BEACON_VERSION_OFFSET )))

/*! Thread Discovery Request/Response TLV bits */
/*! Joiner Flag bit in the byte*/
#define THR_DISCOVERY_REQ_TLV_J_BIT         0x08

/*! Native Commissioner bit in the byte */
#define THR_DISCOVERY_RESP_TLV_N_BIT        0x04

/*! Thread Discovery Request/Response bits and version */
#define THR_DISC_RSP_VER_SHIFT              (4)
#define THR_DISC_RSP_VER_SET(byte, ver)     byte |= (ver) << THR_DISC_RSP_VER_SHIFT
#define THR_DISC_RSP_VER_GET(byte)          ((byte) >> THR_DISC_RSP_VER_SHIFT)

#define THR_DISC_RSP_N_SHIFT                (3)
#define THR_DISC_RSP_N_SET(byte, val)       byte |= (val) << THR_DISC_RSP_N_SHIFT
#define THR_DISC_RSP_N_GET(byte)            (((byte) & (1 << THR_DISC_RSP_N_SHIFT)) >> THR_DISC_RSP_N_SHIFT)

#define THR_DISC_RSP_C_SHIFT                (2)
#define THR_DISC_RSP_C_SET(byte, val)       byte |= (val) << THR_DISC_RSP_C_SHIFT
#define THR_DISC_RSP_C_GET(byte)            (((byte) & (1 << THR_DISC_RSP_C_SHIFT)) >> THR_DISC_RSP_C_SHIFT)

#define THR_DISC_REQ_VER_SHIFT              (4)
#define THR_DISC_REQ_VER_SET(byte, ver)     byte |= (ver) << THR_DISC_REQ_VER_SHIFT
#define THR_DISC_REQ_VER_GET(byte)          ((byte) >> THR_DISC_REQ_VER_SHIFT)

#define THR_DISC_REQ_J_SHIFT                (3)
#define THR_DISC_REQ_J_SET(byte, val)       byte |= (val) << THR_DISC_REQ_J_SHIFT
#define THR_DISC_REQ_J_GET(byte)            (((byte) & (1 << THR_DISC_REQ_J_SHIFT)) >> THR_DISC_REQ_J_SHIFT)

#define THR_SERVICE_DATA_MAX_LEN            10
#define THR_SERVER_DATA_MAX_LEN             16
#define THR_MAX_PSKC_LEN                    16

#define THR_PARENT_PRIORITY_OFFSET          6

#define THR_ML_PREFIX_LEN_BITS  (64U)

#define gUnusedValue_c gInvalidInstanceId_c
/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! Thread status */
typedef enum thrStatus_tag
{
    gThrStatus_Success_c = 0x00,
    gThrStatus_Failed_c = 0x01,
    gThrStatus_InvalidInstance_c = 0x02,
    gThrStatus_InvalidParam_c = 0x03,
    gThrStatus_NotPermitted_c = 0x04,
    gThrStatus_NotStarted_c = 0x05,
    gThrStatus_NoMem_c = 0x06,
    gThrStatus_UnsupportedAttr_c = 0x07,
    gThrStatus_EmptyEntry_c = 0x08,
    gThrStatus_InvalidValue_c = 0x09,
    gThrStatus_AlreadyConnected_c = 0x0A,
    gThrStatus_AlreadyCreated_c   = 0x0B,
    gThrStatus_NoTimers_c         = 0x0C,

    gThrStatus_EntryNotFound_c = THR_ALL_FFs8
} thrStatus_t;

/*! Device roles */
typedef enum thrInternalDeviceRole_tag
{
    gThrDevRole_Disconnected,   /*!< Device is disconnected */
    gThrDevRole_SED_c,          /*!< Sleepy End Device, no routing capability */
    gThrDevRole_MED_c,          /*!< Minimal End Device, no routing capability */
    gThrDevRole_FED_c,          /*!< Full End Device, address discovery and no routing capability */
    gThrDevRole_REED_c,         /*!< Router eligible end device (REED) */
    gThrDevRole_Router_c,       /*!< Router device */
    gThrDevRole_Leader_c        /*!< Leader device */
} thrInternalDeviceRole_t;

/*! Device roles */
typedef enum thrDeviceRole_tag
{
    gThrDeviceRole_SED_c,           /*!< Sleepy End Device, no routing capability */
    gThrDeviceRole_MED_c,           /*!< Minimal End Device, no routing capability */
    gThrDeviceRole_FED_c,           /*!< Full End Device, has routing capability */
    gThrDeviceRole_REED_c,          /*!< Router eligible end device (REED) */
} thrDeviceRole_t;

/*! Device types */
typedef enum thrDeviceType_tag
{
    gThrDevType_EndNode_c,       /*!< The node can be sleepy or non-sleepy end device (no routing capability )  */
    gThrDevType_ComboNode_c      /*!< The node can have any device role above */
} thrDeviceType_t;

/*! IP Address Types */
typedef enum nwkIPAddrType_tag
{
    gLL64Addr_c = 0x00,         /*!< Link-Local 64 address (the IID is MAC Extended address Which is not the factory-assigned IEEE EUI-64,)*/
    gMLEIDAddr_c = 0x01,        /*!< Mesh-Local Endpoint Identifier address (the IID is randmon) */
    gRLOCAddr_c = 0x02,         /*!< Routing Locator address (the IID encodes the Router and Child IDs.)*/
    gGUAAddr_c  = 0x03,         /*!< Global Unicast Address*/
    gAnycastAddr_c = 0x04,      /*!< Anycast IPv6 addresses */
    gDUAAddr_c = 0x05,          /*!< Domain Unicast Address */
    gAnyIpv6_c = 0x06,          /*!< All IPv6 address */
    gAllThreadNodes_c = 0x07,   /*!< All Thread nodes address */
} nwkIPAddrType_t;

/*! REED and route states */
typedef enum thrRouterState_tag
{
    gThrReedIdle_c,
    gThrReedReqRouterId_c,
    gThrReedReatachJitter_c,
    gThrReedReqRouterIdJitter_c,
    gThrRouterIdle_c,
    gThrRouterDownGrdIdJitter_c,
    gThrRouterDownGrd_c,
} thrRouterState_t;

/*! The Stateless Address Autoconfiguration (SLAAC) policy */
typedef enum thrSlaacPolicy_t
{
    gThrSlaacRandom_c,   /*!< the addresses is randomly generate */
    gThrSlaacManual_c,   /*!< it is provided by the application*/
    gThrSlaacMlIid_c,    /*!< use ML-EID address*/
    //gThrSlaacRfc7217_c,  /*!< Not implemented */
} thrSlaacPolicy_t;

/*! Thread Commissioner mode */
typedef enum thrCommissionerMode_tag
{
    gThrCommissionerModeDisabled_c = 0x00,  /*!< Commissioner disabled (normal thread node) */
    gThrCommissionerModeNative_c = 0x02,    /*!< Native (802.15.4) commissioner */
    gThrCommissionerModeEthernet_c = 0x04,  /*!<  Ethernet commissioner*/
    gThrCommissionerModeOnMesh_c = 0x08,    /*!< The commissioner is on mesh network.
                                              A thread node can become a commissioner at run time */
    gThrCommissionerModeClosing_c = 0x10,   /*!< The Commissioner is in closing mode */
} thrCommissionerMode_t;

/*! parent priority */
typedef enum
{
    gThrRouterPriorityMed_c = 0x00U,        /* default */
    gThrRouterPriorityHigh_c = 0x01U,       /* high */
    gThrRouterPriorityRsvd_c = 0x02U,   /* reserved - must not be sent */
    gThrRouterPriorityLow_c_c = 0x03U,      /* low */
} thrParentPriority_e;

/*! Specific octet string type, 16 bytes */
typedef struct thrOctet16_tag
{
    uint8_t length;
    uint8_t aStr[16];
} thrOctet16_t;

/*! Specific octet string type, 32 bytes */
typedef struct thrOctet32_tag
{
    uint8_t length;
    uint8_t aStr[32];
} thrOctet32_t;

/*! Specific octet string type, 64 bytes */
typedef struct thrOctet64_tag
{
    uint8_t length;
    uint8_t aStr[64];
} thrOctet64_t;

/*! ML prefix */
typedef struct thrPrefixAttr_tag
{
    ipAddr_t prefix;
    uint8_t prefixLenBits;
} thrPrefixAttr_t;

/*! Mac filtering neighbor data */
typedef struct macFilteringNeighborData_tag
{
    uint64_t  extendedAddress;
    uint16_t  shortAddress;
    uint8_t   linkIndicator;
    bool_t    blockNeighbor;
} macFilteringNeighborData_t;

/*! Thread Beacon Info */
typedef struct thrBeaconInfo_tag
{
    uint64_t address;               /*!<  MAC extended address */
    uint16_t panid;                 /*!< PAN ID */
    macAbsAddrModeType_t addrType;  /*!< MAC address type: short or long (usually long)*/
    uint8_t channel;                /*!< received on channel */
    uint8_t lqi;                    /*!< received Lqi*/
    uint8_t unused;
    instanceId_t slwpInstId;        /*!< 6lowpan instanec ID */
    uint32_t payloadSize;           /*!< beacon payload size*/
    struct
    {
        uint8_t protocolId; /*!< thread protocol ID */
        uint8_t flags;      /*!< the beacon flags*/
        uint8_t nwkName[16]; /*!< network name */
        uint8_t xpanId[8];   /*!< extended PAN ID */
        uint8_t aTlvs[]; /*!<  where beacon tlv starts*/
    } payload;
} thrBeaconInfo_t;

/*! Network Discovery Entry - Each entry represents a Thread network */
typedef struct thrNwkActiveScanEntry_tag
{
    uint16_t numOfRcvdBeacons;     /*!< number of received beacons on that channel  */
    uint16_t panid;                /*!< PAN ID */
    uint8_t  channel;              /*!< received channel */
    uint8_t  lqi;                  /*!< link quality indicator */
} thrNwkActiveScanEntry_t;

/*! Scan type structure */
typedef enum
{
    gThrNwkScan_EnergyDetect_c = 0x01,  /*!<Energy Detect only*/
    gThrNwkScan_ActiveScan_c   = 0x02,  /*!<Beacon request only */
    gThrNwkScan_BothScans_c    = 0x03   /*!<Energy detect and beacon request*/
} thrNwkScanType_t;

/*! This structure is used to perform a network scan */
typedef struct thrNwkScan_tag
{
    uint32_t     scanChannelsMask;       /*!< What channels to scan; 0x07FFF800 means all 16 channels are used (from 11 to 26) */
    thrNwkScanType_t scanType;           /*!<what scan should be performed : energy, active or both*/
    uint8_t      scanDuration;           /*!<This is an exponential scale,
                                            as seen in the 802.15.4 specification (Range:1 - 14) */
    uint16_t     maxThrNwkToDisc;        /*! The maximum the Thread network to be discovered */
} thrNwkScan_t;

/*! The Network scan results */
typedef struct thrNwkScanResults_tag
{
    thrNwkScan_t scanInfo;
    uint8_t      numOfEnergyDetectEntries;
    uint8_t     *pEnergyDetectList;     /*!<One byte for each channel. Only the channels from scanInfo.scanChannelsMask should be handled;
                                            the rest of the channels are zeros  */
    uint8_t      numOfNwkScanEntries;   /*!<Number of discovered network performing an active scan */
    thrNwkActiveScanEntry_t nwkScanList[]; /* where the network discovery list begins; Note that all these buffres shall be freed */
} thrNwkScanResults_t;

/*! Thread Neighbor */
typedef struct thrNeighbor_tag
{
    uint64_t              extendedAddress;       /*!< Extended Address */

    uint32_t              timestamp;             /*!< Last Time of Communication */
    uint32_t              timeoutSec;            /*!< Device Timeout value */

    uint16_t              shortAddress;          /*!< Short Address */
    uint8_t               inLinkMargin;          /*!< Link Margin of incoming frames from neighbor */
    uint8_t               outLinkQuality;        /*!< Link Quality of sent frames to neighbor */

    uint16_t              thrVersion;            /*!< Thread protocol version */
    uint8_t               mode;                  /*!< Device mode */
    uint8_t               attachMode;            /*!< Device mode at attach time */
    uint8_t               state;                 /*!< Device state */
    uint8_t               txFailure;             /*!< Number of consecutive transmission failures */
    uint8_t               mleReqCount;           /*!< Number of consecutive MLE Req trans sent */
} thrNeighbor_t;

/*! Handle Tracking Table Entry */
typedef struct handleTrackingTable_tag
{
    uint64_t                destAddr;       /*!< link layer address destination */
    uint8_t                 msduHandle;     /*!< message handle */
    macAbsAddrModeType_t    destAddrMode;   /*!< link layer address mode */
} handleTrackingTable_t;

/*! Thread ID Assignment set */
typedef struct thrIdAssignSet_tag
{
    uint32_t thrReuseTime;              /*!< time interval after which the ID can be reused */
    uint8_t thrOwnerEui[gLlayerAddrEui64_c];    /*!< link layer address of the ID owner */
} thrIdAssignSet_t;

/*! Leader Data TLV - Over the Air mapping structure */
typedef struct mleOtaTlvLeaderData_tag
{
    uint8_t type;                           /*!< TLV Type */
    uint8_t length;                         /*!< Length of Leader Data TLV */
    uint8_t partitionId[4];                 /*!< Network Segment Identifier */
    uint8_t weighting;                      /*!< Weighting value for the network fragment */
    uint8_t dataVersion;                    /*!< Version of the Network Data */
    uint8_t stableDataVersion;              /*!< Stable Version of the Network Data */
    uint8_t leaderId;                       /*!< Network Leader Router ID */
} mleOtaTlvLeaderData_t;

/*! External Route Set */
typedef struct externalRouteSet_tag
{
    uint16_t brShortAddr;    /*!< Border Router short address */
    uint8_t  hasRouteFlags;  /*!< Border Router external route flags (Value of R_preference)*/
    uint8_t  brPrefixIndex;  /*!< Border Route prefix index */
    uint8_t  brDomainId;     /*!< Domain ID */
    uint8_t  brLifetime[4];  /*!< Entry lifetime */
    bool_t   isStable;       /*!< TRUE - if prefix is valid more than THR_NWK_DATA_MIN_STABLE_LIFETIME_SEC
                                  FALSE - otherwise */
    bool_t   bAdvertised;    /*!< TRUE - Prefix was advertised in the Thread network,
                                  FALSE - otherwise */
} externalRouteSet_t;

/*! Border router (BR) Set */
typedef struct borderRouterSet_tag
{
    uint16_t brShortAddr;       /*!< Border Router short address */
    uint8_t  brPrefixFlags[2];  /*!< Byte 0: BR Flags; Byte 1: Bits 0-6 Reserved, 7 ND_DNS bit; */
    uint8_t  brPrefixIndex;     /*!< Border Route prefix index */
    uint8_t  brDomainId;        /*!< Domain ID */
    uint8_t  brLifetime[4];     /*!< Entry lifetime */
    bool_t   bIsStable;         /*!< TRUE - if prefix is valid more than THR_NWK_DATA_MIN_STABLE_LIFETIME_SEC,
                                     FALSE - otherwise */
    bool_t   bAdvertised;       /*!< TRUE - Prefix was advertised in the Thread network,
                                     FALSE - otherwise */
} borderRouterSet_t;

/*! Context Id Set */
typedef struct contextIdSet_tag
{
    uint8_t  contextFlags;          /* Bits 7-5: Reserved
                                     * Bits   4: CID Compress
                                     * Bits 3-0: CID
                                     */
    uint8_t  contextLength;         /*!< Length of context address */
    uint8_t  contextPrefixIndex;    /*!< Prefix index corresponding to context */
    bool_t   isStable;              /*!< TRUE - if prefix is valid more than THR_NWK_DATA_MIN_STABLE_LIFETIME_SEC,
                                     FALSE - otherwise */
    uint32_t removeTstamp;          /*!< Timestamp after which context can be used only for decompression */
} contextIdSet_t;

typedef enum nwkDataServerFlags_tag
{
    gNwkDataUnusedServer_c = 0x00,
    gNwkDataLocalServer_c = 0x01,
    gNwkDataRemoteServer_c = 0x02
} nwkDataServerFlags_t;

typedef uint8_t nwkDataServerStatus_t;

typedef struct serverTlv_tag
{
    uint8_t sServer16[2];                           /*!< Server's short address */
    uint8_t sDataLen;                               /*!< Length of service data */
    uint8_t sServerData[THR_SERVER_DATA_MAX_LEN];   /*!< Service data */
    nwkDataServerStatus_t flags;                    /*!< Whether the server is unused, local or remote */
} serverTlv_t;

typedef struct serviceSet_tag
{
    uint8_t sFlags;                                             /*!< Service flags */
    uint8_t sEntNb[4];                                          /*!< Enterprise number */
    uint8_t sDataLen;                                           /*!< Service Data length */
    uint8_t sData[THR_SERVICE_DATA_MAX_LEN];                    /*!< Service data */
    serverTlv_t sServers[THR_SERVICE_DATA_MAX_SERVER_SUBTLVS];  /*!< Server TLV */
    bool_t bLocalService;                                       /*!< Whether the service is advertised by us */
    bool_t bIsStable;                                           /*!< Whether the service is stable */
} serviceSet_t;

/*! Child Version Number Set */
typedef struct childVersNbSet_tag
{
    uint16_t        childShortAddr;        /*!< Child short address */
    bool_t          childStableOnly;       /*!< TRUE - Child requires only stable data,
                                                FALSE - otherwise */
    uint8_t         childVersion;          /*!< Child's version of network data */
    uint32_t        childRetTstamp;        /*!< Child's retry timestamp */
} childVersNbSet_t;

/*! Server Data */
typedef struct serverData_tag
{
    ipAddr_t           *pPrefixTbl;     /*!< Pointer to Prefix in Prefix Table */
    uint8_t            *pPrefixLenTbl;  /*!< Pointer to Prefix length */
    externalRouteSet_t *pExtRouteTbl;   /*!< Pointer to External Route */
    borderRouterSet_t  *pBRSetTbl;      /*!< Pointer to External Route Set */
} serverData_t;

/*! Thread Network Data Structure */
typedef struct nwkDataInterfaceSet_tag
{
    ipAddr_t             *pPrefixTbl;           /*!< Pointer to Prefix Table */
    uint8_t              *pPrefixLenTbl;        /*!< Pointer to Prefix Length Table */

    childVersNbSet_t     *pChildVersNbSet;      /*!< Pointer to Children Version Number Set */
    borderRouterSet_t    *pBRSetTbl;            /*!< Pointer to Valid Prefix (Border Router) Set */
    externalRouteSet_t   *pExtRouteTbl;         /*!< Pointer to External Route Set */
    contextIdSet_t       *pContextTbl;          /*!< Pointer to 6LoWPAN Context ID Set */
    serviceSet_t         *pServiceSetTbl;       /*!< Pointer to Service Set */

    serverData_t          serverData;           /*!< Server Data */
    mleOtaTlvLeaderData_t leaderData;           /*!< Leader Data TLV */
} nwkDataInterfaceSet_t;

typedef struct thrLqCacheEntry_tag
{
    ipAddr_t eid;               /*!< IP Address */
    uint16_t address16;         /*!< Short address */
    uint8_t discoveryTimeout;   /*!< The time remaining for waiting for responses to an Address Query,
                                or zero if there is no outstanding Address Query*/
    uint8_t discoveryFail;      /*!< The number of consecutive Address Query messages for which no
                            corresponding response was received before discoveryTimeout expires*/
    uint32_t retryTimeout;      /*!< The time a device must wait before sending another Address Query message.*/
    uint32_t ageSec;            /*!< Last usage of cache entry */
} thrLqCacheEntry_t;

typedef struct thrAqInterfaceSet_tag
{
    ipPktInfo_t *pIpPktInfoBuffer;      /*!< Pointer to the outstanding IP packet */
    tmrTimerID_t addrDiscTimer;         /*!< Timer ID for address discovery */
    uint32_t minClientLastTransaTime;   /*!< Last transaction time */
    uint8_t  mAddrNotifyMlEid[8];       /*!< Mesh Local EID */
} thrAqInterfaceSet_t;

/*! Thread sleepy child ID table entry */
typedef struct thrAddrRegEntry_tag
{
    uint8_t contextId;  /*!< Context ID*/
    uint8_t addrIid[8]; /*!< Interface identifier */
} thrAddrRegEntry_t;

/*! Thread RFD child address registration table */
typedef struct thrChildAddrRegEntry_tag
{
    uint8_t neighborIdx; /*!< Entry in neighbor table */
    thrAddrRegEntry_t childAddrEntry[THR_CHILD_ADDR_REG_ENTIRES]; /*!< Registered IID */
    ipAddr_t multicastAddr[THR_CHILD_MCAST_ADDR_REG_ENTIRES]; /* Registered Multicast address */
} thrChildAddrRegEntry_t;


/*! Thread routing Link set */
typedef struct thrLinkSet_tag
{
    uint32_t thrLinkAge;
    uint16_t thrShortAddr;
    uint8_t thrLinkMargin;
    uint8_t thrOutgoingQual;
} thrLinkSet_t;

/*! Thread routing Route set */
typedef struct thrRouteSet_tag
{
    uint16_t thrMultiHopRouterId;
    uint16_t thrNextHopRouterId;
    uint8_t thrMultihopRouteCost;
    uint8_t thrRouteStatus;
} thrRouteSet_t;

/*! Thread routing Router ID set */
typedef struct thrRouterIdSet_tag
{
    uint8_t thrIdSeqNb; /*!< Sequence number */
    uint8_t thrIdSet[THR_ROUTER_MASK_BYTES];    /*!< Router ID Set */
} thrRouterIdSet_t;

/*! Structure with all Thread routing parameters for an interface */
typedef struct thrInterfaceSet_tag
{
    thrRouteSet_t *pThreadRoutingTbl;   /*!< Pointer to Routing Table */
    thrLinkSet_t  *pThreadLinkSet;      /*!< Pointer to link set */

    uint32_t dgradeTstamp;              /*!<Timestamp for when downgrading to REED */

    uint16_t deviceShortAddr;           /*!< Device's short address */
    tmrTimerID_t singleShotTmrId;       /*!< Timer id for single shot operations */
    tmrTimerID_t periodicTmrId;         /*!< Timer id for periodic operations */

    uint8_t thrRouterCount;             /*!< Number of routers in network */
    bool_t bIsLeader;                   /*!< TRUE - if device is Leader, FALSE - otherwise */
    bool_t bIsInit;                     /*!< TRUE - if is initialized, FALSE - otherwise */
    thrRouterState_t devState;          /*!< Device's state */
    uint8_t leaderCost;                 /*!< Route cost to Leader */

    thrRouterIdSet_t threadRouterIdSet; /*!< Router ID Set */
} thrInterfaceSet_t;

/*! Mac Key Index */
typedef struct thrMacRcvdDiffKeyIndexInd_tag
{
    instanceId_t            macInstId; /*!< MAC instance ID */
    uint8_t                 keyIdMode; /*!< Key ID mode */
    uint8_t                 keyIndex;  /*!< Key index*/
} thrMacRcvdDiffKeyIndexInd_t;

/*!  Thread event data */
typedef union thrEventData_tag
{
    thrNwkScanResults_t         nwkScanCnf;                   /*!< network scan confirm - result */
    thrMacRcvdDiffKeyIndexInd_t thrMacRcvdDiffKeyIndexInd;    /*!< the MAC received a different key index data*/
    thrBeaconInfo_t             nwkSelectParentsInd;          /*!< network select parents indication */
} thrEventData_t;

/*! Thread event code */
typedef uint32_t thrEvCode_t;

/*! Thread event parameters header */
typedef struct thrEvmParams_tag
{
    thrEvCode_t code;               /*!< Event Code */
    uint16_t eventDataSize;         /*!< Event Data Size */
    uint16_t thrInstId;             /*!< Instance Id */
    uint32_t id;                    /*!< Identifier for this event (used in multicore events) */
    thrEventData_t *pEventData;     /*!< pointer to event data */
} thrEvmParams_t;


/*! reset CPU status enum */
typedef enum resetCpuStatus_tag
{
    gResetCpuSuccess_c = 0x00,
    gResetCpuPending_c
} resetCpuStatus_t;

/*! Structure used to specify input parameters for PSKc generation */
typedef struct thrPskcInputParams_tag
{
    uint8_t *pPskcStr;  /*!< PSKc string */
    uint32_t pskcStrLen;/*!< PSKc string length */
    uint8_t *pXpanId;   /*!< Extended PAN ID */
    uint8_t *pNwkName;  /*!< Network name */
    uint32_t nwkNameLen;/*!< Network name length*/
    uint8_t *pPskcOut;  /*!< PSKc */
} thrPskcInputParams_t;

typedef enum meshcopSteeringMatch_tag
{
    gMeshcopSteeringMatchNA_c = 0x00,       /*!< Matching not performed */
    gMeshcopSteeringMatchNone_c = 0x01,     /*!< No matching */
    gMeshcopSteeringMatchFfs_c = 0x02,      /*!< Matched a 0xFF mask */
    gMeshcopSteeringMatchSpecific_c = 0x04  /*!< Matched specific bits */
} meshcopSteeringMatch_t;

typedef struct thrNwkJoiningEntry_tag
{
    uint8_t  euiAddr[8];    /*!< Link layer address */
    uint8_t aXpanId[8];     /*!< Extended PAN ID*/
    uint8_t  channel;       /*!< Channel */
    meshcopSteeringMatch_t steeringMatch; /*!< Steering Data match */
    uint16_t panId;     /*!< PAN ID */
    uint16_t  joinerUDPPort; /*!< if not used, it will be set to 0x0000 */
    uint16_t  commissionerUDPPort; /*!< if not used, it will be set to 0x0000 */
} thrNwkJoiningEntry_t;


/*! Thread Event Sets */
typedef enum
{
    gThrEvSet_NwkScan_c          = 0x00010000 | gEvmModuleThread_c, /*!< network scan event set */
    gThrEvSet_NwkCreate_c        = 0x00020000 | gEvmModuleThread_c, /*!< network create event set */
    gThrEvSet_NwkJoin_c          = 0x00030000 | gEvmModuleThread_c, /*!< network join event set */
    gThrEvSet_NwkSelectParents_c = 0x00040000 | gEvmModuleThread_c, /*!< network select parent event set */
    gThrEvSet_NwkGeneral_c       = 0x00050000 | gEvmModuleThread_c, /*!< network general event set */
    gThrEvSet_NwkCommissioning_c = 0x00060000 | gEvmModuleThread_c  /*!< network commissioning event set */
} thrEvSets_t;

/*! Thread Discovery method */
typedef enum thrJoinDiscoveryMethod_tag
{
    gUseMACBeacon_c,  /*!< use MAC beacons for discovery  */
    gUseThreadDiscovery_c /*!< use Thread Discovery request */
} thrJoinDiscoveryMethod_t;

/*! Discovery Request TX options*/
typedef enum thrDiscReqTxOptions_tag
{
    gThrNoSecurityAtMacLevel_c, /*!< no security is used at Mac level */
    gThrEncryptedAtMacLevel_c  /*!< encrypted with the well-known key and Extended Address at Mac level */
} thrDiscReqTxOptions_t;

/*! Discovery Request TX parameters */
typedef struct thrNwkDiscoveryReqTxOpt_tag
{
    thrDiscReqTxOptions_t discReqTxOpt;
    uint32_t scanChannelMask; /*!< the scan channel mask (0x07FFF800 means all 16 channels are used ). */
    uint16_t destPanId; /*!<  destination PAN ID (it can be 0xFFFF or a specific PAN ID) */
    uint8_t  flags; /*!<  flags from Discovery Request TLV: THR_DISCOVERY_REQ_TLV_J_BIT or zero.
                          Note that the Protocol Version will be always added  */
    uint8_t  extraTlvsLength; /*!<  extra TLV length. More TLVs can be added in the payload
                                   (eg extended pan ID, application specific TLVs). Maximum 70 bytes.*/
    uint8_t *pExtraTlvs;  /*!< pointer to extra TLV */
} thrNwkDiscoveryReqTxOpt_t;

/*! The announce events used by the thrAnnounceCb_t callback */
typedef enum thrAnnounceEvent_tag
{
    gThrSearchThreadNwkStarted_c,
    gThrAnnounceRespRcv_c,
    gThrSearchThreadNwkStopped_c
} thrAnnounceEvent_t;

/*! The announcement callback */
typedef void (*thrAnnounceCb_t)(instanceId_t thrInstId,
                                thrAnnounceEvent_t event, uint8_t lqi, uint16_t tlvsSize,
                                uint8_t *pTlvs);

/*! Thread Instance search type */
typedef enum thrInstSearchType_tag
{
    gThrIfUniqueIdSearch_c = 0x00,
    gThrSlwpInstSearch_c,
    gThrMacInstIdSearch_c,
    gThrInstSearch_c,
    gThrIfHandleSearch_c
} thrInstSearchType_t;

/*! Thread Proxy group element */
typedef struct thrMcastFwTblEntry_tag
{
    ip6McastFwTblEntry_t *mcastEntry;
    uint32_t timeoutSec;
}thrMcastFwTblEntry_t;

/*!  */
typedef struct thrMcastKeepAliveEntry_tag
{
    ipAddr_t* mcastAddr;
    uint32_t  updateTstamp;
}thrMcastKeepAliveEntry_t;

typedef enum thrMcastRegStatus_tag
{
    thrMCastRegStatusSuccess_c         = 0x00,
    thrMCastRegStatusInvalidIpAddr_c      = 0x02,
    thrMCastRegStatusMissResources_c   = 0x04,
    thrMCastRegStatusBBRNotPrimary     = 0x05,
    thrMCastRegStatusGeneralFailure_c  = 0x06
} thrMcastRegStatus_t;

typedef enum thrQueryType_tag
{
    thrQueryTypeMesh_c             = 0x00,
    thrQueryTypeBbrDad_c           = 0x01,
    thrQueryTypeBbrTargetDisc_c    = 0x02,
    thrQueryTypeBbrDisc_c          = 0x03,
} thrQueryType_t;

typedef struct thrAddrQueryListEntry_tag
{
    ipAddr_t    targetAddr;
    ipAddr_t    sourceAddr;
    uint64_t    expirationTstamp;
    uint8_t     mlEid[8];
    instanceId_t thrInstanceId;
    ipPktInfo_t *pIpPktInfo;
    uint32_t    timeSLTrans;
    thrQueryType_t  queryType;
    uint8_t  retryCount;
} thrAddrQueryListEntry_t;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/

/*================================================================================================*/
#endif  /* _THREAD_TYPES_H */
/*!
** @}
*/
