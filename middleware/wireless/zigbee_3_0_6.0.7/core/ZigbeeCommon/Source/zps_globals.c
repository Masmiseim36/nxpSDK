/*
* The Clear BSD License
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
     
/*!=============================================================================
\file       zps_globals.c
\brief      Zigbee Protocol Stack Globals
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <app_pdum_cfg.h>
#include "ZQueue.h"
#include "app_zps_cfg.h"
//#include "app_zcl_cfg.h"

#include "zps_apl.h"
#include "zps_apl_aib.h"
#include "zps_apl_af.h"

#include "FunctionLib.h"



/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define COMPILE_TIME_ASSERT(pred)    switch(0){case 0:case pred:;}

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/*** ZDP Context **************************************************/

typedef struct {
    uint8 u8ZdpSeqNum;
} zps_tsZdpContext;

/*** ZDO Context **************************************************/
#define ZPS_APL_ZDO_VSOUI_LENGTH		3
typedef bool (*zps_tprAplZdoServer)(void *pvApl, void *pvServerContext, ZPS_tsAfEvent *psZdoServerEvent);

typedef struct {
    zps_tprAplZdoServer prServer;
    void *pvServerContext;
} zps_tsAplZdoServer;

typedef struct
{
    uint8               au8Key[ZPS_NWK_KEY_LENGTH];
    uint8               u8KeySeqNum;
    uint8               u8KeyType;
} zps_tsAplZdoInitSecKey;

typedef struct {
    uint64 u64InitiatorAddr;
    uint64 u64ResponderAddr;
    ZPS_tsTsvTimer sTimer;
    uint8 au8Key[ZPS_NWK_KEY_LENGTH];
} zps_tsRequestKeyRequests;

typedef struct {
#pragma pack(push, 16)
    uint8 au8VsOUIBytes[ZPS_APL_ZDO_VSOUI_LENGTH];
#pragma pack(pop)
    uint8 eNetworkState; /* ZPS_teZdoNetworkState */
    uint8 eZdoDeviceType; /* ZPS_teZdoDeviceType */
    uint8 eNwkKeyState; /* ZPS_teZdoNwkKeyState */
    uint8 u8PermitJoinTime;
    uint8 u8StackProfile;
    uint8 u8ZigbeeVersion;
    uint8 u8ScanDuration;
    bool_t bLookupTCAddr;
    const zps_tsAplZdoServer *psZdoServers;
    void (*prvZdoServersInit)(void);
    ZPS_tsTsvTimer sAuthenticationTimer;
    ZPS_tsTsvTimer sAuthenticationPollTimer;
    uint8 u8NumPollFailures;
    uint8 u8MaxNumPollFailures;
    bool_t bSecurityDisabled;
    zps_tsAplZdoInitSecKey *psInitSecKey;
    uint8 u8DevicePermissions;
    bool_t (*prvZdoReqFilter)(uint16);
    bool (*pfzps_bAplZdoBindRequestServer)(void *,
            void *,
            ZPS_tsAfEvent *);
    zps_tsRequestKeyRequests *psRequestKeyReqs;
    uint32 u32ReqKeyTimeout;
    uint8 u8MaxNumSimulRequestKeyReqs;
} zps_tsZdoContext;

/**** Context for the ZDO servers data confirms and acks***********/

typedef struct {
    uint8 eState;
    uint8 u8SeqNum;
    uint8 u8ConfAck;
} zps_tsZdoServerConfAckContext;

/*** Trust Center Context *****************************************/

typedef struct
{
    uint16 u16AddrLkup;
    ZPS_teDevicePermissions eDevPermissions;
} zps_tsAplTCDeviceTable;

typedef struct
{
    zps_tsAplTCDeviceTable *asTCDeviceTable;
    uint16  u16SizeOfTCDeviceTable;
} zps_tsAplTCib;


typedef struct
{
    void (*prvTrustCenterInit)(void*);
    void (*prvTrustCenterUpdateDevice)(void*, uint64, uint64, uint8, uint16);
    void (*prvTrustCenterRequestKey)(void*, uint64, uint8, uint64);
    zps_tsAplTCib sTCib;
    bool_t bTcOverride;
    bool_t bChangeOverride;
} zps_tsTrustCenterContext;

/*** AF Context ***************************************************/

typedef struct zps_tsAplAfDynamicContext zps_tsAplAfDynamicContext;

typedef struct _zps_tsAplAfSimpleDescCont
{
    ZPS_tsAplAfSimpleDescriptor sSimpleDesc;
    const PDUM_thAPdu *phAPduInClusters;
    bool_t bEnabled;
} zps_tsAplAfSimpleDescCont;

typedef struct {
    zps_tsAplAfDynamicContext *psDynamicContext;
    ZPS_tsAplAfNodeDescriptor *psNodeDescriptor;
    ZPS_tsAplAfNodePowerDescriptor *psNodePowerDescriptor;
    uint32 u32NumSimpleDescriptors;
    zps_tsAplAfSimpleDescCont *psSimpleDescConts;
    ZPS_tsAplAfUserDescriptor *psUserDescriptor;
    void* hOverrunMsg;   
    uint8 zcp_u8FragApsAckValue;
    uint8 zcp_u8FragBlockControl;
} zps_tsAfContext;

/*** APS Context **************************************************/

typedef struct
{
    uint8 u8Type;
    uint8 u8ParamLength;
} ZPS_tsAplApsmeDcfmIndHdr;

typedef struct
{
    uint8 u8Type;
    uint8 u8ParamLength;
} ZPS_tsAplApsdeDcfmIndHdr;

typedef struct {
    ZPS_tuAddress uDstAddr;
    PDUM_thAPduInstance hAPduInst;
    uint8 *pu8SeqCounter;
    uint16 u16ProfileId;
    uint16 u16ClusterId;
    uint8 u8DstEndpoint;
    uint8 u8SrcEndpoint;
    uint8 u8Radius;
    uint8 eDstAddrMode;
    uint8 eTxOptions;
} ZPS_tsAplApsdeReqData;

typedef union
{
    ZPS_tsAplApsdeReqData  sReqData;
} ZPS_tuAplApsdeReqRspParam;

typedef struct
{
    uint8                 u8Type;
    uint8                 u8ParamLength;
    uint16                u16Pad;
    ZPS_tuAplApsdeReqRspParam uParam;
} ZPS_tsAplApsdeReqRsp;

typedef struct
{
    struct {
        uint32 u6Reserved       : 6;
        uint32 u2Fragmentation  : 2;
        uint32 u24Padding       : 24;
    } sEFC;
    uint8 u8BlockNum;
    uint8 u8Ack;
} zps_tsExtendedFrameControlField;

typedef union {
    struct {
        uint8   u8DstEndpoint;
        uint16  u16ClusterId;
        uint16  u16ProfileId;
        uint8   u8SrcEndpoint;
        uint8   u8ApsCounter;
    } sUnicast;

    struct {
            uint16  u16GroupAddr;
            uint16  u16ClusterId;
            uint16  u16ProfileId;
            uint8   u8SrcEndpoint;
            uint8   u8ApsCounter;
        } sGroup;
} zps_tuApsAddressingField;

