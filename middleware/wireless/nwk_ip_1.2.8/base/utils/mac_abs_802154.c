/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       mac_abs_802154.c
\brief      This is a public source file for the MAC abstraction module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* Basic types */
#include "EmbeddedTypes.h"

#include "MemManager.h"
#include "FunctionLib.h"

/* MAC interface */
#include "MacInterface.h"

/* Other */
#include "mac_abs_802154.h"
#include "Phy.h"
#include "sixlowpan_interface.h"
#include "sixlowpan.h"
#include "app_init.h"
#include "app_led.h"
#include "mac_filtering.h"

#include "TimersManager.h"
#include "event_manager.h"

#include "NVM_Interface.h"
#include "thread_network.h"

#if THR_ENABLE_MGMT_DIAGNOSTICS
#include "thread_utils.h"
#include "thread_mgmt.h"
#include "debug_log.h"
#endif

/*==================================================================================================
Private macros
==================================================================================================*/

/* If defined, the MSDU is copied to the end of the MCPS_Data.Req structure */
#define MAC_DEFAULT_PIB_SIZE        16U
#define MAC_BEACON_PIB_SIZE         54U

#define MAC_DEFAULT_KEY_SOURCE      0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define MAC_LQI_AVG_BITS            4
#define MAC_LQI_AVG_MAX             (1UL << MAC_LQI_AVG_BITS)

#ifdef _DEBUG
#define NWKDBG_STATS_ENABLED
//#define NWKDBG_TRACKTIME
#endif

#ifdef NWKDBG_TRACKTIME
#define CODE_TIME_MAX_ENTRIES       100U
#endif

#ifdef DEBUG_MAC_ERRORS
    /* User can use this define to count a specific type of error reported by MAC layer
     * To specify the errors, add to the mDebugMacErrors array the requested errors
     * To enable this feature, see debug_log.h */
    #define DEBUG_MAC_COUNTERS      1
#else
    #define DEBUG_MAC_COUNTERS      0
#endif

/*==================================================================================================
Private type definitions
==================================================================================================*/
typedef struct mcpsGenericMsg_tag
{
   mcpsToNwkMessage_t* pMsg;
   instanceId_t instanceId;
} mcpsGenericMsg_t;

/*==================================================================================================
Private prototypes
==================================================================================================*/
static resultType_t MCPS_NWK_SapHandlerCB(mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);
static resultType_t MLME_NWK_SapHandlerCB(nwkMessage_t* pMsg, instanceId_t instanceId);

/* MAC Requests */
/* MCPS Services */
static macAbsResultType_t MAC_McpsDataReq(macAbsMcpsDataReq_t * pParam, instanceId_t instanceId,
                                                  uint32_t upperLayerInstanceId);
static macAbsResultType_t MAC_McpsPurgeReq(uint8_t msduHandle, instanceId_t instanceId);

/* MLME Services */
static macAbsMlmeGetCnf_t * MAC_MlmeGetReq(macAbsMlmeGetReq_t * pParams, instanceId_t instanceId);
static macAbsMlmeResetCnf_t MAC_MlmeResetReq(macAbsMlmeResetReq_t * pParam, instanceId_t instanceId);
static macAbsMlmeScanCnf_t  MAC_MlmeScanReq(macAbsMlmeScanReq_t * pParam, instanceId_t instanceId);
static macAbsMlmeSetCnf_t   MAC_MlmeSetReq(macAbsMlmeSetReq_t * pParam, instanceId_t instanceId);
static macAbsMlmeStartCnf_t MAC_MlmeStartReq(macAbsMlmeStartReq_t * pParam, instanceId_t instanceId);
static void MAC_MlmePollReq(macAbsMlmePollReq_t * pParam, instanceId_t instanceId);

static bool_t MAC_SetPib(uint32_t pibAttribute, void* pPibValue, instanceId_t instanceId);
static bool_t MAC_GetPib(uint32_t pibAttribute, void* pPibValue, instanceId_t instanceId);
static bool_t MAC_SetPibUint8Val(uint32_t pibAttribute, uint8_t pibValue, instanceId_t instanceId);

static uint32_t MAC_GetMaxMsduSize(macAbsMcpsDataReq_t * pParam, instanceId_t instanceId);
static bool_t MAC_SetPANId(uint16_t panId, instanceId_t instanceId);
static uint16_t MAC_GetPANId(instanceId_t instanceId);
static bool_t MAC_SetShortAddress(uint16_t shortAddress, instanceId_t instanceId);
static uint16_t MAC_GetShortAddress(instanceId_t instanceId);
static bool_t MAC_SetExtendedAddress(uint64_t extendedAddress, instanceId_t instanceId);
static bool_t MAC_SetBeaconSourceExtendedAddress(bool_t enable, instanceId_t instanceId);
static uint64_t MAC_GetExtendedAddress(instanceId_t instanceId);
static bool_t MAC_SetKey(uint8_t idxInKeyTable, uint8_t* key, uint8_t keyIndex, instanceId_t instanceId);
static bool_t MAC_GetKey(uint8_t idxInKeyTable, uint8_t* key, instanceId_t instanceId);
static bool_t MAC_SetChannel(uint8_t channel, instanceId_t instanceId);
static uint8_t MAC_GetChannel(instanceId_t instanceId);
static bool_t MAC_SetRxOnWhenIdle(bool_t bRxOnWhenIdle, instanceId_t instanceId);
static bool_t MAC_GetRxOnWhenIdle(instanceId_t instanceId);
static bool_t MAC_SetFrameCounter(uint32_t frameCounter, instanceId_t instanceId);
static uint32_t MAC_GetFrameCounter(instanceId_t instanceId);
static bool_t MAC_GetNeighborFrameCounter(uint32_t *pFrameCtr,uint8_t neighborIdx,
              uint8_t maxNeighbors, uint8_t macKeyIdx, uint8_t maxMacKeys, instanceId_t instanceId);
static void  MAC_SetPanSecurity(uint8_t maxNbOfKeys, uint8_t maxNbOfNeighbors, uint8_t index,
                                          instanceId_t instanceId, bool_t autoReqSecLevel);
static void  MAC_SetNeighborSecurity(uint8_t neighborIdx, uint8_t maxNeighbors,
            uint8_t macKeyIdx, uint8_t maxMacKeys, uint64_t extAddr, uint16_t shortAddr, uint16_t panId,
            uint32_t frameCounter, instanceId_t instanceId);
static void  MAC_SetMlmeCallbacks(macAbsCallbacks_t* mlmeCallbacks, instanceId_t instanceId);

/* MAC Callbacks */
static void MAC_McpsDataIndCB(mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);
static void MAC_McpsDataCnfCB(mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);
static void MAC_McpsPurgeCnfCB(mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);

static void MAC_MlmeBeaconNotifyIndCB(mlmeBeaconNotifyInd_t * pPayload, instanceId_t instanceId);
static void MAC_MlmeScanCnfCB(mlmeScanCnf_t * pPayload, instanceId_t instanceId);
static void MAC_MlmeCommStatusIndCB(mlmeCommStatusInd_t *pPayload, instanceId_t instanceId);
static void MAC_MlmePollCnfCB(mlmePollCnf_t * pPayload, instanceId_t instanceId);
static void MAC_MlmePollIndCB(mlmePollNotifyInd_t * pPayload, instanceId_t instanceId);

static bool_t MAC_PhyAddToNeighborTable(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId);
static bool_t MAC_PhyRemoveFromNeighborTable(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId);
static int8_t MAC_PhyConvertLQIToRSSI(uint8_t LQI);
static void MAC_AllowBroadcast(instanceId_t instanceId, bool_t allowBroadcast);
static void MCPS_NWK_SapHandlerInternal(uint8_t *param);
#if DEBUG_MAC_COUNTERS
static bool_t MAC_DebugCheckInErrorsArray(resultType_t status);
#endif
/*==================================================================================================
Private global variables declarations
==================================================================================================*/

static const macAbsRequests_t macAbsReq =
{
    .MCPS_DataReq = MAC_McpsDataReq,
    .MCPS_PurgeReq = MAC_McpsPurgeReq,
    .MLME_SetReq = MAC_MlmeSetReq,
    .MLME_GetReq = MAC_MlmeGetReq,
    .MLME_ResetReq = MAC_MlmeResetReq,
    .MLME_StartReq = MAC_MlmeStartReq,
    .MLME_PollReq = MAC_MlmePollReq,
    .MLME_ScanReq = MAC_MlmeScanReq,
    .GetMaxMsduSize = MAC_GetMaxMsduSize,
    .SetPANId = MAC_SetPANId,
    .GetPANId = MAC_GetPANId,
    .SetShortAddress = MAC_SetShortAddress,
    .GetShortAddress = MAC_GetShortAddress,
    .SetExtendedAddress = MAC_SetExtendedAddress,
    .GetExtendedAddress = MAC_GetExtendedAddress,
    .SetKey = MAC_SetKey,
    .GetKey = MAC_GetKey,
    .SetChannel = MAC_SetChannel,
    .GetChannel = MAC_GetChannel,
    .SetRxOnWhenIdle = MAC_SetRxOnWhenIdle,
    .GetRxOnWhenIdle = MAC_GetRxOnWhenIdle,
    .SetPanSecurity = MAC_SetPanSecurity,
    .SetNeighborSecurity = MAC_SetNeighborSecurity,
    .SetMlmeCallbacks = MAC_SetMlmeCallbacks,
    .SetFrameCounter = MAC_SetFrameCounter,
    .GetFrameCounter = MAC_GetFrameCounter,
    .GetNeighborFrameCounter = MAC_GetNeighborFrameCounter,
    .SetBeaconSourceExtendedAddress = MAC_SetBeaconSourceExtendedAddress,
    .PhyAddToNeighborTable = MAC_PhyAddToNeighborTable,
    .PhyRemoveFromNeighborTable = MAC_PhyRemoveFromNeighborTable,
    .PhyConvertLQIToRSSI = MAC_PhyConvertLQIToRSSI,
    .AllowBroadcast = MAC_AllowBroadcast
};

