/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       shell_ip.c
\brief      This is a public source file for the shell application. It contains the implementation
            of the shell commands used in the application.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "coap.h"

#if THREAD_USE_SHELL
#include "shell_ip.h"
#include "app_stack_config.h"
#include "stack_config.h"
#include "app_init.h"

#include "shell.h"
#include "ModuleInfo.h"
#include "sockets.h"
#include "ip_if_management.h"
#include "ip6.h"
#include "icmp.h"
#include "sixlowpan.h"
#include "FunctionLib.h"
#include "session.h"
#include "nd.h"
#include "nd_cfg.h"
#include "mac_filtering.h"
#include "thread_utils.h"
#include "thread_types.h"
#include "thread_meshcop.h"
#include "thread_meshcop_mgmt.h"
#include "thread_mgmt.h"
#include "thread_attributes.h"
/* LED Application */
#include "app_led.h"

#if STACK_THREAD
#include "thread_network.h"
#include "thread_utils.h"
#include "mle_basic.h"
#include "app_thread_config.h"
#endif

#if THR_ENABLE_EVENT_MONITORING
#include "app_event_monitoring.h"
#endif

#if SOCK_DEMO
#include "app_socket_utils.h"
#endif

#if UDP_ECHO_PROTOCOL
#include "app_echo_udp.h"
#endif

#if COAP_OBSERVE_CLIENT || COAP_OBSERVE_SERVER
#include "app_observe_demo.h"
#endif

#if DNS_ENABLED
#include "dns_client.h"
#include "dns_utils.h"
#include "thread_dns.h"
#endif

extern bool_t gbRetryInterrupt;
#ifdef USE_MSD_BOOTLOADER
extern void NvEraseSector(uint32_t sectorAddr);
#endif

#if THR_ENABLE_EVENT_MONITORING
extern moduleInfo_t IP_version;
#endif

#if ECHO_PROTOCOL
extern uint16_t mEchoUdpCounter;
#endif

/*==================================================================================================
Private macros
==================================================================================================*/
#define EOL                         "\r\n"

#define PING_ID                     (1U)
#define PING_SEQ_NB                 (1U)
#define PING_PAYLOAD_DEFAULT_SIZE   (32U)
#define PING_PAYLOAD_START          '@'
#define PING_PAYLOAD_END            'W'
#if SHELL_DUT_COMMISSIONER
#define SHELL_CMD_MAX_ARGS          (25U)
#else
#define SHELL_CMD_MAX_ARGS          (20U)
#endif
#define PING_HEADER_SIZE            (4U)
#define DEFAULT_TIMEOUT             (2000U)
#define SHELL_PING_MIN_TIMEOUT      (2000U)
#define PING_DELAY                  (500U)
#if COAP_OBSERVE_CLIENT
    #define SHELL_MAX_COAP_URI      (30U)
#endif
#define STRING_TRUE                 "TRUE"
#define STRING_FALSE                "FALSE"

/*! NXP Thread stack Version - Human readable form, for SHELL */
#define gNXPThreadStackVersion_c "NXP " \
                                 QUH(gIpVerMajor_c) "." \
                                 QUH(gIpVerMinor_c) "." \
                                 QUH(gIpVerPatch_c)

#define SHELL_HEX_PREFIX_LC     "0x"    /* lower case */
#define SHELL_HEX_PREFIX_UC     "0X"    /* upper case */
#define SHELL_HEX_PREFIX_LEN    2
/*==================================================================================================
Private type definitions
==================================================================================================*/
typedef void (*pfGetTable_t)(void* param);

typedef enum shellValueType_tag
{
    gString_c,
    gDecimal_c,
    gHex_c,
    gHexReversed_c,
    gArray_c,
    gTable_c,
}shellValueType_t;

typedef struct aShellThrSetAttr_tag
{
    char            *pAttrName;
    thrAttrId_t      attrId;
    shellValueType_t valueType;
    uint32_t         maxSize;
    pfGetTable_t     pfGetTable;
    bool_t           writable;
}aShellThrSetAttr_t;

typedef struct aShellThrInterface_tag
{
    uint8_t        *pAttrName;
    ipIfUniqueId_t ifId;
}aShellThrInterface_t;

#if SHELL_DUT_COMMISSIONER
typedef struct aShellThrMgmtSetTlv_tag
{
    char                *pTlvName;
    meshCopTlv_t        tlvId;
    uint32_t            tlvLen;
}aShellThrMgmtSetTlv_t;

typedef struct shellMgmtCb_tag
{
    uint32_t tlvsLen;
    uint8_t  pTlvs[];
}shellMgmtCb_t;
#endif
/*==================================================================================================
Private prototypes
==================================================================================================*/

static void SHELL_Resume(void);
static int8_t SHELL_Reboot(uint8_t argc, char *argv[]);
static int8_t SHELL_Identify(uint8_t argc, char *argv[]);
static void SHELL_Process(uint8_t *param);
static void SHELL_ProcessCb(char *pCmd, uint16_t length);
static void SHELL_InterruptCb(void);
static int8_t SHELL_Ifconfig(uint8_t argc, char *argv[]);
#if IP_IP6_ROUTING_ENABLE
static int8_t SHELL_Routes(uint8_t argc, char *argv[]);
static void SHELL_PrintIPv6Routes(void *param);
#endif
static int8_t SHELL_FactoryResetReq(uint8_t argc, char *argv[]);
static int8_t SHELL_ThrCommands(uint8_t argc, char *argv[]);
static void SHELL_ThrNwkJoin(uint8_t *param);

#if !THREAD_ED_CONFIG
static void SHELL_ThrNwkCreate(uint8_t *param);
static void SHELL_ThrStartCommissioner(uint8_t *param);
static void SHELL_ThrStopCommissioner(uint8_t *param);
static void SHELL_PrintJoinersTbl(void *param);
static void SHELL_PrintRoutingTbl(void *param);
#endif

static ipIfUniqueId_t SHELL_SearchInterfaceByName(uint8_t *pName, uint8_t length);
static uint8_t *SHELL_SearchInterfaceById(ipIfUniqueId_t ifID);
static void SHELL_PrintStatus(thrStatus_t statusCode);
static void SHELL_PrintParentInfo(void *param);
static void SHELL_PrintNeighborTbl(void *param);
static void SHELL_PrintNeighborInfo(void *param);
static void SHELL_PrintNeighborState(uint8_t state);
static void SHELL_PrintDevRole(void *param);
static void SHELL_PrintDevType(void *param);
static void SHELL_PrintNwkCapabilities(void *param);
static void SHELL_PrintBuff(uint8_t *buff, uint32_t length);
static void SHELL_ReadBuff(char *pValue, uint8_t *pDest, uint32_t length);
static void SHELL_PrintActiveTimestamp(void *param);

/* CoAP functions */
static int8_t SHELL_CoapSend(uint8_t argc, char *argv []);
static void SHELL_CoapAckReceive(coapSessionStatus_t sessionStatus, uint8_t *pData, coapSession_t *pSession, uint32_t dataLen);

/* Ping functions */
static int8_t SHELL_Ping(uint8_t argc, char *argv[]);
static ipPktInfo_t *PING_CreatePktInfo(ipAddr_t *pDstAddr, uint32_t payloadLen);
void PING_EchoReplyReceiveAsync(ipPktInfo_t *pRxIpPktInfo);
static void PING_EchoReplyReceive(uint8_t *pParam);
static void PING_TimerCallback(void *param);
static void PING_HandleTimerCallback(uint8_t *param);
static void PING_RetransmitCallback(void *param);
static void PING_RetransmitHandle(uint8_t *param);
static void PING_SendPing(ipPktInfo_t *pPingIpPktInfo, ipAddr_t *pSrcIpAddr);
#if ICMP_STATISTICS_ENABLED
static void SHELL_PrintPingStatistics();
static void float_division(char *intPart, char *fractPart, uint32_t sent, uint32_t lost);
#endif
static int8_t SHELL_MulticastGroups(uint8_t argc, char *argv[]);

#if SOCK_DEMO
static int8_t SHELL_Socket(uint8_t argc, char *argv[]);
#endif

#if UDP_ECHO_PROTOCOL
static int8_t SHELL_EchoUdp(uint8_t argc, char *argv[]);
#endif

#if DNS_ENABLED
static int8_t SHELL_SendDns(uint8_t argc, char *argv[]);
void SHELL_DnsService(uint8_t* pMsg);
void SHELL_DnsNdDataService(coapSessionStatus_t sessionStatus, uint8_t *pData,
                                   coapSession_t *pSession, uint32_t dataLen);
#endif

#if MAC_FILTERING_ENABLED
static int8_t SHELL_Filter(uint8_t argc, char *argv[]);
static void SHELL_Filtering_Print(uint32_t instanceID);
#endif /* MAC_FILTERING_ENABLED */

#ifdef USE_MSD_BOOTLOADER
static int8_t SHELL_FlashEraseWithMSDFunc(uint8_t argc, char *argv[]);
#endif

#if gHybridApp_d
static int8_t SHELL_BreakReq(uint8_t argc, char *argv[]);
#endif

#if IP_IP6_FIREWALL_ENABLE
static int8_t SHELL_IpFirewall(uint8_t argc, char *argv[]);
static void SHELL_PrintFirewallRules(void * param);
#endif

#if GETIPv6ADDRESSES_APP
/*===================================GETML64ADDRESSES=============================================*/
static int8_t SHELL_GetNeighborsIpAddr(uint8_t argc, char *argv[]);
void SHELL_MgmtDiagnosticAppCb(mgmtDiagnostic_RspData_t *mgmtDiagRspData);
/*================================================================================================*/
#endif

#if SHELL_DUT_COMMISSIONER
static void SHELL_ReadMgmtTlv(uint8_t argc, char *argv[], uint8_t *pTlvBuff,
                                        uint32_t* pTlvBuffLen, bool_t bTlvSet);

static void SHELL_MgmtPanIdConflictCbHandler(uint8_t *param);

static void SHELL_MgmtGetCbHandler(uint8_t *param);
static void SHELL_MgmtSetCbHandler(uint8_t *param);
#endif
static bool_t SHELL_ReadIPv6Addr(uint8_t argc, char *argv[], char *pOption, ipAddr_t *pIpAddr);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static bool_t           mContinuousPing = FALSE;
static uint32_t         mPingTimeoutMs;
static uint16_t         mDefaultSeqNb = PING_SEQ_NB;
static uint16_t         mPingSize = 0;
static uint16_t         mPingCounter = 0;
static ipAddr_t         mDstIpAddr;
static uint32_t         mPingMacSec = 5;
static ipIfUniqueId_t   mPingInterfaceId = gIpIfUndef_c;

static tmrTimerID_t     mDelayTimerID = gTmrInvalidTimerID_c;
static char             mAddrStr[INET6_ADDRSTRLEN];
static tmrTimerID_t     mTimerIDSelect = gTmrInvalidTimerID_c;

static bool_t           mShellCommandsEnabled = FALSE;    /*!< Avoid initializing the module multiple times */
static uint8_t          mCoapInstId = THR_ALL_FFs8;       /*!< CoAP instance */
static bool_t           mShellBoardIdentify = FALSE;
static uint8_t          mPingHopLimit = ICMP_DEFAULT_HOP_LIMIT;
#if COAP_OBSERVE_CLIENT
    static uint8_t mResourceString[SHELL_MAX_COAP_URI] = {0};
    static coapUriPath_t mResource;
#endif
/*==================================================================================================
Public global variables declarations
==================================================================================================*/

