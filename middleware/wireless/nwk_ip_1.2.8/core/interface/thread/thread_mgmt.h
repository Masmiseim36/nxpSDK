/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
* \file     thread_mgmt.h
* \brief    This is the header file for the Thread network management and diagnostic system.
*/

#ifndef _THREAD_MGMT_H
#define _THREAD_MGMT_H

/*==================================================================================================
Include Files
==================================================================================================*/
#include "thread_types.h"
#include "coap.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/* Thread specification - chapter 10. Section 10.11.1.4  Diagnostic TLVs */
#define gDiagnosticTlv_SourceAddr_c                     0x00    /*!< EUI - 64 bit */
#define gDiagnosticTlv_ShortAddr_c                      0x01    /*!< Address - 16 bit */
#define gDiagnosticTlv_Mode_c                           0x02    /*!< Mode - Capability Information */
#define gDiagnosticTlv_Timeout_c                        0x03    /*!< Timeout - Sleepy polling rate */
#define gDiagnosticTlv_Connectivity_c                   0x04    /*!< Link Quality - only for Leader/Router/Reed*/
#define gDiagnosticTlv_RoutingTable_c                   0x05    /*!< Routing table - only for Leader/Router/Reed */
#define gDiagnosticTlv_LeaderData_c                     0x06    /*!< Leader data - only for Leader/Router/Reed */
#define gDiagnosticTlv_NwkData_c                        0x07    /*!< Network data */
#define gDiagnosticTlv_IPv6AddrList_c                   0x08    /*!< List of all Ipv6 addresses registered by the device */
#define gDiagnosticTlv_MacCounters_c                    0x09    /*!< Packet/event counters for the MAC 802.15.4 interface */
#define gDiagnosticTlv_BatteryLevel_c                   0x0E    /*!< 8-bit unsigned integer that indicates the percentage
                                                                 *   of remaining battery energy */
#define gDiagnosticTlv_SupplyVoltage_c                  0x0F    /*!< 16-bit unsigned integer that indicates
                                                                 *   the current supply voltage */
#define gDiagnosticTlv_ChildTable_c                     0x10    /*!< Structure containing information on all children
                                                                 *   (only for Leader/Router/Reed) */
#define gDiagnosticTlv_ChannelPages_c                   0x11    /*!< A list of one or more 8-bit integers that indicates
                                                                 *   the set of supported IEEE 802.15.4 ChannelPage values */
#define gDiagnosticTlv_TypeList_c                       0x12    /*!< An array of 8-bit TLV Type codes to get values for
                                                                 *   as a part of a MGMT_GET command query */

/* Freescale specific */
#define gDiagnosticTlv_FslMac6LowPanNvmCount_c          0xA0    /*!< MAC, 6lowpan (Mac Filtering, 6LowPan) NVM data save count */
#define gDiagnosticTlv_FslNetworkNvmCount_c             0xA1    /*!< Network (IP, DHCP, Leader Id Assignment, Trickle, MPL, ND)
                                                                 *   NVM data save count */
#define gDiagnosticTlv_FslSecurityNvmCount_c            0xA2    /*!< Security NVM data save count */
#define gDiagnosticTlv_FslFunctionalNvmCount_c          0xA3    /*!< attributes, events NVM data save count */
#define gDiagnosticTlv_FslBoardName_c                   0xA4    /*!< Board Name */
#define gDiagnosticTlv_FslUniqueMcuId_c                 0xA5    /*!< Unique MCU Id */
#define gDiagnosticTlv_FslStackVersion_c                0xA6    /*!< Stack version */
#define gDiagnosticTlv_FslSwVersion_c                   0xA7    /*!< Software version */
#define gDiagnosticTlv_IeeeAddr_c                       0xA8    /*!< IEEE Address */