static macAbsCallbacks_t mMacCallbackFunctions = {NULL};

#ifdef NWKDBG_STATS_ENABLED
macAbsStats_t macAbsStats = {0};

#ifdef NWKDBG_TRACKTIME
static uint32_t time_idx = 0;
static code_time_t code_time[CODE_TIME_MAX_ENTRIES] = {0};
#endif

#endif

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
bool_t mAllowBeacons = FALSE;
bool_t mAllowBroadcasts = TRUE;

#if DEBUG_MAC_COUNTERS
/* For other errors, see resultType_t in MacTypes.h */
const resultType_t mDebugMacErrors[] =
{
    gChannelAccessFailure_c,
};
#endif

extern taskMsgQueue_t mThreadStackMsgQueue;

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn    instanceId_t MAC_RegisterAbsCb_802154(instanceId_t macInstanceId)
\brief Public interface function for the MAC abstraction module. This function initializes the
       internal MAC callbacks structure and returns the pointer to the Mac Abstraction Requests
       structure.

\param[in]  nwkId  The instance of the upper layer ( NWK layer)
\param[out]  nwkId  The instance of the MAC layer

\retval      macAbsRequests_t       pointer address to the Mac Abstraction Requests
***************************************************************************************************/
macAbsRequests_t* MAC_RegisterAbsCb_802154
(
    instanceId_t nwkInstanceId, //IN
    absInstanceId_t* pMacInstanceId //OUT
)
{

    *pMacInstanceId =  BindToMAC(nwkInstanceId);
    if(*pMacInstanceId != gInvalidInstanceId_c)
    {
        Mac_RegisterSapHandlers(MCPS_NWK_SapHandlerCB, MLME_NWK_SapHandlerCB, *pMacInstanceId);
        return ((macAbsRequests_t*)&macAbsReq);
    }
    return NULL;
}

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\fn    void MAC_SetMlmeCallbacks(uint16_t shortAddress)
\brief Public interface function for the MAC abstraction module. This function sets the callbacks for
       the MLME asynchronous indication and confirm functions.

\param [in]  pMlmeCallbacks      Pointer to structure of MLME callbacks
\param [in]  instanceId          Mac instance ID

\return      none
***************************************************************************************************/
static void MAC_SetMlmeCallbacks
(
    macAbsCallbacks_t* pMlmeCallbacks,
    instanceId_t instanceId
)
{
    FLib_MemCpy(&mMacCallbackFunctions,pMlmeCallbacks,sizeof(macAbsCallbacks_t));
}

/*!*************************************************************************************************
\fn    void MAC_SetPANId(uint16_t panId, instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function sets the PAN Id.

\param [in]   panId             New PAN Id of the node.
\param [in]  instanceId         Mac instance ID

\return       bool_t          returns FALSE if the operation failed
***************************************************************************************************/
static bool_t MAC_SetPANId
(
    uint16_t panId,
    instanceId_t instanceId
)
{
    return MAC_SetPib(gMPibPanId_c, &panId, instanceId);
}
/*!*************************************************************************************************
\fn    void MAC_GetPANId(instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function gets the PAN Id.

\param [in]  instanceId         Mac instance ID

\return       uint16_t          The PAN identifier of the device.
***************************************************************************************************/
static uint16_t MAC_GetPANId
(
    instanceId_t instanceId
)
{
    uint16_t panId = 0;

    (void)MAC_GetPib(gMPibPanId_c, &panId, instanceId);

    return panId;
}

/*!*************************************************************************************************
\fn    void MAC_SetShortAddress(uint16_t shortAddress, instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function sets the node short
       address.

\param [in]  shortAddress       Short address of the node.
\param [in]  instanceId         Mac instance ID

\return      bool_t          returns FALSE if the operation failed
***************************************************************************************************/
static bool_t MAC_SetShortAddress
(
    uint16_t shortAddress,
    instanceId_t instanceId
)
{
    return MAC_SetPib(gMPibShortAddress_c, &shortAddress, instanceId);
}

/*!*************************************************************************************************
\fn    uint16_t MAC_GetShortAddress( instanceId_t instanceId )
\brief Public interface function for the MAC abstraction module. This function gets the short
       address for the mac instance.

\param [in]  instanceId         Mac instance ID

\return       uint16_t          The short link layer address of the device.
***************************************************************************************************/
static uint16_t MAC_GetShortAddress
(
    instanceId_t instanceId
)
{
    uint16_t shortAddress = 0;

    (void)MAC_GetPib(gMPibShortAddress_c, &shortAddress, instanceId);

    return shortAddress;
}

/*!*************************************************************************************************
\fn    bool_t MAC_SetExtendedAddress(uint64_t extendedAddress,instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function sets the extended
       address for the mac instance.

\param [in]  extendedAddress    extendded address of the node.
\param [in]  instanceId         mac instance id


\return       bool_t            Returns FALSE if operation has failed.
***************************************************************************************************/
static bool_t MAC_SetExtendedAddress
(
    uint64_t extendedAddress,
    instanceId_t instanceId
)
{
    return MAC_SetPib(gMPibExtendedAddress_c, &extendedAddress, instanceId);
}

/*!*************************************************************************************************
\fn     bool_t MAC_SetBeaconSourceExtendedAddress(bool_t enable, instanceId_t instanceId)
\brief  Public interface function for the MAC abstraction module. This function sets/clears the
        extended source address usage in beacon.

\param  [in] instanceId      mac instance id
\param  [in] enable          flag used for enbling/disabling functionality

\return      bool_t         Returns FALSE if operation has failed.
***************************************************************************************************/
static bool_t MAC_SetBeaconSourceExtendedAddress
(
    bool_t enable,
    instanceId_t instanceId
)
{
    return MAC_SetPib(gMPibUseExtendedAddressForBeacon_c, &enable, instanceId);
}

/*!*************************************************************************************************
\fn    uint64_t MAC_GetExtendedAddress(instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function gets the extended
       address.

\param [in] instanceId      mac instance id

\return     uint64_t        mac extended address
***************************************************************************************************/
static uint64_t MAC_GetExtendedAddress
(
    instanceId_t instanceId
)
{
    uint64_t extendedAddress = 0;

    (void)MAC_GetPib(gMPibExtendedAddress_c, &extendedAddress, instanceId);

    return extendedAddress;
}

/*!*************************************************************************************************
\fn     void MAC_SetKey(uint8_t idxInKeyTable, uint8_t* key, uint8_t keyIndex, instanceId_t instanceId)
\brief  This function is used for setting the Security Key into MAC security PIB

\param [in] idxInKeyTable   index in Key table
\param [in] key             pointer to Key
\param [in] keyIndex        key index
\param [in] instanceId      mac instance id

\return     bool_t         returns FALSE if the operation failed
***************************************************************************************************/
static bool_t MAC_SetKey
(
    uint8_t      idxInKeyTable,
    uint8_t*     key,
    uint8_t      keyIndex,
    instanceId_t instanceId
)
{
    uint8_t lookupData[]    = {MAC_DEFAULT_KEY_SOURCE, 0x00};
    bool_t  result          = MAC_SetPib(gMPibiKeyTableCrtEntry_c, &idxInKeyTable, instanceId);

    if(TRUE == result)
    {
        result = MAC_SetPib(gMPibKey_c, key, instanceId);
    }

    if(TRUE == result)
    {
        lookupData[8]   = keyIndex;
        result          = MAC_SetPib(gMPibKeyIdLookupData_c, lookupData, instanceId);
    }

    return result;
}

/*!*************************************************************************************************
\fn     void MAC_GetKey(uint8_t idxInKeyTable, uint8_t* key, instanceId_t instanceId)
\brief  This function is used for getting the Security Key from MAC security PIB

\param [in] idxInKeyTable   index in Key table
\param [in] key             pointer to put the key value
\param [in] instanceId      mac instance id

\return     bool_t         returns FALSE if the operation failed
***************************************************************************************************/
static bool_t MAC_GetKey
(
    uint8_t      idxInKeyTable,
    uint8_t*     key,
    instanceId_t instanceId
)
{
    bool_t result = MAC_SetPib(gMPibiKeyTableCrtEntry_c, &idxInKeyTable, instanceId);

    if(TRUE == result)
    {
        result = MAC_GetPib(gMPibKey_c, key, instanceId);
    }

    return result;
}

/*!*************************************************************************************************
\fn    bool_t MAC_SetChannel(uint8_t channel, instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function sets the channel.

\param [in] channel         logical channel
\param [in] instanceId      mac instance id

\return     bool_t         returns FALSE if the operation failed
***************************************************************************************************/
static bool_t MAC_SetChannel
(
    uint8_t channel,
    instanceId_t instanceId
)
{
    return MAC_SetPib(gMPibLogicalChannel_c, &channel, instanceId);
}

/*!*************************************************************************************************
\fn    uint8_t MAC_GetChannel(instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function gets the channel.

\param [in] instanceId      mac instance id

\return     uint8_t         logical channel
***************************************************************************************************/
static uint8_t MAC_GetChannel
(
    instanceId_t instanceId
)
{
    uint8_t channel = 0;

    (void)MAC_GetPib(gMPibLogicalChannel_c, &channel, instanceId);

    return channel;
}

/*!*************************************************************************************************
\fn    bool_t MAC_SetRxOnWhenIdle(bool_t bRxOnWhenIdle, instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function sets the receiver
       state.

\param [in] bRxOnWhenIdle   receiver state
\param [in] instanceId      mac instance id

\return     bool_t         returns FALSE if the operation failed
***************************************************************************************************/
static bool_t MAC_SetRxOnWhenIdle
(
    bool_t bRxOnWhenIdle,
    instanceId_t instanceId
)
{
    return MAC_SetPib(gMPibRxOnWhenIdle_c, &bRxOnWhenIdle, instanceId);
}

