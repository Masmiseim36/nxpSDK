/*! *********************************************************************************
 * \defgroup NWK  Thread Network Interface
 * @{
 ***********************************************************************************/
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _THREAD_NETWORK_H
#define _THREAD_NETWORK_H

/*!
\file       thread_network.h
\brief      This is a header file for the Thread Network module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "network_utils.h"
#include "stack_manager_if.h"
#include "thread_types.h"
#include "thread_attributes.h"
#include "event_manager.h"
#include "thread_meshcop.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*! Thread Device Capabilities */
#define THR_NWKCAP_CAN_CREATE_NEW_NETWORK       (1<<0) /*!< The node can create a new network */
#define THR_NWKCAP_CAN_BECOME_ACTIVE_ROUTER     (1<<1) /*!< The node can become an active router */
#define THR_NWKCAP_IS_POLLING_END_DEVICE        (1<<2) /*!< The node is a polling end device (sleepy end device) */
#define THR_NWKCAP_IS_FULL_THREAD_DEVICE        (1<<3) /*!< The node is a full Thread device (FTD) */

/* Update according to number of capabilities flags! */
/*! Thread Device Capabilities bit mask */
#define THR_NWKCAP_BIT_MASK                      0x1F

/*! Network scan events */
typedef enum
{
    gThrEv_NwkScanCnf_Results_c = gThrEvSet_NwkScan_c + 1,    /*!< nwk scan confirm -  results */
} thrEvCodesNwkScan_t;

/*! Network Create Events */
typedef enum
{
    gThrEv_NwkCreateCnf_Success_c = gThrEvSet_NwkCreate_c + 1, /*!< nwk create confirm - success */
    gThrEv_NwkCreateCnf_Failed_c,                              /*!< nwk create confirm - failed */
    gThrEv_NwkCreateInd_SelectBestChannel_c,                   /*!< nwk create indication - select best channel */
    gThrEv_NwkCreateInd_GeneratePSKc_c                         /*!< nwk create indication - generate PSKc */
} thrEvCodesCreate_t;

/*! Network Join Events */
typedef enum
{
    gThrEv_NwkJoinInd_Attaching_c = gThrEvSet_NwkJoin_c + 1,        /*!< nwk join indication - attaching */
    gThrEv_NwkJoinCnf_Success_c,                                    /*!< nwk join confirm - success */
    gThrEv_NwkJoinCnf_Failed_c,                                     /*!< nwk join confirm - failed */
} thrEvCodesJoin_t;

/*! Network Select Parent when joining with Commissioner  */
typedef enum
{
    gThrEv_NwkSelectParentsInd_ScanStarted_c = gThrEvSet_NwkSelectParents_c + 1,  /*!< network select parent indication - scan started */
    gThrEv_NwkSelectParentsInd_RcvBeacon_c,                                       /*!< network select parent indication - received beacon */
    gThrEv_NwkSelectParentsInd_ScanEnded_c,                                       /*!< network select parent indication - scan ended */
} thrEvCodesJoinSelectParent_t;