/* Large Network diagnostics */
#define gDiagnosticTlv_DiagTestColdReset_c                0xB0    /*!< Cold/Factory reset */
#define gDiagnosticTlv_DiagTestWarmReset_c                0xB1    /*!< CPU reset */
#define gDiagnosticTlv_DiagTestData_c                     0xB2    /*!< Large network data */
#define gDiagnosticTlv_DiagTestResults_c                  0xB3    /*!< Large network results */

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! Enumeration for network diagnostic commands */
typedef enum mgmtDiagnosticCmd_tag
{
      gMgmtDiagnostic_GetReq_c = 0x00,    /*!< Get Request command */
      gMgmtDiagnostic_GetRsp_c,           /*!< Get Response command */
      gMgmtDiagnostic_GetQryReq_c,        /*!< Get Query Request command */
      gMgmtDiagnostic_GetQryRsp_c,        /*!< Get Query Response command */
      gMgmtDiagnostic_GetAnsReq_c,        /*!< Get Answer Request command */
      gMgmtDiagnostic_GetAnsRsp_c,        /*!< Get Answer Response command */
      gMgmtDiagnostic_ResetReq_c,         /*!< Reset Request command */
      gMgmtDiagnostic_ResetRsp_c,         /*!< Reset Response command */
      gMgmtDiagnostic_DiagTestReq_c,      /*!< Diagnostic Test Request command */
      gMgmtDiagnostic_DiagTestRsp_c       /*!< Diagnostic Test Response command */
}mgmtDiagnosticCmd_t;

/*! Enumeration for network diagnostic link data operation mode */
enum{
      gMgmtDiagnostic_LinkDataGet_c = 0x00,     /*!< Get counters */
      gMgmtDiagnostic_LinkDataReset_c,          /*!< Reset counters */
      gMgmtDiagnostic_LinkData1StepInc_c,       /*!< Increment with 1 */
};

/*! Enumeration for network link diagnostic structure internal */
enum{
      gMgmtDiagnostic_ifInUnknownProtosPkts_c = 0xB0,  /*!< Counter for unknown protocol received packets */
      gMgmtDiagnostic_ifInErrorsPkts_c,                /*!< Counter for received packets with errors */
      gMgmtDiagnostic_ifOutErrorsPkts_c,               /*!< Counter for transmitted packets with errors */
      gMgmtDiagnostic_ifInUcastPkts_c,                 /*!< Counter for received unicast packets */
      gMgmtDiagnostic_ifInBroadcastPkts_c,             /*!< Counter for received broadcast packets */
      gMgmtDiagnostic_ifInDiscardsPkts_c,              /*!< Counter for received discarded packets */
      gMgmtDiagnostic_ifOutUcastPkts_c,                /*!< Counter for transmitted unicast packets */
      gMgmtDiagnostic_ifOutBroadcastPkts_c,            /*!< Counter for transmitted broadcast packets */
      gMgmtDiagnostic_ifOutDiscardsPkts_c              /*!< Counter for transmitted discarded packets */
};

/*! Network diagnostic CoAP message format structure */
typedef struct mgmtDiagnostic_CoapMsg_tag
{
      uint8_t dstIpAddr[16];        /*!< CoAP destination IP address */
      uint8_t payloadSize;          /*!< CoAP payload size */
      uint8_t payload[];            /*!< CoAP payload - variable data size */
}mgmtDiagnostic_CoapMsg_t;

/*! Large network diagnostic CoAP message format structure */
typedef struct mgmtDiagTest_ReqCoapMsg_tag
{
      uint8_t cmdId;                /*!< Requested command */
      uint8_t reqLatency[4];        /*!< Latency on request */
      uint8_t rspLatency[4];        /*!< Latency on response */
      uint8_t offset[4];            /*!< Calculated offset */
      uint8_t seqNum;               /*!< Sequence number (useful in multicast) */
}mgmtDiagTest_CoapMsg_t;

/* Thread specification - chapter 10. Section 10.11.3.1 and RFC2863 */