/*!*************************************************************************************************
\fn    bool_t MAC_GetRxOnWhenIdle(instanceId_t instanceId)
\brief Public interface function for the MAC abstraction module. This function gets the receiver
state.

\param [in] instanceId      mac instance id

\return     bool_t         TRUE/FALSE
***************************************************************************************************/
static bool_t MAC_GetRxOnWhenIdle
(
    instanceId_t instanceId
)
{
    bool_t bRxOnWhenIdle = FALSE;

    (void)MAC_GetPib(gMPibRxOnWhenIdle_c, &bRxOnWhenIdle, instanceId);

    return bRxOnWhenIdle;
}
/*!*************************************************************************************************
\fn     uint32_t MAC_GetMaxMsduSize(macAbsGetMaxMsduSizeReq_t *pParam, instanceId_t instanceId)
\brief  This function is used for getting the maximum available MAC payload for a given set of
        transmission parameters

\param [in]   pParam          pointer to the data request settings
\param [in]   instanceId      mac instance id

\return      uint32_t         size of the maximum available MAC payload
***************************************************************************************************/
static uint32_t MAC_GetMaxMsduSize
(
    macAbsMcpsDataReq_t *pParam,
    instanceId_t instanceId
)
{
    uint32_t maxMsduSize;
    mcpsDataReq_t mcpsDataReq;

    mcpsDataReq.srcPanId      = MAC_GetPANId(instanceId);
    mcpsDataReq.srcAddrMode   = (addrModeType_t)pParam->srcAddrMode;
    mcpsDataReq.dstAddrMode   = (addrModeType_t)pParam->dstAddrMode;
    mcpsDataReq.dstPanId      = pParam->dstPANId;
    mcpsDataReq.dstAddr       = pParam->dstAddr;
    mcpsDataReq.txOptions     = (macTxOptions_t)pParam->txOptions;
    mcpsDataReq.securityLevel = (macSecurityLevel_t)pParam->securityLevel;
    mcpsDataReq.keyIdMode     = (keyIdModeType_t)pParam->keyIdMode;
    mcpsDataReq.keySource     = pParam->keySource;
    mcpsDataReq.keyIndex      = pParam->keyIndex;

    maxMsduSize = Mac_GetMaxMsduLength(&mcpsDataReq);

    return maxMsduSize;
}
/*!*************************************************************************************************
\fn     void MAC_SetPanSecurity(uint8_t maxNbOfKeys,uint8_t maxNbOfNeighbors,uint8_t index,
                                instanceId_t instanceId,bool_t autoReqSecLevel)
\brief  This function is used for setting the Pan security information in MAC security PIBs

\param [in] maxNbOfKeys         maximum number of keys
\param [in] maxNbOfNeighbors    maximum number of neighbors
\param [in] index               index in the macKeyTable where the security material should be added
\param [in] instanceId          mac instance id
\param [in] autoReqSecLevel     enable/disable auto request security level

\return     none
***************************************************************************************************/
static void MAC_SetPanSecurity
(
     uint8_t      maxNbOfKeys,
     uint8_t      maxNbOfNeighbors,
     uint8_t      index,
     instanceId_t instanceId,
     bool_t       autoReqSecLevel
)
{
    uint8_t     defaultKeySource[]  = {MAC_DEFAULT_KEY_SOURCE};
    uint8_t     keyIdx, neighborIdx;

    /* Unused */
    (void)autoReqSecLevel;

    /* General Security PIBs */

    (void)MAC_SetPibUint8Val(gMPibSecurityEnabled_c,                         1,                  instanceId);
    (void)MAC_SetPib(gMPibDefaultKeySource_c,                                &defaultKeySource,  instanceId);
    //(void)MAC_SetPib(gMPibFrameCounter_c,                                    &frameCounter,      instanceId);

    /* Auto Request security settings */
    /*
        Setting the gMPibAutoRequestKeySource_c is not required since Key Id
        mode 1 uses the macDefaultKeySource.
    */

    //MAC_SetPib(gMPibAutoRequestSecurityLevel_c,                             &autoReqSecLevel,            instanceId);
    //pibValue = gMacAbsKeyIdMode1_c;
    //MAC_SetPib(gMPibAutoRequestKeyIdMode_c,                                 &pibValue,            instanceId);

    /* Key Table */

    for(keyIdx = 0; keyIdx < maxNbOfKeys; keyIdx++)
    {
        if(keyIdx != index)
        {
          continue;
        }

        (void)MAC_SetPib(gMPibiKeyTableCrtEntry_c,                           &keyIdx,            instanceId);

        /* KeyDescriptor - KeyIdLookupList */

        (void)MAC_SetPibUint8Val(gMPibiKeyIdLookuplistCrtEntry_c,            0,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibKeyIdLookupDataSize_c,                 1,                  instanceId);

        /* KeyDescriptor - KeyDeviceList */

        for(neighborIdx = 0; neighborIdx < maxNbOfNeighbors; neighborIdx++)
        {
            /* Index in the macDeviceTable */
            uint8_t devDescHandle = index * maxNbOfNeighbors + neighborIdx;
            uint32_t frameCounter = 0;

            (void)MAC_SetPib(gMPibiKeyDeviceListCrtEntry_c,                  &neighborIdx,       instanceId);
            (void)MAC_SetPib(gMPibKeyDeviceDescriptorHandle_c,               &devDescHandle,     instanceId);
            (void)MAC_SetPibUint8Val(gMPibUniqueDevice_c,                    0,                  instanceId);
            (void)MAC_SetPibUint8Val(gMPibBlackListed_c,                     0,                  instanceId);

            /* Reset the frame counter in the macDeviceTable */
            (void)MAC_SetPib(gMPibiDeviceTableCrtEntry_c,           &devDescHandle,    instanceId);
            (void)MAC_SetPib(gMPibDeviceDescriptorFrameCounter_c,   &frameCounter,      instanceId);
        }

        /* KeyDescriptor - KeyUsageList */

        /* Data Frames entry */
        (void)MAC_SetPibUint8Val(gMPibiKeyUsageListCrtEntry_c,               0,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibKeyUsageFrameType_c,                   1,                  instanceId);


        /* MAC command entry */
        (void)MAC_SetPibUint8Val(gMPibiKeyUsageListCrtEntry_c,               1,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibKeyUsageFrameType_c,                   3,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibKeyUsageCommnadFrameIdentifier_c,      4,                  instanceId);

        /* Key Table - Security Level Table */

        (void)MAC_SetPibUint8Val(gMPibiSecurityLevelTableCrtEntry_c,          0,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibSecLevFrameType_c,                      1,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibSecLevCommnadFrameIdentifier_c,         0,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibSecLevSecurityMinimum_c,                0,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibSecLevDeviceOverrideSecurityMinimum_c,  1,                  instanceId);

        /* Accept secured data request commands */
        (void)MAC_SetPibUint8Val(gMPibiSecurityLevelTableCrtEntry_c,          1,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibSecLevFrameType_c,                      3,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibSecLevCommnadFrameIdentifier_c,         4,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibSecLevSecurityMinimum_c,                5,                  instanceId);
        (void)MAC_SetPibUint8Val(gMPibSecLevDeviceOverrideSecurityMinimum_c,  0,                  instanceId);

    }

}

/*!*************************************************************************************************
\fn     void MAC_SetNeighborSecurity(uint8_t neighborIdx, uint64_t extAddr, uint16_t shortAddr,
                                    uint16_t panId, uint32_t frameCounter, instanceId_t instanceId)
\brief  This function is used for setting the neighbor information in MAC security PIBs

\param [in] neighborIdx     Index in deviceTable
\param [in] maxNeighbors    Maximum number of neighbors
\param [in] macKeyIdx       Index of the active MAC key
\param [in] maxMacKeys      Maximum number of MAC keys
\param [in] extAddr         neighbor extended address
\param [in] shortAddr       neighbor short address
\param [in] panId           neighbor pan identifier
\param [in] frameCounter    neighbor frame counter
\param [in] instanceId      mac instance id

\return     none
***************************************************************************************************/
static void MAC_SetNeighborSecurity
(
    uint8_t         neighborIdx,
    uint8_t         maxNeighbors,
    uint8_t         macKeyIdx,
    uint8_t         maxMacKeys,
    uint64_t        extAddr,
    uint16_t        shortAddr,
    uint16_t        panId,
    uint32_t        frameCounter,
    instanceId_t    instanceId
)
{
    uint8_t i, devDescIdx;

    for(i = 0; i < maxMacKeys; i++)
    {
        devDescIdx = i * maxNeighbors + neighborIdx;

        (void)MAC_SetPib(gMPibiDeviceTableCrtEntry_c,           &devDescIdx,    instanceId);
        (void)MAC_SetPib(gMPibDeviceDescriptorExtAddress_c,     &extAddr,           instanceId);
        (void)MAC_SetPib(gMPibDeviceDescriptorShortAddress_c,   &shortAddr,         instanceId);
        (void)MAC_SetPib(gMPibDeviceDescriptorPanId_c,          &panId,             instanceId);
        (void)MAC_SetPibUint8Val(gMPibDeviceDescriptorExempt,   1,                  instanceId);

        if(i !=  macKeyIdx)
        {
            continue;
        }

        (void)MAC_SetPib(gMPibDeviceDescriptorFrameCounter_c,   &frameCounter,      instanceId);
    }
}

/*!*************************************************************************************************
\fn     static resultType_t MCPS_NWK_SapHandlerCB(mcpsToNwkMessage_t* pMsg, instanceId_t instanceId)
\brief  Private function for handle mcps data indications and confirms. MCPS to NWK SAP handler.

\param [in] pMsg          pointer to mcps to nwk msg
\param [in] instanceId    mac instance id

\return     gSuccess_c
***************************************************************************************************/
static resultType_t MCPS_NWK_SapHandlerCB
(
   mcpsToNwkMessage_t *pMsg,
   instanceId_t instanceId
)
{
    mcpsGenericMsg_t *pMcpsMessage;
    pMcpsMessage = MEM_BufferAlloc(sizeof(mcpsGenericMsg_t));

    if (pMcpsMessage)
    {
        pMcpsMessage->pMsg = pMsg;
        pMcpsMessage->instanceId = instanceId;

        if (FALSE == NWKU_SendMsg(MCPS_NWK_SapHandlerInternal, (void*)pMcpsMessage, &mThreadStackMsgQueue))
        {
            MEM_BufferFree(pMsg);
            MEM_BufferFree(pMcpsMessage);
        }
    }
    else
    {
        MEM_BufferFree(pMsg);
    }

    return gSuccess_c;
}

