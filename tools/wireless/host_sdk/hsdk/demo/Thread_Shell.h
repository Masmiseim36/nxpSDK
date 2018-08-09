/*
* \file Thread_Shell.h
 * Header file used to demonstrate THCI over UART/SPI for the Thread IP stack
 * from a simulated user-friendly shell interface.
 *
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

typedef uint8_t bool_t;

#define TRUE 1
#define FALSE 0

// FSCI protocol attributes
#define LENGTH_FIELD_SIZE           2  // size in bytes of FSCI length field
#define CRC_FIELD_SIZE              1  // size in bytes of FSCI CRC field
#define VIF                         0  // do not use FSCI virtual interfaces


// FSCI operation groups and codes
#define TX_OG                       0xCE
#define THR_RX_OG                   0xCF
#define MWS_TX_OG                   0xC0
#define MWS_RX_OG                   0xC1
#define MWS_REQ_ACCESS_OC           0x03
#define MWS_REL_ACCESS_OC           0x04

#define VTUN_LINK_IND_OC            0xF1
#define VTUN_TX_OC                  0xF2
#define VTUN_RX_OC                  0xF3

#define IFCONFIG_OC                 0x0D
#define NWKU_PING_OC                0x0E
#define THR_GET_ATTR_OC             0x17
#define THR_SET_ATTR_OC             0x18
#define THR_GET_IP_ADDR_OC          0x19
#define THR_CREATE_NWK_OC           0x1B
#define THR_JOIN_NWK_OC             0x1C
#define FACTORY_RESET_OC            0x1F
#define CPU_RESET_OC                0x21
#define CPU_RESET_IND_OC            0x22
#define THR_BR_ADD_ENTRY_OC         0x29
#define THR_BR_SYNC_OC              0x2C
#define MESHCOP_START_COMM          0x40
#define MESHCOP_STOP_COMM           0x41
#define MESHCOP_ADD_JOINER_OC       0x42
#define MESHCOP_SYNC_STEERING_OC    0x46
#define MESHCOP_MGMT_SET_OC         0x4B
#define THR_EVT_COMM_DIAG_IND_OC    0x4E
#define THR_EVT_NWK_CREATE_CNF_OC   0x51
#define THR_EVT_NWK_JOIN_IND_OC     0x52
#define THR_EVT_GENERAL_CNF_OC      0x54
#define THR_EVT_NWK_COMM_IND_OC     0x55
#define THR_MGMT_DIAG_GET_OC        0x61
#define THR_MGMT_DIAG_GET_IND_OC    0x63
#define COAP_SEND_OC                0x90
#define COAP_MSG_RCVD_OC            0x92
#define COAP_CREATE_INST_OC         0x93
#define MESHCOP_COMM_GET_OC         0xA0
#define MESHCOP_COMM_SET_OC         0xA1
#define MESHCOP_ACTIVE_GET_OC       0xA2
#define MESHCOP_ACTIVE_SET_OC       0xA3
#define MESHCOP_PENDING_GET_OC      0xA4
#define MESHCOP_PENDING_SET_OC      0xA5
#define MESHCOP_PANID_QUERY_OC      0xA8
#define MESHCOP_PANID_CONFLICT_OC   0xA9

/*! IPV6 realm local all nodes address */
#define IN6ADDR_REALMLOCAL_ALLNODES_INIT \
        0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01

/* Thread specification - chapter 10. Section 10.11.1.4  Diagnostic TLVs */
#define gDiagnosticTlv_SourceAddr_c     0x00  /*!< EUI - 64 bit */
#define gDiagnosticTlv_ShortAddr_c      0x01  /*!< Address - 16 bit */
#define gDiagnosticTlv_Mode_c           0x02  /*!< Mode - Capability Information */
#define gDiagnosticTlv_Timeout_c        0x03  /*!< Timeout - Sleepy polling rate */
#define gDiagnosticTlv_Connectivity_c   0x04  /*!< Link Quality - only for Leader/Router/Reed*/
#define gDiagnosticTlv_RoutingTable_c   0x05  /*!< Routing table - only for Leader/Router/Reed */
#define gDiagnosticTlv_LeaderData_c     0x06  /*!< Leader data - only for Leader/Router/Reed */
#define gDiagnosticTlv_NwkData_c        0x07  /*!< Network data */
#define gDiagnosticTlv_IPv6AddrList_c   0x08  /*!< List of all Ipv6 addresses registered by the device */
#define gDiagnosticTlv_MacCounters_c    0x09  /*!< Packet/event counters for the MAC 802.15.4 interface */
#define gDiagnosticTlv_BatteryLevel_c   0x0E  /*!< 8-bit unsigned integer that indicates the percentage
                                               *   of remaining battery energy */
