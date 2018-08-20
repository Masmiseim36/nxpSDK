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

/*==================================================================================================
Include Files
==================================================================================================*/
/* Framework */
#include "RNG_Interface.h"

/* Thread */
#include "thread_types.h"
#include "thread_cfg.h"
#include "thread_network.h"
#include "thread_app_callbacks.h"
#include "thread_utils.h"
#include "thread_meshcop_mgmt.h"
#include "thread_attributes.h"
#include "app_thread_config.h"
#include "ip_if_usbenet.h"
#include "Panic.h"
#include "dns_client.h"
#include "mle_extended.h"

#if gEnableOTAClient_d
    #include "app_ota.h"
#endif

#if STACK_THREAD
/*! Slaac Manual IID */
extern uint8_t gaThrSlaacManualIID[INET6_IID_LEN];
/*==================================================================================================
Private macros
==================================================================================================*/
#define ENABLE_MESHCOP_JOINER_FILTER_NWK_NAME    0    /*!< Enable Meshcop joiner filter network name */
#define ENABLE_MESHCOP_JOIN_FIN_FILTER_URL       1    /*!< Enable Meshcop JOIN_FIN filter URL */

#if THREAD_USE_THCI
#undef ENABLE_MESHCOP_JOINER_FILTER_NWK_NAME
#undef ENABLE_MESHCOP_JOIN_FIN_FILTER_URL
#define ENABLE_MESHCOP_JOINER_FILTER_NWK_NAME    0
#define ENABLE_MESHCOP_JOIN_FIN_FILTER_URL       1
#endif

/* this is just an example value */
#define VENDOR_SPECIFIC_IID_PREFIX_VALUE    0x00ABCD00    /*!< Vendor specific IID prefix value */
#define VENDOR_SPECIFIC_IID_PREFIX_MASK     0x000000FF    /*!< Vendor specific IID prefix mask */
#define IID_UNIVERSAL_LOCAL_BIT_MASK        0x0200000000000000 /* IID universal local bit mask (1 << 57) */

/*==================================================================================================
Private type definitions
==================================================================================================*/
/*! Structure used to track the best channel information */
typedef struct {
   uint8_t bestchannel;          /*!< Best channel */
   uint8_t energylevel;          /*!< Energy level */
   uint16_t numOfRvcdBeacons;    /*!< Number of received beacons */
}thrBestChannelInfo_t;

/*! Structure used to mark multiple positions for multiple PanIds */
typedef struct thrNwkJoiningPanCounter_tag
{
    uint16_t panId;       /*!< PAN ID */
    uint8_t positions;    /*!< Position in mask */
}thrNwkJoiningPanCounter_t;

/*==================================================================================================
Private prototypes
==================================================================================================*/
static bool_t APP_JoinerNwkListAdd(thrNwkJoiningEntry_t *pJoiningEntry, instanceId_t thrInstId);
static thrNwkActiveScanEntry_t* APP_GetNwkScanResultEntry(uint8_t channel, thrNwkScanResults_t *pScanResults);
static uint8_t APP_NwkCreateSelectBestChannel(instanceId_t thrInstId, thrNwkScanResults_t *pScanResults);
static void THR_GenerateLeaderPskc(instanceId_t thrInstId);
static void APP_NwkSelectUniquePanId(instanceId_t thrInstId, thrNwkScanResults_t *pScanResults,
                                     uint8_t bestChannel,  uint8_t *pPanId);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
thrNwkJoiningEntry_t *gpNwkJoiningList = NULL;    /*!< Network Joining List */
uint8_t gNbOfNwkJoiningEntries = 0;               /*!< Number of network joining retries */

/* Function pointer registered by application that keeps the IPv6 address of a service server */
registerServiceServerAddr_t pfRegisterServiceServerAddr = NULL;