/*!*************************************************************************************************
\fn     static void MCPS_NWK_SapHandlerInternal(uint8_t *param)
\brief  Helper function for MCPS_NWK_SapHandlerCB

\param [in] pMsg          pointer to mcsp message

\return     none
***************************************************************************************************/
static void MCPS_NWK_SapHandlerInternal
(
    uint8_t *param
)
{
    mcpsToNwkMessage_t *pMsg = ((mcpsGenericMsg_t*)param)->pMsg;
    instanceId_t instanceId = ((mcpsGenericMsg_t*)param)->instanceId;

    switch (pMsg->msgType)
    {
        case gMcpsDataCnf_c:
            MAC_McpsDataCnfCB(pMsg, instanceId);
            break;

        case gMcpsDataInd_c:
            MAC_McpsDataIndCB(pMsg, instanceId);
            break;

        case gMcpsPurgeCnf_c:
            MAC_McpsPurgeCnfCB(pMsg, instanceId);
            break;

        default:
            break;
    }

    MEM_BufferFree(param);
}

/*!*************************************************************************************************
\fn     static resultType_t MLME_NWK_SapHandlerCB(nwkMessage_t* pMsg, instanceId_t instanceId)
\brief

\param [in] pMsg            pointer to nwk msg
\param [in] instanceId      mac instance id

\return     resultType_t    mac result type
***************************************************************************************************/
static resultType_t MLME_NWK_SapHandlerCB
(
    nwkMessage_t* pMsg,
    instanceId_t instanceId
)
{
    switch (pMsg->msgType)
    {
        case gMlmeScanCnf_c:
            MAC_MlmeScanCnfCB(&pMsg->msgData.scanCnf, instanceId);
            NvClearCriticalSection();
            break;

        case gMlmeBeaconNotifyInd_c:
            MAC_MlmeBeaconNotifyIndCB(&pMsg->msgData.beaconNotifyInd, instanceId);
            break;

        case gMlmeCommStatusInd_c:
            MAC_MlmeCommStatusIndCB(&pMsg->msgData.commStatusInd, instanceId);
            break;

        case gMlmePollCnf_c:
            MAC_MlmePollCnfCB(&pMsg->msgData.pollCnf, instanceId);
            break;

        case gMlmePollNotifyInd_c:
            MAC_MlmePollIndCB(&pMsg->msgData.pollNotifyInd, instanceId);
            break;

        default:
            break;
    }

    /* Free Mlme Message */
    MEM_BufferFree(pMsg);

    return gSuccess_c;
}