typedef struct {
    uint16    *psDuplicateTableSrcAddr;
    uint32    *psDuplicateTableHash;
    uint8     *psDuplicateTableApsCnt;
    uint8     u8TableIndex;
} zps_tsApsDuplicateTable;

typedef struct zps_tsMsgRecord_tag {
    struct zps_tsMsgRecord_tag *psNext;
    ZPS_tsAplApsdeReqRsp sApsdeReqRsp;
    ZPS_tsTsvTimer sAckTimer;       /* ack timer */
    uint8       u8ReTryCnt;
    uint8       u8ApsCount;
} zps_tsMsgRecord;

typedef struct zps_tsDcfmRecord_tag{
    union {
        uint16 u16DstAddr;
        uint64 u64DstAddr;
    };
    uint8   u8Handle;
    uint8   u8SrcEp;
    uint8   u8DstEp;
    uint8   u8DstAddrMode;
    uint8   u8SeqNum;
} zps_tsDcfmRecord;

typedef struct zps_tsDcfmRecordPool_tag{
    zps_tsDcfmRecord *psDcfmRecords;
    uint8 u8NextHandle;
    uint8 u8NumRecords;
} zps_tsDcfmRecordPool;

typedef struct zps_tsFragmentTransmit_tag {
    enum {
        ZPS_FRAG_TX_STATE_IDLE,
        ZPS_FRAG_TX_STATE_SENDING,
        ZPS_FRAG_TX_STATE_RESENDING,
        ZPS_FRAG_TX_STATE_WAIT_FOR_ACK
    }eState;
    PDUM_thAPduInstance hAPduInst;
    uint16  u16DstAddress;
    uint16  u16ProfileId;
    uint16  u16ClusterId;
    uint8   u8DstEndpoint;
    uint8   u8SrcEndpoint;
    uint8   u8Radius;
    uint8   u8SeqNum;

    ZPS_tsTsvTimer sAckTimer;
    uint8   u8CurrentBlock;
    uint8   u8SentBlocksInWindow;
    uint8   u8MinBlockNumber;
    uint8   u8MaxBlockNumber;
    uint8   u8TotalBlocksToSend;
    uint8   u8RetryCount;
    uint8   u8AckedBlocksInWindow;
    uint8   u8WindowSize;
    uint8   u8BlockSize;
    bool_t  bSecure;
} zps_tsFragmentTransmit;

typedef struct zps_tsfragTxPool_tag {
    zps_tsFragmentTransmit *psFragTxRecords;
    uint8   u8NumRecords;
} zps_tsFragTxPool;

typedef struct zps_tsFragmentReceive_tag {
    enum {
        ZPS_FRAG_RX_STATE_IDLE,
        ZPS_FRAG_RX_STATE_RECEIVING,
        ZPS_FRAG_RX_STATE_PERSISTING
    }eState;
    PDUM_thAPduInstance hAPduInst;
    uint16  u16SrcAddress;
    uint16  u16ProfileId;
    uint16  u16ClusterId;
    uint8   u8DstEndpoint;
    uint8   u8SrcEndpoint;
    uint8   u8SeqNum;

    ZPS_tsTsvTimer  sWindowTimer;
    PDUM_thNPdu     hNPduPrevious;
    uint16  u16ReceivedBytes;
    uint8   u8TotalBlocksToReceive;
    uint8   u8ReceivedBlocksInWindow;
    uint8   u8MinBlockNumber;
    uint8   u8MaxBlockNumber;
    uint8   u8HighestUnAckedBlock;
    uint8   u8WindowSize;
    uint8   u8BlockSize;
    uint8   u8PreviousBlock;
} zps_tsFragmentReceive;

typedef struct zps_tsfragRxPool_tag {
    zps_tsFragmentReceive *psFragRxRecords;
    uint8   u8NumRecords;
    uint8   u8PersistanceTime;
} zps_tsFragRxPool;

typedef struct zps_tsApsPollTimer {
    ZPS_tsTsvTimer sPollTimer;
    uint16 u16PollInterval;
    uint8 u8PollActive;
} zps_tsApsPollTimer;

typedef struct zps_tsApsmeCmdContainer {
    struct zps_tsApsmeCmdContainer *psNext; /* must be first element of struct */
    ZPS_tsNwkNldeReqRsp sNldeReqRsp;
    ZPS_tsTsvTimer sTimer;
    PDUM_thNPdu hNPduCopy;
    uint8 u8Retries;
} zps_tsApsmeCmdContainer;

typedef struct {
    zps_tsApsmeCmdContainer *psFreeList;
    zps_tsApsmeCmdContainer *psSubmittedList;
} zps_tsApsmeCmdMgr;

typedef struct {
    void* pvParam;
    ZPS_tsAplApsdeDcfmIndHdr *psApsdeDcfmIndHdr;
}zps_tsLoopbackDataContext;

typedef struct {
    /* APSDE */
    void *pvParam;
    ZPS_tsAplApsdeDcfmIndHdr *(*prpsGetApsdeBuf)(void *);
    void (*prvPostApsdeDcfmInd)(void *, ZPS_tsAplApsdeDcfmIndHdr *);
    /* APSME */
    void *pvApsmeParam;
    ZPS_tsAplApsmeDcfmIndHdr *(*prpsGetApsmeBuf)(void *);
    void (*prvPostApsmeDcfmInd)(void *, ZPS_tsAplApsmeDcfmIndHdr *);

    zps_tsApsDuplicateTable *psApsDuplicateTable;
    zps_tsMsgRecord  *psSyncMsgPool;
    uint8 u8ApsDuplicateTableSize;
    uint8 u8SeqNum;
    uint8 u8SyncMsgPoolSize;
    uint8 u8MaxFragBlockSize;
    zps_tsDcfmRecordPool sDcfmRecordPool;
    zps_tsFragRxPool sFragRxPool;
    zps_tsFragTxPool sFragTxPool;
    ZPS_teStatus (*preStartFragmentTransmission)(void *, ZPS_tsAplApsdeReqRsp *, uint16, uint8);
    void (*prvHandleExtendedDataAck)(void *, ZPS_tsNwkNldeDcfmInd *, zps_tuApsAddressingField *, zps_tsExtendedFrameControlField *);
    void (*prvHandleDataFragmentReceive)(void *, ZPS_tsAplApsdeDcfmIndHdr *);
    zps_tsApsmeCmdMgr sApsmeCmdMgr;
    zps_tsApsPollTimer sApsPollTimer;
    zps_tsLoopbackDataContext sLoopbackContext;
    ZPS_tsTsvTimer sLoopbackTimer;
} zps_tsApsContext;

/*** APL Context **************************************************/