/*==================================================================================================
Public global functions declarations
==================================================================================================*/
/*!*************************************************************************************************
\fn     APP_DiscoveryReqCb(instanceId_t thrInstId,  uint16_t tlvsSize, uint8_t* pTlvs)
\brief  This is a callback used by the Application to accept or deny the Discovery Requests.
        The Discovery Request messages could contain some application specific TLVs, and the
        APP could have filters based on these TLVs.

\param  [in]  thrInstId  Thread instance ID
\param  [in]  tlvsSize   Discovery request TLVs size
\param  [in]  pTlvs      Pointer to Discovery Request TLVs

\retval       TRUE       Send the Discovery Response
\retval       FALSE      Otherwise
***************************************************************************************************/
bool_t APP_DiscoveryReqCb
(
    instanceId_t thrInstId,  
    uint16_t tlvsSize, 
    uint8_t* pTlvs
)
{
    return TRUE;
}

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
void APP_JoinerDiscoveryRespCb
(
    instanceId_t thrInstId,
    thrDiscoveryEvent_t event,
    uint8_t lqi,
    thrDiscoveryRespInfo_t* pDiscoveryRespInfo,
    meshcopDiscoveryRespTlvs_t *pDiscoveryRespTlvs
)
{
    bool_t addToJoiningList = FALSE;

    if (event == gThrDiscoveryStarted_c)
    {
        const uint16_t maxSize  = THR_MAX_NWK_JOINING_ENTRIES * sizeof(thrNwkJoiningEntry_t);
        gNbOfNwkJoiningEntries = 0;
        if(gpNwkJoiningList == NULL)
        {
            gpNwkJoiningList = MEM_BufferAlloc(maxSize);
        }
        if(gpNwkJoiningList)
        {
            /* Reset the Network Joining list */
            FLib_MemSet(gpNwkJoiningList, 0, maxSize);
        }
    }
    else if ((event == gThrDiscoveryRespRcv_c) && gpNwkJoiningList)
    {
        thrNwkJoiningEntry_t joinerEntry = {0};
        joinerEntry.steeringMatch = gMeshcopSteeringMatchNA_c;
        joinerEntry.channel = pDiscoveryRespInfo->channel;
        joinerEntry.panId = pDiscoveryRespInfo->panId;
        FLib_MemCpy(joinerEntry.euiAddr, pDiscoveryRespInfo->aEui, 8);
        FLib_MemCpy(joinerEntry.aXpanId, pDiscoveryRespTlvs->pXpanIdTlv->xPanId,
            pDiscoveryRespTlvs->pXpanIdTlv->len);

        if(pDiscoveryRespTlvs->pCommissionerUdpPortTlv)
        {
            joinerEntry.commissionerUDPPort = ntohas(pDiscoveryRespTlvs->pCommissionerUdpPortTlv->aPort);
        }
        if(pDiscoveryRespTlvs->pJoinerUdpPortTlv)
        {
            joinerEntry.joinerUDPPort = ntohas(pDiscoveryRespTlvs->pJoinerUdpPortTlv->aPort);
        }

        /* Joiner case */
        if(gMeshcopCommissionerMode != gThrCommissionerModeNative_c)
        {
            if(THR_DISC_RSP_VER_GET(pDiscoveryRespTlvs->pDiscRespTlv->verFlags) ==  THR_PROTOCOL_VERSION)
            {
                if (THR_GetAttr_IsDevCommissioned(thrInstId) == TRUE)
                {
                    addToJoiningList = TRUE;
                }
                else
                {
                    joinerEntry.steeringMatch = MESHCOP_CheckSteeringData(pDiscoveryRespTlvs->pSteeringDataTlv);
                    if(joinerEntry.steeringMatch != gMeshcopSteeringMatchNone_c)
                    {
                        addToJoiningList = TRUE;
                    }
                }
            }
        }
        /* Native Commissioner case */
        else
        {
            /* Join as a Native Commissioner. Add filters here.*/
            if(
                (THR_DISC_RSP_VER_GET(pDiscoveryRespTlvs->pDiscRespTlv->verFlags) == THR_PROTOCOL_VERSION) &&
                (THR_DISC_RSP_N_GET(pDiscoveryRespTlvs->pDiscRespTlv->verFlags) == 1)
                )
            {
                addToJoiningList = TRUE;
            }
        }

        if(addToJoiningList)
        {
            addToJoiningList = APP_JoinerNwkListAdd(&joinerEntry, thrInstId);
        }
    }
    else if ((event == gThrDiscoveryStopped_c) && gpNwkJoiningList)
    {
        /* Start the joining process. The list will be freed by the function. */
        MESHCOP_NwkJoinWithCommissioning(thrInstId, gpNwkJoiningList, gNbOfNwkJoiningEntries);
        gpNwkJoiningList = NULL;
        gNbOfNwkJoiningEntries=0;
    }
}