/*!*************************************************************************************************
\fn     static void MAC_McpsDataIndCB(mcpsToNwkMessage_t* pMsg, instanceId_t instanceId)
\brief

\param [in]   pMsg pointer to mcps to nwk msg
\param [in]   instanceId      mac instance id

\return       resultType_t    mac result type
***************************************************************************************************/
static void MAC_McpsDataIndCB
(
    mcpsToNwkMessage_t* pMsg,
    instanceId_t instanceId
)
{
#if THR_ENABLE_MGMT_DIAGNOSTICS
    bool_t packetSentToSlwp = FALSE;
    bool_t isBcastAddr = FALSE;
#endif
#ifdef NWKDBG_TRACKTIME
    code_time[time_idx].dataIndTime = TMR_GetTimestamp();
#endif

    /* Thread/6lowpan protections from zigbee/zgp packets */
    if((pMsg->msgData.dataInd.srcAddrMode == gAddrModeNoAddress_c)||
       (pMsg->msgData.dataInd.srcAddrMode == gAddrModeReserved_c))
    {
#if THR_ENABLE_MGMT_DIAGNOSTICS
        /* Count packets of unknown or unsupported protocols */
        (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(*((instanceId_t*)((slwpStruct_t*)instanceId)->macInstanceId), NULL, gThrMacInstIdSearch_c),
                                             gMgmtDiagnostic_ifInUnknownProtosPkts_c,
                                             gMgmtDiagnostic_LinkData1StepInc_c);
#endif
        /* Free mcpsToNwkMessage_t block from mac */
        MEM_BufferFree(pMsg);
        return;
    }

    if((pMsg->msgData.dataInd.dstAddrMode == gAddrModeShortAddress_c) &&
        (pMsg->msgData.dataInd.dstAddr == 0xFFFFU) &&
        (mAllowBroadcasts == FALSE))
    {
        /* Free mcpsToNwkMessage_t block from mac */
        MEM_BufferFree(pMsg);
        return;
    }

    if (TRUE == MacFiltering_KeepPacket((macAbsAddrModeType_t)pMsg->msgData.dataInd.srcAddrMode,
                                        pMsg->msgData.dataInd.srcAddr, &pMsg->msgData.dataInd.mpduLinkQuality))
    {
        macAbsMcpsDataInd_t * pMcpsDataInd = NWKU_MEM_BufferAlloc(sizeof(macAbsMcpsDataInd_t));

        /* Packet is dropped in case there are no more memory buffers */
        if (pMcpsDataInd)
        {
                /* Populate the MAC abstraction structure */
                pMcpsDataInd->instanceId        = instanceId;
                pMcpsDataInd->srcAddrMode       = (macAbsAddrModeType_t)pMsg->msgData.dataInd.srcAddrMode;
                pMcpsDataInd->dstAddrMode       = (macAbsAddrModeType_t)pMsg->msgData.dataInd.dstAddrMode;
                pMcpsDataInd->dstPANId          = pMsg->msgData.dataInd.dstPanId;
                pMcpsDataInd->dstAddr           = pMsg->msgData.dataInd.dstAddr;
                pMcpsDataInd->srcAddr           = pMsg->msgData.dataInd.srcAddr;
                pMcpsDataInd->srcPANId          = pMsg->msgData.dataInd.srcPanId;
                pMcpsDataInd->msduLength        = pMsg->msgData.dataInd.msduLength;
                pMcpsDataInd->timestamp         = pMsg->msgData.dataInd.timestamp;
                pMcpsDataInd->mpduLinkQuality   = pMsg->msgData.dataInd.mpduLinkQuality;
                pMcpsDataInd->dsn               = pMsg->msgData.dataInd.dsn;
                pMcpsDataInd->securityLevel     = (macAbsSecurityLevel_t)pMsg->msgData.dataInd.securityLevel;
                pMcpsDataInd->keyIdMode         = (macAbsKeyIdModeType_t)pMsg->msgData.dataInd.keyIdMode;
                pMcpsDataInd->keySource         = pMsg->msgData.dataInd.keySource;
                pMcpsDataInd->keyIndex          = pMsg->msgData.dataInd.keyIndex;
                pMcpsDataInd->qualityOfService  = (macAbsQoS_t)0;
                /* Reuse McpsDataInd buffer */
                pMcpsDataInd->pMsdu             = (void *)pMsg;

                /* Clean the short source and destination short addresses */
                if(pMcpsDataInd->srcAddrMode == gMacAbsAddrModeShortAddress_c)
                {
                    pMcpsDataInd->srcAddr &= 0x000000000000FFFF;
                }
                #if THR_ENABLE_MGMT_DIAGNOSTICS
                if(pMcpsDataInd->dstAddrMode == gMacAbsAddrModeShortAddress_c)
                {
                    pMcpsDataInd->dstAddr &= 0x000000000000FFFF;
                    if(pMcpsDataInd->dstAddr == 0x000000000000FFFF)
                    {
                        isBcastAddr = TRUE;
                    }
                }
                #endif
                FLib_MemInPlaceCpy(pMcpsDataInd->pMsdu, pMsg->msgData.dataInd.pMsdu, pMcpsDataInd->msduLength);

                if(mMacCallbackFunctions.mcpsKeyIdMode2DataInd &&
                   (pMcpsDataInd->keyIdMode == gMacAbsKeyIdMode2_c))
                {
                   // This will reset the incoming frame counter
                   mMacCallbackFunctions.mcpsKeyIdMode2DataInd(pMcpsDataInd);
                }

                /* Call the 6LoWPAN handling function */
                SLWP_McpsDataIndCB(pMcpsDataInd, (macAbsAddrModeType_t)pMcpsDataInd->srcAddrMode,
                                   &pMcpsDataInd->srcAddr);
#if THR_ENABLE_MGMT_DIAGNOSTICS
                packetSentToSlwp = TRUE;
#endif
        }
        else
        {
            /* Call with NULL in order for the 6LoWPAN to perform clean up for the packet */
            SLWP_McpsDataIndCB(NULL, (macAbsAddrModeType_t)pMsg->msgData.dataInd.srcAddrMode,
                               &pMsg->msgData.dataInd.srcAddr);

            /* Free mcpsToNwkMessage_t block from MAC */
            MEM_BufferFree(pMsg);
        }
    }
    else
    {
        /* Free mcpsToNwkMessage_t block from mac */
        MEM_BufferFree(pMsg);
    }

#if THR_ENABLE_MGMT_DIAGNOSTICS
    if(!packetSentToSlwp)
    {
        /* Count dropped inbound packets because of lack of memory */
        (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(*((instanceId_t*)((slwpStruct_t*)instanceId)->macInstanceId), NULL, gThrMacInstIdSearch_c),
                                             gMgmtDiagnostic_ifInDiscardsPkts_c,
                                             gMgmtDiagnostic_LinkData1StepInc_c);
    }
    else
    {
        /* Count unicast and broadcast inbound packets */
        (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(*((instanceId_t*)((slwpStruct_t*)instanceId)->macInstanceId), NULL, gThrMacInstIdSearch_c),
                                             (isBcastAddr)?gMgmtDiagnostic_ifInBroadcastPkts_c:gMgmtDiagnostic_ifInUcastPkts_c,
                                             gMgmtDiagnostic_LinkData1StepInc_c);
    }
#endif
}
/*!*************************************************************************************************
\fn     static void MAC_McpsDataCnfCB(mcpsToNwkMessage_t* pMsg, instanceId_t instanceId)
\brief

\param [in]   pMsg pointer to mcps to nwk msg
\param [in]   instanceId      mac instance id
***************************************************************************************************/
static void MAC_McpsDataCnfCB
(
    mcpsToNwkMessage_t* pMsg,
    instanceId_t instanceId
)
{
    macAbsMcpsDataCnf_t macAbsMcpsDataCnf = {0};
    macAbsMcpsDataCnf.instanceId  = instanceId;
    macAbsMcpsDataCnf.msduHandle  = pMsg->msgData.dataCnf.msduHandle;
    macAbsMcpsDataCnf.status      = (macAbsResultType_t)pMsg->msgData.dataCnf.status;
    macAbsMcpsDataCnf.timestamp   = pMsg->msgData.dataCnf.timestamp;

    /* Reuse MAC buffer */
    FLib_MemCpy((void*)pMsg, &macAbsMcpsDataCnf, sizeof(macAbsMcpsDataCnf_t));

    if (mMacCallbackFunctions.mcpsDataCnf)
    {
        mMacCallbackFunctions.mcpsDataCnf(&macAbsMcpsDataCnf);
    }

    SLWP_McpsDataCnfCB((macAbsMcpsDataCnf_t*)pMsg);

#ifdef NWKDBG_STATS_ENABLED
    if(gSuccess_c != pMsg->msgData.dataCnf.status)
    {
        macAbsStats.mNbDataCnfFail++;
    }
    else
    {
        macAbsStats.mNbDataCnfSuccess++;
    }
#endif

#if THR_ENABLE_MGMT_DIAGNOSTICS
    if(gSuccess_c != pMsg->msgData.dataCnf.status)
    {
#if DEBUG_MAC_COUNTERS
        if(MAC_DebugCheckInErrorsArray(pMsg->msgData.dataCnf.status))
        {
#endif
            /* Count inbound packets that could not be transmitted because of errors */
            (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(instanceId, NULL, gThrMacInstIdSearch_c),
                                                 gMgmtDiagnostic_ifInErrorsPkts_c,
                                                 gMgmtDiagnostic_LinkData1StepInc_c);
#if DEBUG_MAC_COUNTERS
        }
#endif
    }
#endif
#if gLEDSupported_d
    Led_MacTxOff();
#endif
}
/*!*************************************************************************************************
\fn     static void MAC_McpsPurgeCnfCB(mcpsToNwkMessage_t* pMsg, instanceId_t instanceId)
\brief  Mcps Purge confirm callback function

\param [in]   pMsg pointer to mcps to nwk msg
\param [in]   instanceId      mac instance id
***************************************************************************************************/
static void MAC_McpsPurgeCnfCB
(
    mcpsToNwkMessage_t* pMsg,
    instanceId_t instanceId
)
{
    MEM_BufferFree(pMsg);
}
/*!*************************************************************************************************
\fn     static macAbsResultType_t MAC_McpsDataReq(macAbsMcpsDataReq_t * pParam, instanceId_t instanceId)
\brief

\param [in] pParam       pointer to mac data request msg
\param [in] instanceId   mac instance id

\return     void
***************************************************************************************************/
static macAbsResultType_t MAC_McpsDataReq
(
    macAbsMcpsDataReq_t * pParam,
    instanceId_t instanceId,
    uint32_t upperLayerInstanceId
)
{
    nwkToMcpsMessage_t *pNwkToMcpsMsg = NULL;
    resultType_t result = gTransactionOverflow_c;
#if THR_ENABLE_MGMT_DIAGNOSTICS
    bool_t isBcastAddr = FALSE;
#endif
#ifdef NWKDBG_STATS_ENABLED
    macAbsStats.mNbDataReq++;

#ifdef NWKDBG_TRACKTIME
    code_time[time_idx].dataReqTime = TMR_GetTimestamp();
    code_time[time_idx].deltaTime = code_time[time_idx].dataReqTime - code_time[time_idx].dataIndTime;
    time_idx++;
    if(time_idx > CODE_TIME_MAX_ENTRIES-1) time_idx = 0;
#endif

#endif

    pNwkToMcpsMsg = NWKU_MEM_BufferAlloc(sizeof(nwkToMcpsMessage_t) + pParam->msduLength);

    if (pNwkToMcpsMsg)
    {
        pNwkToMcpsMsg->msgData.dataReq.pMsdu = (uint8_t*)pNwkToMcpsMsg + sizeof(nwkToMcpsMessage_t);
        FLib_MemCpy(pNwkToMcpsMsg->msgData.dataReq.pMsdu, pParam->pMsdu, pParam->msduLength);

        pNwkToMcpsMsg->msgType = gMcpsDataReq_c;
        pNwkToMcpsMsg->msgData.dataReq.srcPanId = MAC_GetPANId(instanceId);

        if (gMacAbsAddrModeShortAddress_c == pParam->srcAddrMode)
        {
            pNwkToMcpsMsg->msgData.dataReq.srcAddr = MAC_GetShortAddress(instanceId);
        }
        else if (gMacAbsAddrModeExtendedAddress_c == pParam->srcAddrMode)
        {
            pNwkToMcpsMsg->msgData.dataReq.srcAddr = MAC_GetExtendedAddress(instanceId);
        }

        pNwkToMcpsMsg->msgData.dataReq.dstAddr       = pParam->dstAddr;
        pNwkToMcpsMsg->msgData.dataReq.dstAddrMode   = (addrModeType_t)pParam->dstAddrMode;
        pNwkToMcpsMsg->msgData.dataReq.dstPanId      = pParam->dstPANId;
        pNwkToMcpsMsg->msgData.dataReq.keyIdMode     = (keyIdModeType_t)pParam->keyIdMode;
        pNwkToMcpsMsg->msgData.dataReq.keyIndex      = pParam->keyIndex;
        pNwkToMcpsMsg->msgData.dataReq.msduHandle    = pParam->msduHandle;
        pNwkToMcpsMsg->msgData.dataReq.msduLength    = pParam->msduLength;
        pNwkToMcpsMsg->msgData.dataReq.securityLevel = (macSecurityLevel_t)pParam->securityLevel;
        pNwkToMcpsMsg->msgData.dataReq.srcAddrMode   = (addrModeType_t)pParam->srcAddrMode;
        pNwkToMcpsMsg->msgData.dataReq.txOptions     = (macTxOptions_t)pParam->txOptions;
        pNwkToMcpsMsg->msgData.dataReq.keySource     = pParam->keySource;
#if THR_ENABLE_MGMT_DIAGNOSTICS
        /* Check if destination address is short and broadcast */
        if((gMacAbsAddrModeShortAddress_c == pParam->dstAddrMode) && (0x000000000000FFFF == pParam->dstAddr))
        {
            isBcastAddr = TRUE;
        }
#endif
        result = NWK_MCPS_SapHandler(pNwkToMcpsMsg, instanceId);

        /* Since the MAC abstraction function doesn`t return a synchronous status, a DataCnf is called. */
        if (gSuccess_c != result)
        {
#ifdef NWKDBG_STATS_ENABLED
            macAbsStats.mNbDataReqFail++;
#endif

            /* Send MCPS Data Confirm */
            /* Reuse pNwkToMcpsMsg, check if some fields from pNwkToMcpsMsg are reused*/
            macAbsMcpsDataCnf_t *pMcpsDataCnf = (macAbsMcpsDataCnf_t *)pNwkToMcpsMsg;
            pMcpsDataCnf->msduHandle          = pNwkToMcpsMsg->msgData.dataReq.msduHandle;
            pMcpsDataCnf->instanceId          = (uint32_t)upperLayerInstanceId;
            pMcpsDataCnf->status              = (macAbsResultType_t)result;
            pMcpsDataCnf->timestamp           = 0;  /*Timestamp unavailable. */

            SLWP_McpsDataCnfCB(pMcpsDataCnf);

#if THR_ENABLE_MGMT_DIAGNOSTICS
            /* Count outbound packets that could not be transmitted because of errors */
            (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(instanceId, NULL, gThrMacInstIdSearch_c),
                                                 gMgmtDiagnostic_ifOutErrorsPkts_c,
                                                 gMgmtDiagnostic_LinkData1StepInc_c);
#endif
        }
        else
        {
#ifdef NWKDBG_STATS_ENABLED
            macAbsStats.mNbDataReqSuccess++;
#endif
#if gLEDSupported_d
            Led_MacTxOn();
#endif

#if THR_ENABLE_MGMT_DIAGNOSTICS
            /* Count unicast and broadcast outbound packets */
            (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(instanceId, NULL, gThrMacInstIdSearch_c),
                                                 (isBcastAddr)?gMgmtDiagnostic_ifOutBroadcastPkts_c:gMgmtDiagnostic_ifOutUcastPkts_c,
                                                 gMgmtDiagnostic_LinkData1StepInc_c);
#endif
        }
    }
    else
    {
        SLWP_McpsDataCnfCB(NULL);

#if THR_ENABLE_MGMT_DIAGNOSTICS
        /* Count dropped outbound packets because of lack of memory */
        (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(instanceId, NULL, gThrMacInstIdSearch_c),
                                             gMgmtDiagnostic_ifOutDiscardsPkts_c,
                                             gMgmtDiagnostic_LinkData1StepInc_c);
#endif
    }

    return (macAbsResultType_t)result;
}

