/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!=================================================================================================
\file       thci.c
\brief      This is a public source file for the Thread Host Control Interface (THCI) module.
            It contains the implementation of the THCI commands and events.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "app_stack_config.h"
#include "app_thread_config.h"
#include "stack_config.h"

#include "EmbeddedTypes.h"
#include <string.h>
#include "board.h"
#include "app_led.h"

#include "ip_if_serial_tun.h"
#include "serial_tun_driver.h"

#include "Panic.h"
#include "FunctionLib.h"
#include "SerialManager.h"
#include "FsciInterface.h"

#include "network_utils.h"
#include "sockets.h"
#include "ip_if_management.h"
#include "ip6.h"
#include "icmp.h"
#include "sixlowpan.h"
#include "thci.h"
#include "event_manager.h"

#include "dtls.h"
#include "app_init.h"

#include "mac_filtering.h"

#include "thread_attributes.h"
#include "thread_app_callbacks.h"
#include "thread_utils.h"
#include "thread_meshcop.h"
#include "thread_meshcop_mgmt.h"
#include "thread_network.h"
#include "thread_border_router.h"

#if DNS_ENABLED
    #include "dns_client.h"
#endif

#if THR_ENABLE_EVENT_MONITORING
    #include "app_event_monitoring.h"
#endif

#if THREAD_USE_THCI

#if UDP_ECHO_PROTOCOL
    #include "app_echo_udp.h"
#endif

#if THR_ENABLE_MGMT_DIAGNOSTICS
    #include "thread_mgmt.h"
#endif

#if USB_RNDIS_IF
    #include "ip_if_usbenet.h"
#endif

#if (gFsciIncluded_c == FALSE)
    #error "*** ERROR: FSCI module is not enabled - check app_framework_config.h"
#endif

/*==================================================================================================
Private macros
==================================================================================================*/

#define TLV_HEADER_SIZE             (2U)
#define PING_ID                     (1U)
#define PING_SEQ_NB                 (1U)
#define PING_PAYLOAD_DEFAULT_SIZE   (32U)
#define PING_PAYLOAD_START          'a'
#define PING_PAYLOAD_END            'w'
#define SHELL_CMD_MAX_ARGS          (10U)
#define PING_HEADER_SIZE            (4U)
#define DEFAULT_TIMEOUT             (2000U)
#define SHELL_PING_MIN_TIMEOUT      (2000U)

#define THCI_COAP_MAX_URI_PATHS     (0x5U)
#define THCI_COAP_MAX_URI_PATH_OPT_SIZE (30U)
/*==================================================================================================
Private type definitions
==================================================================================================*/
typedef enum ztFSCIrorCode_tag
{
    mTHCI_Ok_c                      = 0x00U,
    mTHCI_NoSpace_c                 = 0x01U,
    mTHCI_PingTimeout_c             = 0x02U,
    mTHCI_PingWrongDestAddress_c    = 0x03U,
    mTHCI_Err_c                     = 0xFFU
} thciErrorCode_t;

/* device info extended capabilities bit map*/
typedef enum thciExtCapBitMap_tag
{
    gThciIsBr_c                    = 0x02U,
    gThciIsBrNdRouter_c            = 0x08U,
} thciExtCapBitMap_t;

typedef struct thciGenericMsg_tag
{
    void *pData;
    uint32_t interfaceId;
} thciGenericMsg_t;

typedef struct thciTlv_tag
{
    uint8_t tlvType;    /*!< Type */
    uint8_t count;      /*!< Number of sub-TLVs or number of bytes following */
    uint8_t pData[];    /*!< Value */
} thciTlv_t;

typedef struct sockSocket_tag
{
    uint8_t sockId;
} sockSocket_t;

typedef struct sockAccept_tag
{
    uint8_t status;
    uint8_t sock;
} sockAccept_t;

typedef struct sockGetOpt_tag
{
    uint8_t status;
    uint8_t optValue[4];
} sockGetOpt_t;

typedef struct pingReply_tag
{
    uint8_t status;
    uint8_t reply[2];
} pingReply_t;

typedef struct thciDtlsConnectConfirm_tag
{
    uint8_t status;
    uint8_t peerIdx;
} thciDtlsConnectConfirm_t;

typedef struct thciDataConfirm_tag
{
    uint8_t peerIndex;
    uint8_t size[2];
    uint8_t data[];
} dtlsDataConfirm_t;

typedef struct thciGetExpectedJoinerConfirm_tag
{
    uint8_t status;
    uint8_t selected;
    uint8_t pskLen;
    uint8_t psk[];
} thciGetExpectedJoinerConfirm_t;


/* Structures for casting incoming data */
typedef struct sockParams_tag
{
    uint8_t domain;
    uint8_t type;
    uint8_t protocol;
} sockParams_t;

typedef struct shutdownParams_tag
{
    uint8_t sock;
} shutdownParams_t;

typedef struct bindParams_tag
{
    uint8_t sock;
    uint8_t ipAddr[16];
    uint8_t port[2];
    uint8_t family;
} bindParams_t;

typedef struct sendParams_tag
{
    uint8_t sock;
    uint8_t flags;
    uint8_t size[2];
    uint8_t data[1];
} sendParams_t;

typedef struct sendtoParams_tag
{
    uint8_t sock;
    uint8_t flags;
    uint8_t bSecured;
    uint8_t size[2];
    uint8_t port[2];
    uint8_t ipAddress[16];
    uint8_t data[];
} sendtoParams_t;

typedef struct recvParams_tag
{
    uint8_t sock;
    uint8_t size[2];
    uint8_t flags;
} recvParams_t;

typedef struct connectParams_tag
{
    uint8_t sock;
    uint8_t ipAddr[16];
    uint8_t port[2];
    uint8_t family;
} connectParams_t;

typedef struct listenParams_tag
{
    uint8_t sock;
    uint8_t backlog;
} listenParams_t;

typedef struct acceptParams_tag
{
    uint8_t sock;
} acceptParams_t;

typedef struct setOptParams_tag
{
    uint8_t sock;
    uint8_t level;
    uint16_t optName;
    uint8_t optVal[4];
} setOptParams_t;

typedef struct getOptParams_tag
{
    uint8_t sock;
    uint8_t level;
    uint16_t optName;
} getOptParams_t;

typedef struct ifconfigBindParams_tag
{
    ipAddr_t ipAddr;    /*!< IP address */
    uint8_t ifId;       /*!< Interface ID */
} ifconfigBindParams_t;

typedef struct pingParams_tag
{
    ipAddr_t destIpAddr;    /*!< Destination IP address */
    ipAddr_t sourceIpAddr;  /*!< Source IP address */
    uint8_t length[2];      /*!< Size of the ping packet payload */
    uint8_t timeout[2];     /*!< Timeout */
    uint8_t security;       /*!< Security Enabled */
    uint8_t hopLimit;       /*!< Hop Limit */
    //uint8_t count;        /*!< Number of iterations */
} pingParams_t;

typedef struct echoUdpParams_tag
{
    ipAddr_t destIpAddr;    /*!< Destination IP address */
    ipAddr_t sourceIpAddr;  /*!< Source IP address */
    uint8_t length[2];      /*!< Size of the ping packet payload */
    uint8_t timeout[2];     /*!< Timeout */
    uint8_t count;          /*!< Number of iterations */
} echoUdpParams_t;


/* DTLS structures */
typedef struct dtlsOpenParams_tag
{
    uint8_t maxRetrCount;
    uint8_t timeout[2];
    uint8_t port[2];
} dtlsOpenParams_t;

typedef struct dtlsCloseContextParams_tag
{
    uint8_t contextNumber;
} dtlsCloseContextParams_t;

typedef struct dtlsClosePeerParams_tag
{
    uint8_t peerNumber;
} dtlsClosePeerParams_t;

typedef struct dtlsConnectParams_tag
{
    uint8_t contextNumber;
    uint8_t ipAddr[16];
    uint8_t port[2];
} dtlsConnectParams_t;

typedef struct dtlsSendParams_tag
{
    uint8_t peerNumber;
    uint8_t size[2];
    uint8_t data[];
} dtlsSendParams_t;

typedef struct meshCopEuiParams_tag
{
    uint8_t instanceId;
    uint8_t selected;
    uint8_t euiType; /* unused -> needs to be removed */
    uint8_t aEui[8];
    uint8_t pskLen;   /* password size */
    uint8_t psk[];  /* password */
} meshCopEuiParams_t;

typedef struct meshCopEuiGetParams_tag
{
    uint8_t instanceId;
    uint8_t type;   /* EUI type */
    uint8_t aEui[8];
} meshCopEuiGetParams_t;

/* instance list */
typedef struct instanceList_tag
{
    uint8_t instanceType;
    uint8_t instanceId;
} instanceList_t;

/* network config
     - get instances Rsp */
typedef struct getInstancesRsp_tag
{
    uint8_t noOfInstances;
    instanceList_t instanceList[1];
} getInstancesRsp_t;

/* network config
     - set attr request */
typedef struct setAttrReq_tag
{
    uint8_t instanceId;
    uint8_t attrId;
    uint8_t index;
    uint8_t size;
    uint8_t attrValue[1];
} setAttrReq_t;

/* network config
     - get attr request */
typedef struct getAttrReq_tag
{
    uint8_t instanceId;
    uint8_t attrId;
    uint8_t index;
} getAttrReq_t;

/* network config
     - get attr response */
typedef struct getAttrRsp_tag
{
    uint8_t instanceId;
    uint8_t attrId;
    uint8_t index;
    uint8_t status;
    uint8_t size;
    uint8_t attrValue[1];
} getAttrRsp_t;

/* network config
     - get dev info request */
typedef struct getDevInfoReq_tag
{
    uint8_t instanceId;
} getDevInfoReq_t;

/* network config
     - get dev info response */
typedef struct getDevInfoRsp_tag
{
    uint8_t instanceId;
    uint8_t nwkCapabilities;
    uint8_t extendedCapabilities;
} getDevInfoRsp_t;

/* network config
    - add to mac filter */
typedef struct addMacFilter_tag
{
    uint8_t instanceId;
    uint8_t extendedAddr[8];
    uint8_t shortAddr[2];
    uint8_t linkIndicator;
    uint8_t blockNeighbor;
} addMacFilter_t;

/* network config
    - get mac filter table request */
typedef struct getTableReq_tag
{
    uint8_t instanceId;
    uint8_t startIndex;
    uint8_t noOfElements;
} getTableReq_t;

typedef struct getNeighborInfoReq_tag
{
    uint8_t instanceId;
    uint8_t shortAddr[2];
} getNeighborInfoReq_t;

typedef struct getNeighborInfoRsp_tag
{
    uint8_t status;
    uint8_t extendedAddress[8];       /*!< Extended Address */
    uint8_t shortAddress[2];          /*!< Short Address */
    uint8_t timestamp[4];             /*!< Last Time of Communication */
    uint8_t inRssi;                   /*!< Rssi of incoming frames from neighbor */
    uint8_t timeoutSec[4];            /*!< Device Timeout value */
} getNeighborInfoRsp_t;

/* network config
    - get mac filter table response */
typedef struct getTableRsp_tag
{
    uint8_t instanceId;
    uint8_t noOfElements;
    uint8_t pTblEntry[1]; /* variable data containing: extendedAddr, shortAddr, linkIndicator */
} getTableRsp_t;


typedef struct addMcastGrpManageReq_tag
{
    uint8_t interfaceId;
    uint8_t manageReqType;
    uint8_t bUseTimeout;
    uint8_t mcastAddrCount;
    uint8_t timeout[4];
    uint8_t dstIpAddr[16];
    uint8_t mcastAddr[16];
} addMcastGrpManageReq_t;

typedef struct thrNeighborTbl_tag
{
    uint8_t extendedAddr[8];
    uint8_t shortAddr[2];
    uint8_t lastCommTime[4];
    uint8_t lastRSSI;
} thrNeighborTbl_t;

typedef struct thrChildrenTbl_tag
{
    uint8_t extendedAddr[8];
    uint8_t shortAddr[2];
    uint8_t lastCommTime[4];
    uint8_t lastRSSI;
    uint8_t timeout[4];
} thrChildrenTbl_t;


typedef struct getRoutingTblRsp_tag
{
    uint8_t noOfElements;
    uint8_t idSeqNb;
    uint8_t routerIdMask[8];
    uint8_t routingTable[1]; /* variable data containing: routerId,
                                  nextHop, cost, nOut, nIn */
} getRoutingTblRsp_t;

typedef struct sendRoutingTblInfo_tag
{
    uint8_t routerId;
    uint8_t shortAddr[2];
    uint8_t nextHop[2];
    uint8_t cost;
    uint8_t nOut;
    uint8_t nIn;
} sendRoutingTblInfo_t;

/* network utils
    - get thread Ip address */
typedef struct getThrIpAddrReq_tag
{
    uint8_t instanceId;
    uint8_t addrType;
    uint8_t startIndex;
    uint8_t noOfElements;
} getThrIpAddrReq_t;

/* network utils
    - get thread Ip address */
typedef struct getThrIpAddrRsp_tag
{
    uint8_t instanceId;
    uint8_t status;
    uint8_t addrType;
    uint8_t noOfIpAddr;
    uint8_t ipAddr[1];
} getThrIpAddrRsp_t;

/* network utils
    - get parent rsp */
typedef struct getThrParentRsp_tag
{
    uint8_t status;
    uint8_t instanceId;
    uint8_t shortAddr[2];
    uint8_t extendedAddr[8];
} getThrParentRsp_t;

/* network utils
    - get border router network data table request */
typedef struct getBrNwkDataReq_tag
{
    uint8_t instanceId;
    uint8_t startIndex;
    uint8_t noOfElements;
} getBrNwkDataReq_t;

/* network utils
    - get border router network data table response */
typedef struct getBrNwkDataRsp_tag
{
    uint8_t instanceId;
    uint8_t noOfElements;
    uint8_t brNwkDataTable[1]; /* variable data containing: prefixLength,
                                  prefixValue, prefixFlags, prefixLifetime,
                                  prefixAdvertised, externalRouteFlags,
                                  external Route lifetime, external Route Advertised*/
} getBrNwkDataRsp_t;

/* network diagnostic response*/
typedef struct thciMgmtDiagRsp_t
{
    uint8_t status;
    uint8_t msgId[2];
    uint8_t dataLen[2];
    uint8_t noOfTLVs;
    uint8_t pData[];
} thciMgmtDiagRsp_t;

typedef enum thciThrThresholdType_tag
{
    thrRouterUpgradeThreshold_c = 0x00,
    thrRouterDowngradeThreshold_c,
    thrMinDowngradeNeigbors_c,
    thrMaxAllowedRouters_c,
    thrContextReuseDelay_c,
    thrBhMlrMinTimeoutSec_c
} thciThrThresholdType_t;

/* thci reset cpu success payload */
typedef struct thciResetCpuSuccess_tag
{
    uint8_t boardNameLen;
    uint8_t boardName[sizeof(BOARD_NAME)];
    uint8_t uniqueMcuId[16];
    uint8_t stackVersion[6];
    thrOctet16_t swVersion;
} thciResetSuccess_t;

/* thci reset cpu pending payload */
typedef struct thciResetCpuPending_tag
{
    uint8_t timeout[4]; /* milliseconds*/
} thciResetCpuPending_t;

/* thci reset cpu payload */
typedef struct thciResetCpuEvent_tag
{
    resetCpuStatus_t status; /* success, pending*/
    union
    {
        thciResetSuccess_t resetSuccess;
        thciResetCpuPending_t resetPending;
    } resetPayload;
} thciResetCpuEvent_t;

/* Coap ACK response*/
typedef struct thciCoapStruct_tag
{
    uint8_t reserved; /* Used for instance id in NWKU_CoapSend.Request and for session status in
                                NWKU_CoapRcvd.Indication*/
    uint8_t remoteAddr[16];
    uint8_t udpPort[2];
    uint8_t requestType;
    uint8_t code;
    uint8_t uriPath[THCI_COAP_MAX_URI_PATH_OPT_SIZE];
    uint8_t payloadLen;
    uint8_t payload;
} thciCoapStruct_t;

typedef struct thciCoapCreateInstance_tag
{
    uint8_t udpPort[2];
    uint8_t addrFamily;
} thciCoapCreateInstance_t;

typedef struct thciCoapRegisterCb_tag
{
    uint8_t instance;
    uint8_t addrFamily;
    uint8_t uriPath[THCI_COAP_MAX_URI_PATH_OPT_SIZE];
    uint8_t udpPort[2];
} thciCoapRegisterCb_t;

typedef struct thciMeshcopDiagHdr_tag
{
    uint8_t direction;      /*!< IN/OUT */
    uint8_t meshcopType;    /*!< JOIN_FIN/JOIN_ENT/DTLS_CLOSE_NOTIFY req/rsp */
    uint8_t aAddr[8];       /*!< ieee address of the sender */
    uint8_t tlvsLen;
} thciMeshcopDiagHdr_t;

typedef struct thciNwkDataService_tag
{
    uint8_t instanceId;
    uint8_t serviceName[9];
    uint8_t serverAddr[16];
} thciNwkDataService_t;

typedef struct thciDnsResponse_tag
{
    uint8_t serverIpAddr[16];
    uint8_t domainName[30];
} thciDnsResponse_t;

typedef enum thciMcastGroupManage_tag
{
    gThciMcastGroupAdd_c    = 0x01,
    gThciMcastGroupLeave_c  = 0x02,
    gThciMcastGroupComm_c   = 0x03,
} thciMcastGroupManage_t;


typedef enum thciRoutesManage_tag
{
    gThciRouteAdd_c    = 0x01,
    gThciRouteRemove_c  = 0x02
} thciRoutesManage_t;

typedef struct thciRoutes_tag
{
    uint8_t destAddr[16];
    uint8_t nextHopAddr[16];
    uint8_t prefixLen;
    uint8_t routeMetric;
    uint8_t interfaceId;
} thciRoutes_t;

typedef struct thciEidRlocCache_tag
{
    uint8_t eid[16];
    uint8_t rloc[2];
} thciEidRlocCache_t;
#if IP_IP6_FIREWALL_ENABLE
typedef struct thciIpFirewall_tag
{
    uint8_t ruleNbEntry;        /*!< Entry number in IP6 Firewall table */
    uint8_t srcPrefix[16];      /*!< Source IPv6 address or prefix of network */
    uint8_t dstPrefix[16];      /*!< Destination IPv6 address or prefix of network */
    uint8_t srcPort[2];         /*!< Source port */
    uint8_t dstPort[2];         /*!< Destination port */
    uint8_t srcPrefixLenBits;   /*!< Source IPv6 prefix length in bits */
    uint8_t dstPrefixLenBits;   /*!< Destination IPv6 prefix length in bits */
    int8_t secLevel;            /*!< MAC security level */
    uint8_t interfaceId;        /*!< Interface ID */
    bool_t rule;                /*!< Firewall rule can be accept or drop */
}thciIpFirewall_t;
#endif /* IP_IP6_FIREWALL_ENABLE */
/*==================================================================================================
Private prototypes
==================================================================================================*/
static void THCI_DataIndCb(void *pData, void *param, uint32_t interfaceId);
static void THCI_DataIndHandler(uint8_t *param);