typedef struct {
    void *pvNwk;
    const void *pvNwkTableSizes;
    const void *pvNwkTables;
    
    ZPS_tsNwkNib *psNib;
    ZPS_tsAplAib *psAib;
    
    void* hZpsMutex;
    void* hDefaultStackEventMsg;
    void* hMcpsDcfmIndMsg;
    void* hMlmeDcfmIndMsg;
    void* hTimeEventMsg;
    
    /* sub-layer contexts */
    zps_tsZdpContext sZdpContext;
    zps_tsZdoContext sZdoContext;
    zps_tsAfContext  sAfContext;
    zps_tsApsContext sApsContext;

    /* trust center context if present */
    zps_tsTrustCenterContext *psTrustCenterContext;

} zps_tsApl;

/*** NIB Defaults **************************************************/

typedef struct
{
    uint32 u32VsOldRouteExpiryTime;
    uint8  u8MaxRouters;
    uint8  u8MaxChildren;
    uint8  u8MaxDepth;
    uint8  u8PassiveAckTimeout;
    uint8  u8MaxBroadcastRetries;
    uint8  u8MaxSourceRoute;
    uint8  u8NetworkBroadcastDeliveryTime;
    uint8  u8UniqueAddr;
    uint8  u8AddrAlloc;
    uint8  u8UseTreeRouting;
    uint8  u8SymLink;
    uint8  u8UseMulticast;
    uint8  u8LinkStatusPeriod;
    uint8  u8RouterAgeLimit;
    uint8  u8RouteDiscoveryRetriesPermitted;
    uint8  u8VsFormEdThreshold;
    uint8  u8SecurityLevel;
    uint8  u8AllFresh;
    uint8  u8SecureAllFrames;
    uint8  u8VsTxFailThreshold;
    uint8  u8VsMaxOutgoingCost;
    uint8  u8VsLeaveRejoin;
    uint8  u8ZedTimeout;
    uint8  u8ZedTimeoutDefault;
} zps_tsNwkNibInitialValues;

/****************************************************************************/
/***        External Dependencies                                         ***/
/****************************************************************************/

extern zps_tsAplAfSimpleDescCont s_asSimpleDescConts[];
/* Node Descriptor */
extern ZPS_tsAplAfNodeDescriptor s_sNodeDescriptor;
/* Node Power Descriptor */
extern ZPS_tsAplAfNodePowerDescriptor s_sNodePowerDescriptor;
#if AF_ENABLE_USER_DESCRIPTOR
/* User Descriptor */
extern ZPS_tsAplAfUserDescriptor s_sUserDescriptor;
#endif

PUBLIC ZPS_teStatus zps_eStartFragmentedTransmission(void *, ZPS_tsAplApsdeReqRsp *, uint16 , uint8);
PUBLIC void zps_vHandleExtendedDataAckFrame(void *, ZPS_tsNwkNldeDcfmInd *, zps_tuApsAddressingField *, zps_tsExtendedFrameControlField * );
PUBLIC void zps_vHandleApsdeDataFragIndNotSupported(void *pvApl, ZPS_tsAplApsdeDcfmIndHdr *);
PUBLIC void zps_vHandleApsdeDataFragInd(void *, ZPS_tsAplApsdeDcfmIndHdr *psApsdeDcfmInd);
bool_t g_pbZpsMutex = FALSE;
PUBLIC void* zps_vGetZpsMutex(void);
PUBLIC uint8 u8ZpsConfigStackProfileId = 2;
PUBLIC const uint32 g_u32ApsFcSaveCountBitShift = APS_FC_SAVE_COUNT_BIT_SHIFT;
PUBLIC const uint32 g_u32NwkFcSaveCountBitShift = NWK_FC_SAVE_COUNT_BIT_SHIFT;

/* Green Power */
#ifdef ZPS_GREENPOWER
PRIVATE ZPS_tsAfZgpTxGpQueueEntry aZgpTxGpQueue[5];
ZPS_tsTsvTimer sTxAgingTimer;
TSV_Timer_s sTxBiDirTimer;
ZPS_tsAfZgpTxGpQueue sZgpTxGpQueue = { aZgpTxGpQueue, 5 };
PRIVATE ZPS_tsAfZgpGpstEntry aZgpGpst[5];
ZPS_tsAfZgpGpst sZgpGpst = { aZgpGpst, 5 };
ZPS_tsAfZgpGreenPowerContext gsGreenPowerContext = { &sZgpGpst, &sZgpTxGpQueue, &sTxAgingTimer, &sTxBiDirTimer, 100, 0 };
ZPS_tsAfZgpGreenPowerContext *g_psGreenPowerContext = &gsGreenPowerContext;
#else
ZPS_tsAfZgpGreenPowerContext *g_psGreenPowerContext = NULL;
#endif
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void vZdoServersInit(void);