const cmd_tbl_t aShellCommands[] =
{
    {
        "                   ", SHELL_CMD_MAX_ARGS, 0, NULL
#if SHELL_USE_HELP
        ,"",
        ""
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    {
        "thr", SHELL_CMD_MAX_ARGS, 0, SHELL_ThrCommands
#if SHELL_USE_HELP
    ,"Thread Stack commands",
    "Commands for Thread Network\r\n"
#if !THREAD_ED_CONFIG
        "   thr create\r\n"
#endif
        "   thr join\r\n"
        "   thr scan [active|energy|both]\r\n"
        "   thr detach\r\n"
#if !THREAD_ED_CONFIG
        "   thr commissioner [start|stop]\r\n"
        "   thr joiner add <psk> <eui>\r\n"
        "   thr joiner remove <eui>\r\n"
        "   thr joiner removeall\r\n"
        "   thr joiner view\r\n"
        "   thr sync steering\r\n"
        "   thr sync nwkdata\r\n"
#endif
        "   thr get attributes - displays a list of all Thread attributes available for getting/setting\r\n"
        "   thr get <ATTRNAME/TABLENAME> - displays the value for the specified attribute\r\n"
        "   thr set <ATTRNAME> <value> - changes the value of the attribute with the specified value\r\n"
#if !THREAD_ED_CONFIG
        "   thr nwkdata add slaac -p <Prefix> - len <prefixLength> -t <lifetime in seconds>\r\n"
#if DHCP6_SERVER_ENABLED
        "   thr nwkdata add dhcpserver -p <Prefix> - len <prefixLength> -t <lifetime in seconds>\r\n"
#endif
        "   thr nwkdata add extroute -p <Prefix> - len <prefixLength> -t <lifetime in seconds>\r\n"
#if DNS_ENABLED
        "   thr nwkdata add dnsserver <ipAddress>\r\n"
        "   thr nwkdata remove dnsserver <ipAddress>\r\n"
#endif
        "   thr nwkdata remove -p <Prefix> - len <prefixLength>\r\n"
        "   thr nwkdata removeall\r\n"
#endif
#if SHELL_DUT_COMMISSIONER
        "   thr mgmt gettlvs - displays a list of all TLV names available for get/set\r\n"
        "   thr mgmt activeset <addr> <tlvname> <value>\r\n"
        "   thr mgmt activeget <addr> <tlvname>\r\n"
        "   thr mgmt pendset <addr> <tlvname> <value>\r\n"
        "   thr mgmt pendget <addr> <tlvname>\r\n"
        "   thr mgmt commset <addr> <tlvname> <value>\r\n"
        "   thr mgmt commget <addr> <tlvname>\r\n"
        "   thr mgmt query <addr> <tlvname> <value>\r\n"
        "   thr mgmt reenroll <addr> \r\n"
        "   thr mgmt leave <addr> \r\n"
#endif
        //"   thr remove  router <router's short address>\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    {
        "ifconfig", SHELL_CMD_MAX_ARGS, 0, SHELL_Ifconfig
#if SHELL_USE_HELP
        ,"IP Stack interfaces configuration",
        "IP Stack interfaces configuration\r\n"
        "   ifconfig - displays all interfaces and addresses configured on the device\r\n"
#if ND_ENABLED
        "   ifconfig ncache - displays the ND neighbors\r\n"
#endif
#endif /* SHELL_USE_HELP */
        "   ifconfig <interface ID> ip <IP address>", NULL
    },
#if IP_IP6_ROUTING_ENABLE
    {
        "route", SHELL_CMD_MAX_ARGS, 0, SHELL_Routes
#if SHELL_USE_HELP
        ,"IP static routes management",
        "IP static routes management\r\n"
        "   route add -p <dest ipv6 addr> -h <next hop IPv6 addr> -I <next hop interface> -len <prefix length> -m <route metric>\r\n"
        "   route remove -p <dest ipv6 address> -h <next hop>\r\n"
        "   route print\r\n"
        "   Valid interfaces: 6LoWPAN, eth, serialtun, usbEnet\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
 #endif

#if IP_IP6_FIREWALL_ENABLE
     {
         "firewall", SHELL_CMD_MAX_ARGS, 0, SHELL_IpFirewall
#if SHELL_USE_HELP
         ,"Manage IP Firewall rules",
         "Manage IP Firewall rules\r\n"
         "   firewall enable\r\n"
         "   firewall disable\r\n"
         "   firewall add -dst <IPv6Addr> -src <IPv6Addr> -srcport <UDPPort> -dstport <UDPPort> -lendst <bitsLen> -lensrc <bitsLen> -sec <secLevel> -I <IP Interface>\r\n"
         "   firewall remove <ruleNb>\r\n"
         "   firewall show\r\n"
         "   Valid interfaces: 6LoWPAN, eth, serialtun, usbEnet\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
         ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
     },
#endif
    {
        "mcastgroup", SHELL_CMD_MAX_ARGS, 0, SHELL_MulticastGroups
#if SHELL_USE_HELP
        ,"Multicast groups management",
        "Multicast groups management\r\n"
        "   mcastgroup show - displays all joined multicast groups\r\n"
        "   mcastgroup add <IP group address> <interface id> <timeout/permanent>- joins to a new multicast group\r\n"
        "   mcastgroup leave <IP group address> <interface id>- leaves a multicast group\r\n"
        "Warning! Do not remove addresses that may affect stack's behaviour!"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    {
        "ping", SHELL_CMD_MAX_ARGS, 0, SHELL_Ping
#if SHELL_USE_HELP
        ,"IP Stack ping tool",
        "IP Stack ping IPv4/IPv6 addresses\r\n"
        "   ping <ip address> -I <interface> -i <timeout> -c <count> -h <hoplimit> -s <size> -t <continuous ping> -S <source IP address>\r\n"
        "   Valid interfaces: 6LoWPAN, eth, serialtun, usbEnet\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#if DNS_ENABLED
    {
        "dnsrequest", SHELL_CMD_MAX_ARGS, 0, SHELL_SendDns
#if SHELL_USE_HELP
        ,"Send DNS request",
        "dnsrequest <name> [-4]"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif
    {
        "coap", SHELL_CMD_MAX_ARGS, 0, SHELL_CoapSend
#if SHELL_USE_HELP
    ,"Send CoAP message",
    "Send CoAP message\r\n"
    "   coap <reqtype: CON/NON> <reqcode (GET/POST/PUT/DELETE)> <IP addr dest> <URI path> <payload ASCII>\r\n"
#if COAP_OBSERVE_CLIENT
    "   coap start observe client <server's IP address> <resource>\r\n"
    "   coap stop observe client <server's IP address> <resource>\r\n"
#endif
#if COAP_OBSERVE_SERVER
    "   coap start observe server\r\n"
#endif
    "Example: coap CON POST 2001::1 /led on \r\n"
    "         coap CON POST 2001::1 /led off \r\n"
    "         coap CON POST 2001::1 /led toggle \r\n"
    "         coap CON POST 2001::1 /led flash\r\n"
    "         coap CON POST 2001::1 /led rgb r255 g255 b255\r\n"
    "         coap CON GET 2001::1 /temp\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#if SOCK_DEMO
    {
        "socket", SHELL_CMD_MAX_ARGS, 0, SHELL_Socket
#if SHELL_USE_HELP
        ,"IP Stack BSD Sockets commands",
        "IP Stack BSD Sockets commands\r\n"
        "   socket open <protocol> <local ip addr> <local port> -local address can be 0::0 and local port 0 for automatic selection\r\n"
        "   socket send <socket id> <payload>\r\n"
        "   socket close <socket id> \r\n"
        "   socket connect <socket id> <remote ip addr> <remote port>\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif
    {
        "reboot", SHELL_CMD_MAX_ARGS, 0, SHELL_Reboot
#if SHELL_USE_HELP
        , "MCU Reset",
        "MCU Reset"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#if gHybridApp_d
    {
        "break", SHELL_CMD_MAX_ARGS, 0, SHELL_BreakReq
#if SHELL_USE_HELP
        ,"Stop process Command",
        "Stop process Command\r\n"
        "   break\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif
    {
        "factoryreset", SHELL_CMD_MAX_ARGS, 0, SHELL_FactoryResetReq
#if SHELL_USE_HELP
        ,"FactoryReset Command",
        "FactoryReset Command\r\n"
        "   factoryreset\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#if UDP_ECHO_PROTOCOL
    {
        "echoudp", SHELL_CMD_MAX_ARGS, 0, SHELL_EchoUdp
#if SHELL_USE_HELP
        ,"Echo udp client",
        "Echo udp client\r\n"
        "   echoudp -s<size> -S<source address> -t<continuous request> -i<timeout> <target ip address>\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif /* UDP_ECHO_PROTOCOL */


#if MAC_FILTERING_ENABLED
    {
        "macfilter", SHELL_CMD_MAX_ARGS, 0, SHELL_Filter
#if SHELL_USE_HELP
        ,"MAC filtering commands",
        "MAC filtering commands\r\n"
        "   macfilter enable <reject|accept>\r\n"
        "   macfilter add <neighbor extended address> reject <0/1> lqi <neighbor link indicator>\r\n"
        "   macfilter remove <neighbor extended address>\r\n"
        "   macfilter disable \r\n"
        "   macfilter show \r\n"
        "   \r\n"
        "   Example: macfilter enable accept\r\n"
        "   macfilter add 0x1122334455667788 reject 1\r\n"
        "   Neighbor link indicator values:\r\n"
        "\tGood Link: 20 - 255 \r\n"
        "\tMedium Link: 11 - 20 \r\n"
        "\tBad Link: 3 - 10  \r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif /* MAC_FILTERING_ENABLED */
#ifdef USE_MSD_BOOTLOADER
    {
        "flasherase", SHELL_CMD_MAX_ARGS, 0, SHELL_FlashEraseWithMSDFunc
#if SHELL_USE_HELP
        ,"FlashErase Command",
        "FlashErase Command\r\n"
        "   flasherase\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */

    },
#endif /* USE_MSD_BOOTLOADER */
    {
        "identify", SHELL_CMD_MAX_ARGS, 0, SHELL_Identify
#if SHELL_USE_HELP
        ,"Identify Device",
        "Led flash to identify device\r\n"
        "   identify\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    #if GETIPv6ADDRESSES_APP
    {
        "getnodesip", 5, 0, SHELL_GetNeighborsIpAddr
#if SHELL_USE_HELP
    ,"Gets IPv6 addresses from all Thread nodes",
    "Gets IPv6 addresses from all Thread nodes\r\n"
    "   getnodesip \r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
    ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    #endif /* GETIPv6ADDRESSES_APP */
};

const icmpProtMsgTypeHandler_t gaShellProtMsgTypeHandlerTbl6[] =
{
    {PING_EchoReplyReceiveAsync, 129U, ICMP_CODE_DEFAULT}
};

const uint8_t gShellProtMsgTypeHandlerTbl6Size = sizeof(gaShellProtMsgTypeHandlerTbl6)/sizeof(icmpProtMsgTypeHandler_t);

const icmpProtMsgTypeHandler_t gaShellProtMsgTypeHandlerTbl4[] =
{
    {PING_EchoReplyReceiveAsync, 0U, ICMP_CODE_DEFAULT}
};

const uint8_t gShellProtMsgTypeHandlerTbl4Size = sizeof(gaShellProtMsgTypeHandlerTbl4)/sizeof(icmpProtMsgTypeHandler_t);

const aShellThrSetAttr_t aShellThrAttr[] = {
{"activetimestamp",         gNwkAttrId_MeshCop_ActiveTimestamp_c,  gDecimal_c,      GetSizeOfMember(thrActiveAttr_t, timestamp), SHELL_PrintActiveTimestamp, TRUE},
{"autostart",               gNwkAttrId_NvmRestoreAutoStart_c,      gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
{"bestchannelthreshold",    gNwkAttrId_SelectBestChEDThreshold_c,  gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
{"channel",                 gNwkAttrId_Channel_c,                  gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
#if !THREAD_ED_CONFIG
{"childaddrmask",           gNwkAttrId_ChildAddrMask_c,            gHex_c,          GetSizeOfMember(thrAttr_t, childAddrMask), NULL, FALSE},
#endif
{"childreqfullnd",          gNwkAttrId_ChildEDReqFullNwkData_c,    gDecimal_c,      sizeof(bool_t),  NULL, TRUE},
{"childtimeout",            gNwkAttrId_SEDTimeoutPeriod_c,         gDecimal_c,      sizeof(uint32_t), NULL, TRUE},
{"devicerole",              gNwkAttrId_DeviceRole_c,               gString_c,       sizeof(uint8_t), SHELL_PrintDevRole,
#if !THREAD_ED_CONFIG
    TRUE},
#else
    FALSE},
#endif
{"devicetype",              gNwkAttrId_DeviceType_c,               gDecimal_c,      sizeof(uint8_t), SHELL_PrintDevType, FALSE},
{"eui",                     gNwkAttrId_IeeeAddr_c,                 gHexReversed_c,  sizeof(uint64_t), NULL, FALSE},
{"hashaddr",                gNwkAttrId_HashIeeeAddr_c,             gHexReversed_c,  sizeof(uint64_t), NULL, FALSE},
{"iscommissioned",          gNwkAttrId_IsDevCommissioned_c,        gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
{"isconnected",             gNwkAttrId_IsDevConnected_c,           gDecimal_c,      sizeof(uint8_t), NULL, FALSE},
{"isfastpollenabled",       gNwkAttrId_IsFastPollEnabled_c,        gDecimal_c,      sizeof(bool_t), NULL, FALSE},
{"joinlqithreshold",        gNwkAttrId_JoinLqiThreshold_c,         gDecimal_c,      sizeof(uint8_t),NULL, TRUE},
{"keyrotationinterval",     gNwkAttrId_KeyRotationInterval_c,      gDecimal_c,      sizeof(uint32_t), NULL, TRUE},
{"keyswitchguard",          gNwkAttrId_KeySwitchGuardTime_c,       gDecimal_c,      sizeof(uint32_t), NULL, TRUE},
#if !THREAD_ED_CONFIG
{"leaderweight",            gNwkAttrId_LeaderWeight_c,             gDecimal_c,      sizeof(uint8_t),NULL, TRUE},
#endif
{"masterkey",               gNwkAttrId_NwkMasterKey_c,             gArray_c,        GetSizeOfMember(thrActiveAttr_t, nwkMasterKey), NULL, TRUE},
{"mlprefix",                gNwkAttrId_MLPrefix_c,                 gArray_c,        (THR_ML_PREFIX_LEN_BITS >> 3), NULL, TRUE},
{"modelname",               gNwkAttrId_ModelName_c,                gString_c,       sizeof(thrOctet16_t), NULL, FALSE},
{"neighbor",                (thrAttrId_t)0,                        gTable_c,        0, SHELL_PrintNeighborInfo, FALSE},
{"neighbors",               (thrAttrId_t)0,                        gTable_c,        0, SHELL_PrintNeighborTbl, FALSE},
{"nvm",                     gNwkAttrId_NvmRestore_c,               gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
{"nvmData",                 gNwkAttrId_NvmRestoreData_c,           gDecimal_c,      sizeof(uint8_t), NULL, FALSE},
{"nwkcapabilities",         gNwkAttrId_ThreadNwkCapabilites_c,     gHex_c,          sizeof(uint8_t), SHELL_PrintNwkCapabilities, TRUE},
{"nwkname",                 gNwkAttrId_NwkName_c,                  gString_c,       sizeof(thrOctet16_t), NULL, TRUE},
{"panid",                   gNwkAttrId_PanId_c,                    gHex_c,          sizeof(uint16_t), NULL, TRUE},
{"parent",                  (thrAttrId_t)0,                        gTable_c,        0, SHELL_PrintParentInfo, FALSE},
#if !THREAD_ED_CONFIG
{"parentholdtime",          gNwkAttrId_ParentHoldTime_c,           gDecimal_c,      sizeof(uint16_t), NULL, TRUE},
#endif
{"partitionid",             gNwkAttrId_PartitionId_c,              gHex_c,          sizeof(uint32_t), NULL, TRUE},
#if !THREAD_ED_CONFIG
{"permitjoin",              gNwkAttrId_PermitJoin_c,               gDecimal_c,      sizeof(uint8_t),NULL, FALSE},
#endif
{"provisioningurl",         gNwkAttrId_ProvisioningURL_c,          gString_c,       sizeof(thrOctet64_t), NULL, TRUE},
{"pskcpassword",            gNwkAttrId_PSKc_c,                     gString_c,       sizeof(thrOctet32_t), NULL, TRUE},
{"pskd",                    gNwkAttrId_PSKd_c,                     gString_c,       sizeof(thrOctet32_t), NULL, TRUE},
{"randomaddr",              gNwkAttrId_RandExtAddr_c,              gHexReversed_c,  sizeof(uint64_t), NULL, TRUE},
#if !THREAD_ED_CONFIG
{"routes",                  (thrAttrId_t)0,                        gTable_c,        0, SHELL_PrintRoutingTbl, FALSE},
#endif
{"rxonidle",                gNwkAttrId_RxOnIdle_c,                 gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
{"scanduration",            gNwkAttrId_ScanDuration_c,             gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
{"scanmask",                gNwkAttrId_ScanChannelMask_c,          gHex_c,          sizeof(uint32_t),NULL, TRUE},
{"sedfastpollinterval",     gNwkAttrId_SEDFastPollInterval_c,      gDecimal_c,      sizeof(uint32_t), NULL, TRUE},
{"sedpollinterval",         gNwkAttrId_SEDPollInterval_c,          gDecimal_c,      sizeof(uint32_t), NULL, TRUE},
{"shortaddr",               gNwkAttrId_ShortAddr_c,                gHex_c,          sizeof(uint16_t), NULL, FALSE},
{"slaacpolicy",             gNwkAttrId_SlaacPolicy_c,              gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
{"stackversion",            gNwkAttrId_StackVersion_c,             gString_c,       0, NULL, FALSE},
{"swversion",               gNwkAttrId_SWVersion_c,                gString_c,       sizeof(thrOctet16_t), NULL, FALSE},
{"uniqueaddr",              gNwkAttrId_UniqueExtAddr_c,            gDecimal_c,      sizeof(uint8_t), NULL, TRUE},
{"vendordata",              gNwkAttrId_VendorData_c,               gString_c,       sizeof(thrOctet64_t), NULL, FALSE},
{"vendorname",              gNwkAttrId_VendorName_c,               gString_c,       sizeof(thrOctet32_t), NULL, FALSE},
{"xpan",                    gNwkAttrId_ExtendedPanId_c,            gArray_c,        GetSizeOfMember(thrActiveAttr_t, xPanId), NULL, TRUE}
};

const char* mMacFilteringStatus[] =
{
   "Disabled\r\n",
   "Enabled - Default policy:Reject\r\n",
   "Enabled - Default policy:Accept\r\n",
};

const char* mThrDevRole[] =
{
    "Disconnected\r\n",
    "Sleepy End Device\r\n",
    "Minimal End Device\r\n",
    "Full End Device\r\n",
    "Router Eligible End Device\r\n",
    "Router\r\n",
    "Leader\r\n",
};

/* Interface table. User is advised to add the name of the interface according to the existing interfaces. */
const aShellThrInterface_t mThrInterfaces[] =
{
    {(uint8_t*)"6LoWPAN",      gIpIfSlp0_c},
    {(uint8_t*)"eth",          gIpIfEth0_c},
    {(uint8_t*)"serialtun",    gIpIfSerialTun_c},
    {(uint8_t*)"usbEnet",      gIpIfUsbRndis_c},
    {(uint8_t*)"6LoBLE",       gIpIfBle0_c},
    {(uint8_t*)"Loopback",     gIpIfLoopback_c},
};

/* Ping variables */
uint64_t pingTimeStamp = 0;
tmrTimerID_t pingTimerID = gTmrInvalidTimerID_c;

taskMsgQueue_t *pmMainThreadMsgQueue;   /*!< Pointer to main thread message queue */

ipAddr_t *pSrcIpAddr = NULL; /* Used for keeping ping source address */

uint32_t threadInstanceID = 0;

#if SHELL_DUT_COMMISSIONER
const aShellThrMgmtSetTlv_t aShellThrMgmtTlvs[] =
{
    {"ch", gMeshCopTlvChannel_c, sizeof(meshCopChannelTlv_t)- MLE_TLV_HEADER_SIZE},
    {"pan", gMeshCopTlvPanID_c, sizeof(meshCopNwkPanIdTlv_t) - MLE_TLV_HEADER_SIZE},
    {"xpan", gMeshCopTlvXpanID_c, sizeof(meshCopNwkXPanIdTlv_t) - MLE_TLV_HEADER_SIZE},
    {"nwkname", gMeshCopTlvNwkName_c, 16},
    {"pskc", gMeshCopTlvPskc_c, 16},
    {"masterkey", gMeshCopTlvNwkMasterKey_c, sizeof(meshCopNwkMasterKeyTlv_t) - MLE_TLV_HEADER_SIZE},
    {"mlprefix", gMeshCopTlvNwkMlUla_c, sizeof(meshCopNwkMlUlaTlv_t) - MLE_TLV_HEADER_SIZE},
    {"steering", gMeshCopTlvSteeringData_c, 16},
    {"brloc", gMeshCopTlvBorderRouterLoc_c, sizeof(meshCopBrLocTlv_t) - MLE_TLV_HEADER_SIZE},
    {"sid", gMeshCopTlvCommSessId_c, sizeof(meshCopCommSessIdTlv_t) - MLE_TLV_HEADER_SIZE},
    {"secpol", gMeshCopTlvSecPolicy_c, sizeof(meshCopSecurityPolicyTlv_t) - MLE_TLV_HEADER_SIZE},
    {"activets", gMeshCopTlvActiveTimestamp_c, sizeof(meshCopActiveTimestampTlv_t) - MLE_TLV_HEADER_SIZE},
    {"pendts", gMeshCopTlvPendingTimestamp_c, sizeof(meshCopActiveTimestampTlv_t) - MLE_TLV_HEADER_SIZE},
    {"delaytmr", gMeshCopTlvDelayTimer_c, sizeof(meshCopDelayTimerTlv_t) - MLE_TLV_HEADER_SIZE},
    {"chmsk", gMeshCopTlvChannelMask_c, sizeof(meshCopChannelMaskTlv_t) - MLE_TLV_HEADER_SIZE},
    {"scan", gMeshCopTlvScanDuration_c, sizeof(meshCopScanDurationTlv_t) - MLE_TLV_HEADER_SIZE},
    {"energylist", gMeshCopTlvEnergyList_c, 10},
    {"future", gMeshCopTlvFuture_c, 0},
};
#endif

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     void SHELLComm_Init(taskMsgQueue_t *pMsgQueue)
\brief  This function is used to initialize the SHELL commands module.

\param  [in]    pMsgQueue    Pointer to the message queue
***************************************************************************************************/
void SHELLComm_Init
(
    taskMsgQueue_t *pMsgQueue
)
{
    if(!mShellCommandsEnabled)
    {
        // TBD: check the pointer to media interface.
        pmMainThreadMsgQueue = pMsgQueue;

        /* Initialize Shell module */
        shell_init("$ ");

        pfShellProcessCommand =  SHELL_ProcessCb;

          /* Register functions */
        shell_register_function_array((cmd_tbl_t *)&aShellCommands[0], NumberOfElements(aShellCommands));
        mShellCommandsEnabled = TRUE;
    }
}

/*!*************************************************************************************************
\fn     void SHELL_PrintIpAddr(threadAddrTypes_t addrType)
\brief  This function is used for printing in shell terminal the IP addresses of a certain type.

\param  [in]    threadAddrTypes_t    the type of the IP addresses to be printed
***************************************************************************************************/
void SHELL_PrintIpAddr
(
    threadAddrTypes_t addrType
)
{
    uint32_t iIpIdx;
    int32_t ifIndex;
    char addrStr[INET6_ADDRSTRLEN];
    ip6IfAddrData_t *pIp6AddrData = NULL;
#if IP_IP4_ENABLE
    ip4IfAddrData_t *pIp4AddrData = NULL;
#endif
    ipIfUniqueId_t ifId;

    /* IP Interfaces */
    for(ifId = gIpIfSlp0_c; ifId <= gIpIfLoopback_c; ifId++)
    {
        ifIndex = IP_IF_GetIfIndex(ifId);
        if(ifIndex != -1)
        {
            uint8_t *ifName = SHELL_SearchInterfaceById(ifId);

            shell_printf("\n\rInterface %d: %s", ifIndex, ifName);
#if IP_IP4_ENABLE
            for(iIpIdx = 0; iIpIdx < IP_IF_IP4_ADDR_NB; iIpIdx++)
            {
                /* Get IPv4 addresses for an interface */
                pIp4AddrData = IP_IF_GetAddrByIf4(ifId, iIpIdx);

                if(pIp4AddrData && (pIp4AddrData->ipIfId != gIpIfUndef_c))
                {
                    ipAddr_t inIpAddr;

                    NWKU_ConvertIp4Addr(pIp4AddrData->ip4Addr, &inIpAddr);
                    ntop(AF_INET, &inIpAddr, addrStr, INET_ADDRSTRLEN);
                    shell_printf("\n\r\tIPv4 Address: %s", addrStr);
                }
            }
#endif

            /* Get IPv6 addresses for an interface */
            for(iIpIdx = 0; iIpIdx < IP_IF_IP6_ADDR_NB; iIpIdx++)
            {
                /* Get IP address by index */
                /* Do not print anycast addresses */
                pIp6AddrData = IP_IF_GetAddrByIf6(ifId, iIpIdx, FALSE);

                if(pIp6AddrData && (pIp6AddrData->ipIfId != gIpIfUndef_c))
                {
                    ntop(AF_INET6, &pIp6AddrData->ip6Addr, addrStr, INET6_ADDRSTRLEN);

                    if(((addrType == gMeshLocalAddr_c)||(addrType == gAllIpAddr_c)) &&
                        (IP6_IsUniqueLocalAddr(&pIp6AddrData->ip6Addr)))
                    {
                        thrPrefixAttr_t thrMLPrefix;

                        (void)THR_GetAttr(threadInstanceID, gNwkAttrId_MLPrefix_c, 0, NULL, (uint8_t*)&(thrMLPrefix));

                        if(FLib_MemCmp(&pIp6AddrData->ip6Addr, &thrMLPrefix.prefix, 8))
                        {
                            shell_printf("\n\r\tMesh local address");

                            if(IP6_IsAddrEui64(&pIp6AddrData->ip6Addr))
                            {
                                shell_printf(" (ML64): %s", addrStr);
                            }
                            else
                            {
                                shell_printf(" (ML16): %s", addrStr);
                            }
                        }
                        else
                        {
                            shell_printf("\n\r\tUnique local address: %s", addrStr);
                        }
                    }

                    if(((addrType == gGlobalAddr_c) ||(addrType == gAllIpAddr_c)) &&
                        (IP6_IsGlobalAddr(&pIp6AddrData->ip6Addr)))
                    {
                        shell_printf("\n\r\tGlobal address: %s", addrStr);
                    }

                    if(((addrType == gLinkLocalAddr_c) || (addrType == gAllIpAddr_c)) &&
                        (IP6_IsLinkLocalAddr(&pIp6AddrData->ip6Addr)))
                    {
                        if(IP6_IsAddrEui64(&pIp6AddrData->ip6Addr))
                        {
                            shell_printf("\n\r\tLink local address (LL64): %s", addrStr);
                        }
                        else
                        {
                            shell_printf("\n\r\tLink local address (LL16): %s", addrStr);
                        }
                    }
                }
            }

            /* Link-local all-Thread-nodes, Realm-Local All Thread Nodes */
            if(addrType == gAllIpAddr_c)
            {
                /* Link-local all-Thread-nodes */
                if(IP_IF_IsMyMulticastGroupAddr6(ifId, (ipAddr_t *)&in6addr_linklocal_allthreadnodes))
                {
                    ntop(AF_INET6, &in6addr_linklocal_allthreadnodes, addrStr, INET6_ADDRSTRLEN);
                    shell_printf("\n\r\tLink local all Thread Nodes(MCast):  %s", addrStr);
                }
                /* Realm-Local All Thread Nodes */
                if(IP_IF_IsMyMulticastGroupAddr6(ifId, (ipAddr_t *)&in6addr_realmlocal_allthreadnodes))
                {
                    ntop(AF_INET6, &in6addr_realmlocal_allthreadnodes, addrStr, INET6_ADDRSTRLEN);
                    shell_printf("\n\r\tRealm local all Thread Nodes(MCast): %s", addrStr);
                }
            }
        }
    }
}

#if THR_ENABLE_EVENT_MONITORING
/*!*************************************************************************************************
\fn     bool_t SHELL_EvMonitorInit(instanceId_t instanceId)
\brief  This function is used to initialize the Shell event monitor.

\param  [in]   instanceId    Instance ID
***************************************************************************************************/
void SHELL_EvMonitorInit
(
    instanceId_t instanceId
)
{
    /* Init demo application  */
    shell_write("\n\rSHELL starting...\n");
    shell_printf("\r%s\n", *(IP_version.moduleString));
    shell_write("\rEnter \"thr join\" to join a network, or \"thr create\" to start new network\n");
    shell_write("\rEnter \"help\" for other commands\n");
    shell_refresh();
}

/*!*************************************************************************************************
\fn     bool_t SHELL_EvMonitor(void *param)
\brief  This function is used to handle the events received from the Event monitor.

\param  [in]   param    Pointer to monitor event
***************************************************************************************************/
void SHELL_EvMonitor
(
    void *param
)
{
    eventMonitorData_t *pEventMonitorData = (eventMonitorData_t *)param;

    if(pEventMonitorData)
    {
        uint16_t attrPanId = THR_GetAttr_PanId(pEventMonitorData->instanceId);
        uint8_t attrCapabilities = THR_GetAttr_NwkCapabilities(pEventMonitorData->instanceId);
        bool_t printMeshLocal = FALSE;
#if !THREAD_ED_CONFIG
        uint8_t attrChannel = THR_GetAttr_Channel(pEventMonitorData->instanceId);
#endif
        uint32_t eventCode = ((pEventMonitorData->eventSet[0]<<16) + (pEventMonitorData->eventSet[1]<<24) +
                              (pEventMonitorData->eventStatus[0]) + (pEventMonitorData->eventStatus[1]<<8));
        switch(eventCode)
        {
            case gThrEv_NwkJoinInd_Attaching_c:
                if(APP_GetState(threadInstanceID) != gDeviceState_JoiningOrAttaching_c)
                {
                    shell_write("\rJoining a Thread network...\n\r");
                }
                break;
            case gThrEv_NwkJoinCnf_Success_c:
                shell_printf("\rAttached to network with PAN ID: 0x%x \n\r", attrPanId);
                /* Check if the node is polling ED or ED */
                if(attrCapabilities & 0x04)
                {
                    shell_write("\rNode started as End Device\n\r");
                    printMeshLocal = TRUE;
                }
                /* Check if REED or ED */
                if((attrCapabilities == 0) || ((attrCapabilities & THR_NWKCAP_CAN_BECOME_ACTIVE_ROUTER)
#if THR_ROUTING_ENABLE
                   && (THR_GetRouterUpgradeThreshold() == 1)
#endif
                  ))
                {
                    printMeshLocal = TRUE;
                }
                break;
            case gThrEv_NwkJoinCnf_Failed_c:
            case gThrEv_MeshCop_JoinerDtlsError_c:
            case gThrEv_MeshCop_JoinerError_c:
                shell_write("\rJoin failed\n\r");
                shell_refresh();
                break;
            case gThrEv_GeneralInd_Disconnected_c:
                shell_write("\rDisconnected\n\r");
                break;
            case gThrEv_GeneralInd_RequestGlobalAddr_c:
                shell_write("\rRequesting Global Address...\n\r");
                break;
            case gThrEv_GeneralInd_GlobalAddrAssigned_c:
                shell_write("\rGlobal Address has been assigned");
                shell_refresh();
                break;
            case gThrEv_GeneralInd_ResetMcuTimeout_c:
                shell_printf("\rReset MCU in: %d milliseconds\n", APP_GetResetMcuTimeout());
                break;
#if !THREAD_ED_CONFIG
            case gThrEv_GeneralInd_DeviceIsLeader_c:
                shell_write("\rNode has taken the Leader role");
                shell_refresh();
                break;
            case gThrEv_NwkCreateCnf_Success_c:
                shell_printf("\rCreated a new Thread network on channel %d and PAN ID:0x%x\n\r", attrChannel, attrPanId);
                printMeshLocal = TRUE;
                break;
            case gThrEv_GeneralInd_RequestRouterId_c:
                shell_write("\rRequesting to become Active Router...\n\r");
                break;
            case gThrEv_GeneralInd_RouterIdAssigned_c:
                shell_write("\rSuccess\n\r");
                printMeshLocal = TRUE;
                break;
            case gThrEv_MeshCop_CommissionerPetitionAccepted_c:
                shell_write("\r(Local) Commissioner Started\n\r");
                shell_refresh();
                break;
            case gThrEv_MeshCop_CommissionerJoinerAccepted_c:
                shell_write("\rJoiner accepted\n\r");
                shell_refresh();
                break;
            case gThrEv_MeshCop_JoinerAccepted_c:
                shell_write("\rCommissioning successful\n\r");
                shell_refresh();
                break;
            case  gThrEv_GeneralInd_ChildIdAssigned_c:
                break;
#endif
            default:
                break;
        }

        if(printMeshLocal)
        {
            SHELL_PrintIpAddr(gMeshLocalAddr_c);
            shell_refresh();
        }

        MEM_BufferFree(pEventMonitorData);
    }
}
#endif /* THR_ENABLE_EVENT_MONITORING */

/*!*************************************************************************************************
\fn     void SHELL_NwkScanPrint(thrNwkScanResults_t *pScanResults)
\brief  This function is used to network scan information.

\param  [in]    pScanResults   The scan results
***************************************************************************************************/
void SHELL_NwkScanPrint
(
    thrNwkScanResults_t *pScanResults
)
{
   /* Handle the network scan result here */
   if(pScanResults)
   {
       if((pScanResults->numOfEnergyDetectEntries == 0) &&
          (pScanResults->numOfNwkScanEntries == 0) )
       {
           shell_write("\n\rNo scan results\n\r");
           return;
       }

       if(pScanResults->pEnergyDetectList)
       {
           shell_write("\n\rEnergy on channel(s): 0x");
           SHELL_PrintBuff(pScanResults->pEnergyDetectList, pScanResults->numOfEnergyDetectEntries);
           shell_printf("\n\r");
       }

       if(pScanResults->numOfNwkScanEntries)
       {
           thrNwkActiveScanEntry_t *pNwkDiscovery;
           uint8_t i;

           for(i = 0; i < pScanResults->numOfNwkScanEntries; i++)
           {
               pNwkDiscovery = &pScanResults->nwkScanList[i];

               if(pNwkDiscovery)
               {
                   shell_printf("\rThread Network: %d\n\r", i);
                   shell_printf("\r\tPAN ID: 0x%x \n\r", pNwkDiscovery->panid);
                   shell_printf("\r\tChannel: %d \n\r", pNwkDiscovery->channel);
                   shell_printf("\r\tLQI: %d\n\r", pNwkDiscovery->lqi);
                   shell_printf("\r\tReceived beacons: %d\n\r", pNwkDiscovery->numOfRcvdBeacons);
               }
           }
       }
       shell_refresh();
   }
}

/*!*************************************************************************************************
\fn     void PING_EchoReplyReceiveAsync(ipPktInfo_t *pIpPktInfo)
\brief  Interface function for the user application. It handles a received Ping Echo Reply message.

\param  [in]    pIpPktInfo    Pointer to the packet information structure.
***************************************************************************************************/
void PING_EchoReplyReceiveAsync
(
    ipPktInfo_t *pIpPktInfo
)
{
    if(FALSE == NWKU_SendMsg(PING_EchoReplyReceive, (void *)pIpPktInfo, pmMainThreadMsgQueue))
    {
        NWKU_FreeIpPktInfo(&pIpPktInfo);
    }
}

/*!*************************************************************************************************
\private
\fn     void SHELL_DnsNdDataService(coapSessionStatus_t sessionStatus, uint8_t *pData,
                                           coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for the ND_DATA.req message.

\param  [in]    sessionStatus    Status of the session
\param  [in]    pData            Pointer to data
\param  [in]    pSession         Pointer to CoAP session
\param  [in]    dataLen          Length of data
***************************************************************************************************/
void SHELL_DnsNdDataService
(
    coapSessionStatus_t sessionStatus,
    uint8_t *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
#if DNS_ENABLED
    uint8_t *pCursor = pData;
    uint8_t iCount, noServers, noDomainSrvBytes, domainSrvLen;
    char serverAddrStr[INET6_ADDRSTRLEN];

    if(dataLen > 0 && pCursor != NULL)
    {
        /* check type */
        if(*pCursor != 9)  // THR_NWK_ND_DATA_TLV_TYPE
        {
            shell_write("Did not receive a valid ND_DATA.rsp!\r\n");
            return;
        }
        pCursor += MLE_TLV_HEADER_SIZE;
        dataLen -= MLE_TLV_HEADER_SIZE;

        while(dataLen > 0)
        {
            switch(*pCursor)
            {
                case gNdOptionRDNSS_c:
                    shell_write("Recursive DNS Servers:\r\n");
                    noServers = (*(++pCursor) - 1) >> 1;
                    dataLen -= 1;

                    pCursor += 7;  // skip Length(1), Reserved(2) and Lifetime(4)
                    dataLen -= 7;

                    for(iCount = 0; iCount < noServers; iCount++)
                    {
                        /* Create aligned IP Prefix */
                        ipAddr_t ipAddr;
                        FLib_MemCpy(&ipAddr.addr8[0], pCursor, sizeof(ipAddr_t));

                        shell_write("\t");
                        shell_write(ntop(AF_INET6, &ipAddr, serverAddrStr, INET6_ADDRSTRLEN));
                        shell_write("\r\n");
                        pCursor += sizeof(ipAddr_t);
                        dataLen -= sizeof(ipAddr_t);
                    }

                    break;

                case gNdOptionDNSSL_c:
                    shell_write("DNS Search Lists:\r\n");
                    noDomainSrvBytes = (*(++pCursor) - 1) << 3;
                    dataLen -= 1;

                    pCursor += 7;  // skip Length(1), Reserved(2) and Lifetime(4)
                    dataLen -= 7;

                    while(*pCursor != '\0')
                    {
                        shell_write("\t");
                        shell_write((char *)pCursor);
                        shell_write("\r\n");
                        domainSrvLen = FLib_StrLen((char *)pCursor) + 1;  // +1 to include '\0'
                        pCursor += domainSrvLen;
                        dataLen -= domainSrvLen;
                        noDomainSrvBytes -= domainSrvLen;
                    }
                    pCursor += noDomainSrvBytes;
                    dataLen -= noDomainSrvBytes;  // jump over extra padding bytes

                    break;
            }
        }
    }
    else
    {
        shell_write("There is no BR able to supply DNS information!\r\n");
    }

    if (pSession)
    {
        COAP_CloseSession(pSession);
    }

    SHELL_Resume();
#endif /* DNS_ENABLED */
}

/*!*************************************************************************************************
\fn     void SHELL_DnsService(uint8_t *pMsg)
\brief  This function is the callback function for the DNS response message.

\param  [in]    pMsg    Pointer to message
***************************************************************************************************/
void SHELL_DnsService
(
    uint8_t *pMsg
)
{
#if DNS_ENABLED
    uint8_t af;
    hostent_t *pDnsResponse = (hostent_t *)pMsg;
    ipAddr_t *pIpAddr = NULL;
    char respAddrStr[INET6_ADDRSTRLEN];

    if (pMsg != NULL)
    {
        shell_printf("%s is at ", pDnsResponse->pHostName);

        do
        {
            pIpAddr = ListGetHeadMsg(pDnsResponse->pHostAddrList);

            if (pIpAddr)
            {
                af = IP_IsAddrIPv4(pIpAddr) ? AF_INET : AF_INET6;
                ntop(af, pIpAddr, respAddrStr, INET6_ADDRSTRLEN);
                shell_printf("%s ", respAddrStr);

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
    else
    {
        shell_write("No response received!\n");
    }

    SHELL_Resume();
#endif /* DNS_ENABLED */
}

#if SHELL_DUT_COMMISSIONER
/*!*************************************************************************************************
\fn     SHELL_MeshcopMgmtPanIdConflictCb
\brief  This function is used to handle the response of Management Pan Id Query command.

\param  [in]    pIdHandlerEntry pointer to meshcop handlers
\param  [in]    pTlvs           pointer to the received tlvs
\param  [in]    tlvsLen         size of the received tlvs

\return         void
***************************************************************************************************/
void SHELL_MeshcopMgmtPanIdConflictCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    shellMgmtCb_t* pMgmtCb = MEM_BufferAlloc(sizeof(shellMgmtCb_t) + tlvsLen);

    if (pMgmtCb != NULL)
    {
        pMgmtCb->tlvsLen = tlvsLen;
        FLib_MemCpy(&pMgmtCb->pTlvs, pTlvs, tlvsLen);
        if (!NWKU_SendMsg(SHELL_MgmtPanIdConflictCbHandler, pMgmtCb, pmMainThreadMsgQueue))
        {
            MEM_BufferFree(pMgmtCb);
        }
    }
}

/*!*************************************************************************************************
\fn     SHELL_MeshcopMgmtSetCb
\brief  This function is used to handle the response of Management Set command.

\param  [in]    pIdHandlerEntry pointer to meshcop handlers
\param  [in]    pTlvs           pointer to the received tlvs
\param  [in]    tlvsLen         size of the received tlvs

\return         void
***************************************************************************************************/
void SHELL_MeshcopMgmtSetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    shellMgmtCb_t* pMgmtCb = MEM_BufferAlloc(sizeof(shellMgmtCb_t) + tlvsLen);

    if (pMgmtCb != NULL)
    {
        pMgmtCb->tlvsLen = tlvsLen;
        FLib_MemCpy(&pMgmtCb->pTlvs, pTlvs, tlvsLen);
        if (!NWKU_SendMsg(SHELL_MgmtSetCbHandler, pMgmtCb, pmMainThreadMsgQueue))
        {
            MEM_BufferFree(pMgmtCb);
        }
    }
};

/*!*************************************************************************************************
\fn     SHELL_MeshcopMgmtGetCb
\brief  This function is used to handle the response of Management Get command.

\param  [in]    pIdHandlerEntry pointer to meshcop handlers
\param  [in]    pTlvs           pointer to the received tlvs
\param  [in]    tlvsLen         size of the received tlvs

\return         void
***************************************************************************************************/
void SHELL_MeshcopMgmtGetCb
(
    meshcopHandlers_t *pIdHandlerEntry,
    uint8_t *pTlvs,
    uint32_t tlvsLen
)
{
    shellMgmtCb_t* pMgmtCb = MEM_BufferAlloc(sizeof(shellMgmtCb_t) + tlvsLen);

    if (pMgmtCb != NULL)
    {
        pMgmtCb->tlvsLen = tlvsLen;
        FLib_MemCpy(&pMgmtCb->pTlvs, pTlvs, tlvsLen);
        if (!NWKU_SendMsg(SHELL_MgmtGetCbHandler, pMgmtCb, pmMainThreadMsgQueue))
        {
            MEM_BufferFree(pMgmtCb);
        }
    }
};
#endif /* SHELL_DUT_COMMISSIONER */

/*==================================================================================================
Private functions
==================================================================================================*/

/*!*************************************************************************************************
\private
\fn     static void SHELL_Resume(void)
\brief  This function is used to resume SHELL execution after a pause was issued.
***************************************************************************************************/
static void SHELL_Resume
(
    void
)
{
    /* Stop Ping timer */
    if(pingTimerID != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(pingTimerID);
        pingTimerID = gTmrInvalidTimerID_c;
    }
    mContinuousPing = FALSE;

    if(mTimerIDSelect != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(mTimerIDSelect);
        mTimerIDSelect = gTmrInvalidTimerID_c;
    }

    if(mDelayTimerID != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(mDelayTimerID);
        mDelayTimerID = gTmrInvalidTimerID_c;
    }

    if(pSrcIpAddr)
    {
        MEM_BufferFree(pSrcIpAddr);
        pSrcIpAddr = NULL;
    }

    shell_refresh();
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_Reboot(uint8_t argc, char *argv[])
\brief  This function is used to reset the device.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_Reboot
(
    uint8_t argc,
    char *argv[]
)
{
    ResetMCU();

    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\fn     static void SHELL_Identify(void* param)
\brief  This function is used to start the board identification by flashing the TX LED.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_Identify
(
    uint8_t argc,
    char *argv[]
)
{
    mShellBoardIdentify = TRUE;
    Led_Identify(mShellBoardIdentify);

    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_Process(uint8_t *param)
\brief  This function is used to process the input command.

\param  [in]   param    Pointer to input
***************************************************************************************************/
static void SHELL_Process
(
    uint8_t *param
)
{
    extern int8_t (*mpfShellBreak)(uint8_t argc, char *argv[]);
    uint8_t argc;
    char *argv[SHELL_MAX_ARGS + 1] = {0};    /* NULL terminated  */
    cmd_tbl_t *cmdtp;
    int16_t ret = CMD_RET_FAILURE;

    // Split command into arguments
    argc = make_argv((char *)param, SHELL_MAX_ARGS + 1, argv);
    if(argc >= SHELL_MAX_ARGS)
    {
        shell_write("** Too many args (max. ");
        shell_writeDec(SHELL_MAX_ARGS);
        shell_write (") **\r\n");
    }
    // Search for the appropriate command
    cmdtp = shell_find_command(argv[0]);
    if((cmdtp != NULL) && (cmdtp->cmd != NULL))
    {
        if(argc > cmdtp->maxargs)
        {
            ret = CMD_RET_USAGE;
        }
        else
        {
            ret = (cmdtp->cmd)(argc, argv);
        }
    }
    else
    {
        shell_write("Unknown command '");
        shell_write(argv[0]);
#if SHELL_USE_HELP
        shell_write("' - try 'help'\r\n");
#else
        shell_write("' ");
#endif
        ret = CMD_RET_FAILURE;
    }
#if SHELL_USE_HELP
    if(ret == CMD_RET_USAGE)
    {
        if(cmdtp->usage != NULL)
        {
            shell_write(cmdtp->name);
            shell_writeN(" - ", 3);
            shell_write(cmdtp->usage);
            SHELL_NEWLINE();
        }

        if(cmdtp->help != NULL)
        {
            shell_write("Usage:\r\n");
            shell_write(cmdtp->name);
            shell_writeN(" ", 1);
            shell_write(cmdtp->help);
            SHELL_NEWLINE();
        }
        else
        {
            shell_write("- No additional help available.\r\n");
        }
    }
#endif
    if( ret == CMD_RET_ASYNC )
    {
        mpfShellBreak = cmdtp->cmd;
    }
    else
    {
        mpfShellBreak = NULL;
        shell_refresh();
    }
    MEM_BufferFree(param);
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_ProcessCb(char *pCmd, uint16_t length)
\brief  This function is used as a callback function for inputting commands.

\param  [in]    pCmd      Pointer to the command
\param  [in]    length    Length of the command
***************************************************************************************************/
static void SHELL_ProcessCb
(
    char *pCmd,
    uint16_t length
)
{
    if((NULL == pCmd) && (0 == length))
    {
        SHELL_InterruptCb();
    }
    else
    {
        uint8_t *pData = MEM_BufferAlloc(length + 1);

        if(NULL != pData)
        {
            FLib_MemSet(pData, 0, length + 1);
            FLib_MemCpy(pData, pCmd, length);

            if (FALSE == NWKU_SendMsg(SHELL_Process, pData, pmMainThreadMsgQueue))
            {
                MEM_BufferFree(pData);
            }
        }
    }
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_InterruptCb(void)
\brief  This function is used to intrerupt the ping or the board identification on user request.
***************************************************************************************************/
static void SHELL_InterruptCb
(
    void
)
{
    if(TRUE == mShellBoardIdentify)
    {
        mShellBoardIdentify = FALSE;
        Led_Identify(mShellBoardIdentify);
    }
    else
    {
        gbRetryInterrupt = FALSE;
        mContinuousPing = FALSE;
#if ECHO_PROTOCOL
        mEchoUdpCounter = 0;
#endif
    }
}

/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_Ifconfig(uint8_t argc, char *argv[])
\brief  This function is used for ifconfig.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_Ifconfig
(
    uint8_t argc,
    char *argv[]
)
{
#if IP_IP6_ENABLE
    int32_t interfaceId;
    ifHandle_t ifHandle;

    /* ifconfig all */
    if(argc == 1)
    {
        SHELL_PrintIpAddr(gAllIpAddr_c);
    }
#if ND_ENABLED
    /* ifconfig ncache */
    else if((argc == 2) && !strcmp(argv[1], "ncache"))
    {
        uint32_t iIf = 0;
        ndNeighborEntry_t *pNdNeighbor = NULL;
        char addrStr[INET6_ADDRSTRLEN];
        uint8_t *ifName = NULL;

        shell_write("ND Neighbor Cache:\n\r");
        /* Get interfaces by index */
        ifHandle = IP_IF_GetIfByIndex(iIf);

        while(ifHandle)
        {
            ifName = SHELL_SearchInterfaceById(ifHandle->ifUniqueId);
            shell_printf("\n\r\tInterface %d: %s\n\r", iIf, ifName);

            for(uint32_t iCount = 0U; iCount < ND_NEIGHBOR_CACHE_SIZE; iCount++)
            {
                pNdNeighbor = ND_NeighborCacheGetByIdx(ifHandle->ifUniqueId, iCount);

                if(pNdNeighbor)
                {
                    ntop(AF_INET6, &pNdNeighbor->ipAddr, addrStr, INET6_ADDRSTRLEN);
                    shell_printf("\t%s\n\r", addrStr);
                }
            }
            /* Go to the next interface */
            iIf++;
            ifHandle = IP_IF_GetIfByIndex(iIf);
        }
    }
#endif /* ND_ENABLED */

    /* ifconfig <interface ID> ip <IP address> */
    else if((argc == 4) && !strcmp(argv[2], "ip"))
    {
        ipAddr_t inIpAddr;
        uint32_t bindStatus = gIpOk_c;

        interfaceId = NWKU_atoi(argv[1]);

        /* Check IP address */
        if(pton(AF_INET6, argv[3], &inIpAddr) != 1)
        {
            shell_write("Malformed IP address");
        }
        else
        {
            ifHandle = IP_IF_GetIfByIndex(interfaceId);
            bindStatus = IP_IF_BindAddr6(ifHandle->ifUniqueId, &inIpAddr,ip6AddrTypeManual_c,
                                         IP6_ADDRESS_LIFETIME_INFINITE, 64U);
            switch(bindStatus)
            {
                case gIpOk_c:
                    shell_write("interface configured");
                    break;

                case gIpNoAddressSpaceError_c:
                    shell_write("address limit reached");
                    break;

                default:
                    shell_write("error");
                    break;
            }
        }
    }
    else
    {
        shell_write("Unknown command\n\r");
    }
#endif

    return CMD_RET_SUCCESS;
}

#if IP_IP6_ROUTING_ENABLE
/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_Routes(uint8_t argc, char *argv[])
\brief  This function is used for managing IPv6 routes.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_Routes(uint8_t argc, char *argv[])
{
    /*route add -p <dest ip address> -I <interface> -len <prefix length> -m <route metric> -h <next hop>*/

    char *pValue = NULL;
    ipAddr_t prefix = {0};
    ipAddr_t nextHop = {0};
    uint8_t prefixLength = 64;
    uint8_t routeMetric = 0;
    ipIfUniqueId_t interfaceId = gIpIfUndef_c;

    if (!strcmp(argv[1], "print"))
    {
        SHELL_PrintIPv6Routes(NULL);
    }
    else
    {
        /* Get destination IP address */
        SHELL_ReadIPv6Addr(argc, argv, "-p", &prefix);

        /* Get next hop */
        SHELL_ReadIPv6Addr(argc, argv, "-h", &nextHop);

        if (!strcmp(argv[1], "add"))
        {
            /* Get interface */
            pValue = shell_get_opt(argc, argv, "-I");
            if(pValue)
            {
                interfaceId = SHELL_SearchInterfaceByName((uint8_t *)pValue, strlen(pValue));
            }
            if(interfaceId == gIpIfUndef_c)
            {
                 shell_write("Interface doesn't exist or hasn't been started!");
                 return CMD_RET_SUCCESS;
            }

            /* Get prefix length */
            pValue = shell_get_opt(argc, argv, "-len");
            if(pValue)
            {
                prefixLength = (uint8_t)NWKU_atoi(pValue);
            }

            /* Get route metric */
            pValue = shell_get_opt(argc, argv, "-m");
            if(pValue)
            {
                routeMetric = (uint8_t)NWKU_atoi(pValue);
            }

            IP6_SetStaticRoute(&prefix, prefixLength, routeMetric, &nextHop, interfaceId);
        }
        else if (!strcmp(argv[1], "remove"))
        {
            IP6_RemoveRoute(&prefix, &nextHop);
        }
        SHELL_PrintStatus(gThrStatus_Success_c);
    }

    return CMD_RET_SUCCESS;
}
/*!*************************************************************************************************
\private
\fn     void SHELL_PrintIPv6Routes(void *param)
\brief  This function is used to print IPv6 routes.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintIPv6Routes
(
    void *param
)
{
    int32_t freeSpaces = 0;
    char addrString[INET6_ADDRSTRLEN];
    uint32_t routingTblSize = IP6_GetRoutingTableSize();
    ip6RoutingTblEntry_t *pRoute = NULL;

    shell_printf("Destination                                   Next Hop                                      NextHopIf PrefixLen Metric\n\r");
    for(uint32_t i = 0; i < routingTblSize; i++)
    {
        pRoute = IP6_GetRoute(i);
        if ((NULL != pRoute) && (1 == pRoute->validRoute))
        {
            ntop(AF_INET6, &pRoute->dstPrefix, addrString, INET6_ADDRSTRLEN);
            shell_write(addrString);
            /* Align IPv6 address strings to their maximum  length */
            freeSpaces = INET6_ADDRSTRLEN - strlen(addrString);
            while (freeSpaces)
            {
                shell_write(" ");
                freeSpaces--;
            }

            ntop(AF_INET6, &pRoute->nextHopRouter, addrString, INET6_ADDRSTRLEN);
            shell_write(addrString);
            freeSpaces = INET6_ADDRSTRLEN - strlen(addrString);
            while (freeSpaces)
            {
                shell_write(" ");
                freeSpaces--;
            }

            uint8_t * ifName =
            SHELL_SearchInterfaceById(pRoute->nextHopIfId);
            shell_write((char *)ifName);
            shell_write("\t");

            shell_printf("%d\t", pRoute->prefixLen);
            shell_printf("  %d", pRoute->routeMetric);
            shell_printf("\r\n");
        }
    }
}
#endif

#if IP_IP6_FIREWALL_ENABLE
/*!*************************************************************************************************
\private
\fn     int8_t SHELL_IpFirewall(uint8_t argc, char *argv[])
\brief  This function is used to manage IP Firewall rules.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_IpFirewall(uint8_t argc, char *argv[])
{
    char *pValue = NULL;
    thrStatus_t status = gThrStatus_Success_c;

    if (!strcmp(argv[1], "enable"))
    {
        IP6_FirewallEnabled(TRUE);
        SHELL_PrintStatus(gThrStatus_Success_c);
    }
    else if (!strcmp(argv[1], "disable"))
    {
        IP6_FirewallEnabled(FALSE);
        SHELL_PrintStatus(gThrStatus_Success_c);
    }
    else if (!strcmp(argv[1], "show"))
    {
        SHELL_PrintFirewallRules(NULL);
    }
    else if (!strcmp(argv[1], "add"))
    {
        ip6FirewallTblEntry_t ip6FirewallRule = {0};

        SHELL_ReadIPv6Addr(argc, argv, "-dst", (ipAddr_t*)&ip6FirewallRule.dstPrefix);
        SHELL_ReadIPv6Addr(argc, argv, "-src", (ipAddr_t*)&ip6FirewallRule.srcPrefix);

        /* Get prefix length */
        pValue = shell_get_opt(argc, argv, "-lendst");
        if(pValue)
        {
            ip6FirewallRule.dstPrefixLenBits = (uint8_t)NWKU_atoi(pValue);
        }
        pValue = shell_get_opt(argc, argv, "-lensrc");
        if(pValue)
        {
            ip6FirewallRule.srcPrefixLenBits = (uint8_t)NWKU_atoi(pValue);
        }

        /* Get interface */
        pValue = shell_get_opt(argc, argv, "-I");
        if(pValue)
        {
            ip6FirewallRule.ifUniqueId = SHELL_SearchInterfaceByName((uint8_t *)pValue, strlen(pValue));
        }
        if(ip6FirewallRule.ifUniqueId == gIpIfUndef_c)
        {
             shell_write("Interface doesn't exist or hasn't been started!");
             return CMD_RET_SUCCESS;
        }

        /* Get destination port */
        pValue = shell_get_opt(argc, argv, "-portdst");
        if(pValue)
        {
            ip6FirewallRule.dstPort = (uint16_t)NWKU_atoi(pValue);
        }

        /* Get source port */
        pValue = shell_get_opt(argc, argv, "-portsrc");
        if(pValue)
        {
            ip6FirewallRule.srcPort = (uint16_t)NWKU_atoi(pValue);
        }

        /* Get security level */
        pValue = shell_get_opt(argc, argv, "-sec");
        if(pValue)
        {
            ip6FirewallRule.secLevel = (int8_t)NWKU_atoi(pValue);
        }

        ip6FirewallRule.rule = TRUE;
        ip6FirewallRule.enable = TRUE;
        if (gIpOk_c != IP6_FirewallSetRule(&ip6FirewallRule))
        {
            status = gThrStatus_Failed_c;
        }
        SHELL_PrintStatus(status);
    }
    else if (!strcmp(argv[1], "remove"))
    {
        uint32_t removeRuleNb = NWKU_atoi(argv[2]);
        IP6_FirewallRemoveRule(removeRuleNb);
        SHELL_PrintStatus(gThrStatus_Success_c);
    }


    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\private
\fn     void SHELL_PrintFirewallRules(void *param)
\brief  This function is used to print IPv6 Firewall rules.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintFirewallRules(void * param)
{
    char addrString[INET6_ADDRSTRLEN];
    uint32_t firewallTableSize = IP6_GetFirewallTableSize();
    ip6FirewallTblEntry_t *pFirewallEntry = NULL;

    shell_printf("ruleNb src     dst    srcPort dstPort lenSrc lenDst secLevel ifId\n\r");
    for(uint32_t i = 0; i < firewallTableSize; i++)
    {
        pFirewallEntry = IP6_GetFirewallRule(i);
        if (NULL != pFirewallEntry)
        {
            shell_printf("%d\t", i);

            ntop(AF_INET6, (ipAddr_t*)&pFirewallEntry->srcPrefix, addrString, INET6_ADDRSTRLEN);
            shell_write(addrString);
            shell_write("\t");
            ntop(AF_INET6, (ipAddr_t*)&pFirewallEntry->dstPrefix, addrString, INET6_ADDRSTRLEN);
            shell_write(addrString);
            shell_write("\t");

            shell_printf("%d\t", pFirewallEntry->srcPort);
            shell_printf("%d\t", pFirewallEntry->dstPort);

            shell_printf("%d\t", pFirewallEntry->srcPrefixLenBits);
            shell_printf("%d\t", pFirewallEntry->dstPrefixLenBits);

            shell_printf("%d\t", pFirewallEntry->secLevel);

            uint8_t * ifName = SHELL_SearchInterfaceById((ipIfUniqueId_t)pFirewallEntry->ifUniqueId);
            shell_write((char *)ifName);

            shell_printf("\r\n");
        }
    }
}
#endif /* IP_IP6_FIREWALL_ENABLE */
/*!*************************************************************************************************
\private
\fn     int8_t SHELL_FactoryResetReq(uint8_t argc, char *argv[])
\brief  This function is used to reset the device and restore the factory defaults.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_FactoryResetReq
(
    uint8_t argc,
    char *argv[]
)
{
    shell_write("\rReset Thread stack to factory defaults!\n\r");
    THR_FactoryReset();
    return FALSE;
}

/*!************************************************************************************************
\private
\fn     static void SHELL_ThrCommands(uint8_t argc, char *argv[])
\brief  This function is used to send commands for Thread network.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_ThrCommands
(
    uint8_t argc,
    char *argv[]
)
{
    thrStatus_t status = gThrStatus_EntryNotFound_c;

#if !THREAD_ED_CONFIG
    if(!strcmp(argv[1], "create"))
    {
       if(FALSE == THR_GetAttr_IsDevConnected(threadInstanceID))
       {
           shell_write("\rCreating network...");
           NWKU_SendMsg(SHELL_ThrNwkCreate, NULL, pmMainThreadMsgQueue);
       }
       else if((TRUE == THR_GetAttr_IsDevConnected(threadInstanceID)) &&
               (FALSE == THR_GetAttr_NvmAutostart(threadInstanceID)))
       {
           shell_write("\rRejoining network...");
           NWKU_SendMsg(SHELL_ThrNwkJoin, NULL, pmMainThreadMsgQueue);
       }
       else
       {
           shell_write("\rAlready connected!");
       }
       shell_refresh();
    }
    else
#endif
    if(!strcmp(argv[1], "join"))
    {
        if(FALSE == THR_GetAttr_IsDevConnected(threadInstanceID))
        {
           shell_write("\rJoining network...");
           NWKU_SendMsg(SHELL_ThrNwkJoin, NULL, pmMainThreadMsgQueue);
        }
        else if((TRUE == THR_GetAttr_IsDevConnected(threadInstanceID)) &&
                (FALSE == THR_GetAttr_NvmAutostart(threadInstanceID)))
        {
           shell_write("\rRejoining network...");
           NWKU_SendMsg(SHELL_ThrNwkJoin, NULL, pmMainThreadMsgQueue);
        }
        else
        {
            shell_write("\rAlready connected!");
        }
        shell_refresh();
    }
    else if(!strcmp(argv[1], "detach"))
    {
        if(TRUE == THR_GetAttr_IsDevConnected(threadInstanceID))
        {
            shell_write("\rDetaching from network...\r\n");
            status = THR_NwkDetach(threadInstanceID);
        }
        else
        {
            shell_write("\rDevice already disconnected from network!");
        }
        shell_refresh();
    }
#if !THREAD_ED_CONFIG
    else if(!strcmp(argv[1], "commissioner"))
    {
        if(TRUE == THR_GetAttr_IsDevConnected(threadInstanceID))
        {
            if(!strcmp(argv[2], "start"))
            {
                NWKU_SendMsg(SHELL_ThrStartCommissioner, NULL, pmMainThreadMsgQueue);
            }
            else if(!strcmp(argv[2], "stop"))
            {
                NWKU_SendMsg(SHELL_ThrStopCommissioner, NULL, pmMainThreadMsgQueue);
            }
        }
        else
        {
            shell_write("\rDevice is not connected to Thread network!");
        }
    }
    else if(!strcmp(argv[1], "joiner"))
    {
        if(!strcmp(argv[2], "add"))
        {
            uint8_t eui[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
            int32_t pskLen = strlen(argv[3]);
            uint8_t psk[32];

            FLib_MemSet(&psk, 0, 32);
            FLib_MemCpy(&psk, argv[3], pskLen);

            if(NULL != argv[4])
            {
                FLib_MemSet(&eui, 0, 8);
                SHELL_ReadBuff(argv[4] + 2, eui, 8);
            }

            if(MESHCOP_AddExpectedJoiner(threadInstanceID, eui, psk, pskLen, TRUE))
            {
                status = gThrStatus_Success_c;
            }
        }
        else if(!strcmp(argv[2], "remove"))
        {
            uint8_t eui[8];

            FLib_MemSet(&eui, 0, 8);

            if(NULL != argv[3])
            {
                SHELL_ReadBuff(argv[3] + 2, eui, 8);
            }

            if(MESHCOP_RemoveExpectedJoiner(threadInstanceID, NULL, eui))
            {
                status = gThrStatus_Success_c;
            }
        }
        else if(!strcmp(argv[2], "removeall"))
        {
            MESHCOP_RemoveAllExpectedJoiners(threadInstanceID);
            status = gThrStatus_Success_c;
        }
        else if(!strcmp(argv[2], "view"))
        {
            SHELL_PrintJoinersTbl(NULL);
        }
    }
    else if(!strcmp(argv[1], "sync"))
    {
        if (!strcmp(argv[2], "steering"))
        {
            MESHCOP_SyncSteeringData(threadInstanceID, gMeshcopEuiMaskExpectedJoinerList_c);
        }
        else if (!strcmp(argv[2], "nwkdata"))
        {
            status = THR_BrPrefixAttrSync(threadInstanceID);
        }
    }
#endif /* !THREAD_ED_CONFIG */
    else if(!strcmp(argv[1], "scan"))
    {
        thrNwkScan_t thrNwkScan;

        thrNwkScan.maxThrNwkToDisc = 10; /* maxim 10 thread networks */

        thrNwkScan.scanChannelsMask = THR_GetAttr_ScanChannelMask(threadInstanceID);
        thrNwkScan.scanDuration = THR_GetAttr_ScanDuration(threadInstanceID);

        if(!strcmp(argv[2], "energy"))
        {
            thrNwkScan.scanType = gThrNwkScan_EnergyDetect_c;
        }
        else if(!strcmp(argv[2], "active"))
        {
            thrNwkScan.scanType = gThrNwkScan_ActiveScan_c;
        }
        else if(!strcmp(argv[2], "both"))
        {
            thrNwkScan.scanType = gThrNwkScan_BothScans_c;
        }
        else
        {
            thrNwkScan.scanType = gThrNwkScan_ActiveScan_c;
        }

        if ((!strcmp(argv[2], "allchannel")) || (!strcmp(argv[3], "allchannel")))
        {
            thrNwkScan.scanChannelsMask = 0x07FFF800; /* all channels*/
        }

        if(THR_NwkScanWithBeacon(threadInstanceID, &thrNwkScan) == gThrStatus_NotPermitted_c)
        {
            shell_printf("Scan already active!\n\r");
        }
    }
#if !THREAD_ED_CONFIG
    else if(!strcmp(argv[1], "nwkdata"))
    {
        char *pValue;
        ipAddr_t prefix;
        uint8_t prefixLength = 64;
        uint32_t lifetime = THR_GetNwkDataMinStableLifetime();
        thrOtaBrPrefixSet_t brPrefixSet;

        FLib_MemSet(&brPrefixSet, 0, sizeof(thrOtaBrPrefixSet_t));

        /* Get prefix */
        if ((strcmp(argv[2], "removeall")) && (strcmp(argv[3], "dnsserver")))
        {
            SHELL_ReadIPv6Addr(argc, argv, "-p", &prefix);
        }

        /* Get prefix length */
        pValue = shell_get_opt(argc, argv, "-len");
        if(pValue)
        {
            prefixLength = (uint8_t)NWKU_atoi(pValue);
        }

        /* Get prefix lifetime */
        pValue = shell_get_opt(argc, argv, "-t");
        if(pValue)
        {
            lifetime = NWKU_atoi(pValue);
        }

        brPrefixSet.thrBrPrefixLength = prefixLength;
        htonal(brPrefixSet.thrBrPrefixLifetime, lifetime);
        FLib_MemCpy(brPrefixSet.thrBrPrefixValue, &prefix, sizeof(ipAddr_t));

        if(!strcmp(argv[2], "add"))
        {
#if DHCP6_SERVER_ENABLED
            pValue = shell_get_opt(argc, argv, "dhcpserver");
            if(pValue)
            {
                THR_BR_FLAGS_SET(brPrefixSet.thrBrPrefixFlags[1], 1, THR_BR_PREFIX_FLAGS_P_DHCP_MASK, THR_BR_PREFIX_FLAGS_P_DHCP_OFFSET);
                THR_BR_FLAGS_SET(brPrefixSet.thrBrPrefixFlags[1], 1, THR_BR_PREFIX_FLAGS_P_ON_MESH_MASK, THR_BR_PREFIX_FLAGS_P_ON_MESH_OFFSET);
                THR_BR_FLAGS_SET(brPrefixSet.thrBrPrefixFlags[1], 1, THR_BR_PREFIX_FLAGS_P_DEFAULT_MASK, THR_BR_PREFIX_FLAGS_P_DEFAULT_OFFSET);
                brPrefixSet.thrBrPrefixAdvertised = TRUE;
            }
#endif

            pValue = shell_get_opt(argc, argv, "slaac");
            if(pValue)
            {
                THR_BR_FLAGS_SET(brPrefixSet.thrBrPrefixFlags[1], 1, THR_BR_PREFIX_FLAGS_P_SLAAC_MASK, THR_BR_PREFIX_FLAGS_P_SLAAC_OFFSET);
                THR_BR_FLAGS_SET(brPrefixSet.thrBrPrefixFlags[1], 1, THR_BR_PREFIX_FLAGS_P_ON_MESH_MASK, THR_BR_PREFIX_FLAGS_P_ON_MESH_OFFSET);
                brPrefixSet.thrBrPrefixAdvertised = TRUE;
            }

            pValue = shell_get_opt(argc, argv, "extroute");
            if(pValue)
            {
                THR_BR_FLAGS_SET(brPrefixSet.thrBrExtRouteFlags, 1, THR_BR_EXT_ROUTE_FLAGS_R_PREF_MASK, THR_BR_EXT_ROUTE_FLAGS_R_PREF_OFFSET);
                brPrefixSet.thrBrExtRouteAdvertised = TRUE;
                FLib_MemCpy(brPrefixSet.thrBrExtRouteLifetime, &lifetime, sizeof(uint32_t));
            }

            status = THR_BrPrefixAttrAddEntry(threadInstanceID, &brPrefixSet);
#if DNS_ENABLED
            pValue = shell_get_opt(argc, argv, "dnsserver");
            if (pValue)
            {
                thrLocalServiceSet_t thrServerSet = {.thrSenterpriseNumber = THREAD_ENTERPRISE_NUMBER_ARRAY,
                                                     .thrSserviceDataLen   = 9,
                                                     .thrSserviceData      = "dnsserver",
                                                     .thrSserver16Addr     = {0xFF, 0xFF},
                                                     .thrSserverDataLen    = 0,
                                                     .thrSserverData       = {0},
                                                     .thrSserviceId        = 0,
                                                     .thrSstable           = TRUE};

                thrServerSet.thrSserverDataLen = 16;
                pton(AF_INET6,argv[4], (ipAddr_t *)&thrServerSet.thrSserverData);
                status = THR_ServiceAttrAddEntry(threadInstanceID, &thrServerSet);
            }
#endif
        }
        else if(!strcmp(argv[2], "remove"))
        {
#if DNS_ENABLED
            if (!strcmp(argv[3], "dnsserver"))
            {
                thrLocalServiceSet_t thrServerSet = {.thrSserviceDataLen   = 9,
                                                     .thrSserviceData      = "dnsserver",
                                                     .thrSserverDataLen    = 0,
                                                     .thrSserverData       = {0}};
                pton(AF_INET6,argv[4], (ipAddr_t*)&thrServerSet.thrSserverData);

                status = THR_BrServiceAttrRemoveEntry(threadInstanceID,
                        (uint8_t*)&thrServerSet.thrSserviceData, thrServerSet.thrSserviceDataLen,
                        (uint8_t*)&thrServerSet.thrSserverData, thrServerSet.thrSserverDataLen);
            }
            else
#endif
            {
                status = THR_BrPrefixAttrRemoveEntry(threadInstanceID, (uint32_t)prefixLength, (uint8_t *)&prefix);
            }
        }
        else if(!strcmp(argv[2], "removeall"))
        {
            status = THR_BrPrefixAttrRemoveAll(threadInstanceID);
        }
    }
#endif
    else if(!strcmp(argv[1], "get"))
    {
        uint32_t i;

        if(!strcmp(argv[2], "attributes"))
        {
            shell_printf("Thread network attributes:\n\r");
            for (i = 0; i < NumberOfElements(aShellThrAttr); i++)
            {
                shell_printf("\t%s - %s\n\r", aShellThrAttr[i].pAttrName, aShellThrAttr[i].writable ? "RW" : "RO");
            }
        }
        else
        {
            uint8_t aValue[64];
            uint32_t attrSize = 0;
            uint64_t hexReversed = 0;

            FLib_MemSet(aValue, 0, 64);
            for (i = 0; i < NumberOfElements(aShellThrAttr); i++)
            {
                if(!strcmp(argv[2], aShellThrAttr[i].pAttrName))
                {
                    /* check if a function handles this */
                    if(NULL != aShellThrAttr[i].pfGetTable)
                    {
                        if(argv[3] != NULL)
                        {
                            aShellThrAttr[i].pfGetTable((void *)NWKU_atoi(argv[3]));
                        }
                        else
                        {
                            aShellThrAttr[i].pfGetTable(NULL);
                        }
                    }
                    else
                    {
                        uint64_t value = 0;
                        THR_GetAttr(threadInstanceID, aShellThrAttr[i].attrId, 0, &attrSize, aValue);

                        if((attrSize >= aShellThrAttr[i].maxSize) || (attrSize >= 64))
                        {
                            if(aShellThrAttr[i].maxSize > 64)
                            {
                                attrSize = 64;
                            }
                            else
                            {
                                attrSize = aShellThrAttr[i].maxSize;
                            }
                        }

                        switch(aShellThrAttr[i].valueType)
                        {
                            case gDecimal_c:
                                FLib_MemCpy(&value, aValue, sizeof(value));
                                shell_printf("%s: %d\n\r", aShellThrAttr[i].pAttrName, value);
                            break;
                            case gHex_c:
                            {
                                uint8_t  index = 0;
                                while(attrSize)
                                {
                                    FLib_MemCpy(&value, aValue+index, sizeof(value));
                                    if(attrSize <= 4)
                                    {
                                        shell_printf("%s: 0x%04x\n\r", aShellThrAttr[i].pAttrName, value);
                                        attrSize = 0;
                                    }
                                    else
                                    {
                                        shell_printf("%s: 0x%08x\n\r", aShellThrAttr[i].pAttrName, value);
                                        index += 8;
                                        attrSize -= (attrSize < 8 ? attrSize : 8);
                                    }
                                }
                            }
                            break;
                            case gArray_c:
                                shell_printf("%s: 0x", aShellThrAttr[i].pAttrName);
                                SHELL_PrintBuff(aValue, attrSize);
                                shell_printf("\n\r");
                            break;
                            case gString_c:
                                if(!strcmp(argv[2],"pskcpassword"))
                                {
                                    shell_printf("%s: 0x", "Binary PSKc");
                                    SHELL_PrintBuff(aValue, attrSize);
                                    shell_printf("\n\r");
                                }
                                else
                                {
                                    if(!strcmp(argv[2],"stackversion"))
                                    {
                                        FLib_MemSet(aValue, 0, sizeof(aValue));
                                        FLib_MemCpy(aValue, gNXPThreadStackVersion_c, sizeof(gNXPThreadStackVersion_c));
                                    }

                                    shell_printf("%s: %s\n\r", aShellThrAttr[i].pAttrName, aValue);
                                }
                            break;
                            case gHexReversed_c:
                            {
                                THR_GetAttr(threadInstanceID, aShellThrAttr[i].attrId, 0, &attrSize, (uint8_t*)&hexReversed);
                                NWKU_SwapArrayBytes((uint8_t *)&hexReversed, 8);
                                shell_printf("%s: 0x", aShellThrAttr[i].pAttrName);
                                SHELL_PrintBuff((uint8_t *)&hexReversed, 8);
                                shell_printf("\n\r");
                            }
                            break;
                            default:
                            break;
                        }
                    }
                    break;
                }
            }

            if(i == NumberOfElements(aShellThrAttr))
            {
                shell_write("Unknown parameter!");
            }
        }
    }
    else if(!strcmp(argv[1], "set"))
    {
        uint32_t i;

        for(i = 0; i < NumberOfElements(aShellThrAttr); i++)
        {
            if(!strcmp(argv[2], aShellThrAttr[i].pAttrName))
            {
                if(!aShellThrAttr[i].writable)
                {
                    shell_write("Entry is read only!");
                    break;
                }

                switch(aShellThrAttr[i].valueType)
                {
                    case gDecimal_c:
                    {
                        int64_t pValue = NWKU_atol(argv[3]);
                        /* Set channel cmd. requires a set of scanchannelmask */
                        if(aShellThrAttr[i].attrId == gNwkAttrId_Channel_c)
                        {
                            uint32_t scanMask = 1 << pValue;
                            THR_SetAttr(threadInstanceID, gNwkAttrId_ScanChannelMask_c, 0, sizeof(uint32_t), (void *)&scanMask);
                        }
                        else if(gNwkAttrId_MeshCop_ActiveTimestamp_c == aShellThrAttr[i].attrId)
                        {
                            /* Ticks are not included. Add 0 for ticks */
                            pValue = pValue << 16;
                            /* Active timestamp is stored as big endian */
                            pValue = htonll(pValue);
                        }
                        status = THR_SetAttr(threadInstanceID, aShellThrAttr[i].attrId, 0, aShellThrAttr[i].maxSize, (uint8_t*)&pValue);
                    }
                    break;
                    case gHex_c:
                    {
                        uint32_t value = 0;
                        value = NWKU_AsciiToHex((uint8_t *)(argv[3] + 2), FLib_StrLen(argv[3] + 2));
                        status = THR_SetAttr(threadInstanceID, aShellThrAttr[i].attrId, 0, aShellThrAttr[i].maxSize, (uint8_t*)&value);
                    }
                    break;
                    case gString_c:
                    {
                        uint8_t pValue[32];
                        int32_t len = strlen(argv[3]);
                        FLib_MemCpy(&pValue, argv[3], len);

                        if(gNwkAttrId_PSKc_c == aShellThrAttr[i].attrId)
                        {
                            /* generate and set the pskc attribute (binary value) */
                            status = THR_SetPskcPassphrase(threadInstanceID, pValue, len);
                        }
#if !THREAD_ED_CONFIG
                        else if(gNwkAttrId_DeviceRole_c == aShellThrAttr[i].attrId)
                        {
                            thrDeviceRole_t devRole = gThrDeviceRole_REED_c;

                            if(FLib_MemCmp(pValue, "SED", 3))
                            {
                                devRole = gThrDeviceRole_SED_c;
                            }
                            else if(FLib_MemCmp(pValue, "FED", 3))
                            {
                                devRole = gThrDeviceRole_FED_c;
                            }
                            else if(FLib_MemCmp(pValue, "MED", 3))
                            {
                                devRole = gThrDeviceRole_MED_c;
                            }

                            status = THR_SetDeviceRole(threadInstanceID, devRole);
                        }
#endif /* THREAD_ED_CONFIG */
                        else
                        {
                            status = THR_SetAttr(threadInstanceID, aShellThrAttr[i].attrId, 0, len, pValue);
                        }
                    }
                    break;
                    case gArray_c:
                    {
                        uint8_t pValue[16];
                        SHELL_ReadBuff(argv[3] + 2, pValue, aShellThrAttr[i].maxSize);
                        status = THR_SetAttr(threadInstanceID, aShellThrAttr[i].attrId, 0, aShellThrAttr[i].maxSize, (uint8_t*)&pValue);
                    }
                    break;
                    case gHexReversed_c:
                    {
                        uint8_t pValue[8];
                        SHELL_ReadBuff(argv[3] + 2, pValue, 8);
                        NWKU_SwapArrayBytes(pValue, 8);
                        status = THR_SetAttr(threadInstanceID, aShellThrAttr[i].attrId, 0, aShellThrAttr[i].maxSize, (uint8_t*)&pValue);
                    }
                    break;
                    default:
                    break;
                }
                break;
            }

        }
        if(i == NumberOfElements(aShellThrAttr))
        {
            shell_write("Unknown parameter!");
        }
    }
#if !THREAD_ED_CONFIG
    else if(!strcmp(argv[1], "remove"))
    {
        if(!strcmp(argv[2], "router"))
        {
            uint16_t shortAddress;
            shortAddress = NWKU_AsciiToHex((uint8_t *)(argv[3] + 2), FLib_StrLen(argv[3] + 2));
            status = THR_LeaderRemoveRouterID(threadInstanceID, THR_SHORT_ADDR_TO_R_ID(shortAddress));
        }
    }
#endif
#if SHELL_DUT_COMMISSIONER
    else if (!strcmp(argv[1], "mgmt"))
    {
        if (!strcmp(argv[2], "gettlvs"))
        {
            for (uint32_t iCount = 0; iCount < NumberOfElements(aShellThrMgmtTlvs); iCount++)
            {
                shell_printf("%s\r\n", aShellThrMgmtTlvs[iCount].pTlvName);
            }
        }
        else
        {
            ipAddr_t destIpAddr = {0};

            bool_t set = FALSE;
            bool_t get = FALSE;
            uint8_t* pTlvBuff = NULL;
            uint32_t maxBuffLen = 0;
            uint32_t tlvBuffLen = 0;
            meshcopMgmtParams_t mgmtParams = {0};
            nwkStatus_t nwkStatus = gNwkStatusFail_c;

            if (argv[3] != NULL)
            {
                pton(AF_INET6, argv[3], &destIpAddr);
            }

            if ((!strcmp(argv[2], "activeset")) || (!strcmp(argv[2], "pendset")) || (!strcmp(argv[2], "commset")))
            {
                set = TRUE;
                for (uint32_t iCount = 0; iCount < NumberOfElements(aShellThrMgmtTlvs); iCount++)
                {
                    maxBuffLen += aShellThrMgmtTlvs[iCount].tlvLen + MLE_TLV_HEADER_SIZE;
                }
            }
            else if ((!strcmp(argv[2], "activeget")) || (!strcmp(argv[2], "pendget")) || (!strcmp(argv[2], "commget")))
            {
                get = TRUE;
                maxBuffLen = NumberOfElements(aShellThrMgmtTlvs);
            }
            else if (!strcmp(argv[2], "query"))
            {
                char *pValue = NULL;
                uint8_t channelMask[4] = {0};
                uint16_t pan = 0;

                pValue = shell_get_opt(argc, argv, "chmsk");
                if (pValue)
                {
                    SHELL_ReadBuff(pValue + 2, channelMask, sizeof(uint32_t));
                }
                pValue = shell_get_opt(argc, argv, "pan");
                if (pValue)
                {
                    pan = (uint16_t)NWKU_AsciiToHex((uint8_t *)(pValue+ 2), FLib_StrLen(pValue+ 2));
                }

                nwkStatus = MESHCOP_MgmtSendPanIdQuery(threadInstanceID, ntohal(channelMask), pan,
                SHELL_MeshcopMgmtPanIdConflictCb, &destIpAddr);

            }
            else if (!strcmp(argv[2], "leave"))
            {
                MESHCOP_SendNetworkLeave(&destIpAddr, NULL);
            }

            if (get || set)
            {
                pTlvBuff = MEM_BufferAlloc(maxBuffLen);
                if (pTlvBuff == NULL)
                {
                    shell_write("No memory!");
                    return CMD_RET_SUCCESS;
                }
                FLib_MemSet(pTlvBuff, 0, maxBuffLen);
                SHELL_ReadMgmtTlv(argc, argv, pTlvBuff, &tlvBuffLen, set);

                mgmtParams.pDstIpAddr = (uint8_t *)&destIpAddr;
                mgmtParams.pTlvs = pTlvBuff;
                mgmtParams.tlvsLength = tlvBuffLen;
                mgmtParams.thrInstId = threadInstanceID;
                if (mgmtParams.tlvsLength == 0)
                {
                    mgmtParams.pTlvs = NULL;
                }
                if (set)
                {
                    mgmtParams.pfCb = SHELL_MeshcopMgmtSetCb;

                    if (!strcmp(argv[2], "activeset"))
                    {
                        nwkStatus = MESHCOP_MgmtActiveSet(&mgmtParams);
                    }
                    if (!strcmp(argv[2], "commset"))
                    {
                        nwkStatus = MESHCOP_MgmtCommSet(&mgmtParams);
                    }
                    if (!strcmp(argv[2], "pendset"))
                    {
                        nwkStatus = MESHCOP_MgmtPendingSet(&mgmtParams);
                    }
                }
                if (get)
                {
                    mgmtParams.pfCb = SHELL_MeshcopMgmtGetCb;

                    if (!strcmp(argv[2], "activeget"))
                    {
                        nwkStatus = MESHCOP_MgmtActiveGet(&mgmtParams);
                    }
                    if (!strcmp(argv[2], "commget"))
                    {
                        nwkStatus = MESHCOP_MgmtCommGet(&mgmtParams);
                    }
                    if (!strcmp(argv[2], "pendget"))
                    {
                        nwkStatus = MESHCOP_MgmtPendingGet(&mgmtParams);
                    }
                }
            }
            if (nwkStatus == gNwkStatusSuccess_c)
            {
                status = gThrStatus_Success_c;
            }
            else
            {
                status = gThrStatus_Failed_c;
            }
            MEM_BufferFree(pTlvBuff);
        }
    }
#endif
    else
    {
        shell_write("Unknown command!");
    }

    if(status != gThrStatus_EntryNotFound_c)
    {
        SHELL_PrintStatus(status);
    }

    return CMD_RET_SUCCESS;
}
#if SHELL_DUT_COMMISSIONER
/*!************************************************************************************************
\private
\fn     static void SHELL_ReadMgmtTlv(uint8_t argc, char *argv[], uint8_t *pTlvBuff,
                                    uint32_t* pTlvBuffLen,    bool_t bTlvSet)
\brief  This function is to read the TLV's values from terminal, process and copy them in a memory
buffer.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments
\param  [in]    pTlvBuff  Pointer to the start of the memory buffer.
\param  [in]    bTlvSet   TRUE - if it is a set command (i.e all TLV names are followed by a value)
                          FALSE - if it is a get command (all arguments are TLV names)
\param  [out]    pTlvBuffLen    Length of the output buffer

\return         none
***************************************************************************************************/
static void SHELL_ReadMgmtTlv
(
    uint8_t argc,
    char *argv[],
    uint8_t *pTlvBuff,
    uint32_t* pTlvBuffLen,
    bool_t bTlvSet
)
{
    uint8_t argNbTlv = 4;
    uint8_t *pCrtPos = pTlvBuff;
    uint32_t tlvBuffLen = 0;

    while ((argv[argNbTlv] != NULL) && (argNbTlv < argc))
    {
        for (uint32_t iCount = 0; iCount < NumberOfElements(aShellThrMgmtTlvs); iCount++)
        {
            if (!strcmp(argv[argNbTlv], aShellThrMgmtTlvs[iCount].pTlvName))
            {
                *pCrtPos++ = aShellThrMgmtTlvs[iCount].tlvId;
                if (bTlvSet)
                {
                    *pCrtPos++ = aShellThrMgmtTlvs[iCount].tlvLen;
                    argNbTlv++;
                    switch (aShellThrMgmtTlvs[iCount].tlvId)
                    {
                        case gMeshCopTlvChannel_c:
                        {
                            meshCopChannelTlv_t channelTlv = {0};
                            uint16_t value = NWKU_atoi(argv[argNbTlv]);
                            channelTlv.channelPage = 0;
                            htonas(channelTlv.channel, value);
                            FLib_MemCpy(pCrtPos, (uint8_t*)&channelTlv+MLE_TLV_HEADER_SIZE, aShellThrMgmtTlvs[iCount].tlvLen);
                            pCrtPos += aShellThrMgmtTlvs[iCount].tlvLen;
                            tlvBuffLen += aShellThrMgmtTlvs[iCount].tlvLen+MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvPanID_c:
                        case gMeshCopTlvBorderRouterLoc_c:
                        case gMeshCopTlvCommSessId_c:
                        {
                            uint16_t value = 0;
                            value = (uint16_t)NWKU_AsciiToHex((uint8_t *)(argv[argNbTlv] + 2), FLib_StrLen(argv[argNbTlv] + 2));
                            htonas(pCrtPos, value);
                            pCrtPos += aShellThrMgmtTlvs[iCount].tlvLen;
                            tlvBuffLen += aShellThrMgmtTlvs[iCount].tlvLen+MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvXpanID_c:
                        case gMeshCopTlvNwkMlUla_c:
                        {
                            SHELL_ReadBuff(argv[argNbTlv] + 2, pCrtPos, aShellThrMgmtTlvs[iCount].tlvLen);
                            pCrtPos += aShellThrMgmtTlvs[iCount].tlvLen;
                            tlvBuffLen += aShellThrMgmtTlvs[iCount].tlvLen+MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvNwkName_c:
                        {
                            int32_t len = strlen(argv[argNbTlv]);
                            pCrtPos--;
                            *pCrtPos++ = len;
                            FLib_MemCpy(pCrtPos, argv[argNbTlv], len);
                            pCrtPos += len;
                            tlvBuffLen += len + MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvNwkMasterKey_c:
                        case gMeshCopTlvPskc_c:
                        {
                            SHELL_ReadBuff(argv[argNbTlv] + 2, pCrtPos, 16);
                            pCrtPos += 16;
                            tlvBuffLen += 16 + MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvSteeringData_c:
                        {
                            uint8_t len = (strlen(argv[argNbTlv])-2) >> 1;;
                            pCrtPos--;
                            *pCrtPos++ = len;
                            SHELL_ReadBuff(argv[argNbTlv] + 2, pCrtPos, len);
                            pCrtPos += len;
                            tlvBuffLen += len + MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvSecPolicy_c:
                        {
                            meshCopSecurityPolicyTlv_t secPolicyTlv = {0};
                            uint16_t pValue = NWKU_atoi(argv[argNbTlv]);
                            htonas(secPolicyTlv.rotationInterval, pValue);
                            argNbTlv++;
                            secPolicyTlv.policy = NWKU_AsciiToHex((uint8_t *)(argv[argNbTlv] + 2), FLib_StrLen(argv[argNbTlv] + 2));
                            FLib_MemCpy(pCrtPos, (uint8_t*)&secPolicyTlv + MLE_TLV_HEADER_SIZE,
                            aShellThrMgmtTlvs[iCount].tlvLen);
                            pCrtPos += aShellThrMgmtTlvs[iCount].tlvLen;
                            tlvBuffLen += aShellThrMgmtTlvs[iCount].tlvLen + MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvActiveTimestamp_c:
                        case gMeshCopTlvPendingTimestamp_c:
                        {
                            uint64_t pValue = NWKU_atol(argv[argNbTlv]);
                            pValue = pValue << 16;
                            htonall(pCrtPos, pValue);
                            pCrtPos += aShellThrMgmtTlvs[iCount].tlvLen;
                            tlvBuffLen += aShellThrMgmtTlvs[iCount].tlvLen+MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvDelayTimer_c:
                        {
                            uint32_t pValue = NWKU_atoi(argv[argNbTlv]);
                            htonal(pCrtPos, pValue);
                            pCrtPos += aShellThrMgmtTlvs[iCount].tlvLen;
                            tlvBuffLen += aShellThrMgmtTlvs[iCount].tlvLen + MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvChannelMask_c:
                        {
                            meshCopChannelMaskTlv_t channelMaskTlv = {0};
                            SHELL_ReadBuff(argv[argNbTlv] + 2, channelMaskTlv.channelMask, sizeof(channelMaskTlv.channelMask));
                            channelMaskTlv.channelPage = 0;
                            channelMaskTlv.maskLength = sizeof(channelMaskTlv.channelMask);
                            FLib_MemCpy(pCrtPos, (uint8_t*)&channelMaskTlv+MLE_TLV_HEADER_SIZE,
                                aShellThrMgmtTlvs[iCount].tlvLen);
                            pCrtPos += aShellThrMgmtTlvs[iCount].tlvLen;
                            tlvBuffLen += aShellThrMgmtTlvs[iCount].tlvLen + MLE_TLV_HEADER_SIZE;
                        }
                            break;
                        case gMeshCopTlvFuture_c:
                            tlvBuffLen += MLE_TLV_HEADER_SIZE;
                        default:
                            break;
                    }
                }
                else
                {
                    tlvBuffLen++;
                }
                break;
            }
        }
        argNbTlv++;
    }
    *pTlvBuffLen = tlvBuffLen;
}


/*!*************************************************************************************************
\private
\fn     SHELL_MgmtPanIdConflictCbHandler
\brief  This function is used to handle the response of Management Pan Id Query command.

\param  [in]    param

\return         void
***************************************************************************************************/
static void SHELL_MgmtPanIdConflictCbHandler
(
    uint8_t *param
)
{
    shellMgmtCb_t* pMgmtCb = (shellMgmtCb_t*)param;

    if (pMgmtCb->tlvsLen != 0)
    {
        uint32_t channelMask = ntohal(pMgmtCb->pTlvs);
        uint16_t panId = ntohas(pMgmtCb->pTlvs + sizeof(channelMask));
        shell_printf("Pan ID conflict on channel mask: %i, pan ID: %i\n\r", channelMask, panId);
    }
    MEM_BufferFree(param);
    SHELL_Resume();
}

/*!*************************************************************************************************
\private
\fn     SHELL_MgmtSetCbHandler
\brief  This function is used to handle the response of Management Set command.

\param  [in]    param

\return         void
***************************************************************************************************/
static void SHELL_MgmtSetCbHandler
(
    uint8_t *param
)
{
    shellMgmtCb_t* pMgmtCb = (shellMgmtCb_t*)param;

    meshCopStateTlv_t *pStateTlv = (meshCopStateTlv_t*)pMgmtCb->pTlvs;

    if ((pStateTlv) && (pStateTlv->type == gMeshCopTlvState_c))
    {
        shell_write("Received state: ");
        if (pStateTlv->state == MESHCOP_STATE_ACCEPT)
        {
            shell_write("Accept");
        }
        else if (pStateTlv->state == MESHCOP_STATE_PENDING)
        {
            shell_write("Pending");
        }
        if (pStateTlv->state == MESHCOP_STATE_REJECT)
        {
            shell_write("Reject");
        }
    }
    MEM_BufferFree(param);
    SHELL_Resume();
}


/*!*************************************************************************************************
\private
\fn     SHELL_MgmtGetCbHandler
\brief  This function is used to handle the response of Management Get command.

\param  [in]    param

\return         void
***************************************************************************************************/
static void SHELL_MgmtGetCbHandler
(
    uint8_t *param
)
{
    shellMgmtCb_t* pMgmtCb = (shellMgmtCb_t*)param;
    uint8_t *pTlvs = pMgmtCb->pTlvs;
    int32_t remainingLen = pMgmtCb->tlvsLen;
    uint8_t tlvId;
    uint8_t tlvLen;

    while (remainingLen > 0)
    {
        tlvId = *pTlvs++;
        tlvLen = *pTlvs++;
        remainingLen -= MLE_TLV_HEADER_SIZE;
        for (uint32_t iCount = 0; iCount < NumberOfElements(aShellThrMgmtTlvs); iCount++)
        {
            if (aShellThrMgmtTlvs[iCount].tlvId == tlvId)
            {
                shell_printf("%s: ", aShellThrMgmtTlvs[iCount].pTlvName);
                pTlvs -= MLE_TLV_HEADER_SIZE;
                switch (aShellThrMgmtTlvs[iCount].tlvId)
                {
                    case gMeshCopTlvChannel_c:
                    {
                        meshCopChannelTlv_t *pChTlv = (meshCopChannelTlv_t*)pTlvs;
                        shell_printf("%d\r\n", ntohas(pChTlv->channel));
                    }
                        break;
                    case gMeshCopTlvPanID_c:
                    {
                        meshCopNwkPanIdTlv_t *pPanTlv = (meshCopNwkPanIdTlv_t*)pTlvs;
                        shell_printf("0x%04x\n\r", ntohas(pPanTlv->panId));
                    }
                        break;
                    case gMeshCopTlvXpanID_c:
                    {
                        meshCopNwkXPanIdTlv_t *pXpanTlv = (meshCopNwkXPanIdTlv_t*)pTlvs;
                        shell_write("0x");
                        SHELL_PrintBuff(pXpanTlv->xPanId, sizeof(pXpanTlv->xPanId));
                        shell_printf("\n\r");
                    }
                        break;
                    case gMeshCopTlvNwkName_c:
                    {
                        uint8_t aStr[16] = {0};
                        meshCopNwkNameTlv_t *pNwkNameTlv = (meshCopNwkNameTlv_t*)pTlvs;
                        FLib_MemCpy(aStr, pNwkNameTlv->nwkName, pNwkNameTlv->len);
                        shell_printf("%s\n\r", aStr);
                    }
                        break;
                    case gMeshCopTlvPskc_c:
                    {
                        meshCopPskcTlv_t *pPskcTlv = (meshCopPskcTlv_t*)pTlvs;
                        shell_write("0x");
                        SHELL_PrintBuff(pPskcTlv->pskc, pPskcTlv->len);
                        shell_printf("\n\r");
                    }
                        break;
                    case gMeshCopTlvNwkMasterKey_c:
                    {
                        meshCopNwkMasterKeyTlv_t *pMasterKeyTlv = (meshCopNwkMasterKeyTlv_t*)pTlvs;
                        shell_write("0x");
                        SHELL_PrintBuff(pMasterKeyTlv->masterKey, pMasterKeyTlv->len);
                        shell_printf("\n\r");
                    }
                        break;
                    case gMeshCopTlvNwkMlUla_c:
                    {
                        meshCopNwkMlUlaTlv_t *pMlTlv = (meshCopNwkMlUlaTlv_t*)pTlvs;
                        shell_write("0x");
                        SHELL_PrintBuff(pMlTlv->mlUla, pMlTlv->len);
                        shell_printf("\n\r");
                    }
                        break;
                    case gMeshCopTlvSteeringData_c:
                    {
                        meshCopSteeringTlv_t *pSteeringTlv = (meshCopSteeringTlv_t*)pTlvs;
                        shell_write("0x");
                        SHELL_PrintBuff(pSteeringTlv->filter, pSteeringTlv->len);
                        shell_printf("\n\r");
                    }
                        break;
                    case gMeshCopTlvBorderRouterLoc_c:
                    {
                        meshCopBrLocTlv_t *pBrTlv = (meshCopBrLocTlv_t*)pTlvs;
                        shell_printf("0x%04x\n\r", ntohas(pBrTlv->addr));
                    }
                        break;
                    case gMeshCopTlvCommSessId_c:
                    {
                        meshCopCommSessIdTlv_t *pCommSid = (meshCopCommSessIdTlv_t*)pTlvs;
                        shell_printf("0x%04x\n\r", ntohas(pCommSid->id));
                    }
                        break;
                    case gMeshCopTlvSecPolicy_c:
                    {
                        meshCopSecurityPolicyTlv_t *pSecPol = (meshCopSecurityPolicyTlv_t*)pTlvs;
                        shell_printf("%d, ", ntohas(pSecPol->rotationInterval));
                        shell_printf("0x%x\n\r", pSecPol->policy);
                    }
                        break;
                    case gMeshCopTlvActiveTimestamp_c:
                    case gMeshCopTlvPendingTimestamp_c:
                    {
                        meshCopActiveTimestampTlv_t *pTimestamp = (meshCopActiveTimestampTlv_t*)pTlvs;
                        uint64_t seconds = ntohall(pTimestamp->seconds);
                        uint16_t ticks = ntohas(pTimestamp->ticks);
                        uint8_t activeTimestampString[16] = {0};
                        seconds = seconds >> 16;

                        NWKU_PrintDec(seconds, activeTimestampString, THR_ALL_FFs8, FALSE);
                        shell_printf("%s sec, ", activeTimestampString);
                        shell_printf(" %d ticks\r\n", ticks);
                    }
                        break;
                    case gMeshCopTlvDelayTimer_c:
                    {
                        meshCopDelayTimerTlv_t *pDelayTmr = (meshCopDelayTimerTlv_t*)pTlvs;
                        shell_printf("%d ms\r\n", ntohal(pDelayTmr->timeRemaining));
                    }
                        break;
                    case gMeshCopTlvChannelMask_c:
                    {
                        meshCopChannelMaskTlv_t *pChMask = (meshCopChannelMaskTlv_t*)pTlvs;
                        shell_printf("0x%08x\n\r", ntohal(pChMask->channelMask));
                    }
                        break;
                default:
                        break;
                }

                pTlvs += tlvLen + MLE_TLV_HEADER_SIZE;
                remainingLen -= tlvLen;
                break;
            }
        }
    }
    MEM_BufferFree(param);
    SHELL_Resume();
}

#endif
/*!*************************************************************************************************
\private
\fn     static void SHELL_ThrNwkJoin(uint8_t *param)
\brief  This function is used to join to a Thread network.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_ThrNwkJoin
(
    uint8_t *param
)
{
    thrStatus_t status = gThrStatus_Failed_c;

    gbRetryInterrupt = FALSE;
    status = THR_NwkJoin(threadInstanceID, THR_APP_JOIN_DISCOVERY_METHOD);

    switch(status)
    {
        case gThrStatus_InvalidParam_c:
        case gThrStatus_InvalidInstance_c:
            shell_printf("\rInvalid Thread instance!\n\r");
            break;
        case gThrStatus_AlreadyConnected_c:
            shell_printf("\rAlready connected!\n\r");
            break;
        case gThrStatus_NotPermitted_c:
            shell_printf("\rJoin not permitted!\n\r");
            break;
        default:
            /* Default is gThrStatus_Success_c */
            break;
    }
}

#if !THREAD_ED_CONFIG
/*!*************************************************************************************************
\private
\fn     static void SHELL_ThrNwkCreate(uint8_t *param)
\brief  This function is used to create a Thread network.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_ThrNwkCreate
(
    uint8_t *param
)
{
    thrStatus_t status = gThrStatus_Failed_c;

    status = THR_NwkCreate(threadInstanceID);
    switch(status)
    {
        case gThrStatus_InvalidParam_c:
            shell_printf("Invalid parameters!\n\r");
            break;
        case gThrStatus_AlreadyConnected_c:
            shell_printf("Already connected!\n\r");
            break;
        case gThrStatus_NotPermitted_c:
            shell_printf("Creating the network is not permitted!\n\r");
            break;
        default:
            /* Default is gThrStatus_Success_c */
            break;
    }
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_ThrStartCommissioner(uint8_t *param)
\brief  This function is used to start a commissioner.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_ThrStartCommissioner
(
    uint8_t *param
)
{
    MESHCOP_StartCommissioner(threadInstanceID);
}

/*!************************************************************************************************
*
\private
\fn     static void SHELL_ThrStopCommissioner(uint8_t *param)
\brief  This function is used to stop a commissioner..

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_ThrStopCommissioner
(
    uint8_t *param
)
{
    MESHCOP_StopCommissioner(threadInstanceID, TRUE);
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintJoinersTbl(void *param)
\brief  This function is used to print the list of allowed joiners.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintJoinersTbl
(
    void *param
)
{
    uint32_t index = 0;
    expectedJoinerEntry_t *pJoinerEntry = MESHCOP_GetExpectedJoinerList(threadInstanceID);

    shell_printf("Index  EUI                 PSK\n\r");
    while(pJoinerEntry)
    {
        shell_printf("\r%d     ", index);

        shell_printf(" 0x");
        if (pJoinerEntry->ffEntry == TRUE)
        {
            uint64_t allFfs64 = THR_ALL_FFs64;
            SHELL_PrintBuff((uint8_t *)&allFfs64, sizeof(allFfs64));
        }
        else
        {
            SHELL_PrintBuff((uint8_t *)&pJoinerEntry->aHashEui, sizeof(pJoinerEntry->aHashEui));
        }

        FLib_MemSet(pJoinerEntry->aPsk + pJoinerEntry->pskLen, 0, 32 - pJoinerEntry->pskLen);
        shell_printf("  %s", pJoinerEntry->aPsk);

        shell_printf("\n\r");

        pJoinerEntry = ListGetNextMsg(pJoinerEntry);
        index++;
    }
}

/*!*************************************************************************************************
\private
\fn     void SHELL_PrintRoutingTbl(void *param)
\brief  This function is used to print routing table.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintRoutingTbl
(
    void *param
)
{
    thrRouterIdSet_t *pRouterIdSet = NULL;
    uint32_t routerCount = 0;
    thrInterfaceSet_t *pThrIfTbl = (thrInterfaceSet_t *)THR_GetRoutingInterfaceParams(threadInstanceID);

    pRouterIdSet = THR_GetRouterIdSet(threadInstanceID);

    if (pRouterIdSet == NULL)
    {
      shell_printf("Router ID set is empty");
      return;
    }

    shell_printf("ID Sequence: %d\n\r", pRouterIdSet->thrIdSeqNb);
    shell_printf("Router ID Mask: ");
    SHELL_PrintBuff(pRouterIdSet->thrIdSet, 8);
    shell_printf("\n\r");

    shell_printf("RouterID    Short Address    Next Hop    Cost    NOut    NIn \n\r");
    /* count the number of allocated router IDs */
    for(uint32_t i = 0; i <= THR_MAX_ROUTER_ID; i++)
    {
        if(THR_R_ID_IS_SET_IN_MASK(pRouterIdSet->thrIdSet, i))
        {
            if(THR_SHORT_ADDR_TO_R_ID(pThrIfTbl->deviceShortAddr) != i)
            {
                uint8_t routeCost = 0;
                uint8_t nOut = 0;
                uint8_t nIn = 0;

                shell_printf("%d", i);

                shell_printf("           ");
                shell_printf("0x%04X", THR_R_ID_TO_SHORT_ADDR(i));

                shell_printf("           ");
                shell_printf("0x%04X",pThrIfTbl->pThreadRoutingTbl[i].thrNextHopRouterId);

                shell_printf("      ");
                routeCost = (*((uint8_t *)pRouterIdSet + sizeof(thrRouterIdSet_t) + routerCount)) & 0x0F;
                shell_printf("%d", routeCost);

                shell_printf("       ");
                nOut = ((*((uint8_t *)pRouterIdSet + sizeof(thrRouterIdSet_t) + routerCount)) & 0xC0) >> 6;
                shell_printf("%d", nOut);

                shell_printf("       ");
                nIn = ((*((uint8_t *)pRouterIdSet + sizeof(thrRouterIdSet_t) + routerCount)) & 0x30)>> 4;
                shell_printf("%d", nIn);

                shell_printf("\n\r");
            }
            routerCount++;
        }
    }
    MEM_BufferFree(pRouterIdSet);
}
#endif

/*!*************************************************************************************************
\fn     static void SHELL_PrintStatus(thrStatus_t statusCode)
\brief  This function is used for printing in shell terminal the status of the input operation.

\param  [in]    statusCode    The code of the status to be printed
***************************************************************************************************/
static void SHELL_PrintStatus
(
    thrStatus_t statusCode
)
{
    if(gThrStatus_Success_c != statusCode)
    {
        shell_write(" An error has occurred!");
    }
    else
    {
        shell_write(" Success!");
    }
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintParentInfo(void *param)
\brief  This function is used to print information about the parent of the node.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintParentInfo
(
    void *param
)
{
    thrNeighbor_t *pParent = NULL;

    pParent = THR_GetParent(threadInstanceID);
    if(NULL != pParent)
    {
        uint64_t reversedExtAddr = pParent->extendedAddress;

        NWKU_SwapArrayBytes((uint8_t *)&reversedExtAddr,8);
        shell_printf("Parent short address: 0x%04X\n\r",pParent->shortAddress);
        shell_printf("Parent extended address: 0x");
        SHELL_PrintBuff((uint8_t*)&reversedExtAddr,8);
        shell_printf("\n\r");
    }
    else
    {
        shell_write("No parent found!");
    }
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintNeighborTbl(void *param)
\brief  This function is used to print neighbor table.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintNeighborTbl
(
    void *param
)
{
    uint32_t iCount;
    thrNeighbor_t *pThrNeighbor;
    uint16_t attrShortAddr;
    uint32_t thrNeighborTblSize = THR_GetNeighborsTblSize(threadInstanceID);

    (void)THR_GetAttr(threadInstanceID, gNwkAttrId_ShortAddr_c, 0, NULL, (uint8_t*)&(attrShortAddr));

    shell_printf("\n\rIndex Extended Address     ShortAddr  LastTime LinkMargin Child State\n\r");
    for (iCount = 0; iCount < thrNeighborTblSize; iCount++)
    {
        pThrNeighbor = THR_GetNeighborTable(iCount);

        if (NULL != pThrNeighbor)
        {
            uint64_t reversedExtAddr = pThrNeighbor->extendedAddress;

            NWKU_SwapArrayBytes((uint8_t *)&reversedExtAddr, 8);
            shell_printf("\r%d     ", iCount);
            shell_printf("0x");
            SHELL_PrintBuff((uint8_t *)&reversedExtAddr, 8);

            shell_printf("   ");
            shell_printf("0x%04X", pThrNeighbor->shortAddress);

            shell_printf("      ");
            shell_printf("%d  ", ((uint32_t)TmrMicrosecondsToSeconds(TMR_GetTimestamp()) - pThrNeighbor->timestamp));

            shell_printf("\t%d ", pThrNeighbor->inLinkMargin);

            if(THR_IS_MY_CHILD(pThrNeighbor->shortAddress, attrShortAddr))
            {
                shell_printf("\t   yes");
            }
            else
            {
                shell_printf("\t   no");
            }

            shell_printf("  ");
            SHELL_PrintNeighborState(pThrNeighbor->state);

            shell_printf("\n\r");
        }
    }
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintNeighborInfo(void *param)
\brief  This function is used to print information about a specific neighbor.

\param  [in]    param    Neighbor index
***************************************************************************************************/
static void SHELL_PrintNeighborInfo
(
    void *param
)
{
    uint32_t neighborIdx = (uint32_t)param;
    thrNeighbor_t *pThrNeighbor;

    pThrNeighbor = THR_GetNeighborTable(neighborIdx);
    if (NULL != pThrNeighbor)
    {
        uint64_t reversedExtAddr = pThrNeighbor->extendedAddress;

        NWKU_SwapArrayBytes((uint8_t *)&reversedExtAddr,8);
        shell_printf("Extended Address: 0x");
        SHELL_PrintBuff((uint8_t *)&reversedExtAddr,8);
        shell_printf("\n\r");

        shell_printf("Short Address:    0x%04X\n\r", pThrNeighbor->shortAddress);

        shell_printf("Last communication: %d seconds ago\n\r",
               ((uint32_t)TmrMicrosecondsToSeconds(TMR_GetTimestamp()) - pThrNeighbor->timestamp));
        shell_printf("InLinkMargin: %d\n\r", pThrNeighbor->inLinkMargin);

        shell_printf("Device Timeout value: %d seconds\n\r", pThrNeighbor->timeoutSec);
        SHELL_PrintNeighborState(pThrNeighbor->state);
    }
    else
    {
        shell_write("Neighbor not found!");
    }
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintNeighborState(uint8_t state)
\brief  This function is used to print the state of a specified neighbor.

\param  [in]    state    Neighbor state
***************************************************************************************************/
static void SHELL_PrintNeighborState
(
    uint8_t state
)
{
    switch(state)
    {
        case 0x00: /* mThrNeigborFreeEntry_c */
            shell_write("Neighbor Free Entry");
            break;

        case 0x01: /* mThrNeigborAttachToMe_c */
            shell_write("Neighbor Attaching To Me");
            break;

        case 0x02: /* mThrNeigborUnsynced_c */
            shell_write("Neighbor Unsynced");
            break;

        case 0x03: /* mThrNeigborInLinkSync_c */
            shell_write("Neighbor In Link Sync");
            break;

        case 0x04: /* mThrNeigborSynced_c */
            shell_write("Neighbor Synced");
            break;

        default:
            shell_write("Unknown state");
            break;
    }
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintDevRole(void *param)
\brief  This function is used to print the device's role.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintDevRole
(
    void *param
)
{
    uint8_t devRole = THR_ALL_FFs8;

    THR_GetAttr(threadInstanceID, gNwkAttrId_DeviceRole_c, 0, NULL, &devRole);
    shell_printf("devrole: %s", mThrDevRole[devRole]);
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintDevType(void *param)
\brief  This function is used to print the device's type.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintDevType
(
    void *param
)
{
    uint8_t devType = THR_ALL_FFs8;

    THR_GetAttr(threadInstanceID, gNwkAttrId_DeviceType_c, 0, NULL, &devType);
    if(devType == gThrDevType_EndNode_c)
    {
        shell_printf("devtype: End Node");
    }
    else
    {
        shell_printf("devtype: Combo Node");
    }
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintActiveTimestamp(void *param)
\brief  This function is used to print the active timestamp.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintActiveTimestamp
(
    void *param
)
{
    uint64_t activeTimestamp;
    uint8_t activeTimestampString[16] = {0};

    THR_GetAttr(threadInstanceID, gNwkAttrId_MeshCop_ActiveTimestamp_c, 0, NULL, (uint8_t*)&activeTimestamp);
    /* Active timestamp is stored as big endian */
    activeTimestamp = ntohall((uint8_t*)&activeTimestamp);
    /* Remove ticks. */
    activeTimestamp = activeTimestamp >> 16;
    NWKU_PrintDec(activeTimestamp, activeTimestampString, THR_ALL_FFs8, FALSE);
    shell_printf("%s seconds\n\r", activeTimestampString);
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintNwkCapabilities(void *param)
\brief  This function is used to print the device's network capabilities.

\param  [in]    param    Not used
***************************************************************************************************/
static void SHELL_PrintNwkCapabilities
(
    void *param
)
{
    uint8_t nwkCap = THR_ALL_FFs8;
    char *canCreateNewNwk, *isActiveRouter, *isPolling, *isFfd;

    canCreateNewNwk = STRING_FALSE;
    isActiveRouter = STRING_FALSE;
    isPolling = STRING_FALSE;
    isFfd = STRING_FALSE;

    THR_GetAttr(threadInstanceID, gNwkAttrId_ThreadNwkCapabilites_c, 0, NULL, &nwkCap);

    if(nwkCap & THR_NWKCAP_CAN_CREATE_NEW_NETWORK)
    {
        canCreateNewNwk = STRING_TRUE;
    }
    shell_printf("Can create new network: %s\r\n", canCreateNewNwk);

    if(nwkCap & THR_NWKCAP_CAN_BECOME_ACTIVE_ROUTER)
    {
        isActiveRouter = STRING_TRUE;
    }
    shell_printf("Can become active router: %s\r\n", isActiveRouter);

    if(nwkCap & THR_NWKCAP_IS_FULL_THREAD_DEVICE)
    {
        isFfd = STRING_TRUE;
    }
    shell_printf("Is full Thread device: %s\r\n", isFfd);

    if(nwkCap & THR_NWKCAP_IS_POLLING_END_DEVICE)
    {
        isPolling = STRING_TRUE;
    }
    shell_printf("Is polling end device: %s\r\n", isPolling);
}

/*!*************************************************************************************************
\fn     static void SHELL_PrintBuff(uint8_t *buff, uint32_t length)
\brief  Send to serial interface a data buffer

\param  [in]    buff      Pointer to the buffer to be send to the serial interface
\param  [in]    length    The length of data
***************************************************************************************************/
static void SHELL_PrintBuff
(
    uint8_t *buff,
    uint32_t length
)
{
    uint32_t i;

    for(i = 0; i < length; i++)
    {
        shell_printf("%02X", buff[i]);
    }
}

/*!*************************************************************************************************
\fn     static void SHELL_ReadBuff(char *pValue, uint8_t *pDest, uint32_t length)
\brief  Read hex stream bytes from ASCII.

\param  [in]     pValue       Pointer to start of the ASCII stream
\param  [out]    pDest        Pointer to the start of the output buffer
\param  [in]     length       The length of the ASCII stream
***************************************************************************************************/
static void SHELL_ReadBuff
(
    char *pValue,
    uint8_t *pDest,
    uint32_t length
)
{
    uint32_t iCount;
    uint16_t hexDig;

    for(iCount = 0; iCount < length; iCount++)
    {
        hexDig = NWKU_AsciiToHex((uint8_t *)pValue, sizeof(hexDig));
        pValue += sizeof(hexDig);
        pDest[iCount] = hexDig;
    }
}

/*!*************************************************************************************************
\fn     static bool_t SHELL_ReadIPv6Addr(uint8_t argc, char *argv[],char *pOption,ipAddr_t *pIpAddr)
\brief  Read IPv6 address from a command.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments
\param  [in]    pOption   Pointer to parameter name
\param  [out]   pIpAddr   Pointer to IPv6 output address
***************************************************************************************************/
static bool_t SHELL_ReadIPv6Addr
(
    uint8_t argc,
    char *argv[],
    char *pOption,
    ipAddr_t *pIpAddr
)
{
    char* pValue = NULL;
    bool_t retStatus = TRUE;

    pValue = shell_get_opt(argc, argv, pOption);
    if(pValue)
    {
        if(1 != pton(AF_INET6, pValue, pIpAddr))
        {
            shell_write("Invalid IPv6 address");
            retStatus = FALSE;
        }
    }
    else
    {
        shell_write(" Please set a valid IPv6 address");
        retStatus = FALSE;
    }

    return retStatus;
}
/*!*************************************************************************************************
\private
\fn     void SHELL_SearchAndValidateInterface(uint8_t *pName, uint8_t length)
\brief  This function is used to search and validate the requested interface by the given text.

\param  [in]    pName             Pointer to name
\param  [in]    length            Length of name

\return         ipIfUniqueId_t    Interface ID
***************************************************************************************************/
static ipIfUniqueId_t SHELL_SearchInterfaceByName
(
    uint8_t *pName,
    uint8_t length
)
{
    ipIfUniqueId_t result = gIpIfUndef_c;

    /* Search and verify that the interface exists on the Thread device */
    for(int i = 0; i < NumberOfElements(mThrInterfaces); i++)
    {
        if(FLib_MemCmp(mThrInterfaces[i].pAttrName, pName, length))
        {
            ifHandle_t pIfHandle = IP_IF_GetIfHandle(mThrInterfaces[i].ifId);

            if(pIfHandle != NULL)
            {
                result = mThrInterfaces[i].ifId;
                break;
            }
        }
    }

    return result;
}

/*!*************************************************************************************************
\private
\fn     uint8_t *SHELL_SearchInterfaceById(ipIfUniqueId_t ifID)
\brief  This function is used to search and validate the requested interface by the given interface ID.

\param  [in]    ifID         Interface ID

\return         uint8_t *    Pointer to the name of the interface ID
***************************************************************************************************/
static uint8_t *SHELL_SearchInterfaceById
(
    ipIfUniqueId_t ifID
)
{
    uint8_t *result = NULL;

    /* Search and verify that the interface exists on the Thread device */
    for(int i = 0; i < NumberOfElements(mThrInterfaces); i++)
    {
        if(ifID == mThrInterfaces[i].ifId)
        {
            result = mThrInterfaces[i].pAttrName;
            break;
        }
    }
    return result;
}

/*!*************************************************************************************************
\private
\fn     void SHELL_CoapSend(uint8_t argc, char *argv[])
\brief  This function is used for "coap" command.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_CoapSend
(
    uint8_t argc,
    char *argv[]
)
{
    /*coap <reqtype: CON/NON> <reqcode (GET/POST/PUT/DELETE)> <IP addr dest> <URI path> <payload ASCII>*/
    char *pValue;
    command_ret_t ret = CMD_RET_ASYNC;
    coapMessageTypes_t requestType = gCoapConfirmable_c;
    coapReqRespCodes_t requestCode = gCoapPOST_c;
    coapSession_t* pCoapSession = NULL;
    uint8_t *pCoapPayload = NULL;
    uint32_t coapPayloadSize = 0;
    ipAddr_t destAddr;
    sockaddrStorage_t coapParams = {0};

    NWKU_SetSockAddrInfo(&coapParams, NULL, AF_INET6, COAP_DEFAULT_PORT, 0, gIpIfSlp0_c);

#if COAP_OBSERVE_CLIENT
    if((!strcmp(argv[2], "observe")) && (!strcmp(argv[3], "client")))
    {
        /* Get destination address */
        if(-1 == pton(AF_INET6,argv[4], &destAddr))
        {
            shell_write("Invalid IP address!\n\r");
            return CMD_RET_SUCCESS;
        }

        if(NULL != argv[5])
        {
            FLib_MemSet(&mResource, 0, sizeof(coapUriPath_t));
            FLib_MemSet(&mResourceString, 0, SHELL_MAX_COAP_URI);

            mResource.length = strlen(argv[5]);
            if (mResource.length > SHELL_MAX_COAP_URI)
            {
                mResource.length = SHELL_MAX_COAP_URI;
            }
            FLib_MemCpy(&mResourceString, argv[5], mResource.length);
            mResource.pUriPath = (uint8_t*)&mResourceString;

            if(!strcmp(argv[1], "start"))
            {
                APP_ObserveStartClient(pmMainThreadMsgQueue, gIpIfSlp0_c, &destAddr, &mResource);
            }
            if(!strcmp(argv[1], "stop"))
            {
                APP_ObserveStopClient(&destAddr, &mResource);
            }
            ret = CMD_RET_SUCCESS;
        }
        else
        {
            shell_write("No resource!\n\r");
            ret = CMD_RET_SUCCESS;
        }
    }
    else
#endif
#if COAP_OBSERVE_SERVER
    if((!strcmp(argv[1], "start")) && (!strcmp(argv[2], "observe")) && (!strcmp(argv[3],"server")))
    {
        APP_ObserveStartServer(pmMainThreadMsgQueue, gIpIfSlp0_c);
        SHELL_PrintStatus(gThrStatus_Success_c);
        ret = CMD_RET_SUCCESS;
    }
    else
#endif
    if (argc >= 4)
    {
        if(!strcmp(argv[1], "CON"))
        {
            requestType = gCoapConfirmable_c;
        }
        else if(!strcmp(argv[1], "NON"))
        {
            requestType = gCoapNonConfirmable_c;
            ret = CMD_RET_SUCCESS;
        }

        if(!strcmp(argv[2], "GET"))
        {
            requestCode = gCoapGET_c;
        }
        else if(!strcmp(argv[2], "POST"))
        {
            requestCode = gCoapPOST_c;
        }
        else if(!strcmp(argv[2], "PUT"))
        {
            requestCode = gCoapPUT_c;
        }
        else if(!strcmp(argv[2], "DELETE"))
        {
            requestCode = gCoapDELETE_c;
        }
        else
        {
            SHELL_PrintStatus(gThrStatus_Failed_c);
            return CMD_RET_SUCCESS;
        }

        if(THR_ALL_FFs8 == mCoapInstId)
        {
            mCoapInstId = COAP_CreateInstance(NULL, &coapParams, NULL, 0);
        }

        pCoapSession = COAP_OpenSession(mCoapInstId);

        if(NULL != pCoapSession)
        {
            /* Get destination address */
            pton(AF_INET6,argv[3], &destAddr);

            /* Multicast CON is converted to NON in the stack - return the shell */
            if (IP6_IsMulticastAddr(&destAddr) && (gCoapConfirmable_c == requestType))
            {
                ret = CMD_RET_SUCCESS;
            }

            /* Get URI path */
            if(argc >= 5)
            {
                pValue = argv[4];
                if(pValue)
                {
                    char* pCrtPos = pValue;
                    char* pOption = '\0';
                    uint8_t size = 0;
                    uint8_t option = COAP_URI_PATH_OPTION;

                    while(*pCrtPos != '\0')
                    {
                        if((*pCrtPos == '/') && (option == COAP_URI_PATH_OPTION))
                        {
                            if (size != 0)
                            {
                                COAP_AddOptionToList(pCoapSession, option, (uint8_t *)pOption, size);

                                /* Reset counters */
                                size = 0;
                            }
                            pOption = pCrtPos+1;
                        }
                        else if (*pCrtPos == '?')
                        {
                            /* start of URI-query, end of URI-path */
                            COAP_AddOptionToList(pCoapSession, option, (uint8_t *)pOption, size);
                            /* Reset counters */
                            size = 0;
                            option = COAP_URI_QUERY_OPTION;
                            pOption = pCrtPos+1;

                        }
                         else if ((*pCrtPos == '&') && (option == COAP_URI_QUERY_OPTION))
                        {
                            option = COAP_URI_QUERY_OPTION;
                            COAP_AddOptionToList(pCoapSession, COAP_URI_QUERY_OPTION, (uint8_t *)pOption, size);
                            /* Reset counters */
                            size = 0;
                            pOption = pCrtPos+1;
                        }
                        else
                        {
                            size++;
                        }
                        pCrtPos++;
                    }

                    if (size != 0)
                    {
                        COAP_AddOptionToList(pCoapSession, option, (uint8_t *)pOption, size);
                    }
                }
            }

            /* Get payload */
            if(argc >= 6)
            {
                pValue = argv[5];

                if(pValue)
                {
                    /* Check if payload is string or hex */
                    if (FLib_MemCmp(argv[5], SHELL_HEX_PREFIX_LC, SHELL_HEX_PREFIX_LEN)
                    || FLib_MemCmp(argv[5], SHELL_HEX_PREFIX_UC, SHELL_HEX_PREFIX_LEN))
                    {
                        /* payload is hex stream */
                        coapPayloadSize = (strlen(argv[5]) - SHELL_HEX_PREFIX_LEN)/2;
                        pCoapPayload = MEM_BufferAlloc(coapPayloadSize);
                        if(pCoapPayload)
                        {
                            SHELL_ReadBuff(argv[5]+SHELL_HEX_PREFIX_LEN, pCoapPayload, coapPayloadSize);
                        }
                    }
                    else
                    {
                        /* payload is string */
                        coapPayloadSize = strlen(pValue);

                        if(!strcmp(argv[5], "rgb"))
                        {
                            coapPayloadSize += strlen(argv[6]) + strlen(argv[7]) + strlen(argv[8]) + 4;
                        }
                        pCoapPayload = MEM_BufferAlloc(coapPayloadSize);

                        if(pCoapPayload)
                        {
                            FLib_MemSet(pCoapPayload, 0, coapPayloadSize);
                            FLib_MemCpy(pCoapPayload, pValue, coapPayloadSize);
                        }
                    }
                }
            }
            /* Send CoAP message */
            FLib_MemCpy(&pCoapSession->remoteAddrStorage.ss_addr, &destAddr, sizeof(ipAddr_t));
            pCoapSession->code = requestCode;
            pCoapSession->msgType = requestType;
            COAP_SetCallback(pCoapSession, SHELL_CoapAckReceive);
            COAP_Send(pCoapSession, gCoapMsgTypeUseSessionValues_c, pCoapPayload, coapPayloadSize);

            MEM_BufferFree(pCoapPayload);
        }
    }
    else
    {
        shell_write("Invalid number of parameters!\n\r");
        ret = CMD_RET_SUCCESS;
    }

    return ret;
}

/*!*************************************************************************************************
\private
\fn     void SHELL_CoapAckReceive(coapSessionStatus_t sessionStatus, uint8_t *pData,
                                  coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for the COAP ACK message.

\param  [in]    sessionStatus    Status of the session
\param  [in]    pData            Pointer to data
\param  [in]    pSession         Pointer to CoAP session
\param  [in]    dataLen          Length of data
***************************************************************************************************/
static void SHELL_CoapAckReceive
(
    coapSessionStatus_t sessionStatus,
    uint8_t *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    char remoteAddrStr[INET6_ADDRSTRLEN];

    if(gCoapSuccess_c == sessionStatus)
    {
        ntop(AF_INET6, (ipAddr_t*)&pSession->remoteAddrStorage.ss_addr, remoteAddrStr, INET6_ADDRSTRLEN);
        /* coap rsp from <IP addr>: <ACK> <rspcode: X.XX> <payload ASCII> */
        shell_printf("coap rsp from ");
        shell_printf(remoteAddrStr);

        if (gCoapAcknowledgement_c == pSession->msgType)
        {
            shell_printf(" ACK ");
        }
        /* Print Code */
        shell_writeDec(pSession->code);

        /* Print options - for now just Location-Path, more can be added */
        bool_t first = TRUE;
        coapOption_t  *pOptDetails = ListGetHeadMsg(&pSession->pRxOptionList);
        while (pOptDetails)
        {
            switch (pOptDetails->optName)
            {
                case COAP_LOCATION_PATH_OPTION:
                {
                    if (first)
                    {
                        shell_printf("\r\nLocation-Path: ");
                        first = FALSE;
                    }
                    shell_printf("/");
                    shell_writeN((char*)pOptDetails->optValue, pOptDetails->optValueLen);
                    break;
                }
                default:
                {
                    break;
                }
            }
            pOptDetails = ListGetNextMsg(pOptDetails);
        }
        shell_printf("\r\n");
        /* Check if payload is ascii */
        if(0 != dataLen)
        {
            uint32_t index = 0;
            uint8_t* pCrtChar = (uint8_t*)pData;

            while ((*pCrtChar < 127) && (index < dataLen))
            {
                pCrtChar ++;
                index ++;
            }
            /* if all characters ar ascii, print as string */
            if (index == dataLen)
            {
                shell_write(" ");
                shell_writeN((char*)pData, dataLen);
            }
            /* else, print as hex */
            else
            {
                shell_printf(" 0x");
                SHELL_PrintBuff(pData, dataLen);
            }
        }
    }
    else
    {
        shell_printf("No response received!");
    }
    shell_cmd_finished();
}

/*!*************************************************************************************************
\private
\fn     static int8_t  SHELL_Ping(uint8_t argc, char *argv[])
\brief  This function is used for "ping" shell command.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_Ping
(
    uint8_t argc,
    char *argv[]
)
{
    ipPktInfo_t *pIpPktInfo;
    command_ret_t ret = CMD_RET_SUCCESS;
    uint8_t i, ap = AF_UNSPEC, hopLimit;
    uint16_t count;
    char *pValue;
    bool_t validDstIpAddr = FALSE;

    /* Stop infinite ping */
    if(argc == 0)
    {
        if(mContinuousPing)
        {
            SHELL_Resume();
        }
    }
    /* Check number of arguments according to the shellComm table */
    else
    {
        /* Reset the size of the ping */
        mPingSize = PING_PAYLOAD_DEFAULT_SIZE;
        mPingTimeoutMs = DEFAULT_TIMEOUT;
        count = 0; /* infinite ping */

        /* Get option value for -I */
        pValue = shell_get_opt(argc, argv, "-I");
        if(pValue && !mContinuousPing)
        {
            mPingInterfaceId = SHELL_SearchInterfaceByName((uint8_t *)pValue, strlen(pValue));

            if(mPingInterfaceId == gIpIfUndef_c)
            {
                shell_write("Interface doesn't exist or hasn't been started!");
                return CMD_RET_SUCCESS;
            }
        }

        /* Get option value for -s */
        pValue = shell_get_opt(argc, argv, "-s");
        if(pValue)
        {
            mPingSize = (uint16_t)NWKU_atoi(pValue);
        }

        /* Get option value for -i */
        pValue = shell_get_opt(argc, argv, "-i");
        if(pValue)
        {
            mPingTimeoutMs = (uint16_t)NWKU_atoi(pValue);

            if (mPingTimeoutMs < SHELL_PING_MIN_TIMEOUT)
            {
                mPingTimeoutMs = SHELL_PING_MIN_TIMEOUT;
            }
        }

        /* Get option value for -c */
        pValue = shell_get_opt(argc, argv, "-c");
        if(pValue)
        {
            count = (uint16_t)NWKU_atoi(pValue);
        }

        /* Get option value for -h */
        pValue = shell_get_opt(argc, argv, "-h");
        if(pValue)
        {
            hopLimit = (uint16_t)NWKU_atoi(pValue);
            if (hopLimit < ICMP_DEFAULT_HOP_LIMIT)
            {
                mPingHopLimit = hopLimit;
            }
        }
        else
        {
            /* Use default value if no options available */
            mPingHopLimit = ICMP_DEFAULT_HOP_LIMIT;
        }

        /* Get option value for -src */
        pValue = shell_get_opt(argc, argv, "-S");
        if(pValue)
        {
            /* Use MEM_BufferAllocForever() in case the ping takes more than 2 minutes */
            pSrcIpAddr = MEM_BufferAllocForever(sizeof(ipAddr_t), AppPoolId_d);
            pton(AF_INET6, pValue, pSrcIpAddr);
        }

        /* Find "-t" option */
        for (i = 1; i < argc; i++)
        {
            if (FLib_MemCmp(argv[i], "-t", 2))
            {
                count = THR_ALL_FFs16;
                break;
            }
        }

        /* Check if the destination IPv4/IPv6 address is valid */
        for (i = 1; i < argc; i++)
        {
            /* Verify IP address (v4 or v6) */
            uint8_t *pText = (uint8_t *)argv[i];

            while (*pText != '\0')
            {
                if (*pText == '.')
                {
                    ap = AF_INET;
                    break;
                }
                if (*pText == ':')
                {
                    ap = AF_INET6;
                    break;
                }
                pText++;
            }

            if ((ap != AF_UNSPEC) && (1 == pton(ap, argv[i], &mDstIpAddr)))
            {
                validDstIpAddr = TRUE;
                break;
            }
        }

        if (!validDstIpAddr)
        {
            shell_write("Invalid destination IP address");
            return CMD_RET_FAILURE;
        }

        /* Pause SHELL command parsing */
        ret = CMD_RET_ASYNC;
        /* Check number of counts */
        mContinuousPing = TRUE;
        mPingCounter = THR_ALL_FFs16;

        if(count == 0)
        {
            mPingCounter = 3;
        }
        else if(count == 1)
        {
            mContinuousPing = FALSE;

        }
        else if(count != THR_ALL_FFs16)
        {
            mPingCounter = count - 1;
        }
        /* Create Ping packet */
        pIpPktInfo = PING_CreatePktInfo(&mDstIpAddr, mPingSize);

        if(NULL != pIpPktInfo)
        {
            PING_SendPing(pIpPktInfo, pSrcIpAddr);

            if(gTmrInvalidTimerID_c == pingTimerID)
            {
                /* Allocate ping timer */
                pingTimerID = TMR_AllocateTimer();

                if(pingTimerID != gTmrInvalidTimerID_c)
                {
                    /* Start timer */
                    TMR_StartSingleShotTimer(pingTimerID, mPingTimeoutMs, PING_TimerCallback, NULL);

                    shell_printf("Pinging %s with %u bytes of data:\r\n", argv[i], mPingSize);
                }
                else
                {
                    shell_write("Timer cannot be allocated!");
                    ret = CMD_RET_SUCCESS;
                }
            }
            else
            {
                shell_write("Timer already allocated!");
                ret = CMD_RET_SUCCESS;
            }
        }
        else
        {
            shell_write("No memory for creating the ping packet");
            ret = CMD_RET_SUCCESS;
        }
    } /* Correct number of arguments */

    return ret;
}

/*!*************************************************************************************************
\private
\fn     ipPktInfo_t *PING_CreatePktInfo(ipAddr_t *pDstAddr, uint32_t payloadLen)
\brief  This function is used to create the packet info structure needed by ICMP ping.

\param  [in]    pDstAddr         Pointer to the destination IP address
\param  [in]    payloadLen       The size of the payload

\return         ipPktInfo_t      Pointer to a packet info structure
***************************************************************************************************/
static ipPktInfo_t *PING_CreatePktInfo
(
    ipAddr_t *pDstAddr,
    uint32_t payloadLen
)
{
    uint16_t echoId = PING_ID, idx, iPayload;
    ipPktInfo_t *pIpPktInfo = NULL;
    uint8_t *pPayload;

    pIpPktInfo = NWKU_CreateIpPktInfo();

    if(NULL != pIpPktInfo)
    {
        pIpPktInfo->pIpDstAddr = &pIpPktInfo->ipDstAddr;
        pIpPktInfo->pIpSrcAddr = &pIpPktInfo->ipSrcAddr;

        /* Allocate and populate pIpPktInfo->pNwkBuff using Echo request payload and
           echo request identifier and sequence number */
        pIpPktInfo->pNwkBuff = NWKU_CreateNwkBuffer(payloadLen + sizeof(echoId) + sizeof(mDefaultSeqNb));

        if(NULL != pIpPktInfo->pNwkBuff)
        {
            /* Allocate and populate pIpPktInfo->pIpDstAddr */
            IP_AddrCopy(pIpPktInfo->pIpDstAddr, pDstAddr);

            /* Determine IP source address based on IP destination address */
            if(IP_IsAddrIPv6(pIpPktInfo->pIpDstAddr))
            {
#if IP_IP6_ENABLE
              ipAddr_t *pSrcAddr = IP_IF_SelSrcAddr6(mPingInterfaceId, pIpPktInfo->pIpDstAddr);
              IP_AddrCopy(pIpPktInfo->pIpSrcAddr, pSrcAddr);
#endif /* IP_IP6_ENABLE */
            }
            else
            {
#if IP_IP4_ENABLE
              NWKU_ConvertIp4Addr(IP_IF_SelSrcAddr4(pIpPktInfo->pIpDstAddr, pIpPktInfo->ipPktOptions.ifHandle),
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

            for(idx = 0; idx < payloadLen; idx++, iPayload++)
            {
                if(iPayload > (PING_PAYLOAD_END - PING_PAYLOAD_START))
                {
                    iPayload = 0;
                }
                pPayload[idx] = iPayload + PING_PAYLOAD_START;
            }

            /* Set the interface handler */
            if(mPingInterfaceId != gIpIfUndef_c)
            {
                ifHandle_t pIfHandle = IP_IF_GetIfHandle(mPingInterfaceId);

                if(pIfHandle != NULL)
                {
                    /* Add the found interface handle to the packet */
                    pIpPktInfo->ipPktOptions.ifHandle = (void *)pIfHandle;
                }
                else
                {
                    NWKU_FreeIpPktInfo(&pIpPktInfo);
                    mPingInterfaceId = gIpIfUndef_c;
                }
            }
        }
        else
        {
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }

    return pIpPktInfo;
}

/*!*************************************************************************************************
\private
\fn     void PING_EchoReplyReceive(uint8_t *pParam)
\brief  Interface function for the user application. It handles a received Ping Echo Reply message.

\param  [in]    pParam    Pointer to the packet information structure.
***************************************************************************************************/
static void PING_EchoReplyReceive
(
    uint8_t *pParam
)
{
    uint16_t echoId, seqNb;
    uint32_t payloadLen;
    uint64_t tempTimestamp = TMR_GetTimestamp();
    ipPktInfo_t *pIpPktInfo = (ipPktInfo_t *)pParam;

    /* Reply from desired IP address */
    if(IP_IsAddrEqual(&mDstIpAddr, pIpPktInfo->pIpSrcAddr) || IP6_IsMulticastAddr(&mDstIpAddr))
    {
        /* Get first the echo request identifier */
        echoId = ntohas(pIpPktInfo->pNextProt);

        if(echoId == PING_ID)
        {
            /* Get the echo request sequence number */
            seqNb = ntohas(pIpPktInfo->pNextProt + sizeof(echoId));

            if(seqNb == mDefaultSeqNb - 1)
            {
                /* Get payload length from the ICMP packet.
                 * The ping payload is with an offset of 4 */
                payloadLen = pIpPktInfo->prot.nextProtLen - PING_HEADER_SIZE;

                if(IP_IsAddrIPv6(pIpPktInfo->pIpSrcAddr))
                {
                    ntop(AF_INET6, pIpPktInfo->pIpSrcAddr, mAddrStr, INET6_ADDRSTRLEN);
                }
                else
                {
                    ntop(AF_INET, pIpPktInfo->pIpSrcAddr, mAddrStr, INET_ADDRSTRLEN);
                }

                /* Free the input pIpPktInfo  */
                NWKU_FreeIpPktInfo(&pIpPktInfo);

                shell_write("Reply from ");
                shell_write(mAddrStr);
                shell_write(": bytes=");
                shell_writeDec(payloadLen);
                shell_write(" time=");
                tempTimestamp -= pingTimeStamp;
                tempTimestamp /= 1000;
                shell_writeDec(tempTimestamp);
                shell_write("ms");
                SHELL_NEWLINE();

                /* Stop timer */
                if(pingTimerID != gTmrInvalidTimerID_c)
                {
                    TMR_StopTimer(pingTimerID);
                }

                /* Continuous ping: restart */
                if(mContinuousPing)
                {
                    /* Start timer */
                    if(mDelayTimerID == gTmrInvalidTimerID_c)
                    {
                        mDelayTimerID = TMR_AllocateTimer();
                    }
                    TMR_StartSingleShotTimer(mDelayTimerID, PING_DELAY, PING_RetransmitCallback, NULL);
                }
                else
                {
#if ICMP_STATISTICS_ENABLED
                    SHELL_PrintPingStatistics();
#endif
                    SHELL_Resume();
                    mPingInterfaceId = gIpIfUndef_c;
                }
            }
            else
            {
                shell_write("Reply sequence number not matching\n\r");
                /* Free the input pIpPktInfo */
                NWKU_FreeIpPktInfo(&pIpPktInfo);
            }
        }
        else
        {
            shell_write("Reply PING ID not matching\n\r");
            /* Free the input pIpPktInfo */
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }
    else
    {
        shell_write("Reply IP source address not matching\n\r");
        /* Free the input pIpPktInfo */
        NWKU_FreeIpPktInfo(&pIpPktInfo);
    }
}

/*!*************************************************************************************************
\private
\fn     static void PING_TimerCallback(void *param)
\brief  This function is used as a asynchronous callback for the ping timeout.

\param  [in]    param    Not used
***************************************************************************************************/
static void PING_TimerCallback
(
    void *param
)
{
    (void)NWKU_SendMsg(PING_HandleTimerCallback, NULL, pmMainThreadMsgQueue);
}

/*!*************************************************************************************************
\private
\fn     static void PING_HandleTimerCallback(uint8_t *param)
\brief  This function is used to handle the ping timeout.

\param  [in]    param    Not used
***************************************************************************************************/
static void PING_HandleTimerCallback
(
    uint8_t *param
)
{
    /* Ping reply was not received */
    shell_write("Request timed out.");
    SHELL_NEWLINE();

    if(mContinuousPing)
    {
        ipPktInfo_t *pPingIpPktInfo;

        /* Create Ping packet */
        pPingIpPktInfo = PING_CreatePktInfo(&mDstIpAddr, mPingSize);

        if(NULL != pPingIpPktInfo)
        {
            if(pSrcIpAddr)
            {
                IP_AddrCopy(pPingIpPktInfo->pIpSrcAddr, pSrcIpAddr);
            }

            /* Send packet to ICMP for transmission */
            if(IP_IsAddrIPv6(&mDstIpAddr))
            {
               ICMP_Send(pPingIpPktInfo, gIcmp6TypeEchoRequest_c, ICMP_CODE_DEFAULT);
            }
            else
            {
               ICMP_Send(pPingIpPktInfo, gIcmp4TypeEchoRequest_c, ICMP_CODE_DEFAULT);
            }

            if(mPingCounter > 0 && mPingCounter != THR_ALL_FFs16)
            {
                mPingCounter--;
            }

            /* Counter have reached 0: stop pinging */
            if(mPingCounter == 0)
            {
                mContinuousPing = FALSE;
                mPingInterfaceId = gIpIfUndef_c;
            }

            /* Get timestamp */
            pingTimeStamp = TMR_GetTimestamp();

            if(pingTimerID != gTmrInvalidTimerID_c)
            {
                /* Start timer */
                TMR_StartSingleShotTimer(pingTimerID, mPingTimeoutMs, PING_TimerCallback, NULL);
            }
            else
            {
                shell_write("Invalid Timer ID!");
            }
        }
        else
        {
            shell_write("No memory for creating the ping packet");
            SHELL_Resume();
        }
    }
    else
    {
#if ICMP_STATISTICS_ENABLED
        SHELL_PrintPingStatistics();
#endif
        SHELL_Resume();
    }
}

/*!*************************************************************************************************
\private
\fn     void PING_RetransmitCallback(void *param)
\brief  This function is used as a asynchronous callback for the 500ms timeout.

\param  [in]    param    Not used
***************************************************************************************************/
static void PING_RetransmitCallback
(
    void *param
)
{
    (void)NWKU_SendMsg(PING_RetransmitHandle, NULL, pmMainThreadMsgQueue);
}

/*!*************************************************************************************************
\private
\fn     void PING_RetransmitHandle(uint8_t *param)
\brief  This function is handles the 500ms timeout. This timer is used to send another Ping.Request
        after a Ping.Reply packet was received.

\param  [in]    param    Not used
***************************************************************************************************/
static void PING_RetransmitHandle
(
    uint8_t *param
)
{
    ipPktInfo_t *pPingIpPktInfo;

    /* Create Ping packet */
    pPingIpPktInfo = PING_CreatePktInfo(&mDstIpAddr, mPingSize);

    if(NULL != pPingIpPktInfo)
    {
        PING_SendPing(pPingIpPktInfo, pSrcIpAddr);

        /* Start timer */
        TMR_StartSingleShotTimer(pingTimerID, mPingTimeoutMs, PING_TimerCallback, NULL);

        if(mPingCounter > 0 && mPingCounter != THR_ALL_FFs16)
        {
            mPingCounter--;
        }

        /* Counter have reached 0: stop pinging */
        if(mPingCounter == 0)
        {
            mContinuousPing = FALSE;
            mPingInterfaceId = gIpIfUndef_c;
            MEM_BufferFree(pSrcIpAddr);
            pSrcIpAddr = NULL;
        }

        TMR_FreeTimer(mDelayTimerID);
        mDelayTimerID = gTmrInvalidTimerID_c;
    }
    else
    {
        mContinuousPing = FALSE;
        mPingInterfaceId = gIpIfUndef_c;
        MEM_BufferFree(pSrcIpAddr);
        pSrcIpAddr = NULL;

        TMR_FreeTimer(mDelayTimerID);
        mDelayTimerID = gTmrInvalidTimerID_c;

        TMR_FreeTimer(pingTimerID);
        pingTimerID = gTmrInvalidTimerID_c;

        shell_write("No memory for creating the ping packet");
        SHELL_Resume();
    }
}

/*!*************************************************************************************************
\private
\fn     static void PING_SendPing(ipPktInfo_t *pPingIpPktInfo, ipAddr_t *pSrcIpAddr)
\brief  This function is used to send the ping.

\param  [in]    pPingIpPktInfo    Pointer to the ping IP packet information
\param  [in]    pSrcIpAddr        Pointer to the source IP address
***************************************************************************************************/
static void PING_SendPing
(
    ipPktInfo_t *pPingIpPktInfo,
    ipAddr_t *pSrcIpAddr
)
{
    /* If we have a specified source address: set it */
    if(pSrcIpAddr)
    {
       IP_AddrCopy(pPingIpPktInfo->pIpSrcAddr, pSrcIpAddr);
    }

    /* Send packet to ICMP for transmission */
    if(IP_IsAddrIPv6(&mDstIpAddr))
    {
       ICMP_Send(pPingIpPktInfo, gIcmp6TypeEchoRequest_c, ICMP_CODE_DEFAULT);
    }
    else
    {
       ICMP_Send(pPingIpPktInfo, gIcmp4TypeEchoRequest_c, ICMP_CODE_DEFAULT);
    }

    /* Get timestamp */
    pingTimeStamp = TMR_GetTimestamp();
}

#if ICMP_STATISTICS_ENABLED
/*!*************************************************************************************************
\private
\fn     static void SHELL_PrintPingStatistics(void)
\brief  This function prints ping statistics.
***************************************************************************************************/
static void SHELL_PrintPingStatistics
(
    void
)
{
    uint32_t sent = 0, received = 0, lost;
    char aLoss[5], intPart[3], fractPart[2];

    if(IP_IsAddrIPv6(&mDstIpAddr))
    {
#if IP_IP6_ENABLE
        SHELL_NEWLINE();

        sent = ICMP_GetStatistics(gIcmpStatsProtIcmp6_c, gIcmpStatsDirTransmit_c, gIcmpStatsTypeEchoRequest6_c);
        received = ICMP_GetStatistics(gIcmpStatsProtIcmp6_c, gIcmpStatsDirReceive_c, gIcmpStatsTypeEchoReply6_c);

        if(sent != 0)
        {
            shell_printf("Ping statistics for %s:", ntop(AF_INET6, &mDstIpAddr, mAddrStr, INET6_ADDRSTRLEN));
        }
#endif /* IP_IP6_ENABLE */
    }
    else
    {
#if IP_IP4_ENABLE
        SHELL_NEWLINE();

        sent = ICMP_GetStatistics(gIcmpStatsProtIcmp4_c, gIcmpStatsDirTransmit_c, gIcmpStatsTypeEchoRequest4_c);
        received = ICMP_GetStatistics(gIcmpStatsProtIcmp4_c, gIcmpStatsDirReceive_c, gIcmpStatsTypeEchoReply4_c);

        if(sent != 0)
        {
            shell_printf("Ping statistics for %s:", ntop(AF_INET, &mDstIpAddr, mAddrStr, INET_ADDRSTRLEN));
        }
#endif /* IP_IP4_ENABLE */
    }

    if((IP_IP4_ENABLE && !IP_IsAddrIPv6(&mDstIpAddr)) || (IP_IP6_ENABLE && IP_IsAddrIPv6(&mDstIpAddr)))
    {
        SHELL_NEWLINE();

        lost = sent - received;

        if(sent != 0)
        {
            if(sent == lost)
            {
                sprintf(aLoss, "%s", "100");
            }
            else if(lost == 0)
            {
                sprintf(aLoss, "%s", "0");
            }
            else
            {
                float_division(intPart, fractPart, sent, lost);

                if (strcmp(fractPart, "00") == 0)
                {
                    sprintf(aLoss, "%s", "0");
                }
                else
                {
                    sprintf(aLoss, "%s.%s", intPart, fractPart);
                }
            }

            shell_printf("Packets: Sent = %d, Received = %d, Lost = %d (%s%% loss)", sent, received, lost, aLoss);
        }

        ICMP_ResetStatistics();
    }
}
#endif /* ICMP_STATISTICS_ENABLED */

/*!*************************************************************************************************
\fn     static int8_t SHELL_MulticastGroups(uint8_t argc, char *argv[])
\brief  This function is used for checking the joined multicast groups or to join a new one.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_MulticastGroups
(
    uint8_t argc,
    char *argv[]
)
{
    char groupAddrStr[INET6_ADDRSTRLEN];
    ipAddr_t groupAddr = {0};
    command_ret_t ret = CMD_RET_SUCCESS;
    thrStatus_t retStatus = gThrStatus_Failed_c;
    uint32_t *pTimeout = NULL;
    uint32_t timeout = 0;

    ipIfStruct_t *pIfTable = NULL;
    uint32_t intTblSize = IP_IF_GetInterfaceTableSize();

    ip6MulticastAddrData_t *pMulticastAddrTable = NULL;
    uint32_t mcastAddrTblSize = IP_IF_GetMcastAddrTableSize();

    if(!strcmp(argv[1], "show"))
    {
        for(uint32_t ifCount = 0; ifCount < intTblSize; ifCount++)
        {
            pIfTable = IP_IF_GetInterfaceTableEntry(ifCount);

            if(pIfTable->ifUniqueId != gIpIfUndef_c)
            {
                shell_printf("\r\nInterface %d:\r\n", ifCount);
                for(uint32_t iCount = 0; iCount < mcastAddrTblSize; iCount++)
                {
                    pMulticastAddrTable = IP_IF_GetMcastAddrTableEntry(iCount);

                    if((NULL != pMulticastAddrTable) &&
                    (pMulticastAddrTable->ifUniqueId == pIfTable->ifUniqueId))
                    {
                        ntop(AF_INET6, &pMulticastAddrTable->mCastAddr, groupAddrStr, INET6_ADDRSTRLEN);
                        shell_printf("Multicast Group: ");
                        shell_printf(groupAddrStr);
                        shell_printf("\r\n");
                    }
                }
            }
        }
    }
    else if((!strcmp(argv[1], "add")) || (!strcmp(argv[1], "leave")) || (!strcmp(argv[1], "commissioner")))
    {
        if((argv[2] != NULL) && (argv[3] != NULL))
        {
            if(1 != pton(AF_INET6, argv[2], &groupAddr))
            {
                shell_write("Invalid multicast group address");
            }
            else
            {
                ipIfUniqueId_t ifId = IP_IF_GetIfIdByIndex(NWKU_atoi(argv[3]));
                if(argv[4] != NULL)
                {
                    if (!strcmp(argv[4], "permanent"))
                    {
                        timeout = THR_ALL_FFs32;
                    }
                    else
                    {
                        timeout = NWKU_atoi((char*)argv[4]);
                    }
                    pTimeout = &timeout;

                }

                if(ifId != gIpIfUndef_c)
                {
                    if(!strcmp(argv[1], "add"))
                    {
                        switch (ifId)
                        {
                            case gIpIfSlp0_c:
                            case gIpIfSlp1_c:
                                retStatus = THR_RegisterMulticastGroup(threadInstanceID, &groupAddr,
                                                                       pTimeout);

                                break;

                            default:
                                if (gIpOk_c == IP_IF_AddMulticastGroup6(ifId, &groupAddr))
                                {
                                    retStatus = gThrStatus_Success_c;
                                }
                                break;
                        }
                    }
                    else if (!strcmp(argv[1], "leave"))
                    {
                        switch (ifId)
                        {
                            case gIpIfSlp0_c:
                            case gIpIfSlp1_c:
                                retStatus = THR_UnregisterMulticastGroup(threadInstanceID, &groupAddr);

                                break;

                            default:
                                if (gIpOk_c == IP_IF_LeaveMulticastGroup6(ifId, &groupAddr))
                                {
                                    retStatus = gThrStatus_Success_c;
                                }
                                break;
                         }
                    }

                    SHELL_PrintStatus(retStatus);
                }
                else
                {
                    shell_write("Invalid interface ID");
                }
            }
        }
        else
        {
            shell_write("Type a multicast group address and an interface ID");
        }
    }

    return ret;
}

#if SOCK_DEMO
/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_Socket(uint8_t argc, char *argv[])
\brief  This function is used for sockets shell command.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_Socket
(
    uint8_t argc,
    char *argv[]
)
{
    command_ret_t ret = CMD_RET_ASYNC;
    appSockCmdParams_t *pAppSockCmdParams = MEM_BufferAlloc(sizeof(appSockCmdParams_t));

    /* Stop any socket pending commands */
    if((argc == 0) || (pAppSockCmdParams == NULL))
    {
        SHELL_Resume();
        ret = CMD_RET_SUCCESS;
        return ret;
    }

    FLib_MemSet(pAppSockCmdParams, 0, sizeof(appSockCmdParams_t));

    /* socket open */
    if(!strcmp(argv[1], "open"))
    {
        /* Check number of arguments according to the shellComm table */
        if (argc == 5)
        {
            uint32_t result = 1;

            /* socket open udp */
            if(!strcmp(argv[2], "udp"))
            {
                pAppSockCmdParams->appSocketsCmd = gSockOpenUdp;
                pAppSockCmdParams->appSocketsTrans = gSockUdp;
            }
            else if(!strcmp(argv[2], "tcp"))
            {
                pAppSockCmdParams->appSocketsCmd = gSockOpenTcp;
                pAppSockCmdParams->appSocketsTrans = gSockTcp;
            }
            else
            {
                shell_write("Protocol type is unknown");
                SHELL_NEWLINE();
                ret = CMD_RET_FAILURE;
                result = -1;
            }
            if (result != -1)
            {
                /* Set local information */
                pAppSockCmdParams->ipVersion = AF_INET6;

                if(pAppSockCmdParams->ipVersion == AF_INET6)
                {
                    pAppSockCmdParams->sin6_port = NWKU_atoi((char const*)argv[4]);
                    result = pton(AF_INET6, argv[3], &pAppSockCmdParams->sin6_addr);
                }
                else
                {
                    pAppSockCmdParams->sin_port = NWKU_atoi((char const*)argv[4]);
                    result = pton(AF_INET, argv[3], &pAppSockCmdParams->sin_addr);
                }

                if (result == -1)
                {
                    shell_write("IP address has a wrong format");
                    SHELL_NEWLINE();
                    ret = CMD_RET_FAILURE;
                }
            }
        }
        else
        {
            ret = CMD_RET_FAILURE;
            shell_write("Wrong number of parameters");
        }
    }
    /* socket close */
    else if(!strcmp(argv[1], "close"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockClose;
        pAppSockCmdParams->sock32 = NWKU_atoi((const char*)argv[2]);
    }
    /* socket shutdown */
    else if(!strcmp(argv[1], "shutdown"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockShutdown;
        pAppSockCmdParams->sock32 = NWKU_atoi((const char*)argv[2]);
    }
    /* socket send */
    else if(!strcmp(argv[1], "send"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockSend;

        /* Check number of arguments according to the shellComm table */
        if(argc == 4)
        {
            /* Get socket id */
            pAppSockCmdParams->sock32 = NWKU_atoi((const char *)argv[2]);
            pAppSockCmdParams->dataLen = strlen((char const *)argv[3]);
            pAppSockCmdParams->pData = MEM_BufferAlloc(pAppSockCmdParams->dataLen);

            if(NULL != pAppSockCmdParams->pData)
            {
                FLib_MemCpy(pAppSockCmdParams->pData,argv[3],pAppSockCmdParams->dataLen);
            }
            else
            {
                ret = CMD_RET_FAILURE;
                shell_write("No memory for creating the command");
            }
        }
        else
        {
            ret = CMD_RET_FAILURE;
            shell_write("Wrong number of parameters");
        }
    }
    /* socket connect */
    else if(!strcmp(argv[1], "connect"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockConnect;

        /* Check number of arguments according to the shellComm table */
        if(argc == 5)
        {
            uint32_t result = -1;

            /* Get socket ID from command */
            pAppSockCmdParams->sock32 = NWKU_atoi((char const*)argv[2]);

            /* Set local information */
            pAppSockCmdParams->ipVersion = AF_INET6;
            if(pAppSockCmdParams->ipVersion == AF_INET6)
            {
                pAppSockCmdParams->sin6_port = NWKU_atoi((char const*)argv[4]);
                result = pton(AF_INET6, argv[3], &pAppSockCmdParams->sin6_addr);
            }
            else
            {
                pAppSockCmdParams->sin_port = NWKU_atoi((char const*)argv[4]);
                result = pton(AF_INET, argv[3], &pAppSockCmdParams->sin_addr);
            }

            if (result == -1)
            {
                shell_write("IP address has a wrong format");
                SHELL_NEWLINE();
                ret = CMD_RET_FAILURE;
            }
        }
        else
        {
            ret = CMD_RET_FAILURE;
            shell_write("Wrong number of parameters");
        }
    }
    /* socket accept */
    else if(!strcmp(argv[1], "accept"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockAccept;

        /* Check number of arguments according to the shellComm table */
        if(argc == 3)
        {
            pAppSockCmdParams->sock32 = NWKU_atoi((const char*)argv[2]);
        }
        else
        {
            shell_write("Wrong number of parameters");
            ret = CMD_RET_FAILURE;
        }
    }
    else if(!strcmp(argv[1], "bench"))
    {
        if(argc == 4)
        {
            if(!strcmp(argv[2], "udp"))
            {
                pAppSockCmdParams->appSocketsTrans = gSockUdp;
                if(!strcmp(argv[3], "rx"))
                {
                    pAppSockCmdParams->appSocketsCmd = gSockBenchRxUdp;
                }
                else if(!strcmp(argv[3], "tx"))
                {
                    pAppSockCmdParams->appSocketsCmd = gSockBenchTxUdp;
                }
            }
            else if(!strcmp(argv[2], "tcp"))
            {
                pAppSockCmdParams->appSocketsTrans = gSockTcp;
                if(!strcmp(argv[3], "rx"))
                {
                    pAppSockCmdParams->appSocketsCmd = gSockBenchRxTcp;
                }
                else if(!strcmp(argv[3], "tx"))
                {
                    pAppSockCmdParams->appSocketsCmd = gSockBenchTxTcp;
                }
            }
        }
        else
        {
            ret = CMD_RET_FAILURE;
            shell_write("Wrong number of parameters");
        }
    }
    else
    {
        shell_write("Wrong command syntax");
        ret = CMD_RET_FAILURE;
    }

    if(ret == CMD_RET_ASYNC)
    {
        if(FALSE == App_SocketSendAsync(pAppSockCmdParams))
        {
            ret = CMD_RET_FAILURE;
            shell_write("No memory for creating the command");
            MEM_BufferFree(pAppSockCmdParams);
        }
    }
    else
    {
        MEM_BufferFree(pAppSockCmdParams);
    }

    return ret;
}
#endif /* SOCK_DEMO */

#if UDP_ECHO_PROTOCOL
/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_EchoUdp(uint8_t argc, char *argv[])
\brief  This function is used to send packets with echo protocol.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_EchoUdp
(
    uint8_t argc,
    char *argv[]
)
{
    return ECHO_ShellUdp(argc, argv);
}
#endif

#if DNS_ENABLED
/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_SendDns(uint8_t argc, char *argv[])
\brief  This function is used to send a DNS request.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_SendDns
(
    uint8_t argc,
    char *argv[]
)
{
    int8_t status = CMD_RET_FAILURE;

    if (strcmp(argv[1], "nddata") == 0)
    {
        THR_DNS_ND_DataReq(threadInstanceID, SHELL_DnsNdDataService);
        status = CMD_RET_ASYNC;
    }
    else
    {
        /* protection against enormous URLs */
        if (strlen(argv[1]) < DNS_MAX_NAME_LEN)
        {
            uint8_t af = AF_INET6;

            if ((argc > 1) && (strcmp(argv[2], "-4") == 0))
            {
                af = AF_INET;
            }

            if (mDnsResolverNoDnsServer ==
                DNS_GetHostByName((uint8_t*)argv[1], af, mDnsClassIN_c, SHELL_DnsService, pmMainThreadMsgQueue))
            {
                shell_write("No DNS server configured!"EOL);
            }
            else
            {
                status = CMD_RET_ASYNC;
            }
        }
    }

    return status;
}
#endif /* DNS_ENABLED */

#if MAC_FILTERING_ENABLED
/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_Filter(uint8_t argc, char *argv[])
\brief  This function is used for "filter" shell command.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_Filter
(
    uint8_t argc,
    char *argv[]
)
{
    command_ret_t ret = CMD_RET_SUCCESS;
    char *pValue;

    /* Stop any socket pending commands */
    if(argc > 0)
    {
        if(!strcmp(argv[1], "add"))
        {
            uint64_t extendedAddress = 0;
            uint8_t linkIndicator = THR_ALL_FFs8;
            bool_t block = FALSE;

            /* Get the extended address of the neighbor */
            if(argv[2])
            {
                SHELL_ReadBuff(argv[2] + 2,(uint8_t *)&extendedAddress,8);
                NWKU_SwapArrayBytes((uint8_t*)&extendedAddress,8);
            }
            /* Get the value for LQI */
            pValue = shell_get_opt(argc, argv, "lqi");

            if(pValue)
            {
                linkIndicator = (uint8_t)NWKU_atoi(pValue);
            }
            pValue = shell_get_opt(argc, argv, "reject");

            if(pValue)
            {
                block = (bool_t)NWKU_atoi(pValue);
            }

            if(0 != extendedAddress)
            {
                if(MacFiltering_AddNeighbor(threadInstanceID, extendedAddress, THR_ALL_FFs16,
                                            linkIndicator, block) == gThrStatus_Success_c)
                {
                    shell_write("Entry has been added!\r\n");
                }
                else
                {
                    shell_write("An error has occurred!\r\n");
                }
            }
        }
        else if(!strcmp(argv[1], "remove"))
        {
            uint64_t extendedAddress = 0;

            if(argv[2])
            {
                SHELL_ReadBuff(argv[2] + 2, (uint8_t *)&extendedAddress, 8);
                NWKU_SwapArrayBytes((uint8_t *)&extendedAddress, 8);
            }
            else
            {
                shell_write("Enter the extended address of the neighbor!\r\n");
                ret = CMD_RET_ASYNC;
            }

            if (0 != extendedAddress)
            {
                if(MacFiltering_RemoveNeighbor(threadInstanceID, extendedAddress) == gThrStatus_Success_c)
                {
                    shell_write("Entry has been removed!\r\n");
                }
                else
                {
                    shell_write("An error has occurred!\r\n");
                }
            }

        }
        else if(!strcmp(argv[1], "enable"))
        {
            if(NULL != argv[2])
            {
                if(!strcmp(argv[2], "reject"))
                {
                    MacFiltering_Active(threadInstanceID, mMacFilteringDefaultPolicyReject_c);
                }
                else if(!strcmp(argv[2], "accept"))
                {
                    MacFiltering_Active(threadInstanceID, mMacFilteringDefaultPolicyAccept_c);
                }
            }
            else
            {
                MacFiltering_Active(threadInstanceID, mMacFilteringDefaultPolicyAccept_c);
            }

            shell_printf("MAC Filtering Status: %s",
            mMacFilteringStatus[MacFiltering_IsActive(threadInstanceID)]);
        }
        else if(!strcmp(argv[1], "disable"))
        {
            MacFiltering_Active(threadInstanceID, mMacFilteringDisabled_c);
            shell_write("MAC Filtering Disabled!\r\n");
        }
        else if(!strcmp(argv[1], "show"))
        {
            shell_printf("MAC Filtering Status: %s",
            mMacFilteringStatus[MacFiltering_IsActive(threadInstanceID)]);

            SHELL_Filtering_Print(threadInstanceID);
        }
        else
        {
            ret = CMD_RET_ASYNC;
        }
    }
    else
    {
        SHELL_Resume();
    }

    return ret;
}

/*!*************************************************************************************************
\private
\fn     static void SHELL_Filtering_Print(uint32_t instanceID)
\brief  This function is used to print the filtered addresses.

\param  [in]    instanceID    Thread instance ID
***************************************************************************************************/
static void SHELL_Filtering_Print
(
    uint32_t instanceID
)
{
    uint32_t iCount = MAC_FILTERING_TABLE_SIZE;
    macFilteringNeighborData_t macFilterEntry;

    shell_printf("Idx   Extended Address     Short Address   Link Quality  Reject\n\r");
    do
    {
        iCount--;

        if(THR_GetAttr(instanceID, gNwkAttrId_WhiteListEntry_c, iCount, NULL, (uint8_t*)&(macFilterEntry)) == gThrStatus_Success_c)
        {
            uint64_t reversedExtAddr = macFilterEntry.extendedAddress;

            NWKU_SwapArrayBytes((uint8_t*)&reversedExtAddr,8);
            shell_printf("%d", iCount);
            shell_printf("     ");
            shell_printf("0x");
            SHELL_PrintBuff((uint8_t*)&reversedExtAddr,8);
            shell_printf("   ");

            if(macFilterEntry.shortAddress != THR_ALL_FFs16)
            {
                shell_printf("0x%04X", macFilterEntry.shortAddress);
            }
            else
            {
                shell_write("------");
            }
            shell_printf("          ");
            shell_printf("%d", macFilterEntry.linkIndicator);
            shell_printf("            ");

            if (macFilterEntry.blockNeighbor == TRUE)
            {
                shell_printf("TRUE\r\n");
            }
            else
            {
                shell_printf("FALSE\r\n");
            }
        }
    }
    while (iCount);

    shell_printf("\r\nEnd of MAC Filtering Table.\r\n");
}
#endif /* MAC_FILTERING_ENABLED */

#ifdef USE_MSD_BOOTLOADER
/*!*************************************************************************************************
\private
\fn     int8_t SHELL_FlashEraseWithMSDFunc(uint8_t argc, char *argv[])
\brief  This function is used for firmware erase and re-enabling the MSD bootloader.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_FlashEraseWithMSDFunc
(
    uint8_t argc,
    char *argv[]
)
{
    shell_write("\r Start Firmware Erase and enable the MSD Bootloader! \n\r");
    NvEraseSector(0xBFF8);
    ResetMCU();
    return FALSE;
}
#endif /* USE_MSD_BOOTLOADER */

#if gHybridApp_d
/*!*************************************************************************************************
\private
\fn     static int8_t SHELL_BreakReq(uint8_t argc, char *argv[])
\brief  This function is used to resume a continuous process(ex: ping).

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_BreakReq
(
    uint8_t argc,
    char *argv[]
)
{
    if(TRUE == mShellBoardIdentify)
    {
        mShellBoardIdentify = FALSE;
        Led_Identify(mShellBoardIdentify);
    }
    else
    {
        gbRetryInterrupt = FALSE;
        mContinuousPing = FALSE;
#if ECHO_PROTOCOL
        mEchoUdpCounter = 0;
#endif
    }
    SHELL_Resume();
    return FALSE;
}
#endif

#if GETIPv6ADDRESSES_APP
/*===================================GETIPv6ADDRESSES=============================================*/
/*!*************************************************************************************************
\private
\fn     int8_t SHELL_GetNeighborsIpAddr()
\brief  This function is the callback function for "getneighborsip" shell command.

\param  [in]    argc      Number of arguments the command was called with
\param  [in]    argv      Pointer to a list of pointers to the arguments

\return         int8_t    Status of the command
***************************************************************************************************/
static int8_t SHELL_GetNeighborsIpAddr
(
    uint8_t argc,
    char *argv[]
)
{
    ipAddr_t coapDestAddress =  in6addr_realmlocal_allthreadnodes;
#if THR_ENABLE_MGMT_DIAGNOSTICS
    static bool_t bIsCbRegistered = FALSE;
#endif /* THR_ENABLE_MGMT_DIAGNOSTICS */

    /* Request the IPv6 Address List from each node */
    /* This could be an array of multiple TLVs */
    uint8_t requestedTLVs[] = {gDiagnosticTlv_IPv6AddrList_c};

    mgmtDiagnostic_CoapMsg_t *pDiagGetTlv = MEM_BufferAlloc(sizeof(mgmtDiagnostic_CoapMsg_t) + NumberOfElements(requestedTLVs));

    if(pDiagGetTlv != NULL)
    {
#if THR_ENABLE_MGMT_DIAGNOSTICS
        if(FALSE == bIsCbRegistered)
        {
            /* Register the application callback for receiving the diagnostic reply messages */
            (void)MgmtDiagnostic_RegisterAppCb(SHELL_MgmtDiagnosticAppCb);
            bIsCbRegistered = TRUE;
        }
#endif /* THR_ENABLE_MGMT_DIAGNOSTICS */
        /* Set the destination address */
        /* By default, the destination is All Thread Nodes */
        FLib_MemCpy(pDiagGetTlv->dstIpAddr, &coapDestAddress, sizeof(ipAddr_t));

        pDiagGetTlv->payloadSize = NumberOfElements(requestedTLVs);
        FLib_MemCpy(pDiagGetTlv->payload, requestedTLVs, NumberOfElements(requestedTLVs));

#if THR_ENABLE_MGMT_DIAGNOSTICS
        /* Send Diagnostic Message */
        MgmtDiagnostic_SendGetReq(threadInstanceID, pDiagGetTlv, NULL);
#endif /* THR_ENABLE_MGMT_DIAGNOSTICS */

        MEM_BufferFree(pDiagGetTlv);
    }

    /* Refresh Shell */
    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\fn     static void SHELL_MgmtDiagnosticAppCb(mgmtDiagnostic_RspData_t mgmtDiagRspData)
\brief  This function is the callback function for receiving reply to diagnostic message.

\param  [in]    mgmtDiagRspData     Diagnostic reply data
***************************************************************************************************/
void SHELL_MgmtDiagnosticAppCb
(
    mgmtDiagnostic_RspData_t *mgmtDiagRspData
)
{
    mgmtDiagnostic_Tlv_t *pTlvHeader = (mgmtDiagnostic_Tlv_t *)mgmtDiagRspData->pPayload;
    uint32_t remainingBytes = mgmtDiagRspData->payloadLen;
    uint32_t tlvSize = 0;
    uint8_t *pHeader = (uint8_t *)mgmtDiagRspData->pPayload;

    if(mgmtDiagRspData->status == gThrStatus_Success_c)
    {
        while(remainingBytes != 0)
        {
            tlvSize = 2 + pTlvHeader->length;

            /* Check if the reply has IPv6AddrList TLV */
            if(pTlvHeader->tlvType == gDiagnosticTlv_IPv6AddrList_c)
            {
                char addrStr[INET6_ADDRSTRLEN] = {0};
                uint8_t noOfAddr = pTlvHeader->length / sizeof(ipAddr_t);

                shell_write("\r\nReceived IP Addresses from node:\r\n");

                /* Print addresses */
                for(uint32_t iCount = 0; iCount < noOfAddr; iCount++)
                {
                    ipAddr_t ipAddr = {0};
                    ipAddr_t *pIpAddr = (ipAddr_t *)(mgmtDiagRspData->pPayload + sizeof(mgmtDiagnostic_Tlv_t) + sizeof(ipAddr_t) * iCount);

                    /* Need aligned data structure */
                    FLib_MemCpy(&ipAddr, pIpAddr, sizeof(ipAddr_t));

                    if(IP_IsAddrIPv6(&ipAddr))
                    {
                        ntop(AF_INET6, &ipAddr, addrStr, INET6_ADDRSTRLEN);
                        shell_write(addrStr);
                        SHELL_NEWLINE();
                    }
                }
                shell_refresh();
            }
            /* Add here other TLVs to be processed with else if condition */

            remainingBytes -= tlvSize;
            pHeader = pHeader + tlvSize;
            /* Move pointer to the next TLV in reply */
            pTlvHeader = (mgmtDiagnostic_Tlv_t *)pHeader;
        }
    }
}
#endif

#if ICMP_STATISTICS_ENABLED
/*!*************************************************************************************************
\private
\fn     static void float_division(char *intPart, char *fractPart, int sent, int lost)
\brief  This function determines integer and fractional part of lost / sent division.

\param  [in]    intPart     Pointer to retained integer part of division
\param  [in]    fractPart   Pointer to retained fractional part of division
\param  [in]    sent        Number of packets sent
\param  [in]    lost        Number of packets lost
***************************************************************************************************/
static void float_division(char *intPart, char *fractPart, uint32_t sent, uint32_t lost)
{
    uint32_t multiplier = 1, mulFactor, lostCopy = lost, sentCopy = sent;
    uint16_t loss;
    char aLoss[5];
    int8_t cIntPartDigits;
    uint8_t cLostDigits = 0, cSentDigits = 0;

    while(lostCopy != 0)
    {
      cLostDigits++;
      lostCopy = lostCopy / 10;
    }

    while(sentCopy != 0)
    {
      cSentDigits++;
      sentCopy = sentCopy / 10;
    }

    mulFactor = 3 + cLostDigits;
    cIntPartDigits = 2 + (cLostDigits - cSentDigits);

    while(mulFactor > 0)
    {
        multiplier *= 10;
        mulFactor--;
    }

    loss = lost * multiplier / sent;
    sprintf(aLoss, "%d", loss);
    aLoss[4] = '\0';

    switch(cIntPartDigits)
    {
        case 2:
            intPart[0] = aLoss[0];
            intPart[1] = aLoss[1];
            intPart[2] = '\0';

            fractPart[0] = aLoss[2];
            fractPart[1] = aLoss[3];
            fractPart[2] = '\0';
            break;

        case 1:
            intPart[0] = aLoss[0];
            intPart[1] = '\0';

            fractPart[0] = aLoss[1];
            fractPart[1] = aLoss[2];
            fractPart[2] = '\0';
            break;

        case 0:
            intPart[0] = '0';
            intPart[1] = '\0';

            fractPart[0] = aLoss[0];
            fractPart[1] = aLoss[1];
            fractPart[2] = '\0';
            break;

        case -1:
            intPart[0] = '0';
            intPart[1] = '\0';

            fractPart[0] = '0';
            fractPart[1] = aLoss[0];
            fractPart[2] = '\0';
            break;

        default:
            sprintf(intPart, "%s", "0");
            sprintf(fractPart, "%s", "00");
            break;
    }
}
#endif/* ICMP_STATISTICS_ENABLED */

#endif /* THREAD_USE_SHELL */

/*==================================================================================================
Private debug functions
==================================================================================================*/