/*!*************************************************************************************************
\fn     static macAbsResultType_t MAC_McpsPurgeReq(uint8_t msduHandle, instanceId_t instanceId)
\brief  Request purging of a packet from the MAC indirect queue using the msduHandle of the packet

\param [in] msduHandle      msduHandle of the packet being purged
\param [in] instanceId      mac instance id
***************************************************************************************************/
static macAbsResultType_t MAC_McpsPurgeReq
(
    uint8_t msduHandle,
    instanceId_t instanceId
)
{

    resultType_t result = gTransactionOverflow_c;
    mcpsPurgeReq_t *pPurgeReq;
    nwkToMcpsMessage_t* pNwkToMcpsMsg = NWKU_MEM_BufferAlloc(sizeof(nwkToMcpsMessage_t));
    if(pNwkToMcpsMsg)
    {
        pNwkToMcpsMsg->msgType = gMcpsPurgeReq_c;
        pPurgeReq = &pNwkToMcpsMsg->msgData.purgeReq;

        pPurgeReq->msduHandle = msduHandle;

        result = NWK_MCPS_SapHandler(pNwkToMcpsMsg, instanceId);
        if(gSuccess_c != result)
        {
            MEM_BufferFree(pNwkToMcpsMsg);
        }
    }

    return (macAbsResultType_t)result;
}
/*!*************************************************************************************************
\fn     static macAbsMlmeSetCnf_t MAC_MlmeSetReq(macAbsMlmeSetReq_t * pParam, instanceId_t instanceId)
\brief

\param [in]
\param [in]   instanceId      mac instance id
***************************************************************************************************/
static macAbsMlmeSetCnf_t MAC_MlmeSetReq(macAbsMlmeSetReq_t * pParam, instanceId_t instanceId)
{
    mlmeMessage_t mlmeMsg = {.msgType = gMlmeSetReq_c};
    resultType_t result;

    mlmeMsg.msgData.setReq.pibAttribute = pParam->pibAttribute;
    mlmeMsg.msgData.setReq.pibAttributeIndex = pParam->pibAttributeIndex;
    mlmeMsg.msgData.setReq.pibAttributeValue = pParam->pPibAttributeValue;
    result = NWK_MLME_SapHandler(&mlmeMsg, instanceId);

    return (macAbsMlmeSetCnf_t)result;
}
/*!*************************************************************************************************
\fn     static macAbsMlmeGetCnf_t * MAC_MlmeGetReq(macAbsMlmeSetReq_t * pParam, instanceId_t instanceId)
\brief

\param [in]
\param [in]  instanceId      mac instance id
***************************************************************************************************/
static macAbsMlmeGetCnf_t * MAC_MlmeGetReq
(
    macAbsMlmeGetReq_t * pParams,
    instanceId_t instanceId
)
{
    mlmeMessage_t mlmeMsg = {.msgType = gMlmeGetReq_c};
    macAbsMlmeGetCnf_t * pMacAdaptMlmeGetCnf;
    uint8_t * pAttributeValue;
    resultType_t result;
    uint8_t defaultPibSize = MAC_DEFAULT_PIB_SIZE;

    pMacAdaptMlmeGetCnf = (macAbsMlmeGetCnf_t *)NWKU_MEM_BufferAlloc(sizeof(macAbsMlmeGetCnf_t));
    if (pMacAdaptMlmeGetCnf)
    {
        if (gMacAbsMPibBeaconPayload_c == pParams->pibAttribute)
        {
            defaultPibSize = MAC_BEACON_PIB_SIZE;
        }

        pAttributeValue = (uint8_t *)NWKU_MEM_BufferAlloc(defaultPibSize);
        if (pAttributeValue)
        {
            mlmeMsg.msgData.getReq.pibAttributeIndex = pParams->pibAttributeIndex;
            mlmeMsg.msgData.getReq.pibAttribute = pParams->pibAttribute;
            mlmeMsg.msgData.getReq.pibAttributeValue = pAttributeValue;
            result = NWK_MLME_SapHandler(&mlmeMsg, instanceId);

            pMacAdaptMlmeGetCnf->pibAttribute = pParams->pibAttribute;
            pMacAdaptMlmeGetCnf->pibAttributeIndex = pParams->pibAttributeIndex;
            pMacAdaptMlmeGetCnf->pPibAttributeValue = pAttributeValue;
            pMacAdaptMlmeGetCnf->status = (macAbsResultType_t)result;
        }
        else
        {
            MEM_BufferFree(pMacAdaptMlmeGetCnf);
        }
    }
    return pMacAdaptMlmeGetCnf;
}
/*!*************************************************************************************************
\fn     static macAbsMlmeResetCnf_t MAC_MlmeResetReq(macAbsMlmeResetReq_t * pParam, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static macAbsMlmeResetCnf_t MAC_MlmeResetReq
(
    macAbsMlmeResetReq_t * pParam,
    instanceId_t instanceId
)
{
    macAbsMlmeResetCnf_t mlmeResetCnf = gMacAbsSuccess_c;


    return (macAbsMlmeResetCnf_t)mlmeResetCnf;
}
/*!*************************************************************************************************
\fn     static macAbsMlmeScanCnf_t MAC_MlmeScanReq(macAbsMlmeScanReq_t * pParam, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static macAbsMlmeScanCnf_t MAC_MlmeScanReq
(
    macAbsMlmeScanReq_t * pParam,
    instanceId_t instanceId
)
{
    resultType_t result;
    macAbsMlmeScanCnf_t scanCnf = {0};
    mlmeScanReq_t *pScanReq;

    scanCnf.instanceId = instanceId;
    scanCnf.status = (macAbsResultType_t)gMacAbsTransactionOverflow_c;

    mlmeMessage_t* pMlmeMsg = NWKU_MEM_BufferAlloc(sizeof(mlmeMessage_t));

    if(pMlmeMsg)
    {
        pMlmeMsg->msgType = gMlmeScanReq_c;
        pScanReq = &pMlmeMsg->msgData.scanReq;

        pScanReq->scanType = (macScanType_t)pParam->scanType;
        pScanReq->scanChannels = pParam->scanChannels;
        pScanReq->scanDuration = pParam->scanDuration;
        pScanReq->securityLevel = gMacSecurityNone_c;

        NvSetCriticalSection();
        if(pScanReq->scanType == gScanModeActive_c)
        {
            mAllowBeacons = TRUE;
        }

        result = NWK_MLME_SapHandler(pMlmeMsg, instanceId);

        scanCnf.status = (macAbsResultType_t)result;

        if(result != gSuccess_c)
        {
            mAllowBeacons = FALSE;
            NvClearCriticalSection();

            if(mMacCallbackFunctions.mlmeScanCnf)
            {
                /* Send confirm with error status */
                macAbsMlmeScanCnf_t *pMlmeScanCnf = (macAbsMlmeScanCnf_t*)pMlmeMsg;

                pMlmeScanCnf->status = (macAbsResultType_t)result;
                pMlmeScanCnf->instanceId = instanceId;
                pMlmeScanCnf->scanType =  (macAbsScanType_t)pParam->scanType;
                pMlmeScanCnf->resultListSize = 0;
                pMlmeScanCnf->pEnergyDetectList = NULL;
                /* Call the registered callback */
                mMacCallbackFunctions.mlmeScanCnf(pMlmeScanCnf);

                if(pParam->scanType == (macAbsScanType_t)gScanModeActive_c)
                {
                  mAllowBeacons = FALSE;
                }
            }
            else
            {
              MEM_BufferFree(pMlmeMsg);
            }
        }
    }
    return scanCnf;
}
/*!*************************************************************************************************
\fn     static macAbsMlmeStartCnf_t MAC_MlmeStartReq(macAbsMlmeStartReq_t * pParam,
        instanceId_t instanceId)
\brief Generates a MLME Start command to MAC

\param [in] pParam          mlme start request paremeters
\param [in] instanceId      mac instance id
***************************************************************************************************/
static macAbsMlmeStartCnf_t MAC_MlmeStartReq
(
    macAbsMlmeStartReq_t * pParam,
    instanceId_t instanceId
)
{

    resultType_t result = gTransactionOverflow_c;
    mlmeStartReq_t *pStartReq;
    mlmeMessage_t* pMlmeMsg = NWKU_MEM_BufferAlloc(sizeof(mlmeMessage_t));
    if(pMlmeMsg)
    {
        pMlmeMsg->msgType = gMlmeStartReq_c;
        pStartReq = &pMlmeMsg->msgData.startReq;

        pStartReq->beaconOrder = 0x0F;
        pStartReq->superframeOrder = 0x0F;
        pStartReq->panCoordinator = FALSE;
        pStartReq->beaconSecurityLevel = gMacSecurityNone_c;
        pStartReq->coordRealignSecurityLevel = gMacSecurityNone_c;
        pStartReq->batteryLifeExtension = FALSE;
        pStartReq->coordRealignment = FALSE;
        pStartReq->logicalChannel = pParam->channel;
        pStartReq->channelPage = gChannelPageId0_c;
        pStartReq->panId = pParam->panId;

        result = NWK_MLME_SapHandler(pMlmeMsg, instanceId);
        if(gSuccess_c != result)
        {
            MEM_BufferFree(pMlmeMsg);
        }
    }


    return (macAbsMlmeStartCnf_t)result;
}

