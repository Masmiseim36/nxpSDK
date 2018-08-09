/*! *********************************************************************************
 * \defgroup CLB  Thread Application Callbacks Interface
 * @{
 ***********************************************************************************/
/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
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

#ifndef _THREAD_APP_CALLBACKS_H
#define _THREAD_APP_CALLBACKS_H

/*!
\file       thread_app_callbacks.h
\brief      This is a header file for the Application Callbacks module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "thread_types.h"
#include "thread_meshcop.h"
#include "thread_meshcop_mgmt.h"

#include "dhcp6_client.h"
#include "dhcp6_server.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*! Maximum number of networks to perform the joining procedure */
#ifndef THR_MAX_NWK_JOINING_ENTRIES
    #define THR_MAX_NWK_JOINING_ENTRIES               2
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*!< Structure for holding pointer to the IPv6 address of a server service advertised in network data */
typedef void (*registerServiceServerAddr_t) (ipAddr_t* pIpAddr);

/*==================================================================================================
Public global variables declarations
==================================================================================================*/


/*==================================================================================================
Public function prototypes
==================================================================================================*/

/*!*************************************************************************************************
\fn     void APP_JoinerSelectNwkWithBeaconCb(void *pParam)
\brief  This function is used to handle the network events during the meshcop joining. This is the
        callback function used to select the potential network to join when THR_NwkJoin() is called.

\param  [in]  pParam -  pointer to event messages (thrEvmParams_t *)
***************************************************************************************************/
void APP_JoinerSelectNwkWithBeaconCb(void *pParam);

/*!*************************************************************************************************
\fn     bool_t APP_OutOfBandSelectNwkWithBeaconCb(instanceId_t thrInstId, thrBeaconInfo_t *pThrBeacon)
\brief  This is the callback function used to select a thread network (find the panId, channel etc.)
        when the device in out-of-band configured and THR_NwkJoin(thrInst, gUseMACBeacon_c) is called.
        This function should filter the received beacons and select a thread network to start the
        attachment process.

\param  [in]  thrInstId   The thread instance ID
\param  [in]  pThrBeacon  Pointer to received Beacon

\return       TRUE        A network has been selected
\return       FALSE       No network has been selected
***************************************************************************************************/
bool_t APP_OutOfBandSelectNwkWithBeaconCb(instanceId_t thrInstId, thrBeaconInfo_t *pThrBeacon);