/*! Network link diagnostic structure */
typedef struct mgmtDiagnostic_LinkData_tag
{
      uint32_t ifInUnknownProtosPkts;         /*!< The number of packets received via the interface
                                               *   which were discarded because of an unknown or
                                               *   unsupported protocol. */
      uint32_t ifInErrorsPkts;                /*!< The number of inbound packets that contained errors
                                               *   preventing them from being deliverable to a
                                               *   higher-layer protocol. */
      uint32_t ifOutErrorsPkts;               /*!< The number of outbound packets that could not be
                                               *   transmitted because of errors. */
      uint32_t ifInUcastPkts;                 /*!< The number of packets, delivered by this sub-layer
                                               *   to a higher (sub-)layer, which were not addressed to
                                               *   a multicast or broadcast address at this sub-layer. */
      uint32_t ifInBroadcastPkts;             /*!< The number of packets, delivered by this sub-layer
                                               *   to a higher (sub-)layer, which were addressed to a
                                               *   broadcast address at this sub-layer. */
      uint32_t ifInDiscardsPkts;              /*!< The number of inbound packets which were chosen to
                                               *   be discarded even though no errors had been detected to
                                               *   prevent their being deliverable to a higher-layer protocol.
                                               *   One possible reason for discarding such a packet could be to
                                               *   free up buffer space.*/
      uint32_t ifOutUcastPkts;                /*!< The total number of packets that higher-level protocols
                                               *   requested be transmitted, and which were not addressed to a
                                               *   multicast or broadcast address at this sub-layer, including
                                               *   those that were discarded or not sent. */
      uint32_t ifOutBroadcastPkts;            /*!< The total number of packets that higher-level protocols
                                               *   requested be transmitted, and which were addressed to a
                                               *   broadcast address at this sub-layer, including those that
                                               *   were discarded or not sent. */
      uint32_t ifOutDiscardsPkts;             /*!< The number of outbound packets which were chosen to be
                                               *   discarded even though no errors had been detected to prevent
                                               *   their being transmitted.  One possible reason for discarding
                                               *   such a packet could be to free up buffer space. */
}mgmtDiagnostic_LinkData_t;

/*! Network diagnostic device info structure */
typedef struct mgmtDiagnostic_DeviceData_tag
{
     uint8_t batteryLevel;           /*!< Percentage of battery level left */
     uint16_t voltage;               /*!< Voltage in mV*/
     thrOctet32_t boardName;         /*!< Board Type : TWR-KW24D512, FRDM-KW24 ...*/
}mgmtDiagnostic_DeviceData_t;

/*! Network diagnostic TLV format structure */
typedef struct mgmtDiagnostic_Tlv_tag
{
    uint8_t tlvType;    /*!< TLV type */
    uint8_t length;     /*!< TLV length */
    uint8_t pData[];    /*!< Pointer to TLV data */
}mgmtDiagnostic_Tlv_t;

/*! Network diagnostic extended TLV format structure */
typedef struct mgmtDiagnostic_ExtTlv_tag
{
    uint8_t reserved1;      /*!< Reserved. Value = 0xFF */
    uint8_t tlvType;        /*!< TLV type */
    uint8_t reserved2;      /*!< Reserved. Value = 0xFF */
    uint8_t length[2];      /*!< TLV length */
    uint8_t pData[];        /*!< Pointer to TLV data */
}mgmtDiagnostic_ExtTlv_t;

/*! Network diagnostic NVM data structure */
typedef struct mgmtDiagnostic_NvmData_tag
{
  uint16_t nvmId;       /*!< NVM ID */
  uint8_t tlvId;        /*!< TLV ID */
  uint16_t *pData;      /*!< Pointer to data */
}mgmtDiagnostic_NvmData_t;

/*! Network diagnostic response data format structure */
typedef struct mgmtDiagnostic_RspData_tag
{
  mgmtDiagnosticCmd_t commandId;  /*!< Command Id: gMgmtDiagnostic_GetRsp_c, gMgmtDiagnostic_RsetRsp_c */
  thrStatus_t status;             /*!< Status: gThrStatus_Success_c, gThrStatus_Failed_c */
  uint16_t msgId;                 /*!< CoAP message Id: used for synchronization between Req and Rsp */
  uint32_t payloadLen;            /*!< Payload length */
  uint8_t *pPayload;              /*!< Payload */
}mgmtDiagnostic_RspData_t;

/*!
\brief    Callback function for diagnostic response

\param    [in]    mgmtDiagRspData    Diagnostic response structure to be used in callback
*/
typedef void ( *mgmtDiagRspCallback_t)(mgmtDiagnostic_RspData_t *mgmtDiagRspData);

/*!
\brief    Callback function for diagnostic event

\param  [in]    reqLatency           Request latency
\param  [in]    seqNum               Sequence number
*/
typedef void (*mgmtDiagEventCallback_t) (uint32_t reqLatency, uint8_t seqNum);

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
\fn     void MgmtDiagnostic_Init(instanceId_t instanceId)
\brief  This function is used to initialize network diagnostic module.

\param  [in]    instanceId    Thread instance Id
***************************************************************************************************/
void MgmtDiagnostic_Init(instanceId_t instanceId);