static void THCI_BSDSockReqSocket(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t *pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqShutdown(uint8_t *pClientPacket, uint32_t interfaceId,
                                    statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqClose(uint8_t *pClientPacket,  uint32_t interfaceId,
                                    statusConfirm_t *pReplyData,   uint16_t *pDataSize);
static void THCI_BSDSockReqBind(uint8_t *pClientPacket, uint32_t interfaceId,
                                statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqSend(uint8_t *pClientPacket, uint32_t interfaceId,
                                statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqSendTo(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqRecv(uint8_t *pClientPacket, uint32_t interfaceId,
                                uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqRecvFrom(uint8_t *pClientPacket, uint32_t interfaceId,
                                    uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqConnect(uint8_t *pClientPacket, uint32_t interfaceId,
                                   statusConfirm_t *pReplyData, uint16_t *pDataSize);
#if BSDS_STREAM_SUPPORT && TCP_ENABLED
static void THCI_BSDSockReqListen(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqAccept(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize);
#endif /* BSDS_STREAM_SUPPORT && TCP_ENABLED */
static void THCI_BSDSockReqSetOpt(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BSDSockReqGetOpt(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize);

static void THCI_IfconfigAll(uint8_t *pClientPacket, uint32_t interfaceId,
                             uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_IfconfigBind(uint8_t *pClientPacket, uint32_t interfaceId,
                              statusConfirm_t *pReplyData, uint16_t *pDataSize);
static bool_t THCI_Ping(uint8_t *pClientPacket, uint32_t interfaceId,
                        uint8_t **pReplyData, uint16_t *pDataSize);



#if SERIAL_TUN_IF
/*
 * SERIAL_TUN Interface functions
 */
static void THCI_SerialtunLinkIndication(uint8_t *pClientPacket, uint32_t interfaceId,
                                            statusConfirm_t *pReplyData, uint16_t *pDataSize);
static bool_t THCI_SerialtunSend(uint8_t *pClientPacket, uint32_t interfaceId,
                                 uint8_t **pReplyData, uint16_t *pDataSize);
#endif /* SERIAL_TUN_IF */


static void THCI_GetInstancesReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                 uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_SetAttrReq(uint8_t *pClientPacket, uint32_t interfaceId,
                            statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_GetAttrReq(uint8_t *pClientPacket, uint32_t interfaceId,
                            uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_GetDeviceInfo(uint8_t *pClientPacket, uint32_t interfaceId,
                            uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_SetDeviceConfig(uint8_t *pClientPacket, uint32_t interfaceId,
                                 statusConfirm_t *pReplyData, uint16_t *pDataSize);
/* network interface */
static void THCI_NwkScanReq(uint8_t *pClientPacket, uint32_t interfaceId,
                            statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_NwkJoinReq(uint8_t *pClientPacket, uint32_t interfaceId,
                            statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_NwkDisconnectReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_ThrGetParentReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                 uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_NwkCreateReq(uint8_t *pClientPacket, uint32_t interfaceId,
                              statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_FactoryResetReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                 statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_ThrCpuResetReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_ChildUpdateToParent(uint8_t *pClientPacket, uint32_t interfaceId,
                                     statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_NwkDiscoveryReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                 statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_NwkDiscoveryStop(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_SearchNwkWithAnnounce(uint8_t *pClientPacket, uint32_t interfaceId,
                                       statusConfirm_t *pReplyData, uint16_t *pDataSize);

/* MAC filter table */
static void THCI_AddMacFilterEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                   statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_RemoveMacFilterEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                      statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_EnableMacFiltering(uint8_t *pClientPacket, uint32_t interfaceId,
                                    statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_GetMacFilterTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                   uint8_t **pReplyData, uint16_t *pDataSize);

/* nwk data */
static void THCI_LeaderRemoveRouterId(uint8_t *pClientPacket, uint32_t interfaceId,
                                      statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_ThrSwitchKey(uint8_t *pClientPacket, uint32_t interfaceId,
                              statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_ThrGenerateKey(uint8_t *pClientPacket, uint32_t interfaceId,
                                statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_GetThrIpAddr(uint8_t *pClientPacket, uint32_t interfaceId,
                              uint8_t **pReplyData, uint16_t *pDataSize);

/* server data */
#if !THREAD_ED_CONFIG
static void THCI_BrPrefixAttrAddEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                      statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BrPrefixAttrRemoveEntry(uint8_t *pClientPacket, uint32_t interfaceId,
        statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BrPrefixAttrGetTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                      uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_BrPrefixAttrSync(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BrPrefixAttrRemoveAll(uint8_t *pClientPacket, uint32_t interfaceId,
                                       statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BrServiceAttrRemoveEntry(uint8_t *pClientPacket, uint32_t interfaceId,
        statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_BrServiceAttrAddEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                       statusConfirm_t *pReplyData, uint16_t *pDataSize);
#endif /* !THREAD_ED_CONFIG */

static void THCI_ThrSetThreshold(uint8_t *pClientPacket, uint32_t interfaceId,
                                 statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_ThrSetThreshold32(uint8_t *pClientPacket, uint32_t interfaceId,
                                 statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_SendProactiveAddrNotify(uint8_t *pClientPacket, uint32_t interfaceId,
        statusConfirm_t *pReplyData, uint16_t *pDataSize);

#if THR_ENABLE_MGMT_DIAGNOSTICS /* network diagnostic */
static void THCI_MgmtDiagnosticGetReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                      statusCoapConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_MgmtDiagnosticResetReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                        statusCoapConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_MgmtReadMemoryReq(uint8_t *pClientPacket, uint32_t interfaceId, uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_MgmtWriteMemoryReq(uint8_t *pClientPacket);
static void THCI_MgmtDiagnosticDiagTestReq(uint8_t *pClientPacket, uint32_t interfaceId,
        statusCoapConfirm_t *pReplyData, uint16_t *pDataSize);
#endif

static void THCI_ThrIdentify(uint8_t *pClientPacket, uint32_t interfaceId,
                             statusCoapConfirm_t *pReplyData, uint16_t *pDataSize);

#if UDP_ECHO_PROTOCOL
static bool_t THCI_EchoUdp(uint8_t *pClientPacket, uint32_t interfaceId, uint8_t **pReplyData,
                           uint16_t *pDataSize);
#endif
static void THCI_ThrGetNeighborInfo(uint8_t *pClientPacket, uint32_t interfaceId,
                                    uint8_t **pReplyData, uint16_t *pDataSize);

static void THCI_ThrGetChildrenTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                     uint8_t **pReplyData, uint16_t *pDataSize);

static void THCI_ThrGetNeighborTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                     uint8_t **pReplyData, uint16_t *pDataSize);

#if !THREAD_ED_CONFIG
static void THCI_ThrGetRoutingTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                    uint8_t **pReplyData, uint16_t *pDataSize);
#endif
static void THCI_ThrReattach(uint8_t *pClientPacket, uint32_t interfaceId,
                             statusConfirm_t *pReplyData, uint16_t *pDataSize);

#if DTLS_ENABLED && THCI_DTLS_ENABLED
    static void THCI_DtlsOpen(uint8_t *pClientPacket, statusConfirm_t *pConfirm);
    static void THCI_DtlsCloseContext(uint8_t *pClientPacket, statusConfirm_t *pConfirm);
    static void THCI_DtlsClosePeer(uint8_t *pClientPacket, statusConfirm_t *pConfirm);
    static void THCI_DtlsConnect(uint8_t *pClientPacket, thciDtlsConnectConfirm_t *pConfirm);
    static void THCI_DtlsSend(uint8_t *pClientPacket, statusConfirm_t *pConfirm);
#endif /* DTLS_ENABLED && THCI_DTLS_ENABLED*/

#if MESHCOP_ENABLED
static void THCI_MeshCopAddExpectedJoiner(uint8_t *pClientPacket, statusConfirm_t *pConfirm);
static void THCI_MeshCopGetExpectedJoiner(uint8_t *pClientPacket, uint8_t **pConfirm,
        uint16_t *pConfirmLength);
static void THCI_MeshCopRemoveExpectedJoiner(uint8_t *pClientPacket,
        statusConfirm_t *pConfirm);
static void THCI_MeshCopSetCommissionerCredential(uint8_t *pClientPacket,
        statusConfirm_t *pConfirm);

static bool_t THCI_MeshCopMgmtSet2(uint8_t *pClientPacket, statusConfirm_t *pConfirm,
                                   meshcopHandlerCb_t pfSetCb, uint8_t type);
static void THCI_MeshcopMgmtSetCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
static void THCI_MeshcopMgmtReply(uint8_t *pTlvs, uint32_t tlvsLen, uint8_t opGroup, uint8_t opCode);
void THCI_MeshcopMgmtCommSetCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
void THCI_MeshcopMgmtActiveSetCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
void THCI_MeshcopMgmtPendingSetCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);

static bool_t THCI_MeshCopMgmtGet(uint8_t *pClientPacket, statusConfirm_t *pConfirm);
static void THCI_MeshcopMgmtGetCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
void THCI_MeshcopMgmtPanIdConflictCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
void THCI_MeshcopMgmtCommGetCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
void THCI_MeshcopMgmtActiveGetCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
void THCI_MeshcopMgmtPendingGetCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
static void THCI_MeshcopMgmtEdReportCb(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pData, uint32_t dataLen);

static bool_t THCI_MeshCopMgmtSet(uint8_t *pClientPacket, statusConfirm_t *pConfirm);

static bool_t THCI_OOBNwkForm(uint8_t *pClientPacket, statusConfirm_t *pReplyData,
                              uint16_t *pDataSize);
static void THCI_MeshcopNetFormCb(meshcopHandlers_t *pHandlerEntry, uint8_t *pTlvs, uint32_t tlvsLen);
#endif

void PING_EchoReplyReceiveAsync(ipPktInfo_t *pIpPktInfo);
static void PING_EchoReplyReceive(uint8_t *pParam);
static void PING_TimerCallback(void *param);
static void PING_HandleTimerCallback(uint8_t *param);
static ipPktInfo_t *PING_CreatePktInfo(ipAddr_t *pDstAddr, uint32_t payloadLen);

void THCI_ResetCpuEvent(resetCpuStatus_t resetStatus, uint32_t timeoutUs);

/* CoAP */
static bool_t THCI_CoapSend(uint8_t *pClientPacket, uint32_t interfaceId, statusConfirm_t *pReplyData, uint16_t *pDataSize);
static bool_t THCI_CoapCreateInstance(uint8_t *pClientPacket, uint32_t interfaceId, statusConfirm_t *pReplyData, uint16_t *pDataSize);
void THCI_CoapReceiveCb(coapSessionStatus_t sessionStatus, uint8_t *pData, coapSession_t *pSession, uint32_t dataLen);
static bool_t THCI_CoapRegisterCb(uint8_t *pClientPacket, uint32_t interfaceId, statusConfirm_t *pReplyData, uint16_t *pDataSize);
#if !THREAD_ED_CONFIG
static bool_t THCI_CoapPromoteAsRouter(uint8_t *pClientPacket, uint32_t interfaceId, statusConfirm_t *pReplyData,
                                       uint16_t *pDataSize);
#endif
void THCI_HandleMeshcopDiagnostics(meshcopDiagnosticType_t meshcopType,
                                   meshcopDiagnosticDir_t dir, uint8_t *pEui, uint8_t *pTlvs, uint32_t tlvsLen);
#if DNS_ENABLED
void THCI_DnsService(uint8_t *pMsg);
static bool_t THCI_DnsSendRequest(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize);
#endif
static void THCI_MulticastGroupShow(uint8_t *pClientPacket, uint32_t interfaceId,
                                    uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_MulticastGroupManage(uint8_t *pClientPacket, uint32_t interfaceId,
                                      statusConfirm_t *pReplyData, uint16_t *pDataSize);
#if IP_IP6_ROUTING_ENABLE
static void THCI_RoutesManage(uint8_t *pClientPacket, uint32_t interfaceId,
                              statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_RoutesShow(uint8_t *pClientPacket, uint32_t interfaceId,
                            uint8_t **pReplyData, uint16_t *pDataSize);
#endif

#if IP_IP6_FIREWALL_ENABLE
static void THCI_FirewallRuleAdd(uint8_t *pClientPacket, uint32_t interfaceId,
                                           statusConfirm_t *pReplyData, uint16_t *pDataSize);
static void THCI_FirewallShow(uint8_t *pClientPacket, uint32_t interfaceId,
                                        uint8_t **pReplyData, uint16_t *pDataSize);
#endif /* IP_IP6_FIREWALL_ENABLE */
static void THCI_EidRlocMappingTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                     uint8_t **pReplyData, uint16_t *pDataSize);
static void THCI_transmitPayload(uint8_t OG, uint8_t OC, void *pMsg, uint16_t msgLen, uint32_t fsciInterface);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static uint8_t mFsciAppInterfaceId = FSCI_INTERFACE; /* Index in the mFsciSerials array */
#if DTLS_ENABLED && THCI_DTLS_ENABLED
    static int32_t maDtlsContexts[2];
    static int32_t maDtlsPeers[DTLS_MAX_PEERS];
#endif
static taskMsgQueue_t *pmMainThreadMsgQueue;

static ipAddr_t mDstIpAddr;
static uint64_t mPingTimeStamp = 0;
static tmrTimerID_t mPingTimerID = gTmrInvalidTimerID_c;
static uint8_t *pmQueuedReplyData = NULL;
static opCode_t mQueuedOpCode;
static uint16_t mQueuedDataSize;
static uint16_t mDefaultSeqNb = PING_SEQ_NB;
static uint16_t mPingTimeoutMs;
static uint32_t mPingMacSec = 5;
static uint8_t  mPingHopLimit = ICMP_DEFAULT_HOP_LIMIT;
/* CoAP instance */
static uint8_t mCoapInstId = THR_ALL_FFs8;

static coapUriPath_t aCoapUriPathStruct[THCI_COAP_MAX_URI_PATHS] = {0};
static uint8_t aCoapUriPaths[THCI_COAP_MAX_URI_PATHS][THCI_COAP_MAX_URI_PATH_OPT_SIZE] = {{0}, {0}};
static bool_t mThciBoardIdentify = FALSE;
static uint32_t gaUniqueId[4];
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
#if DTLS_ENABLED && THCI_DTLS_ENABLED
    dtlsCallbacks_t mDtlsCallbacks;
#endif

const icmpProtMsgTypeHandler_t gaShellProtMsgTypeHandlerTbl6[] =
{
    {PING_EchoReplyReceiveAsync, 129U, ICMP_CODE_DEFAULT}
};

const uint8_t gShellProtMsgTypeHandlerTbl6Size = sizeof(gaShellProtMsgTypeHandlerTbl6) / sizeof(icmpProtMsgTypeHandler_t);

const icmpProtMsgTypeHandler_t gaShellProtMsgTypeHandlerTbl4[] =
{
    {PING_EchoReplyReceiveAsync, 0U, ICMP_CODE_DEFAULT}
};

const uint8_t gShellProtMsgTypeHandlerTbl4Size = sizeof(gaShellProtMsgTypeHandlerTbl4) / sizeof(icmpProtMsgTypeHandler_t);


/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     void APP_FsciInterface(void)
\brief  This function is used to initialize the FSCI communication.

\param  [in]    pointer to the message queue

\return         void
***************************************************************************************************/
void APP_FsciInterface
(
    taskMsgQueue_t *pMainThreadMsgQueue
)
{
    pmMainThreadMsgQueue = pMainThreadMsgQueue;

    /* Register Handler for requests coming from the serial interface */
    if (FSCI_RegisterOpGroup(gFSCI_IpStackOpGReq_c, gFsciMonitorMode_c, THCI_DataIndCb, NULL,
                             mFsciAppInterfaceId) != gFsciSuccess_c)
    {
        panic(0, 0, 0, 0);
    }

    /* Harness needed data handler */
    MESHCOP_SetDiagHandler(0, THCI_HandleMeshcopDiagnostics);

    THR_GetUniqueId(gaUniqueId);
#if !THCI_USBENET_ENABLE
    /* inform application */
    THCI_ResetCpuEvent(gResetCpuSuccess_c, 0);
#endif
}

/*!*************************************************************************************************
\fn     void PING_EchoReplyReceiveAsync(ipPktInfo_t ipPktInfo)
\brief  Interface function for the user app. It handles a received Ping Echo Reply message.

\param  [in]    pIpPktInfo      Pointer to the packet information structure.
***************************************************************************************************/
void PING_EchoReplyReceiveAsync
(
    ipPktInfo_t *pIpPktInfo
)
{
    if (FALSE == NWKU_SendMsg(PING_EchoReplyReceive, (void *)pIpPktInfo, pmMainThreadMsgQueue))
    {
        NWKU_FreeIpPktInfo(&pIpPktInfo);
    }
}

#if SERIAL_TUN_IF
/*!*************************************************************************************************
\fn     void Serialtun_Send(uint8_t *pInData, uint32_t size)
\brief  This function is used to send data from stack to the host through FSCI

\param  [in]    pInData         Pointer to the data which has to be send.
\param  [in]    size            Size of the data which has to be send.
***************************************************************************************************/
void Serialtun_Send
(
    uint8_t *pInData,
    uint32_t size
)
{
    FSCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_IpStackSerialtunReceive_c, pInData, size,
                         FSCI_INTERFACE);
    MEM_BufferFree(pInData);
}
#endif /* SERIAL_TUN_IF */

#if DNS_ENABLED
/*!*************************************************************************************************
\fn     void THCI_DnsService(uint8_t* pMsg)
\brief  This function is the callback function for the DNS response message.

\param  [in]

\return         void
***************************************************************************************************/
void THCI_DnsService
(
    uint8_t *pMsg
)
{
    thciDnsResponse_t dnsResponse = {0};
    hostent_t *pDnsResponse = (hostent_t *)pMsg;
    ipAddr_t *pIpAddr = NULL;

    if (pMsg != NULL)
    {
        FLib_MemCpy(&dnsResponse.domainName, pDnsResponse->pHostName, strlen((char *)pDnsResponse->pHostName));

        do
        {
            pIpAddr = ListGetHeadMsg(pDnsResponse->pHostAddrList);

            if (pIpAddr)
            {
                FLib_MemCpyReverseOrder(&dnsResponse.serverIpAddr, pIpAddr, sizeof(ipAddr_t));


                ListRemoveMsg(pIpAddr);
                MEM_BufferFree(pIpAddr);
            }
        }
        while (pIpAddr);

        MEM_BufferFree(pDnsResponse->pHostName);
        MEM_BufferFree(pDnsResponse->pHostAliasesList);
        MEM_BufferFree(pDnsResponse->pHostAddrList);
        MEM_BufferFree(pMsg);
    }

    /* send response */
    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_ThrDnsRcvd_c, &dnsResponse,
                         sizeof(thciDnsResponse_t), mFsciAppInterfaceId);
}
#endif /* DNS_ENABLED */

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\private
\fn     void THCI_DataIndCb(void *pData, void* param, uint32_t interfaceId)
\brief  This is the SAP used to call different socket functions.

\param  [in]    pMsg        pointer to the MCPS message received from the MAC layer instanceId of
                            the MAC
\param  [in]    param       pointer to the parameter to be passed to this function
\param  [in]    interfaceId id of the FSCI interface

\return         void
***************************************************************************************************/
static void THCI_DataIndCb
(
    void *pData,
    void *param,
    uint32_t interfaceId
)
{
    thciGenericMsg_t *pThciMessage;


    pThciMessage = MEM_BufferAlloc(sizeof(thciGenericMsg_t));

    if (pThciMessage)
    {
        pThciMessage->interfaceId = interfaceId;
        pThciMessage->pData = pData;

        if (FALSE == NWKU_SendMsg(THCI_DataIndHandler, (void *)pThciMessage, pmMainThreadMsgQueue))
        {
            MEM_BufferFree(pThciMessage);
        }
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_DataIndHandler(uint8_t* param)
\brief  This is the SAP used to call different socket functions.

\param  [in]    param   pointer to the parameter to be passed to this function

\return         void
***************************************************************************************************/
static void THCI_DataIndHandler
(
    uint8_t *param
)
{
    void *pData = ((thciGenericMsg_t *)param)->pData;
    uint32_t interfaceId = ((thciGenericMsg_t *)param)->interfaceId;
    clientPacket_t *pRxClientPacket = (clientPacket_t *)pData;
    uint8_t *pClientPacket = (uint8_t *)pData;
    uint16_t replyDataSize = 0;
    uint8_t *pReplyData = NULL;
    bool_t sendReply = TRUE;
    bool_t freePacket = FALSE;
    statusConfirm_t basicConfirm;
    statusCoapConfirm_t coapConfirm;
#if DTLS_ENABLED && THCI_DTLS_ENABLED
    thciDtlsConnectConfirm_t dtlsConnectConfirm;
#endif
    opCode_t   opCode = pRxClientPacket->structured.header.opCode;

    /* removed thci header to have an aligned pointer*/
    FLib_MemInPlaceCpy(pClientPacket, pRxClientPacket->structured.payload, pRxClientPacket->structured.header.len);

    switch (opCode)
    {
        case gTHCI_IpStackBsdSocket_c:
            THCI_BSDSockReqSocket(pClientPacket, interfaceId, (uint8_t *)&basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackBsdShutdown_c:
            THCI_BSDSockReqShutdown(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackBsdClose_c:
            THCI_BSDSockReqClose(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackBsdBind_c:
            THCI_BSDSockReqBind(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackBsdSend_c:
            THCI_BSDSockReqSend(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackBsdSendto_c:
            THCI_BSDSockReqSendTo(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackBsdRecv_c:
            THCI_BSDSockReqRecv(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_IpStackBsdRecvfrom_c:
            THCI_BSDSockReqRecvFrom(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_IpStackBsdConnect_c:
            THCI_BSDSockReqConnect(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
#if BSDS_STREAM_SUPPORT && TCP_ENABLED

        case gTHCI_IpStackBsdListen_c:
            THCI_BSDSockReqListen(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackBsdAccept_c:
            THCI_BSDSockReqAccept(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;
#endif /* BSDS_STREAM_SUPPORT */

        case gTHCI_IpStackBsdSetsockopt_c:
            THCI_BSDSockReqSetOpt(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackBsdGetsockopt_c:
            THCI_BSDSockReqGetOpt(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_IpStackIfconfigAll_c:
            THCI_IfconfigAll(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_IpStackIfconfigBind_c:
            THCI_IfconfigBind(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackPing_c:
            sendReply = THCI_Ping(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_IpStackMcastGroupShow_c:
            THCI_MulticastGroupShow(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_IpStackMcastGroupManage_c:
            THCI_MulticastGroupManage(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
#if IP_IP6_ROUTING_ENABLE

        case gTHCI_IpStackRoutesManage_c:
            THCI_RoutesManage(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_IpStackRoutesShow_c:
            THCI_RoutesShow(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;
#endif

#if IP_IP6_FIREWALL_ENABLE
        case gTHCI_IpFirewallEnable_c:
            IP6_FirewallEnabled((bool_t)pClientPacket[0]);
            basicConfirm.status = gThrStatus_Success_c;
            replyDataSize = sizeof(statusConfirm_t);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
        case gTHCI_IpFirewallRuleAdd_c:
            THCI_FirewallRuleAdd(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
        case gTHCI_IpFirewallRuleRemove_c:
            basicConfirm.status = gThrStatus_Failed_c;
            if (gIpOk_c == IP6_FirewallRemoveRule(pClientPacket[0]))
            {
                basicConfirm.status = gThrStatus_Success_c;
            }
            replyDataSize = sizeof(statusConfirm_t);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
        case gTHCI_IpFirewallTableShow_c:
            THCI_FirewallShow(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;
#endif /* IP_IP6_FIREWALL_ENABLE */
        case gTHCI_ThrRlocEidMappingTable_c:
            THCI_EidRlocMappingTable(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;
#if SERIAL_TUN_IF
        case gTHCI_IpStackSerialtunLinkIndication_c:
            THCI_SerialtunLinkIndication(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t*)&basicConfirm;
            freePacket = TRUE;
            break;
        case gTHCI_IpStackSerialtunSend_c:
            sendReply = THCI_SerialtunSend(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;
#endif /* SERIAL_TUN_IF */

        case gTHCI_GetInstancesReq_c:
            THCI_GetInstancesReq(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_GetAttrReq_c:
            THCI_GetAttrReq(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_ThrGetDeviceInfo_c:
            THCI_GetDeviceInfo(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_SetAttrReq_c:
            THCI_SetAttrReq(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_GetThrIpAddr_c:
            THCI_GetThrIpAddr(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_SetDeviceConfig_c:
            THCI_SetDeviceConfig(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrGetNeighborInfo_c:
            THCI_ThrGetNeighborInfo(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_ThrGetChildrenTable_c:
            THCI_ThrGetChildrenTable(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_ThrGetNeighborTable_c:
            THCI_ThrGetNeighborTable(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

#if !THREAD_ED_CONFIG
        case gTHCI_ThrGetRoutingTable_c:
            THCI_ThrGetRoutingTable(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;
#endif
        /* network interface */
        case gTHCI_ThrNwkScan_c:
            THCI_NwkScanReq(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrNwkCreate_c:
            THCI_NwkCreateReq(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrFactoryReset_c:
            THCI_FactoryResetReq(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrCpuReset_c:
            THCI_ThrCpuResetReq(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrChildUpdateToParent_c:
            THCI_ChildUpdateToParent(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrNwkJoin_c:
            THCI_NwkJoinReq(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrNwkDisconnect_c:
            THCI_NwkDisconnectReq(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrGetParent_c:
            THCI_ThrGetParentReq(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_ThrNwkDiscoveryReq_c:
            THCI_NwkDiscoveryReq(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrNwkDiscoveryStop_c:
            THCI_NwkDiscoveryStop(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrSearchNwkWithAnounce_c:
            THCI_SearchNwkWithAnnounce(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        /* mac filtering: */
        case gTHCI_AddMacFilterEntry_c:
            THCI_AddMacFilterEntry(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_RemoveMacFilterEntry_c:
            THCI_RemoveMacFilterEntry(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_EnableMacFiltering_c:
            THCI_EnableMacFiltering(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_GetMacFilterTable_c:
            THCI_GetMacFilterTable(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_ThrReattach_c:
            THCI_ThrReattach(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        /* Nwk data*/
        case gTHCI_LeaderRemoveRouterId_c:
            THCI_LeaderRemoveRouterId(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrSwitchKey_c:
            THCI_ThrSwitchKey(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrGenerateKey_c:
            THCI_ThrGenerateKey(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
#if !THREAD_ED_CONFIG

        case gTHCI_BrPrefixAttrAddEntry_c:
            THCI_BrPrefixAttrAddEntry(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_BrPrefixAttrRemoveEntry_c:
            THCI_BrPrefixAttrRemoveEntry(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_BrPrefixAttrGetTable_c:
            THCI_BrPrefixAttrGetTable(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gTHCI_BrPrefixAttrSync_c:
            THCI_BrPrefixAttrSync(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_BrPrefixAttrRemoveAll_c:
            THCI_BrPrefixAttrRemoveAll(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_BrServiceAdd_c:
            THCI_BrServiceAttrAddEntry(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_BrServiceRemove_c:
            THCI_BrServiceAttrRemoveEntry(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrSetThreshold_c:
            THCI_ThrSetThreshold(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrSetThreshold32_c:
            THCI_ThrSetThreshold32(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrNetworkIdTimeout_c:
        {
            uint32_t networkIdTimeoutSec;
            FLib_MemCpy(&networkIdTimeoutSec, pClientPacket, sizeof(uint32_t));
            THR_SetNetworkIdTimeoutSec(networkIdTimeoutSec);
            basicConfirm.status = gThrStatus_Success_c;
            replyDataSize = sizeof(statusConfirm_t);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
        }

        case gTHCI_ThrRouterPromotionSec_c:
        {
            uint32_t routerMaxJitter;
            FLib_MemCpy(&routerMaxJitter, pClientPacket, sizeof(uint32_t));
            THR_SetThrRouterSelJitterSec(routerMaxJitter);
            basicConfirm.status = gThrStatus_Success_c;
            replyDataSize = sizeof(statusConfirm_t);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
        }

        case gTHCI_ThrSetParentPriority_c:
            THR_SetParentPriority(*pClientPacket);
            basicConfirm.status = gThrStatus_Success_c;
            replyDataSize = sizeof(statusConfirm_t);
            pReplyData = (uint8_t*)&basicConfirm;
            break;

        case gTHCI_THRCoapAddressSolicit_c:
            THCI_CoapPromoteAsRouter(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
#endif /* THREAD_ED_CONFIG */

        case gTHCI_ThrCoapSend_c:
            sendReply = THCI_CoapSend(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_THRCoapCreateInstance_c:
            THCI_CoapCreateInstance(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrCoapRegisterUri_c:
            sendReply = THCI_CoapRegisterCb(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
#if DNS_ENABLED

        case gTHCI_ThrDnsSend_c:
            sendReply = THCI_DnsSendRequest(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
#endif

        case gTHCI_ThrSetManualSlaacIID_c:
            THR_SetSlaacManualIID(pClientPacket, INET6_IID_LEN);
            basicConfirm.status = gThrStatus_Success_c;
            replyDataSize = sizeof(statusConfirm_t);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_ThrProactiveAddrNotification_c:
            THCI_SendProactiveAddrNotify(pClientPacket, interfaceId, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;
#if THR_ENABLE_MGMT_DIAGNOSTICS /* network diagnostic */

        case gFsci_MgmtDiagnosticGetReq_c:
            THCI_MgmtDiagnosticGetReq(pClientPacket, interfaceId, &coapConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&coapConfirm;
            break;

        case gFsci_MgmtDiagnosticResetReq_c:
            THCI_MgmtDiagnosticResetReq(pClientPacket, interfaceId, &coapConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&coapConfirm;
            break;

        case gFsci_MgmtReadMemoryReq_c:
            THCI_MgmtReadMemoryReq(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;

        case gFsci_MgmtWriteMemoryReq_c:
            THCI_MgmtWriteMemoryReq(pClientPacket);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            ((statusConfirm_t *)pReplyData)->status = mTHCI_Ok_c;
            break;

        case gFsci_MgmtDiagnosticDiagTestReq_c:
            THCI_MgmtDiagnosticDiagTestReq(pClientPacket, interfaceId, &coapConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&coapConfirm;
            break;
#endif

        case gTHCI_ThrIdentify_c:
            THCI_ThrIdentify(pClientPacket, interfaceId, &coapConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&coapConfirm;
            break;
#if UDP_ECHO_PROTOCOL

        case gTHCI_ThrEchoUdp_c:
            sendReply = THCI_EchoUdp(pClientPacket, interfaceId, &pReplyData, &replyDataSize);
            freePacket = TRUE;
            break;
#endif
#if DTLS_ENABLED && THCI_DTLS_ENABLED

        case gTHCI_DtlsOpen_c:
        {
            THCI_DtlsOpen(pClientPacket, &basicConfirm);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_DtlsCloseContext_c:
        {
            THCI_DtlsCloseContext(pClientPacket, &basicConfirm);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_DtlsClosePeer_c:
        {
            THCI_DtlsClosePeer(pClientPacket, &basicConfirm);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_DtlsConnect_c:
        {
            THCI_DtlsConnect(pClientPacket, &dtlsConnectConfirm);
            pReplyData = (uint8_t *)&dtlsConnectConfirm;
            replyDataSize = sizeof(thciDtlsConnectConfirm_t);
            break;
        }

        case gTHCI_DtlsSend_c:
        {
            THCI_DtlsSend(pClientPacket, &basicConfirm);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

#endif /* DTLS_ENABLED && THCI_DTLS_ENABLED */
#if MESHCOP_ENABLED
#if !THREAD_ED_CONFIG

        case gTHCI_MeshCopStartCommissioner_c:
            MESHCOP_StartCommissioner(pClientPacket[0]);
            basicConfirm.status = mTHCI_Ok_c;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;

        case gTHCI_MeshCopStopCommissioner_c:
            MESHCOP_StopCommissioner(pClientPacket[0], TRUE);
            basicConfirm.status = mTHCI_Ok_c;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;

        case gTHCI_MeshCopAddExpectedJoiner_c:
            THCI_MeshCopAddExpectedJoiner(pClientPacket, &basicConfirm);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;

        case gTHCI_MeshCopGetExpectedJoiner_c:
            THCI_MeshCopGetExpectedJoiner(pClientPacket, &pReplyData, &replyDataSize);

            if (!pReplyData)
            {
                basicConfirm.status = mTHCI_Err_c;
                replyDataSize = sizeof(statusConfirm_t);
                pReplyData = (uint8_t *)&basicConfirm;
            }
            else
            {
                freePacket = TRUE;
            }

            break;

        case gTHCI_MeshCopRemoveExpectedJoiner_c:
            THCI_MeshCopRemoveExpectedJoiner(pClientPacket, &basicConfirm);
            basicConfirm.status = mTHCI_Ok_c;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;

        case gTHCI_MeshCopRemoveAllExpectedJoiners_c:
            MESHCOP_RemoveAllExpectedJoiners(pClientPacket[0]);
            basicConfirm.status = mTHCI_Ok_c;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;

        case gTHCI_MeshCopSyncSteeringData_c:
            MESHCOP_SyncSteeringData(pClientPacket[0],
                                     (meshcopEuiMask_t)pClientPacket[1]);
            basicConfirm.status = mTHCI_Ok_c;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;

        case gTHCI_MeshCopNwkForm_c:
            sendReply = THCI_OOBNwkForm(pClientPacket, &basicConfirm, &replyDataSize);
            pReplyData = (uint8_t *)&basicConfirm;
            break;

        case gTHCI_MeshCopNwkJoin_c:
            //MESHCOP_SendNetworkJoin();
            break;

        case gTHCI_MeshCopNwkScan_c:
            //MESHCOP_SendNetworkScan();
            break;

        case gTHCI_MeshCopSet_c:
        {
            sendReply = THCI_MeshCopMgmtSet(pClientPacket, &basicConfirm);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_MeshCopGet_c:
        {
            sendReply = THCI_MeshCopMgmtGet(pClientPacket, &basicConfirm);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }
        case gTHCI_MeshCopNwkLeave_c:
        {
            ipAddr_t ipAddr;

            FLib_MemCpyReverseOrder(&ipAddr.addr8, pClientPacket, 16);
            MESHCOP_SendNetworkLeave(&ipAddr, NULL);

            replyDataSize = sizeof(statusConfirm_t);
            pReplyData = (uint8_t *)&basicConfirm;
        }
            break;

        case gTHCI_MeshCopSetCommissionerCredential_c:
        {
            THCI_MeshCopSetCommissionerCredential(pClientPacket, &basicConfirm);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_MeshCopMgmtCommGet_c:
        {
            uint8_t *pInput = pClientPacket;
            ipAddr_t ipAddr;
            nwkStatus_t nwkStatus;
            meshcopMgmtParams_t mgmtParams = {0};

            mgmtParams.thrInstId = *pInput;
            pInput++;
            FLib_MemCpyReverseOrder(&ipAddr.addr8, pInput, 16);
            pInput += 16;
            mgmtParams.tlvsLength = *pInput;
            pInput++;

            if (mgmtParams.tlvsLength)
            {
                mgmtParams.pTlvs = pInput;
            }

            if (IP_IsAddrEqual(&ipAddr, &in6addr_any))
            {
                mgmtParams.pDstIpAddr = NULL;
            }
            else
            {
                mgmtParams.pDstIpAddr = (uint8_t *)&ipAddr;
            }

            mgmtParams.pfCb = THCI_MeshcopMgmtCommGetCb;
            nwkStatus = MESHCOP_MgmtCommGet(&mgmtParams);

            basicConfirm.status = nwkStatus;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_MeshCopMgmtCommSet_c:
        {
            sendReply = THCI_MeshCopMgmtSet2(pClientPacket, &basicConfirm,
                                             THCI_MeshcopMgmtCommSetCb, gTHCI_MeshCopMgmtCommSet_c);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_MeshCopMgmtActiveGet_c:
        {
            uint8_t *pInput = pClientPacket;
            ipAddr_t ipAddr;
            nwkStatus_t nwkStatus;
            meshcopMgmtParams_t mgmtParams = {0};

            mgmtParams.thrInstId = *pInput;
            pInput++;
            FLib_MemCpyReverseOrder(&ipAddr.addr8, pInput, 16);
            pInput += 16;
            mgmtParams.tlvsLength = *pInput;
            pInput++;

            if (mgmtParams.tlvsLength)
            {
                mgmtParams.pTlvs = pInput;
            }

            if (IP_IsAddrEqual(&ipAddr, &in6addr_any))
            {
                mgmtParams.pDstIpAddr = NULL;
            }
            else
            {
                mgmtParams.pDstIpAddr = (uint8_t *)&ipAddr;
            }

            mgmtParams.pfCb = THCI_MeshcopMgmtActiveGetCb;
            nwkStatus = MESHCOP_MgmtActiveGet(&mgmtParams);

            basicConfirm.status = nwkStatus;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_MeshCopMgmtActiveSet_c:
        {
            sendReply = THCI_MeshCopMgmtSet2(pClientPacket, &basicConfirm,
                                             THCI_MeshcopMgmtActiveSetCb, gTHCI_MeshCopMgmtActiveSet_c);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_MeshCopMgmtPendingGet_c:
        {
            uint8_t *pInput = pClientPacket;
            ipAddr_t ipAddr;
            nwkStatus_t nwkStatus;
            meshcopMgmtParams_t mgmtParams = {0};

            mgmtParams.thrInstId = *pInput;
            pInput++;
            FLib_MemCpyReverseOrder(&ipAddr.addr8, pInput, 16);
            pInput += 16;
            mgmtParams.tlvsLength = *pInput;
            pInput++;

            if (mgmtParams.tlvsLength)
            {
                mgmtParams.pTlvs = pInput;
            }

            if (IP_IsAddrEqual(&ipAddr, &in6addr_any))
            {
                mgmtParams.pDstIpAddr = NULL;
            }
            else
            {
                mgmtParams.pDstIpAddr = (uint8_t *)&ipAddr;
            }

            mgmtParams.pfCb = THCI_MeshcopMgmtPendingGetCb;
            nwkStatus = MESHCOP_MgmtPendingGet(&mgmtParams);

            basicConfirm.status = nwkStatus;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_MeshCopMgmtPendingSet_c:
            THCI_MeshCopMgmtSet2(pClientPacket, &basicConfirm, THCI_MeshcopMgmtPendingSetCb,
                                 gTHCI_MeshCopMgmtPendingSet_c);
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;

        case gTHCI_MeshCopStartNativeCommissioner_c:
            MESHCOP_StartNativeCommissionerScan(pClientPacket[0]);
            basicConfirm.status = mTHCI_Ok_c;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;

        /* Meshcop New Features */
        case gTHCI_MeshCopMgmtPanIdQuery_c:
        {
            uint8_t *pInput = pClientPacket;
            instanceId_t thrInstId;
            uint8_t aChannelMask[4];
            uint32_t channelMask;
            uint16_t panId;
            ipAddr_t ipAddr;
            nwkStatus_t nwkStatus;

            thrInstId = *pInput;
            pInput++;
            FLib_MemCpyReverseOrder(aChannelMask, pInput, sizeof(aChannelMask));
            pInput += sizeof(aChannelMask);
            FLib_MemCpy((void *)&panId, pInput, 2);
            pInput += 2;
            FLib_MemCpyReverseOrder(&ipAddr.addr8, pInput, 16);

            channelMask = ntohal(aChannelMask);

            nwkStatus = MESHCOP_MgmtSendPanIdQuery(thrInstId, channelMask, panId,
                                                   THCI_MeshcopMgmtPanIdConflictCb, &ipAddr);
            basicConfirm.status = nwkStatus;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);

            break;
        }
        case gTHCI_MeshCopMgmtEdScan_c:
        {
            uint8_t *pInput = pClientPacket;
            instanceId_t thrInstId;
            uint8_t aChannelMask[4];
            uint32_t channelMask;
            uint32_t count;
            uint16_t period;
            uint16_t scanDuration;
            ipAddr_t ipAddr;
            nwkStatus_t nwkStatus;

            thrInstId = *pInput;
            pInput++;
            FLib_MemCpyReverseOrder(aChannelMask, pInput, sizeof(aChannelMask));
            pInput += sizeof(aChannelMask);
            count = *pInput;
            pInput++;
            FLib_MemCpy((void *)&period, pInput, 2);
            pInput += 2;
            FLib_MemCpy((void *)&scanDuration, pInput, 2);
            pInput += 2;
            FLib_MemCpyReverseOrder(&ipAddr.addr8, pInput, 16);

            channelMask = ntohal(aChannelMask);

            nwkStatus = MESHCOP_MgmtSendEdScan(thrInstId, channelMask, count, period, scanDuration,
                                               THCI_MeshcopMgmtEdReportCb, &ipAddr);

            basicConfirm.status = nwkStatus;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }

        case gTHCI_MeshCopMgmtAnnounceBegin_c:
        {
            uint8_t *pInput = pClientPacket;
            instanceId_t thrInstId;
            uint16_t commissionerSessionId;
            uint8_t aChannelMask[4];
            uint32_t channelMask;
            uint8_t count;
            uint16_t period;
            ipAddr_t ipAddr;
            nwkStatus_t nwkStatus;

            thrInstId = *pInput;
            pInput++;
            FLib_MemCpy(&commissionerSessionId, pInput, sizeof(uint16_t));
            pInput += sizeof(uint16_t);
            FLib_MemCpyReverseOrder(aChannelMask, pInput, sizeof(aChannelMask));
            pInput += sizeof(aChannelMask);
            count = *pInput;
            pInput += sizeof(uint8_t);
            FLib_MemCpy(&period, pInput, 2);
            pInput += sizeof(uint16_t);
            FLib_MemCpyReverseOrder(&ipAddr.addr8, pInput, 16);

            channelMask = ntohal(aChannelMask);

            nwkStatus = MESHCOP_MgmtSendAnnounceBegin(thrInstId, commissionerSessionId,
                        channelMask, count, period, &ipAddr);

            basicConfirm.status = nwkStatus;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            break;
        }
#endif /* !THREAD_ED_CONFIG */
#endif /* MESHCOP_ENABLED */

        case gTHCI_ResetCpu_c:
            ResetMCU();
            break;

        default:
            break;
    }

    /* Do we want to send the reply now? */
    if (sendReply)
    {
        if (replyDataSize == 0x00)
        {
            basicConfirm.status = mTHCI_Err_c;
            pReplyData = (uint8_t *)&basicConfirm;
            replyDataSize = sizeof(statusConfirm_t);
            freePacket = FALSE;
        }

        /* Send reply */
        if (pReplyData)
        {
            THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, opCode,
                                 pReplyData, replyDataSize, interfaceId);

            if (freePacket)
            {
                MEM_BufferFree(pReplyData);
            }
        }
    }
    /* Do not send the reply and keep some pointers */
    else
    {
        /* Keep variables for later reply */
        mQueuedOpCode = opCode;
        pmQueuedReplyData = pReplyData;
        mQueuedDataSize = replyDataSize;
    }

    /* Clear received packet */
    MEM_BufferFree(pData);
    MEM_BufferFree(param);
}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqSocket(uint8_t *pClientPacket, uint32_t interfaceId)
\brief  This function is used to expose the socket() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqSocket
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t *pReplyData,
    uint16_t *pDataSize
)
{
    sockParams_t *pSockParams = (sockParams_t *)pClientPacket;

    /* Try to create a new socket */
    *pReplyData = (uint8_t)socket(pSockParams->domain, pSockParams->type, pSockParams->protocol);

    /* Set up reply */
    *pDataSize = sizeof(uint8_t);

}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqShutdown(uint8_t *pClientPacket, uint32_t interfaceId,
        statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to expose the shutdown() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqShutdown
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    int32_t result;
    shutdownParams_t *pShutdownParams = (shutdownParams_t *)pClientPacket;
    pReplyData->status = mTHCI_Err_c;

    /* Shutdown a socket */
    result = shutdown(pShutdownParams->sock, 0);

    if (result == 0)
    {
        pReplyData->status = mTHCI_Ok_c;
    }

    /* Set up reply */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqClose(uint8_t *pClientPacket, uint32_t interfaceId,
        statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to expose the close() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqClose
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    int32_t result;
    shutdownParams_t *pShutdownParams = (shutdownParams_t *)pClientPacket;
    pReplyData->status = mTHCI_Err_c;

    /* Close a socket */
    result = closesock(pShutdownParams->sock);

    if (result == 0)
    {
        pReplyData->status = mTHCI_Ok_c;
    }

    /* Set up reply */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqBind(uint8_t *pClientPacket, uint32_t interfaceId,
        statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to expose the bind() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqBind
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    int32_t result = 0;
    sockaddrStorage_t sockAddr;
    pReplyData->status = mTHCI_Err_c;
    bindParams_t *pBindParams;
    bool_t validParam = TRUE;

    *pDataSize = sizeof(statusConfirm_t);

    /* Cast received data to a known structure */
    pBindParams = (bindParams_t *)pClientPacket;

    /* Create sockAddr structure depending on family */
    if (pBindParams->family == AF_INET6)
    {
        ((sockaddrIn6_t *)&sockAddr)->sin6_family = AF_INET6;
        FLib_MemCpy(&((sockaddrIn6_t *)&sockAddr)->sin6_port, pBindParams->port, sizeof(pBindParams->port));
        FLib_MemCpy(((sockaddrIn6_t *)&sockAddr)->sin6_addr.addr8, pBindParams->ipAddr,
                    sizeof(ipAddr_t));
    }
    else if (pBindParams->family == AF_INET)
    {
        ((sockaddrIn_t *)&sockAddr)->sin_family = AF_INET;
        FLib_MemCpy(&((sockaddrIn_t *)&sockAddr)->sin_port, pBindParams->port, sizeof(pBindParams->port));
        FLib_MemCpy(&((sockaddrIn_t *)&sockAddr)->sin_addr, pBindParams->ipAddr,
                    sizeof(ipAddr_t));
    }
    else
    {
        validParam = FALSE;
    }

    if (validParam == TRUE)
    {
        /* Reverse IP address bytes */
        NWKU_SwapArrayBytes(sockAddr.ss_addr, sizeof(ipAddr_t));

        /* Bind socket */
        result = bind(pBindParams->sock, &sockAddr, sizeof(sockaddrStorage_t));

        /* Set FSCI command payload */
        if (result == 0)
        {
            pReplyData->status = mTHCI_Ok_c;
        }
    }

}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqSend(uint8_t *pClientPacket, uint32_t interfaceId,
        statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to expose the send() function.

\param  [in]    pClientPacket pointer to the packet received from FSCI
\param  [in]    interfaceId   id of the FSCI interface
\param  [out]   pReplyData    pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize     size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqSend
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    int32_t result = 0;
    sendParams_t *pSendParam;
    uint16_t len;

    /* Cast received data to a known structure */
    pSendParam = (sendParams_t *)pClientPacket;
    pReplyData->status = mTHCI_Err_c;

    /* Send data */
    FLib_MemCpy(&len, pSendParam->size, sizeof(len));
    result = send(pSendParam->sock, (uint8_t *)pSendParam->data, len, pSendParam->flags);

    /* Set FSCI command payload */
    if (result > 0)
    {
        pReplyData->status = mTHCI_Ok_c;
    }

    /* Set up reply */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqSendTo(uint8_t *pClientPacket, uint32_t interfaceId, statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to expose the send() function.

\param  [in]    pClientPacket pointer to the packet received from FSCI
\param  [in]    interfaceId   id of the FSCI interface
\param  [out]   pReplyData    pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize     size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqSendTo
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    sendtoParams_t      *pSendParam;
    sockaddrStorage_t   sockAddr;
    int32_t             result = 0;
    uint16_t            len;

    /* Cast received data to a known structure */
    pSendParam = (sendtoParams_t *)pClientPacket;
    pReplyData->status = mTHCI_Err_c;
    *pDataSize = sizeof(statusConfirm_t);


    /* Copy IP address */
    FLib_MemCpy(sockAddr.ss_addr, pSendParam->ipAddress, sizeof(ipAddr_t));

    /* Revert IP address */
    NWKU_SwapArrayBytes((uint8_t *)sockAddr.ss_addr, sizeof(ipAddr_t));

    /* Set family */
    sockAddr.ss_family = (IP_IsAddrIPv6((ipAddr_t *)sockAddr.ss_addr)) ? AF_INET6 : AF_INET;

    /* Set remote port */
    if (sockAddr.ss_family == AF_INET6)
    {
        ((sockaddrIn6_t *)&sockAddr)->sin6_port = ntohas(pSendParam->port);
        if (TRUE == pSendParam->bSecured)
        {
            /*use MAC default security level 5 */
            ((sockaddrIn6_t *)&sockAddr)->sin6_flowinfo = BSDS_SET_TX_SEC_FLAGS(1, 5);
        }
        else
        {
            ((sockaddrIn6_t *)&sockAddr)->sin6_flowinfo = BSDS_SET_TX_SEC_FLAGS(1, 0);
        }
    }
    else
    {
        ((sockaddrIn_t *)&sockAddr)->sin_port = ntohas(pSendParam->port);
    }

    /* Send data */
    FLib_MemCpy(&len, pSendParam->size, sizeof(len));
    result = sendto(pSendParam->sock, (uint8_t *)pSendParam->data, len, pSendParam->flags, &sockAddr,
                    sizeof(sockaddrStorage_t));

    /* Set FSCI command payload */
    if (result > 0)
    {
        pReplyData->status = mTHCI_Ok_c;
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqRecv(uint8_t *pClientPacket, uint32_t interfaceId)
\brief  This function is used to expose the recv() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
 ***************************************************************************************************/
static void THCI_BSDSockReqRecv
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    recvParams_t *pRecvParam;
    uint8_t *pSockRecv;
    int32_t rxLength = 0, optLen;
    uint16_t reqSize;

    *pDataSize = 0x00;

    /* Cast received data to a known structure */
    pRecvParam = (recvParams_t *)pClientPacket;
    FLib_MemCpy(&reqSize, pRecvParam->size, sizeof(pRecvParam->size));

    (void)getsockopt(pRecvParam->sock, SOL_SOCKET, SO_RCVBUF, (uint8_t *)&rxLength, (int32_t *)&optLen);

    if (!rxLength)
    {
        return;
    }

    if (reqSize > rxLength)
    {
        reqSize = rxLength ;
    }

    /* Create output packet */
    pSockRecv = MEM_BufferAlloc(sizeof(uint16_t) + reqSize);

    if (pSockRecv)
    {
        /* Set size */
        FLib_MemCpy(pSockRecv, &reqSize, sizeof(reqSize));
        (void)recv(pRecvParam->sock, (uint8_t *)(pSockRecv + sizeof(uint16_t)), reqSize,
                   pRecvParam->flags);

        /* Set up reply */
        *pReplyData = pSockRecv;
        *pDataSize = sizeof(uint16_t) + reqSize;
    }

}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqRecvFrom(uint8_t *pClientPacket, uint32_t interfaceId)
\brief  This function is used to expose the recv() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
 ***************************************************************************************************/
static void THCI_BSDSockReqRecvFrom
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    recvParams_t        *pRecvParam;
    uint8_t             *pSockRecv;
    sockaddrStorage_t   sockAddr;
    int32_t             rxLength = 0;
    uint32_t            outputLength = 0;
    socklen_t           sockAddrLen = sizeof(sockaddrStorage_t);
    int32_t             optLen;
    uint32_t            reqSize;

    /* Cast received data to a known structure */
    pRecvParam = (recvParams_t *)pClientPacket;
    FLib_MemCpy(&reqSize, pRecvParam->size, sizeof(reqSize));

    *pDataSize = 0x00;

    (void)getsockopt(pRecvParam->sock, SOL_SOCKET, SO_RCVBUF, (uint8_t *)&rxLength, (int32_t *)&optLen);

    if (!rxLength)
    {
        return;
    }

    if (reqSize < rxLength)
    {
        rxLength = reqSize;
    }

    /* Create the output packet */
    outputLength =
        1 +                 /* status code */
        sizeof(ipAddr_t) +  /* remote IP address */
        sizeof(uint16_t) +  /* remote port */
        sizeof(uint16_t) +  /* length of the received packet */
        rxLength;           /* received data */

    pSockRecv = MEM_BufferAlloc(outputLength);

    if (!pSockRecv)
    {
        return;
    }

    FLib_MemSet(pSockRecv, 0, outputLength - rxLength);

    *pSockRecv = mTHCI_Err_c;

    if (rxLength == THR_ALL_FFs16)
    {
        /* Error */
    }
    else
    {
        /* Copy received bytes */
        (void)recvfrom(pRecvParam->sock, (uint8_t *)(pSockRecv + outputLength - rxLength),
                       rxLength, pRecvParam->flags, &sockAddr, &sockAddrLen);

        /* Set IP address */
        FLib_MemCpy(pSockRecv + 1, sockAddr.ss_addr, sizeof(ipAddr_t));

        /* Revert IP address */
        NWKU_SwapArrayBytes(pSockRecv + 1, sizeof(ipAddr_t));

        /* Set port depending on RX packet */
        if (sockAddr.ss_family == AF_INET6)
        {
            FLib_MemCpy(pSockRecv + 1 + sizeof(ipAddr_t), &((sockaddrIn6_t *)&sockAddr)->sin6_port,
                        sizeof(uint16_t));
        }
        else
        {
            FLib_MemCpy(pSockRecv + 1 + sizeof(ipAddr_t), &((sockaddrIn_t *)&sockAddr)->sin_port,
                        sizeof(uint16_t));
        }

        /* Set the length of the received packet */
        FLib_MemCpy(
            pSockRecv +
            1 +                  /* status code */
            sizeof(ipAddr_t) +   /* remote IP address */
            sizeof(uint16_t),     /* remote port */
            &rxLength,
            sizeof(uint16_t));

        *pSockRecv = mTHCI_Ok_c;
    }



    /* Set up reply */
    *pReplyData = (uint8_t *)pSockRecv;
    *pDataSize = outputLength;

}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqConnect(uint8_t *pClientPacket, uint32_t interfaceId)
\brief  This function is used to expose the connect() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqConnect
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    connectParams_t *pConnectParam;
    sockaddrStorage_t sockAddr;
    pReplyData->status = mTHCI_Err_c;
    *pDataSize = sizeof(statusConfirm_t);
    int32_t result;

    /* Cast received data to a known structure */
    pConnectParam = (connectParams_t *)pClientPacket;

    /* Create sockAddr structure depending on family */
    if (pConnectParam->family == AF_INET6)
    {
        ((sockaddrIn6_t *)&sockAddr)->sin6_family = AF_INET6;
        FLib_MemCpy(&((sockaddrIn6_t *)&sockAddr)->sin6_port, pConnectParam->port, sizeof(uint16_t));
    }
    else if (pConnectParam->family == AF_INET)
    {
        ((sockaddrIn_t *)&sockAddr)->sin_family = AF_INET;
        FLib_MemCpy(&((sockaddrIn_t *)&sockAddr)->sin_port, pConnectParam->port, sizeof(uint16_t));
    }

    FLib_MemCpy(&((sockaddrIn_t *)&sockAddr)->sin_addr, pConnectParam->ipAddr,
                sizeof(ipAddr_t));
    /* Reverse address bytes */
    NWKU_SwapArrayBytes(sockAddr.ss_addr, sizeof(ipAddr_t));

    /* Connect to the remote socket */
    result = connect(pConnectParam->sock, &sockAddr, sizeof(sockaddrStorage_t));

    /* Set FSCI command payload */
    if (result == 0)
    {
        pReplyData->status = mTHCI_Ok_c;
    }

}

#if BSDS_STREAM_SUPPORT && TCP_ENABLED
/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqListen(uint8_t *pClientPacket, uint32_t interfaceId)
\brief  This function is used to expose the listen() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqListen
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    listenParams_t *pListenParam;
    int32_t result;

    /* Cast received data to a known structure */
    pListenParam = (listenParams_t *)pClientPacket;

    /* Connect to the remote socket */
    result = listen(pListenParam->sock, pListenParam->backlog);

    /* Set FSCI command payload */
    if (result == 0)
    {
        pReplyData->status = mTHCI_Ok_c;
    }
    else
    {
        pReplyData->status = mTHCI_Err_c;
    }

    /* Set up reply */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqAccept(uint8_t *pClientPacket, uint32_t interfaceId)
\brief  This function is used to expose the listen() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqAccept
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    sockAccept_t *pSockAccept = MEM_BufferAlloc(sizeof(sockAccept_t));
    *pDataSize = 0x00;

    if (pSockAccept)
    {
        acceptParams_t *pAcceptParam;
        int32_t result;

        thciErrorCode_t error = mTHCI_Ok_c;
        sockaddrStorage_t sockAddr;


        /* Cast received data to a known structure */
        pAcceptParam = (acceptParams_t *)pClientPacket;
        pSockAccept->status = mTHCI_Err_c;


        /* Connect to the remote socket */
        result = accept(pAcceptParam->sock, &sockAddr, sizeof(sockaddrStorage_t));

        if (result != -1)
        {
            pSockAccept->status = mTHCI_Ok_c;
            pSockAccept->sock = result;
        }

        /* Set up reply */
        *pReplyData = (uint8_t *)pSockAccept;
        *pDataSize = sizeof(sockAccept_t);

    }
}
#endif /* BSDS_STREAM_SUPPORT && TCP_ENABLED */

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqSetOpt(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to expose the setsockopt() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqSetOpt
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    /* Cast received data to a known structure */
    setOptParams_t *pSetOptParams = (setOptParams_t *)pClientPacket;

    pReplyData->status = mTHCI_Err_c;

    /* Process setsockopt */
    if (0U == setsockopt(pSetOptParams->sock, pSetOptParams->level,
                         pSetOptParams->optName, pSetOptParams->optVal, 4U))
    {
        pReplyData->status = mTHCI_Ok_c;
    }

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);

}

/*!*************************************************************************************************
\private
\fn     void THCI_BSDSockReqGetOpt(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to expose the getsockopt() function.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BSDSockReqGetOpt
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    sockGetOpt_t *pSockGetOpt = MEM_BufferAlloc(sizeof(sockGetOpt_t));
    *pDataSize = 0x00;

    if (pSockGetOpt)
    {

        int32_t optLen;
        uint32_t optValue;

        /* Cast received data to a known structure */
        getOptParams_t *pGetOptParams = (getOptParams_t *)pClientPacket;

        pSockGetOpt->status = mTHCI_Err_c;


        /* Process getsockopt */
        if (0 == getsockopt(pGetOptParams->sock, pGetOptParams->level,
                                             pGetOptParams->optName, (uint8_t *)&optValue, (int32_t *)&optLen))
        {
            pSockGetOpt->status = mTHCI_Ok_c;
            htonal(pSockGetOpt->optValue, optValue);
        }


        /* Set up reply data */
        *pReplyData = (uint8_t *)pSockGetOpt;
        *pDataSize = sizeof(sockGetOpt_t);
    }
}


/*!*************************************************************************************************
\private
\fn     void THCI_MulticastGroupAdd(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to add an address to a multicast groups.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_MulticastGroupShow
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint32_t addrCount = 0, index = 0;
    ip6MulticastAddrData_t *pMulticastAddrTable = NULL;
    uint32_t mcastAddrTblSize = IP_IF_GetMcastAddrTableSize();
    uint8_t *pAllMcastAddr = NULL;

    ifHandle_t ifHandle = IP_IF_GetIfByIndex(((uint8_t *)pClientPacket)[0]);

    for (uint32_t iCount = 0; iCount < mcastAddrTblSize; iCount++)
    {
        pMulticastAddrTable = IP_IF_GetMcastAddrTableEntry(iCount);

        if ((NULL != pMulticastAddrTable)
                && (pMulticastAddrTable->ifUniqueId == ifHandle->ifUniqueId))
        {
            addrCount++;
        }
    }

    pAllMcastAddr = MEM_BufferAlloc(addrCount * sizeof(ipAddr_t) + 1);


    if (NULL != pAllMcastAddr)
    {
        /* Set up reply */
        pAllMcastAddr[index] = addrCount;
        index++;

        for (uint32_t iCount = 0; iCount < mcastAddrTblSize; iCount++)
        {
            pMulticastAddrTable = IP_IF_GetMcastAddrTableEntry(iCount);

            if ((NULL != pMulticastAddrTable)
                    && (pMulticastAddrTable->ifUniqueId == ifHandle->ifUniqueId))
            {
                FLib_MemCpyReverseOrder(&pAllMcastAddr[index], &pMulticastAddrTable->mCastAddr,
                                        sizeof(ipAddr_t));
                index += sizeof(ipAddr_t);
            }
        }

    }

    *pDataSize = addrCount * sizeof(ipAddr_t) + 1;
    *pReplyData = (uint8_t *)pAllMcastAddr;
}


/*!*************************************************************************************************
\private
\fn     void THCI_MulticastGroupAdd(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to join a multicast groups.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_MulticastGroupManage
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    addMcastGrpManageReq_t *pManageReq = (addMcastGrpManageReq_t *)pClientPacket;
    ifHandle_t ifHandle = IP_IF_GetIfByIndex(pManageReq->interfaceId);
    thrStatus_t retStatus = gThrStatus_Failed_c;
    ipAddr_t mcastAddr;
    uint32_t *pTimeout = NULL;
    uint32_t timeout;

    FLib_MemCpy(&timeout, pManageReq->timeout, sizeof(uint32_t));
    FLib_MemCpy(&mcastAddr, pManageReq->mcastAddr, sizeof(ipAddr_t));

    if (TRUE == pManageReq->bUseTimeout)
    {
        pTimeout = &timeout;
    }

    if (gThciMcastGroupAdd_c == pManageReq->manageReqType)
    {
        switch (ifHandle->ifUniqueId)
        {
            case gIpIfSlp0_c:
            case gIpIfSlp1_c:
                retStatus = THR_RegisterMulticastGroup(0, &mcastAddr, pTimeout);

                if (gThrStatus_Success_c != retStatus)
                {
                     pReplyData->status = gIpGeneralError_c;
                }
                else
                {
                    pReplyData->status = gIpOk_c;
                }
                break;

            default:
                pReplyData->status = IP_IF_AddMulticastGroup6(ifHandle->ifUniqueId, &mcastAddr);

                break;
        }
    }

    if (gThciMcastGroupLeave_c == pManageReq->manageReqType)
    {
        switch (ifHandle->ifUniqueId)
        {
            case gIpIfSlp0_c:
            case gIpIfSlp1_c:
                retStatus = THR_UnregisterMulticastGroup(0, &mcastAddr);

                if (gThrStatus_Success_c != retStatus)
                {
                     pReplyData->status = gIpGeneralError_c;
                }
                else
                {
                    pReplyData->status = gIpOk_c;
                }
                break;

            default:
                 pReplyData->status = IP_IF_LeaveMulticastGroup6(ifHandle->ifUniqueId, &mcastAddr);

                break;
        }
    }

    if (gThciMcastGroupComm_c == pManageReq->manageReqType)
    {
        switch (ifHandle->ifUniqueId)
        {
            case gIpIfSlp0_c:
            case gIpIfSlp1_c:
                if (gThrStatus_Success_c != retStatus)
                {
                     pReplyData->status = gIpGeneralError_c;
                }
                else
                {
                    pReplyData->status = gIpOk_c;
                }
                break;

            default:
                 pReplyData->status = gIpInvalidParameterError_c;

                break;
        }
    }


    /* Set up reply */
    *pDataSize = sizeof(statusConfirm_t);
}

#if IP_IP6_ROUTING_ENABLE
/*!*************************************************************************************************
\private
\fn     void THCI_RoutesManage(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to manage IPv6 routes.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_RoutesManage
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thciRoutes_t *pRoutes = (thciRoutes_t *)(&pClientPacket[1]);
    ipAddr_t destinationAddr = {0};
    ipAddr_t nextHopAddr = {0};

    FLib_MemCpyReverseOrder(&destinationAddr, &pRoutes->destAddr, sizeof(ipAddr_t));
    FLib_MemCpyReverseOrder(&nextHopAddr, &pRoutes->nextHopAddr, sizeof(ipAddr_t));

    if (pClientPacket[0] == gThciRouteAdd_c)
    {
        IP6_SetStaticRoute(&destinationAddr, pRoutes->prefixLen, pRoutes->routeMetric, &nextHopAddr,
                           IP_IF_GetIfIdByIndex(pRoutes->interfaceId));
    }

    if (pClientPacket[0] == gThciRouteRemove_c)
    {
        IP6_RemoveRoute(&destinationAddr, &nextHopAddr);
    }

    pReplyData->status = mTHCI_Ok_c;

    /* Set up reply */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_RoutesShow(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to display the IPv6 routes.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_RoutesShow
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint8_t *pRoutes = NULL;
    uint8_t routesNb = 0;
    uint8_t index = 0;
    uint32_t routingTblSize = IP6_GetRoutingTableSize();
    ip6RoutingTblEntry_t *pRoute = NULL;

    for (uint32_t i = 0; i < routingTblSize; i++)
    {
        pRoute = IP6_GetRoute(i);

        if ((NULL != pRoute) && (1 == pRoute->validRoute))
        {
            routesNb++;
        }
    }

    pRoutes = MEM_BufferAlloc(routesNb * sizeof(thciRoutes_t) + 1);

    if (pRoutes != NULL)
    {
        pRoutes[index] = routesNb;
        index++;

        for (uint32_t i = 0; i < routingTblSize; i++)
        {
            pRoute = IP6_GetRoute(i);

            if ((NULL != pRoute) && (1 == pRoute->validRoute))
            {
                thciRoutes_t route = {0};
                FLib_MemCpyReverseOrder(&route.destAddr, &pRoute->dstPrefix, sizeof(ipAddr_t));
                FLib_MemCpyReverseOrder(&route.nextHopAddr, &pRoute->nextHopRouter, sizeof(ipAddr_t));
                route.interfaceId = pRoute->nextHopIfId;
                route.prefixLen = pRoute->prefixLen;
                route.routeMetric = pRoute->routeMetric;
                FLib_MemCpy(&pRoutes[index], &route, sizeof(thciRoutes_t));
                index += sizeof(thciRoutes_t);
            }
        }
    }

    *pDataSize = routesNb * sizeof(thciRoutes_t) + 1;
    *pReplyData = (uint8_t *)pRoutes;
}
#endif

#if IP_IP6_FIREWALL_ENABLE
/*!*************************************************************************************************
\private
\fn     void THCI_RoutesManage(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to manage IPv6 routes.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_FirewallRuleAdd
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thciIpFirewall_t *pRule = (thciIpFirewall_t *)(&pClientPacket[0]);
    ip6FirewallTblEntry_t ip6FirewallTblEntry;

    FLib_MemCpyReverseOrder(&ip6FirewallTblEntry.dstPrefix, &pRule->dstPrefix, sizeof(ipAddr_t));
    FLib_MemCpyReverseOrder(&ip6FirewallTblEntry.srcPrefix, &pRule->srcPrefix, sizeof(ipAddr_t));

    FLib_MemCpy(&ip6FirewallTblEntry.dstPort, &pRule->dstPort, sizeof(uint16_t));
    FLib_MemCpy(&ip6FirewallTblEntry.srcPort, &pRule->srcPort, sizeof(uint16_t));

    ip6FirewallTblEntry.dstPrefixLenBits = pRule->dstPrefixLenBits;
    ip6FirewallTblEntry.srcPrefixLenBits = pRule->srcPrefixLenBits;

    ip6FirewallTblEntry.secLevel = pRule->secLevel;
    ip6FirewallTblEntry.ifUniqueId = IP_IF_GetIfIdByIndex(pRule->interfaceId);

    ip6FirewallTblEntry.rule = pRule->rule;
    ip6FirewallTblEntry.enable = TRUE;

    IP6_FirewallSetRule(&ip6FirewallTblEntry);

    pReplyData->status = mTHCI_Ok_c;

    /* Set up reply */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_FirewallShow(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to display the IPv6 Firewall table.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_FirewallShow
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint8_t* pTable = NULL;
    uint8_t entriesNb = 0;
    uint8_t index = 0;
    uint32_t firewallTableSize = IP6_GetFirewallTableSize();
    ip6FirewallTblEntry_t *pFirewallEntry = NULL;

    for(uint32_t i = 0; i < firewallTableSize; i++)
    {
        pFirewallEntry = IP6_GetFirewallRule(i);
        if (NULL != pFirewallEntry)
        {
            entriesNb++;
        }
    }

    pTable = MEM_BufferAlloc(entriesNb * sizeof(thciIpFirewall_t) + 1);

    if (pTable != NULL)
    {
        pTable[index] = entriesNb;
        index++;

        for (uint32_t i = 0; i < firewallTableSize; i++)
        {
            pFirewallEntry = IP6_GetFirewallRule(i);
            if (NULL != pFirewallEntry)
            {
                thciIpFirewall_t entry = {0};
                entry.ruleNbEntry = i;
                FLib_MemCpyReverseOrder(&entry.dstPrefix, &pFirewallEntry->dstPrefix, sizeof(ipAddr_t));
                FLib_MemCpyReverseOrder(&entry.srcPrefix, &pFirewallEntry->srcPrefix, sizeof(ipAddr_t));

                FLib_MemCpy(&entry.dstPort, &pFirewallEntry->dstPort, sizeof(uint16_t));
                FLib_MemCpy(&entry.srcPort, &pFirewallEntry->srcPort, sizeof(uint16_t));

                entry.interfaceId = IP_IF_GetIfIndex((ipIfUniqueId_t)pFirewallEntry->ifUniqueId);
                entry.dstPrefixLenBits = pFirewallEntry->dstPrefixLenBits;
                entry.srcPrefixLenBits = pFirewallEntry->srcPrefixLenBits;
                entry.secLevel = pFirewallEntry->secLevel;
                entry.rule = pFirewallEntry->rule;
                FLib_MemCpy(&pTable[index], &entry, sizeof(thciIpFirewall_t));
                index += sizeof(thciIpFirewall_t);
            }
        }
    }

    *pDataSize = entriesNb * sizeof(thciIpFirewall_t) + 1;
    *pReplyData = (uint8_t *)pTable;
}

#endif /* IP_IP6_FIREWALL_ENABLE */
/*!*************************************************************************************************
\private
\fn     void THCI_EidRlocMappingTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to display the RLOC to EID mapping table.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_EidRlocMappingTable
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint8_t *pTable = NULL;
    uint8_t entries = 0;
    uint8_t index = 0;
    thrLqCacheEntry_t *pCacheEntry = NULL;
    uint32_t counter = 0;

    while ((pCacheEntry = THR_GetRlocToEidMapByEntry(counter)) != NULL)
    {
        if (!FLib_MemCmp(&pCacheEntry->eid, (ipAddr_t *)&in6addr_any, sizeof(ipAddr_t)))
        {
            entries++;
        }

        counter++;
    }

    pTable = MEM_BufferAlloc(entries * sizeof(thciEidRlocCache_t) + 1);

    if (pTable != NULL)
    {
        pTable[index] = entries;
        index++;
        counter = 0;

        while ((pCacheEntry = THR_GetRlocToEidMapByEntry(counter)) != NULL)
        {
            if (!FLib_MemCmp(&pCacheEntry->eid, (ipAddr_t *)&in6addr_any, sizeof(ipAddr_t)))
            {
                thciEidRlocCache_t cacheEntry = {0};
                FLib_MemCpyReverseOrder(&cacheEntry.eid, &pCacheEntry->eid, sizeof(ipAddr_t));
                FLib_MemCpy(&cacheEntry.rloc, &pCacheEntry->address16, sizeof(uint16_t));
                FLib_MemCpy(&pTable[index], &cacheEntry, sizeof(thciEidRlocCache_t));
                index += sizeof(thciEidRlocCache_t);
            }

            counter++;
        }
    }

    *pDataSize = entries * sizeof(thciRoutes_t) + 1;
    *pReplyData = (uint8_t *)pTable;
}

/*!*************************************************************************************************
\private
\fn     void THCI_IfconfigAll(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to show all interfaces with their IP addresses..

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_IfconfigAll
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint8_t *pIfconfigAllRes = NULL;
    uint32_t cBytes = 0, cIndex = 0;
    uint8_t i, j, iIf = 0;
    uint8_t ifconfigInfo[IP_IF_NB];
    ip6IfAddrData_t *pIp6AddrData;
    ifHandle_t ifHandle;
    ipIfUniqueId_t ifUniqueId;
#if IP_IP4_ENABLE
    ip4IfAddrData_t *pIp4AddrData;
#endif
    /* Number of addresses excluding anycast */
    uint8_t ifconfigNoAny[IP_IF_NB];

    *pDataSize = 0x00;
    FLib_MemSet(ifconfigInfo, 0, IP_IF_NB);
    FLib_MemSet(ifconfigNoAny, 0, IP_IF_NB);

    /* Count addresses */
    for (i = 0; i < IP_IF_NB; i++)
    {
        ifHandle = IP_IF_GetIfByIndex(i);

        if ((NULL != ifHandle) && (ifHandle->ifUniqueId != gIpIfUndef_c)) /* validate ifHandle */
        {
            iIf++;
            ifUniqueId = ifHandle->ifUniqueId;
            /* Get the number of addresses, including anycast */
            ifconfigInfo[i] = IP_IF_GetAddrNbByIfIndex6(i, TRUE);
            /* Get the number of addresses, excluding anycast */
            ifconfigNoAny[i] = IP_IF_GetAddrNbByIfIndex6(i, FALSE);
#if IP_IP4_ENABLE
            pIp4AddrData = IP_IF_GetAddrByIf4(ifHandle->ifUniqueId, 0);

            if (pIp4AddrData)
            {
                ifconfigNoAny[i] += 1;
            }
#endif

            /* Link-local all-Thread-nodes */
            if (IP_IF_IsMyMulticastGroupAddr6(ifUniqueId, (ipAddr_t *)&in6addr_linklocal_allthreadnodes))
            {
                ifconfigNoAny[i] += 1;
            }

            /* Realm-Local All Thread Nodes */
            if (IP_IF_IsMyMulticastGroupAddr6(ifUniqueId, (ipAddr_t *)&in6addr_realmlocal_allthreadnodes))
            {
                ifconfigNoAny[i] += 1;
            }

            /* Reserve space for addresses (excluding anycast) + interfaceId +
               countIpAddrs */
            cBytes += ((ifconfigNoAny[i] * sizeof(ipAddr_t)) + 2);
        }
    }

    /* Allocate output buffer */
    pIfconfigAllRes = MEM_BufferAlloc(cBytes + 1); /* countInterfaces */

    if (!pIfconfigAllRes)
    {
        return;
    }

    pIfconfigAllRes[cIndex] = iIf; /* count interfaces */
    cIndex++;

    /* Loop through addresses */
    for (i = 0; i < IP_IF_NB; i++)
    {
        ifHandle = IP_IF_GetIfByIndex(i);
        ifUniqueId = ifHandle->ifUniqueId;
        pIfconfigAllRes[cIndex] = i; /* interface id */
        cIndex++;
        pIfconfigAllRes[cIndex] = ifconfigNoAny[i]; /* count IP addresses, excluding anycast */
        cIndex++;
#if IP_IP4_ENABLE
        /* Get IPv4 address for an interface */
        pIp4AddrData = IP_IF_GetAddrByIf4(ifHandle->ifUniqueId, 0);

        if (pIp4AddrData)
        {
            ipAddr_t inIpAddr;
            NWKU_ConvertIp4Addr(pIp4AddrData->ip4Addr, &inIpAddr);
            FLib_MemCpyReverseOrder(&pIfconfigAllRes[cIndex], &inIpAddr.addr8[0], sizeof(ipAddr_t));
            cIndex += sizeof(ipAddr_t);
        }

#endif

        for (j = 0; j < ifconfigInfo[i]; j++)
        {
            /* Get IPv6 addresses for an interface, excluding anycast addresses */
            pIp6AddrData = IP_IF_GetAddrByIf6(ifUniqueId, j, FALSE);

            if (pIp6AddrData)
            {
                if (pIp6AddrData->ip6Addr.addr8[14] != 0xFC)
                {
                    FLib_MemCpyReverseOrder(&pIfconfigAllRes[cIndex], &pIp6AddrData->ip6Addr.addr8[0], sizeof(ipAddr_t));
                    cIndex += sizeof(ipAddr_t);
                }
            }
        }

        /* Link-local all-Thread-nodes */
        if (IP_IF_IsMyMulticastGroupAddr6(ifUniqueId, (ipAddr_t *)&in6addr_linklocal_allthreadnodes))
        {
            FLib_MemCpyReverseOrder(&pIfconfigAllRes[cIndex], &in6addr_linklocal_allthreadnodes, sizeof(ipAddr_t));
            cIndex += sizeof(ipAddr_t);
        }

        /* Realm-Local All Thread Nodes */
        if (IP_IF_IsMyMulticastGroupAddr6(ifUniqueId, (ipAddr_t *)&in6addr_realmlocal_allthreadnodes))
        {
            FLib_MemCpyReverseOrder(&pIfconfigAllRes[cIndex], &in6addr_realmlocal_allthreadnodes, sizeof(ipAddr_t));
            cIndex += sizeof(ipAddr_t);
        }
    }

    /* Set up reply data */
    *pReplyData = (uint8_t *)pIfconfigAllRes;
    *pDataSize = cIndex;
}

/*!*************************************************************************************************
\private
\fn     void THCI_IfconfigBind(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to bind an IP address to a configured interface.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_IfconfigBind
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    ipAddr_t ipAddress;
    ifHandle_t ifHandle;
    uint32_t bindStatus;

    /* Cast received data to a known structure */
    ifconfigBindParams_t *pIfconfigParams = (ifconfigBindParams_t *)pClientPacket;

    pReplyData->status = mTHCI_Err_c;

    /* Reverse address bytes */
    FLib_MemCpyReverseOrder(ipAddress.addr8, pIfconfigParams->ipAddr.addr8, sizeof(ipAddr_t));

    /* Check IP address */
    if (!IP_IsAddrIPv6(&ipAddress))
    {
        //shell_write("Malformed IP address");
    }
    else
    {
        ifHandle = IP_IF_GetIfByIndex(pIfconfigParams->ifId);

        if (NULL != ifHandle)
        {
            bindStatus = IP_IF_BindAddr6(ifHandle->ifUniqueId, &ipAddress, ip6AddrTypeManual_c,
                                         IP6_ADDRESS_LIFETIME_INFINITE, 64U);

            switch (bindStatus)
            {
                case gIpOk_c:
                    pReplyData->status = mTHCI_Ok_c;
                    break;

                case gIpNoAddressSpaceError_c:
                    pReplyData->status = mTHCI_NoSpace_c;
                    break;
            }
        }
    }

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     bool_t THCI_Ping(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to send a ping request.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static bool_t THCI_Ping
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    pingReply_t *pPingRes = MEM_BufferAlloc(sizeof(pingReply_t));
    bool_t res = TRUE;
    *pDataSize = 0x00;

    /* If there was already an ongoing ping, stop it and send the new one */
    if (mPingTimerID != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(mPingTimerID);
        mPingTimerID = gTmrInvalidTimerID_c;

        if (NULL != pmQueuedReplyData)
        {
            MEM_BufferFree(pmQueuedReplyData);
            pmQueuedReplyData = NULL;
        }
    }

    if (pPingRes)
    {
        ipAddr_t sourceIpAddress;
        ipPktInfo_t *pIpPktInfo;

        /* Cast received data to a known structure */
        pingParams_t *pPingParams = (pingParams_t *)pClientPacket;

        pPingRes->status = mTHCI_Err_c;

        /* Reverse address bytes */
        FLib_MemCpyReverseOrder(mDstIpAddr.addr8, pPingParams->destIpAddr.addr8, sizeof(ipAddr_t));

        /* Reverse address bytes */
        FLib_MemCpyReverseOrder(sourceIpAddress.addr8, pPingParams->sourceIpAddr.addr8,
                                sizeof(ipAddr_t));

        FLib_MemCpy(&mPingTimeoutMs, pPingParams->timeout, sizeof(uint16_t));

        if (mPingTimeoutMs < SHELL_PING_MIN_TIMEOUT)
        {
            mPingTimeoutMs = SHELL_PING_MIN_TIMEOUT;
        }

        /* Set timeout value in reply packet */
        FLib_MemCpy(&(pPingRes->reply), &mPingTimeoutMs, sizeof(uint16_t));

        /* Set security level */
        if (TRUE == pPingParams->security)
        {
            mPingMacSec = 5;
        }
        else
        {
            mPingMacSec = 0;
        }

        mPingHopLimit = pPingParams->hopLimit;

        /* Destination address was set */
        if (!IP_IsAddrEqual(&mDstIpAddr, &in6addr_any))
        {
            /* Create Ping packet */
            uint16_t length;
            FLib_MemCpy(&length, pPingParams->length, sizeof(uint16_t));
            pIpPktInfo = PING_CreatePktInfo(&mDstIpAddr, length);

            if (NULL != pIpPktInfo)
            {
                /* If we have a specified source address: set it */
                if (!(IP_IsAddrEqual(&sourceIpAddress, &in6addr_any)))
                {
                    IP_AddrCopy(pIpPktInfo->pIpSrcAddr, &sourceIpAddress);
                }

                /* Send packet to ICMP for transmission */
                ICMP_Send(pIpPktInfo, gIcmp6TypeEchoRequest_c, ICMP_CODE_DEFAULT);

                /* Get timestamp */
                mPingTimeStamp = TMR_GetTimestamp();

                if (gTmrInvalidTimerID_c == mPingTimerID)
                {
                    /* Allocate ping timer */
                    mPingTimerID = TMR_AllocateTimer();

                    if (mPingTimerID != gTmrInvalidTimerID_c)
                    {
                        /* Start timer */
                        TMR_StartSingleShotTimer(mPingTimerID, mPingTimeoutMs, PING_TimerCallback, NULL);
                    }
                    else
                    {
                        //ret = CMD_RET_SUCCESS;
                    }
                }
                else
                {
                    //ret = CMD_RET_SUCCESS;
                }

                /* Set up reply data */
                *pReplyData = (uint8_t *)pPingRes;
                res = FALSE;
            }
            else
            {
                /* Set up reply data */
                pPingRes->status = mTHCI_Err_c;
                *pReplyData = (uint8_t *)pPingRes;
            }
        }
        else
        {
            /* Set up reply data */
            pPingRes->status = mTHCI_PingWrongDestAddress_c;
            *pReplyData = (uint8_t *)pPingRes;
        }

        *pDataSize = sizeof(pingReply_t);
    }

    return res;
}

/*!*************************************************************************************************
\private
\fn     void PING_EchoReplyReceive(uint8_t *pParam)
\brief  Interface function for the user app. It handles a received Ping Echo Reply message.

\param  [in]    pIpPktInfo      Pointer to the packet information structure.
***************************************************************************************************/
static void PING_EchoReplyReceive
(
    uint8_t *pParam
)
{
    uint16_t echoId;
    uint16_t seqNb;
    //uint32_t payloadLen;
    uint64_t tempTimestamp;
    ipPktInfo_t *pIpPktInfo = (ipPktInfo_t *)pParam;

    if (pmQueuedReplyData)
    {
        /* Reply from desired IP address */
        if (IP_IsAddrEqual(&mDstIpAddr, pIpPktInfo->pIpSrcAddr) || IP6_IsMulticastAddr(&mDstIpAddr))
        {
            /* Get first the echo request identifier */
            //htonas(pIpPktInfo->pNwkBuff->pData, echoId);
            echoId = ntohas(pIpPktInfo->pNextProt);

            if (echoId == PING_ID)
            {
                /* Get the echo request sequence number */
                //htonas(pIpPktInfo->pNwkBuff->pData + sizeof(echoId), defaultSeqNb);
                seqNb = ntohas(pIpPktInfo->pNextProt + sizeof(echoId));

                if (seqNb == mDefaultSeqNb - 1)
                {
                    /* Get payload length from the ICMP packet.
                     * The ping payload is with an offset of 4 */
                    //payloadLen = pIpPktInfo->prot.nextProtLen - PING_HEADER_SIZE;

                    /* Compare payload */
                    /*if(FLib_MemCmp(pIpPktInfo->pNextProt + sizeof(echoId) + sizeof(seqNb),
                            (void*)PING_PAYLOAD, payloadLen - (sizeof(echoId) + sizeof(seqNb))))
                    {*/

                    tempTimestamp = TMR_GetTimestamp();
                    tempTimestamp -= mPingTimeStamp;
                    tempTimestamp /= 1000;
                    pingReply_t *pPingReplyData;

                    pPingReplyData = (pingReply_t *)pmQueuedReplyData;
                    uint16_t tmstp = (uint16_t)tempTimestamp;

                    //htonas((uint8_t*)pPingReplyData->reply, (uint16_t)tempTimestamp);
                    FLib_MemCpy(pPingReplyData->reply, &tmstp, sizeof(pPingReplyData->reply));

                    /* Stop timer */
                    if (mPingTimerID != gTmrInvalidTimerID_c)
                    {
                        TMR_FreeTimer(mPingTimerID);
                        mPingTimerID = gTmrInvalidTimerID_c;
                    }

                    /* Send reply */
                    ((pingReply_t *)pmQueuedReplyData)->status = mTHCI_Ok_c;
                    /*else
                    {
                        shell_write("Reply payload not matching\n\r");
                    }*/
                }
                else
                {
                    // shell_write("Reply sequence number not matching\n\r");
                    ((pingReply_t *)pmQueuedReplyData)->status = mTHCI_Err_c;
                }
            }
            else
            {
                // shell_write("Reply PING ID not matching\n\r");
                ((pingReply_t *)pmQueuedReplyData)->status = mTHCI_Err_c;
            }
        }
        else
        {
            // shell_write("Reply IP source address not matching\n\r");
            ((pingReply_t *)pmQueuedReplyData)->status = mTHCI_Err_c;
        }

        THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, mQueuedOpCode, pmQueuedReplyData,
                             mQueuedDataSize, mFsciAppInterfaceId);

        MEM_BufferFree(pmQueuedReplyData);
        pmQueuedReplyData = NULL;
    }

    /* Free the input pIpPktInfo  */
    NWKU_FreeIpPktInfo(&pIpPktInfo);
}

/*!*************************************************************************************************
\private
\fn     void PING_TimerCallback(void *param)
\brief  This function sets the timeout expire value.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void PING_TimerCallback(void *param)
{
    (void)NWKU_SendMsg(PING_HandleTimerCallback, NULL, pmMainThreadMsgQueue);
}

/*!*************************************************************************************************
\private
\fn     void PING_HandleTimerCallback(uint8_t *param)
\brief  This function sets the timeout expire value.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void PING_HandleTimerCallback(uint8_t *param)
{
    /* Ping reply was not received */
    //shell_write("Request timed out");
    /* Stop Ping timer */
    if (mPingTimerID != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(mPingTimerID);
        mPingTimerID = gTmrInvalidTimerID_c;
    }

    if (pmQueuedReplyData)
    {
        /* Send reply */
        ((pingReply_t *)pmQueuedReplyData)->status = mTHCI_PingTimeout_c;
        THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, mQueuedOpCode, pmQueuedReplyData,
                             mQueuedDataSize, mFsciAppInterfaceId);

        MEM_BufferFree(pmQueuedReplyData);
        pmQueuedReplyData = NULL;
    }
}

/*!*************************************************************************************************
\private
\fn     ipPktInfo_t *PING_CreatePktInfo(uint32_t payloadLen)
\brief  This function is used to create the packet info structure needed by ICMP ping.

\param  [in]    pDstAddr    pointer to the destination IP address
\param  [in]    payloadLen  the size of the payload

\return         ipPktInfo_t pointer to a packet info structure
***************************************************************************************************/
static ipPktInfo_t *PING_CreatePktInfo(ipAddr_t *pDstAddr, uint32_t payloadLen)
{
    uint16_t echoId = PING_ID;
    ipPktInfo_t *pIpPktInfo = NULL;
    uint16_t idx;
    uint16_t iPayload;
    uint8_t *pPayload;

    pIpPktInfo = NWKU_CreateIpPktInfo();

    if (NULL != pIpPktInfo)
    {
        pIpPktInfo->pIpDstAddr = &pIpPktInfo->ipDstAddr;
        pIpPktInfo->pIpSrcAddr = &pIpPktInfo->ipSrcAddr;

        /* Allocate and populate pIpPktInfo->pNwkBuff using Echo request payload and
           echo request identifier and sequence number */
        pIpPktInfo->pNwkBuff = NWKU_CreateNwkBuffer(payloadLen + sizeof(echoId) + sizeof(mDefaultSeqNb));

        /* validate memory allocation conditions */
        if (NULL != pIpPktInfo->pNwkBuff)
        {
            /* Allocate and populate pIpPktInfo->pIpDstAddr */
            IP_AddrCopy(pIpPktInfo->pIpDstAddr, pDstAddr);

            /* Determine IP source address based on IP destination address */
            if (IP_IsAddrIPv6(pIpPktInfo->pIpDstAddr))
            {
#if IP_IP6_ENABLE
                ipAddr_t *pSrcAddr = IP_IF_SelSrcAddr6(gIpIfUndef_c, pIpPktInfo->pIpDstAddr);

                if (pSrcAddr)
                {
                    IP_AddrCopy(pIpPktInfo->pIpSrcAddr, pSrcAddr);
                }

#endif /* IP_IP6_ENABLE */
            }
            else
            {
#if IP_IP4_ENABLE
                NWKU_ConvertIp4Addr(IP_IF_SelSrcAddr4(pIpPktInfo->pIpDstAddr,
                                    pIpPktInfo->ipPktOptions.ifHandle),
                                    pIpPktInfo->pIpSrcAddr);
#endif /* IP_IP4_ENABLE */
            }

            /* Populate pIpPktInfo->pIpPktOptions */
            pIpPktInfo->ipPktOptions.hopLimit = mPingHopLimit;
            pIpPktInfo->ipPktOptions.security = mPingMacSec;
            // TBD: pIpPktInfo->ipPktOptions.lqi
            // TBD: pIpPktInfo->ipPktOptions.qos

            /* Populate first the echo request identifier */
            htonas(pIpPktInfo->pNwkBuff->pData, echoId);

            /* Populate the echo request sequence number with a default value */
            htonas(pIpPktInfo->pNwkBuff->pData + sizeof(echoId), mDefaultSeqNb++);

            /* Set ping payload: 0x61..0x77(a..w) */
            pPayload = pIpPktInfo->pNwkBuff->pData + sizeof(echoId) + sizeof(mDefaultSeqNb);
            iPayload = 0;

            for (idx = 0; idx < payloadLen; idx++, iPayload++)
            {
                if (iPayload > (PING_PAYLOAD_END - PING_PAYLOAD_START))
                {
                    iPayload = 0;
                }

                pPayload[idx] = iPayload + PING_PAYLOAD_START;
            }
        }
        else
        {
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }

    return pIpPktInfo;
}

#if SERIAL_TUN_IF
/*
 * SERIAL_TUN Interface functions
 */
/*!*************************************************************************************************
\private
\fn     void THCI_SerialtunLinkIndication(uint8_t *pClientPacket, uint32_t interfaceId,
\       statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to initialize the Virtual TUN interface.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_SerialtunLinkIndication
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    uint8_t linkState = pClientPacket[0];
    ifHandle_t ifHandle = IP_IF_GetIfHandle(gIpIfSerialTun_c);

    if (ifHandle != NULL)
    {
        ndCfg_t **ppNdCfg = ND_GetCfg(gIpIfSerialTun_c);
        ndCfg_t *pNdCfg = ppNdCfg ? *ppNdCfg : NULL;

        if (pNdCfg && pNdCfg->ndIf && pNdCfg->ndIf->ndLinkStateChanged)
        {
            pNdCfg->ndIf->ndLinkStateChanged(ifHandle, linkState);
        }
    }

    /* Set up reply data */
    pReplyData->status = gThrStatus_Success_c;
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     bool_t THCI_SerialtunSend(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used transfer the data from FSCI to SERIAL_TUN interface.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static bool_t THCI_SerialtunSend
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint16_t dataSize;
    uint8_t *pData;

    FLib_MemCpy(&dataSize, pClientPacket, sizeof(uint16_t));
    pData = MEM_BufferAlloc(dataSize);

    if (pData)
    {
        FLib_MemCpy(pData, pClientPacket + 2, dataSize);
        IP_serialtunRecv(pData, dataSize);

        /* Set up reply data */
        *pReplyData = NULL;
    }

    *pDataSize = 0;
    return FALSE;
}
#endif /* SERIAL_TUN_IF */

/*!*************************************************************************************************
\private
\fn     void THCI_GetInstancesReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to get the instances.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_GetInstancesReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{

    //To Do
    *pDataSize = 0;
}
/*!*************************************************************************************************
\private
\fn     void THCI_GetAttrReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to get the network parameters.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_GetAttrReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    getAttrReq_t *pGetAttrReq;
    uint32_t attrSize;

    *pDataSize = 0x00;
    pGetAttrReq = (getAttrReq_t *)(pClientPacket);

    if (!THR_GetAttr(pGetAttrReq->instanceId, (thrAttrId_t)pGetAttrReq->attrId, pGetAttrReq->index,
                     &attrSize, NULL))
    {
        uint16_t rspLength;
        getAttrRsp_t *pGetAttrRsp = MEM_BufferAlloc(sizeof(getAttrRsp_t) - 1 + attrSize);

        if (pGetAttrRsp)
        {
            rspLength = sizeof(pGetAttrRsp->instanceId) + sizeof(pGetAttrRsp->attrId) + \
                        sizeof(pGetAttrRsp->index) + sizeof(pGetAttrRsp->status);

            pGetAttrRsp->instanceId = pGetAttrReq->instanceId;
            pGetAttrRsp->attrId = pGetAttrReq->attrId;
            pGetAttrRsp->index = pGetAttrReq->index;
            pGetAttrRsp->status = THR_GetAttr(pGetAttrReq->instanceId, (thrAttrId_t)pGetAttrReq->attrId, pGetAttrReq->index,
                                              &attrSize, (uint8_t *)&pGetAttrRsp->attrValue);
            pGetAttrRsp->size = attrSize;

            if (pGetAttrRsp->status == gThrStatus_Success_c)
            {
                rspLength += sizeof(pGetAttrRsp->size) + pGetAttrRsp->size;
            }

            /* Set up reply data */
            *pReplyData = (uint8_t *)pGetAttrRsp;
            *pDataSize = rspLength;
        }
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_GetDeviceInfo(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to get extended capabilities of the device.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_GetDeviceInfo
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{

    *pDataSize = sizeof(getDevInfoRsp_t);
    getDevInfoReq_t *pGetDevInfoReq = (getDevInfoReq_t *)(pClientPacket);
    getDevInfoRsp_t *pGetDevInfoRsp = MEM_BufferAlloc(sizeof(getDevInfoRsp_t));

    bool_t bDevIsBr =  FALSE;
    bool_t bDevIsNdRouterBr =  FALSE;

    THR_GetAttr(pGetDevInfoReq->instanceId, gNwkAttrId_ThreadNwkCapabilites_c, 0,NULL,
                &pGetDevInfoRsp->nwkCapabilities);

    /* set extended capabilities bits */
    pGetDevInfoRsp->extendedCapabilities = 0;

#if SERIAL_TUN_IF || ENET_IF || USB_RNDIS_IF
    bDevIsBr = TRUE;
#endif
#if BR_ROUTER_MODE
    bDevIsNdRouterBr = TRUE;
#else

#endif

    if (TRUE == bDevIsBr)
    {
        pGetDevInfoRsp->extendedCapabilities |= gThciIsBr_c;
    }

    if (TRUE == bDevIsNdRouterBr)
    {
        pGetDevInfoRsp->extendedCapabilities |= gThciIsBrNdRouter_c;
    }

    *pReplyData = (uint8_t *)pGetDevInfoRsp;

}
/*!*************************************************************************************************
\private
\fn     void THCI_SetAttrReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to add an entry to mac filter table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_SetAttrReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    setAttrReq_t *pSetAttrReq;

    pSetAttrReq = (setAttrReq_t *)(pClientPacket);

    /* Set up reply data */
    if ((thrAttrId_t)pSetAttrReq->attrId == gNwkAttrId_PSKc_c)
    {
        pReplyData->status = THR_SetPskcPassphrase(pSetAttrReq->instanceId, pSetAttrReq->attrValue,
                             pSetAttrReq->size);
    }
    else
    {
        if ((thrAttrId_t)pSetAttrReq->attrId == gNwkAttrId_ThreadNwkCapabilites_c)
        {
            if (SERIAL_TUN_IF && ((*((uint8_t *)pSetAttrReq->attrValue) & THR_NWKCAP_IS_FULL_THREAD_DEVICE) == 0))
            {
                pReplyData->status = gThrStatus_NotPermitted_c;
            }
            else
            {
                pReplyData->status = THR_SetAttr(pSetAttrReq->instanceId, (thrAttrId_t)pSetAttrReq->attrId,
                                                 pSetAttrReq->index, pSetAttrReq->size, pSetAttrReq->attrValue);
            }
        }
        else
        {
            pReplyData->status = THR_SetAttr(pSetAttrReq->instanceId, (thrAttrId_t)pSetAttrReq->attrId,
                                             pSetAttrReq->index, pSetAttrReq->size, pSetAttrReq->attrValue);

            /* Set channel cmd. requires a set of scanchannelmask */
            if ((thrAttrId_t)pSetAttrReq->attrId == gNwkAttrId_Channel_c)
            {
                uint8_t channel = *pSetAttrReq->attrValue;
                uint32_t scanMask = 1 << channel;
                pReplyData->status = THR_SetAttr(pSetAttrReq->instanceId, gNwkAttrId_ScanChannelMask_c,
                                                 pSetAttrReq->index, sizeof(uint32_t), (void *)&scanMask);
            }
        }
    }

    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_SetDeviceConfig(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to set the device's configuration.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_SetDeviceConfig
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thrDeviceConfig_t thrDeviceConfig;
    uint16_t cIndex = 1;

    FLib_MemCpy(&thrDeviceConfig, &pClientPacket[cIndex],
                sizeof(thrDeviceConfig_t));
    cIndex += GetRelAddr(thrDeviceConfig_t, nwkName) + thrDeviceConfig.nwkName.length + 1;
    thrDeviceConfig.MLprefix.prefixLenBits = pClientPacket[cIndex];
    cIndex++;
    FLib_MemCpyReverseOrder(&thrDeviceConfig.MLprefix.prefix, &pClientPacket[cIndex],
                            sizeof(ipAddr_t));
    cIndex += sizeof(ipAddr_t);
    FLib_MemCpyReverseOrder(&thrDeviceConfig.masterKey, &pClientPacket[cIndex], 16);

    /* Set up reply data */
    pReplyData->status = THR_SetDeviceConfig(pClientPacket[0], &thrDeviceConfig);
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_NwkScanReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to do a Scan request.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_NwkScanReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thrNwkScan_t thrNwkScan;

    FLib_MemCpy((uint8_t *)&thrNwkScan, pClientPacket + 1, sizeof(thrNwkScan_t));

    pReplyData->status = THR_NwkScanWithBeacon(((uint8_t *)pClientPacket)[0], &thrNwkScan);

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_NwkCreateReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to create an instance.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_NwkCreateReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = THR_NwkCreate(((uint8_t *)pClientPacket)[0]);

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_FactoryResetReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to reset the device to factory default.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_FactoryResetReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    THR_FactoryReset();
    pReplyData->status = gThrStatus_Success_c;

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_ThrCpuResetReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to reset the device using a specific timeout.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrCpuResetReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    uint32_t timeout = 0;
    FLib_MemCpy(&timeout, pClientPacket, sizeof(uint32_t));
    THR_TimeoutResetMcu(timeout, FALSE);
    pReplyData->status = gThrStatus_Success_c;

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_ChildUpdateToParent(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to send the updated child parameters to a parent.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ChildUpdateToParent
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    instanceId_t thrInstID = ((uint8_t *)pClientPacket)[0];

    pReplyData->status = THR_ChildUpdateToParent(thrInstID);

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_NwkJoinReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to join the network.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_NwkJoinReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = THR_NwkJoin(pClientPacket[0], (thrJoinDiscoveryMethod_t)pClientPacket[1]);
    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_NwkDisconnectReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to disconnect.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_NwkDisconnectReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{

    pReplyData->status = THR_NwkDetach(((uint8_t *)pClientPacket)[0]);
    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_ThrGetParentReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to disconnect.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrGetParentReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    thrNeighbor_t *pParent =  THR_GetParent(((uint8_t *)pClientPacket)[0]);

    *pDataSize = 0x00;

    if (pParent)
    {
        getThrParentRsp_t *pGetThrParentRsp = MEM_BufferAlloc(sizeof(getThrParentRsp_t));

        if (pGetThrParentRsp)
        {
            pGetThrParentRsp->instanceId = (((uint8_t *)pClientPacket)[0]);

            FLib_MemCpy(&pGetThrParentRsp->shortAddr, &pParent->shortAddress, sizeof(uint16_t));
            FLib_MemCpy(&pGetThrParentRsp->extendedAddr, &pParent->extendedAddress, sizeof(uint64_t));
            pGetThrParentRsp->status = gThrStatus_Success_c;

            /* Set up reply data */
            *pReplyData = (uint8_t *)pGetThrParentRsp;
            *pDataSize = sizeof(getThrParentRsp_t);
        }
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_NwkDiscoveryReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function starts the Thread Discovery Procedure.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_NwkDiscoveryReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thrNwkDiscoveryReqTxOpt_t discReqTxOpt;
    instanceId_t thrInstId = pClientPacket[0];

    discReqTxOpt.destPanId = THR_ALL_FFs16;
    THR_GetAttr(thrInstId, gNwkAttr_DiscoveryReqMacTxOptions_c, 0, NULL, (uint8_t *)&discReqTxOpt.discReqTxOpt);
    discReqTxOpt.flags = THR_DISCOVERY_REQ_TLV_J_BIT;
    THR_GetAttr(thrInstId, gNwkAttrId_ScanChannelMask_c, 0, NULL, (uint8_t *)&discReqTxOpt.scanChannelMask);
    discReqTxOpt.pExtraTlvs = NULL;
    discReqTxOpt.extraTlvsLength = 0;

    pReplyData->status = THR_NwkDiscoveryReq(thrInstId, &discReqTxOpt, APP_JoinerDiscoveryRespCb);
    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_NwkDiscoveryStop(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function stops the Thread Discovery Procedure.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_NwkDiscoveryStop
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = THR_NwkDiscoveryStop(pClientPacket[0]);
    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_SearchNwkWithAnnounce(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used in searching for Thread networks using Thread Discovery Procedure.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_SearchNwkWithAnnounce
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    uint32_t scanChannelMask = 1 << 11;

    THR_GetAttr(pClientPacket[0], gNwkAttrId_ScanChannelMask_c, 0, NULL, (uint8_t *)&scanChannelMask);
    pReplyData->status = THR_SearchThreadNwkWithAnnounce(pClientPacket[0],
                         scanChannelMask,
                         APP_JoinerSelectNwkWithAnnounceCb);

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_AddMacFilterEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to add an entry in Mac filtering table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_AddMacFilterEntry
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    uint64_t extendedAddr;
    uint16_t shortAddr;
    addMacFilter_t *pMacFilter = (addMacFilter_t *)pClientPacket;

    FLib_MemCpy(&extendedAddr, pMacFilter->extendedAddr, sizeof(uint64_t));
    FLib_MemCpy(&shortAddr, pMacFilter->shortAddr, sizeof(uint16_t));
    /* Set up reply data */
    pReplyData->status = MacFiltering_AddNeighbor(pMacFilter->instanceId, extendedAddr, shortAddr,
                         pMacFilter->linkIndicator, pMacFilter->blockNeighbor);
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_RemoveMacFilterEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to remove an entry from Mac filtering table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_RemoveMacFilterEntry
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    uint64_t extendedAddr;
    FLib_MemCpy(&extendedAddr, &((uint8_t *)pClientPacket)[1], sizeof(uint64_t));
    /* Set up reply data */
    pReplyData->status = MacFiltering_RemoveNeighbor(((uint8_t *)pClientPacket)[0], extendedAddr);
    *pDataSize = sizeof(statusConfirm_t);

}
/*!*************************************************************************************************
\private
\fn     void THCI_EnableMacFiltering(uint8_t *pClientPacket, uint32_t interfaceId,
                                   statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to remove an entry from Mac filtering table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_EnableMacFiltering
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    /* Set up reply data */
    macFilteringDefault_t filteringType;

    filteringType = (macFilteringDefault_t)((uint8_t *)pClientPacket)[1];
    pReplyData->status = MacFiltering_Active(((uint8_t *)pClientPacket)[0], filteringType);
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_GetMacFilterTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  Get Mac filtering table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_GetMacFilterTable
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    getTableReq_t *pGetMacFilterReq;
    getTableRsp_t *pGetMacFilterRsp;
    uint16_t rspLength;
    uint8_t i;

    *pDataSize = 0x00;
    pGetMacFilterReq = (getTableReq_t *)pClientPacket;
    rspLength = sizeof(getTableRsp_t) - 1;

    if (pGetMacFilterReq->noOfElements + pGetMacFilterReq->startIndex > MAC_FILTERING_TABLE_SIZE)
    {
        pGetMacFilterReq->noOfElements = MAC_FILTERING_TABLE_SIZE - pGetMacFilterReq->startIndex;
    }

    rspLength += pGetMacFilterReq->noOfElements * sizeof(macFilteringNeighborData_t);
    pGetMacFilterRsp = MEM_BufferAlloc(rspLength);

    if (pGetMacFilterRsp)
    {
        uint32_t index;

        pGetMacFilterRsp->instanceId = pGetMacFilterReq->instanceId;
        pGetMacFilterRsp->noOfElements = 0x00;
        index = pGetMacFilterRsp->noOfElements;

        for (i = pGetMacFilterReq->startIndex; i < pGetMacFilterReq->noOfElements; i++)
        {
            macFilteringNeighborData_t macFilterEntry;

            if (THR_GetAttr(pGetMacFilterReq->instanceId, gNwkAttrId_WhiteListEntry_c, i, NULL,
                            (uint8_t *) & (macFilterEntry)) == gThrStatus_Success_c)
            {
                FLib_MemCpy(&pGetMacFilterRsp->pTblEntry[index], &macFilterEntry,
                            sizeof(uint64_t) + sizeof(uint16_t) + 2 * sizeof(uint8_t));
                pGetMacFilterRsp->noOfElements++;
                index =  pGetMacFilterRsp->noOfElements * (sizeof(uint64_t) + sizeof(uint16_t) + 2 * sizeof(uint8_t));
            }
            else
            {
                rspLength -= sizeof(macFilteringNeighborData_t);
            }
        }

        /* Set up reply data */
        *pReplyData = (uint8_t *)pGetMacFilterRsp;
        *pDataSize = rspLength;
    }
}
/*!*************************************************************************************************
\private
\fn     void THCI_LeaderRemoveRouterId(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to remove a specified router id

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_LeaderRemoveRouterId
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
#if STACK_THREAD
    /* payload = InstanceId - 1 byte, routerShortAddr = 2 bytes*/
    uint16_t routerId = pClientPacket[1] + (pClientPacket[2] << 8);
    routerId = THR_SHORT_ADDR_TO_R_ID(routerId);
    pReplyData->status = THR_LeaderRemoveRouterID(pClientPacket[0], routerId);
#else
    pReplyData->status = mTHCI_Err_c;
#endif /* STACK_THREAD */

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_ThrGetNeighborInfo(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to retrieve detailed info about a neighbor

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrGetNeighborInfo
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    getNeighborInfoReq_t *pNeighborInfoReq;
    getNeighborInfoRsp_t *pNeighborInfoRsp;
    thrNeighbor_t *pThrNeighbor = NULL;
    uint32_t iCount;
    uint16_t shortAddr;
    uint32_t thrNeighborTblSize = 0;

    pNeighborInfoReq = (getNeighborInfoReq_t *)pClientPacket;
    pNeighborInfoRsp = MEM_BufferAlloc(sizeof(getNeighborInfoRsp_t));
    thrNeighborTblSize = THR_GetNeighborsTblSize(pNeighborInfoReq->instanceId);

    if (NULL != pNeighborInfoRsp)
    {
        pNeighborInfoRsp->status = gThrStatus_Failed_c;
        FLib_MemCpy(&shortAddr, pNeighborInfoReq->shortAddr, sizeof(uint16_t));

        for (iCount = 0; iCount < thrNeighborTblSize; iCount++)
        {
            pThrNeighbor = THR_GetNeighborTable(iCount);

            if ((NULL != pThrNeighbor) && (pThrNeighbor->shortAddress == shortAddr))
            {
                uint32_t lastCommTime;

                pNeighborInfoRsp->status = gThrStatus_Success_c;
                FLib_MemCpy(pNeighborInfoRsp->extendedAddress, &pThrNeighbor->extendedAddress, 8);
                FLib_MemCpy(pNeighborInfoRsp->shortAddress, &pThrNeighbor->shortAddress, sizeof(uint16_t));
                lastCommTime = ((uint32_t)TmrMicrosecondsToSeconds(TMR_GetTimestamp()) - pThrNeighbor->timestamp);
                FLib_MemCpy(pNeighborInfoRsp->timestamp, &lastCommTime, sizeof(uint32_t));
                pNeighborInfoRsp->inRssi = pThrNeighbor->inLinkMargin;
                FLib_MemCpy(pNeighborInfoRsp->timeoutSec, &pThrNeighbor->timeoutSec, sizeof(uint32_t));
                break;
            }
        }

        /* Set up reply data */
        if (pNeighborInfoRsp->status == gThrStatus_Failed_c)
        {
            *pReplyData = &pNeighborInfoRsp->status;
            *pDataSize = sizeof(uint8_t);
        }
        else
        {
            *pReplyData = (uint8_t *)pNeighborInfoRsp;
            *pDataSize = sizeof(getNeighborInfoRsp_t);
        }
    }

}
/*!*************************************************************************************************
\private
\fn     void THCI_ThrGetChildrenTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to get the children table.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrGetChildrenTable
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint32_t iCount;
    thrNeighbor_t *pThrNeighbor;
    getTableReq_t *pGetNeighborTblReq;
    getTableRsp_t *pGetNeighborTblRsp;
    uint32_t endCounter;
    uint32_t index = 0;
    uint16_t rspLength;
    uint32_t thrNeighborTblSize = 0;
    uint32_t attrLength;
    uint16_t attrShortAddr = THR_ALL_FFs16;
    uint8_t nbOfChildren = 0;

    (void)THR_GetAttr(0, gNwkAttrId_ShortAddr_c, 0, &(attrLength), (uint8_t *)&attrShortAddr);

    pGetNeighborTblReq = (getTableReq_t *)pClientPacket;
    thrNeighborTblSize = THR_GetNeighborsTblSize(pGetNeighborTblReq->instanceId);
    rspLength = sizeof(getTableRsp_t) - 1;

    endCounter = pGetNeighborTblReq->startIndex + pGetNeighborTblReq->noOfElements;

    if (endCounter > thrNeighborTblSize)
    {
        endCounter = thrNeighborTblSize;
    }

    /* Allocate the exact memory needed for the device's children */
    for (iCount = pGetNeighborTblReq->startIndex; iCount < thrNeighborTblSize; iCount++)
    {
        pThrNeighbor = THR_GetNeighborTable(iCount);

        if ((NULL != pThrNeighbor) && (THR_IS_MY_CHILD(pThrNeighbor->shortAddress, attrShortAddr)))
        {
            rspLength += sizeof(thrChildrenTbl_t);
            nbOfChildren++;
            if (nbOfChildren == pGetNeighborTblReq->noOfElements)
            {
                break;
            }
        }
    }

    pGetNeighborTblRsp = MEM_BufferAlloc(rspLength);

    if (NULL != pGetNeighborTblRsp)
    {
        pGetNeighborTblRsp->noOfElements = 0;
        pGetNeighborTblRsp->instanceId = pGetNeighborTblReq->instanceId;

        for (iCount = pGetNeighborTblReq->startIndex; iCount < thrNeighborTblSize; iCount++)
        {
            pThrNeighbor = THR_GetNeighborTable(iCount);

            if ((NULL != pThrNeighbor) && (THR_IS_MY_CHILD(pThrNeighbor->shortAddress, attrShortAddr)))
            {
                thrChildrenTbl_t childrenTblEntry;
                uint32_t lastCommTime;

                FLib_MemCpy(&childrenTblEntry.extendedAddr, &pThrNeighbor->extendedAddress, 8);
                FLib_MemCpy(&childrenTblEntry.shortAddr, &pThrNeighbor->shortAddress, sizeof(uint16_t));
                lastCommTime = ((uint32_t)TmrMicrosecondsToSeconds(TMR_GetTimestamp()) - pThrNeighbor->timestamp);
                FLib_MemCpy(&childrenTblEntry.lastCommTime, &lastCommTime, sizeof(uint32_t));
                childrenTblEntry.lastRSSI = pThrNeighbor->inLinkMargin;
                FLib_MemCpy(&childrenTblEntry.timeout, &pThrNeighbor->timeoutSec, sizeof(uint32_t));

                FLib_MemCpy(&pGetNeighborTblRsp->pTblEntry[index], &childrenTblEntry, sizeof(thrChildrenTbl_t));
                index += sizeof(thrChildrenTbl_t);
                pGetNeighborTblRsp->noOfElements++;

                if (pGetNeighborTblRsp->noOfElements == pGetNeighborTblReq->noOfElements)
                {
                    break;
                }
            }
        }

        /* Set up reply data */
        *pReplyData = (uint8_t *)pGetNeighborTblRsp;
        *pDataSize = rspLength;
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_ThrGetNeighborTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to get the neighbor table.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrGetNeighborTable
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint32_t iCount;
    thrNeighbor_t *pThrNeighbor;
    getTableReq_t *pGetNeighborTblReq;
    getTableRsp_t *pGetNeighborTblRsp;
    uint32_t endCounter;
    uint32_t index = 0;
    uint16_t rspLength;
    uint32_t thrNeighborTblSize = 0;

    pGetNeighborTblReq = (getTableReq_t *)pClientPacket;
    thrNeighborTblSize = THR_GetNeighborsTblSize(pGetNeighborTblReq->instanceId);
    rspLength = sizeof(getTableRsp_t) - 1;

    endCounter = pGetNeighborTblReq->noOfElements + pGetNeighborTblReq->startIndex;

    if (endCounter > thrNeighborTblSize)
    {
        endCounter = thrNeighborTblSize;
    }

    rspLength += endCounter * sizeof(thrNeighborTbl_t);

    pGetNeighborTblRsp = MEM_BufferAlloc(rspLength);

    if (NULL != pGetNeighborTblRsp)
    {
        pGetNeighborTblRsp->noOfElements = 0;

        pGetNeighborTblRsp->instanceId = pGetNeighborTblReq->instanceId;

        for (iCount = pGetNeighborTblReq->startIndex; iCount < endCounter; iCount++)
        {
            pThrNeighbor = THR_GetNeighborTable(iCount);

            if ((NULL != pThrNeighbor) && (0 == (pThrNeighbor->shortAddress & CHILD_ID_MASK)))
            {
                thrNeighborTbl_t neighborTblEntry;
                uint32_t lastCommTime;

                FLib_MemCpy(&neighborTblEntry.extendedAddr, &pThrNeighbor->extendedAddress, 8);
                //FLib_MemCpyReverseOrder(&neighborTblEntry.mlEid, pThrNeighbor->mlIid, 8);
                FLib_MemCpy(&neighborTblEntry.shortAddr, &pThrNeighbor->shortAddress, sizeof(uint16_t));
                lastCommTime = ((uint32_t)TmrMicrosecondsToSeconds(TMR_GetTimestamp()) - pThrNeighbor->timestamp);
                FLib_MemCpy(&neighborTblEntry.lastCommTime, &lastCommTime, sizeof(uint32_t));
                neighborTblEntry.lastRSSI = pThrNeighbor->inLinkMargin;

                FLib_MemCpy(&pGetNeighborTblRsp->pTblEntry[index], &neighborTblEntry, sizeof(thrNeighborTbl_t));
                index += sizeof(thrNeighborTbl_t);
                pGetNeighborTblRsp->noOfElements++;
            }
            else
            {
                rspLength -= sizeof(thrNeighborTbl_t);
            }
        }

        /* Set up reply data */
        *pReplyData = (uint8_t *)pGetNeighborTblRsp;
        *pDataSize = rspLength;

    }
}
#if !THREAD_ED_CONFIG
/*!*************************************************************************************************
\private
\fn     void THCI_ThrGetRoutingTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to get the routing table.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrGetRoutingTable
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    getTableReq_t *pGetRoutingTblReq = NULL;
    getRoutingTblRsp_t *pGetRoutingTblRsp;
    thrRouterIdSet_t *pRouterIdSet = NULL;
    uint16_t rspLength;
    uint8_t i;
    uint32_t routerCount = 0;
    thrInterfaceSet_t *pThrIfTbl = NULL;

    *pDataSize = 0x00;
    pGetRoutingTblReq = (getTableReq_t *)pClientPacket;
    rspLength = sizeof(getRoutingTblRsp_t) - 1;

    if ((pGetRoutingTblReq->noOfElements + pGetRoutingTblReq->startIndex) > THR_MAX_ROUTER_ID)
    {
        pGetRoutingTblReq->noOfElements = THR_MAX_ROUTER_ID - pGetRoutingTblReq->startIndex;
    }

    rspLength += pGetRoutingTblReq->noOfElements * sizeof(sendRoutingTblInfo_t);
    pGetRoutingTblRsp = MEM_BufferAlloc(rspLength);

    pRouterIdSet = THR_GetRouterIdSet(pGetRoutingTblReq->instanceId);
    pThrIfTbl = (thrInterfaceSet_t *)THR_GetRoutingInterfaceParams(pGetRoutingTblReq->instanceId);

    if ((NULL != pRouterIdSet) && (NULL != pGetRoutingTblRsp))
    {
        pGetRoutingTblRsp->idSeqNb = pRouterIdSet->thrIdSeqNb;
        FLib_MemCpyReverseOrder(&pGetRoutingTblRsp->routerIdMask, &pRouterIdSet->thrIdSet, 8);

        if (pGetRoutingTblRsp)
        {
            uint32_t index;

            //pGetRoutingTblRsp->instanceId = pGetRoutingTblReq->instanceId;
            pGetRoutingTblRsp->noOfElements = 0x00;
            index = pGetRoutingTblRsp->noOfElements;

            for (i = pGetRoutingTblReq->startIndex; i < (pGetRoutingTblReq->startIndex + pGetRoutingTblReq->noOfElements); i++)
            {
                if (pRouterIdSet->thrIdSet[i / 8] & 0x80 >> (i % 8))
                {
                    if (THR_SHORT_ADDR_TO_R_ID(pThrIfTbl->deviceShortAddr) != i)
                    {
                        sendRoutingTblInfo_t routingTblEntry;
                        uint16_t routerShortAddr;

                        routerShortAddr = THR_R_ID_TO_SHORT_ADDR(i);
                        routingTblEntry.routerId = i;
                        FLib_MemCpy(&routingTblEntry.shortAddr, &routerShortAddr, sizeof(uint16_t));
                        FLib_MemCpy(&routingTblEntry.nextHop, &pThrIfTbl->pThreadRoutingTbl[i].thrNextHopRouterId, sizeof(uint16_t));
                        routingTblEntry.cost = (*((uint8_t *)pRouterIdSet + sizeof(thrRouterIdSet_t) + routerCount)) & 0x0F;
                        routingTblEntry.nIn = ((*((uint8_t *)pRouterIdSet + sizeof(thrRouterIdSet_t) + routerCount)) & 0x30) >> 4;
                        routingTblEntry.nOut = ((*((uint8_t *)pRouterIdSet + sizeof(thrRouterIdSet_t) + routerCount)) & 0xC0) >> 6;

                        FLib_MemCpy(&pGetRoutingTblRsp->routingTable[index], &routingTblEntry, sizeof(sendRoutingTblInfo_t));
                        pGetRoutingTblRsp->noOfElements++;
                        index =  pGetRoutingTblRsp->noOfElements * (sizeof(sendRoutingTblInfo_t));

                    }

                    routerCount ++;
                }
                else
                {
                    rspLength -= sizeof(sendRoutingTblInfo_t);
                }

            }

            /* Set up reply data */
            *pReplyData = (uint8_t *)pGetRoutingTblRsp;
            *pDataSize = rspLength;
        }
    }

    MEM_BufferFree(pRouterIdSet);
}
#endif
/*!*************************************************************************************************
\private
\fn     void THCI_ThrSwitchKey(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to switch thread key

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrSwitchKey
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
#if STACK_THREAD
    uint32_t thrKeySequenceCounter;

    FLib_MemCpy(&thrKeySequenceCounter, &pClientPacket[1], sizeof(uint32_t));

    if (thrKeySequenceCounter != THR_ALL_FFs32)
    {
        pReplyData->status = THR_NwkSwitchKey(pClientPacket[0], thrKeySequenceCounter);
    }

#else
    pReplyData->status = mTHCI_Err_c;
#endif /* STACK_THREAD */

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_ThrSwitchKey(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to switch thread key

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrReattach
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
#if STACK_THREAD
    pReplyData->status = THR_NwkAttach(pClientPacket[0]);
#else
    pReplyData->status = mTHCI_Err_c;
#endif /* STACK_THREAD */

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);

}

#if !THREAD_ED_CONFIG
/*!*************************************************************************************************
\private
\fn     void THCI_BrPrefixAttrAddEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to set an attribute in server data table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BrPrefixAttrAddEntry
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thrOtaBrPrefixSet_t *pOtaBrPrefixSet = (thrOtaBrPrefixSet_t *)&pClientPacket[1];

    /* set the on-mesh flag by default, as the mechanism for handling not on-mesh prefixes
    is not supported*/
    //if (0 == THR_BR_FLAGS_GET(pOtaBrPrefixSet->thrBrPrefixFlags[1], THR_BR_PREFIX_FLAGS_P_ON_MESH_MASK, THR_BR_PREFIX_FLAGS_P_ON_MESH_OFFSET))
    //{
    //THR_BR_FLAGS_SET(pOtaBrPrefixSet->thrBrPrefixFlags[1], 1, THR_BR_PREFIX_FLAGS_P_ON_MESH_MASK, THR_BR_PREFIX_FLAGS_P_ON_MESH_OFFSET);
    //}

    /* swap bytes for endianess consistency between shell and thci */
    NWKU_SwapArrayBytes(pOtaBrPrefixSet->thrBrPrefixLifetime, 4);

    pReplyData->status = THR_BrPrefixAttrAddEntry(pClientPacket[0], pOtaBrPrefixSet);

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_BrServiceAttrAddEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to set an attribute in server data table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BrServiceAttrAddEntry
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thciNwkDataService_t *pServiceInfo = (thciNwkDataService_t *)pClientPacket;

    thrLocalServiceSet_t thrServerSet = {.thrSenterpriseNumber = THREAD_ENTERPRISE_NUMBER_ARRAY,
                                         .thrSserviceDataLen   = 9,
                                         .thrSserviceData      = {0},
                                         .thrSserver16Addr     = {0xFF, 0xFF},
                                         .thrSserverDataLen    = 16,
                                         .thrSserverData       = {0},
                                         .thrSserviceId        = 0,
                                         .thrSstable           = TRUE
                                        };

    FLib_MemCpy(&thrServerSet.thrSserverData, pServiceInfo->serverAddr, sizeof(ipAddr_t));
    FLib_MemCpy(&thrServerSet.thrSserviceData, pServiceInfo->serviceName, 9);

    pReplyData->status = THR_ServiceAttrAddEntry(pServiceInfo->instanceId, &thrServerSet);

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_BrPrefixAttrRemoveEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to remove an attribute from server data table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BrPrefixAttrRemoveEntry
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = THR_BrPrefixAttrRemoveEntry(pClientPacket[0],
                         pClientPacket[1],
                         &pClientPacket[2]);
    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_BrServiceAttrRemoveEntry(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to remove an attribute from server data table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BrServiceAttrRemoveEntry
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thciNwkDataService_t *pServiceInfo = (thciNwkDataService_t *)pClientPacket;


    pReplyData->status = THR_BrServiceAttrRemoveEntry(pServiceInfo->instanceId, pServiceInfo->serviceName,
                         sizeof(pServiceInfo->serviceName), pServiceInfo->serverAddr, sizeof(ipAddr_t));
    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_BrPrefixAttrGetTable(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  Get border router prefix table

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BrPrefixAttrGetTable
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    getBrNwkDataReq_t *pGetBrNwkDataReq;
    getBrNwkDataRsp_t *pGetBrNwkDataRsp;
    uint16_t rspLength;

    *pDataSize = 0x00;
    pGetBrNwkDataReq = (getBrNwkDataReq_t *)pClientPacket;
    rspLength = sizeof(getBrNwkDataRsp_t) - 1;

    if (pGetBrNwkDataReq->noOfElements + pGetBrNwkDataReq->startIndex > THR_SERVER_DATA_PREFIX_TBL_SIZE)
    {
        /* set the max returned number */
        pGetBrNwkDataReq->noOfElements = THR_SERVER_DATA_PREFIX_TBL_SIZE - pGetBrNwkDataReq->startIndex;
    }

    rspLength += pGetBrNwkDataReq->noOfElements * sizeof(thrOtaBrPrefixSet_t);
    pGetBrNwkDataRsp = MEM_BufferAlloc(rspLength);

    if (pGetBrNwkDataRsp)
    {
        pGetBrNwkDataRsp->instanceId = pGetBrNwkDataReq->instanceId;
        pGetBrNwkDataRsp->noOfElements = 0x00;

        THR_BrPrefixAttrGetTable(
            pGetBrNwkDataReq->instanceId,
            pGetBrNwkDataReq->startIndex,
            pGetBrNwkDataReq->noOfElements,
            &pGetBrNwkDataRsp->noOfElements,
            pGetBrNwkDataRsp->brNwkDataTable
        );

        /* Set up reply data */
        rspLength = sizeof(getBrNwkDataRsp_t) - 1 + pGetBrNwkDataRsp->noOfElements * sizeof(thrOtaBrPrefixSet_t);
        *pReplyData = (uint8_t *)pGetBrNwkDataRsp;
        *pDataSize = rspLength;
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_BrPrefixAttrSync(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to synchronize Border Router prefix set with
            over the air network data information

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BrPrefixAttrSync
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = THR_BrPrefixAttrSync(pClientPacket[0]);
    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_BrPrefixAttrSync(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to remove remove all border router prefix attr entries

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_BrPrefixAttrRemoveAll
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = THR_BrPrefixAttrRemoveAll(pClientPacket[0]);
    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

#endif /* !THREAD_ED_CONFIG */

/*!*************************************************************************************************
\private
\fn     void THCI_ThrSetThreshold32(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to set thread Thresholds that are 4 bytes long. Some
thresholds used by Thread 1.2 are bigger than 255.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrSetThreshold32
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = gThrStatus_Success_c;

    uint32_t thresholdValue;
    FLib_MemCpy(&thresholdValue, &pClientPacket[2], sizeof(uint32_t));

    /* req payload format: instanceId[1 Byte], threshold type [1 Byte], data[4 Bytes] */
    switch (pClientPacket[1])
    {
        case thrContextReuseDelay_c:
            THR_SetContextReuseDelaySec(thresholdValue);
            break;
        default:
            pReplyData->status = gThrStatus_InvalidParam_c;
            break;
    }

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     void THCI_ThrSetThreshold(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to set thread Thresholds that are 1 byte long.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrSetThreshold
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = gThrStatus_Success_c;

    uint8_t thresholdValue = pClientPacket[2];

    /* req payload format: instanceId[1 Byte], threshold type [1 Byte], data[1 Byte] */
    switch (pClientPacket[1])
    {
#if !THREAD_ED_CONFIG

        case thrRouterUpgradeThreshold_c:

            /* Upgrade threshold must be smaller than Downgrade threshold */
            if (thresholdValue > THR_GetRouterDowngradeThreshold())
            {
                pReplyData->status = gThrStatus_InvalidParam_c;
            }
            else
            {
                THR_SetRouterUpgradeThreshold(thresholdValue);
            }

            break;

        case thrRouterDowngradeThreshold_c:

            /* Downgrade threshold must be higher than Upgrade threshold */
            if (thresholdValue < THR_GetRouterUpgradeThreshold())
            {
                pReplyData->status = gThrStatus_InvalidParam_c;
            }
            else
            {
                THR_SetRouterDowngradeThreshold(thresholdValue);
            }

            break;

        case thrMinDowngradeNeigbors_c:
            THR_SetMinDowngradNeigbors(thresholdValue);
            break;

        case thrMaxAllowedRouters_c:
            THR_SetMaxAllowedRouters(thresholdValue);
            break;
#endif /* THREAD_ED_CONFIG */

        default:
            pReplyData->status = gThrStatus_InvalidParam_c;
            break;
    }

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_ThrGenerateKey(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to switch thread key

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrGenerateKey
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
#if STACK_THREAD
    uint32_t thrKeySequenceCounter;

    FLib_MemCpy(&thrKeySequenceCounter, &pClientPacket[1], sizeof(uint32_t));

    if (thrKeySequenceCounter != THR_ALL_FFs32)
    {
        pReplyData->status = THR_SetAttr(pClientPacket[0], gNwkAttrId_NwkKeySeq_c, 0,
                                         sizeof(thrKeySequenceCounter) , (uint8_t *)&thrKeySequenceCounter);

        if (pReplyData->status == gThrStatus_Success_c)
        {
            pReplyData->status = THR_GenerateAllKeys(pClientPacket[0], thrKeySequenceCounter);
        }
    }

#else
    pReplyData->status = mTHCI_Err_c;
#endif /* STACK_THREAD */

    /* Set up reply data */
    *pDataSize = sizeof(statusConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_GetThrIpAddr(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to get Thread Ip Address.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_GetThrIpAddr
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    getThrIpAddrReq_t *pGetThrIpAddrReq = (getThrIpAddrReq_t *)pClientPacket;
    getThrIpAddrRsp_t *pGetThrIpAddrRsp;
    uint16_t rspLength;
    uint8_t i, addrCount;
    ipAddr_t ipAddr;
    bool_t bMultipleAddresses = FALSE;

    *pDataSize = 0x00;
    rspLength = sizeof(getThrIpAddrRsp_t) - 1;

    addrCount = THR_NumOfIP6Addr(pGetThrIpAddrReq->instanceId, (nwkIPAddrType_t)pGetThrIpAddrReq->addrType);

    if ((pGetThrIpAddrReq->addrType == gGUAAddr_c) ||
        (pGetThrIpAddrReq->addrType == gAnycastAddr_c))
    {
        bMultipleAddresses = TRUE;
    }

    if (TRUE == bMultipleAddresses)
    {
        addrCount = (addrCount > pGetThrIpAddrReq->noOfElements) ? pGetThrIpAddrReq->noOfElements : addrCount;
    }

    rspLength += (sizeof(ipAddr_t) * addrCount);

    /* Allocate output buffer */
    pGetThrIpAddrRsp = MEM_BufferAlloc(rspLength);

    if (pGetThrIpAddrRsp)
    {
        /* complete rsp */
        pGetThrIpAddrRsp->instanceId = pGetThrIpAddrReq->instanceId;
        pGetThrIpAddrRsp->addrType = pGetThrIpAddrReq->addrType;
        pGetThrIpAddrRsp->noOfIpAddr = 0x00;

        if (FALSE == bMultipleAddresses)
        {
            pGetThrIpAddrRsp->noOfIpAddr = THR_GetIP6Addr(pGetThrIpAddrReq->instanceId, (nwkIPAddrType_t)pGetThrIpAddrReq->addrType, &ipAddr, NULL);
            pGetThrIpAddrRsp->status = gThrStatus_Failed_c;

            if (pGetThrIpAddrRsp->noOfIpAddr != 0)
            {
                pGetThrIpAddrRsp->status = gThrStatus_Success_c;
                FLib_MemCpyReverseOrder(&pGetThrIpAddrRsp->ipAddr[0], &ipAddr.addr8[0], sizeof(ipAddr_t));
            }
        }
        else if (pGetThrIpAddrReq->addrType == gGUAAddr_c)
        {
            for (i = pGetThrIpAddrReq->startIndex; i < (pGetThrIpAddrReq->startIndex + addrCount); i++)
            {
                if (THR_GetGuaByIndex(pGetThrIpAddrReq->instanceId, i, &ipAddr.addr8[0]) == gThrStatus_Success_c)
                {
                    FLib_MemCpyReverseOrder(&pGetThrIpAddrRsp->ipAddr[(i - pGetThrIpAddrReq->startIndex)*sizeof(ipAddr_t)],
                                            &ipAddr.addr8[0], sizeof(ipAddr_t));
                    pGetThrIpAddrRsp->noOfIpAddr++;
                }
            }

            pGetThrIpAddrRsp->status = gThrStatus_Success_c;
        }
        else if (pGetThrIpAddrReq->addrType == gAnycastAddr_c)
        {
            for (i = pGetThrIpAddrReq->startIndex; i < (pGetThrIpAddrReq->startIndex + addrCount); i++)
            {
                if (THR_GetAnycastByIndex(pGetThrIpAddrReq->instanceId, i, &ipAddr.addr8[0]) == gThrStatus_Success_c)
                {
                    FLib_MemCpyReverseOrder(&pGetThrIpAddrRsp->ipAddr[(i - pGetThrIpAddrReq->startIndex)*sizeof(ipAddr_t)],
                                            &ipAddr.addr8[0], sizeof(ipAddr_t));
                    pGetThrIpAddrRsp->noOfIpAddr++;
                }
            }

            pGetThrIpAddrRsp->status = gThrStatus_Success_c;
        }


        /* Set up reply data */
        *pReplyData = (uint8_t *)pGetThrIpAddrRsp;
        *pDataSize = rspLength;
    }
}

#if THR_ENABLE_MGMT_DIAGNOSTICS
/*!*************************************************************************************************
\private
\fn     void THCI_MgmtDiagnosticGetReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to send a MgmtDiagnostic_GetReq command

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_MgmtDiagnosticGetReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusCoapConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    uint16_t msgId = 0;

    pReplyData->status = MgmtDiagnostic_SendGetReq(pClientPacket[0],
                         (mgmtDiagnostic_CoapMsg_t *)&pClientPacket[1],
                         &msgId);

    FLib_MemCpy(pReplyData->msgId, &msgId, sizeof(msgId));
    /* Set up reply data */
    *pDataSize = sizeof(statusCoapConfirm_t);
}
/*!*************************************************************************************************
\private
\fn     void THCI_MgmtDiagnosticResetReq(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to send a MgmtDiagnostic_ResetReq command

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_MgmtDiagnosticResetReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusCoapConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{

    uint16_t msgId = 0;

    pReplyData->status = MgmtDiagnostic_SendResetReq(pClientPacket[0],
                         (mgmtDiagnostic_CoapMsg_t *)&pClientPacket[1],
                         &msgId);

    FLib_MemCpy(pReplyData->msgId, &msgId, sizeof(msgId));
    /* Set up reply data */
    *pDataSize = sizeof(statusCoapConfirm_t);
}

static void THCI_MgmtReadMemoryReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    uint8_t *pSrc = NULL;

    FLib_MemCpy(&pSrc, &pClientPacket, sizeof(uint8_t *));
    *pDataSize = pClientPacket[4];
    *pReplyData = MEM_BufferAlloc(*pDataSize);

    if (*pReplyData)
    {
        FLib_MemCpy(*pReplyData, pSrc, *pDataSize);
    }
}

static void THCI_MgmtWriteMemoryReq
(
    uint8_t *pClientPacket
)
{
    uint8_t *pDest = NULL;
    FLib_MemCpy(&pDest, &pClientPacket[0], sizeof(uint8_t *));
    FLib_MemCpy(pDest, &pClientPacket[5], pClientPacket[4]);
}

static void THCI_MgmtDiagnosticDiagTestReq
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusCoapConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    uint16_t msgId = 0;

    pReplyData->status = MgmtDiagnostic_DiagTestReq(pClientPacket[0],
                         (mgmtDiagnostic_CoapMsg_t *)&pClientPacket[1],
                         &msgId);

    FLib_MemCpy(pReplyData->msgId, &msgId, sizeof(msgId));
    /* Set up reply data */
    *pDataSize = sizeof(statusCoapConfirm_t);
}
#endif

/*!*************************************************************************************************
\private
\fn     THCI_ThrIdentify(uint8_t *pClientPacket, uint32_t interfaceId, statusCoapConfirm_t *pReplyData,
                         uint16_t *pDataSize)
\brief  This function is used to identify the board by its TX LED.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_ThrIdentify
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusCoapConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    mThciBoardIdentify = !mThciBoardIdentify;
    Led_Identify(mThciBoardIdentify);

    pReplyData->status = gThrStatus_Success_c;

    /* Set up reply data */
    *pDataSize = sizeof(statusCoapConfirm_t);
}

/*!*************************************************************************************************
\private
\fn     THCI_OOBNwkForm
\brief  This function is used to switch thread key

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         TRUE            reply is needed
\return         FALSE           reply will be sent later
***************************************************************************************************/
static bool_t THCI_OOBNwkForm
(
    uint8_t *pClientPacket,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    uint8_t *pData = pClientPacket;
    meshcopNwkFormParams_t sendNwkForm;
    nwkStatus_t nwkStatus;
    bool_t replyNow;

    sendNwkForm.instanceId = *pData;
    pData++;
    sendNwkForm.networkNameSize = *pData;
    pData++;
    sendNwkForm.pNwkName = pData;
    pData += sendNwkForm.networkNameSize;
    sendNwkForm.masterKeySize = *pData;
    pData++;
    sendNwkForm.pMasterKey = pData;
    pData += sendNwkForm.masterKeySize;
    sendNwkForm.pskcSize = *pData;
    pData++;
    sendNwkForm.pPskc = pData;
    pData += sendNwkForm.pskcSize;

    if (*pData)
    {
        pData++;
        sendNwkForm.channel = *pData;
    }
    else
    {
        sendNwkForm.channel = 0;
    }

    sendNwkForm.pfGetCb = THCI_MeshcopNetFormCb;

    nwkStatus = MESHCOP_SendNetworkForm(&sendNwkForm);

    /* Set up reply data */
    if (nwkStatus == gNwkStatusSuccess_c)
    {
        pReplyData->status = gThrStatus_Success_c;
        replyNow = FALSE;
    }
    else
    {
        pReplyData->status = gThrStatus_Failed_c;
        replyNow = TRUE;
    }

    *pDataSize = sizeof(statusConfirm_t);

    return replyNow;
}

/*!*************************************************************************************************
\private
\fn     bool_t THCI_CoapSend(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to send a COAP message.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static bool_t THCI_CoapCreateInstance
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    thciCoapCreateInstance_t *pCoapInstanceParam = (thciCoapCreateInstance_t *)pClientPacket;
    sockaddrStorage_t coapParams = {0};
    uint16_t port = 0;

    FLib_MemCpy(&port, pCoapInstanceParam->udpPort, sizeof(uint16_t));
    NWKU_SetSockAddrInfo(&coapParams, NULL, pCoapInstanceParam->addrFamily, port, 0, gIpIfSlp0_c);
    pReplyData->status = gThrStatus_Failed_c;

    mCoapInstId = COAP_CreateInstance(NULL, &coapParams, NULL, 0);

    if (THR_ALL_FFs8 != mCoapInstId)
    {
        pReplyData->status = gThrStatus_Success_c;
    }

    *pDataSize = sizeof(statusConfirm_t);

    return TRUE;
}
#if !THREAD_ED_CONFIG
/*!*************************************************************************************************
\private
\fn     bool_t THCI_CoapPromoteAsRouter(uint8_t *pClientPacket, uint32_t interfaceId,
                                           uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to send an address solicit CoAP message.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static bool_t THCI_CoapPromoteAsRouter
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = THR_PromoteAsRouter(pClientPacket[0], pClientPacket[1]);;
    *pDataSize = sizeof(statusConfirm_t);

    return TRUE;
}
#endif

/*!*************************************************************************************************
\private
\fn     bool_t THCI_CoapSend(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to send a COAP message.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static bool_t THCI_CoapSend
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    coapSession_t *coapSession = NULL;
    uint32_t iCount = 0;


    thciCoapStruct_t *pCoapSendParams = (thciCoapStruct_t *)pClientPacket;
    pReplyData->status = gThrStatus_Failed_c;

    if (THR_ALL_FFs8 != mCoapInstId)
    {
        coapSession = COAP_OpenSession(mCoapInstId);
    }

    if (NULL != coapSession)
    {

        FLib_MemCpyReverseOrder(&coapSession->remoteAddrStorage.ss_addr, &pCoapSendParams->remoteAddr, sizeof(ipAddr_t));
        coapSession->code = (coapReqRespCodes_t)pCoapSendParams->code;
        coapSession->msgType = (coapMessageTypes_t)pCoapSendParams->requestType;
        COAP_SetCallback(coapSession, THCI_CoapReceiveCb);

        /* Keep URI-path as a const string in order to reduce memory allocation and to allow
        URI-paths up to 30 characters long */
        for (uint32_t iCount = 0; iCount < THCI_COAP_MAX_URI_PATHS; iCount++)
        {
            if ((aCoapUriPathStruct[iCount].length == 0)
                    || ((FLib_MemCmp(aCoapUriPathStruct[iCount].pUriPath, &pCoapSendParams->uriPath, aCoapUriPathStruct[iCount].length))
                        && (aCoapUriPathStruct[iCount].length == strlen((char *)pCoapSendParams->uriPath))))
            {
                aCoapUriPathStruct[iCount].length = strlen((char *)pCoapSendParams->uriPath);

                if (aCoapUriPathStruct[iCount].length != 0)
                {
                    FLib_MemCpy(aCoapUriPaths[iCount], &pCoapSendParams->uriPath, aCoapUriPathStruct[iCount].length);
                    aCoapUriPathStruct[iCount].pUriPath = aCoapUriPaths[iCount];
                    coapSession->pUriPath = &aCoapUriPathStruct[iCount];
                }

                break;
            }
        }

        if (gNwkStatusSuccess_c == COAP_Send(coapSession, gCoapMsgTypeUseSessionValues_c, &pCoapSendParams->payload, pCoapSendParams->payloadLen))
        {
            pReplyData->status = gThrStatus_Success_c;

            /* Free the space allocated for URI-path if the message is NON. If the message is CON,
            the URI_path is still needed for retransmissions */
            if (pCoapSendParams->requestType == gCoapNonConfirmable_c)
            {
                FLib_MemSet(&aCoapUriPathStruct[iCount], 0, sizeof(coapUriPath_t));
                FLib_MemSet(aCoapUriPaths[iCount], 0, THCI_COAP_MAX_URI_PATH_OPT_SIZE);
            }
        }
    }

    *pDataSize = sizeof(statusConfirm_t);

    return TRUE;
}

/*!*************************************************************************************************
\private
\fn     bool_t THCI_SendProactiveAddrNotify(uint8_t *pClientPacket, uint32_t interfaceId,
                                  statusConfirm_t *pReplyData, uint16_t *pDataSize)
\brief  This function is used to send ADDR_NTF.ntf - Proactive Address Notification.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static void THCI_SendProactiveAddrNotify
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    pReplyData->status = THR_SendProactiveAddressNotification(pClientPacket[0], (ipAddr_t *)&pClientPacket[1]);
    *pDataSize = sizeof(statusConfirm_t);
}

#if DNS_ENABLED
/*!*************************************************************************************************
\private
\fn     bool_t THCI_CoapSend(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to send a COAP message.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static bool_t THCI_DnsSendRequest
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    appReturnHandler_t pfAppReturn = THCI_DnsService;

    DNS_GetHostByName(pClientPacket, AF_INET6, mDnsClassIN_c, pfAppReturn, pmMainThreadMsgQueue);
    pReplyData->status = gThrStatus_Success_c;

    *pDataSize = sizeof(statusConfirm_t);

    return TRUE;
}


#endif
/*!*************************************************************************************************
\private
\fn     bool_t THCI_CoapSend(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to send a COAP message.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static bool_t THCI_CoapRegisterCb
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    statusConfirm_t *pReplyData,
    uint16_t *pDataSize
)
{
    /* CoAP instance */
    uint8_t coapInstId;
    coapRegCbParams_t callbacksStruct = {0};
    sockaddrStorage_t coapParams = {0};
    thciCoapRegisterCb_t *pCoapRegCb = (thciCoapRegisterCb_t *)pClientPacket;
    uint8_t freeEntry = THR_ALL_FFs8;
    uint16_t port = 0;
    pReplyData->status = gThrStatus_Failed_c;

    callbacksStruct.pCallback = THCI_CoapReceiveCb;

    for (uint32_t iCount = 0; iCount < THCI_COAP_MAX_URI_PATHS; iCount++)
    {
        if (aCoapUriPathStruct[iCount].length == 0)
        {
            freeEntry = iCount;
        }
        else if ((FLib_MemCmp(aCoapUriPathStruct[iCount].pUriPath, &pCoapRegCb->uriPath, aCoapUriPathStruct[iCount].length))
                 && (aCoapUriPathStruct[iCount].length == strlen((char *)pCoapRegCb->uriPath)))
        {
            freeEntry = THR_ALL_FFs8;
            break;
        }
    }

    if (freeEntry != THR_ALL_FFs8)
    {
        aCoapUriPathStruct[freeEntry].length = strlen((char *)pCoapRegCb->uriPath);
        FLib_MemCpy(aCoapUriPaths[freeEntry], &pCoapRegCb->uriPath, aCoapUriPathStruct[freeEntry].length);
        aCoapUriPathStruct[freeEntry].pUriPath = aCoapUriPaths[freeEntry];

        callbacksStruct.pUriPathStruct = &aCoapUriPathStruct[freeEntry];
        FLib_MemCpy(&port, pCoapRegCb->udpPort, sizeof(uint16_t));
        NWKU_SetSockAddrInfo(&coapParams, NULL, AF_INET6, port, 0, gIpIfSlp0_c);
        coapInstId = COAP_CreateInstance(NULL, &coapParams, &callbacksStruct, 1);

        if (THR_ALL_FFs8 != coapInstId)
        {
            pReplyData->status = gThrStatus_Success_c;
            *pDataSize = sizeof(statusConfirm_t);
        }
    }

    return TRUE;
}

void THCI_HandleMeshcopDiagnostics
(
    meshcopDiagnosticType_t meshcopType,
    meshcopDiagnosticDir_t dir,
    uint8_t *pEui,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    thciMeshcopDiagHdr_t *pDiagPacket;

    pDiagPacket = MEM_BufferAlloc(sizeof(thciMeshcopDiagHdr_t) + tlvsLen);

    if (pDiagPacket)
    {
        pDiagPacket->meshcopType = meshcopType;
        pDiagPacket->direction = dir;

        if (pEui)
        {
            FLib_MemCpy(pDiagPacket->aAddr, pEui, sizeof(pDiagPacket->aAddr));
        }
        else
        {
            FLib_MemSet(pDiagPacket->aAddr, 0, sizeof(pDiagPacket->aAddr));
        }

        pDiagPacket->tlvsLen = tlvsLen;
        FLib_MemCpy(pDiagPacket + 1, pTlvs, tlvsLen);
        THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopDiagnostic_c, (void *)pDiagPacket,
                             sizeof(thciMeshcopDiagHdr_t) + tlvsLen, mFsciAppInterfaceId);
        MEM_BufferFree(pDiagPacket);
    }
}

/*!*************************************************************************************************
\fn     void THCI_CoapReceiveCb(coapSessionStatus_t sessionStatus, uint8_t *pData,
                                  coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for the COAP ACK message.

\param  [in]

\return         void
***************************************************************************************************/
void THCI_CoapReceiveCb
(
    coapSessionStatus_t sessionStatus,
    uint8_t *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    thciCoapStruct_t *pCoapRcvd = MEM_BufferAlloc(sizeof(thciCoapStruct_t) + dataLen);
    uint16_t port = 0;

    if (NULL != pCoapRcvd)
    {
        FLib_MemSet(pCoapRcvd, 0, sizeof(thciCoapStruct_t) + dataLen);
        pCoapRcvd->reserved = sessionStatus;
        pCoapRcvd->code = pSession->code;
        pCoapRcvd->requestType = pSession->msgType;
        port = ((sockaddrIn6_t *)&pSession->remoteAddrStorage)->sin6_port;

        FLib_MemCpyReverseOrder(pCoapRcvd->remoteAddr, &pSession->remoteAddrStorage.ss_addr, sizeof(ipAddr_t));
        FLib_MemCpy(pCoapRcvd->udpPort, &port, sizeof(uint16_t));
        FLib_MemCpy(pCoapRcvd->uriPath, pSession->pUriPath->pUriPath, pSession->pUriPath->length);
        pCoapRcvd->payloadLen = dataLen;
        FLib_MemCpy(&pCoapRcvd->payload, pData, dataLen);

        /* send CoAP ACK with no payload if it was received a CON msg */
        if ((pSession->msgType == gCoapConfirmable_c) && (sessionStatus != gCoapFailure_c))
        {
            COAP_Send(pSession, gCoapMsgTypeAckSuccessContent_c, NULL, 0);
        }

        /* send response */
        THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_THRCoapRcvd_c, pCoapRcvd,
                             (sizeof(thciCoapStruct_t) + dataLen), mFsciAppInterfaceId);
        MEM_BufferFree(pCoapRcvd);
    }

}
#if UDP_ECHO_PROTOCOL
/*!*************************************************************************************************
\private
\fn     bool_t THCI_EchoUdp(uint8_t *pClientPacket, uint32_t interfaceId,
                                  uint8_t **pReplyData, uint16_t *pDataSize)
\brief  This function is used to send an echo UDP message.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [in]    interfaceId     id of the FSCI interface
\param  [out]   pReplyData      double pointer to the buffer of data which needs to be sent back
\param  [out]   pDataSize       size of the buffer of data which needs to be sent back

\return         void
***************************************************************************************************/
static bool_t THCI_EchoUdp
(
    uint8_t *pClientPacket,
    uint32_t interfaceId,
    uint8_t **pReplyData,
    uint16_t *pDataSize
)
{
    ipAddr_t destIpAddr;
    ipAddr_t sourceIpAddr;
    bool_t res = FALSE;
    uint16_t timeout;
    uint16_t length;

    /* Cast received data to a known structure */
    echoUdpParams_t *pEchoUdpParams = (echoUdpParams_t *)pClientPacket;
    FLib_MemCpy(&timeout, pEchoUdpParams->timeout, sizeof(uint16_t));

    /* Reverse address bytes */
    FLib_MemCpyReverseOrder(destIpAddr.addr8, pEchoUdpParams->destIpAddr.addr8, sizeof(ipAddr_t));

    /* Reverse address bytes */
    FLib_MemCpyReverseOrder(sourceIpAddr.addr8, pEchoUdpParams->sourceIpAddr.addr8, sizeof(ipAddr_t));
    FLib_MemCpy(&length, pEchoUdpParams->length, sizeof(uint16_t));
    ECHO_ThciUdp(length, timeout, pEchoUdpParams->count, &destIpAddr, &sourceIpAddr);

    return res;
}

#endif

#if DTLS_ENABLED && THCI_DTLS_ENABLED

static void DTLS_GetJpakePasswd(dtlsPeerPtr_t pPeer, uint32_t *pSecretLen, uint8_t **ppSecret)
{
    static uint8_t *passwd = (uint8_t *)"kinetis";

    *pSecretLen = strlen((char *)passwd);
    *ppSecret = passwd;
}

static void DTLS_Received
(
    dtlsPeerPtr_t pPeer,
    uint8_t *pData,
    uint32_t len
)
{
    dtlsDataConfirm_t *pDataRes;

    pDataRes = MEM_BufferAlloc(sizeof(dtlsDataConfirm_t) + len);

    if (pDataRes)
    {
        FLib_MemCpy(pDataRes->size, &len, sizeof(uint16_t));
        FLib_MemCpy(pDataRes->data, pData, len);

        pDataRes->peerIndex = NWKU_GetTblEntry((uint32_t)pPeer, (uint32_t *)maDtlsPeers,
                                               NumberOfElements(maDtlsPeers));
        THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_DtlsReceive_c, (uint8_t *)pDataRes,
                             sizeof(dtlsDataConfirm_t) + len, mFsciAppInterfaceId);

        MEM_BufferFree(pDataRes);
    }
}

static void DTLS_Event
(
    void *pPeer,
    uint8_t level,
    uint8_t code
)
{
    uint8_t error = 1;
    thciDtlsConnectConfirm_t connectConfirm;

    /* Check alerts */
    if ((level == DTLS_ALERT_LEVEL_OK) && (code == DTLS_ALERT_CONNECTED))
    {
        uint32_t idx;

        idx = NWKU_AddTblEntry((uint32_t)pPeer, (uint32_t *)maDtlsPeers,
                               NumberOfElements(maDtlsPeers));

        /* If the peer could be added */
        if (idx != (uint32_t)(-1))
        {
            connectConfirm.status = mTHCI_Ok_c;
            THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_DtlsClientConnected_c,
                                 (uint8_t *)&connectConfirm, sizeof(thciDtlsConnectConfirm_t), mFsciAppInterfaceId);

            error = 0;
        }
    }
    else
    {
        /* error */
    }

    if (error)
    {
        connectConfirm.status = mTHCI_Err_c;
        THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_DtlsClientConnected_c,
                             (uint8_t *)&connectConfirm, sizeof(thciDtlsConnectConfirm_t), mFsciAppInterfaceId);
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_DtlsOpen(uint8_t *pClientPacket, dtlsStatusConfirm_t *pConfirm)
\brief  This function is used to initialize a DTLS context.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_DtlsOpen
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    thciErrorCode_t error = mTHCI_Err_c;
    dtlsOpenParams_t *pOpenParams;
    dtlsInitParams_t initParams;
    dtlsContext_t *pCtx;
    uint8_t iCtx;
    sockaddrIn6_t addrStorage;
    uint16_t timeout;

    /* Cast received data to a known structure */
    pOpenParams = (dtlsOpenParams_t *)pClientPacket;

    initParams.maxRetransmitCnt = pOpenParams->maxRetrCount;
    FLib_MemCpy(&timeout, pOpenParams->timeout, sizeof(uint16_t));
    initParams.retransmitTimeUnits = timeout * 10;
    initParams.ipIfId = gIpIfSlp0_c;
    mDtlsCallbacks.event = DTLS_Event;
    mDtlsCallbacks.rcvd = DTLS_Received;
    mDtlsCallbacks.getJpakePasswd = DTLS_GetJpakePasswd;

    addrStorage.sin6_family = AF_INET6;
    FLib_MemCpy(&addrStorage.sin6_port, pOpenParams->port, sizeof(uint16_t));
    IP_AddrCopy(&addrStorage.sin6_addr, &in6addr_any); /* TBD  FSCI: Server addr can be set here */
    pCtx = DTLS_NewContext(&initParams, &mDtlsCallbacks, (sockaddrStorage_t *)&addrStorage);

    if (pCtx)
    {
        iCtx = NWKU_AddTblEntry((uint32_t)pCtx, (uint32_t *)maDtlsContexts,
                                NumberOfElements(maDtlsContexts));
        error = mTHCI_Ok_c;
    }

    /* Set FSCI command payload */
    if (error == mTHCI_Ok_c)
    {
        pConfirm->status = iCtx;
    }
    else
    {
        pConfirm->status = mTHCI_Err_c;
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_DtlsOpen(uint8_t *pClientPacket, dtlsStatusConfirm_t *pConfirm)
\brief  This function is used to initialize a DTLS context.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_DtlsCloseContext
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    dtlsCloseContextParams_t *pCloseContextParams;
    dtlsContext_t *pCtx;

    /* Cast received data to a known structure */
    pCloseContextParams = (dtlsCloseContextParams_t *)pClientPacket;

    pCtx = (dtlsContext_t *)NWKU_GetTblEntry(pCloseContextParams->contextNumber, (uint32_t *)maDtlsContexts,
            NumberOfElements(maDtlsContexts));

    /* Set FSCI command payload */
    if (pCtx)
    {
        maDtlsContexts[pCloseContextParams->contextNumber] = 0;
        DTLS_FreeContext(pCtx);
        pConfirm->status = mTHCI_Ok_c;
    }
    else
    {
        pConfirm->status = mTHCI_Err_c;
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_DtlsClosePeer(uint8_t *pClientPacket, dtlsStatusConfirm_t *pConfirm)
\brief  This function is used to initialize a DTLS context.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_DtlsClosePeer
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    dtlsClosePeerParams_t *pClosePeerParams;
    dtlsPeerPtr_t pPeer;

    /* Cast received data to a known structure */
    pClosePeerParams = (dtlsClosePeerParams_t *)pClientPacket;

    pPeer = (dtlsPeerPtr_t)NWKU_GetTblEntry(pClosePeerParams->peerNumber, (uint32_t *)maDtlsPeers,
                                            NumberOfElements(maDtlsPeers));

    /* Set FSCI command payload */
    if (pPeer)
    {
        maDtlsPeers[pClosePeerParams->peerNumber] = 0;
        DTLS_Close(pPeer);
        pConfirm->status = mTHCI_Ok_c;
    }
    else
    {
        pConfirm->status = mTHCI_Err_c;
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_DtlsConnect(uint8_t *pClientPacket, dtlsStatusConfirm_t *pConfirm)
\brief  This function is used to initialize a DTLS context.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_DtlsConnect
(
    uint8_t *pClientPacket,
    thciDtlsConnectConfirm_t *pConfirm
)
{
    dtlsConnectParams_t *pConnectParams;
    dtlsContext_t *pCtx;
    sockaddrStorage_t sockAddr;

    /* Cast received data to a known structure */
    pConnectParams = (dtlsConnectParams_t *)pClientPacket;

    pCtx = (dtlsContext_t *)NWKU_GetTblEntry(pConnectParams->contextNumber, (uint32_t *)maDtlsContexts,
            NumberOfElements(maDtlsContexts));

    if (pCtx)
    {
        dtlsPeerPtr_t pPeer;

        /* Create sockAddr structure depending on family */
        ((sockaddrIn6_t *)&sockAddr)->sin6_family = AF_INET6;
        FLib_MemCpy(&((sockaddrIn6_t *)&sockAddr)->sin6_port, pConnectParams->port, sizeof(uint16_t));

        /* Reverse address bytes */
        FLib_MemCpyReverseOrder(sockAddr.ss_addr, pConnectParams->ipAddr,
                                sizeof(ipAddr_t));

        pPeer = DTLS_Connect(pCtx, &sockAddr);
        pConfirm->peerIdx = (uint8_t)NWKU_AddTblEntry((uint32_t)pPeer, (uint32_t *)maDtlsPeers,
                            NumberOfElements(maDtlsPeers));

        if (pConfirm->peerIdx != (uint8_t)(-1))
        {
            pConfirm->status = mTHCI_Ok_c;
        }
    }
    else
    {
        pConfirm->status = mTHCI_Err_c;
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_DtlsSend(uint8_t *pClientPacket, dtlsStatusConfirm_t *pConfirm)
\brief  This function is used to initialize a DTLS context.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_DtlsSend
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    dtlsSendParams_t *pSendParams;
    dtlsPeerPtr_t pPeer;

    /* Cast received data to a known structure */
    pSendParams = (dtlsSendParams_t *)pClientPacket;

    pPeer = (dtlsPeerPtr_t)NWKU_GetTblEntry(pSendParams->peerNumber, (uint32_t *)maDtlsPeers,
                                            NumberOfElements(maDtlsPeers));

    /* Set FSCI command payload */
    if (pPeer)
    {
        uint16_t size;
        FLib_MemCpy(&size, pSendParams->size, sizeof(uint16_t));
        DTLS_Send(pPeer, pSendParams->data, size);
        pConfirm->status = mTHCI_Ok_c;
    }
    else
    {
        pConfirm->status = mTHCI_Err_c;
    }
}

#endif /* DTLS_ENABLED && THCI_DTLS_ENABLED*/

#if MESHCOP_ENABLED
/*!*************************************************************************************************
\private
\fn     void THCI_MeshCopAddExpectedJoiner
\brief  This function is used to add an expected Joiner.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_MeshCopAddExpectedJoiner
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    uint8_t aEui[8];
    meshCopEuiParams_t *pEuiParams;
    bool_t status;

    pConfirm->status = mTHCI_Err_c;

    /* Cast received data to a known structure */
    pEuiParams = (meshCopEuiParams_t *)pClientPacket;

    FLib_MemCpyReverseOrder(aEui, pEuiParams->aEui, sizeof(pEuiParams->aEui));

    status = MESHCOP_AddExpectedJoiner(pEuiParams->instanceId, aEui, pEuiParams->psk,
                                       pEuiParams->pskLen, pEuiParams->selected);

    if (status == TRUE)
    {
        pConfirm->status = mTHCI_Ok_c;
    }
}

/*!*************************************************************************************************
\private
\fn     void THCI_MeshCopGetExpectedJoiner
\brief  This function is used to retrieve information about a Joiner.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_MeshCopGetExpectedJoiner
(
    uint8_t *pClientPacket,
    uint8_t **pConfirm,
    uint16_t *pConfirmLength
)
{
    meshCopEuiGetParams_t *pEuiParams;
    uint8_t aEui[8];
    expectedJoinerEntry_t *pExpectedJoinerEntry;
    thciGetExpectedJoinerConfirm_t *pGetJoinerConfirm;

    pGetJoinerConfirm = MEM_BufferAlloc(sizeof(thciGetExpectedJoinerConfirm_t) +
                                        MESHCOP_MAX_PSK_LEN);

    if (pGetJoinerConfirm)
    {
        pGetJoinerConfirm->status = mTHCI_Err_c;

        /* Cast received data to a known structure */
        pEuiParams = (meshCopEuiGetParams_t *)pClientPacket;

        FLib_MemCpyReverseOrder(aEui, pEuiParams->aEui, sizeof(pEuiParams->aEui));

        pExpectedJoinerEntry = MESHCOP_GetExpectedJoiner(pEuiParams->instanceId, NULL, aEui);

        if (pExpectedJoinerEntry)
        {
            pGetJoinerConfirm->status = mTHCI_Ok_c;
            pGetJoinerConfirm->selected = pExpectedJoinerEntry->selected;
            pGetJoinerConfirm->pskLen = pExpectedJoinerEntry->pskLen;
            FLib_MemCpy(pGetJoinerConfirm->psk, pExpectedJoinerEntry->aPsk,
                        pExpectedJoinerEntry->pskLen);
            *pConfirmLength = sizeof(thciGetExpectedJoinerConfirm_t) +
                              pGetJoinerConfirm->pskLen;
        }
    }

    *pConfirm = (uint8_t *)pGetJoinerConfirm;
}

/*!*************************************************************************************************
\private
\fn     void THCI_MeshCopRemoveExpectedJoiner
\brief  This function is used to remove Joiner entry.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_MeshCopRemoveExpectedJoiner
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    meshCopEuiGetParams_t *pEuiParams;
    bool_t status;
    uint8_t aEui[8];
    pConfirm->status = mTHCI_Err_c;

    /* Cast received data to a known structure */
    pEuiParams = (meshCopEuiGetParams_t *)pClientPacket;

    FLib_MemCpyReverseOrder(aEui, pEuiParams->aEui, sizeof(pEuiParams->aEui));
    status = MESHCOP_RemoveExpectedJoiner(pEuiParams->instanceId, NULL, aEui);

    if (status == TRUE)
    {
        pConfirm->status = mTHCI_Ok_c;
    }
}
/*!*************************************************************************************************
\private
\fn     THCI_MeshCopMgmtSet
\brief  This function is used to do a Management Set request.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         TRUE            reply is needed
\return         FALSE           reply will be sent later
***************************************************************************************************/
static bool_t THCI_MeshCopMgmtSet
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    uint8_t *pInput = pClientPacket;
    nwkStatus_t status = gNwkStatusFail_c;
    instanceId_t instId;
    uint8_t pskcSize = 0;
    uint8_t networkNameSize = 0;
    uint8_t *pPskc = NULL;
    uint8_t *pNetworkName = NULL;
    uint8_t *pPolicy = NULL;
    uint8_t *pTlvs = NULL;
    uint32_t tlvsSize = 0;
    bool_t replyNow = FALSE;
    uint8_t *pPos;
    bool_t generatePskc = FALSE;
    uint8_t *pTimestampData = NULL;

    instId = *pInput;
    pInput++;

    /* Check if PSKc is enabled */
    if (*pInput)
    {
        pInput++;
        pskcSize = *pInput;
        pInput++;

        if (pskcSize) /* check PSKc size */
        {
            pPskc = pInput;
            pInput += pskcSize;
        }
    }
    else
    {
        pInput++;
    }

    /* Check if Network Name is enabled */
    if (*pInput)
    {
        pInput++;
        networkNameSize = *pInput;
        pInput++;

        if (networkNameSize) /* check network name size */
        {
            pNetworkName = pInput;
            pInput += networkNameSize;
        }
    }
    else
    {
        pInput++;
    }

    /* Check policy */
    if (*pInput)
    {
        pInput++;
        pPolicy = pInput;
        pInput += GetSizeOfMember(meshCopSecurityPolicyTlv_t, policy) +
                  GetSizeOfMember(meshCopSecurityPolicyTlv_t, rotationInterval);
    }
    else
    {
        pInput++;
    }

    /* Check for Timestamp */
    if (*pInput)
    {
        pInput++;
        pTimestampData = pInput;
    }

    if (pPolicy)
    {
        tlvsSize = sizeof(meshCopSecurityPolicyTlv_t);
        status = gNwkStatusSuccess_c;
    }

    if (pNetworkName)
    {
        if (pPskc)
        {
            tlvsSize += sizeof(meshCopNwkNameTlv_t) + networkNameSize;
            tlvsSize += sizeof(meshCopPskcTlv_t) + THR_MAX_PSKC_LEN;
            status = gNwkStatusSuccess_c;
        }
        else
        {
            status = gNwkStatusFail_c;
        }
    }
    else
    {
        if (pPskc)
        {
            tlvsSize += sizeof(meshCopPskcTlv_t) + THR_MAX_PSKC_LEN;
            status = gNwkStatusSuccess_c;
        }
    }

    if (pTimestampData)
    {
        tlvsSize += sizeof(gMeshCopTlvActiveTimestamp_c);
    }

    if (status == gNwkStatusSuccess_c)
    {
        /* Populate TLVs */
        pTlvs = MEM_BufferAlloc(tlvsSize);

        if (pTlvs)
        {
            pPos = pTlvs;

            if (pPolicy)
            {
                meshCopSecurityPolicyTlv_t *pPolicyTlv = (meshCopSecurityPolicyTlv_t *)pPos;
                pPolicyTlv->type = gMeshCopTlvSecPolicy_c;
                pPolicyTlv->len = sizeof(pPolicyTlv->policy) + sizeof(pPolicyTlv->rotationInterval);
                pPolicyTlv->policy = (*pPolicy) << 6;
                FLib_MemCpy(pPolicyTlv->rotationInterval, pPolicy + 1,
                            sizeof(pPolicyTlv->rotationInterval));
                pPos += sizeof(meshCopSecurityPolicyTlv_t);
            }

            if (pNetworkName)
            {
                /* If the network name is changed than the PSKc MUST be also changed */
                if (pPskc)
                {
                    meshCopNwkNameTlv_t *pNwkNameTlv;

                    pNwkNameTlv = (meshCopNwkNameTlv_t *)pPos;
                    pNwkNameTlv->type = gMeshCopTlvNwkName_c;
                    pNwkNameTlv->len = networkNameSize;
                    FLib_MemCpy(pNwkNameTlv->nwkName, pNetworkName, networkNameSize);
                    pPos += sizeof(meshCopNwkNameTlv_t) + pNwkNameTlv->len;

                    /* Save network name on our device also */
                    (void)THR_SetAttr(0, gNwkAttrId_NwkName_c, 0, networkNameSize, pNetworkName);

                    generatePskc = TRUE;
                }
            }
            else
            {
                /* PSKc can be changed w/o changing the network name */
                if (pPskc)
                {
                    generatePskc = TRUE;
                }
            }

            if (generatePskc)
            {
                meshCopPskcTlv_t *pPskcTlv;
                uint32_t pskcAttrLen = 0;

                pPskcTlv = (meshCopPskcTlv_t *)pPos;
                pPskcTlv->type = gMeshCopTlvPskc_c;

                /* Compute new pskc */
                (void)THR_SetPskcPassphrase(0, pPskc, pskcSize);
                (void)THR_GetAttr(0, gNwkAttrId_PSKc_c, 0, &pskcAttrLen, pPskcTlv->pskc);
                pPskcTlv->len = (uint8_t)pskcAttrLen;

                pPos += sizeof(meshCopPskcTlv_t) + pPskcTlv->len;
            }

            /* Add timestamp */
            if (pTimestampData)
            {
                meshCopActiveTimestampTlv_t *pTimestampTlv =
                    (meshCopActiveTimestampTlv_t *)pPos;
                pTimestampTlv->type = gMeshCopTlvActiveTimestamp_c;
                pTimestampTlv->len = GetSizeOfMember(meshCopActiveTimestampTlv_t, seconds) +
                                     GetSizeOfMember(meshCopActiveTimestampTlv_t, ticks);
                FLib_MemCpyReverseOrder(pTimestampTlv->seconds, pTimestampData,
                                        GetSizeOfMember(meshCopActiveTimestampTlv_t, seconds));
                FLib_MemCpyReverseOrder(pTimestampTlv->ticks, pTimestampData +
                                        GetSizeOfMember(meshCopActiveTimestampTlv_t, seconds),
                                        GetSizeOfMember(meshCopActiveTimestampTlv_t, ticks));
                pPos += sizeof(meshCopActiveTimestampTlv_t);
            }

            status = MESHCOP_Set(instId, pTlvs, pPos - pTlvs, THCI_MeshcopMgmtSetCb);
            MEM_BufferFree(pTlvs);
        }
        else
        {
            status = gNwkStatusFail_c;
        }
    }

    if (status == gNwkStatusSuccess_c)
    {
        pConfirm->status = mTHCI_Ok_c;
        replyNow = FALSE;
    }
    else
    {
        pConfirm->status = mTHCI_Err_c;
        replyNow = TRUE;
    }

    return replyNow;
}

/*!*************************************************************************************************
\private
\fn     THCI_MeshCopMgmtSet2
\brief  This function is used to do a Management Comm/Active/Pending Set request.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         TRUE            reply is needed
\return         FALSE           reply will be sent later
***************************************************************************************************/
static bool_t THCI_MeshCopMgmtSet2
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm,
    meshcopHandlerCb_t pfSetCb,
    uint8_t type
)
{
    uint8_t *pInput = pClientPacket;
    nwkStatus_t status = gNwkStatusFail_c;
    uint8_t *pSessId = NULL;
    uint8_t *pBrLoc = NULL;
    uint8_t *pNewSessId = NULL;
    uint8_t *pSteeringData = NULL;
    uint8_t steeringDataLen = 0;
    bool_t channelTlvPresent = FALSE;
    uint8_t channel = 0;
    uint8_t *pChannelMask = NULL;
    uint8_t channelPage = 0;
    uint8_t channelMaskLength = 0;
    uint8_t *pXpanId = NULL;
    uint8_t *pPrefix = NULL;
    uint8_t *pMasterKey = NULL;
    uint8_t nwkNameLength = 0;
    uint8_t *pNwkName = NULL;
    uint8_t *pPanId = NULL;
    uint8_t *pPolicy = NULL;
    uint8_t pskcLen = 0;
    uint8_t *pPskc = NULL;
    uint8_t *pActiveTimestamp = NULL;
    uint8_t *pPendingTimestamp = NULL;
    uint8_t *pDelayTimer = NULL;
    bool_t futureTlv = FALSE;
    ipAddr_t ipAddr;
    bool_t replyNow = FALSE;
    uint8_t futureLen = 0;
    meshcopMgmtParams_t mgmtParams = {0};

    mgmtParams.thrInstId = *pInput;
    pInput++;

    FLib_MemCpyReverseOrder(&ipAddr.addr8, pInput, 16);
    pInput += 16;

    /* Check if SessionID is enabled */
    if (*pInput)
    {
        pInput++;
        pSessId = pInput;
        pInput += GetSizeOfMember(meshCopCommSessIdTlv_t, id);
        mgmtParams.tlvsLength += sizeof(meshCopCommSessIdTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Commissioning Dataset TLVs */
    /* Check if Border Router Locator is enabled */
    if (*pInput)
    {
        pInput++;
        pBrLoc = pInput;
        pInput += GetSizeOfMember(meshCopBrLocTlv_t, addr);
        mgmtParams.tlvsLength += sizeof(meshCopBrLocTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check if a new SessionID is enabled */
    if (*pInput)
    {
        pInput++;
        pNewSessId = pInput;
        pInput += GetSizeOfMember(meshCopCommSessIdTlv_t, id);
        mgmtParams.tlvsLength += sizeof(meshCopCommSessIdTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check if Steering Data is enabled */
    if (*pInput)
    {
        pInput++;
        steeringDataLen = *pInput;
        pInput++;
        pSteeringData = pInput;
        pInput += steeringDataLen;
        mgmtParams.tlvsLength += sizeof(meshCopSteeringTlv_t) + steeringDataLen;
    }
    else
    {
        pInput++;
    }

    /* Active Dataset TLVs */
    /* Check if Channel is enabled */
    if (*pInput)
    {
        pInput++;
        channel = *pInput;
        mgmtParams.tlvsLength += sizeof(meshCopChannelTlv_t);
        pInput++;
        channelTlvPresent = TRUE;
    }
    else
    {
        pInput++;
    }

    /* Check if Channel Mask is enabled */
    if (*pInput)
    {
        pInput++;
        channelPage = *pInput;
        pInput++;
        channelMaskLength = *pInput;
        pInput++;
        pChannelMask = pInput;
        pInput += channelMaskLength;
        mgmtParams.tlvsLength += sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) +
                                 sizeof(uint8_t) + channelMaskLength;
    }
    else
    {
        pInput++;
    }

    /* Check if XPanId is enabled */
    if (*pInput)
    {
        pInput++;
        pXpanId = pInput;
        pInput += TLV_NETWORK_XPANID_LEN;
        mgmtParams.tlvsLength += sizeof(meshCopNwkXPanIdTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check if ML prefix is enabled */
    if (*pInput)
    {
        pInput++;

        pPrefix = pInput;
        pInput += 16;

        //prefixLength = *pInput;
        pInput++;

        mgmtParams.tlvsLength += sizeof(meshCopNwkMlUlaTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check if Master Key is enabled */
    if (*pInput)
    {
        pInput++;
        pMasterKey = pInput;
        pInput += TLV_NETWORK_KEY_LEN;
        mgmtParams.tlvsLength += sizeof(meshCopNwkMasterKeyTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check if Network Name is enabled */
    if (*pInput)
    {
        pInput++;

        nwkNameLength = *pInput;
        pInput++;
        pNwkName = pInput;
        pInput += nwkNameLength;
        mgmtParams.tlvsLength += sizeof(meshCopNwkNameTlv_t) + nwkNameLength;
    }
    else
    {
        pInput++;
    }

    /* Check if PanID is enabled */
    if (*pInput)
    {
        pInput++;
        pPanId = pInput;
        pInput += GetSizeOfMember(meshCopNwkPanIdTlv_t, panId);
        mgmtParams.tlvsLength += sizeof(meshCopNwkPanIdTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check if PSKc is enabled */
    if (*pInput)
    {
        pInput++;
        pskcLen = *pInput;
        pInput++;
        pPskc = pInput;
        pInput += pskcLen;
        mgmtParams.tlvsLength += sizeof(meshCopPskcTlv_t) + THR_MAX_PSKC_LEN;
    }
    else
    {
        pInput++;
    }

    /* Check policy */
    if (*pInput)
    {
        pInput++;
        pPolicy = pInput;
        pInput += GetSizeOfMember(meshCopSecurityPolicyTlv_t, policy) +
                  GetSizeOfMember(meshCopSecurityPolicyTlv_t, rotationInterval);
        mgmtParams.tlvsLength += sizeof(meshCopSecurityPolicyTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check active timestamp */
    if (*pInput)
    {
        pInput++;
        pActiveTimestamp = pInput;
        pInput += GetSizeOfMember(meshCopActiveTimestampTlv_t, seconds) +
                  GetSizeOfMember(meshCopActiveTimestampTlv_t, ticks);
        mgmtParams.tlvsLength += sizeof(meshCopActiveTimestampTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check pending timestamp */
    if (*pInput)
    {
        pInput++;
        pPendingTimestamp = pInput;
        pInput += GetSizeOfMember(meshCopPendingTimestampTlv_t, seconds) +
                  GetSizeOfMember(meshCopPendingTimestampTlv_t, ticks);
        mgmtParams.tlvsLength += sizeof(meshCopPendingTimestampTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check delay timer */
    if (*pInput)
    {
        pInput++;
        pDelayTimer = pInput;
        pInput += 4;
        mgmtParams.tlvsLength += sizeof(meshCopDelayTimerTlv_t);
    }
    else
    {
        pInput++;
    }

    /* Check if future TLV is enabled */
    if (*pInput)
    {
        futureTlv = TRUE;
        pInput++;
        futureLen = *pInput;
        mgmtParams.tlvsLength += (2 + *pInput);
        pInput++;
    }

    /* Populate TLVs */
    mgmtParams.pTlvs = MEM_BufferAlloc(mgmtParams.tlvsLength);

    if (mgmtParams.pTlvs)
    {
        uint8_t *pPos = mgmtParams.pTlvs;
        meshCopPskcTlv_t *pPskcTlv = NULL;
        uint8_t pskcBitMask = 0; /* Bitmask representing the presence of some input values required
                                            for PSKC update:
                                            X X X X X X X X
                                                      | | +---> pskc
                                                      | +-----> X pan ID
                                                      +-------> network name */
        meshCopNwkXPanIdTlv_t *pXpanTlv;
        meshCopNwkNameTlv_t *pNwkNameTlv;

        if (pSessId)
        {
            meshCopCommSessIdTlv_t *pSessIdTlv = (meshCopCommSessIdTlv_t *)pPos;
            pSessIdTlv->type = gMeshCopTlvCommSessId_c;
            pSessIdTlv->len = GetSizeOfMember(meshCopCommSessIdTlv_t, id);
            FLib_MemCpyReverseOrder(pSessIdTlv->id, pSessId, pSessIdTlv->len);
            pPos += sizeof(meshCopCommSessIdTlv_t);
        }

        /* Commissioner Dataset TLVs */
        if (pBrLoc)
        {
            meshCopBrLocTlv_t *pBrLocIdTlv = (meshCopBrLocTlv_t *)pPos;
            pBrLocIdTlv->type = gMeshCopTlvBorderRouterLoc_c;
            pBrLocIdTlv->len = GetSizeOfMember(meshCopBrLocTlv_t, addr);
            FLib_MemCpyReverseOrder(pBrLocIdTlv->addr, pBrLoc, pBrLocIdTlv->len);
            pPos += sizeof(meshCopBrLocTlv_t);
        }

        if (pNewSessId)
        {
            meshCopCommSessIdTlv_t *pSessIdTlv = (meshCopCommSessIdTlv_t *)pPos;
            pSessIdTlv->type = gMeshCopTlvCommSessId_c;
            pSessIdTlv->len = GetSizeOfMember(meshCopCommSessIdTlv_t, id);
            FLib_MemCpyReverseOrder(pSessIdTlv->id, pNewSessId, pSessIdTlv->len);
            pPos += sizeof(meshCopCommSessIdTlv_t);
        }

        if (pSteeringData)
        {
            meshCopSteeringTlv_t *pSteeringDataTlv = (meshCopSteeringTlv_t *)pPos;
            pSteeringDataTlv->type = gMeshCopTlvSteeringData_c;
            pSteeringDataTlv->len = steeringDataLen;
            FLib_MemCpy(pSteeringDataTlv->filter, pSteeringData, pSteeringDataTlv->len);
            pPos += sizeof(meshCopSteeringTlv_t) + steeringDataLen;
        }

        /* Active/Pending Dataset TLVs */
        if (channelTlvPresent == TRUE)
        {
            meshCopChannelTlv_t *pChannelTlv = (meshCopChannelTlv_t *)pPos;
            pChannelTlv->type = gMeshCopTlvChannel_c;
            pChannelTlv->len = sizeof(pChannelTlv->channel) + sizeof(pChannelTlv->channelPage);
            pChannelTlv->channelPage = 0;
            htonas(pChannelTlv->channel, channel);
            pPos += sizeof(meshCopChannelTlv_t);
        }

        if (pChannelMask)
        {
            meshCopChannelMaskTlv_t *pChannelMaskTlv = (meshCopChannelMaskTlv_t *)pPos;
            uint32_t channelMask;

            channelMask = ntohal(pChannelMask);
            channelMask = NWKU_ReverseBits(channelMask);

            pChannelMaskTlv->type = gMeshCopTlvChannelMask_c;
            pChannelMaskTlv->len = sizeof(pChannelMaskTlv->channelMask) +
                                   sizeof(pChannelMaskTlv->channelPage) + sizeof(pChannelMaskTlv->maskLength);
            pChannelMaskTlv->channelPage = channelPage;
            pChannelMaskTlv->maskLength = channelMaskLength;
            htonal(pChannelMaskTlv->channelMask, channelMask);
            pPos += sizeof(meshCopChannelMaskTlv_t);
        }

        if (pXpanId)
        {
            pXpanTlv = (meshCopNwkXPanIdTlv_t *)pPos;
            pXpanTlv->type = gMeshCopTlvXpanID_c;
            pXpanTlv->len = TLV_NETWORK_XPANID_LEN;
            FLib_MemCpyReverseOrder(pXpanTlv->xPanId, pXpanId, pXpanTlv->len);
            pPos += sizeof(meshCopNwkXPanIdTlv_t);
            pskcBitMask |= 0x02;
        }

        if (pPrefix)
        {
            meshCopNwkMlUlaTlv_t *pPrefixTlv = (meshCopNwkMlUlaTlv_t *)pPos;
            pPrefixTlv->type = gMeshCopTlvNwkMlUla_c;
            pPrefixTlv->len = TLV_NETWORK_ML_ULA_LEN;
            FLib_MemCpy(pPrefixTlv->mlUla, pPrefix, pPrefixTlv->len);
            pPos += sizeof(meshCopNwkMlUlaTlv_t);
        }

        if (pMasterKey)
        {
            meshCopNwkMasterKeyTlv_t *pMasterKeyTlv = (meshCopNwkMasterKeyTlv_t *)pPos;
            pMasterKeyTlv->type = gMeshCopTlvNwkMasterKey_c;
            pMasterKeyTlv->len = TLV_NETWORK_KEY_LEN;
            FLib_MemCpyReverseOrder(pMasterKeyTlv->masterKey, pMasterKey, pMasterKeyTlv->len);
            pPos += sizeof(meshCopNwkMasterKeyTlv_t);
        }

        if (pNwkName)
        {
            pNwkNameTlv = (meshCopNwkNameTlv_t *)pPos;
            pNwkNameTlv->type = gMeshCopTlvNwkName_c;
            pNwkNameTlv->len = nwkNameLength;
            FLib_MemCpy(pNwkNameTlv->nwkName, pNwkName, pNwkNameTlv->len);
            pPos += sizeof(meshCopNwkNameTlv_t) + pNwkNameTlv->len;
            pskcBitMask |= 0x04;
        }

        if (pPanId)
        {
            meshCopNwkPanIdTlv_t *pPanIdTlv = (meshCopNwkPanIdTlv_t *)pPos;
            pPanIdTlv->type = gMeshCopTlvPanID_c;
            pPanIdTlv->len = TLV_NETWORK_PANID_LEN;
            FLib_MemCpyReverseOrder(pPanIdTlv->panId, pPanId, pPanIdTlv->len);
            pPos += sizeof(meshCopNwkPanIdTlv_t);
        }

        if (pPskc)
        {
            pPskcTlv = (meshCopPskcTlv_t *)pPos;
            pPskcTlv->type = gMeshCopTlvPskc_c;
            pPskcTlv->len = THR_MAX_PSKC_LEN;
            FLib_MemSet(pPskcTlv->pskc, 0, THR_MAX_PSKC_LEN);
            FLib_MemCpy(pPskcTlv->pskc, pPskc, (pskcLen > THR_MAX_PSKC_LEN) ? THR_MAX_PSKC_LEN : pskcLen);
            pPos += sizeof(meshCopPskcTlv_t) + THR_MAX_PSKC_LEN;
            pskcBitMask |= 0x01;
        }

        if (pPolicy)
        {
            meshCopSecurityPolicyTlv_t *pPolicyTlv = (meshCopSecurityPolicyTlv_t *)pPos;
            pPolicyTlv->type = gMeshCopTlvSecPolicy_c;
            pPolicyTlv->len = sizeof(pPolicyTlv->policy) + sizeof(pPolicyTlv->rotationInterval);
            FLib_MemCpyReverseOrder(pPolicyTlv->rotationInterval, pPolicy,
                                    sizeof(pPolicyTlv->rotationInterval));
            pPolicyTlv->policy = *(pPolicy + sizeof(pPolicyTlv->rotationInterval));
            pPos += sizeof(meshCopSecurityPolicyTlv_t);
        }

        if (pActiveTimestamp)
        {
            meshCopActiveTimestampTlv_t *pActiveTimestampTlv = NULL;
            pActiveTimestampTlv = (meshCopActiveTimestampTlv_t *)pPos;
            pActiveTimestampTlv->type = gMeshCopTlvActiveTimestamp_c;
            pActiveTimestampTlv->len = sizeof(pActiveTimestampTlv->seconds) +
                                       sizeof(pActiveTimestampTlv->ticks);
            FLib_MemCpyReverseOrder(pActiveTimestampTlv->seconds, pActiveTimestamp,
                                    sizeof(pActiveTimestampTlv->seconds));
            FLib_MemCpyReverseOrder(pActiveTimestampTlv->ticks, pActiveTimestamp +
                                    sizeof(pActiveTimestampTlv->seconds), sizeof(pActiveTimestampTlv->ticks));
            pPos += sizeof(meshCopActiveTimestampTlv_t);
        }

        /* Pending only Dataset TLVs */
        if (pPendingTimestamp)
        {
            meshCopActiveTimestampTlv_t *pPendingTimestampTlv = NULL;
            pPendingTimestampTlv = (meshCopActiveTimestampTlv_t *)pPos;
            pPendingTimestampTlv->type = gMeshCopTlvPendingTimestamp_c;
            pPendingTimestampTlv->len = sizeof(pPendingTimestampTlv->seconds) +
                                        sizeof(pPendingTimestampTlv->ticks);
            FLib_MemCpyReverseOrder(pPendingTimestampTlv->seconds, pPendingTimestamp,
                                    sizeof(pPendingTimestampTlv->seconds));
            FLib_MemCpyReverseOrder(pPendingTimestampTlv->ticks, pPendingTimestamp +
                                    sizeof(pPendingTimestampTlv->seconds), sizeof(pPendingTimestampTlv->ticks));
            pPos += sizeof(meshCopPendingTimestampTlv_t);
        }

        if (pDelayTimer)
        {
            meshCopDelayTimerTlv_t *pDelayTimerTlv = (meshCopDelayTimerTlv_t *)pPos;
            pDelayTimerTlv->type = gMeshCopTlvDelayTimer_c;
            pDelayTimerTlv->len = sizeof(pDelayTimerTlv->timeRemaining);
            FLib_MemCpyReverseOrder(pDelayTimerTlv->timeRemaining, pDelayTimer,
                                    sizeof(pDelayTimerTlv->timeRemaining));
            pPos += sizeof(meshCopDelayTimerTlv_t);
        }

        if (futureTlv)
        {
            *pPos = 0x82;
            pPos++;
            *pPos = futureLen;
        }

        mgmtParams.pfCb = pfSetCb;
        mgmtParams.pDstIpAddr = (uint8_t *)&ipAddr;

        if (type == gTHCI_MeshCopMgmtCommSet_c)
        {
            status = MESHCOP_MgmtCommSet(&mgmtParams);
        }
        else if (type == gTHCI_MeshCopMgmtActiveSet_c)
        {
            if ((pskcBitMask == 0) || (pskcBitMask == 0x07) || (pskcBitMask == 0x05) ||
                    (pskcBitMask == 0x03) || 1)
            {
                status = MESHCOP_MgmtActiveSet(&mgmtParams);
            }
        }
        else if (type == gTHCI_MeshCopMgmtPendingSet_c)
        {
            if ((pskcBitMask == 0) || (pskcBitMask == 0x07) || (pskcBitMask == 0x05) ||
                    (pskcBitMask == 0x03) || 1)
            {
                status = MESHCOP_MgmtPendingSet(&mgmtParams);
            }
        }

        MEM_BufferFree(mgmtParams.pTlvs);

        if (status == gNwkStatusSuccess_c)
        {
            pConfirm->status = mTHCI_Ok_c;
        }
        else
        {
            pConfirm->status = mTHCI_Err_c;
        }

        replyNow = TRUE;
    }

    return replyNow;
}

/*!*************************************************************************************************
\private
\fn     THCI_MeshcopMgmtSetCb
\brief  This function is used to handle the response of Management Set command.

\param  [in]    pTlvs           pointer to the received tlvs
\param  [in]    tlvsLen         size of the received tlvs

\return         void
***************************************************************************************************/
static void THCI_MeshcopMgmtSetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    THCI_MeshcopMgmtReply(pTlvs, tlvsLen, gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopSet_c);
}

void THCI_MeshcopMgmtCommSetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    THCI_MeshcopMgmtReply(pTlvs, tlvsLen, gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopMgmtCommSet_c);
}

void THCI_MeshcopMgmtActiveSetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    THCI_MeshcopMgmtReply(pTlvs, tlvsLen, gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopMgmtActiveSet_c);
}

void THCI_MeshcopMgmtPendingSetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    THCI_MeshcopMgmtReply(pTlvs, tlvsLen, gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopMgmtPendingSet_c);
}

static void THCI_MeshcopMgmtReply
(
    uint8_t *pTlvs,
    uint32_t tlvsLen,
    uint8_t opGroup,
    uint8_t opCode
)
{
    uint8_t *pReply;
    uint32_t replyLen;
    uint8_t status = mTHCI_Err_c;
    bool_t freePacket = FALSE;

    /* Error occurred */
    if (!pTlvs)
    {
        replyLen = 1;
        pReply = &status;
    }
    /* Success */
    else
    {
        replyLen = 1 + tlvsLen;
        pReply = MEM_BufferAlloc(replyLen);

        if (pReply)
        {
            *pReply = mTHCI_Ok_c;
            FLib_MemCpy(pReply + 1, pTlvs, tlvsLen);
            freePacket = TRUE;
        }
    }

    if (!pReply)
    {
        replyLen = 1;
        pReply = &status;
    }

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopSet_c, pReply, replyLen,
                         mFsciAppInterfaceId);

    if (freePacket)
    {
        MEM_BufferFree(pReply);
    }
}

/*!*************************************************************************************************
\private
\fn     THCI_MeshcopMgmtGetCb
\brief  This function is used to handle the response of Management Get command.

\param  [in]    pTlvs           pointer to the received tlvs
\param  [in]    tlvsLen         size of the received tlvs

\return         void
***************************************************************************************************/
static void THCI_MeshcopMgmtGetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    uint8_t *pReply;
    uint32_t replyLen;
    uint8_t status = mTHCI_Err_c;
    bool_t freePacket = FALSE;

    /* Error occurred */
    if (!pTlvs)
    {
        replyLen = 1;
        pReply = &status;
    }
    /* Success */
    else
    {
        replyLen = 1 + tlvsLen;
        pReply = MEM_BufferAlloc(replyLen);

        if (pReply)
        {
            *pReply = mTHCI_Ok_c;
            FLib_MemCpy(pReply + 1, pTlvs, tlvsLen);
            freePacket = TRUE;
        }
    }

    if (!pReply)
    {
        replyLen = 1;
        pReply = &status;
    }

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopGet_c, pReply, replyLen,
                         mFsciAppInterfaceId);

    if (freePacket)
    {
        MEM_BufferFree(pReply);
    }
}


/*!*************************************************************************************************
\private
\fn     THCI_MeshcopMgmtPanIdConflictCb
\brief  This function is used to handle PanId conflicts.

\param  [in]    pTlvs           pointer to the received tlvs
\param  [in]    tlvsLen         size of the received tlvs

\return         void
***************************************************************************************************/
void THCI_MeshcopMgmtPanIdConflictCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    uint8_t *pReply;
    uint32_t replyLen;
    uint8_t status = mTHCI_Err_c;
    bool_t freePacket = FALSE;

    /* Error occurred */
    if (!pTlvs)
    {
        replyLen = 1;
        pReply = &status;
    }
    /* Success */
    else
    {
        replyLen = 1 + tlvsLen;
        pReply = MEM_BufferAlloc(replyLen);

        if (pReply)
        {
            *pReply = mTHCI_Ok_c;
            FLib_MemCpy(pReply + 1, pTlvs, tlvsLen);
            freePacket = TRUE;
        }
    }

    if (!pReply)
    {
        replyLen = 1;
        pReply = &status;
    }

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopMgmtPanIdConflict_c, pReply, replyLen,
                         mFsciAppInterfaceId);

    if (freePacket)
    {
        MEM_BufferFree(pReply);
    }
}

void THCI_MeshcopMgmtCommGetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    uint8_t *pReply;
    uint32_t replyLen;
    uint8_t aStatus[1 + sizeof(tlvsLen)] = {mTHCI_Err_c, 0, 0, 0, 0};
    bool_t freePacket = FALSE;

    /* Error occurred */
    if (!pTlvs)
    {
        replyLen = 1;
        pReply = aStatus;
        tlvsLen = 0;
    }
    /* Success */
    else
    {
        replyLen = 1 + sizeof(tlvsLen) + tlvsLen;
        pReply = MEM_BufferAlloc(replyLen);

        if (pReply)
        {
            *pReply = mTHCI_Ok_c;
            FLib_MemCpy(pReply + 1 + sizeof(tlvsLen), pTlvs, tlvsLen);
            freePacket = TRUE;
        }
    }

    if (!pReply)
    {
        replyLen = 1 + sizeof(tlvsLen);
        pReply = aStatus;
        tlvsLen = 0;
    }

    /* Set Length */
    FLib_MemCpy(pReply + 1, &tlvsLen, sizeof(tlvsLen));

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopMgmtCommGet_c, pReply, replyLen,
                         mFsciAppInterfaceId);

    if (freePacket)
    {
        MEM_BufferFree(pReply);
    }
}

void THCI_MeshcopMgmtActiveGetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    uint8_t *pReply;
    uint32_t replyLen;
    uint8_t aStatus[1 + sizeof(tlvsLen)] = {mTHCI_Err_c, 0, 0, 0, 0};
    bool_t freePacket = FALSE;

    /* Error occurred */
    if (!pTlvs)
    {
        replyLen = 1;
        pReply = aStatus;
        tlvsLen = 0;
    }
    /* Success */
    else
    {
        replyLen = 1 + sizeof(tlvsLen) + tlvsLen;
        pReply = MEM_BufferAlloc(replyLen);

        if (pReply)
        {
            *pReply = mTHCI_Ok_c;
            FLib_MemCpy(pReply + 1 + sizeof(tlvsLen), pTlvs, tlvsLen);
            freePacket = TRUE;
        }
    }

    if (!pReply)
    {
        replyLen = 1 + sizeof(tlvsLen);
        pReply = aStatus;
        tlvsLen = 0;
    }

    /* Set Length */
    FLib_MemCpy(pReply + 1, &tlvsLen, sizeof(tlvsLen));

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopMgmtActiveGet_c, pReply, replyLen,
                         mFsciAppInterfaceId);

    if (freePacket)
    {
        MEM_BufferFree(pReply);
    }
}

void THCI_MeshcopMgmtPendingGetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    uint8_t *pReply;
    uint32_t replyLen;
    uint8_t aStatus[1 + sizeof(tlvsLen)] = {mTHCI_Err_c, 0, 0, 0, 0};
    bool_t freePacket = FALSE;

    /* Error occurred */
    if (!pTlvs)
    {
        replyLen = 1;
        pReply = aStatus;
        tlvsLen = 0;
    }
    /* Success */
    else
    {
        replyLen = 1 + sizeof(tlvsLen) + tlvsLen;
        pReply = MEM_BufferAlloc(replyLen);

        if (pReply)
        {
            *pReply = mTHCI_Ok_c;
            FLib_MemCpy(pReply + 1 + sizeof(tlvsLen), pTlvs, tlvsLen);
            freePacket = TRUE;
        }
    }

    if (!pReply)
    {
        replyLen = 1 + sizeof(tlvsLen);
        pReply = aStatus;
        tlvsLen = 0;
    }

    /* Set Length */
    FLib_MemCpy(pReply + 1, &tlvsLen, sizeof(tlvsLen));

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopMgmtPendingGet_c, pReply, replyLen,
                         mFsciAppInterfaceId);

    if (freePacket)
    {
        MEM_BufferFree(pReply);
    }
}

/*!*************************************************************************************************
\private
\fn     THCI_MeshcopMgmtEdReportCb
\brief  This function is used to handle PanId conflicts.

\param  [in]    pTlvs           pointer to the received tlvs
\param  [in]    tlvsLen         size of the received tlvs

\return         void
***************************************************************************************************/
static void THCI_MeshcopMgmtEdReportCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pData,
    uint32_t dataLen
)
{
    uint8_t *pReply = NULL;
    uint8_t *pPos = NULL;
    uint32_t replyLen;
    uint8_t status = mTHCI_Err_c;
    bool_t freePacket = FALSE;

    /* Error occurred */
    if (pData == NULL)
    {
        replyLen = sizeof(uint8_t);
        pReply = &status;
    }
    /* Success */
    else
    {
        replyLen = sizeof(uint8_t) + dataLen;
        pReply = MEM_BufferAlloc(replyLen);

        if (pReply)
        {
            pPos = pReply;
            *pPos = mTHCI_Ok_c;
            pPos++;
            FLib_MemCpy(pPos, pData, dataLen);
            freePacket = TRUE;
        }
        else
        {
            replyLen = 1;
            pReply = &status;
        }
    }

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopMgmtEdReport_c, pReply, replyLen,
                         mFsciAppInterfaceId);

    if (freePacket)
    {
        MEM_BufferFree(pReply);
    }
}

/*!*************************************************************************************************
\private
\fn     THCI_MeshcopNetFormCb
\brief  This function is used to handle the response of Net Form command.

\param  [in]    pTlvs           pointer to the received tlvs
\param  [in]    tlvsLen         size of the received tlvs

\return         void
***************************************************************************************************/
static void THCI_MeshcopNetFormCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    uint8_t *pReply;
    uint32_t replyLen;
    uint8_t status = mTHCI_Err_c;

    pReply = &status;
    replyLen = 1;

    if (pTlvs)
    {
        meshCopStateTlv_t *pStateTlv;
        pStateTlv = (meshCopStateTlv_t *)NWKU_GetTlv(gMeshCopTlvState_c, pTlvs, tlvsLen, NULL, 0);

        if (pStateTlv)
        {
            if (pStateTlv->state == MESHCOP_STATE_ACCEPT)
            {
                status = mTHCI_Ok_c;
            }
        }
    }

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_MeshCopGet_c, pReply, replyLen,
                         mFsciAppInterfaceId);
}

/*!*************************************************************************************************
\private
\fn     bool_t THCI_MeshCopMgmtGet
\brief  This function is used to do a Management Set request.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         TRUE            reply is needed
\return         FALSE           reply will be sent later
***************************************************************************************************/
static bool_t THCI_MeshCopMgmtGet
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    uint8_t *pInput = pClientPacket;
    nwkStatus_t status;
    bool_t replyNow;

    status = MESHCOP_Get(*pInput, pInput + 2, *(pInput + 1), THCI_MeshcopMgmtGetCb);

    if (status == gNwkStatusSuccess_c)
    {
        pConfirm->status = mTHCI_Ok_c;
        replyNow = FALSE;
    }
    else
    {
        pConfirm->status = mTHCI_Err_c;
        replyNow = TRUE;
    }

    return replyNow;
}

/*!*************************************************************************************************
\private
\fn     void THCI_MeshCopSetCommissionerCredential
\brief  This function is used to set the attributes for an user specified network on this device.
        This will be used later when trying to connect to that network using
        MESHCOP_StartCommmissioner.

\param  [in]    pClientPacket   pointer to the packet received from FSCI
\param  [out]   pConfirm        pointer to the confirm structure

\return         void
***************************************************************************************************/
static void THCI_MeshCopSetCommissionerCredential
(
    uint8_t *pClientPacket,
    statusConfirm_t *pConfirm
)
{
    uint8_t *pInput = pClientPacket;
    meshcopCredentialInput_t inputParams;
    instanceId_t thrInstId;

    thrInstId = *pInput;
    pInput++;

    inputParams.pskcStrLen = *pInput;
    pInput++;
    inputParams.pPskcStr = pInput;
    pInput += inputParams.pskcStrLen;
    inputParams.pXpanId = pInput;
    pInput += 8;
    inputParams.nwkNameLen = *pInput;
    pInput++;
    inputParams.pNwkName = pInput;

    MESHCOP_SetCommissionerCredential(thrInstId, &inputParams);
    pConfirm->status = gThrStatus_Success_c;
}
#endif /* MESHCOP_ENABLED */

/*!*************************************************************************************************
\fn    void THCI_ResetCpuEvent(resetCpuStatus_t resetStatus, uint32_t timeoutMs)
\brief  This function is used to send the reset cpu event

\param [in]   resetStatus    reset status: gResetCpuSuccess_c or gResetCpuPending_c
[in]   timeoutMs      timeout in milliseconds: used for ResetCpuPending_c event
***************************************************************************************************/
void THCI_ResetCpuEvent(resetCpuStatus_t resetStatus, uint32_t timeoutMs)
{
    uint8_t payloadLen = sizeof(uint8_t);
    thciResetCpuEvent_t resetCpuEvent =
    {
        .status = gResetCpuSuccess_c,
        .resetPayload.resetSuccess.boardNameLen = sizeof(BOARD_NAME),
        .resetPayload.resetSuccess.boardName = BOARD_NAME,
        .resetPayload.resetSuccess.uniqueMcuId = {0x00, 0x00, 0x00, 0x00},
        .resetPayload.resetSuccess.stackVersion = THR_VENDOR_STACK_VERSION,
        .resetPayload.resetSuccess.swVersion = THR_VENDOR_SW_VERSION
    };

    resetCpuEvent.status = resetStatus;

    if (resetStatus == gResetCpuPending_c)
    {
        resetCpuEvent.resetPayload.resetPending.timeout[0] = (uint8_t)timeoutMs;
        resetCpuEvent.resetPayload.resetPending.timeout[1] = (uint8_t)(timeoutMs >> 8);
        resetCpuEvent.resetPayload.resetPending.timeout[2] = (uint8_t)(timeoutMs >> 16);
        resetCpuEvent.resetPayload.resetPending.timeout[3] = (uint8_t)(timeoutMs >> 24);
        payloadLen += sizeof(thciResetCpuPending_t);
    }
    else
    {
        /* get unique MCU ID */
        THR_GetUniqueId((uint32_t *)&resetCpuEvent.resetPayload.resetSuccess.uniqueMcuId);

        payloadLen += (sizeof(thciResetSuccess_t) - sizeof(thrOctet16_t));
        payloadLen += (1 + resetCpuEvent.resetPayload.resetSuccess.swVersion.length);
    }

    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_ThrCpuResetIndication_c,
                         &resetCpuEvent, payloadLen, mFsciAppInterfaceId);
}

#if THR_ENABLE_EVENT_MONITORING
/*!*************************************************************************************************
\fn    uint16_t THCI_EvMonitorGetSize(eventMonitorData_t *pEventMonitorData)
\brief

\param [in]   param    pointer to monitor event
***************************************************************************************************/
uint16_t THCI_EvMonitorGetSize(eventMonitorData_t *pEventMonitorData)
{
    uint16_t len = 0;

    if (pEventMonitorData)
    {
        len = ((uint16_t)(pEventMonitorData->dataSize[1] << 8)) + pEventMonitorData->dataSize[0];
        len += sizeof(eventMonitorData_t) - 2; /* event set is ignored  */
    }

    return len;
}

/*!*************************************************************************************************
\fn    uint16_t APP_EvMonitorFsciGetSize(eventMonitorData_t *pEventMonitorData)
\brief

\param [in]   param    pointer to monitor event
***************************************************************************************************/
uint8_t THCI_EvMonitorGetOpcode(eventMonitorData_t *pEventMonitorData)
{
    uint8_t opcode = 0x00;
    uint32_t eventSet = ((pEventMonitorData->eventSet[0] << 16) + (pEventMonitorData->eventSet[1] << 24));
    uint32_t eventCode = pEventMonitorData->eventStatus[0] + (pEventMonitorData->eventStatus[1] << 8) + eventSet;

    switch (eventSet)
    {
        case gThrEvSet_NwkScan_c:
            opcode = gTHCI_EventNwkScan_c;
            break;

        case gThrEvSet_NwkCreate_c:
            opcode = gTHCI_EventNwkCreate_c;
            break;

        case gThrEvSet_NwkJoin_c:
            opcode = gTHCI_EventNwkJoin_c;
            break;

        case gThrEvSet_NwkGeneral_c:
            opcode = gTHCI_EventNwkGeneral_c;

            if (eventCode == gThrEv_GeneralInd_ResetMcuTimeout_c)
            {
                THCI_ResetCpuEvent(gResetCpuPending_c, APP_GetResetMcuTimeout());
                opcode = 0x00;
            }

            break;

        case gThrEvSet_NwkSelectParents_c:
            opcode = gTHCI_EventNwkSelectParents_c;
            break;

        case gThrEvSet_NwkCommissioning_c:
            opcode = gTHCI_EventNwkCommissioning_c;
            break;

        default:
            break;
    }

    return opcode;
}

/*!*************************************************************************************************
\fn    bool_t THCI_EvMonitor( void* param)
\brief

\param [in]   param    pointer to monitor event
***************************************************************************************************/
void THCI_EvMonitor
(
    void *param
)
{
    if (param)
    {
        eventMonitorData_t *pTempValue = (eventMonitorData_t *)param;
        uint8_t opCode = THCI_EvMonitorGetOpcode(pTempValue);

        if (opCode)
        {
            THCI_EventData(opCode, THCI_EvMonitorGetSize(pTempValue),
                           &pTempValue->instanceId);
        }

        MEM_BufferFree(param);
    }
}
#endif

#if UDP_ECHO_PROTOCOL
/*!*************************************************************************************************
\fn    void THCI_EchoUdpRcv(uint8_t status)
\brief

\param [in]   status    status of the event
***************************************************************************************************/
void THCI_EchoUdpRcv
(
    uint8_t status
)
{
    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_ThrEchoUdp_c,
                         &status, sizeof(uint8_t), mFsciAppInterfaceId);
}
#endif
/*!*************************************************************************************************
\fn     void THCI_EventData(uint8_t opCode, uint8_t length, uint8_t *pData)
\brief

\param  [in]    opCode          FSCI opcode
\param  [in]    length          FSCI message length
\param  [in]    pData           pointer to data

\return         void
***************************************************************************************************/
void THCI_EventData(uint8_t opCode, uint8_t length, uint8_t *pData)
{
    THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, opCode,
                         pData, length, mFsciAppInterfaceId);
}

#if THR_ENABLE_MGMT_DIAGNOSTICS && LARGE_NETWORK
/*!*************************************************************************************************
\fn     void THCI_DiagnosticsEventDataCb(uint32_t reqLatency, uint8_t seqNum)
\brief  This function is used to send over THCI the request latency when receiving a multicast
        DiagTest request for latency data, in a large network.

\param  [in]    reqLatency           Request latency
\param  [in]    seqNum               Sequence number

\return         void
***************************************************************************************************/
void THCI_DiagnosticsEventDataCb
(
    uint32_t reqLatency,
    uint8_t seqNum
)
{
    uint16_t length = sizeof(uint32_t) + sizeof(uint8_t);
    uint8_t *pData = NULL;

    pData = NWKU_MEM_BufferAlloc(length);

    if (pData != NULL)
    {
        FLib_MemCpy(pData, &reqLatency, sizeof(uint32_t));
        FLib_MemCpy(pData + sizeof(uint32_t), &seqNum, sizeof(uint8_t));

        THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gFsci_MgmtDiagnosticDiagTestEv_c, pData, length, mFsciAppInterfaceId);
        MEM_BufferFree(pData);
    }
}
#endif

/*!*************************************************************************************************
\fn    THCI_MgmtDiagnosticAppCb(mgmtDiagnostic_RspData_t mgmtDiagRspData)
\brief This function is used to receive the network diagnostic response
       at the application level. The message is forwarding using FSCI interface

\param  [in]    commandId       Network diagnostic command Id
\param  [in]    coapStatus      CoapStatus
\param  [in]    coapMsgId       Coap Message ID
\param  [in]    dataLen         data length
\param  [in]    pData           pointer to data

\return         void
***************************************************************************************************/
void THCI_MgmtDiagnosticAppCb(mgmtDiagnostic_RspData_t *mgmtDiagRspData)
{
#if THR_ENABLE_MGMT_DIAGNOSTICS
    uint8_t opCode;
    thciMgmtDiagRsp_t *pMgmtDiagnostic;
    uint16_t remainingBytes = 0;
    uint8_t *pPos = NULL;

    pMgmtDiagnostic = MEM_BufferAlloc(sizeof(thciMgmtDiagRsp_t) + mgmtDiagRspData->payloadLen);

    if (pMgmtDiagnostic)
    {
        FLib_MemSet(pMgmtDiagnostic, 0, sizeof(thciMgmtDiagRsp_t) + mgmtDiagRspData->payloadLen);
        pMgmtDiagnostic->status = mgmtDiagRspData->status;

        switch (mgmtDiagRspData->commandId)
        {
            case gMgmtDiagnostic_GetRsp_c:
                opCode = gFsci_MgmtDiagnosticGetRsp_c;
                pMgmtDiagnostic->noOfTLVs = 0;
                break;

            case gMgmtDiagnostic_ResetRsp_c:
                opCode = gFsci_MgmtDiagnosticResetRsp_c;
                break;

            case gMgmtDiagnostic_DiagTestRsp_c:
                opCode = gFsci_MgmtDiagnosticDiagTestRsp_c;
                pMgmtDiagnostic->noOfTLVs = 1;
                break;

            default:
                MEM_BufferFree(pMgmtDiagnostic);
                return;
        }

        pMgmtDiagnostic->msgId[0] = (uint8_t)mgmtDiagRspData->msgId;
        pMgmtDiagnostic->msgId[1] = (uint8_t)(mgmtDiagRspData->msgId >> 8);
        pMgmtDiagnostic->dataLen[0] = (uint8_t)mgmtDiagRspData->payloadLen;
        pMgmtDiagnostic->dataLen[1] = (uint8_t)(mgmtDiagRspData->payloadLen >> 8);

        if(mgmtDiagRspData->commandId == gMgmtDiagnostic_GetRsp_c)
        {
            if(mgmtDiagRspData->payloadLen != 0)
            {
                remainingBytes = (uint16_t)mgmtDiagRspData->payloadLen;
            }

            if(mgmtDiagRspData->pPayload != NULL)
            {
                /* Put pointer on the length of the first TLV */
                pPos = (uint8_t *)(mgmtDiagRspData->pPayload + 1);
            }

            while(remainingBytes != 0)
            {
                /* Increment number of TLVs in packet */
                pMgmtDiagnostic->noOfTLVs++;
                /* Decrement with number of bytes in the whole TLV */
                remainingBytes -= (*pPos + 2);
                /* Move to the length value in the next TLV */
                pPos += (*pPos + 2);
            }
        }

        if((mgmtDiagRspData->pPayload != NULL) || (mgmtDiagRspData->payloadLen != 0))
        {
            FLib_MemCpy(pMgmtDiagnostic->pData, mgmtDiagRspData->pPayload, mgmtDiagRspData->payloadLen);
        }

        /* send response */
        THCI_transmitPayload(gFSCI_IpStackOpGCnf_c, opCode, pMgmtDiagnostic,
                             sizeof(thciMgmtDiagRsp_t) + mgmtDiagRspData->payloadLen, mFsciAppInterfaceId);
        MEM_BufferFree(pMgmtDiagnostic);
    }

#else
    (void)mgmtDiagRspData;
#endif
}


#if THCI_USBENET_ENABLE
#include "rndis.h"
typedef uint8_t   fsci_enet_address_t[6];
extern fsci_enet_address_t usbenet_internal_mac_addr;
extern fsci_enet_address_t usbenet_rndis_mac_addr;
typedef struct fscienetHdr_tag
{
    uint8_t    dest[6];
    uint8_t    source[6];
    uint8_t    type[2];
} fscienetHdr_t;

#define gFsciProt_c  0xB588

#endif

/*!*************************************************************************************************
\fn     void THCI_transmitPayload(uint8_t OG, uint8_t OC, void * pMsg, uint16_t msgLen, uint32_t fsciInterface)
\brief

\param[in] OG operation Group
\param[in] OC operation Code
\param[in] pMsg pointer to payload
\param[in] msgLen length of the payload
\param[in] fsciInterface the interface on which the packet should be sent

\return         void
***************************************************************************************************/
static void THCI_transmitPayload(uint8_t OG, uint8_t OC, void *pMsg, uint16_t msgLen, uint32_t fsciInterface)
{
#if !THCI_USBENET_ENABLE || !USB_RNDIS_IF
    FSCI_transmitPayload(OG, OC, pMsg, msgLen, fsciInterface);
#else
    uint16_t outLen = 0;
    uint8_t *pFormattedPacket = FSCI_GetFormattedPacket(OG, OC, pMsg, msgLen, &outLen);

    if (NULL != pFormattedPacket)
    {
        uint8_t *pUsbenetFrame = MEM_BufferAlloc(outLen + sizeof(fscienetHdr_t));

        if (NULL != pUsbenetFrame)
        {
            fscienetHdr_t *pEnetHdr = (fscienetHdr_t *)pUsbenetFrame;
            uint16_t protType = gFsciProt_c;
            IP_UsbEnetGetMacAddr(pEnetHdr->dest, sizeof(fsci_enet_address_t), FALSE);
            IP_UsbEnetGetMacAddr(pEnetHdr->source, sizeof(fsci_enet_address_t), TRUE);
            FLib_MemCpy(pEnetHdr->type, (void *)&protType, sizeof(uint16_t));
            FLib_MemCpy(pUsbenetFrame + sizeof(fscienetHdr_t), pFormattedPacket, outLen);
            (void)RNDIS_Send(pUsbenetFrame, outLen + sizeof(fscienetHdr_t));
        }

        MEM_BufferFree(pFormattedPacket);
    }

#endif
}

#endif /* THREAD_USE_THCI */

/*==================================================================================================
Private debug functions
==================================================================================================*/