/*!*************************************************************************************************
\fn     void APP_JoinerSelectNwkWithAnnounceCb(instanceId_t thrInstId, thrAnnounceEvent_t event,
                                               uint8_t lqi, uint16_t tlvsSize, uint8_t* pTlvs)
\brief  This callback handles the announcement messages to select
        a Thread Network (channel and panId) and start the attachment process.

\param  [in]  thrInstId  Thread instance
\param  [in]  event      Announcement events (see thrAnnounceEvent_t)
\param  [in]  lqi        Received packet lqi
\param  [in]  tlvsSize   The size of the received Announce TLVs
\param  [in]  pTlvs      Pointer to Announce TLVs
***************************************************************************************************/
void APP_JoinerSelectNwkWithAnnounceCb
(
    instanceId_t thrInstId, 
    thrAnnounceEvent_t event,
    uint8_t lqi,
    uint16_t tlvsSize, 
    uint8_t* pTlvs
)
{
    static uint8_t gThrNwkChannel;
    static uint16_t gThrNwkPanId;
    /* first 6 bytes are the seconds; last 2 bytes are the ticks */
    static uint8_t gThrMaxActiveTimestamp[8];

    if(gThrSearchThreadNwkStarted_c == event)
    {
        gThrNwkChannel = 0;
        gThrNwkPanId = THR_ALL_FFs16;
        FLib_MemSet(gThrMaxActiveTimestamp, 0, 8);
    }
    else if(gThrAnnounceRespRcv_c == event)
    {
        mleOtaTlvChannel_t *pChannelTlv = (mleOtaTlvChannel_t *)NWKU_GetTlv(
            gMleTlvChannel_c, pTlvs, tlvsSize, NULL, NULL);
        mleOtaTlvPanId_t *pPanIdTlv = (mleOtaTlvPanId_t *)NWKU_GetTlv(
            gMleTlvPanId_c, pTlvs, tlvsSize, NULL, NULL);
        mleOtaTlvActiveTimestamp_t *pTimestampTlv = (mleOtaTlvActiveTimestamp_t *) NWKU_GetTlv(
            gMleTlvActiveTimestamp_c, pTlvs, tlvsSize, NULL, NULL);

        if(pChannelTlv && pPanIdTlv && pTimestampTlv)
        {
            /* Select the Thread network with the greatest active timestamp */
            if( (1 == NWKU_isArrayGreater(pTimestampTlv->seconds, &gThrMaxActiveTimestamp[0], 6)) ||
                ( (0 == NWKU_isArrayGreater( pTimestampTlv->seconds, &gThrMaxActiveTimestamp[0], 6))
                   && (1 == NWKU_isArrayGreater( pTimestampTlv->ticks, &gThrMaxActiveTimestamp[6], 2)) )
               )
            {
                /* Store the received information */
                FLib_MemCpy(gThrMaxActiveTimestamp, pTimestampTlv->seconds, 8);
                gThrNwkChannel = ntohas(pChannelTlv->channel);
                gThrNwkPanId = ntohas(pPanIdTlv->panId);
            }
        }
    }
    else if(gThrSearchThreadNwkStopped_c == event)
    {
        if(gThrNwkChannel != 0)
        {
            /* Set the panid and channel and attach. */
            THR_SetAttr(thrInstId, gNwkAttrId_Channel_c, 0,
                sizeof(gpaThrActiveAttr[thrInstId]->channel), &gThrNwkChannel);
            THR_SetAttr(thrInstId, gNwkAttrId_PanId_c, 0, sizeof(gpaThrActiveAttr[thrInstId]->panId),
                &gThrNwkPanId);
            THR_NwkAttach(thrInstId);
        }
        else
        {
            EVM_EventNotify(gThrEv_NwkJoinCnf_Failed_c, NULL, 0, thrInstId);
        }
    }
}

/*!*************************************************************************************************
\fn     void APP_JoinerSelectNwkWithBeaconCb(void *pParam)
\brief  This function is used to handle the network events during the meshcop joining. This is the
        callback function used to select the potential network to join when THR_NwkJoin() is called.

\param  [in]  pParam -  pointer to event messages (thrEvmParams_t *)
***************************************************************************************************/
void APP_JoinerSelectNwkWithBeaconCb
(
    void *pParam
)
{
    thrEvmParams_t *pEventParams = (thrEvmParams_t *)pParam;
    thrBeaconInfo_t *pThrBeacon = &pEventParams->pEventData->nwkSelectParentsInd;
    instanceId_t thrInstId = pEventParams->thrInstId;
    bool_t addToJoiningList = FALSE;
    const uint16_t maxSize  = THR_MAX_NWK_JOINING_ENTRIES * sizeof(thrNwkJoiningEntry_t);

    if (gThrEv_NwkSelectParentsInd_ScanStarted_c == pEventParams->code)
    {
        gNbOfNwkJoiningEntries = 0;
        gpNwkJoiningList = MEM_BufferAlloc(maxSize);
        if(gpNwkJoiningList)
        {
          /* Reset the Network Joining list */
          FLib_MemSet(gpNwkJoiningList, 0, maxSize);
        }

    }
    else if ((gThrEv_NwkSelectParentsInd_RcvBeacon_c == pEventParams->code) && gpNwkJoiningList)
    {
        meshCopJoinerUdpPortTlv_t *pJoinerPortTlv = NULL;
        meshCopCommissionerUdpPortTlv_t* pCommPortTlv = NULL;

        if( 0 == gNbOfNwkJoiningEntries)
        {
           FLib_MemSet(gpNwkJoiningList, 0, maxSize);
        }
        if(gMeshcopCommissionerMode != gThrCommissionerModeNative_c)
        {
            meshCopSteeringTlv_t *pSteeringTlv = (meshCopSteeringTlv_t*)NWKU_GetTlv(
                gMeshCopTlvSteeringData_c, pThrBeacon->payload.aTlvs,
                pThrBeacon->payloadSize - sizeof(pThrBeacon->payload), NULL, NULL);

            /* Join as an end device. Add filters here.*/
            if (   (1 == THR_BEACON_J_FLAG_GET(pThrBeacon->payload.flags))
                && (pSteeringTlv != NULL)
                && (MESHCOP_CheckSteeringData(pSteeringTlv) != gMeshcopSteeringMatchNone_c)
                && (pThrBeacon->lqi >= THR_GetAttr_JoinLqiThreshold(thrInstId))
#if ENABLE_MESHCOP_JOINER_FILTER_NWK_NAME
                &&  FLib_MemCmp(pThrBeacon->payload.nwkName, gaThrDeviceConfig[thrInstId].nwkName.aStr, gaThrDeviceConfig[thrInstId].nwkName.length)
#endif
            )
            {
                pJoinerPortTlv = (meshCopJoinerUdpPortTlv_t*)NWKU_GetTlv(gMeshCopTlvJoinerUdpPort_c,
                    pThrBeacon->payload.aTlvs,
                    pThrBeacon->payloadSize - sizeof(pThrBeacon->payload), NULL, NULL);
                addToJoiningList = TRUE;
            }
        }
        else
        {
            /* Join as a Commissioner. Add filters here.*/
            if ((1 == THR_BEACON_N_FLAG_GET(pThrBeacon->payload.flags))
                && (pThrBeacon->lqi >= THR_GetAttr_JoinLqiThreshold(thrInstId))
            )
            {
                pCommPortTlv = (meshCopCommissionerUdpPortTlv_t*)NWKU_GetTlv(
                    gMeshCopTlvCommissionerUdpPort_c, pThrBeacon->payload.aTlvs,
                    pThrBeacon->payloadSize - sizeof(pThrBeacon->payload), NULL, NULL);
                addToJoiningList = TRUE;
            }
        }

        if(addToJoiningList)
        {
            thrNwkJoiningEntry_t joinerEntry = {0};
            joinerEntry.channel = pThrBeacon->channel;
            joinerEntry.panId = pThrBeacon->panid;
            FLib_MemCpy(joinerEntry.euiAddr, &pThrBeacon->address, 8);
            if(pJoinerPortTlv != NULL)
            {
                joinerEntry.joinerUDPPort = ntohas(pJoinerPortTlv->aPort);
            }
            if(pCommPortTlv != NULL)
            {
                joinerEntry.commissionerUDPPort = ntohas(pCommPortTlv->aPort);
            }
            addToJoiningList = APP_JoinerNwkListAdd(&joinerEntry, thrInstId);
        }
    }
    else if ((gThrEv_NwkSelectParentsInd_ScanEnded_c == pEventParams->code) && gpNwkJoiningList)
    {

        MESHCOP_NwkJoinWithCommissioning(thrInstId, gpNwkJoiningList, gNbOfNwkJoiningEntries);
        gpNwkJoiningList = NULL;
        gNbOfNwkJoiningEntries=0;
    }

    /* Free Event Buffer */
    MEM_BufferFree(pThrBeacon);
    MEM_BufferFree(pEventParams);
}