/*!*************************************************************************************************
\fn     static void MAC_MlmePollReq(macAbsMlmePollReq_t * pParam, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static void MAC_MlmePollReq
(
    macAbsMlmePollReq_t * pParam,
    instanceId_t instanceId
)
{
    resultType_t result;
    mlmePollReq_t *pPollReq;

    mlmeMessage_t *pMlmeMsg = NWKU_MEM_BufferAlloc(sizeof(mlmeMessage_t));
    if (pMlmeMsg)
    {
        pMlmeMsg->msgType = gMlmePollReq_c;
        pPollReq = &pMlmeMsg->msgData.pollReq;

        pPollReq->coordAddrMode = (addrModeType_t)pParam->coordAddrMode;
        pPollReq->coordPanId = pParam->coordPanId;
        pPollReq->coordAddress = pParam->coordAddress;
        pPollReq->securityLevel = (macSecurityLevel_t)pParam->securityLevel;
        pPollReq->keyIdMode = (keyIdModeType_t)pParam->keyIdMode;
        pPollReq->keySource = pParam->keySource;
        pPollReq->keyIndex = pParam->keyIndex;

        result = NWK_MLME_SapHandler(pMlmeMsg, instanceId);

        if (gSuccess_c != result)
        {
            /* Free Request */
            MEM_BufferFree(pMlmeMsg);

            EVM_EventNotify(gThrEv_GeneralInd_AllowDeviceToSleep_c, NULL, 0, 0);
            /* Send Confirm */
            if (mMacCallbackFunctions.mlmePollCnf)
            {
                macAbsMlmePollCnf_t *pMlmePollCnf = NWKU_MEM_BufferAlloc(sizeof(macAbsMlmePollCnf_t));

                if(pMlmePollCnf)
                {
                    pMlmePollCnf->instanceId = instanceId;
                    pMlmePollCnf->status = (macAbsResultType_t)result;

                    mMacCallbackFunctions.mlmePollCnf(pMlmePollCnf);
                }
            }
        }
        else
        {
#if gLEDSupported_d
            Led_MacTxOn();
#endif
        }
    }
}
/*!*************************************************************************************************
\fn     static void MAC_MlmeBeaconNotifyIndCB(mlmeBeaconNotifyInd_t * pPayload, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static void MAC_MlmeBeaconNotifyIndCB
(
    mlmeBeaconNotifyInd_t * pPayload,
    instanceId_t instanceId
)
{
    if((TRUE == MacFiltering_KeepPacket((macAbsAddrModeType_t)pPayload->pPanDescriptor->coordAddrMode,
                                        pPayload->pPanDescriptor->coordAddress ,
                                        &pPayload->pPanDescriptor->linkQuality))
        && mMacCallbackFunctions.mlmeBeaconNotifyInd
        && mAllowBeacons)
    {
        macAbsMlmeBeaconNotifyInd_t *pMlmeBeaconInd = NWKU_MEM_BufferAlloc(pPayload->sduLength +
            sizeof(macAbsMlmeBeaconNotifyInd_t));

        if(NULL == pMlmeBeaconInd)
        {
            /* Free internal elements */
            MEM_BufferFree(pPayload->pBufferRoot);
            return;
        }

        /* Populate the MAC abstraction structure */
        pMlmeBeaconInd->slwpInstId                  = instanceId;
        pMlmeBeaconInd->panDescriptor.address       = pPayload->pPanDescriptor->coordAddress;
        pMlmeBeaconInd->panDescriptor.coordPANId    = pPayload->pPanDescriptor->coordPanId;
        pMlmeBeaconInd->panDescriptor.addrMode      = (macAbsAddrModeType_t)pPayload->pPanDescriptor->coordAddrMode;
        pMlmeBeaconInd->panDescriptor.channel       = pPayload->pPanDescriptor->logicalChannel;
        pMlmeBeaconInd->panDescriptor.linkQuality   = pPayload->pPanDescriptor->linkQuality;
        pMlmeBeaconInd->beaconPloadSize             = pPayload->sduLength;
        FLib_MemCpy(pMlmeBeaconInd->aBeaconPload, pPayload->pSdu, pMlmeBeaconInd->beaconPloadSize);
        /* Call the registered callback */
        mMacCallbackFunctions.mlmeBeaconNotifyInd(pMlmeBeaconInd);
    }

    /* Free internal elements */
    MEM_BufferFree(pPayload->pBufferRoot);

}
/*!*************************************************************************************************
\fn     static void MAC_MlmeScanCnfCB(mlmeScanCnf_t *pPayload, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static void MAC_MlmeScanCnfCB
(
    mlmeScanCnf_t *pPayload,
    instanceId_t instanceId
)
{
    if(mMacCallbackFunctions.mlmeScanCnf)
    {
        uint8_t size  = sizeof(macAbsMlmeScanCnf_t);

        if(pPayload->scanType == gScanModeED_c)
        {
          size += (sizeof(uint8_t) * pPayload->resultListSize);
        }
        macAbsMlmeScanCnf_t *pMlmeScanCnf = NWKU_MEM_BufferAlloc(size);

        if(pMlmeScanCnf)
        {
            /* Populate the MAC abstraction structure */
            pMlmeScanCnf->status = (macAbsResultType_t)pPayload->status;
            pMlmeScanCnf->instanceId = instanceId;
            pMlmeScanCnf->scanType =  (macAbsScanType_t)pPayload->scanType;
            pMlmeScanCnf->resultListSize = 0;
            pMlmeScanCnf->pEnergyDetectList = NULL;
            if(pPayload->scanType == gScanModeED_c)
            {
                if(pPayload->resList.pEnergyDetectList)
                {
                    /* Point at the end and fill the energy detect list */
                    pMlmeScanCnf->resultListSize = pPayload->resultListSize;
                    pMlmeScanCnf->pEnergyDetectList = (uint8_t*)(pMlmeScanCnf+1);
                    FLib_MemCpy(pMlmeScanCnf->pEnergyDetectList,
                        pPayload->resList.pEnergyDetectList,
                        pPayload->resultListSize);
                }
            }
        }
        /* Call the registered callback */
        mMacCallbackFunctions.mlmeScanCnf(pMlmeScanCnf);
    }

    if(pPayload->scanType == gScanModeActive_c)
    {
        mAllowBeacons = FALSE;
        /* Free the PAN Descriptor list */
        panDescriptorBlock_t *pNextDescriptor = pPayload->resList.pPanDescriptorBlockList;
        while (pNextDescriptor)
        {
            panDescriptorBlock_t *pTempNextDescriptor = pNextDescriptor->pNext;
            MEM_BufferFree(pNextDescriptor);
            pNextDescriptor = pTempNextDescriptor;
        }
    }
    else if(pPayload->scanType == gScanModeED_c)
    {
        if(pPayload->resList.pEnergyDetectList)
        {
            MEM_BufferFree(pPayload->resList.pEnergyDetectList);
        }
    }


}
/*!*************************************************************************************************
\fn     static bool_t MAC_SetPib(uint32_t pibAttribute, void* pPibValue, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static bool_t MAC_SetPib
(
    uint32_t pibAttribute,
    void* pPibValue,
    instanceId_t instanceId
)
{
    mlmeMessage_t mlmeMsg = {.msgType = gMlmeSetReq_c};

    mlmeMsg.msgData.setReq.pibAttribute         = pibAttribute;
    mlmeMsg.msgData.setReq.pibAttributeIndex    = 0;
    mlmeMsg.msgData.setReq.pibAttributeValue    = pPibValue;
    return (gSuccess_c == NWK_MLME_SapHandler(&mlmeMsg, instanceId)) ? TRUE : FALSE;
}
/*!*************************************************************************************************
\fn     static bool_t MAC_GetPib(uint32_t pibAttribute,,  void* pPibValue, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static bool_t MAC_GetPib
(
    uint32_t pibAttribute,
    void* pPibValue,
    instanceId_t instanceId
)
{
    mlmeMessage_t mlmeMsg = {.msgType = gMlmeGetReq_c};

    mlmeMsg.msgData.setReq.pibAttribute         = pibAttribute;
    mlmeMsg.msgData.setReq.pibAttributeIndex    = 0;
    mlmeMsg.msgData.setReq.pibAttributeValue    = pPibValue;
    return (gSuccess_c == NWK_MLME_SapHandler(&mlmeMsg, instanceId)) ? TRUE : FALSE;
}
/*!*************************************************************************************************
\fn     static bool_t MAC_SetPibUint8Va(uint32_t pibAttribute, uint8_t pibValue, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static bool_t MAC_SetPibUint8Val
(
    uint32_t pibAttribute,
    uint8_t pibValue,
    instanceId_t instanceId
)
{
    return MAC_SetPib(pibAttribute, &pibValue, instanceId);
}
/*!*************************************************************************************************
\fn     static void MAC_MlmeCommStatusIndCB(mlmeCommStatusInd_t * pPayload, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static void MAC_MlmeCommStatusIndCB
(
    mlmeCommStatusInd_t * pPayload,
    instanceId_t instanceId
)
{
    if (TRUE == MacFiltering_KeepPacket((macAbsAddrModeType_t)pPayload->srcAddrMode, pPayload->srcAddress, NULL))
    {
        if (mMacCallbackFunctions.mlmeCommStatusInd)
        {
            /* Allocate memory for data. MUST be FREED by Upper Layer */
            macAbsMlmeCommStatusInd_t *pMlmeCommStatusInd = NWKU_MEM_BufferAlloc(sizeof(macAbsMlmeCommStatusInd_t));
            if(pMlmeCommStatusInd)
            {
                /* Populate the MAC abstraction structure */
                pMlmeCommStatusInd->dstAddr        = pPayload->destAddress;
                pMlmeCommStatusInd->dstAddrMode    = (macAbsAddrModeType_t)pPayload->destAddrMode;
                pMlmeCommStatusInd->keyIdMode      = (macAbsKeyIdModeType_t)pPayload->keyIdMode;
                pMlmeCommStatusInd->keyIndex       = pPayload->keyIndex;
                pMlmeCommStatusInd->keySource      = pPayload->keySource;
                pMlmeCommStatusInd->panId          = pPayload->panId;
                pMlmeCommStatusInd->securityLevel  = (macAbsSecurityLevel_t)pPayload->securityLevel;
                pMlmeCommStatusInd->srcAddr        = pPayload->srcAddress;
                pMlmeCommStatusInd->srcAddrMode    = (macAbsAddrModeType_t)pPayload->srcAddrMode;
                pMlmeCommStatusInd->status         = (macAbsResultType_t)pPayload->status;

                pMlmeCommStatusInd->macInstanceId  = instanceId;
                /* Fix bug in MAC */
                if (gMacAbsAddrModeShortAddress_c == pMlmeCommStatusInd->dstAddrMode)
                {
                    pMlmeCommStatusInd->dstAddr &= (uint64_t)0xFFFF;
                }
                /* Fix bug in MAC */
                if (gMacAbsAddrModeShortAddress_c == pMlmeCommStatusInd->srcAddrMode)
                {
                    pMlmeCommStatusInd->srcAddr &= (uint64_t)0xFFFF;
                }

#if THR_ENABLE_MGMT_DIAGNOSTICS
                if(pPayload->status != gSuccess_c)
                {
#if DEBUG_MAC_COUNTERS
                    if(MAC_DebugCheckInErrorsArray(pPayload->status))
                    {
#endif
                        /* Count inbound packets that could not be transmitted because of errors */
                        (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(instanceId, NULL, gThrMacInstIdSearch_c),
                                                             gMgmtDiagnostic_ifInErrorsPkts_c,
                                                             gMgmtDiagnostic_LinkData1StepInc_c);
#if DEBUG_MAC_COUNTERS
                    }
#endif
                }
#endif

                mMacCallbackFunctions.mlmeCommStatusInd(pMlmeCommStatusInd);
            }
            else
            {
                /* Free mcpsToNwkMessage_t block from mac */
                MEM_BufferFree(pPayload);
#if THR_ENABLE_MGMT_DIAGNOSTICS
                /* Count dropped inbound packets because of lack of memory */
                (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(instanceId, NULL, gThrMacInstIdSearch_c),
                                                     gMgmtDiagnostic_ifInDiscardsPkts_c,
                                                     gMgmtDiagnostic_LinkData1StepInc_c);
#endif
            }
        }
    }
}
/*!*************************************************************************************************
\fn     static void MAC_MlmePollIndCB( mlmePollNotifyInd_t *pPayload, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static void MAC_MlmePollIndCB
(
    mlmePollNotifyInd_t *pPayload,
    instanceId_t instanceId
)
{
    macAbsMlmePollNotifyInd_t mlmePollInd;

    /* Fix bug in MAC */
    if (gAddrModeShortAddress_c == pPayload->srcAddrMode)
    {
        pPayload->srcAddr &= (uint64_t)0xFFFF;
    }

    mlmePollInd.instanceId = instanceId;
    mlmePollInd.srcAddr = pPayload->srcAddr;
    mlmePollInd.srcAddrMode = (macAbsAddrModeType_t)pPayload->srcAddrMode;
    mlmePollInd.srcPanId = pPayload->srcPanId;

    SLWP_MlmePollIndCB(&mlmePollInd, instanceId);
}
/*!*************************************************************************************************
\fn     static void MAC_MlmePollCnfCB(mlmePollCnf_t * pPayload, instanceId_t instanceId)
\brief

\param [in]
\param [in] instanceId      mac instance id
***************************************************************************************************/
static void MAC_MlmePollCnfCB
(
    mlmePollCnf_t * pPayload,
    instanceId_t instanceId
)
{

    EVM_EventNotify(gThrEv_GeneralInd_AllowDeviceToSleep_c, NULL, 0, 0);
    if (mMacCallbackFunctions.mlmePollCnf)
    {
        macAbsMlmePollCnf_t *pMlmePollCnf = NWKU_MEM_BufferAlloc(sizeof(macAbsMlmePollCnf_t));
        if(pMlmePollCnf)
        {
            pMlmePollCnf->status = (macAbsResultType_t)pPayload->status;
            pMlmePollCnf->instanceId = instanceId;
#if THR_ENABLE_MGMT_DIAGNOSTICS
            if(pPayload->status != gSuccess_c)
            {
#if DEBUG_MAC_COUNTERS
                if(MAC_DebugCheckInErrorsArray(pPayload->status))
                {
#endif
                    /* Count inbound packets that could not be transmitted because of errors */
                    (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(instanceId, NULL, gThrMacInstIdSearch_c),
                                                         gMgmtDiagnostic_ifInErrorsPkts_c,
                                                         gMgmtDiagnostic_LinkData1StepInc_c);
#if DEBUG_MAC_COUNTERS
                }
#endif
            }
#endif
            mMacCallbackFunctions.mlmePollCnf(pMlmePollCnf);
        }
        else
        {
#if THR_ENABLE_MGMT_DIAGNOSTICS
            /* Count dropped inbound packets because of lack of memory */
            (void)MgmtDiagnostic_ProcessLinkData(THR_GetInstId(instanceId, NULL, gThrMacInstIdSearch_c),
                                                 gMgmtDiagnostic_ifInDiscardsPkts_c,
                                                 gMgmtDiagnostic_LinkData1StepInc_c);
#endif
        }
    }
#if gLEDSupported_d
    Led_MacTxOff();
#endif
}