/*!*************************************************************************************************
\fn     bool_t APP_MeshcopValidateJoinerAddrCb(instanceId_t thrInstId, ipAddr_t *pIpAddr)
\brief  This is the callback function used to check if a Joiner will be accepted by our DTLS server.

\param  [in]  thrInstId     The thread instance ID
\param  [in]  pIpAddr       Pointer to client IP address

\return       TRUE          The Joiner is known
\return       FALSE         The Joiner is unknown
***************************************************************************************************/
bool_t APP_MeshcopValidateJoinerAddrCb(instanceId_t thrInstId, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     bool_t APP_MeshCopValidateJoinFinCb(instanceId_t thrInstId, meshCopJoinFinTlvs_t* pJoinFinTlvs)
\brief  Function used to check the TLVs given by the Joiner in the Join Finalization step.

\param  [in]  thrInstId      Thread instance ID
\param  [in]  pJoinFinTlvs   Join Finalization TLVs

\return       TRUE           Continue joining
\return       FALSE          Otherwise
***************************************************************************************************/
bool_t APP_MeshCopValidateJoinFinCb(instanceId_t thrInstId, meshCopJoinFinTlvs_t* pJoinFinTlvs);

/*!*************************************************************************************************
\fn     bool_t APP_MeshCopValidateCommissionerCb(instanceId_t thrInstId, meshcopCommIdTlv_t *pCommIdTlv)
\brief  Function used to check the Commissioner ID. It can be accepted or rejected.

\param  [in]  thrInstId   Thread instance ID
\param  [in]  pCommIdTlv  Pointer to the Commissioner ID TLV

\return       TRUE        Allow this Commissioner
\return       FALSE       Reject this Commissioner
***************************************************************************************************/
bool_t APP_MeshCopValidateCommissionerCb(instanceId_t thrInstId, meshcopCommIdTlv_t *pCommIdTlv);

/*!*************************************************************************************************
\fn     bool_t APP_AddressAssignSlaacCb(instanceId_t thrInstId, ipAddr_t* pPrefix)
\brief  If slaacPolicy attribute is configured to gThrSlaacManual_c this function serves as a
        callback to the application to decide if it wants to bind an address with the prefix or not
        and if so, the application can choose the IID to use with the provided prefix.

\param  [in]     thrInstId  Thread instance ID
\param  [in/out] pPrefix    Pointer to ip prefix and output to store the IID

\return          TRUE       If the address generated with the prefix and IID should be used by the stack
\return          FALSE      If the application does not want to use this prefix and the stack should ignore it
***************************************************************************************************/
bool_t APP_AddressAssignSlaacCb(instanceId_t thrInstId, ipAddr_t* pPrefix);

/*!*************************************************************************************************
\fn     bool_t APP_ServiceServerDataCb(serviceSet_t serviceSet)
\brief  If in a Service TLV, from Network Data TLV, is received a DNS/OTA server service, this
        function serves as a callback to the application to set the IP address of the DNS/OTA server.

\param  [in]  serviceSet       The service set structure which also embodies info about the server.
\param  [in]  bAddService      TRUE - Add service
                               FALSE - Remove service

\return       TRUE             If the service name equals "dnsserver" or "otaserver"
\return       FALSE            Otherwise
***************************************************************************************************/
bool_t APP_ServiceServerDataCb(serviceSet_t serviceSet, bool_t bAddService);

/*!*************************************************************************************************
\fn     void APP_CriticalExitCb(uint32_t location, uint32_t param)
\brief  If the stack is in a deadlock situation, it calls APP_CriticalExitCb.

\param  [in]  location  Address where the Panic occurred
\param  [in]  param     Parameter with extra debug information
***************************************************************************************************/
void APP_CriticalExitCb(uint32_t location, uint32_t param);

/*!*************************************************************************************************
\fn     APP_DiscoveryReqCb(instanceId_t thrInstId,  uint16_t tlvsSize, uint8_t* pTlvs)
\brief  This is a callback used by the Application to accept or deny the Discovery Requests.
        The Discovery Request messages could contain some application specific TLVs, and the
        APP could have filters based on these TLVs.

\param  [in]  thrInstId  Thread instance ID
\param  [in]  tlvsSize   Discovery request TLVs size
\param  [in]  pTlvs      Pointer to Discovery Request TLVs

\return       TRUE       Send the Discovery Response
\return       FALSE      Otherwise
***************************************************************************************************/
bool_t APP_DiscoveryReqCb(instanceId_t thrInstId,  uint16_t tlvsSize, uint8_t* pTlvs);

/*!*************************************************************************************************
\fn     void APP_JoinerDiscoveryRespCb(instanceId_t thrInstId, thrDiscoveryEvent_t event, uint8_t lqi,
                                       thrDiscoveryRespInfo_t* pDiscoveryRespInfo,
                                       meshcopDiscoveryRespTlvs_t *pDiscoveryRespTlvs)
\brief  This callback can be used by the application to handle and filter the
        Discovery Response messages. This function is application specific and could build a
        list of Joiner Routers to start the Meshcop joining process.

\param  [in]     thrInstId             Thread instance ID
\param  [in]     event                 Discovery event
\param  [in]     lqi                   Discovery response packet lqi
\param  [in]     pDiscoveryRespInfo    Discovery Response pan information
\param  [in]     pDiscoveryRespTlvs    Pointer to Discovery Response TLVs
***************************************************************************************************/
void APP_JoinerDiscoveryRespCb(instanceId_t thrInstId, thrDiscoveryEvent_t event, uint8_t lqi,
                               thrDiscoveryRespInfo_t* pDiscoveryRespInfo,
                               meshcopDiscoveryRespTlvs_t *pDiscoveryRespTlvs);

/*!*************************************************************************************************
\fn     void APP_JoinerSelectNwkWithAnnounceCb(instanceId_t thrInstId, thrAnnounceEvent_t event,
                                               uint8_t lqi, uint16_t tlvsSize, uint8_t* pTlvs)
\brief  This callback handles the announcement messages to select
        a Thread Network (channel and panId) and start the attachment process.

\param  [in]  thrInstId   Thread instance
\param  [in]  event       Announcement events (see thrAnnounceEvent_t)
\param  [in]  lqi         Received packet lqi
\param  [in]  tlvsSize    The size of the received Announce TLVs
\param  [in]  pTlvs       Pointer to Announce TLVs
***************************************************************************************************/
void APP_JoinerSelectNwkWithAnnounceCb(instanceId_t thrInstId, thrAnnounceEvent_t event,
                                       uint8_t lqi, uint16_t tlvsSize, uint8_t* pTlvs);

/*!*************************************************************************************************
\fn     void APP_GenerateMLPrefixCb(instanceId_t thrInstID, thrPrefixAttr_t* pMLprefix)
\brief  This callback is called by Thread Stack to generate the MLprefix

\param  [in]   thrInstID  Thread instance ID
\param  [out]  pMLprefix  ML prefix
***************************************************************************************************/
void APP_GenerateMLPrefixCb(instanceId_t thrInstID, thrPrefixAttr_t* pMLprefix);

/*!*************************************************************************************************
\fn     void APP_EnableDHCP6Cb(void)
\brief  This callback can be used by the application to initialize the callbacks for DHCPv6 module.
***************************************************************************************************/
void APP_EnableDHCP6Cb(void);

/*!*************************************************************************************************
\fn     void APP_BeaconFillCb(instanceId_t thrInstID)
\brief  This callback can be used by the application to set the beacon payload.

\param  [in]   thrInstID  Thread instance ID
***************************************************************************************************/
void APP_BeaconFillCb(instanceId_t thrInstID);

/*================================================================================================*/
#endif  /* _THREAD_APP_CALLBACKS_H */