#define gDiagnosticTlv_SupplyVoltage_c  0x0F  /*!< 16-bit unsigned integer that indicates
                                               *   the current supply voltage */
#define gDiagnosticTlv_ChildTable_c     0x10  /*!< Structure containing information on all children
                                               *   (only for Leader/Router/Reed) */
#define gDiagnosticTlv_ChannelPages_c   0x11  /*!< A list of one or more 8-bit integers that indicates
                                               *   the set of supported IEEE 802.15.4 ChannelPage values */
#define gDiagnosticTlv_TypeList_c       0x12  /*!< An array of 8-bit TLV Type codes to get values for
                                               *   as a part of a MGMT_GET command query */

// Other constants
#define ENET_HDR_SIZE               14
#define IPv6_VERSION                0x60
#define MTU                         1500
#define SHELL_MAX_CMD_LEN           0x400
#define THR_INST_ID                 0x00
#define COAP_DEFAULT_PORT           5683

// Supported shell commands
#define SHELL_HARNESS_CONFIG        "harness config"
#define SHELL_RESET                 "reset"
#define SHELL_FACTORY_RESET         "factoryreset"
#define SHELL_IFCONFIG              "ifconfig"
#define SHELL_PING                  "ping"
#define SHELL_COAP                  "coap"
#define SHELL_GET_NODES_IP          "getnodesip"
#define SHELL_PERMIT_JOIN           "ptaon"
#define SHELL_DENY_JOIN             "ptaoff"
#define SHELL_HELP                  "help"
#define SHELL_EXIT                  "exit"
#define SHELL_SYNC_NWKDATA          "thr sync nwkdata"
#define SHELL_NWKDATA_ADD           "thr nwkdata add"
#define SHELL_THR_SET_CHANNEL       "thr set channel"
#define SHELL_THR_SET_ISCOMM        "thr set iscommissioned"
#define SHELL_THR_SET_AUTOSTART     "thr set autostart"
#define SHELL_THR_SET_ACTIVETS      "thr set activets"
#define SHELL_THR_GET_EXT_ADDR      "thr get randomaddr"
#define SHELL_THR_CREATE            "thr create"
#define SHELL_THR_JOIN              "thr join"
#define SHELL_MGMT_GET_TLVS         "thr mgmt gettlvs"
#define SHELL_MGMT_ACTIVESET        "thr mgmt activeset"
#define SHELL_MGMT_ACTIVEGET        "thr mgmt activeget"
#define SHELL_MGMT_COMMSET          "thr mgmt commset"
#define SHELL_MGMT_COMMGET          "thr mgmt commget"
#define SHELL_MGMT_PENDINGSET       "thr mgmt pendingset"
#define SHELL_MGMT_PENDINGGET       "thr mgmt pendingget"
#define SHELL_MGMT_QUERY            "thr mgmt query"
#define SHELL_START_COMM            "thr commissioner start"
#define SHELL_STOP_COMM             "thr commissioner stop"
#define SHELL_ADD_JOINERS           "thr addjoiners"

#define HELP_NWKDATA_ADD            "-p <prefix (ipv6 string)> -l <prefix len (dec)> -t <lifetime seconds (0xhex)>"
#define HELP_ATV                    "<addr> <tlvname> <value>"
#define HELP_AV                     "<addr> <tlvname>"