/*!*************************************************************************************************
\fn    APP_NwkCreateCb
\brief This is the callback to handle the events during the network creation procedure
       (when THR_NwkCreate() is called). This function select the best channel to form the network.

\param  [in] pParam - pointer to received event message (thrEvmParams_t *)

\retval None
***************************************************************************************************/
void APP_NwkCreateCb
(
    void *pParam
)
{
    thrEvmParams_t *pEventParams = (thrEvmParams_t *)pParam;
    instanceId_t thrInstId = pEventParams->thrInstId;

    if (gThrEv_NwkCreateCnf_Success_c == pEventParams->code)
    {
        /* TREAT THIS EVENT HERE */
    }
    else  if (gThrEv_NwkCreateCnf_Failed_c == pEventParams->code)
    {
       /* TREAT THIS EVENT HERE */
    }
    else if (gThrEv_NwkCreateInd_SelectBestChannel_c == pEventParams->code)
    {
        thrNwkScanResults_t *pScanResults = &pEventParams->pEventData->nwkScanCnf;
        uint8_t channel;
        uint16_t panId = THR_GetAttr_PanId(thrInstId);

        if (pScanResults)
        {
            channel = APP_NwkCreateSelectBestChannel(thrInstId, pScanResults);

            if (channel)
            {
                if(panId == THR_ALL_FFs16)
                {
                    APP_NwkSelectUniquePanId (thrInstId, pScanResults, channel, (uint8_t*)&panId);
                }

                /* Set the channel */
                THR_SetAttr(thrInstId, gNwkAttrId_Channel_c, 0, sizeof(channel), &channel);
            }

            /* Free Event Buffer */
            MEM_BufferFree(pScanResults);
        }
        /* Set the pan id */
        THR_SetAttr(thrInstId,gNwkAttrId_PanId_c,0,sizeof(panId),&panId);

        /* Create the network */
        THR_NwkCreate(thrInstId);
    }
    else if (gThrEv_NwkCreateInd_GeneratePSKc_c == pEventParams->code)
    {
#if USBENET_ROUTER
        IP_UsbEnetDisable();
#endif
        /* By default, use the PSKd passphrase to generate the PSKc (see meshcop spec) */
        THR_GenerateLeaderPskc(thrInstId);
#if USBENET_ROUTER
        IP_UsbEnetEnable();
#endif
    }

    MEM_BufferFree(pEventParams);
}