/*!*************************************************************************************************
\fn     bool_t MAC_SetFrameCounter(uint8_t channel, instanceId_t instanceId)
\private
\brief  Public interface function for the MAC abstraction module. This function sets the frame
        counter.

\param  [in]    frameCounter    link layer frame counter
\param  [in]    instanceId      mac instance id

\return         bool_t          returns FALSE if the operation failed
***************************************************************************************************/
static bool_t MAC_SetFrameCounter
(
    uint32_t frameCounter,
    instanceId_t instanceId
)
{
    return MAC_SetPib(gMPibFrameCounter_c, &frameCounter, instanceId);
}

/*!*************************************************************************************************
\fn     uint32_t MAC_GetFrameCounter(instanceId_t instanceId)
\private
\brief  Public interface function for the MAC abstraction module. This function gets the frame
        counter.

\param  [in]    instanceId  mac instance id

\return         uint8_t     frame counter
***************************************************************************************************/
static uint32_t MAC_GetFrameCounter
(
    instanceId_t instanceId
)
{
    uint32_t frameCounter = 0;

    (void)MAC_GetPib(gMPibFrameCounter_c, &frameCounter, instanceId);

    return frameCounter;
}
/*!*************************************************************************************************
\fn     static bool_t MAC_GetNeighborFrameCounter(uint32_t  *pFrameCtr,uint8_t neighborIdx,
        uint8_t  maxNeighbors, uint8_t  macKeyIdx, uint8_t  maxMacKeys,instanceId_t    instanceId)
\brief

\param [in]    pFrameCtr         pointer to rame counter
\param [in]    neighborIdx       neighbor index
\param [in]    maxNeighbors      mac number of neighbors
\param [in]    macKeyIdx         mac key index
\param [in]    maxMacKeys        max number of mac keys
\param [in]    instanceId        mac instance id

\return        bool_t            TRUE in case of sucess
***************************************************************************************************/
static bool_t MAC_GetNeighborFrameCounter
(
    uint32_t       *pFrameCtr,
    uint8_t         neighborIdx,
    uint8_t         maxNeighbors,
    uint8_t         macKeyIdx,
    uint8_t         maxMacKeys,
    instanceId_t    instanceId
)
{
    uint8_t devDescIdx = macKeyIdx * maxNeighbors + neighborIdx;
    bool_t bRetValue = MAC_SetPib(gMPibiDeviceTableCrtEntry_c, &devDescIdx, instanceId);

    if (bRetValue)
    {
        bRetValue = MAC_GetPib(gMPibDeviceDescriptorFrameCounter_c, pFrameCtr, instanceId);
    }

    return bRetValue;
}

/*! *********************************************************************************
* \fn    MAC_PhyAddToNeighborTable
* \brief  Add an device to the PHY neighbor table.
*         If a polling device is not in the neighbor table, the ACK will have FramePending=1
*
* \param[in]  pAddr     Pointer to an 802.15.4 address
* \param[in]  addrMode  The 802.15.4 addressing mode
* \param[in]  PanId     The 802.15.2 PAN Id
*
*\return       TRUE - neighbor was added; otherwise FALSE
********************************************************************************** */
static bool_t MAC_PhyAddToNeighborTable
(
    uint8_t *pAddr,
    uint8_t addrMode,
    uint16_t PanId
)
{
    return (0 == PhyAddToNeighborTable(pAddr, addrMode, PanId));
}

/*! *********************************************************************************
* \fn    MAC_PhyRemoveFromNeighborTable
* \brief  Remove an device to the PHY neighbor table.
*         If a polling device is not in the neighbor table, the ACK will have FramePending=1
*
* \param[in]  pAddr     Pointer to an 802.15.4 address
* \param[in]  addrMode  The 802.15.4 addressing mode
* \param[in]  PanId     The 802.15.2 PAN Id
*
* \return     TRUE - neighbor was removed; otherwise FALSE
********************************************************************************** */
static bool_t MAC_PhyRemoveFromNeighborTable
(
    uint8_t *pAddr,
    uint8_t addrMode,
    uint16_t PanId
)
{
    return (0 == PhyRemoveFromNeighborTable(pAddr, addrMode, PanId));
}

/*! *********************************************************************************
* \fn    MAC_PhyConvertLQIToRSSI
* \brief  This function converts the LQI reported by the PHY into an signed RSSI value
*
* \param[in]  LQI  the LQI reported by the PHY
*
* \return  the RSSI value in dbm
*
********************************************************************************** */
static int8_t MAC_PhyConvertLQIToRSSI
(
    uint8_t LQI
)
{
    return PhyConvertLQIToRSSI(LQI);
}

/*! *********************************************************************************
* \fn    MAC_DisableBroadcastReceive(instanceId_t instanceId,  bool_t allowBroadcast)
* \brief This function enables/disables receiving of broadcast frames at MAC abs level
*
* \param [in]    instanceId        mac instance id
* \param [in]    allowBroadcast    FALSE - if broadcast receiving is disabled
*                                  TRUE - if broadcast receiving is enabled
*
* \return  None
********************************************************************************** */
static void MAC_AllowBroadcast
(
    instanceId_t instanceId,
    bool_t allowBroadcast
)
{
    mAllowBroadcasts = allowBroadcast;
}

#if DEBUG_MAC_COUNTERS
/*! *********************************************************************************
* \fn    MAC_DebugCheckInErrorsArray(resultType_t status)
* \brief This function is used to check the
*
* \param   [in]    status    Status of the MAC operation
*
* \return  bool_t            TRUE if error is found in array, FALSE otherwise
********************************************************************************** */
static bool_t MAC_DebugCheckInErrorsArray
(
    resultType_t status
)
{
    uint8_t i = 1;
    bool_t result = FALSE;

    for(i = 0; i < NumberOfElements(mDebugMacErrors); i++)
    {
        if(status == mDebugMacErrors[i])
        {
            result = TRUE;
            break;
        }
    }

    return result;
}
#endif
/*================================================================================================*/

/*==================================================================================================
Private debug functions
==================================================================================================*/