typedef struct MESHCOP_MgmtSetRequest_tag {
    uint8_t InstanceId;  // Thread instance Id
    uint8_t IPAddress[16];  // The IPv6 address
    bool_t SessionIdEnable;  // SessionIdEnable
    uint16_t SessionId;  // SessionId
    bool_t BorderRouterLocatorEnable;  // BorderRouterLocatorEnable
    uint16_t BorderRouterLocator;  // Border Router Locator
    bool_t NewSessionIdEnable;  // NewSessionIdEnable
    uint16_t NewSessionId;  // Send new session ID
    bool_t SteeringDataEnable;  // SteeringDataEnable
    uint8_t SteeringDataSize;  // The size of the Steering Data
    uint8_t *SteeringData;  // SteeringData
    bool_t ChannelEnable;  // ChannelEnable
    uint8_t Channel;  // Channel
    bool_t ChannelMaskEnable;  // ChannelMaskEnable
    uint8_t ChannelPage;  // ChannelPage
    uint8_t ChannelMaskLength;  // ChannelMaskLength
    uint8_t *ChannelMask;  // ChannelMask
    bool_t XpanIdEnable;  // XpanIdEnable
    uint8_t XpanId[8];  // XpanId
    bool_t MLPrefixEnable;  // MLPrefixEnable
    struct {
        uint8_t PrefixData[16];  // Prefix Data
        uint8_t PrefixLength;  // Prefix length in bits
    } MLPrefix;  // ML Prefix
    bool_t MasterKeyEnable;  // MasterKeyEnable
    uint8_t MasterKey[16];  // MasterKey
    bool_t NwkNameEnable;  // NwkNameEnable
    uint8_t NwkNameSize;  // Network Name Size
    char *NwkName;  // Network Name
    bool_t PanIdEnable;  // PanIdEnable
    uint16_t PanId;  // PanId
    bool_t PSKcEnable;  // PSKcEnable
    uint8_t PskcSize;  // PskcSize
    char *PSKc;  // PSKc
    bool_t PolicyEnable;  // PolicyEnable
    uint16_t RotationInterval;  // RotationInterval
    uint8_t Policy;  // O|N|R|C|B  // Policy
    bool_t ActiveTimestampEnable;  // Will be applied after Delay Timer expires
    uint8_t ActiveSeconds[6];  // Active timestamp seconds
    uint16_t ActiveTicks;  // Active timestamp ticks
    bool_t PendingTimestampEnable;  // Used to compare multiple Pending Set requests
    uint8_t PendingSeconds[6];  // Pending timestamp Seconds
    uint16_t PendingTicks;  // Pending timestamp ticks
    bool_t DelayTimerEnable;  // DelayTimerEnable
    uint32_t Timeout;  // Timeout(ms)
    bool_t FutureTlvEnable;  // Future Tlv Enable - for certification purposes
    struct {
        uint8_t FutureTlvSize;  // Future Tlv Size - for certification purposes
        uint8_t *FutureTlvValue;  // Future Tlv Value - for certification purposes
    } FutureTlv;  // Future TLV Structure - for certification purposes
} MESHCOP_MgmtSetRequest_t;


typedef enum MESHCOP_MgmtGetRequest_TlvIds_TlvId_tag {
    TlvId_BorderRouterLocator = 0x09,
    TlvId_CommissionerSessionId = 0x0B,
    TlvId_SteeringData = 0x08,
    TlvId_Channel = 0x00,
    TlvId_ChannelMask = 0x35,
    TlvId_XpanId = 0x02,
    TlvId_MeshLocalUla = 0x07,
    TlvId_MasterKey = 0x05,
    TlvId_NetworkName = 0x03,
    TlvId_PanId = 0x01,
    TlvId_PSKc = 0x04,
    TlvId_SecurityPolicy = 0x0C,
    TlvId_ActiveTimestamp = 0x0E,
    TlvId_DelayTimer = 0x34,
    TlvId_PendingTimestamp = 0x33,
    TlvId_ScanDuration = 0x38,
    TlvId_EnergyList = 0x39
} MESHCOP_MgmtGetRequest_TlvIds_TlvId_t;


typedef struct MESHCOP_MgmtGetRequest_tag {
    uint8_t InstanceId;  // Thread instance Id
    uint8_t IPAddress[16];  // The IPv6 address
    uint8_t NumberOfTlvIds;  // Number of TLV Ids
    MESHCOP_MgmtGetRequest_TlvIds_TlvId_t *TlvIds;  // TlvIds
} MESHCOP_MgmtGetRequest_t;

/* CoAP request type */
typedef enum NWKU_CoapSendRequest_RequestType_tag {
    NWKU_CoapSendRequest_RequestType_CON = 0x00,
    NWKU_CoapSendRequest_RequestType_NON = 0x01
} NWKU_CoapSendRequest_RequestType_t;

/* CoAP message type */
typedef enum NWKU_CoapSendRequest_MessageType_tag {
    NWKU_CoapSendRequest_MessageType_GET = 0x01,
    NWKU_CoapSendRequest_MessageType_POST = 0x02,
    NWKU_CoapSendRequest_MessageType_PUT = 0x03,
    NWKU_CoapSendRequest_MessageType_DELETE = 0x04
} NWKU_CoapSendRequest_MessageType_t;

typedef struct NWKU_CoapSendRequest_tag {
    uint8_t InstanceID;  // InstanceID
    uint8_t DestinationIpAddress[16];  // Destination Ip Address
    uint16_t UDPPort;  // UDPPort
    NWKU_CoapSendRequest_RequestType_t RequestType;  // CoAP request type
    NWKU_CoapSendRequest_MessageType_t MessageType;  // CoAP message type
    char URIpath[30];  // URI-path options separated by /
    uint8_t PayloadLength;  // CoAP message payload length
    char *Payload;  // CoAP message payload
} NWKU_CoapSendRequest_t;