/*!*************************************************************************************************
\fn     void MgmtDiagnostic_UnInit(void)
\brief  This function is used to un-initialize network diagnostic module.
***************************************************************************************************/
void MgmtDiagnostic_UnInit(void);

/*!*************************************************************************************************
\fn     thrStatus_t MgmtDiagnostic_RegisterAppCb(mgmtDiagRspCallback_t pfMgmtDiagRspAppCb)
\brief  This function is used to register network diagnostic application callback.

\param  [in]    pfMgmtDiagRspCallback   Function used to receive the network diagnostic responses
                                        at the application level.

\return         thrStatus_t             Success or Failed
***************************************************************************************************/
thrStatus_t MgmtDiagnostic_RegisterAppCb(mgmtDiagRspCallback_t pfMgmtDiagRspAppCb);

/*!*************************************************************************************************
\fn     thrStatus_t MgmtDiagnostic_RegisterEventCb(mgmtDiagEventCallback_t pfMgmtEvCb)
\brief  This function is used to register network diagnostic events callback.

\param  [in]    pfMgmtEvCb     Function used to receive the network diagnostic events
                               at the application level.

\return         thrStatus_t    Success or Failed
***************************************************************************************************/
thrStatus_t MgmtDiagnostic_RegisterEventCb(mgmtDiagEventCallback_t pfMgmtEvCb);

/*!*************************************************************************************************
\fn     thrStatus_t MgmtDiagnostic_SendGetReq(instanceId_t instanceId, mgmtDiagnostic_CoapMsg_t *pCoapMsg,
                                              uint16_t *pMsgId)
\brief  This function is used to send a Network Diagnostic Get Request.

\param  [in]    instanceId    Thread instance Id
        [in]    pCoapMsg      Pointer to CoAP message
        [out]   pMsgId        CoAP message Id: used for synchronization between Req and Rsp

\return         thrStatus_t   Status of the operation
***************************************************************************************************/
thrStatus_t MgmtDiagnostic_SendGetReq(instanceId_t instanceId, mgmtDiagnostic_CoapMsg_t *pCoapMsg,
                                      uint16_t *pMsgId);

/*!*************************************************************************************************
\fn     thrStatus_t MgmtDiagnostic_SendResetReq(instanceId_t instanceId, mgmtDiagnostic_CoapMsg_t *pCoapMsg,
                                                uint16_t *pMsgId)
\brief  This function is used to send a Network Diagnostic Reset Request.

\param  [in]    instanceId    Thread instance Id
        [in]    pCoapMsg      Pointer to CoAP message
        [out]   pMsgId        CoAP message Id: used for synchronization between Req and Rsp

\return         thrStatus_t   Status of the operation
***************************************************************************************************/
thrStatus_t MgmtDiagnostic_SendResetReq(instanceId_t instanceId, mgmtDiagnostic_CoapMsg_t *pCoapMsg,
                                        uint16_t *pMsgId);

/*!*************************************************************************************************
\fn     thrStatus_t MgmtDiagnostic_DiagTestReq(instanceId_t instanceId, mgmtDiagnostic_CoapMsg_t *pCoapMsg,
                                               uint16_t *pMsgId)
\brief  This function is used to send a Network Diagnostic Test Request (Large network testing).

\param  [in]    instanceId    Thread instance Id
        [in]    pCoapMsg      Pointer to CoAP message
        [out]   pMsgId        CoAP message Id: used for synchronization between Req and Rsp

\return         thrStatus_t   Status of the operation
***************************************************************************************************/
thrStatus_t MgmtDiagnostic_DiagTestReq(instanceId_t instanceId, mgmtDiagnostic_CoapMsg_t *pCoapMsg,
                                       uint16_t *pMsgId);

/*!*************************************************************************************************
\fn     uint32_t *MgmtDiagnostic_ProcessLinkData(instanceId_t instanceId, uint8_t inId, uint8_t opMode)
\brief  Process link data. Operation modes: Get, Reset, Increment 1 step

\param  [in]   instanceId    Thread instance id
\param  [in]   inId          Network diagnostic internal Id
\param  [in]   opMode        Operation mode: Get, Reset, Increment 1 step

\return        uint32_t*     Pointer to Link data
 ***************************************************************************************************/
uint32_t *MgmtDiagnostic_ProcessLinkData(instanceId_t instanceId, uint8_t inId, uint8_t opMode);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _THREAD_MGMT_H */