/*! Network General Events - warning the order of events impacts the THCI event monitor */
typedef enum
{
    gThrEv_GeneralInd_Disconnected_c = gThrEvSet_NwkGeneral_c + 1,      /*!< general event indication - disconnected */
    gThrEv_GeneralInd_Connected_c,                                      /*!< general event indication - connected */

    gThrEv_GeneralInd_ResetToFactoryDefault_c,                          /*!< general event indication - device started with factory defaults */
    gThrEv_GeneralInd_InstanceRestoreStarted_c,                         /*!< general event indication - start restore from reset */

    gThrEv_GeneralInd_RouterSynced_c,                                   /*!< general event indication - restored from reset with success for router */
    gThrEv_GeneralInd_EndDeviceSynced_c,                                /*!< general event indication - restored from reset with success for end device */

    gThrEv_GeneralInd_ConnectingStarted_c,                              /*!< general event indication - trying to connect to the network */
    gThrEv_GeneralInd_ConnectingFailed_c,                               /*!< general event indication - failed to connect to the network */
    gThrEv_GeneralInd_ConnectingDeffered_c,                             /*!< general event indication - app must initiate connect action */

    gThrEv_GeneralInd_DeviceIsLeader_c,                                 /*!< general event indication - device has leader role */
    gThrEv_GeneralInd_DeviceIsRouter_c,                                 /*!< general event indication - device has router role */
    gThrEv_GeneralInd_DevIsREED_c,                                      /*!< general event indication - device has REED role */
    gThrEv_GeneralInd_DevIsFED_c,                                       /*!< general event indication - device has RX on when idle end device role */
    gThrEv_GeneralInd_DevIsSED_c,                                       /*!< general event indication - device has sleepy end device role */

    gThrEv_GeneralInd_RequestGlobalAddr_c,                              /*!< general event indication - request global address */
    gThrEv_GeneralInd_GlobalAddrAssigned_c,                             /*!< general event indication - global address assigned */
    gThrEv_GeneralInd_RequestRouterId_c,                                /*!< general event indication - request router short address */
    gThrEv_GeneralInd_RouterIdAssigned_c,                               /*!< general event indication - router short address assigned */
    gThrEv_GeneralInd_RouterIdAssignedFailed_c,                         /*!< general event indication - failed to received router short address */
    gThrEv_GeneralInd_AllowDeviceToSleep_c,                             /*!< general event indication - allow device to sleep */
    gThrEv_GeneralInd_DisallowDeviceToSleep_c,                          /*!< general event indication - disallow device to sleep */
    gThrEv_GeneralInd_ChildIdAssigned_c,                                /*!< general event indication - child short address assigned */

    gThrEv_GeneralInd_DevIsMED_c,                                       /*!< general event indication - device has minimal end device role */

    gThrEv_GeneralInd_ChildRemoved_c,                                   /*!< general event indication - child removed */
    gThrEv_GeneralInd_AllChildrenRemoved_c,                             /*!< general event indication - all children removed */
    gThrEv_GeneralInd_RouterRemoved_c,                                  /*!< general event indication - router removed */
    gThrEv_GeneralInd_RoutingUpdates_c,                                 /*!< general event indication - updates in routing table */
    gThrEv_GeneralInd_ChildUpdateRspRcv_c,                              /*!< general event indication - Child Update message received */

    gThrEv_GeneralInd_ResetMcuTimeout_c,                                /*!< general event indication - reset mcu timeout */

} thrEvCodesGeneral_t;


/*! thread device configuration */
typedef struct thrDeviceConfig_tag
{
    /*! If TRUE than the device is out-of-band configured.
        - On network creation, it is not used.
        - On joining,  if it is set TRUE the THR_NwkJoin() will perform only the attaching procedure;
        otherwise it will perform the joining with Commissioner procedure (mesh-cop joining). */
    bool_t       outOfBandConfigured;

    /*! Network creation channel. If different form 0, On network creation (THR_NwkCreate()), will
        OVERRIDE the SCAN channel and only use this channel. Range: 0, 11-26 */
    uint8_t      outOfBandChannel;

    /*! - On network creation (THR_NwkCreate()), the configured value will be used or if it is set to
        0xffff then the device will generate a random pan ID .*/
    uint16_t     panId;

    /*! The channel mask used for scanning for networks and to discover network parameters
        (panId, channel, xpan, network name)*/
    uint32_t     scanChannels;

    /*! - On network creation (THR_NwkCreate()), the configured value will be used or if all bytes are
        0xff then the device will generate a random extended pan ID.
        - On joining using out-of-band configuration (outOfBandConfigured = TRUE), if all bytes are 0xff
        then the device won't filter after the extended pan ID; otherwise it uses this extended pan id
        for filtering. */
    uint8_t      xPanId[8];

    /*! - On network creation (THR_NwkCreate()), the configured value will be used or if all bytes are
        0xff then the device will generate a random master key.
        - On joining using out-of-band configuration (outOfBandConfigured = TRUE), the device uses
        the configured key for communication. */
    uint8_t      masterKey[16];

    /*! On joining with the out-of-band configuration (outOfBandConfigured = TRUE), if (outOfBandChannel == 0)
    and nwkName.length != 0, the device will filter after network name to find the pan id and channel. */
    thrOctet16_t nwkName;

    /*!- On network creation (THR_NwkCreate()), the configured value will be used or if all bytes are
       0xff then the device will generate a random mesh local prefix. */
    thrPrefixAttr_t   MLprefix;
} thrDeviceConfig_t;

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
/*! Thread device configuration */
extern thrDeviceConfig_t   gaThrDeviceConfig[];

/*==================================================================================================
Public function prototypes
==================================================================================================*/
/*!*************************************************************************************************
\fn     void THR_Task(osaTaskParam_t argument)
\brief  Thread application task.

\param [in]  argument  Task private data

\return      NONE
***************************************************************************************************/
void THR_Task(osaTaskParam_t argument);

/*!*************************************************************************************************
\fn     void THR_Init(void)
\brief  Initialize Thread module.

\return      NONE
***************************************************************************************************/
void THR_Init(void);