/*!*************************************************************************************************
\fn     bool_t APP_OutOfBandSelectNwkWithBeaconCb(instanceId_t thrInstId, thrBeaconInfo_t *pThrBeacon)
\brief  This is the callback function used to select a thread network (find the panId, channel etc.)
        when the device in out-of-band configured and THR_NwkJoin(thrInst, gUseMACBeacon_c) is called.
        This function should filter the received beacons and select a thread network to start the
        attachment process.

\param  [in]  thrInstId   The thread instance ID
\param  [in]  pThrBeacon  Pointer to received Beacon

\retval       TRUE        A network has been selected
\retval       FALSE       No network has been selected
***************************************************************************************************/
bool_t APP_OutOfBandSelectNwkWithBeaconCb
(
    instanceId_t thrInstId, 
    thrBeaconInfo_t *pThrBeacon
)
{
    bool_t useThisNWK = FALSE;

    /* ADD YOUR FILTER HERE. */
    if(gaThrDeviceConfig[thrInstId].outOfBandChannel != 0)
    {
        /* If the channel is known, apply this filter */
        if(gaThrDeviceConfig[thrInstId].outOfBandChannel == pThrBeacon->channel)
        {
            useThisNWK = TRUE;
        }
    }
    else if(ntohall(gaThrDeviceConfig[thrInstId].xPanId) != THR_ALL_FFs64)
    {
        /* If the extended pan ID is known, apply this filter */
        if( FLib_MemCmp(pThrBeacon->payload.xpanId, gaThrDeviceConfig[thrInstId].xPanId,8) )
        {
            useThisNWK = TRUE;
        }
    }
#if ENABLE_MESHCOP_JOINER_FILTER_NWK_NAME
    else if(gaThrDeviceConfig[thrInstId].nwkName.length != 0)
    {
        /* If the network name is known, apply this filter */
        if(FLib_MemCmp(pThrBeacon->payload.nwkName, gaThrDeviceConfig[thrInstId].nwkName.aStr, 16))
        {
            useThisNWK = TRUE;
        }
    }
#endif
    else
    {
        useThisNWK = TRUE;
    }

    return useThisNWK;
}