#if ZPS_ZDO_ENABLE_ZDO_DEFAULT_SERVER
PUBLIC bool zps_bAplZdoDefaultServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoDefaultServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_ZDO_CLIENT
PUBLIC bool zps_bAplZdoZdoClient(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoZdoClientInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_DEVICE_ANNCE_SERVER
PUBLIC bool zps_bAplZdoDeviceAnnceServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoDeviceAnnceServerInit(void *);
#endif

#if ZPS_ZDO_ENABLE_ZDO_ACTIVE_EP_SERVER
PUBLIC bool zps_bAplZdoActiveEpServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoActiveEpServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_NWK_ADDR_SERVER
PUBLIC bool zps_bAplZdoNwkAddrServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoNwkAddrServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_IEEE_ADDR_SERVER
PUBLIC bool zps_bAplZdoIeeeAddrServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoIeeeAddrServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_SYSTEM_SERVER_DISCOVERY_SERVER
PUBLIC bool zps_bAplZdoSystemServerDiscoveryServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoSystemServerDiscoveryServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_NODE_DESC_SERVER
PUBLIC bool zps_bAplZdoNodeDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoNodeDescServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_POWER_DESC_SERVER
PUBLIC bool zps_bAplZdoPowerDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoPowerDescServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_MATCH_DESC_SERVER
PUBLIC bool zps_bAplZdoMatchDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMatchDescServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_SIMPLE_DESC_SERVER
PUBLIC bool zps_bAplZdoSimpleDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoSimpleDescServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_MGMT_LQI_SERVER
PUBLIC bool zps_bAplZdoMgmtLqiServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtLqiServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_MGMT_LEAVE_SERVER
PUBLIC bool zps_bAplZdoMgmtLeaveServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtLeaveServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_MGMT_NWK_UPDATE_SERVER
PUBLIC bool zps_bAplZdoMgmtNWKUpdateServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtNWKUpdateServerInit(void *, PDUM_thAPdu , void *);
#endif

#if ZPS_ZDO_ENABLE_ZDO_BIND_UNBIND_SERVER
PUBLIC bool zps_bAplZdoBindUnbindServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoBindUnbindServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_ACTIVE_EP_SERVER
PUBLIC bool zps_bAplZdoExtendedActiveEpServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoExtendedActiveEpServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_SIMPLE_DESC_SERVER
PUBLIC bool zps_bAplZdoExtendedSimpleDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoExtendedSimpleDescServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_BIND_REQUEST_SERVER
PUBLIC bool zps_bAplZdoBindRequestServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoBindRequestServerInit(void *, uint8, uint8, zps_tsZdoServerConfAckContext* );
#endif

#if ZPS_ZDO_ENABLE_ZDO_MGMT_BIND_SERVER
PUBLIC bool zps_bAplZdoMgmtBindServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtBindServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_PERMIT_JOINING_SERVER
PUBLIC bool zps_bAplZdoPermitJoiningServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoPermitJoiningServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_MGMT_RTG_SERVER
PUBLIC bool zps_bAplZdoMgmtRtgServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtRtgServerInit(void *, PDUM_thAPdu );
#endif

#if ZPS_ZDO_ENABLE_ZDO_PARENT_ANNCE_SERVER
PUBLIC bool zps_bAplZdoParentAnnceServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoParentAnnceServerInit(void *, PDUM_thAPdu );
#endif
#if ZPS_ZDO_ENABLE_ZDO_END_DEVICE_BIND_SERVER
PUBLIC bool zps_bAplZdoEndDeviceBindServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoEndDeviceBindServerInit(void *, PDUM_thAPdu , uint32 , uint8 );
#endif

#ifdef ZPS_COORDINATOR
/* Trust Center */
PUBLIC void zps_vAplTrustCenterInit(void *);PUBLIC void zps_vAplTrustCenterUpdateDevice(void *, uint64 , uint64 , uint8 , uint16 );PUBLIC void zps_vAplTrustCenterRequestKey(void *, uint64 , uint8 , uint64 );
#endif
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/*! Binding */
PRIVATE ZPS_tsAplApsmeBindingTableStoreEntry s_bindingTableStorage[ZPS_BINDING_TABLE_SIZE];
PRIVATE ZPS_tsAplApsmeBindingTable s_bindingTable = { s_bindingTableStorage, ZPS_BINDING_TABLE_SIZE };
PRIVATE ZPS_tsAplApsmeBindingTableType s_bindingTables = { NULL, &s_bindingTable };

/*! Group */
#if ZPS_GROUP_TABLE_SIZE
PRIVATE ZPS_tsAplApsmeGroupTableEntry s_groupTableStorage[ZPS_GROUP_TABLE_SIZE];
PRIVATE ZPS_tsAplApsmeAIBGroupTable s_groupTable = { s_groupTableStorage, ZPS_GROUP_TABLE_SIZE };
PRIVATE ZPS_tsAPdmGroupTableEntry s_groupTablePdmStorage[ZPS_GROUP_TABLE_SIZE];
PUBLIC ZPS_tsPdmGroupTable s_groupPdmTable = { s_groupTablePdmStorage, ZPS_GROUP_TABLE_SIZE };
#else
PUBLIC ZPS_tsPdmGroupTable s_groupPdmTable = { NULL, 0 };
#endif

/* ZPS KEY TABLE */
PRIVATE ZPS_tsAplApsKeyDescriptorEntry s_keyPairTableStorage[ZPS_KEY_TABLE_SIZE+3];
#if ZPS_PRECONFIGURED_TRUST_CENTER_LINK_KEY
ZPS_tsAplApsKeyDescriptorEntry  *psAplDefaultTCAPSLinkKey = &s_keyPairTableStorage[1];
#endif
ZPS_tsAplApsKeyDescriptorEntry  *psAplDefaultDistributedAPSLinkKey = &s_keyPairTableStorage[ZPS_KEY_TABLE_SIZE+1];
#if ZPS_INIT_APL_DEFAULT_GLOBAL_APS_LINK_KEY
ZPS_tsAplApsKeyDescriptorEntry  *psAplDefaultGlobalAPSLinkKey = &s_keyPairTableStorage[ZPS_KEY_TABLE_SIZE+2];
#else
ZPS_tsAplApsKeyDescriptorEntry  *psAplDefaultGlobalAPSLinkKey = NULL;
#endif
PRIVATE uint32 au32IncomingFrameCounter[ZPS_KEY_TABLE_SIZE+3];
PRIVATE ZPS_tsAplApsKeyDescriptorTable s_keyPairTable = { s_keyPairTableStorage, ZPS_KEY_TABLE_SIZE};

PRIVATE ZPS_tsAplAib s_sAplAib = {
    ZPS_APS_AIB_INIT_TRUST_CENTER_ADDRESS,//0,
    ZPS_APS_AIB_INIT_USE_EXTENDED_PANID,//0x0000000000000000ULL,
    ZPS_APS_AIB_INIT_CHANNEL_MASK,//0x00000800UL,
    ZPS_APS_AIB_INIT_DESIGNATED_COORDINATOR,//TRUE,
    ZPS_APS_AIB_INIT_USE_INSECURE_JOIN,//TRUE,
    ZPS_APS_AIB_INIT_DECRYPT_INSTALL_CODE,//FALSE,
    ZPS_APS_AIB_INIT_KEY_TYPE,//0,
    /* volatile */
    ZPS_APS_AIB_INIT_NON_MEMBER_RADIUS,//0x02,
    ZPS_APS_AIB_INIT_INTER_FRAME_DELAY,//0x0a,
    ZPS_APS_AIB_INIT_LAST_CHANNEL_ENERGY,//0,
    ZPS_APS_AIB_INIT_LAST_CHANNEL_FAILURE_RATE,//0,
    ZPS_APS_AIB_INIT_CHANNEL_TIMER,//0,
    ZPS_APS_AIB_INIT_MAX_WINDOW_SIZE,//0x08,
    &s_bindingTables,
    #if ZPS_GROUP_TABLE_SIZE
	&s_groupTable,
	#else
	NULL,
	#endif
    &s_keyPairTable,
    &s_keyPairTableStorage[ZPS_KEY_TABLE_SIZE],
    ZPS_APS_AIB_INIT_PARENT_ANNOUNCE_PENDING,//FALSE,
    ZPS_APS_AIB_INIT_USE_INSTALL_CODE,//FALSE,
    ZPS_APS_AIB_INIT_SECURITY_TIMEOUT_PERIOD,//0x1770,
    au32IncomingFrameCounter,
};

#if defined(__ICCARM__)
#if ZPS_ZDO_ENABLE_ZDO_DEFAULT_SERVER
PRIVATE uint8 s_sDefaultServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_ZDO_CLIENT
PRIVATE uint8 s_sZdoClientContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_DEVICE_ANNCE_SERVER
PRIVATE uint8 s_sDeviceAnnceServerContext[ZPS_DEVICE_ANNCE_SERVER_CONTEXT_SIZE];
#endif
#if ZPS_ZDO_ENABLE_ZDO_ACTIVE_EP_SERVER
PRIVATE uint8 s_sActiveEpServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_NWK_ADDR_SERVER
PRIVATE uint8 s_sNwkAddrServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_IEEE_ADDR_SERVER
PRIVATE uint8 s_sIeeeAddrServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_SYSTEM_SERVER_DISCOVERY_SERVER
PRIVATE uint8 s_sSystemServerDiscoveryServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_NODE_DESC_SERVER
PRIVATE uint8 s_sNodeDescServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_POWER_DESC_SERVER
PRIVATE uint8 s_sPowerDescServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_MATCH_DESC_SERVER
PRIVATE uint8 s_sMatchDescServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_SIMPLE_DESC_SERVER
PRIVATE uint8 s_sSimpleDescServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_LQI_SERVER
PRIVATE uint8 s_sMgmtLqiServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_MGMT_LEAVE_SERVER
PRIVATE uint8 s_sMgmtLeaveServerContext[40];
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_NWK_UPDATE_SERVER
PRIVATE uint8 s_sMgmtNWKUpdateServerContext[64];
#endif
#if ZPS_ZDO_ENABLE_ZDO_BIND_UNBIND_SERVER
PRIVATE uint8 s_sBindUnbindServerContext[48];
#endif
#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_ACTIVE_EP_SERVER
PRIVATE uint8 s_sExtendedActiveEpServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_SIMPLE_DESC_SERVER
PRIVATE uint8 s_sExtendedSimpleDescServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_BIND_REQUEST_SERVER
PRIVATE uint8 s_sBindRequestServerContext[56];
PRIVATE zps_tsZdoServerConfAckContext s_sBindRequestServerAcksDcfmContext[ZPS_BIND_REQUEST_SERVER_ACKS_DCFM_CONTEXT_SIZE];
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_BIND_SERVER
PRIVATE uint8 s_sMgmtBindServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_PERMIT_JOINING_SERVER
PRIVATE uint8 s_sPermitJoiningServerContext[8];
#endif
#if  ZPS_ZDO_ENABLE_ZDO_MGMT_RTG_SERVER
PRIVATE uint8 s_sMgmtRtgServerContext[8];
#endif
#if ZPS_ZDO_ENABLE_ZDO_PARENT_ANNCE_SERVER
PRIVATE uint8 s_sParentAnnceServerContext[12];
#endif
#if ZPS_ZDO_ENABLE_ZDO_END_DEVICE_BIND_SERVER
PRIVATE uint8 s_sEndDeviceBindServerContext[64];
#endif

#else

#if ZPS_ZDO_ENABLE_ZDO_DEFAULT_SERVER
PRIVATE uint8 s_sDefaultServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_ZDO_CLIENT
PRIVATE uint8 s_sZdoClientContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_DEVICE_ANNCE_SERVER
PRIVATE uint8 s_sDeviceAnnceServerContext[ZPS_DEVICE_ANNCE_SERVER_CONTEXT_SIZE] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_ACTIVE_EP_SERVER
PRIVATE uint8 s_sActiveEpServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_NWK_ADDR_SERVER
PRIVATE uint8 s_sNwkAddrServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_IEEE_ADDR_SERVER
PRIVATE uint8 s_sIeeeAddrServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_SYSTEM_SERVER_DISCOVERY_SERVER
PRIVATE uint8 s_sSystemServerDiscoveryServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_NODE_DESC_SERVER
PRIVATE uint8 s_sNodeDescServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_POWER_DESC_SERVER
PRIVATE uint8 s_sPowerDescServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_MATCH_DESC_SERVER
PRIVATE uint8 s_sMatchDescServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_SIMPLE_DESC_SERVER
PRIVATE uint8 s_sSimpleDescServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_LQI_SERVER
PRIVATE uint8 s_sMgmtLqiServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_MGMT_LEAVE_SERVER
PRIVATE uint8 s_sMgmtLeaveServerContext[40] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_NWK_UPDATE_SERVER
PRIVATE uint8 s_sMgmtNWKUpdateServerContext[64] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_BIND_UNBIND_SERVER
PRIVATE uint8 s_sBindUnbindServerContext[48] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_ACTIVE_EP_SERVER
PRIVATE uint8 s_sExtendedActiveEpServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_SIMPLE_DESC_SERVER
PRIVATE uint8 s_sExtendedSimpleDescServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_BIND_REQUEST_SERVER
PRIVATE uint8 s_sBindRequestServerContext[56] __attribute__ ((aligned (4)));
PRIVATE zps_tsZdoServerConfAckContext s_sBindRequestServerAcksDcfmContext[ZPS_BIND_REQUEST_SERVER_ACKS_DCFM_CONTEXT_SIZE];
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_BIND_SERVER
PRIVATE uint8 s_sMgmtBindServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_PERMIT_JOINING_SERVER
PRIVATE uint8 s_sPermitJoiningServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_RTG_SERVER
PRIVATE uint8 s_sMgmtRtgServerContext[8] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_PARENT_ANNCE_SERVER
PRIVATE uint8 s_sParentAnnceServerContext[12] __attribute__ ((aligned (4)));
#endif
#if ZPS_ZDO_ENABLE_ZDO_END_DEVICE_BIND_SERVER
PRIVATE uint8 s_sEndDeviceBindServerContext[64] __attribute__ ((aligned (4)));
#endif
#endif
/* ZDO Servers */
PRIVATE const zps_tsAplZdoServer s_asAplZdoServers[] = {
#if ZPS_ZDO_ENABLE_ZDO_ZDO_CLIENT
    { zps_bAplZdoZdoClient, s_sZdoClientContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_DEVICE_ANNCE_SERVER	
    { zps_bAplZdoDeviceAnnceServer, s_sDeviceAnnceServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_ACTIVE_EP_SERVER	
    { zps_bAplZdoActiveEpServer, s_sActiveEpServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_NWK_ADDR_SERVER	
    { zps_bAplZdoNwkAddrServer, s_sNwkAddrServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_IEEE_ADDR_SERVER	
    { zps_bAplZdoIeeeAddrServer, s_sIeeeAddrServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_SYSTEM_SERVER_DISCOVERY_SERVER	
    { zps_bAplZdoSystemServerDiscoveryServer, s_sSystemServerDiscoveryServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_NODE_DESC_SERVER	
    { zps_bAplZdoNodeDescServer, s_sNodeDescServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_POWER_DESC_SERVER	
    { zps_bAplZdoPowerDescServer, s_sPowerDescServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_MATCH_DESC_SERVER	
    { zps_bAplZdoMatchDescServer, s_sMatchDescServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_SIMPLE_DESC_SERVER	
    { zps_bAplZdoSimpleDescServer, s_sSimpleDescServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_LQI_SERVER	
    { zps_bAplZdoMgmtLqiServer, s_sMgmtLqiServerContext },
#endif
#if ZPS_ZDO_ENABLE_MGMT_LEAVE_SERVER
    { zps_bAplZdoMgmtLeaveServer, s_sMgmtLeaveServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_NWK_UPDATE_SERVER
    { zps_bAplZdoMgmtNWKUpdateServer, s_sMgmtNWKUpdateServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_BIND_UNBIND_SERVER	
    { zps_bAplZdoBindUnbindServer, s_sBindUnbindServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_ACTIVE_EP_SERVER	
    { zps_bAplZdoExtendedActiveEpServer, s_sExtendedActiveEpServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_SIMPLE_DESC_SERVER	
    { zps_bAplZdoExtendedSimpleDescServer, s_sExtendedSimpleDescServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_BIND_REQUEST_SERVER	
    { zps_bAplZdoBindRequestServer, s_sBindRequestServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_BIND_SERVER	
    { zps_bAplZdoMgmtBindServer, s_sMgmtBindServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_PERMIT_JOINING_SERVER	
    { zps_bAplZdoPermitJoiningServer, s_sPermitJoiningServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_RTG_SERVER	
    { zps_bAplZdoMgmtRtgServer, s_sMgmtRtgServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_PARENT_ANNCE_SERVER	
    { zps_bAplZdoParentAnnceServer, s_sParentAnnceServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_END_DEVICE_BIND_SERVER
    { zps_bAplZdoEndDeviceBindServer, s_sEndDeviceBindServerContext },
#endif
#if ZPS_ZDO_ENABLE_ZDO_DEFAULT_SERVER	
    { zps_bAplZdoDefaultServer, s_sDefaultServerContext },
#endif	
    { NULL, NULL }
};

PUBLIC void APP_vGenCallback(uint8 u8Endpoint, ZPS_tsAfEvent *psStackEvent);
tszQueue zps_msgMlmeDcfmInd;
tszQueue zps_msgMcpsDcfmInd;
tszQueue zps_TimeEvents;


/* APSDE duplicate table */
PRIVATE uint16 s_au16ApsDuplicateTableAddrs[APS_DUPLICATE_TABLE_SIZE];
PRIVATE uint32 s_au32ApsDuplicateTableHash[APS_DUPLICATE_TABLE_SIZE];
PRIVATE uint8 s_au8ApsDuplicateTableSeqCnts[APS_DUPLICATE_TABLE_SIZE];
PRIVATE zps_tsApsDuplicateTable s_sApsDuplicateTable = { s_au16ApsDuplicateTableAddrs, s_au32ApsDuplicateTableHash, s_au8ApsDuplicateTableSeqCnts, 0 };

/* APSDE sync msg pool */
PRIVATE zps_tsMsgRecord s_asApsSyncMsgPool[APS_SYNC_MSG_POOL_SIZE];

/* APSDE dcfm record pool */
PRIVATE zps_tsDcfmRecord s_asApsDcfmRecordPool[APS_DCFM_RECORD_POOL_SIZE];

#if ZPS_FRAGMENTATION_MAX_NUM_SIMUL_RX
/* APSDE fragmentation rx pool */
PRIVATE zps_tsFragmentReceive s_asApsFragRxPool[ZPS_FRAGMENTATION_MAX_NUM_SIMUL_RX];
#endif

#if ZPS_FRAGMENTATION_MAX_NUM_SIMUL_TX
/* APSDE fragmentation tx pool */
PRIVATE zps_tsFragmentTransmit s_asApsFragTxPool[ZPS_FRAGMENTATION_MAX_NUM_SIMUL_TX];
#endif
/* APSME Command Manager Command Containers */
//APSME_SIMULTANEOUS_COMMANDS
PRIVATE zps_tsApsmeCmdContainer s_sApsmeCmdContainer_4 = { NULL, {0}, {{0}}, NULL, 0 };
PRIVATE zps_tsApsmeCmdContainer s_sApsmeCmdContainer_3 = { &s_sApsmeCmdContainer_4, {0}, {{0}}, NULL, 0 };
PRIVATE zps_tsApsmeCmdContainer s_sApsmeCmdContainer_2 = { &s_sApsmeCmdContainer_3, {0}, {{0}}, NULL, 0 };
PRIVATE zps_tsApsmeCmdContainer s_sApsmeCmdContainer_1 = { &s_sApsmeCmdContainer_2, {0}, {{0}}, NULL, 0 };

/* Trust Center */
#ifdef ZPS_COORDINATOR	
PRIVATE zps_tsAplTCDeviceTable s_asTrustCenterDeviceTable[ZPS_TRUST_CENTER_DEVICE_TABLE_SIZE];
PRIVATE zps_tsTrustCenterContext s_sTrustCenterContext = {
    zps_vAplTrustCenterInit,
    zps_vAplTrustCenterUpdateDevice,
    zps_vAplTrustCenterRequestKey,
    { s_asTrustCenterDeviceTable, ZPS_TRUST_CENTER_DEVICE_TABLE_SIZE },
 FALSE,
 FALSE,
};
#endif

/* Network Layer Context */
#if defined(__ICCARM__)
PRIVATE uint8                   s_sNwkContext[1900];
#else
PRIVATE uint8                   s_sNwkContext[1900] __attribute__ ((aligned (4)));
#endif
PRIVATE ZPS_tsNwkDiscNtEntry    s_asNwkNtDisc[ZPS_NEIGHBOUR_DISCOVERY_TABLE_SIZE];
PRIVATE ZPS_tsNwkActvNtEntry    s_asNwkNtActv[ZPS_NEIGHBOUR_TABLE_SIZE];
PRIVATE ZPS_tsNwkRtDiscEntry    s_asNwkRtDisc[ZPS_ROUTE_DISCOVERY_TABLE_SIZE];
PRIVATE ZPS_tsNwkRtEntry        s_asNwkRt[ZPS_ROUTING_TABLE_SIZE];
PRIVATE ZPS_tsNwkBtr            s_asNwkBtt[ZPS_BROADCAST_TRANSACTION_TABLE_SIZE];
PRIVATE ZPS_tsNwkRctEntry       s_asNwkRct[ZPS_ROUTE_RECORD_TABLE_SIZE];
PRIVATE ZPS_tsNwkSecMaterialSet s_asNwkSecMatSet[ZPS_SECURITY_MATERIAL_SETS];
PRIVATE uint32                  s_asNwkInFCSet[ZPS_NEIGHBOUR_TABLE_SIZE];
PRIVATE uint16                  s_au16NwkAddrMapNwk[ZPS_ADDRESS_MAP_TABLE_SIZE+4];
PRIVATE uint16                  s_au16NwkAddrMapLookup[ZPS_ADDRESS_MAP_TABLE_SIZE+4];
PRIVATE uint64                  s_au64NwkAddrMapExt[ZPS_MAC_ADDRESS_TABLE_SIZE + 4];
PRIVATE uint32                  s_au32ZedTimeoutCount[ZPS_CHILD_TABLE_SIZE];
PRIVATE uint8                   s_au8KeepAliveFlags[ZPS_CHILD_TABLE_SIZE];

PRIVATE const zps_tsNwkNibInitialValues s_sNibInitialValues =
{
    ZPS_NWK_NIB_INIT_VSOLD_ROUTE_EXPIRY_TIME,//600,
    ZPS_NWK_NIB_INIT_MAX_ROUTERS,//05,
    ZPS_NWK_NIB_INIT_MAX_CHILDREN,//7,
    ZPS_NWK_NIB_INIT_MAX_DEPTH,//15,
    ZPS_NWK_NIB_INIT_PASSIVE_ACK_TIMEOUT, //1,
    ZPS_NWK_NIB_INIT_MAX_BROADCAST_RETRIES,//2,
    ZPS_NWK_NIB_INIT_MAX_SOURCE_ROUTE,//12,
    ZPS_NWK_NIB_INIT_NETWORK_BROADCAST_DELIVERY_TIME,//18,
    ZPS_NWK_NIB_INIT_UNIQUE_ADDR,//0,
    ZPS_NWK_NIB_INIT_ADDR_ALLOC,//2,
    ZPS_NWK_NIB_INIT_USE_TREE_ROUTING,//0,
    ZPS_NWK_NIB_INIT_SYMLINK,//1,
    ZPS_NWK_NIB_INIT_USE_MULTICAST,//0,
    ZPS_NWK_NIB_INIT_LINK_STATUS_PERIOD,//15,
    ZPS_NWK_NIB_INIT_ROUTER_AGE_LIMIT,//3,
    ZPS_NWK_NIB_INIT_ROUTE_DISCOVERY_RETRIES_PERMITTED,//3,
    ZPS_NWK_NIB_INIT_VS_FORMED_THRESHOLD,//255,
    ZPS_NWK_NIB_INIT_SECURITY_LEVEL,// 5,
    ZPS_NWK_NIB_INIT_ALL_FRESH,// TRUE,
    ZPS_NWK_NIB_INIT_SECURE_ALL_FRAMES,//TRUE,
    ZPS_NWK_NIB_INIT_VS_TX_FAIL_THRESHOLD,//5,
    ZPS_NWK_NIB_INIT_VS_MAX_OUTGOING_COST,//4,
    ZPS_NWK_NIB_INIT_VS_LEAVE_REJOIN,//1,
    ZPS_NWK_NIB_INIT_ZED_TIMEOUT,//2,
    ZPS_NWK_NIB_INIT_ZED_TIMEOUT_DEFAULT//8
};


//PRIVATE const uint16 u16ChildTableSize = 6;

PRIVATE const ZPS_tsNwkNibTblSize     s_sNwkTblSize = {
    ZPS_NEIGHBOUR_TABLE_SIZE,
    ZPS_ROUTING_TABLE_SIZE,
    ZPS_ROUTE_RECORD_TABLE_SIZE,
    ZPS_ADDRESS_MAP_TABLE_SIZE,
    ZPS_NEIGHBOUR_DISCOVERY_TABLE_SIZE,
    ZPS_ROUTE_DISCOVERY_TABLE_SIZE,
    ZPS_BROADCAST_TRANSACTION_TABLE_SIZE,
    ZPS_SECURITY_MATERIAL_SETS,
    sizeof(s_sNibInitialValues),
    ZPS_CHILD_TABLE_SIZE,
    ZPS_MAC_ADDRESS_TABLE_SIZE
};

PRIVATE const ZPS_tsNwkNibTbl s_sNwkTbl = {
    s_asNwkNtDisc,
    s_asNwkNtActv,
    s_asNwkRtDisc,
    s_asNwkRt,
    s_asNwkBtt,
    s_asNwkRct,
    s_asNwkSecMatSet,
    (ZPS_tsNwkNibInitialValues*)&s_sNibInitialValues,
    s_au16NwkAddrMapNwk,
    s_au16NwkAddrMapLookup,
    s_asNwkInFCSet,
    0,
    s_au64NwkAddrMapExt,
    s_au32ZedTimeoutCount,
    s_au8KeepAliveFlags
};

/* Application Layer Context */
PRIVATE zps_tsRequestKeyRequests s_asRequestKeyRequests[ZPS_MAX_NUM_SIMULTANEOUS_REQUEST_KEY_REQS];
PRIVATE zps_tsApl s_sApl = {
    s_sNwkContext,
    &s_sNwkTblSize,
    &s_sNwkTbl,
    NULL,
    &s_sAplAib,
    (void*)zps_vGetZpsMutex,
    (void*)&APP_vGenCallback,
    &zps_msgMcpsDcfmInd,
    &zps_msgMlmeDcfmInd,
    &zps_TimeEvents,
    { 0 },
    {
        {0},
        ZPS_ZDO_ST_INACTIVE,
        ZPS_ZDO_DEVICE_TYPE,
        ZPS_ZDO_NWK_KEY_STATE,
        ZPS_DEFAULT_PERMIT_JOINING_TIME,
        ZPS_STACK_PROFILE,
        ZPS_ZIGBEE_VERSION,
        ZPS_SCAN_DURATION,
        FALSE,
        s_asAplZdoServers,
        vZdoServersInit,
        {{0}, {0} /* timer struct */},
        {{0}, {0} /* timer struct */},
        0,
        3,
        0,
        NULL,
        0,
        NULL,
        NULL,
    s_asRequestKeyRequests,
    937500,
    ZPS_MAX_NUM_SIMULTANEOUS_REQUEST_KEY_REQS
    },
    {
        NULL,
        &s_sNodeDescriptor,
        &s_sNodePowerDescriptor,
        AF_SIMPLE_DESCRIPTOR_TABLE_SIZE,
        s_asSimpleDescConts,
		#if AF_ENABLE_USER_DESCRIPTOR
        &s_sUserDescriptor,
		#else
		NULL,
		#endif
        NULL,
        0xff,
        0x00
    },
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &s_sApsDuplicateTable,
        s_asApsSyncMsgPool,
        APS_DUPLICATE_TABLE_SIZE,
        0,
        APS_SYNC_MSG_POOL_SIZE,
        0,
        { s_asApsDcfmRecordPool, 1, APS_DCFM_RECORD_POOL_SIZE },
		#if ZPS_FRAGMENTATION_MAX_NUM_SIMUL_RX
        { s_asApsFragRxPool, ZPS_FRAGMENTATION_MAX_NUM_SIMUL_RX, 100 },
		#else
		{ NULL, 0, 0 },
		#endif
		#if ZPS_FRAGMENTATION_MAX_NUM_SIMUL_TX
        { s_asApsFragTxPool, ZPS_FRAGMENTATION_MAX_NUM_SIMUL_TX },
		#else
		{ NULL, 0 },
		#endif
		#if ZPS_FRAGMENTATION_MAX_NUM_SIMUL_TX
        zps_eStartFragmentedTransmission,
        zps_vHandleExtendedDataAckFrame,
		#else
		NULL,
        NULL,
		#endif
		#if ZPS_FRAGMENTATION_MAX_NUM_SIMUL_RX
        zps_vHandleApsdeDataFragInd,
		#else
		zps_vHandleApsdeDataFragIndNotSupported,
		#endif
        { &s_sApsmeCmdContainer_1, NULL },
        { {{0}, {0} /* Timer */}, APS_POLL_INTERVAL, 0 },
        { NULL, NULL },
        {{0}, {0}/* Timer */}
    },
#ifdef ZPS_COORDINATOR		
    &s_sTrustCenterContext
#else
    NULL	
#endif	
};

const void *zps_g_pvApl = &s_sApl;
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/* ZPS Globals Initialisation */
PUBLIC void vZpsGlobalsInit(void)
{
     /* ZPS globals init */
    uint8_t i = 0;
    FLib_MemSet(&s_keyPairTableStorage[0], 0, sizeof(ZPS_tsAplApsKeyDescriptorEntry) * (ZPS_KEY_TABLE_SIZE + 3));
    
    for(i=0; i < (ZPS_KEY_TABLE_SIZE + 3); i++)
    {
        s_keyPairTableStorage[i].u16ExtAddrLkup = 0xFFFF;
    }
    
    #ifdef ZPS_COORDINATOR	
    FLib_MemSet(&s_asTrustCenterDeviceTable[0], 0, sizeof(zps_tsAplTCDeviceTable) * ZPS_TRUST_CENTER_DEVICE_TABLE_SIZE);
    
    for(i=0; i < ZPS_TRUST_CENTER_DEVICE_TABLE_SIZE; i++)
    {
        s_asTrustCenterDeviceTable[i].u16AddrLkup = 0xFFFF;
    }
    #endif
}


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


/* ZDO Server Initialisation */
PRIVATE void vZdoServersInit(void)
{
    /* Version compatibility check */
#if defined(__ICCARM__)
    extern int ZPS_APL_Version_3v0;
    extern int ZPS_NWK_Version_3v0;    
#else 
    asm(".extern ZPS_APL_Version_3v0" : ) ;
    asm("LDR R0, =ZPS_APL_Version_3v0" : );
    asm(".extern ZPS_NWK_Version_3v0" : ) ;
    asm("LDR R0, =ZPS_NWK_Version_3v0" : );
#endif
            
#if ZPS_ZDO_ENABLE_ZDO_DEFAULT_SERVER
    zps_vAplZdoDefaultServerInit(&s_sDefaultServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_ZDO_CLIENT	
    zps_vAplZdoZdoClientInit(&s_sZdoClientContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_DEVICE_ANNCE_SERVER	
    zps_vAplZdoDeviceAnnceServerInit(&s_sDeviceAnnceServerContext);
#endif
#if ZPS_ZDO_ENABLE_ZDO_ACTIVE_EP_SERVER	
    zps_vAplZdoActiveEpServerInit(&s_sActiveEpServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_NWK_ADDR_SERVER	
    zps_vAplZdoNwkAddrServerInit(&s_sNwkAddrServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_IEEE_ADDR_SERVER	
    zps_vAplZdoIeeeAddrServerInit(&s_sIeeeAddrServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_SYSTEM_SERVER_DISCOVERY_SERVER	
    zps_vAplZdoSystemServerDiscoveryServerInit(&s_sSystemServerDiscoveryServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_NODE_DESC_SERVER	
    zps_vAplZdoNodeDescServerInit(&s_sNodeDescServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_POWER_DESC_SERVER	
    zps_vAplZdoPowerDescServerInit(&s_sPowerDescServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_MATCH_DESC_SERVER	
    zps_vAplZdoMatchDescServerInit(&s_sMatchDescServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_SIMPLE_DESC_SERVER	
    zps_vAplZdoSimpleDescServerInit(&s_sSimpleDescServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_LQI_SERVER	
    zps_vAplZdoMgmtLqiServerInit(&s_sMgmtLqiServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_MGMT_LEAVE_SERVER
    zps_vAplZdoMgmtLeaveServerInit(&s_sMgmtLeaveServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_NWK_UPDATE_SERVER
    zps_vAplZdoMgmtNWKUpdateServerInit(&s_sMgmtNWKUpdateServerContext, apduZDP, &s_sApl);
#endif
#if ZPS_ZDO_ENABLE_ZDO_BIND_UNBIND_SERVER	
    zps_vAplZdoBindUnbindServerInit(&s_sBindUnbindServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_ACTIVE_EP_SERVER	
    zps_vAplZdoExtendedActiveEpServerInit(&s_sExtendedActiveEpServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_EXTENDED_SIMPLE_DESC_SERVER	
    zps_vAplZdoExtendedSimpleDescServerInit(&s_sExtendedSimpleDescServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_BIND_REQUEST_SERVER	
    zps_vAplZdoBindRequestServerInit(&s_sBindRequestServerContext, ZPS_BIND_REQUEST_SERVER_TIME_INTERVAL, ZPS_BIND_REQUEST_SERVER_ACKS_DCFM_CONTEXT_SIZE, s_sBindRequestServerAcksDcfmContext);
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_BIND_SERVER	
    zps_vAplZdoMgmtBindServerInit(&s_sMgmtBindServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_PERMIT_JOINING_SERVER
    zps_vAplZdoPermitJoiningServerInit(&s_sPermitJoiningServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_MGMT_RTG_SERVER	
    zps_vAplZdoMgmtRtgServerInit(&s_sMgmtRtgServerContext, apduZDP);
#endif
#if ZPS_ZDO_ENABLE_ZDO_PARENT_ANNCE_SERVER	
    zps_vAplZdoParentAnnceServerInit(&s_sParentAnnceServerContext, apduZDP);
#endif

#if ZPS_ZDO_ENABLE_ZDO_END_DEVICE_BIND_SERVER
    zps_vAplZdoEndDeviceBindServerInit(&s_sEndDeviceBindServerContext, apduZDP, 625000, 3);
#endif

#ifndef ZPS_ENDDEVICE
    zps_vRegisterCallbackForSecondsTick(ZPS_vSecondTimerCallback);
#endif    
	
}

PUBLIC void* ZPS_vGetGpContext(void)
{

    return g_psGreenPowerContext;
}

PUBLIC void* zps_vGetZpsMutex(void)
{

    return &g_pbZpsMutex;
}


PUBLIC void ZPS_vGetOptionalFeatures(void)
{
#ifdef ZPS_GREENPOWER
    ZPS_vRegisterGreenPower();
    ZPS_vZgpInitGpTxQueue();
    ZPS_vZgpInitGpSecurityTable();
#endif
#if ZPS_INTER_PAN
    ZPS_vAfInterPanInit();
#endif   
}

/****************************************************************************/
/***        Configuration erros checks                                    ***/
/****************************************************************************/

#if ZPS_NEIGHBOUR_TABLE_SIZE < ZPS_ADDRESS_MAP_TABLE_SIZE
    //#error The ActiveNeighbourTableSize greater then the MacTable setting for the ZigBeeWirelessNetwork
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