/*!*************************************************************************************************
\fn     thrStatus_t THR_InitAttributes(instanceId_t thrInstId, stackConfig_t *pStackCfg)
\brief  Function that initializes with factory defaults or restores from NVM the Thread Attributes.

\param  [in]  thrInstId    Thread instance Id
\param  [in]  pStackCfg    Pointer to stack configuration

\return       thrStatus_t  Result of the operation
***************************************************************************************************/
thrStatus_t THR_InitAttributes(instanceId_t thrInstId, stackConfig_t *pStackCfg);

/*!*************************************************************************************************
\fn     thrStatus_t THR_StartInstance(instanceId_t thrInstId, stackConfig_t *pStackCfg)
\brief  Function that starts the Thread instance.

\param  [in]  thrInstID      Thread instance ID
\param  [in]  pStackCfg      Pointer to stack configuration

\return       thrStatus_t    Result of the operation
***************************************************************************************************/
thrStatus_t THR_StartInstance(instanceId_t thrInstId, stackConfig_t *pStackCfg);

/*!*************************************************************************************************
\fn     thrStatus_t THR_SetDeviceConfig(instanceId_t thrInstId, thrDeviceConfig_t *pThrDeviceConfig)
\brief  This function is used to set device configuration. This function overwrites the default
        settings (see app_thread_config.h) with a minimum set of attributes needed to start a node.
        The application may not call this function if it wants to use the default settings.

\param [in]  thrInstID          Thread instance Id
\param [in]  pThrDeviceConfig   Pointer to device configuration

\return      thrStatus_t        Status
***************************************************************************************************/
thrStatus_t THR_SetDeviceConfig(instanceId_t thrInstId, thrDeviceConfig_t *pThrDeviceConfig);

/*!*************************************************************************************************
\fn thrStatus_t THR_SetDeviceRole(instanceId_t thrInstID, thrDeviceRole_t thrDeviceRole)
\brief  This is a public function used to set the network capabilities for a Thread device.

\param [in]  thrInstID       Thread instance ID
\param [in]  thrDeviceRole   Thread desired device role.

\return      thrStatus_t     Status
***************************************************************************************************/
thrStatus_t THR_SetDeviceRole(instanceId_t thrInstID, thrDeviceRole_t thrDeviceRole);

/*!*************************************************************************************************
\fn     thrStatus_t THR_NwkScanWithBeacon(instanceId_t thrInstId, thrNwkScan_t *pThrNwkScan)
\brief  This function is used to start a network scan using beacon messages.
        A callback function must be registered (using EVM_RegisterStatic() function) with the
        gThrEvSet_NwkScan_c set to receive the scan results (see thrNwkScanResults_t message).

\param [in]  thrInstID     Thread instance Id
\param [in]  pThrNwkScan   Network scan parameters

\return      thrStatus_t   Status
***************************************************************************************************/
thrStatus_t THR_NwkScanWithBeacon(instanceId_t thrInstId, thrNwkScan_t *pThrNwkScan);

/*!*************************************************************************************************
\fn     thrStatus_t THR_NwkDiscoveryReq(instanceId_t thrInstId,thrNwkDiscoveryReqTxOpt_t* pDiscReqTxOpt,
        thrDiscoveryRespCb_t pfDiscoveryRespCb)
\brief  This function starts the Thread Discovery Procedure. A callback function must be registered
        (pfDiscoveryRespCb) to receive the Discovery Responses.

\param [in]  thrInstID           Thread instance Id
\param [in]  pDiscReqTxOpt       Pointer to Discovery Request Tx options
\param [in]  pfDiscoveryRespCb   Pointer to a callback to receive the Discovery Responses

\return      thrStatus_t         Status
***************************************************************************************************/
thrStatus_t THR_NwkDiscoveryReq(instanceId_t thrInstId,
                                thrNwkDiscoveryReqTxOpt_t *pDiscReqTxOpt, thrDiscoveryRespCb_t pfDiscoveryRespCb);