/*!*************************************************************************************************
\fn     APP_MeshcopValidateJoinerAddrCb
\brief  This is the callback function used to check if a Joiner will be accepted by our DTLS server.

\param  [in]  thrInstId     the thread instance ID
\param  [in]  pIpAddr       pointer to client IP address

\retval       TRUE          the Joiner is known
\retval       FALSE         the Joiner is unknown
***************************************************************************************************/
bool_t APP_MeshcopValidateJoinerAddrCb
(
    instanceId_t thrInstId,
    ipAddr_t *pIpAddr
)
{
    bool_t result = FALSE;
#if !THREAD_ED_CONFIG
    expectedJoinerEntry_t *pJoinerPsk;
    llAddr_t joinerLlAddr;

    if(THR_GetAttr_PermitJoin(thrInstId))
    {

        NWKU_GetLLAddrFromIID(&pIpAddr->addr8[8], &joinerLlAddr);

        /* Check if we have this device recorded */
        pJoinerPsk = MESHCOP_GetExpectedJoiner(thrInstId, joinerLlAddr.eui, NULL);
        if(pJoinerPsk)
        {
            result = TRUE;
        }
        else
        {
            uint8_t aFfs[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
            pJoinerPsk = MESHCOP_GetExpectedJoiner(thrInstId, NULL, aFfs);
            if(pJoinerPsk)
            {
                result = TRUE;
            }
        }
    }
#endif /* !THREAD_ED_CONFIG */
    return result;
}

/*!*************************************************************************************************
\fn     bool_t APP_MeshCopValidateJoinFinCb(instanceId_t thrInstId, meshCopJoinFinTlvs_t* pJoinFinTlvs)
\brief  Function used to check the TLVs given by the Joiner in the Join Finalization step.

\param  [in]  thrInstId      Thread instance ID
\param  [in]  pJoinFinTlvs   Join Finalization TLVs

\retval       TRUE           Continue joining
\retval       FALSE          Otherwise
***************************************************************************************************/
bool_t APP_MeshCopValidateJoinFinCb
(
    instanceId_t thrInstId, 
    meshCopJoinFinTlvs_t* pJoinFinTlvs
)
{
    bool_t result = FALSE;

    /* THE BELOW CODE IS CHECKING ONLY THE URL PROVISIONING TLV.
     * THE APPLICATION CAN CHECK OTHER JOIN_FIN TLVS TO ACCEPT THE JOINER.
     *  */
#if ENABLE_MESHCOP_JOIN_FIN_FILTER_URL
    if(pJoinFinTlvs->pProvUrl && pJoinFinTlvs->pProvUrl->len)
    {
        if(FLib_MemCmp(gpaThrStringAttr[thrInstId]->provisioningURL.aStr, pJoinFinTlvs->pProvUrl->provUrl,
             gpaThrStringAttr[thrInstId]->provisioningURL.length))
        {
            result = TRUE;
        }
    }
    else
    {
        result = TRUE;
    }
#else
    result = TRUE;
#endif

    return result;
}

/*!*************************************************************************************************
\fn     bool_t APP_MeshCopValidateCommissionerCb(instanceId_t thrInstId, meshcopCommIdTlv_t *pCommIdTlv)
\brief  Function used to check the Commissioner ID. It can be accepted or rejected.

\param  [in]  thrInstId   Thread instance ID
\param  [in]  pCommIdTlv  Pointer to the Commissioner ID TLV

\retval       TRUE        Allow this Commissioner
\retval       FALSE       Reject this Commissioner
***************************************************************************************************/
bool_t APP_MeshCopValidateCommissionerCb
(
    instanceId_t thrInstId, 
    meshcopCommIdTlv_t *pCommIdTlv
)
{
    bool_t result = TRUE;

    /* The application can check for the Commissioner ID */

    return result;
}

/*!*************************************************************************************************
\fn     bool_t APP_AddressAssignSlaacCb(instanceId_t thrInstId, ipAddr_t* pPrefix)
\brief  If slaacPolicy attribute is configured to gThrSlaacManual_c this function serves as a
        callback to the application to decide if it wants to bind an address with the prefix or not
        and if so, the application can choose the IID to use with the provided prefix.

\param  [in]     thrInstId  Thread instance ID
\param  [in/out] pPrefix    Pointer to ip prefix and output to store the IID

\retval          TRUE       If the address generated with the prefix and IID should be used by the stack
\retval          FALSE      If the application does not want to use this prefix and the stack should ignore it
***************************************************************************************************/
bool_t  APP_AddressAssignSlaacCb
(
    instanceId_t thrInstId,
    ipAddr_t* pPrefix
)
{

    
    bool_t bRetBindAddr = FALSE;
    uint32_t iidSuffix, iidPrefix;
    uint64_t iid;
    
    if (0 != ntohall(gaThrSlaacManualIID))
    {   
        bRetBindAddr = TRUE;
        FLib_MemCpy(&pPrefix->addr8[8], gaThrSlaacManualIID, INET6_IID_LEN);
    }
    /* decide here to use the prefix or not to create a global address */
    else
    {
        bRetBindAddr = TRUE;

        /* construct the IID - this is an example of and IID based on a EUI-64 format that uses a
           vendor specific prefix and a random value appended at the end */

        /* Generate random data */
        RNG_GetRandomNo(&iidPrefix);
        RNG_GetRandomNo(&iidSuffix);

        /* add vendor specific information */
        iidPrefix &= VENDOR_SPECIFIC_IID_PREFIX_MASK;
        iidPrefix |= VENDOR_SPECIFIC_IID_PREFIX_VALUE;

        /* append prefix and suffix into IID */
        iid = (((uint64_t)iidPrefix) << 32U) | iidSuffix;

        /* Set the Universal/Local bit to 1 as this is not a IANA globally assigned address */
        iid |= IID_UNIVERSAL_LOCAL_BIT_MASK;

        htonall(&pPrefix->addr8[8], iid);
    }
    

    return bRetBindAddr;
 }

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
bool_t APP_ServiceServerDataCb
(
    serviceSet_t serviceSet,
    bool_t bAddService
)
{
    bool_t retStatus = FALSE;

#if DNS_ENABLED
    const char* dnsServiceName = "dnsserver";
    ipAddr_t dnsServerIpAddr;

    /* check if service name is "DNS" */
    if (FLib_MemCmp(serviceSet.sData, (void*)dnsServiceName, serviceSet.sDataLen))
    {
        FLib_MemCpy(&dnsServerIpAddr, serviceSet.sServer.sServerData, sizeof(ipAddr_t));
        DNS_SetDnsServerIp6Addr(&dnsServerIpAddr);
        retStatus = TRUE;
    }
#endif

#if gEnableOTAClient_d && OTA_USE_NWK_DATA
    const char* otaServiceName = "otaserver";

    /* check if service name is "OTA" */
    if (FLib_MemCmp(serviceSet.sData, (void*)otaServiceName, serviceSet.sDataLen))
    {
        OtaClient_SetServerIp6Addr(ntohas(serviceSet.sServer.sServer16));
        retStatus = TRUE;
    }
#endif

    return retStatus;
}

/*!*************************************************************************************************
\fn     void APP_CriticalExitCb(uint32_t location, uint32_t param)
\brief  If the stack is in a deadlock situation, it calls APP_CriticalExitCb.

\param  [in]  location  Address where the Panic occurred
\param  [in]  param     Parameter with extra debug information
***************************************************************************************************/
void APP_CriticalExitCb
(
    uint32_t location, 
    uint32_t param
)
{
   panic(0,location, param,0);
   ResetMCU();
}

/*!*************************************************************************************************
\fn     void APP_GenerateMLPrefixCb(instanceId_t thrInstID, thrPrefixAttr_t* pMLprefix)
\brief  This callback is called by Thread Stack to generate the MLprefix

\param  [in]   thrInstID  Thread instance ID
\param  [out]  pMLprefix  ML prefix
***************************************************************************************************/
void APP_GenerateMLPrefixCb
(
    instanceId_t thrInstID,
    thrPrefixAttr_t* pMLprefix 
)
{
    pMLprefix->prefixLenBits = 64;
    pMLprefix->prefix.addr8[0] = 0xFD;
    NWKU_GenRand(&pMLprefix->prefix.addr8[1], 7);
}

/*!*************************************************************************************************
\fn     void APP_EnableDHCP6Cb(void)
\brief  This callback can be used by the application to initialize the callbacks for DHCPv6 module.
***************************************************************************************************/
void APP_EnableDHCP6Cb(void)
{

#if !THREAD_ED_CONFIG
#if THR_USE_DHCP6_SERVER
    dhcp6ServerCallbacks_t gThrDhcp6ServerCbs = {0}; 

    gThrDhcp6ServerCbs.pfServerInit = DHCP6_Server_Init;
    gThrDhcp6ServerCbs.pfServerStart = DHCP6_Server_Start;
    gThrDhcp6ServerCbs.pfServerStop = DHCP6_Server_Stop;
    DHCP6_Server_RegisterCallbacks(&gThrDhcp6ServerCbs);
#endif
#endif
#if THR_USE_DHCP6_CLIENT
    dhcp6ClientCallbacks_t gThrDhcp6ClientCbs = {0};

    gThrDhcp6ClientCbs.pfClientInit = DHCP6_Client_Init;
    gThrDhcp6ClientCbs.pfClientStart = DHCP6_Client_Start;
    gThrDhcp6ClientCbs.pfClientStop = DHCP6_Client_Stop;
    gThrDhcp6ClientCbs.pfClientIsAddrSolicitPending = DHCP6_Client_IsAddrSolicitPending;
    gThrDhcp6ClientCbs.pfClientRegisterSetAddr = DHCP6_Client_RegisterSetAddr;
    gThrDhcp6ClientCbs.pfClientSolicitAddr = DHCP6_Client_SolicitAddress;
    DHCP6_Client_RegisterCallbacks(&gThrDhcp6ClientCbs);
    
#endif
}
/*!*************************************************************************************************
\fn     void APP_BeaconFillCb(instanceId_t thrInstID)
\brief  This callback can be used by the application to set the beacon payload.

\param  [in]   thrInstID  Thread instance ID
***************************************************************************************************/
void APP_BeaconFillCb
(
    instanceId_t thrInstID
)
{
    THR_BeaconPayloadSet(thrInstID, NULL, 0);
}
/*==================================================================================================
Private functions
==================================================================================================*/

/*!*************************************************************************************************
\fn     APP_JoinerNwkListAdd
\brief  Add a parent in joiner list.

\param[in]      pJoiningEntry -  thread beacon
\param[in]      thrInstId - thread instance id

\retval         TRUE if the thread beacon was added; otherwise return FALSE
***************************************************************************************************/
static bool_t APP_JoinerNwkListAdd
(
    thrNwkJoiningEntry_t *pJoiningEntry,
    instanceId_t thrInstId
)
{
    bool_t bRetStatus = FALSE;

    if (gNbOfNwkJoiningEntries < THR_MAX_NWK_JOINING_ENTRIES)
    {
        FLib_MemCpy(&gpNwkJoiningList[gNbOfNwkJoiningEntries], pJoiningEntry, sizeof(thrNwkJoiningEntry_t));

        gNbOfNwkJoiningEntries++;

        bRetStatus = TRUE;
    }
    else
    {
        uint8_t idx = 0;

        /* 1. Specific steering data has higher priority */
        if(pJoiningEntry->steeringMatch == gMeshcopSteeringMatchSpecific_c)
        {
            for(idx = 0; idx < gNbOfNwkJoiningEntries; idx++)
            {
                if(gpNwkJoiningList[idx].steeringMatch != gMeshcopSteeringMatchSpecific_c)
                {
                    FLib_MemCpy(&gpNwkJoiningList[idx], pJoiningEntry, sizeof(thrNwkJoiningEntry_t));
                    bRetStatus = TRUE;
                    break;
                }
            }
        }
        else
        {
            bRetStatus = TRUE;
            /* Check if this PanId is in the list */
            for(idx = 0; idx < THR_MAX_NWK_JOINING_ENTRIES; idx++)
            {
                if(pJoiningEntry->panId == gpNwkJoiningList[idx].panId)
                {
                    /* PanId is in the list */
                    bRetStatus = FALSE;
                    break;
                }
            }

            if(bRetStatus)
            {
                thrNwkJoiningPanCounter_t panIdCounter[THR_MAX_NWK_JOINING_ENTRIES];
                uint8_t pos = 0;

                bRetStatus = FALSE;
                FLib_MemSet(&panIdCounter, 0, sizeof(panIdCounter));

                /* Search the list for PanIds and mark their found positions */
                for(idx = 0; idx < THR_MAX_NWK_JOINING_ENTRIES; idx++)
                {
                    for(pos = 0; pos < THR_MAX_NWK_JOINING_ENTRIES; pos++)
                    {
                        if(gpNwkJoiningList[idx].panId == panIdCounter[pos].panId)
                        {
                            /* PanId entry is in the counting table */
                            panIdCounter[pos].positions |= (1 << idx);
                            break;
                        }
                        else if(panIdCounter[idx].panId == 0)
                        {
                            /* Free entry found */
                            panIdCounter[pos].panId = gpNwkJoiningList[idx].panId;
                            panIdCounter[pos].positions |= (1 << idx);
                            break;
                        }
                    }
                }
                /* Search for PanIds that have more than one position in the list */
                for(idx = 0; idx < THR_MAX_NWK_JOINING_ENTRIES; idx++)
                {
                    if(NWKU_GetNumOfBits(&panIdCounter[idx].positions, 1, 1) > 1)
                    {
                        uint32_t position = NWKU_GetFirstBitValue(&panIdCounter[idx].positions, 1, 1);
                        /* If found, get the first position and overwrite it with the new entry */
                        FLib_MemCpy(&gpNwkJoiningList[position], pJoiningEntry, sizeof(thrNwkJoiningEntry_t));
                        bRetStatus = TRUE;
                        break;
                    }
                }
            }
        }
    }

    return bRetStatus;
}

/*!*************************************************************************************************
\fn    APP_GetNwkScanResultEntry
\brief This function search the nwk discovery entry from pScanResults if the channel matches.

\param  [in] channel - the channel to match
\param  [in] pScanResults - the scan results

\retval returns the network discovery entry
***************************************************************************************************/
static thrNwkActiveScanEntry_t* APP_GetNwkScanResultEntry
(
    uint8_t channel,
    thrNwkScanResults_t* pScanResults
)
{
    thrNwkActiveScanEntry_t* pDiscEntry = NULL;

    if(pScanResults->numOfNwkScanEntries)
    {
      uint8_t i;

      for(i = 0; i < pScanResults->numOfNwkScanEntries; i++)
      {
          if(channel == pScanResults->nwkScanList[i].channel)
          {
              pDiscEntry =  &pScanResults->nwkScanList[i];
              break;
          }
      }
    }

    return pDiscEntry;
}

/*!*************************************************************************************************
\fn    APP_NwkSelectUniquePanId
\brief This function select an unique PAN ID to form the network

\param  [in] thrInstId - the thread instance ID
\param  [in] pScanResults - the scan result. This list includes the energy detect list and
                            the list of discovered thread networks.
\param  [in] bestChannel - best channel

\retval Returns the selected best channel
***************************************************************************************************/
static void APP_NwkSelectUniquePanId
(
    instanceId_t thrInstId,
    thrNwkScanResults_t* pScanResults,
    uint8_t bestChannel,
    uint8_t* pPanId /* out */
)
{
    thrNwkActiveScanEntry_t* pDiscEntry = NULL;
    uint16_t panid;
    uint8_t i;

    if(pScanResults->numOfNwkScanEntries)
    {
        while(1)
        {
            NWKU_GenRand(pPanId, 2);
            FLib_MemCpy(&panid,pPanId,2);
            for(i = 0; i < pScanResults->numOfNwkScanEntries; i++)
            {
                pDiscEntry =  &pScanResults->nwkScanList[i];
                if((bestChannel == pDiscEntry->channel) &&
                   (panid == pDiscEntry->panid))
                {
                   break;
                }
            }

            if(pScanResults->numOfNwkScanEntries == i)
            {
                break;
            }
        }
    }
    else
    {
        NWKU_GenRand(pPanId, 2);
    }
}

/*!*************************************************************************************************
\fn    APP_NwkCreateSelectBestChannel
\brief This function select the best channel to form the network

\param  [in] thrInstId - the thread instance ID
\param  [in] pScanResults - the scan result. This list includes the energy detect list and
                            the list of discovered thread networks.

\retval Returns the selected best channel
***************************************************************************************************/
static uint8_t APP_NwkCreateSelectBestChannel
(
    instanceId_t thrInstId,
    thrNwkScanResults_t* pScanResults
)
{
    /* HERE energy and active scans were performed.
     * Select the best channel to form the network   */
    uint8_t i,iCount = 0, detectedEnergy, saveChannelInfo;
    uint16_t numOfRvcdBeacons;
    thrBestChannelInfo_t bestChannelInfo = {0, 0xff, 0};
    thrNwkActiveScanEntry_t* pDiscEntry;

    for(i = 11; i <= 26; i++)
    {
        /* Check the channels that were scanned */
        if((pScanResults->scanInfo.scanChannelsMask >>i) &0x0001)
        {
            numOfRvcdBeacons = 0;
            detectedEnergy = pScanResults->pEnergyDetectList[iCount++];
            saveChannelInfo = FALSE;
            pDiscEntry =  APP_GetNwkScanResultEntry(i, pScanResults);
            if(pDiscEntry)
            {
                numOfRvcdBeacons = pDiscEntry->numOfRcvdBeacons;
            }

            if(bestChannelInfo.bestchannel == 0)
            {
                saveChannelInfo = TRUE;
            }
            else
            {
                /* Use a filter for detected energy level */
                if (detectedEnergy < THR_GetAttr_SelBestChEDThreshold(thrInstId))
                {
                    /* Select channel with less beacons */
                    if(bestChannelInfo.numOfRvcdBeacons > numOfRvcdBeacons)
                    {
                        saveChannelInfo = TRUE;
                    }
                    else if(bestChannelInfo.numOfRvcdBeacons == numOfRvcdBeacons)
                    {
                       /* Select best energy level  */
                       if(bestChannelInfo.energylevel > detectedEnergy)
                           saveChannelInfo = TRUE;
                    }
                }
            }

            if(saveChannelInfo == TRUE)
            {
                bestChannelInfo.bestchannel = i;
                bestChannelInfo.energylevel = detectedEnergy;
                bestChannelInfo.numOfRvcdBeacons = numOfRvcdBeacons;
            }
        }
    }

    return bestChannelInfo.bestchannel;
}

/*!*************************************************************************************************
\fn     THR_GenerateLeaderPskc
\brief  This is a sample code for computing the PSKc from the device's PSKd.

\param  [in] thrInstId  thread instance ID

\return none
***************************************************************************************************/
static void THR_GenerateLeaderPskc
(
    instanceId_t thrInstId
)
{
   (void)THR_SetPskcPassphrase(thrInstId, gpaThrStringAttr[thrInstId]->pskD.aStr,
            gpaThrStringAttr[thrInstId]->pskD.length);
}

/*==================================================================================================
Private debug functions
==================================================================================================*/

#endif /* STACK_THREAD */
