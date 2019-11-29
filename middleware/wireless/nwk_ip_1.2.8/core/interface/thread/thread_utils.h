/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
**  @addtogroup thread_utilities
**  @{
*/
#ifndef _THREAD_UTILS_H
#define _THREAD_UTILS_H
/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "thread_network.h"
#include "thread_types.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! Thread Routing link margin to link cost table entry */
typedef struct thrLinkMargintoLinkQEntry_tag
{
    uint8_t min;
    uint8_t max;
    uint16_t quality;
}thrLinkMargintoLinkQEntry_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/

/*!*************************************************************************************************
\fn     uint32_t THR_NumOfIP6Addr(instanceId_t thrInstID, nwkIPAddrType_t nwkIpAddrType)
\brief  This function will return the number of bound IPv6 address of a specific type.

\param  [in]  thrInstID       Thread instance Id.
\param  [in]  nwkIpAddrType   Ip address type

\return       uint32_t        Number of IPv6 addresses
***************************************************************************************************/
uint32_t THR_NumOfIP6Addr(instanceId_t thrInstID, nwkIPAddrType_t nwkIpAddrType);

/*!*************************************************************************************************
\fn      bool_t THR_GetIP6Addr(instanceId_t thrInstID, nwkIPAddrType_t nwkIpAddrType,
                               ipAddr_t *pDstIPAddr, ipAddr_t *pLastDstIPAddr)
\brief   This function return the bound IPv6 addresses specified by IP address type parameter. At
         pDstIPAddr location should be allocated enough memory space to copy all the requested
         IPv6 addresses. Use the THR_NumOfIP6Addr() to find how much space is needed to get the
         requested IPv6 addresses.

\param  [in]      thrInstID           Thread instance Id.
\param  [in]      nwkIpAddrType       Ip address type
\param  [out]     pDstIPAddr          Resulted Ip Address
\param  [in]      pLastDstIPAddr      Last address resulted in case we have more than one

\return           bool_t              TRUE if successful or FALSE otherwise
***************************************************************************************************/
bool_t  THR_GetIP6Addr(instanceId_t thrInstID, nwkIPAddrType_t nwkIpAddrType, ipAddr_t *pDstIPAddr,
                       ipAddr_t *pLastDstIPAddr);

/*!*************************************************************************************************
\fn     uint8_t THR_GetNbOfGUAs(instanceId_t thrInstID)
\brief  This functions gets the number of bound GUA addresses

\param  [in]  thrInstID   Thread instance Id

\return       uint8_t     Number of GUAs
***************************************************************************************************/
uint8_t THR_GetNbOfGUAs(instanceId_t thrInstID);

/*!*************************************************************************************************
\fn     thrStatus_t THR_GetGuaByIndex(instanceId_t thrInstID, uint8_t index, uint8_t *pDstIPAddr)
\brief  This function returns the bound GUA IP address specifying the index. Use THR_GetNbOfGUAs()
        to get the number of GUS addresses.

\param  [in]    thrInstID    Thread instance Id
\param  [in]    index        Index
\param  [out]   pDstIPAddr   Resulted Ip Address

\return         thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t THR_GetGuaByIndex(instanceId_t thrInstID, uint8_t index, uint8_t *pDstIPAddr);

/*!*************************************************************************************************
\fn     uint8_t THR_GetNbOfAnycastAddr(instanceId_t thrInstID)
\brief  This functions gets the number of anycast addresses

\param  [in]  thrInstID   Thread instance Id

\return       uint8_t     Number of anycast addresses
***************************************************************************************************/
uint8_t THR_GetNbOfAnycastAddr(instanceId_t thrInstID);
/*!*************************************************************************************************
\fn     thrStatus_t THR_GetAnycastByIndex(instanceId_t thrInstID, uint8_t index, uint8_t *pDstIPAddr)
\brief  This function returns the anycast IP address specifying the index. Use THR_GetNbOfAnycastAddr()
        to get the number of anycast addresses.

\param  [in]    thrInstID    Thread instance Id
\param  [in]    index        Index
\param  [out]   pDstIPAddr   Resulted Ip Address

\return         thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t THR_GetAnycastByIndex(instanceId_t thrInstID, uint8_t index, uint8_t *pDstIPAddr);

/*!*************************************************************************************************
\fn     thrStatus_t THR_GetCommissionerALOCAddr(instanceId_t thrInstID, ipAddr_t* pCommALOCAddr)
\brief  This function returns the ALOC IP address of the commissioner binded to the current device.

\param  [in]    thrInstID       Thread instance Id
\param  [out]   pCommALOCAddr   Resulted Ip Address

\return         thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t THR_GetCommissionerALOCAddr(instanceId_t thrInstID, ipAddr_t* pCommALOCAddr);

/*!*************************************************************************************************
\fn     void THR_BuildNodeIpAddr(instanceId_t thrInstId, nwkIPAddrType_t thrAddrType,
                                 void *pNodeMacAddr, ipAddr_t *pNodeIpAddr)
\brief  This function builds the IPv6 address knowing the MAC address (short or long address)
        and the IP address type,

\param  [in]   thrInstID        Thread instance Id.
\param  [in]   nwkIpAddrType    Ip address type
\param  [in]   pNodeMacAddr     Pointer to Node MAC address (uint16_t or uint64_t).
                                MUST be properly aligned.
\param  [out]  pNodeIpAddr      Pointer to the storing location of the constructed IP address
***************************************************************************************************/
void THR_BuildNodeIpAddr(instanceId_t thrInstId, nwkIPAddrType_t thrAddrType,
                                    void *pNodeMacAddr, ipAddr_t *pNodeIpAddr);

/*!*************************************************************************************************
\fn             thrStatus_t THR_NwkSwitchKey(instanceId_t thrInstID, uint32_t thrKeySequenceCounter)
\brief          This function will switch the active key sequence (which is specified by nwkKeySeq
                attribute) with the thrKeySequenceCounter parameter.
                Note: The key for thrKeySequenceCounter should already be generated using
                THR_GenerateAllKeys().

\param  [in]  thrInstID               Thread instance Id.
\param  [in]  thrKeySequenceCounter   Key sequence counter

\return       thrStatus_t             Thread status
***************************************************************************************************/
thrStatus_t THR_NwkSwitchKey(instanceId_t thrInstID, uint32_t thrKeySequenceCounter);

/*!*************************************************************************************************
\fn     thrStatus_t THR_GenerateAllKeys(uint32_t thrInstId, uint32_t thrKeySeqNum)
\brief  This function will generate three keys: previous key associated with thrKeySeqNum-1,
        the current key associated with thrKeySeqNum (this is the active key)
        and the next key associated with thrKeySeqNum+1.
        NOTE: Use THR_NwkSwitchKey(thrKeySeqNum+1) to switch to next key associated with
        thrKeySeqNum+1.

\param  [in]  thrInstID       Thread instance Id.
\param  [in]  thrKeySeqNum    Key sequence number

\return       thrStatus_t     Status
***************************************************************************************************/
thrStatus_t THR_GenerateAllKeys(uint32_t thrInstId, uint32_t thrKeySeqNum);

 /*!*************************************************************************************************
\fn     THR_IsInstanceIdValid(instanceId_t thrInstId)
\brief  This function validates the thread instance id.

\param  [in]   thrInstID  Thread instance Id

\return [out]  bool_t     TRUE - Valid, FALSE - Invalid
***************************************************************************************************/
bool_t THR_IsInstanceIdValid(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     ifHandle_t THR_GetIpIfPtrByInstId(instanceId_t thrInstId)
\brief  Function that returns the corresponding interface handler, given the Thread instance ID.

\param  [in]    thrInstID     Thread instance ID

\return         ifHandle_t    Pointer to interface handler
***************************************************************************************************/
ifHandle_t THR_GetIpIfPtrByInstId(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     void THR_GetLeaderRLOCIpAddr(instanceId_t thrInstanceId, ipAddr_t *pLeaderIpAddr)
\brief  Function used to get the Leader Ip address (RLOC).

\param  [in]   thrInstanceId     Thread Instance Id
\param  [out]  pLeaderIpAddr     Pointer to leader Ip address. MUST be aligned to 64 bits
***************************************************************************************************/
void THR_GetLeaderRLOCIpAddr(instanceId_t thrInstanceId, ipAddr_t *pLeaderIpAddr);

/*!*************************************************************************************************
\fn     void THR_GetDHCPv6AgentRLOCIpAddr(instanceId_t thrInstanceId, ipAddr_t *pLeaderIpAddr)
\brief  Function used to get the RLOC for the closest DHCPv6 Agent

\param  [in]   thrInstanceId        Thread Instance Id
\param  [in]   shortAddr            DHCPv6 Agent ALOC (0xFC0X)
\param  [out]  pDHCP6AgentIpAddr    Pointer to the RLOC of the closest DHCPv6 Agent
***************************************************************************************************/
void THR_GetDHCPv6AgentRLOCIpAddr(instanceId_t thrInstanceId, uint16_t shortAddr,
                                  ipAddr_t *pDHCP6AgentIpAddr);

/*!*************************************************************************************************
\fn     thrStatus_t THR_SetPskcPassphrase(instanceId_t thrInstId, uint8_t *pPskcStr, uint8_t pskcStrLen)
\brief  Function used to set the PSKc. This converts the human readable password to non-readable
        value using the PBKDF2 for key stretching (see meshcop specification).

\param  [in]  thrInstId     Thread instance ID
\param  [in]  pPskcStr      Pointer to the human readable PSKc string
\param  [in]  pskcStrLen    Size of the PSKc string
***************************************************************************************************/
thrStatus_t THR_SetPskcPassphrase(instanceId_t thrInstId, uint8_t *pPskcStr, uint8_t pskcStrLen);

/*!*************************************************************************************************
\fn     void THR_GetParentLlIpAddr(instanceId_t thrInstanceId, ipAddr_t *pParentIpAddr)
\brief  Function used to get the parent Ip address (RLOC).

\param  [in]   thrInstanceId     Thread Instance Id
\param  [out]  pLeaderIpAddr     Pointer to parent Ip address. MUST be aligned to 64 bits
***************************************************************************************************/
void THR_GetParentLlIpAddr(instanceId_t thrInstanceId, ipAddr_t *pParentIpAddr);

/*!*************************************************************************************************
\fn     uint16_t THR_NeighborGetShortByExtAddr(uint8_t* pEui)
\brief  This function is used to get a neighbor's short address by its extended address.

\param  [in]  pEui      Pointer to neighbor's extended address

\return       uint16_t   Neighbor's short address
***************************************************************************************************/
uint16_t THR_NeighborGetShortByExtAddr(uint64_t* pEui);

/*!*************************************************************************************************
\fn     void THR_GeneratePskc(thrPskcInputParams_t *pParams)
\brief  Function used to compute the PSKc. This converts the human readable password to a
        non-readable value (to protect the actual user-generated passphrase).

\param  [in]  pParams                 pointer to the input parameters
***************************************************************************************************/
void THR_GeneratePskc(thrPskcInputParams_t *pParams);

/*!*************************************************************************************************
\fn     thrStatus_t THR_NwkStatusToThrStatus
\brief  This function converts from nwkStatus_t type status to thrStatus_t type status

\param  [in]  nwkStatus     Status value in nwkStatus_t type

\return       status        Status value in thrStatus_t type
***************************************************************************************************/
thrStatus_t THR_NwkStatusToThrStatus(nwkStatus_t nwkStatus);

/*!*************************************************************************************************
\fn     uint8_t THR_LinkMarginToQuality(uint8_t linkMargin)
\brief  This function converts from link margin to link quality.

\param  [in]  linkMargin  Link margin

\return       uint8_t     Link cost
***************************************************************************************************/
uint8_t THR_LinkMarginToQuality(uint8_t linkMargin);

/*!*************************************************************************************************
\fn     uint8_t THR_LinkQualityToCost(uint8_t linkQuality)
\brief  This function converts from link quality to link cost

\param  [in]  linkQuality   Link quality

\return       uint8_t       Link cost
***************************************************************************************************/
uint8_t THR_LinkQualityToCost(uint8_t linkQuality);

/*!*************************************************************************************************
\fn     uint8_t THR_LinkMarginAverage(uint8_t linkMargin, uint8_t prevLinkMargin)
\brief  This function calculates a exponentially weighted moving average of the link margin with a
        weighting of 1/THR_WEIGHTING_FACTOR

\param  [in]  linkMargin       Link margin
\param  [in]  prevLinkMargin   Previous link margin average value

\return       uint8_t          Link margin average
***************************************************************************************************/
uint8_t THR_LinkMarginAverage(uint8_t linkMargin, uint8_t prevLinkMargin);

/*!*************************************************************************************************
\fn     bool_t THR_IsIpAddrBasedOnMeshPrefix(instanceId_t thrInstId, ipAddr_t* pIpAddr)
\brief  Checks if the ip address is based on mesh local prefix or not.

\param  [in]  thrInstId    Thread instance id.
\param  [in]  pIpAddr      Pointer to ip address.

\return       bool_t       TRUE if the address is based on thread mesh prefix, FALSE otherwise.
***************************************************************************************************/
bool_t THR_IsIpAddrBasedOnMeshPrefix(instanceId_t thrInstId, ipAddr_t* pIpAddr);

/*!*************************************************************************************************
\fn     bool_t THR_IsMLEIDAddr(instanceId_t thrInstID,ipAddr_t *pIpAddr)
\brief  Checks if the ip address is ML-EID or not.

\param  [in]  thrInstID  Thread instance Id.
\param  [in]  pAddr      Pointer to Ip address


\return       bool_t     TRUE if the address is ML-EID, FALSE otherwise.
***************************************************************************************************/
bool_t THR_IsMLEIDAddr(instanceId_t thrInstID,ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn bool_t THR_IsRLOCAddr(instanceId_t thrInstID, ipAddr_t *pIpAddr)
\brief  Checks if the ip address is RLOC or not.

\param  [in]  thrInstID   Thread instance Id.
\param  [in]  pAddr       Ip address

\return       bool_t      TRUE if the address is RLOC, FALSE otherwise.
***************************************************************************************************/
bool_t THR_IsRLOCAddr(instanceId_t thrInstID, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     instanceId_t THR_GetInstId(uint32_t param, void *ptr, thrInstSearchType_t search)
\brief  Function that returns the corresponding Thread instance ID for the given parameters.
        The search can be done by giving an interface unique ID, a SLWP instance ID, a MAC instance
        ID, a Thread Instance or an interface handler structure.

\param  [in]    param           Value used for searching for the instance ID, depends on the
                                searchType parameter
\param  [in]    ptr             Pointer to the structure used for searching for the instance ID,
                                depends on the searchType parameter
\param  [in]    searchType      Type of search

\return         instanceId_t    Thread instance ID
***************************************************************************************************/
instanceId_t THR_GetInstId(uint32_t param, void *ptr, thrInstSearchType_t searchType);

/*!*************************************************************************************************
\fn thrStatus_t THR_SetPollingDevice(instanceId_t thrInstID, bool_t pollingOn)
\brief  This function sets polling on or off on a sleepy end device. When polling is off
        rxOnWhenIdle is TRUE.

\param[in]      thrInstID           Thread instance Id.
\param[in]      pollingOn           Polling on/off

\retval     thread status
***************************************************************************************************/
thrStatus_t THR_SetPollingDevice(instanceId_t thrInstID, bool_t pollingOn);

/*!*************************************************************************************************
\fn     nwkStatus_t THR_BeaconPayloadSet(threadInstance_t *pThreadInstance)
\brief  Update MAC beacon payload.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    pThreadBeacon           Pointer to the beacon payload
\param  [in]    beaconLen               Beacon payload length

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t THR_BeaconPayloadSet(instanceId_t thrInstId, uint8_t *pThreadBeacon, uint8_t beaconLen);

/*!*************************************************************************************************
\fn thrStatus_t THR_SetBorderRouterIf(instanceId_t thrInstID, ipIfUniqueId_t brIfId);
\brief      This function is used for configuring a device as a Thread Border Router by proving to
            external interface to use for routing outband packets

\param[in]      thrInstID               Thread instance Id.
\param[in]      brIfId                  Border router external interface Unique ID

\return         thrStatus_t             status
***************************************************************************************************/
thrStatus_t THR_SetBorderRouterIf(instanceId_t thrInstId,ipIfUniqueId_t brIfId);


/*!*************************************************************************************************
\fn     thrStatus_t THR_AddDnsServer(instanceId_t thrInstID, ipAddr_t *dnsIpaddr)
\brief  This is a public function used to add a DNS Server IP address for use with DHCPv6 Server.

\param  [in]    thrInstID           Thread instance ID
\param  [in]    dnsIpaddr           The DNS Server IP address

\return         thrStatus_t
***************************************************************************************************/
thrStatus_t THR_AddDnsServer(instanceId_t thrInstID, ipAddr_t *dnsIpaddr);


/*================================================================================================*/
#endif  /* _THREAD_UTILS_H */
/*!
** @}
*/