/*!*************************************************************************************************
\fn     thrStatus_t THR_NwkDiscoveryStop(instanceId_t thrInstId)
\brief  This function stops the discovery process before timing out.

\param [in] thrInstID    Thread instance Id

\return     thrStatus_t  Status
***************************************************************************************************/
thrStatus_t THR_NwkDiscoveryStop(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     thrStatus_t THR_SearchThreadNwkWithAnnounce(instanceId_t thrInstId,uint32_t scanChannelMask,
        thrAnnounceCb_t pfAnnounceCb)
\brief  The device has the Nwk key and searches the thread network using the announcement messages.
        Only Thread networks that have the same Nwk key will respond.
        This function could be used to discover the channel and panId of a Thread network,
        so that to start the attachment process (to perform the out-of-band joining procedure).

\param [in]  thrInstID         Thread instance Id
\param [in]  scanChannelMask   Channel mask to scan
\param [in]  pfAnnounceCb      Pointer to a callback that handles the received announcement messages.

\return      thrStatus_t       Status
***************************************************************************************************/
thrStatus_t THR_SearchThreadNwkWithAnnounce(instanceId_t thrInstId,
        uint32_t scanChannelMask, thrAnnounceCb_t pfAnnounceCb);

/*!*************************************************************************************************
\fn     thrStatus_t THR_NwkCreate(instanceId_t thrInstId)
\brief  This function is used to create a Thread network (start the device as a leader node).
        If the PAN ID and channel attributes are configured (panid != 0xFFFF and channel != 0), the
        function will start the leader using these attributes. Otherwise, the scanChannelMask
        attribute is used to select the best channel and panID.
        A callback function is registered (see thread_app_callbacks.h and app_thread_callbacks.c)
        with the gThrEvSet_NwkCreate_c event set to receive the network creation events(see
        APP_NwkCreateCb callback).
        Note that THR_NWKCAP_CAN_CREATE_NEW_NETWORK bit must be set in the network capabilities.

\param [in]  thrInstID    Thread instance Id

\return      thrStatus_t  Status
***************************************************************************************************/
thrStatus_t THR_NwkCreate(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     thrStatus_t THR_NwkAttach(instanceId_t thrInstId)
\brief  This function is used to perform the attachment procedure using the configured attributes:
        channel, panId, network key.
        NOTE:
        - It can be used for out-of-band joining.
        - A callback function must be registered (using EVM_RegisterStatic() function) with the
        gThrEvSet_NwkJoin_c event set to receive the network join events.

\param [in]  thrInstID     Thread instance Id

\return      thrStatus_t   Status
***************************************************************************************************/
thrStatus_t THR_NwkAttach(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     thrStatus_t THR_NwkJoin(instanceId_t thrInstId, thrJoinDiscoveryMethod_t discMethod)
\brief  This function is used to join a device to a thread network and is using all the above
        functionality. Depending on attributes configuration, it can perform the following actions:
        1. join as an end node using commissioning (calling MESHCOP_NwkJoin()).
           In this case, the device is NOT out-of-band configured;this means the devIsCommissioned
           attribute shall be set to FALSE. Depending on discovery method parameter (discMethod),
           the device will search the panId and channel using Beacon or Thread Discovery messages.
        2. attach to a thread network (calling THR_NwkAttach()). In this case, the device must
           be out-of-band configured;this means the devIsCommissioned attribute shall be set to TRUE.
           Depending on discovery method parameter (discMethod), the device will search the panId and
           channel using Beacon or Announcement messages (see THR_SearchThreadNwkWithAnnounce()).
        A callback function must be registered (using EVM_RegisterStatic() function) with the
        gThrEvSet_NwkJoin_c event set to receive the network join events.

\param [in]  thrInstID    Thread instance Id
\param [in]  discMethod   The discovery method (see thrJoinDiscoveryMethod_t)

\return      thrStatus_t  Status
***************************************************************************************************/
thrStatus_t THR_NwkJoin(instanceId_t thrInstId, thrJoinDiscoveryMethod_t discMethod);

/*!*************************************************************************************************
\fn     thrStatus_t THR_NwkDetach(instanceId_t thrInstId)
\brief  This function is used to detach a joined device. The device will keep the network settings but
        will disconnect itself from the network.

\param [in]  thrInstID     Thread instance Id

\return      thrStatus_t   Status
***************************************************************************************************/
thrStatus_t THR_NwkDetach(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     void THR_SetRxOnWhenJoining(uint32_t thrInstId, bool_t storePreviousRxState)
\brief  Function used to toggle the receiver state when joining.

\param  [in]    thrInstId               Thread Instance ID
\param  [in]    storePreviousRxState    Flag to store the previous RX state
***************************************************************************************************/
void THR_SetRxOnWhenJoining(uint32_t thrInstId, bool_t storePreviousRxState);

/*!*************************************************************************************************
\fn     thrStatus_t THR_SoftwareReset(instanceId_t thrInstID, bool_t factoryReset)
\brief  This function is used to do a thread software reset.

\param [in]  thrInstID      Thread instance Id
\param [in]  factoryReset   If TRUE, the device will be reseted to factory

\return      thrStatus_t   Status
***************************************************************************************************/
thrStatus_t THR_SoftwareReset(instanceId_t thrInstID, bool_t factoryReset);

/*!*************************************************************************************************
\fn     void THR_FactoryReset(void)
\brief  This function is used to reset device to factory default settings.

\return      NONE
***************************************************************************************************/
void THR_FactoryReset(void);

/*!*************************************************************************************************
\fn     void THR_TimeoutResetMcu(uint32_t timeoutMs, bool_t resetToFactory)
\brief  This function is used to reset the device after a specific timeout.

\param [in]  timeoutMs       Time expressed in milliseconds units.
       [in]  resetToFactory  If TRUE, the device will be reseted to factory

\return      NONE
***************************************************************************************************/
void THR_TimeoutResetMcu(uint32_t timeoutMs, bool_t resetToFactory);

/*!*************************************************************************************************
\fn     thrNeighbor_t *  THR_GetParent(instanceId_t thrInstID)
\brief  This is a public function used to get information about parent when node is an end device
        and REED or about the attaching parent in case of a router. For a leader it will return NULL.

\param [in]  thrInstID       Thread instance ID

\return      thrNeighbor_t*  Pointer to neighbor structure.
***************************************************************************************************/
thrNeighbor_t *THR_GetParent(instanceId_t thrInstID);

/*!*************************************************************************************************
\fn     thrNeighbor_t * THR_GetNeighborTable(uint32_t iCount)
\brief  This function is used to get the neighbor from a given index from the neighbor table.

\param [in]  iCount            Entry in neighbor table

\return      thrNeighbor_t *   Pointer to neighbor structure.
 **************************************************************************************************/
thrNeighbor_t *THR_GetNeighborTable(uint32_t iCount);

/*!*************************************************************************************************
\fn     uint16_t THR_NeighborGetShortByExtAddr(uint8_t* pEui)
\brief  This function is used to get a neighbor's short address by its extended address.

\param [in]  pEui        Pointer to neighbor's extended address

\return      uint16_t    Neighbor's short address
***************************************************************************************************/
uint16_t THR_NeighborGetShortByExtAddr(uint64_t *pEui);

/*!*************************************************************************************************
\fn     thrNeighbor_t * THR_NeighborGetByShort(uint16_t shortAddr)
\brief  This function is used to get a neighbor's information by its short address.

\param [in]  shortAddr       Neighbor's short address

\return      thrNeighbor_t   Neighbor info
***************************************************************************************************/
thrNeighbor_t *THR_NeighborGetByShort(uint16_t shortAddr);

/*!*************************************************************************************************
\fn     thrRouterIdSet_t* THR_GetRouterIdSet(instanceId_t thrInstId)
\brief  This function is used to get the thread router ID set.

\param  [in]  thrInstId           Thread instance id

\return       thrRouterIdSet_t*   Pointer to router ID set structure
***************************************************************************************************/
thrRouterIdSet_t *THR_GetRouterIdSet(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     thrStatus_t THR_LeaderRemoveRouterID(instanceId_t thrInstID, uint32_t routerID)
\brief  This  function is used to remove a router from network. It should be called only on the
        leader node.

\param  [in]  thrInstID    Thread instance ID
\param  [in]  routerID     The ID of the router to be removed

\return       thrStatus_t  Status
***************************************************************************************************/
thrStatus_t THR_LeaderRemoveRouterID(instanceId_t thrInstID, uint32_t routerID);

/*!*************************************************************************************************
\fn     thrStatus_t THR_RouterLinkSync(instanceId_t thrInstID, bool_t bOnReset)
\brief  This function will perform the "link synchronization after reset" or
        "initial link synchronization" procedure. Must be called only on a router node.

\param [in]  thrInstID    Thread instance Id
\param [in]  bOnReset     Specify if the router should do the "Router synchronization after reset"
                          procedure or the "Initial Router synchronization" procedure.

\return      thrStatus_t  Status
***************************************************************************************************/
thrStatus_t THR_RouterLinkSync(instanceId_t thrInstID, bool_t bOnReset);

/*!*************************************************************************************************
\fn     thrStatus_t THR_ChildUpdateToParent(instanceId_t thrInstID)
\brief  This function is used to send a ChildUpdate.Request to synchronize the parent with the
        updated attributes (timeout period, node mode flags TLV).

\param [in]  thrInstID    Thread instance Id

\return      thrStatus_t  Status
***************************************************************************************************/
thrStatus_t THR_ChildUpdateToParent(instanceId_t thrInstID);

/*!*************************************************************************************************
\fn     thrStatus_t THR_ChildUpdateKeepAlive(instanceId_t thrInstID)
\brief  This function is used to send a Child Update Keep Alive message from a child to its parent.

\param  [in]    thrInstID   Thread instance id.

\return         thrStatus_t status
***************************************************************************************************/
thrStatus_t THR_ChildUpdateKeepAlive(instanceId_t thrInstID);

/*!*************************************************************************************************
\fn     thrStatus_t THR_SolicitGlobalAddress(instanceId_t thrInstID)
\brief  This function solicits a global address from a DHCPv6 server.

\param [in]  thrInstID    Thread instance Id

\return      thrStatus_t  Status
***************************************************************************************************/
thrStatus_t THR_SolicitGlobalAddress(instanceId_t thrInstID);

/*!*************************************************************************************************
\fn     thrStatus_t THR_BrPrefixAttrAddEntry(instanceId_t thrInstID, thrOtaBrPrefixSet_t *pEntry)
\brief  Add a Border router prefix attribute entry.

\param [in]  thrInstID      Thread instance Id
\param [in]  pEntry         Border router nwk data entry

\return      thrStatus_t    Status
***************************************************************************************************/
thrStatus_t THR_BrPrefixAttrAddEntry(instanceId_t thrInstID, thrOtaBrPrefixSet_t *pEntry);

/*!*************************************************************************************************
\fn     thrStatus_t THR_ServiceAttrAddEntry(instanceId_t thrInstID, thrLocalServiceSet_t *pEntry)
\brief  Add a Border router service attribute entry.

\param [in]   thrInstID      Thread instance Id
\param [in]   pEntry         Border router service set entry

\return       thrStatus_t    Status
 **************************************************************************************************/
thrStatus_t THR_ServiceAttrAddEntry(instanceId_t thrInstID, thrLocalServiceSet_t *pEntry);

/*!*************************************************************************************************
\fn     thrStatus_t THR_BrPrefixAttrRemoveEntry(instanceId_t thrInstID,uint8_t prefixLength,
        uint8_t *pPrefixValue)
\brief  Remove Border Router prefix attribute entry.

\param [in]  thrInstID       Thread instance Id
\param [in]  prefixLength    Prefix size
\param [in]  pPrefixValue    Prefix value

\return      thrStatus_t     Status
***************************************************************************************************/
thrStatus_t THR_BrPrefixAttrRemoveEntry(instanceId_t thrInstID, uint8_t prefixLength,
                                        uint8_t *pPrefixValue);

/*!*************************************************************************************************
\fn     thrStatus_t THR_BrServiceAttrRemoveEntry(instanceId_t thrInstID, uint8_t* pServiceData,
        uint8_t serviceDataLen,uint8_t* pServerData,uint8_t serverDataLen)
\brief  Remove Service attribute entry.

\param [in]  thrInstID         Thread instance Id
\param [in]  pServiceData      Pointer to service data
\param [in]  serviceDataLen    Service data size
\param [in]  pServerData       Pointer to server data
\param [in]  serverDataLen     Server data size

\return      thrStatus_t       Status
 **************************************************************************************************/
thrStatus_t THR_BrServiceAttrRemoveEntry(instanceId_t thrInstID, uint8_t *pServiceData,
        uint8_t serviceDataLen, uint8_t *pServerData, uint8_t serverDataLen);

/*!*************************************************************************************************
\fn     void THR_BrPrefixAttrGetTable(instanceId_t thrInstID, uint8_t startIndex,
            uint8_t reqNoOfElements, uint8_t *pRspNoOfElements, uint8_t *pOutData)
\brief  Get the BR table, from startIndex to a maximum of reqNoOfElements.

\param [in]  thrInstID          Thread instance Id
\param [in]  startIndex         Start to search from this index
\param [in]  reqNoOfElements    Try to retrieve this many elements
\param [out] pRspNoOfElements   Actual number of elements retrieved
\param [out] pOutData           Data buffer to store the information
***************************************************************************************************/
void THR_BrPrefixAttrGetTable(instanceId_t thrInstID, uint8_t startIndex, uint8_t reqNoOfElements,
                              uint8_t *pRspNoOfElements, uint8_t *pOutData);

/*!*************************************************************************************************
\fn     thrStatus_t THR_BrPrefixAttrRemoveAll(instanceId_t thrInstID)
\brief  Remove all Border router prefix attribute entries.

\param [in]  thrInstID       Thread instance Id

\return      thrStatus_t     Status
 **************************************************************************************************/
thrStatus_t THR_BrPrefixAttrRemoveAll(instanceId_t thrInstID);

/*!*************************************************************************************************
\fn     thrStatus_t THR_BrPrefixAttrSync(instanceId_t thrInstID)
\brief  This function is used to synchronize the Border Router prefixes with
        the over-the-air network data information. After adding or removing more BR prefixes,
        this function shall be called to propagate the global network data.

\param [in]  thrInstID       Thread instance Id

\return      thrStatus_t     Status
 **************************************************************************************************/
thrStatus_t THR_BrPrefixAttrSync(instanceId_t thrInstID);

/*!*************************************************************************************************
\fn     thrStatus_t THR_SendProactiveAddressNotification(instanceId_t thrInstId, ipAddr_t *pDestIpAddr)
\brief  This function is used to send ADDR_NTF.ntf - Proactive Address Notification. This is useful
        if the device has changed short address and knows there are devices that likely maintain an
        address cache of that short address.

\param [in]  thrInstId       Thread instance ID
\param [in]  pDestIpAddr     Destination address: unicast or multicast

\return      thrStatus_t     Status
***************************************************************************************************/
thrStatus_t THR_SendProactiveAddressNotification(instanceId_t thrInstId, ipAddr_t *pDestIpAddr);

/*!*************************************************************************************************
\fn    uint64_t THR_GenerateExtendedAddress(bool_t privacyAddr)
\brief This function generates a random extended mac address

\param [in]  privacyAddr       TRUE if the address should be a privacy address

\retval      uint64_t          extended mac address
***************************************************************************************************/
uint64_t THR_GenerateExtendedAddress(bool_t privacyAddr);

/*!*************************************************************************************************
\fn    void THR_GetUniqueId(uint32_t *pOut)
\brief This function retrieves the board's unique id.

\param [in]  pOut               pointer to the memory area where the unique id will be stored

\return      none
***************************************************************************************************/
void THR_GetUniqueId(uint32_t *pOut);
#if !THREAD_ED_CONFIG
/*!*************************************************************************************************
\fn    void THR_SetThrRouterSelJitterSec(uint32_t value)
\brief This function sets the maximum time when soliciting a router ID.

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetThrRouterSelJitterSec(uint32_t value);


/*!*************************************************************************************************
\fn    thrStatus_t THR_PromoteAsRouter(instanceId_t thrInstId, uint8_t solicitReason)
\brief This function is used to send address solicit for given reason

\param [in]   thrInstId       Thread instance ID
\param [in]   solicitReason   reason for address solicit

\retval       thrStatus_t
***************************************************************************************************/
thrStatus_t THR_PromoteAsRouter(instanceId_t thrInstId, uint8_t solicitReason);
#endif
/*!*************************************************************************************************
\fn    void THR_GetDefaultDeviceConfig(instanceId_t thrInst, thrDeviceConfig_t *pData)
\brief This function retrieves the device configuration.

\param [in]  thrInst            Thread Instance ID
\param [out] pData              Pointer to the data where device config will be copied

\return      none
***************************************************************************************************/
void THR_GetDefaultDeviceConfig(instanceId_t thrInst, thrDeviceConfig_t *pData);

/*!*************************************************************************************************
\fn    void THR_SetDefaultDeviceConfig(instanceId_t thrInst, thrDeviceConfig_t *pData)
\brief This function sets the device configuration.

\param [in]  thrInst            Thread Instance ID
\param [in]  pData              Pointer to the data where the device config is stored

\return      none
***************************************************************************************************/
void THR_SetDefaultDeviceConfig(instanceId_t thrInst, thrDeviceConfig_t *pData);

/*!*************************************************************************************************
\fn    void THR_SetSlaacManualIID(uint8_t *pValue, uint32_t length)
\brief This function sets the SLAAC manual IID.

\param [in]  pValue             Pointer to the value to be set
\param [in]  pData              Size of the value to be set

\return      none
***************************************************************************************************/
void THR_SetSlaacManualIID(uint8_t *pValue, uint32_t length);

/*!*************************************************************************************************
\fn    void THR_GetNwkDataMinStableLifetime()
\brief This function returns the network data minimum stable lifetime.

\return      uint32_t   The value of the minimum stable lifetime.
***************************************************************************************************/
uint32_t THR_GetNwkDataMinStableLifetime();

#if THR_ROUTING_ENABLE
/*!*************************************************************************************************
\fn    void THR_SetNetworkIdTimeoutSec(uint32_t value)
\brief This function sets the network ID timeout.

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetNetworkIdTimeoutSec(uint32_t value);

/*!*************************************************************************************************
\fn    void THR_SetContextReuseDelaySec(uint32_t value)
\brief This function sets the context reuse delay.

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetContextReuseDelaySec(uint32_t value);

/*!*************************************************************************************************
\fn    void THR_SetMaxAllowedRouters(uint32_t value)
\brief This function sets the maximum number of allowed routers.

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetMaxAllowedRouters(uint32_t value);

/*!*************************************************************************************************
\fn    void THR_SetMinDowngradNeigbors(uint32_t value)
\brief This function sets the minimum number of neighbors with link quality 2 or better that a
       Router must have to downgrade to a REED. It should be less than 32

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetMinDowngradNeigbors(uint32_t value);

/*!*************************************************************************************************
\fn    void THR_SetRouterDowngradeThreshold(uint32_t value)
\brief This function sets the number of active Routers on the Thread Network above which an active
       Router may decide to become a Child.

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetRouterDowngradeThreshold(uint32_t value);

/*!*************************************************************************************************
\fn    uint32_t THR_GetRouterDowngradeThreshold()
\brief This function return the number of active Routers on the Thread Network above which an active
       Router may decide to become a Child.

\return      uint32_t           The threshold value
***************************************************************************************************/
uint32_t THR_GetRouterDowngradeThreshold();

/*!*************************************************************************************************
\fn    void THR_SetRouterUpgradeThreshold(uint32_t value)
\brief This function sets the number of active Routers on the Thread Network below which a REED may
       decide to become a Router.

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetRouterUpgradeThreshold(uint32_t value);

/*!*************************************************************************************************
\fn    uint32_t THR_GetRouterUpgradeThreshold()
\brief This function gets the number of active Routers on the Thread Network below which a REED may
       decide to become a Router.

\return      uint32_t           The threshold value
***************************************************************************************************/
uint32_t THR_GetRouterUpgradeThreshold();

/*!*************************************************************************************************
\fn    void THR_SetMlrTimeoutMinValue(uint32_t value)
\brief This function sets the minimum value of MLR Timeout parameter in seconds

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetMlrTimeoutMinValue(uint32_t value);

/*!*************************************************************************************************
\fn    void THR_SetParentPriority(uint8_t value)
\brief This function sets the parent priority. This function is used only for testing purposes
and should be run on the parent router.

\param [in]  value              The value to be set

\return      none
***************************************************************************************************/
void THR_SetParentPriority(uint8_t value);
#endif

/*!*************************************************************************************************
\fn     thrLqCacheEntry_t* THR_GetRlocToEidMapByEntry(uint32_t entry)
\brief  This function is used to get a specific entry from eid to rloc mapping cache table.

\param [in]  entry                  Entry table number.

\return      thrLqCacheEntry_t*     Pointer to value of the entry, NULL if entry number is not valid
***************************************************************************************************/
thrLqCacheEntry_t* THR_GetRlocToEidMapByEntry(uint32_t entry);

/*!*************************************************************************************************
\fn     uint32_t THR_GetNeighborsTblSize(instanceId_t thrInstanceID)
\brief  Returns the size of the Thread Neighbor Table

\param [in]    thrInstanceID    Thread instance ID

\return        uint32_t         Size of Thread Neighbor Table
***************************************************************************************************/
uint32_t THR_GetNeighborsTblSize(instanceId_t thrInstanceID);

/*!*************************************************************************************************
\fn    uint32_t* THR_GetRoutingInterfaceParams(uint8_t ifNo)
\brief This function returns pointer to the structure that stores all Thread routing parameters for an interface

\param [in]  ifNo               index of the interface

\return      uint32_t*          pointer to structure or NULL
***************************************************************************************************/
uint32_t THR_GetRoutingInterfaceParams(uint8_t ifNo);

/*================================================================================================*/

/*!*************************************************************************************************
\fn    thrStatus_t THR_RegisterMulticastGroup(instanceId_t thrInstId, ipAddr_t* multicastAddr,
       uint32_t *pTimeoutSec)
\brief This function registers a multicast group on the Thread interface

\param [in]  thrInstId       Thread instance ID
\param [in]  multicastAddr   IPv6 multicast address that the requesting device listens
\param [in]  pTimeoutSec     The multicast timeout to use if value is not NULL

\retval      thrStatus_t     Status of the registration operation
***************************************************************************************************/
thrStatus_t THR_RegisterMulticastGroup(instanceId_t thrInstId, ipAddr_t *multicastAddr,
                                                      uint32_t *pTimeoutSec);

/*!*************************************************************************************************
\fn    thrStatus_t THR_UnregisterMulticastGroup(instanceId_t thrInstId, ipAddr_t* multicastAddr)
\brief This function deregisters a multicast group on the Thread interface

\param [in]  thrInstId       Thread instance ID
\param [in]  multicastAddr   IPv6 multicast address that the requesting device listens

\retval      thrStatus_t       Status
***************************************************************************************************/
thrStatus_t THR_UnregisterMulticastGroup(instanceId_t thrInstId, ipAddr_t* multicastAddr);

#endif  /* _THREAD_NETWORK_H */
/*!
** @}
*/
