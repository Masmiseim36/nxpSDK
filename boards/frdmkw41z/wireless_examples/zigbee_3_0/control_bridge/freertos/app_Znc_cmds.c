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
\file       ZigbeeNodeControlBridge
\brief      app_Znc_cmds.c
==============================================================================*/

/****************************************************************************/
/***    Include files                                                     ***/
/****************************************************************************/
#include "FunctionLib.h"
#include <string.h>
#include "jendefs.h"
#include "app_zcl_cfg.h"
#include "dbg.h"
#include "PDM.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "RNG_Interface.h"
#if gFsciIncluded_c
#include "Messaging.h"
#include "FsciInterface.h"
#endif
#include "NVM_Interface.h"
#include "SerialLink.h"
#ifdef NETWORK_RECOVERY
#include "app_network_recovery.h"
#endif
#include "zps_apl.h"
#include "zps_apl_zdo.h"
#include "zps_apl_aib.h"
#include "zps_apl_af.h"
#include "app_common.h"
#include "app_Znc_cmds.h"
#include "Log.h"
#include "app_events.h"
#include "zcl_options.h"
#include "PDM_IDs.h"
#include "ApplianceStatistics.h"
#include "app_zps_cfg.h"

#ifdef CLD_OTA
#include "app_ota_server.h"
#endif
/****************************************************************************/
/***    Macro Definitions                         ***/
/****************************************************************************/

#ifndef TRACE_APP
#define TRACE_APP   TRUE
#endif

#ifndef VERSION
#define VERSION    0x12345678
#endif
/****************************************************************************/
/***    Type Definitions                          ***/
/****************************************************************************/

/****************************************************************************/
/***    Local Function Prototypes                     ***/
/****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpSystemServerDiscovery ( uint16    u16ServerMask,
                                                     uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpMgmtNetworkUpdateReq  ( uint16    u16Addr,
                                                     uint32    u32ChannelMask,
                                                     uint8     u8ScanDuration,
                                                     uint8     u8ScanCount,
                                                     uint8*    pu8Seq,
                                                     uint16    u16NwkManagerAddr );

PRIVATE ZPS_teStatus APP_eZdpMgmtLeave ( uint16     u16DstAddr,
                                         uint64     u64DeviceAddr,
                                         bool_t     bRejoin,
                                         bool_t     bRemoveChildren,
                                         uint8*     pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpLeaveReq ( ZPS_tuAddress     uAddress,
                                        bool              u8RemoveChildren,
                                        bool              bRejoin );

PRIVATE ZPS_teStatus APP_eZdpRemoveDeviceReq ( ZPS_tuAddress    uParentAddress,
                                               ZPS_tuAddress    uChildAddress );

PRIVATE ZPS_teStatus APP_eZdpNodeDescReq ( uint16     u16Addr,
                                           uint8*     pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpPowerDescReq ( uint16    u16Addr,
                                            uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpSimpleDescReq ( uint16    u16Addr,
                                             uint8     u8Endpoint,
                                             uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpActiveEndpointReq ( uint16    u16Addr,
                                                 uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpMatchDescReq ( uint16    u16Addr,
                                            uint16    u16profile,
                                            uint8     u8InputCount,
                                            uint16*   pu8InputList,
                                            uint8     u8OutputCount,
                                            uint16*   pu8OutputList,
                                            uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpIeeeAddrReq( uint16    u16Dst,
                                          uint16    u16Addr,
                                          uint8     u8RequestType,
                                          uint8     u8StartIndex,
                                          uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpNwkAddrReq ( uint16    u16Dst,
                                          uint64    u64Addr,
                                          uint8     u8RequestType,
                                          uint8     u8StartIndex,
                                          uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpPermitJoiningReq ( uint16    u16DstAddr,
                                                uint8     u8PermitDuration,
                                                bool      bTcSignificance,
                                                uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eBindUnbindEntry ( bool_t         bBind,
                                            uint64         u64SrcAddr,
                                            uint8          u8SrcEndpoint,
                                            uint16         u16ClusterId,
                                            ZPS_tuAddress  *puDstAddress,
                                            uint8          u8DstEndpoint,
                                            uint8          u8DstAddrMode,
                                            uint8          *pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpComplexDescReq ( uint16    u16Addr,
                                              uint16    u16NwkAddressInterst,
                                              uint8*    pu8Seq );
#ifdef FULL_FUNC_DEVICE

PRIVATE void APP_vControlNodeScanStart ( void );

#endif

PRIVATE void APP_vControlNodeStartNetwork ( void );

PRIVATE ZPS_teStatus APP_eZdpMgmtLqiRequest ( uint16    u16Addr,
                                              uint8     u8StartIndex,
                                              uint8     *pu8Seq);

PRIVATE teZCL_Status APP_eZclDiscoverAttributes ( bool             bIsExtReq,
                                                  uint8            u8SourceEndPointId,
                                                  uint8            u8DestinationEndPointId,
                                                  uint16           u16ClusterId,
                                                  bool_t           bDirectionIsServerToClient,
                                                  tsZCL_Address    *psDestinationAddress,
                                                  uint8            *pu8TransactionSequenceNumber,
                                                  uint16           u16AttributeId,
                                                  bool_t           bIsManufacturerSpecific,
                                                  uint16           u16ManufacturerCode,
                                                  uint8            u8MaximumNumberOfIdentifiers );

PRIVATE teZCL_Status APP_eZclDiscoverCommandReceived ( uint8            u8SourceEndPointId,
                                                       uint8            u8DestinationEndPointId,
                                                       uint16           u16ClusterId,
                                                       bool_t           bDirectionIsServerToClient,
                                                       tsZCL_Address    *psDestinationAddress,
                                                       uint8            *pu8TransactionSequenceNumber,
                                                       uint8            u8CommandId,
                                                       bool_t           bIsManufacturerSpecific,
                                                       uint16           u16ManufacturerCode,
                                                       uint8            u8MaximumNumberOfCommands );

PRIVATE teZCL_Status APP_eZclDiscoverCommandGenerated (    uint8            u8SourceEndPointId,
                                                           uint8            u8DestinationEndPointId,
                                                           uint16           u16ClusterId,
                                                           bool_t           bDirectionIsServerToClient,
                                                           tsZCL_Address    *psDestinationAddress,
                                                           uint8            *pu8TransactionSequenceNumber,
                                                           uint8            u8CommandId,
                                                           bool_t           bIsManufacturerSpecific,
                                                           uint16           u16ManufacturerCode,
                                                           uint8            u8MaximumNumberOfCommands );

PRIVATE teZCL_Status APP_eZclBasicResetToFactoryDefaults (  uint8            u8SourceEndPointId,
                                                            uint8            u8DestinationEndPointId,
                                                            tsZCL_Address    *psDestinationAddress,
                                                            uint8            *pu8TransactionSequenceNumber );

PRIVATE ZPS_teStatus APP_eApsProfileDataRequest ( ZPS_tsAfProfileDataReq*    psProfileDataReq,
                                                  uint8*                     pu8Data,
                                                  uint8                      u8DataLength,
                                                  uint8*                     pu8Seq  ) ;


PRIVATE ZPS_teStatus APP_eSetUserDescriptorReq( uint16    u16Addr,
                                                uint16    u16AddrOfInt,
                                                uint8*     pu8Data,
                                                uint8     u8Length,
                                                uint8*    pu8Seq );

PRIVATE ZPS_teStatus APP_eZdpUserDescReq( uint16    u16Addr,
                                          uint16    u16AddrOfInt,
                                          uint8*    pu8Seq );
/****************************************************************************/
/***    Exported Variables                        ***/
/****************************************************************************/

extern tsLedState      s_sLedState;
#if gFsciIncluded_c
extern msgQueue_t mFsciRxPkts;
#endif

/****************************************************************************/
/***    Local Variables                           ***/
/****************************************************************************/
uint16             u16PacketType;
uint16             u16PacketLength;
#if gFsciIncluded_c
uint8*             au8LinkRxBuffer;
#else
uint8              au8LinkRxBuffer[MAX_PACKET_SIZE];
#endif
bool_t             bResetIssued          =  FALSE;
uint32             u32ChannelMask        =  0;
uint32             u32OldFrameCtr;
uint64             u64CallbackMacAddress =  0;
uint32             u32TimerSeconds       =  0;
bool_t             bProcessMessages      =  TRUE;
bool_t             bBlackListEnable      =  FALSE;
ZPS_TclkDescriptorEntry    asTclkStruct[ZNC_MAX_TCLK_DEVICES];
#ifdef FULL_FUNC_DEVICE
bool_t                     bSendFactoryResetOverAir;
#endif
/****************************************************************************/
/***    Exported Public Functions                     ***/
/****************************************************************************/
extern void vZpsGlobalsInit(void);
/****************************************************************************/
/***    Exported Private Functions                      */
/****************************************************************************/

PUBLIC bool APP_bSendHATransportKey ( uint16    u16ShortAddress,
                                      uint64    u64DeviceAddress,
                                      uint64    u64ParentAddress,
                                      uint8     u8Status);

PUBLIC  teZCL_Status  APP_eSendWriteAttributesRequest ( uint8            u8SourceEndPointId,
                                                        uint8            u8DestinationEndPointId,
                                                        uint16           u16ClusterId,
                                                        bool_t           bDirectionIsServerToClient,
                                                        tsZCL_Address    *psDestinationAddress,
                                                        uint8            *pu8TransactionSequenceNumber,
                                                        bool_t           bIsManufacturerSpecific,
                                                        uint16           u16ManufacturerCode,
                                                        uint8            *pu8AttributeRequestList,
                                                        uint8            u8NumberOfAttrib,
                                                        uint16           u16SizePayload );

/****************************************************************************/
/***    Implementation                          */
/****************************************************************************/

PUBLIC void APP_vProcessIncomingSerialCommands ( uint8    u8RxByte )
{
    uint8                  u8SeqNum = 0;
    uint8                  u8Status = 0;
    uint16                 u16TargetAddress;
    tsZCL_Address          sAddress;
    uint8                  au8values[4];
    uint8                  u8Length = 0;
#ifdef FULL_FUNC_DEVICE
    tsBDB_ZCLEvent         sEvent;
#endif
    bool_t newCommand = FALSE;
    
#if gFsciIncluded_c
    (void)u8RxByte;
    newCommand = MSG_Pending(&mFsciRxPkts);
#else
    newCommand = bSL_ReadMessage (&u16PacketType,
                              &u16PacketLength,
                              MAX_PACKET_SIZE,
                              au8LinkRxBuffer,
                              u8RxByte);
#endif
    
    
    if (newCommand)
    {
#if gFsciIncluded_c
        clientPacket_t *pMsg = (clientPacket_t *)MSG_DeQueue(&mFsciRxPkts);
        u16PacketLength = pMsg->structured.header.len;
        u16PacketType  = (pMsg->structured.header.opGroup - ZB_FSCI_CMD_OFFSET) << 8;
        u16PacketType |= pMsg->structured.header.opCode;
        au8LinkRxBuffer = pMsg->structured.payload;
#endif
        if (u16PacketType >= E_SL_MSG_AHI_START && u16PacketType <= E_SL_MSG_AHI_END)
        {
#ifdef APP_AHI_CONTROL
            //APP_vCMDHandleAHICommand(u16PacketType, u16PacketLength, au8LinkRxBuffer, &u8Status);
#endif
        }
        else
        {
            u16TargetAddress                           =  ZNC_RTN_U16( au8LinkRxBuffer , 1);
            sAddress.eAddressMode                      =  au8LinkRxBuffer[0];
            sAddress.uAddress.u16DestinationAddress    =  u16TargetAddress;
            
            vLog_Printf(TRACE_APP,LOG_DEBUG, "\nPacket Type %x \n",u16PacketType );
            
            
            switch ( u16PacketType )
            {
            case (E_SL_MSG_GET_VERSION):
                {
                    uint32     u32Version = VERSION;

#if gFsciIncluded_c
                    MEM_BufferFree(pMsg);
#endif
                    ZNC_BUF_U8_UPD  ( &au8values[ 0 ], u8Status,      u8Length );
                    ZNC_BUF_U8_UPD  ( &au8values[ 1 ], u8SeqNum,      u8Length );
                    ZNC_BUF_U16_UPD ( &au8values[ 2 ], u16PacketType, u8Length );
                    vSL_WriteMessage ( E_SL_MSG_STATUS,
                                      u8Length,
                                      au8values );
                    vSL_WriteMessage ( E_SL_MSG_VERSION_LIST,
                                      sizeof ( uint32 ),
                                      ( uint8* ) &u32Version );
                    return;
                }
                break;
                
                
            case (E_SL_MSG_SET_EXT_PANID):
                {
                    uint64    u64Value ;
                    u64Value    =  ZNC_RTN_U64 ( au8LinkRxBuffer, 0 );
                    u8Status    =  ZPS_eAplAibSetApsUseExtendedPanId ( u64Value );
                }
                break;
                
            case (E_SL_MSG_SET_CHANNELMASK):
                {
                    
                    uint32    u32Value;
                    uint8    u8Channel;
                    u32Value      =  ZNC_RTN_U32 ( au8LinkRxBuffer, 0 );
                    u8Status      =  ZPS_eAplAibSetApsChannelMask ( u32Value );
                    
                    if ( ( u32Value >  10 ) && ( u32Value <  27 ) )
                    {
                        /* ChannelList is a single channel */
                        u32Value =  ( 1 << ZNC_RTN_U32 ( au8LinkRxBuffer, 0 ) );
                    }
                    
                    u32Value  &=  0x07fff800;
                    
                    if (u32Value == 0)
                    {
                        /* ChannelList is 0 (or supplied channel was invalid), indicating to scan all channels */
                        u32Value =  0x07fff800;
                    }
                    for(u8Channel = 11; u8Channel < 27; u8Channel++)
                    {
                        if(u32Value & (1<<u8Channel))
                        {
                            ZPS_vNwkNibSetChannel(ZPS_pvAplZdoGetNwkHandle(), u8Channel);
                            break;
                        }
                    }
                    u32ChannelMask    =  u32Value;
                    sBDB.sAttrib.u32bdbPrimaryChannelSet   =  u32Value;
                    sBDB.sAttrib.u32bdbSecondaryChannelSet =  0;
                }
                break;

            case (E_SL_MSG_GET_PERMIT_JOIN):
                {
                    APP_tsEvent    sAppEvent;
                    
                    sAppEvent.eType = APP_E_EVENT_SEND_PERMIT_JOIN;
                    ZQ_bQueueSend( &APP_msgAppEvents, &sAppEvent ) ;
                }
                break;

            case (E_SL_MSG_POLL_INTERVAL_SET):
                {
                    extern uint8 u8AppPollTimer;
                    u32TimerSeconds    =  ZNC_RTN_U32 ( au8LinkRxBuffer, 0 );
                    ZTIMER_eStart(u8AppPollTimer, ZTIMER_TIME_MSEC(u32TimerSeconds));
                    DBG_vPrintf(TRUE, "u32TimerSeconds %x \n", u32TimerSeconds);
                }
            break;

            case (E_SL_MSG_SET_DEVICETYPE):
                {
                    
                    if(au8LinkRxBuffer[0] >= 3 )
                    {
                        APP_vConfigureDevice(1); /* configure it in HA compatibility mode */
                    }
                    else
                    {
                        APP_vConfigureDevice(au8LinkRxBuffer[0]);
                    }
                    
                    sZllState.u8DeviceType =  au8LinkRxBuffer[0];
                    PDM_eSaveRecordData( PDM_ID_APP_ZLL_CMSSION, &sZllState, sizeof ( sZllState ) );
                }
                break;

            case (E_SL_MSG_NETWORK_REMOVE_DEVICE):
                {
                    ZPS_tuAddress    uParentAddress;
                    ZPS_tuAddress    uChildAddress;
                    
                    uParentAddress.u64Addr    =  ZNC_RTN_U64 (au8LinkRxBuffer, 0 ) ;
                    uChildAddress.u64Addr     =  ZNC_RTN_U64 (au8LinkRxBuffer, 8 ) ;
                    u8Status = APP_eZdpRemoveDeviceReq ( uParentAddress,
                                                        uChildAddress );
                }
                break;
                
            case E_SL_MSG_LEAVE_REQUEST:
                {
                    ZPS_tuAddress    uAddress;
                    bool         u8RemoveChildren;
                    bool         bRejoin;
                    
                    uAddress.u64Addr     =  ZNC_RTN_U64 (au8LinkRxBuffer, 0 ) ;
                    bRejoin              =  au8LinkRxBuffer[8];
                    u8RemoveChildren     =  au8LinkRxBuffer[9];
                    
                    u8Status = APP_eZdpLeaveReq ( uAddress, u8RemoveChildren, bRejoin );
                }
                break;

            case (E_SL_MSG_BIND_GROUP):
                {
                    uint16    u16Clusterid;
                    uint8     u8SrcEp;
                    uint8     offset = 0;
                    uint16    u16GroupAddr;
                    
                    u16Clusterid    =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer, offset, offset );
                    u8SrcEp         =  au8LinkRxBuffer[ offset++ ];
                    u16GroupAddr    =  ZNC_RTN_U16_OFFSET (au8LinkRxBuffer , offset, offset );
                    ZPS_eAplZdoBindGroup ( u16Clusterid,
                                          u8SrcEp,
                                          u16GroupAddr );
                }
                break;
                
            case (E_SL_MSG_UNBIND_GROUP):
                {
                    uint16    u16Clusterid;
                    uint8     u8SrcEp;
                    uint8     offset = 0;
                    uint16    u16GroupAddr;
                    
                    u16Clusterid    =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer, offset, offset );
                    u8SrcEp         =  au8LinkRxBuffer [ offset++ ];
                    u16GroupAddr    =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer, offset, offset );
                    
                    ZPS_eAplZdoUnbindGroup( u16Clusterid,
                                           u8SrcEp,
                                           u16GroupAddr );
                }
                break;
                
                
            case (E_SL_MSG_RESET):
                {
                    bResetIssued    =  TRUE;
                    ZTIMER_eStart( u8IdTimer, ZTIMER_TIME_MSEC ( 1 ) );
                    
                }
                break;

            case (E_SL_MSG_START_NETWORK):
                {
                    APP_vControlNodeStartNetwork();
                }
                break;

#ifdef FULL_FUNC_DEVICE
            case (E_SL_MSG_START_SCAN):
                {
                    APP_vControlNodeScanStart ( ) ;
                }
                break;
#endif

            case (E_SL_MSG_ADD_AUTHENTICATE_DEVICE):
                {
                    APP_tsEvent    sAppEvent;
                    uint8          i = 0;
                    
                    sAppEvent.eType                            =  APP_E_EVENT_ENCRYPT_SEND_KEY;
                    sAppEvent.uEvent.sEncSendMsg.u64Address    =  ZNC_RTN_U64 ( au8LinkRxBuffer, 0 );
                    
                    u8Status    =  ZPS_bAplZdoTrustCenterSetDevicePermissions( sAppEvent.uEvent.sEncSendMsg.u64Address,
                                                                              ZPS_DEVICE_PERMISSIONS_ALL_PERMITED );
                    if(u8Status == ZPS_E_SUCCESS)
                    {
                        while( i < 16)
                        {
                            sAppEvent.uEvent.sEncSendMsg.uKey.au8[i] = au8LinkRxBuffer[ 8 + i ];
                            i++;
                        }
                        ZQ_bQueueSend ( &APP_msgAppEvents, &sAppEvent);
                    }
                }
                break;
                
            case (E_SL_MSG_UPDATE_AUTHENTICATE_DEVICE):
                {
                    uint64         u64Address;
                    
                    u64Address    =  ZNC_RTN_U64 ( au8LinkRxBuffer, 0 );
                    u8Status      =  ZPS_bAplZdoTrustCenterSetDevicePermissions( u64Address,
                                                                                au8LinkRxBuffer[8] );
                    
                }
                break;
                
            case (E_SL_MSG_NETWORK_WHITELIST_ENABLE):
                {
                    bBlackListEnable    =  au8LinkRxBuffer [ 0 ] ;
                    ZPS_vTCSetCallback( (void*)APP_bSendHATransportKey );
                }
                break;

            case (E_SL_MSG_SET_SECURITY):
                {
                    /* for backwards compatibility remap key types */
                    switch ( au8LinkRxBuffer[0] )
                    {
                    case 1:
                    case 3:
                        au8LinkRxBuffer[0] = ZPS_ZDO_PRECONFIGURED_LINK_KEY;
                        break;
                    case 2:
                    case 4:
                        au8LinkRxBuffer[0] = ZPS_ZDO_DISTRIBUTED_LINK_KEY;
                        break;
                    default:
                        au8LinkRxBuffer[0] = ZPS_ZDO_PRCONFIGURED_INSTALLATION_CODE;
                        break;
                    }
                    ZPS_vAplSecSetInitialSecurityState ( au8LinkRxBuffer[0],
                                                        &au8LinkRxBuffer[3],
                                                        au8LinkRxBuffer[1],
                                                        au8LinkRxBuffer[2] );
                }
                break;
                
            case (E_SL_MSG_ERASE_PERSISTENT_DATA):
                {
                    PDM_vDeleteAllDataRecords();
                    bResetIssued    =  TRUE;
                    ZTIMER_eStart( u8IdTimer, ZTIMER_TIME_MSEC ( 10 ) );
                }
                break;
                
#ifdef FULL_FUNC_DEVICE
            case (E_SL_MSG_TOUCHLINK_FACTORY_RESET):
                {
                    sEvent.eType             =  BDB_E_ZCL_EVENT_TL_START;
                    bSendFactoryResetOverAir =  TRUE;
                    BDB_vZclEventHandler ( &sEvent );
                    
                }
                break;
                
            case (E_SL_MSG_ZLL_FACTORY_NEW):
                {
                    if (sZllState.u8DeviceType != ZPS_ZDO_DEVICE_COORD)
                    {
                        ZPS_tsNwkNib *psNib    =  ZPS_psAplZdoGetNib ( );
                        
                        u32OldFrameCtr    =  psNib->sTbl.u32OutFC;
                        APP_vFactoryResetRecords ( );
                    }
                }
                break;
                
            case (E_SL_MSG_INITIATE_TOUCHLINK):
                {
                    if(sZllState.u8DeviceType != ZPS_ZDO_DEVICE_COORD)
                    {
                        APP_vAppAddGroup( 0 , FALSE );
                        sEvent.eType              =  BDB_E_ZCL_EVENT_TL_START;
                        BDB_vZclEventHandler ( &sEvent );
                    }
                    else
                    {
                        u8Status            =  E_SL_MSG_STATUS_BUSY;
                        bProcessMessages    =  FALSE;
                    }
                    
                }
                break;
#endif
            case (E_SL_MSG_SEND_RAW_APS_DATA_PACKET):
                {
                    ZPS_tsAfProfileDataReq    sAfProfileDataReq;
                    uint8                     u8DataLength;
                    
                    sAfProfileDataReq.uDstAddr.u16Addr    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 1 );
                    sAfProfileDataReq.u16ClusterId        =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sAfProfileDataReq.u16ProfileId        =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 );
                    sAfProfileDataReq.eDstAddrMode        =  au8LinkRxBuffer[0];
                    sAfProfileDataReq.u8SrcEp             =  au8LinkRxBuffer[3];
                    sAfProfileDataReq.u8DstEp             =  au8LinkRxBuffer[4];
                    sAfProfileDataReq.eSecurityMode       =  au8LinkRxBuffer[9];
                    sAfProfileDataReq.u8Radius            =  au8LinkRxBuffer[10];
                    u8DataLength                          =  au8LinkRxBuffer[11];
                    
                    u8Status                              =  APP_eApsProfileDataRequest ( &sAfProfileDataReq,
                                                                                         &au8LinkRxBuffer[12],
                                                                                         u8DataLength,
                                                                                         &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_COMPLEX_DESCRIPTOR_REQUEST):
                {
                    uint16    u16PayloadAddress;
                    
                    u16TargetAddress     =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u16PayloadAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 2 );
                    
                    u8Status    =  APP_eZdpComplexDescReq ( u16TargetAddress,
                                                           u16PayloadAddress,
                                                           &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_MATCH_DESCRIPTOR_REQUEST):
                {
                    uint16    au16InClusterList[10];
                    uint16    au16OutClusterList[10];
                    uint16    u16Profile;
                    uint8     i                 =  0 ;
                    uint8     u8InClusterCount  =  au8LinkRxBuffer [ 4 ];
                    uint8     u8OutClusterCount =  au8LinkRxBuffer [ ( ( au8LinkRxBuffer [ 4 ] *
                                                                        ( sizeof ( uint16 ) ) ) + 5) ];
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u16Profile          =  ZNC_RTN_U16 ( au8LinkRxBuffer, 2 );
                    
                    while ( ( i < 10 )  &&
                           ( i < u8InClusterCount ) )
                    {
                        au16InClusterList[ i ]    =  ZNC_RTN_U16 ( au8LinkRxBuffer, ( 5 + ( i * 2 ) ) );
                        i++;
                    }
                    
                    i =  0 ;
                    while ( ( i < 10 )  &&
                           ( i < u8OutClusterCount ) )
                    {
                        au16OutClusterList [ i ]    =  ZNC_RTN_U16 ( au8LinkRxBuffer, ( ( ( u8InClusterCount * 2 )  +  6 )  +
                                                                                       ( i * 2 ) ) ) ;
                        i++;
                    }
                    
                    u8Status    =  APP_eZdpMatchDescReq ( u16TargetAddress,
                                                         u16Profile,
                                                         u8InClusterCount,
                                                         au16InClusterList,
                                                         u8OutClusterCount,
                                                         au16OutClusterList,
                                                         &u8SeqNum );
                    
                }
                break;
                
                
            case (E_SL_MSG_NODE_DESCRIPTOR_REQUEST):
                {
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u8Status            =  APP_eZdpNodeDescReq ( u16TargetAddress,
                                                                &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_SIMPLE_DESCRIPTOR_REQUEST):
                {
                    uint8 u8Endpoint    =  au8LinkRxBuffer[2];
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    
                    u8Status            =  APP_eZdpSimpleDescReq ( u16TargetAddress,
                                                                  u8Endpoint,
                                                                  &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_PERMIT_JOINING_REQUEST):
                {
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer , 0 );
                    
                    if(sZllState.u8DeviceType >=2)
                    {
                        ZPS_eAplAibSetApsTrustCenterAddress ( ZPS_u64NwkNibGetExtAddr ( ZPS_pvAplZdoGetNwkHandle ( ) ) );
                    }
                    
                    u8Status    =  APP_eZdpPermitJoiningReq ( u16TargetAddress,
                                                             au8LinkRxBuffer[2],
                                                             au8LinkRxBuffer[3],
                                                             &u8SeqNum );
                    
                }
                break;
                
                
            case (E_SL_MSG_POWER_DESCRIPTOR_REQUEST):
                {
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u8Status            =  APP_eZdpPowerDescReq ( u16TargetAddress,
                                                                 &u8SeqNum );
                }
                break;
                
                
            case (E_SL_MSG_ACTIVE_ENDPOINT_REQUEST):
                {
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u8Status            =  APP_eZdpActiveEndpointReq ( u16TargetAddress,
                                                                      &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_MANAGEMENT_NETWORK_UPDATE_REQUEST):
                {
                    uint32    u32ChannelMask;
                    uint8     u8ScanDuration;
                    uint8     u8ScanCount;
                    uint16    u16NwkManagerAddr;
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u32ChannelMask      =  ZNC_RTN_U32 ( au8LinkRxBuffer, 2 );
                    u8ScanDuration      =  au8LinkRxBuffer[6];
                    u8ScanCount         =  au8LinkRxBuffer[7];
                    u16NwkManagerAddr   =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8);
                    
                    u8Status        =  APP_eZdpMgmtNetworkUpdateReq ( u16TargetAddress,
                                                                     u32ChannelMask,
                                                                     u8ScanDuration,
                                                                     u8ScanCount,
                                                                     &u8SeqNum,
                                                                     u16NwkManagerAddr);
                }
                break;
                
            case (E_SL_MSG_SYSTEM_SERVER_DISCOVERY):
                {
                    uint16    u16ServerMask;
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u16ServerMask       =  ZNC_RTN_U16 ( au8LinkRxBuffer, 2 );
                    
                    u8Status        =  APP_eZdpSystemServerDiscovery ( u16ServerMask,
                                                                      &u8SeqNum );
                }
                break;
                
                
                
            case (E_SL_MSG_IEEE_ADDRESS_REQUEST):
                {
                    uint16     u16LookupAddress;
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u16LookupAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 2 );
                    
                    u8Status        =  APP_eZdpIeeeAddrReq ( u16TargetAddress,
                                                            u16LookupAddress,
                                                            au8LinkRxBuffer[4],
                                                            au8LinkRxBuffer[5],
                                                            &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_NETWORK_ADDRESS_REQUEST):
                {
                    uint64     u64LookupAddress;
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u64LookupAddress    =  ZNC_RTN_U64 ( au8LinkRxBuffer, 2 );
                    
                    u8Status  =  APP_eZdpNwkAddrReq( u16TargetAddress,
                                                    u64LookupAddress,
                                                    au8LinkRxBuffer[10],
                                                    au8LinkRxBuffer[11],
                                                    &u8SeqNum);
                }
                break;
                
                
            case (E_SL_MSG_MANAGEMENT_LQI_REQUEST):
                {
                    uint8    u8StartIndex;
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 0 );
                    u8StartIndex        =  au8LinkRxBuffer[2];
                    u8Status            = APP_eZdpMgmtLqiRequest ( u16TargetAddress,
                                                                  u8StartIndex,
                                                                  &u8SeqNum );
                    
                }
                break;
                
            case (E_SL_MSG_BIND):
            case (E_SL_MSG_UNBIND):
                {
                    uint64            u64BindAddress;
                    uint16            u16Clusterid;
                    uint8             u8SrcEp;
                    uint8             offset  = 0;
                    uint8             u8DstEp = 0;
                    uint8             u8DstAddrMode;
                    ZPS_tuAddress     uDstAddress;
                    bool_t            bBind = FALSE;
                    
                    u64BindAddress         =  ZNC_RTN_U64_OFFSET ( au8LinkRxBuffer , offset, offset );
                    u8SrcEp                =  au8LinkRxBuffer[ offset++ ];
                    u16Clusterid           =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer , offset, offset );
                    u8DstAddrMode          =  au8LinkRxBuffer[ offset++ ];
                    uDstAddress.u64Addr    =  ZNC_RTN_U64_OFFSET ( au8LinkRxBuffer , offset, offset );
                    
                    if(u8DstAddrMode == 0x3)
                    {
                        u8DstEp    =  au8LinkRxBuffer [ offset++ ] ;
                    }
                    
                    ( (u16PacketType == E_SL_MSG_BIND) ?  ( bBind = TRUE ) :
                           ( bBind = FALSE ) ) ;
                    
                    u8Status    =  APP_eBindUnbindEntry ( bBind,
                                                         u64BindAddress,
                                                         u8SrcEp,
                                                         u16Clusterid,
                                                         &uDstAddress,
                                                         u8DstEp,
                                                         u8DstAddrMode,
                                                         &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_MANAGEMENT_LEAVE_REQUEST):
                {
                    uint64    u64LookupAddress;
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer , 0 );
                    u64LookupAddress    =  ZNC_RTN_U64 ( au8LinkRxBuffer , 2 );
                    
                    u8Status    =  APP_eZdpMgmtLeave ( u16TargetAddress,
                                                      u64LookupAddress,
                                                      au8LinkRxBuffer[10],
                                                      au8LinkRxBuffer[11],
                                                      &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_USER_DESC_SET):
                {
                    uint16    u16AddrInterest;
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer , 0 );
                    u16AddrInterest     =  ZNC_RTN_U16 ( au8LinkRxBuffer , 2 );
                    
                    u8Status    =  APP_eSetUserDescriptorReq ( u16TargetAddress,
                                                              u16AddrInterest,
                                                              &au8LinkRxBuffer[5],
                                                              au8LinkRxBuffer[4],
                                                              &u8SeqNum );
                }
                break;
                
            case (E_SL_MSG_USER_DESC_REQ):
                {
                    uint16    u16AddrInterest;
                    
                    u16TargetAddress    =  ZNC_RTN_U16 ( au8LinkRxBuffer , 0 );
                    u16AddrInterest     =  ZNC_RTN_U16 ( au8LinkRxBuffer , 2 );
                    
                    u8Status    =  APP_eZdpUserDescReq ( u16TargetAddress,
                                                        u16AddrInterest,
                                                        &u8SeqNum );
                }
                break;

            case E_SL_MSG_MANY_TO_ONE_ROUTE_REQUEST:
                {
                    u8Status    = ZPS_eAplZdoManyToOneRouteRequest( au8LinkRxBuffer[3],        // bCacheRoute
                                                                   au8LinkRxBuffer[4] );        // u8Radius
                }
                break;

                /* Group cluster commands */
            case (E_SL_MSG_ADD_GROUP):
                {
                    if ( 0x0000 == u16TargetAddress )
                    {
                        uint16          u16GroupId;
                        uint8           i;
                        ZPS_tsAplAib    *psAplAib = ZPS_psAplAibGetAib();
                        
                        u16GroupId      =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                        
                        vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nAdd Group ID: %x", u16GroupId );
                        vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nAdd EndPoint: %x", au8LinkRxBuffer[4] );
                        
                        /* Request to add the bridge to a group, no name supported... */
                        u8Status    = ZPS_eAplZdoGroupEndpointAdd ( u16GroupId,
                                                                   au8LinkRxBuffer [ 4 ] );
                        
                        for ( i = 0; i < psAplAib->psAplApsmeGroupTable->u32SizeOfGroupTable; i++ )
                        {
                            vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nGroup ID: %x",
                                         psAplAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[i].u16Groupid );
                            vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nEndPoint 0: %x",
                                         psAplAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[i].au8Endpoint[0] );
                        }
                    }
                    else
                    {
                        tsCLD_Groups_AddGroupRequestPayload    sRequest;
                        
                        sRequest.u16GroupId        =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                        sRequest.sGroupName.u8Length       =  0;
                        sRequest.sGroupName.u8MaxLength    =  0;
                        sRequest.sGroupName.pu8Data    =  (uint8*)"";
                        
                        u8Status    =  eCLD_GroupsCommandAddGroupRequestSend( au8LinkRxBuffer [ 3 ],
                                                                             au8LinkRxBuffer [ 4 ],
                                                                             &sAddress,
                                                                             &u8SeqNum,
                                                                             &sRequest );
                    }
                }
                break;
                
            case (E_SL_MSG_REMOVE_GROUP):
                {
                    if ( 0x0000 == u16TargetAddress )
                    {
                        uint16    u16GroupId;
                        
                        u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                        
                        /* Request is for the control bridge */
                        u8Status    =  ZPS_eAplZdoGroupEndpointRemove ( u16GroupId,
                                                                       au8LinkRxBuffer [ 4 ] );
                    }
                    else
                    {
                        /* Request is for a remote node */
                        tsCLD_Groups_RemoveGroupRequestPayload    sRequest;
                        
                        sRequest.u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                        u8Status =  eCLD_GroupsCommandRemoveGroupRequestSend( au8LinkRxBuffer [ 3 ],
                                                                             au8LinkRxBuffer [ 4 ] ,
                                                                             &sAddress,
                                                                             &u8SeqNum,
                                                                             &sRequest);
                    }
                }
                break;
                
            case (E_SL_MSG_REMOVE_ALL_GROUPS):
                {
                    if (0x0000 == u16TargetAddress)
                    {
                        vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nRemove All Groups" );
                        vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nDst EndPoint: %x", au8LinkRxBuffer [ 4 ] );
                        
                        /* Request is for the control bridge */
                        u8Status =  ZPS_eAplZdoGroupAllEndpointRemove( au8LinkRxBuffer [ 4 ] );
                    }
                    else
                    {
                        tsZCL_Address    sAddress;
                        uint16           u16TargetAddress;
                        
                        u16TargetAddress                =  ZNC_RTN_U16 ( au8LinkRxBuffer , 1 );
                        sAddress.eAddressMode           =  au8LinkRxBuffer[0];
                        sAddress.uAddress.u16DestinationAddress =  u16TargetAddress;
                        u8Status = eCLD_GroupsCommandRemoveAllGroupsRequestSend(au8LinkRxBuffer [ 3 ],
                                                                                au8LinkRxBuffer [ 4 ],
                                                                                &sAddress,
                                                                                &u8SeqNum );
                    }
                }
                break;
                
            case (E_SL_MSG_ADD_GROUP_IF_IDENTIFY):
                {
                    tsCLD_Groups_AddGroupRequestPayload    sRequest;
                    
                    sRequest.u16GroupId                =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sRequest.sGroupName.u8Length       =  0;
                    sRequest.sGroupName.u8MaxLength    =  0;
                    sRequest.sGroupName.pu8Data        =  (uint8*)"";
                    
                    u8Status =  eCLD_GroupsCommandAddGroupIfIdentifyingRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                    au8LinkRxBuffer [ 4 ],
                                                                                    &sAddress,
                                                                                    &u8SeqNum,
                                                                                    &sRequest );
                }
                break;
                
            case (E_SL_MSG_VIEW_GROUP):
                {
                    tsCLD_Groups_ViewGroupRequestPayload    sRequest;
                    
                    sRequest.u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u8Status    =  eCLD_GroupsCommandViewGroupRequestSend ( au8LinkRxBuffer[3],
                                                                           au8LinkRxBuffer[4],
                                                                           &sAddress,
                                                                           &u8SeqNum,
                                                                           &sRequest );
                }
                break;
                
            case (E_SL_MSG_GET_GROUP_MEMBERSHIP):
                {
                    tsCLD_Groups_GetGroupMembershipRequestPayload    sRequest;
                    uint16                                           au16GroupList [ 10 ];
                    uint8                                            i = 0 ;
                    
                    while ( ( i < 10 ) &&
                           ( i < au8LinkRxBuffer [ 5 ] ) )
                    {
                        au16GroupList[i]    =  ZNC_RTN_U16( au8LinkRxBuffer, ( 6 + ( i * 2) ) );
                        i++;
                    }
                    sRequest.pi16GroupList    =  ( zint16* ) au16GroupList;
                    sRequest.u8GroupCount     =  au8LinkRxBuffer [ 5 ];
                    
                    u8Status    =  eCLD_GroupsCommandGetGroupMembershipRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                    au8LinkRxBuffer [ 4 ],
                                                                                    &sAddress,
                                                                                    &u8SeqNum,
                                                                                    &sRequest );
                }
                break;
                
                /*Scenes Cluster */
            case (E_SL_MSG_ADD_SCENE):
                {
                   uint8                                 au8Data [ 16 ];
                   uint8                                 i = 0;
                   tsCLD_ScenesAddSceneRequestPayload    sRequest;
                    
                    sRequest.u16GroupId                =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sRequest.u8SceneId                 =  au8LinkRxBuffer[7];
                    sRequest.u16TransitionTime         =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    sRequest.sSceneName.u8Length       =  au8LinkRxBuffer[10];
                    sRequest.sSceneName.u8MaxLength    =  au8LinkRxBuffer[11];
                    
                    while ( ( i < 16 ) &&
                           ( i < sRequest.sSceneName.u8Length ) )
                    {
                        au8Data [ i ]    =  au8LinkRxBuffer[ 12 + i ];
                        i++;
                    }
                    sRequest.sSceneName.pu8Data       =  au8Data;
                    sRequest.sExtensionField.pu8Data      =  NULL;
                    sRequest.sExtensionField.u16Length    =  0;
                   
                    
                    u8Status    =  eCLD_ScenesCommandAddSceneRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                          au8LinkRxBuffer [ 4 ],
                                                                          &sAddress,
                                                                          &u8SeqNum,
                                                                          &sRequest );
                }
                break;
                
            case (E_SL_MSG_REMOVE_SCENE):
                {
                    tsCLD_ScenesRemoveSceneRequestPayload    sRequest;
                    
                    sRequest.u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sRequest.u8SceneId     =  au8LinkRxBuffer[7];
                    u8Status               =  eCLD_ScenesCommandRemoveSceneRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                        au8LinkRxBuffer [ 4 ],
                                                                                        &sAddress,
                                                                                        &u8SeqNum,
                                                                                        &sRequest );
                }
                break;
                
            case (E_SL_MSG_VIEW_SCENE):
                {
                    tsCLD_ScenesViewSceneRequestPayload    sRequest;
                    
                    sRequest.u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sRequest.u8SceneId     =  au8LinkRxBuffer[7];
                    u8Status               =  eCLD_ScenesCommandViewSceneRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                      au8LinkRxBuffer [ 4 ],
                                                                                      &sAddress,
                                                                                      &u8SeqNum,
                                                                                      &sRequest );
                }
                break;
                
                
            case (E_SL_MSG_REMOVE_ALL_SCENES):
                {
                    tsCLD_ScenesRemoveAllScenesRequestPayload    sRequest;
                    
                    sRequest.u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u8Status               =  eCLD_ScenesCommandRemoveAllScenesRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                            au8LinkRxBuffer [ 4 ],
                                                                                            &sAddress,
                                                                                            &u8SeqNum,
                                                                                            &sRequest );
                }
                break;
                
            case (E_SL_MSG_STORE_SCENE):
                {
                    tsCLD_ScenesStoreSceneRequestPayload    sRequest;
                    
                    sRequest.u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sRequest.u8SceneId     =  au8LinkRxBuffer[7];
                    u8Status     =  eCLD_ScenesCommandStoreSceneRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                             au8LinkRxBuffer [ 4 ],
                                                                             &sAddress,
                                                                             &u8SeqNum,
                                                                             &sRequest );
                }
                break;
                
            case (E_SL_MSG_RECALL_SCENE):
                {
                    tsCLD_ScenesRecallSceneRequestPayload    sRequest;
                    
                    sRequest.u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sRequest.u8SceneId     =  au8LinkRxBuffer[7];
                    u8Status    =  eCLD_ScenesCommandRecallSceneRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                             au8LinkRxBuffer [ 4 ],
                                                                             &sAddress,
                                                                             &u8SeqNum,
                                                                             &sRequest );
                }
                break;
#ifdef  CLD_SCENES_CMD_ENHANCED_ADD_SCENE
            case (E_SL_MSG_ADD_ENHANCED_SCENE):
                {
                    tsCLD_ScenesEnhancedAddSceneRequestPayload    sRequest;
                    uint8 i = 0;
                    
                    sRequest.u16GroupId                   =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sRequest.u8SceneId                    =  au8LinkRxBuffer[7];
                    sRequest.u16TransitionTime100ms       =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    sRequest.sSceneName.u8Length          =  au8LinkRxBuffer[ 10 ]; 
                    
                    if(sRequest.sSceneName.u8Length > 16)
                      sRequest.sSceneName.u8Length = 16;
                    
                    i = sRequest.sSceneName.u8Length;
                    
                    sRequest.sSceneName.u8MaxLength       =  16;
                    sRequest.sSceneName.pu8Data           =  &au8LinkRxBuffer[ 11 ];
                    sRequest.sExtensionField.u16Length    =  au8LinkRxBuffer[ 11 + i ];
                    sRequest.sExtensionField.u16MaxLength =  au8LinkRxBuffer[ 12 + i ];
                    sRequest.sExtensionField.pu8Data      =  &au8LinkRxBuffer[ 13 + i ];
                    u8Status    =  eCLD_ScenesCommandEnhancedAddSceneRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                  au8LinkRxBuffer [ 4 ],
                                                                                  &sAddress,
                                                                                  &u8SeqNum,
                                                                                  &sRequest );
                }
                break;
#endif
#ifdef CLD_SCENES_CMD_ENHANCED_VIEW_SCENE
            case (E_SL_MSG_VIEW_ENHANCED_SCENE):
                {
                    tsCLD_ScenesEnhancedViewSceneRequestPayload    sRequest;
                    
                    sRequest.u16GroupId =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sRequest.u8SceneId  =  au8LinkRxBuffer[7];
                    
                    u8Status    =  eCLD_ScenesCommandEnhancedViewSceneRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                   au8LinkRxBuffer [ 4 ],
                                                                                   &sAddress,
                                                                                   &u8SeqNum,
                                                                                   &sRequest );
                }
                break;
#endif
#ifdef CLD_SCENES_CMD_COPY_SCENE
            case (E_SL_MSG_COPY_SCENE):
                {
                    tsCLD_ScenesCopySceneRequestPayload    sRequest;
                    
                    sRequest.u8Mode         =  au8LinkRxBuffer[5];
                    sRequest.u16FromGroupId =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    sRequest.u8FromSceneId  =  au8LinkRxBuffer[8];
                    sRequest.u16ToGroupId   =  ZNC_RTN_U16 ( au8LinkRxBuffer, 9 );
                    sRequest.u8ToSceneId    =  au8LinkRxBuffer[11];
                    
                    u8Status    =  eCLD_ScenesCommandCopySceneSceneRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                au8LinkRxBuffer [ 4 ],
                                                                                &sAddress,
                                                                                &u8SeqNum,
                                                                                &sRequest );
                }
                break;
#endif
            case (E_SL_MSG_SCENE_MEMBERSHIP_REQUEST):
                {
                    tsCLD_ScenesGetSceneMembershipRequestPayload    sRequest;
                    
                    sRequest.u16GroupId    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u8Status               =  eCLD_ScenesCommandGetSceneMembershipRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                               au8LinkRxBuffer [ 4 ],
                                                                                               &sAddress,
                                                                                               &u8SeqNum,
                                                                                               &sRequest);
                }
                break;
                
                /* ON/OFF cluster commands */
            case (E_SL_MSG_ONOFF_EFFECTS):
                {
                    
                    tsCLD_OnOff_OffWithEffectRequestPayload    sRequest;
                    
                    sRequest.u8EffectId          =  au8LinkRxBuffer[5];
                    sRequest.u8EffectVariant     =  au8LinkRxBuffer[6];
                    u8Status                     =  eCLD_OnOffCommandOffWithEffectSend ( au8LinkRxBuffer [ 3 ],
                                                                                        au8LinkRxBuffer [ 4 ],
                                                                                        &sAddress,
                                                                                        &u8SeqNum,
                                                                                        &sRequest );
                }
                break;
                
            case (E_SL_MSG_ONOFF_NOEFFECTS):
                {
                    u8Status = eCLD_OnOffCommandSend ( au8LinkRxBuffer [ 3 ],
                                                      au8LinkRxBuffer [ 4 ],
                                                      &sAddress,
                                                      &u8SeqNum,
                                                      au8LinkRxBuffer [ 5 ] );
                }
                break;
                
            case (E_SL_MSG_ONOFF_TIMED):
                {
                    
                    tsCLD_OnOff_OnWithTimedOffRequestPayload    sRequest;
                    
                    sRequest.u8OnOff      =  au8LinkRxBuffer[5];
                    sRequest.u16OnTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    sRequest.u16OffTime   =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    
                    u8Status    =  eCLD_OnOffCommandOnWithTimedOffSend ( au8LinkRxBuffer [ 3 ],
                                                                        au8LinkRxBuffer [ 4 ],
                                                                        &sAddress,
                                                                        &u8SeqNum,
                                                                        &sRequest );
                }
                break;
                
                /* colour cluster commands */
                
            case (E_SL_MSG_MOVE_HUE):
                {
                    tsCLD_ColourControl_MoveHueCommandPayload    sPayload;
                    
                    sPayload.eMode     =  au8LinkRxBuffer[5];
                    sPayload.u8Rate    =  au8LinkRxBuffer[6];
                    
                    u8Status    =  eCLD_ColourControlCommandMoveHueCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                au8LinkRxBuffer [ 4 ],
                                                                                &sAddress,
                                                                                &u8SeqNum,
                                                                                &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_MOVE_TO_HUE_SATURATION):
                {
                    tsCLD_ColourControl_MoveToHueAndSaturationCommandPayload    sPayload;
                    
                    sPayload.u8Saturation         =  au8LinkRxBuffer[6];
                    sPayload.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 );
                    sPayload.u8Hue                =  au8LinkRxBuffer[5];
                    
                    u8Status    =  eCLD_ColourControlCommandMoveToHueAndSaturationCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                               au8LinkRxBuffer [ 4 ],
                                                                                               &sAddress,
                                                                                               &u8SeqNum,
                                                                                               &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_MOVE_TO_HUE):
                {
                    tsCLD_ColourControl_MoveToHueCommandPayload    sPayload;
                    
                    sPayload.eDirection           =  au8LinkRxBuffer[6];
                    sPayload.u8Hue                =  au8LinkRxBuffer[5];
                    sPayload.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 );
                    
                    u8Status    =  eCLD_ColourControlCommandMoveToHueCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                  au8LinkRxBuffer [ 4 ],
                                                                                  &sAddress,
                                                                                  &u8SeqNum,
                                                                                  &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_STEP_HUE):
                {
                    tsCLD_ColourControl_StepHueCommandPayload    sPayload;
                    
                    sPayload.eMode           =  au8LinkRxBuffer[5];
                    sPayload.u8StepSize      =  au8LinkRxBuffer[6];
                    sPayload.u8TransitionTime    =  au8LinkRxBuffer[7];
                    
                    u8Status     =  eCLD_ColourControlCommandStepHueCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                 au8LinkRxBuffer [ 4 ],
                                                                                 &sAddress,
                                                                                 &u8SeqNum,
                                                                                 &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_MOVE_TO_SATURATION):
                {
                    tsCLD_ColourControl_MoveToSaturationCommandPayload    sPayload;
                    
                    sPayload.u8Saturation         = au8LinkRxBuffer[5];
                    sPayload.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    
                    u8Status    =  eCLD_ColourControlCommandMoveToSaturationCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                         au8LinkRxBuffer [ 4 ],
                                                                                         &sAddress,
                                                                                         &u8SeqNum,
                                                                                         &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_MOVE_SATURATION):
                {
                    tsCLD_ColourControl_MoveSaturationCommandPayload    sPayload;
                    
                    sPayload.eMode    =  au8LinkRxBuffer[5];
                    sPayload.u8Rate   =  au8LinkRxBuffer[6];
                    
                    u8Status    =  eCLD_ColourControlCommandMoveSaturationCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                       au8LinkRxBuffer [ 4 ],
                                                                                       &sAddress,
                                                                                       &u8SeqNum,
                                                                                       &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_STEP_SATURATION):
                {
                    tsCLD_ColourControl_StepSaturationCommandPayload    sPayload;
                    
                    sPayload.eMode               =  au8LinkRxBuffer[5];
                    sPayload.u8StepSize          =  au8LinkRxBuffer[6];
                    sPayload.u8TransitionTime    =  au8LinkRxBuffer[7];
                    
                    u8Status    =  eCLD_ColourControlCommandStepSaturationCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                       au8LinkRxBuffer [ 4 ],
                                                                                       &sAddress,
                                                                                       &u8SeqNum,
                                                                                       &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_MOVE_TO_COLOUR):
                {
                    tsCLD_ColourControl_MoveToColourCommandPayload    sPayload;
                    
                    sPayload.u16ColourX           =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sPayload.u16ColourY           =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 );
                    sPayload.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 9 );
                    
                    
                    u8Status    =  eCLD_ColourControlCommandMoveToColourCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                     au8LinkRxBuffer [ 4 ],
                                                                                     &sAddress,
                                                                                     &u8SeqNum,
                                                                                     &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_MOVE_COLOUR):
                {
                    tsCLD_ColourControl_MoveColourCommandPayload    sPayload;
                    
                    sPayload.i16RateX    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sPayload.i16RateY    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 ) ;
                    
                    u8Status    =  eCLD_ColourControlCommandMoveColourCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                   au8LinkRxBuffer [ 4 ],
                                                                                   &sAddress,
                                                                                   &u8SeqNum,
                                                                                   &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_STEP_COLOUR):
                {
                    tsCLD_ColourControl_StepColourCommandPayload    sPayload;
                    
                    sPayload.i16StepX             =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sPayload.i16StepY             =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 );
                    sPayload.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 9 );
                    
                    u8Status = eCLD_ColourControlCommandStepColourCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                               au8LinkRxBuffer [ 4 ],
                                                                               &sAddress,
                                                                               &u8SeqNum,
                                                                               &sPayload );
                    
                }
                break;
                
            case (E_SL_MSG_COLOUR_LOOP_SET):
                {
                    tsCLD_ColourControl_ColourLoopSetCommandPayload    sPayload;
                    sPayload.u8UpdateFlags    =  au8LinkRxBuffer [ 5 ];
                    sPayload.eAction      =  au8LinkRxBuffer [ 6 ];
                    sPayload.eDirection       =  au8LinkRxBuffer [ 7 ];
                    sPayload.u16Time      =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    sPayload.u16StartHue      =  ZNC_RTN_U16 ( au8LinkRxBuffer, 10 );
                    
                    u8Status = eCLD_ColourControlCommandColourLoopSetCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                  au8LinkRxBuffer [ 4 ],
                                                                                  &sAddress,
                                                                                  &u8SeqNum,
                                                                                  &sPayload );
                }
                break;
                
            case (E_SL_MSG_MOVE_TO_COLOUR_TEMPERATURE):
                {
                    tsCLD_ColourControl_MoveToColourTemperatureCommandPayload    sPayload;
                    
                    sPayload.u16ColourTemperatureMired    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    sPayload.u16TransitionTime            =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 );
                    u8Status    =  eCLD_ColourControlCommandMoveToColourTemperatureCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                                au8LinkRxBuffer [ 4 ],
                                                                                                &sAddress,
                                                                                                &u8SeqNum,
                                                                                                &sPayload );
                }
                break;
                
            case (E_SL_MSG_MOVE_COLOUR_TEMPERATURE):
                {
                    tsCLD_ColourControl_MoveColourTemperatureCommandPayload    sPayload;
                    
                    sPayload.eMode                           =  au8LinkRxBuffer[5];
                    sPayload.u16Rate                         =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    sPayload.u16ColourTemperatureMiredMin    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    sPayload.u16ColourTemperatureMiredMax    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 10);
                    
                    u8Status    =  eCLD_ColourControlCommandMoveColourTemperatureCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                              au8LinkRxBuffer [ 4 ],
                                                                                              &sAddress,
                                                                                              &u8SeqNum,
                                                                                              &sPayload );
                }
                break;
                
            case (E_SL_MSG_STEP_COLOUR_TEMPERATURE):
                {
                    tsCLD_ColourControl_StepColourTemperatureCommandPayload    sPayload;
                    
                    sPayload.eMode                           =  au8LinkRxBuffer[5];
                    sPayload.u16StepSize                     =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    sPayload.u16TransitionTime               =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8);
                    sPayload.u16ColourTemperatureMiredMin    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 10 );
                    sPayload.u16ColourTemperatureMiredMax    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 12);
                    sPayload.u8OptionsMask                   =  au8LinkRxBuffer[14];
                    sPayload.u8OptionsOverride               =  au8LinkRxBuffer[15];
                    
                    u8Status    =  eCLD_ColourControlCommandStepColourTemperatureCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                              au8LinkRxBuffer [ 4 ],
                                                                                              &sAddress,
                                                                                              &u8SeqNum,
                                                                                              &sPayload );
                }
                break;
                
                
            case (E_SL_MSG_ENHANCED_MOVE_TO_HUE):
                {
                    tsCLD_ColourControl_EnhancedMoveToHueCommandPayload    sPayload;
                    
                    sPayload.eDirection           =  au8LinkRxBuffer[5];
                    sPayload.u16EnhancedHue       =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    sPayload.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    
                    u8Status    =  eCLD_ColourControlCommandEnhancedMoveToHueCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                          au8LinkRxBuffer [ 4 ],
                                                                                          &sAddress,
                                                                                          &u8SeqNum,
                                                                                          &sPayload );
                }
                break;
                
            case (E_SL_MSG_ENHANCED_MOVE_HUE):
                {
                    tsCLD_ColourControl_EnhancedMoveHueCommandPayload    sPayload;
                    
                    sPayload.eMode      =  au8LinkRxBuffer[5];
                    sPayload.u16Rate    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    
                    u8Status    =  eCLD_ColourControlCommandEnhancedMoveHueCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                        au8LinkRxBuffer [ 4 ],
                                                                                        &sAddress,
                                                                                        &u8SeqNum,
                                                                                        &sPayload );
                }
                break;
                
            case (E_SL_MSG_ENHANCED_STEP_HUE):
                {
                    tsCLD_ColourControl_EnhancedStepHueCommandPayload    sPayload;
                    
                    sPayload.eMode                = au8LinkRxBuffer[5];
                    sPayload.u16StepSize          =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    sPayload.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    
                    u8Status    =  eCLD_ColourControlCommandEnhancedStepHueCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                        au8LinkRxBuffer [ 4 ],
                                                                                        &sAddress,
                                                                                        &u8SeqNum,
                                                                                        &sPayload );
                }
                break;
                
                
                
            case (E_SL_MSG_ENHANCED_MOVE_TO_HUE_SATURATION):
                {
                    tsCLD_ColourControl_EnhancedMoveToHueAndSaturationCommandPayload    sPayload;
                    
                    sPayload.u8Saturation         =  au8LinkRxBuffer[5];
                    sPayload.u16EnhancedHue       =  ZNC_RTN_U16 ( au8LinkRxBuffer, 6 );
                    sPayload.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    
                    u8Status    =  eCLD_ColourControlCommandEnhancedMoveToHueAndSaturationCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                                       au8LinkRxBuffer [ 4 ],
                                                                                                       &sAddress,
                                                                                                       &u8SeqNum,
                                                                                                       &sPayload );
                }
                break;
                
                
            case (E_SL_MSG_STOP_MOVE_STEP):
                {
                    tsCLD_ColourControl_StopMoveStepCommandPayload    sPayload;
                    
                    sPayload.u8OptionsMask        =  au8LinkRxBuffer [ 5 ];
                    sPayload.u8OptionsOverride    =  au8LinkRxBuffer [ 6 ];
                    u8Status    =  eCLD_ColourControlCommandStopMoveStepCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                                     au8LinkRxBuffer [ 4 ],
                                                                                     &sAddress,
                                                                                     &u8SeqNum,
                                                                                     &sPayload );
                }
                break;
                
                /* level cluster commands */
            case (E_SL_MSG_MOVE_TO_LEVEL_ONOFF):
                {
                    tsCLD_LevelControl_MoveToLevelCommandPayload    sCommand;
                    
                    sCommand.u8Level          =  au8LinkRxBuffer [ 6 ];
                    sCommand.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 );
                    u8Status = eCLD_LevelControlCommandMoveToLevelCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                               au8LinkRxBuffer [ 4 ],
                                                                               &sAddress,
                                                                               &u8SeqNum,
                                                                               au8LinkRxBuffer [ 5 ],
                                                                               &sCommand);
                }
                break;
                
            case (E_SL_MSG_MOVE_TO_LEVEL):
                {
                    tsCLD_LevelControl_MoveCommandPayload    sCommand;
                    
                    sCommand.u8MoveMode     =  au8LinkRxBuffer[6];
                    sCommand.u8Rate         =  au8LinkRxBuffer[7];
                    u8Status    =  eCLD_LevelControlCommandMoveCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                            au8LinkRxBuffer [ 4 ],
                                                                            &sAddress,
                                                                            &u8SeqNum,
                                                                            au8LinkRxBuffer [ 5 ],
                                                                            &sCommand );
                }
                break;
                
                
            case (E_SL_MSG_MOVE_STEP):
                {
                    tsCLD_LevelControl_StepCommandPayload     sCommand;
                    
                    sCommand.u8StepMode           =  au8LinkRxBuffer[6];
                    sCommand.u8StepSize           =  au8LinkRxBuffer[7];
                    sCommand.u16TransitionTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 8 );
                    u8Status    =  eCLD_LevelControlCommandStepCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                            au8LinkRxBuffer [ 4 ],
                                                                            &sAddress,
                                                                            &u8SeqNum,
                                                                            au8LinkRxBuffer [ 5 ],
                                                                            &sCommand );
                }
                break;
                
                
            case (E_SL_MSG_MOVE_STOP_ONOFF):
                {
                    tsCLD_LevelControl_StopCommandPayload    sPayload;
                    
                    sPayload.u8OptionsMask        =  au8LinkRxBuffer [ 6 ];
                    sPayload.u8OptionsOverride    =  au8LinkRxBuffer [ 7 ];
                    u8Status = eCLD_LevelControlCommandStopCommandSend ( au8LinkRxBuffer [ 3 ],
                                                                        au8LinkRxBuffer [ 4 ],
                                                                        &sAddress,
                                                                        &u8SeqNum,
                                                                        au8LinkRxBuffer [ 5 ],
                                                                        &sPayload );
                }
                break;
                
                /* Identify commands*/
                
            case (E_SL_MSG_IDENTIFY_SEND):
                {
                    tsCLD_Identify_IdentifyRequestPayload    sCommand;
                    
                    sCommand.u16IdentifyTime    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u8Status    =  eCLD_IdentifyCommandIdentifyRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                            au8LinkRxBuffer [ 4 ],
                                                                            &sAddress,
                                                                            &u8SeqNum,
                                                                            &sCommand );
                }
                break;
                
            case (E_SL_MSG_IDENTIFY_QUERY):
                {
                    
                    u8Status    =  eCLD_IdentifyCommandIdentifyQueryRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                                 au8LinkRxBuffer [ 4 ],
                                                                                 &sAddress,
                                                                                 &u8SeqNum );
                }
                break;
                
#ifdef  CLD_IDENTIFY_SUPPORT_ZLL_ENHANCED_COMMANDS
            case (E_SL_MSG_IDENTIFY_TRIGGER_EFFECT):
                {
                    u8Status = eCLD_IdentifyCommandTriggerEffectSend ( au8LinkRxBuffer [ 3 ],
                                                                      au8LinkRxBuffer [ 4 ],
                                                                      &sAddress,
                                                                      &u8SeqNum,
                                                                      au8LinkRxBuffer [ 5 ],
                                                                      au8LinkRxBuffer [ 6 ]);
                }
                break;
#endif
                /* profile agnostic commands */
            case (E_SL_MSG_READ_ATTRIBUTE_REQUEST):
                {
                    uint16    au16AttributeList[10];
                    uint16    u16ClusterId;
                    uint16    u16ManId;
                    uint8     i = 0;
                    
                    
                    u16ClusterId    =  ZNC_RTN_U16 (au8LinkRxBuffer, 5 );
                    u16ManId    =  ZNC_RTN_U16 (au8LinkRxBuffer, 9 );
                    
                    while ( ( i < 10 ) &&
                           ( i < au8LinkRxBuffer[11] )
                               )
                    {
                        au16AttributeList [ i ]    =  ZNC_RTN_U16 ( au8LinkRxBuffer , ( 12 + (i * 2) ) );
                        i++;
                    }
                    
                    u8Status    =  eZCL_SendReadAttributesRequest ( au8LinkRxBuffer [ 3 ],
                                                                   au8LinkRxBuffer [ 4 ],
                                                                   u16ClusterId,
                                                                   au8LinkRxBuffer [ 7 ],
                                                                   &sAddress,
                                                                   &u8SeqNum,
                                                                   au8LinkRxBuffer [ 11 ],
                                                                   au8LinkRxBuffer [ 8 ],
                                                                   u16ManId,
                                                                   au16AttributeList );
                }
                break;
                
            case (E_SL_MSG_WRITE_ATTRIBUTE_REQUEST):
                {
                    uint16    u16ClusterId;
                    uint16    u16ManId;
                    uint16    u16SizePayload;
                    
                    u16ClusterId      =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u16ManId          =  ZNC_RTN_U16 ( au8LinkRxBuffer, 9 );
                    
                    /* payload - sum of add mode , short addr, cluster id, manf id, manf specific flag */
                    /* src ep,  dest ep, num attrib , direction*/
                    u16SizePayload    =  u16PacketLength - ( 12 ) ;
                    u8Status          =  APP_eSendWriteAttributesRequest ( au8LinkRxBuffer [ 3 ],
                                                                          au8LinkRxBuffer [ 4 ],
                                                                          u16ClusterId,
                                                                          au8LinkRxBuffer [ 7 ],
                                                                          &sAddress,
                                                                          &u8SeqNum,
                                                                          au8LinkRxBuffer [ 8 ],
                                                                          u16ManId,
                                                                          &au8LinkRxBuffer [ 12 ],
                                                                          au8LinkRxBuffer [ 11 ],
                                                                          u16SizePayload );
                }
                break;
                
                
            case E_SL_MSG_CONFIG_REPORTING_REQUEST:
                {
                    uint16                                         u16ClusterId;
                    uint16                                         u16ManId;
                    tsZCL_AttributeReportingConfigurationRecord    asAttribReportConfigRecord[10];
                    int                                            i;
                    uint8                                          u8Offset = 12;
                    
                    u16ClusterId      =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u16ManId          =  ZNC_RTN_U16 ( au8LinkRxBuffer, 9 );
                    
                    
                    for (i = 0; i < au8LinkRxBuffer[11]; i++)
                    {
                        if (i < 10)
                        {
                            /* Destination structure is not packed so we have to manually load rather than just copy */
                            asAttribReportConfigRecord [ i ].u8DirectionIsReceived          =  au8LinkRxBuffer [ u8Offset++ ];
                            asAttribReportConfigRecord [ i ].eAttributeDataType             =  au8LinkRxBuffer [ u8Offset++ ];
                            asAttribReportConfigRecord [ i ].u16AttributeEnum               =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer,
                                                                                                                   u8Offset,
                                                                                                                   u8Offset );
                            asAttribReportConfigRecord [ i ].u16MinimumReportingInterval    =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer,
                                                                                                                   u8Offset,
                                                                                                                   u8Offset );
                            asAttribReportConfigRecord [ i ].u16MaximumReportingInterval    =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer,
                                                                                                                   u8Offset,
                                                                                                                   u8Offset );
                            asAttribReportConfigRecord [ i ].u16TimeoutPeriodField          =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer,
                                                                                                                   u8Offset,
                                                                                                                   u8Offset );
                            asAttribReportConfigRecord[i].uAttributeReportableChange.zuint8ReportableChange = au8LinkRxBuffer [u8Offset++ ];
                        }
                    }
                    
                    u8Status =  eZCL_SendConfigureReportingCommand(au8LinkRxBuffer [ 3 ],           // u8SourceEndPointId
                                                                   au8LinkRxBuffer [ 4 ],           // u8DestinationEndPointId
                                                                   u16ClusterId,                    // u16ClusterId
                                                                   au8LinkRxBuffer [ 7 ],           // bDirectionIsServerToClient
                                                                   &sAddress,                       // *psDestinationAddress
                                                                   &u8SeqNum,                       // *pu8TransactionSequenceNumber
                                                                   au8LinkRxBuffer [ 11 ],          // u8NumberOfAttributesInRequest
                                                                   au8LinkRxBuffer [ 8 ],           // bIsManufacturerSpecific
                                                                   u16ManId,                        // u16ManufacturerCode
                                                                   asAttribReportConfigRecord );    // *psAttributeReportingConfigurationRecord
                }
                break;
                
            case E_SL_MSG_ATTRIBUTE_DISCOVERY_REQUEST:
            case E_SL_MSG_ATTRIBUTE_EXT_DISCOVERY_REQUEST:
                {
                    bool_t     bIsExtReq = FALSE;
                    uint16     u16ClusterId;
                    uint16     u16AttributeId;
                    uint16     u16ManufacturerCode;
                    
                    if (E_SL_MSG_ATTRIBUTE_EXT_DISCOVERY_REQUEST == u16PacketType)
                    {
                        bIsExtReq = TRUE;
                    }
                    
                    u16ClusterId       =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u16AttributeId     =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7 );
                    u16ManufacturerCode    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 11 );
                    
                    u8Status = APP_eZclDiscoverAttributes(bIsExtReq,
                                                          au8LinkRxBuffer[3],      // u8SourceEndPointId
                                                          au8LinkRxBuffer[4],      // u8DestinationEndPointId
                                                          u16ClusterId,            // u16ClusterId
                                                          au8LinkRxBuffer[9],      // bDirectionIsServerToClient
                                                          &sAddress,               // *psDestinationAddress
                                                          &u8SeqNum,               // *pu8TransactionSequenceNumber
                                                          u16AttributeId,          // u16AttributeId
                                                          au8LinkRxBuffer[10],     // bIsManufacturerSpecific
                                                          u16ManufacturerCode,     // u16ManufacturerCode
                                                          au8LinkRxBuffer[13]);    // u8MaximumNumberOfIdentifiers
                }
                break;
                
            case E_SL_MSG_COMMAND_GENERATED_DISCOVERY_REQUEST:
                {
                    uint16    u16ClusterId;
                    uint16    u16ManufacturerCode;
                    
                    u16ClusterId       =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u16ManufacturerCode    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 10 );
                    
                    u8Status = APP_eZclDiscoverCommandGenerated ( au8LinkRxBuffer [ 3 ],      // u8SourceEndPointId
                                                                 au8LinkRxBuffer [ 4 ],      // u8DestinationEndPointId
                                                                 u16ClusterId,               // u16ClusterId
                                                                 au8LinkRxBuffer [ 7 ],      // bDirectionIsServerToClient
                                                                 &sAddress,                  // *psDestinationAddress
                                                                 &u8SeqNum,                  // *pu8TransactionSequenceNumber
                                                                 au8LinkRxBuffer [ 8 ],      // u8CommandId
                                                                 au8LinkRxBuffer [ 9 ],      // bIsManufacturerSpecific
                                                                 u16ManufacturerCode,        // u16ManufacturerCode
                                                                 au8LinkRxBuffer [ 12 ] );   // u8MaximumNumberOfCommands
                }
                break;
                
            case E_SL_MSG_COMMAND_RECEIVED_DISCOVERY_REQUEST:
                {
                    uint16    u16ClusterId;
                    uint16    u16ManufacturerCode;
                    
                    u16ClusterId           =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5 );
                    u16ManufacturerCode    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 10 );
                    
                    u8Status = APP_eZclDiscoverCommandReceived ( au8LinkRxBuffer [ 3 ],     // u8SourceEndPointId
                                                                au8LinkRxBuffer [ 4 ],     // u8DestinationEndPointId
                                                                u16ClusterId,              // u16ClusterId
                                                                au8LinkRxBuffer [ 7 ],     // bDirectionIsServerToClient
                                                                &sAddress,                 // *psDestinationAddress
                                                                &u8SeqNum,                 // *pu8TransactionSequenceNumber
                                                                au8LinkRxBuffer [ 8 ],     // u8CommandId
                                                                au8LinkRxBuffer [ 9 ],     // bIsManufacturerSpecific
                                                                u16ManufacturerCode,       // u16ManufacturerCode
                                                                au8LinkRxBuffer [ 12 ] );  // u8MaximumNumberOfCommands
                }
                break;
            case E_SL_MSG_READ_REPORT_CONFIG_REQUEST:
                {
                    uint8                                               i;
                    uint8                                               u8NumberOfAttributesInRequest;
                    uint16                                              u16ClusterId;
                    uint16                                              u16ManufacturerCode;
                    tsZCL_AttributeReadReportingConfigurationRecord     asAttributeReadReportingConfigurationRecord[8];
                    uint8                                               u8BufferOffset = 12;
                    
                    u8NumberOfAttributesInRequest    =  au8LinkRxBuffer[8];
                    u16ClusterId                     =  ZNC_RTN_U16 ( au8LinkRxBuffer, 5  );
                    u16ManufacturerCode              =  ZNC_RTN_U16 ( au8LinkRxBuffer, 10 );
                    
                    
                    for (i = 0; i < u8NumberOfAttributesInRequest; i++)
                    {
                        asAttributeReadReportingConfigurationRecord[i].u8DirectionIsReceived     =  au8LinkRxBuffer[u8BufferOffset++];
                        asAttributeReadReportingConfigurationRecord[i].u16AttributeEnum          =  ZNC_RTN_U16_OFFSET ( au8LinkRxBuffer,
                                                                                                                        u8BufferOffset,
                                                                                                                        u8BufferOffset);
                    }
                    
                    u8Status    =  eZCL_SendReadReportingConfigurationCommand ( au8LinkRxBuffer [ 3 ],                                //  u8SourceEndPointId,
                                                                               au8LinkRxBuffer [ 4 ],                                //  u8DestinationEndPointId,
                                                                               u16ClusterId,                                         //  u16ClusterId,
                                                                               au8LinkRxBuffer [ 7 ],                                //  bDirectionIsServerToClient,
                                                                               &sAddress,                                            // *psDestinationAddress,
                                                                               &u8SeqNum,                                            // *pu8TransactionSequenceNumber,
                                                                               u8NumberOfAttributesInRequest,                        //  u8NumberOfAttributesInRequest,
                                                                               au8LinkRxBuffer [ 9 ],                                //  bIsManufacturerSpecific,
                                                                               u16ManufacturerCode,                                  //  u16ManufacturerCode,
                                                                               &asAttributeReadReportingConfigurationRecord[0] );    //  *psAttributeReadReportingConfigurationRecord);
                }
                break;
                
            case E_SL_MSG_SEND_IAS_ZONE_ENROLL_RSP:
                {
                    tsCLD_IASZone_EnrollResponsePayload    sEnrollResponsePayload;
                    
                    sEnrollResponsePayload.e8EnrollResponseCode    =  au8LinkRxBuffer[5];
                    sEnrollResponsePayload.u8ZoneID                =  au8LinkRxBuffer[6];
                    
                    u8Status    = eCLD_IASZoneEnrollRespSend ( au8LinkRxBuffer [ 3 ],         // u8SourceEndPointId,
                                                              au8LinkRxBuffer [ 4 ],         // u8DestinationEndPointId,
                                                              &sAddress,                     // *psDestinationAddress,
                                                              &u8SeqNum,                     // *pu8TransactionSequenceNumber,
                                                              &sEnrollResponsePayload );     // *psPayload);
                }
                break;
                
#ifdef CLD_DOOR_LOCK
            case (E_SL_MSG_LOCK_UNLOCK_DOOR):
                {
                    u8Status     =  eCLD_DoorLockCommandLockUnlockRequestSend ( au8LinkRxBuffer [ 3 ],
                                                                               au8LinkRxBuffer [ 4 ],
                                                                               &sAddress,
                                                                               &u8SeqNum,
                                                                               au8LinkRxBuffer [ 5 ] );
                }
                break;
#endif
                
#ifdef CLD_ASC_LOG
            case E_SL_MSG_ASC_LOG_MSG:
                {
                    tsCLD_ASC_LogNotificationORLogResponsePayload    sNotificationPayload;
                    
                    sNotificationPayload.utctTime              =  ZNC_RTN_U32 ( au8LinkRxBuffer, 5 );
                    sNotificationPayload.u32LogId              =  ZNC_RTN_U32 ( au8LinkRxBuffer, 9 );
                    sNotificationPayload.u32LogLength          =  ZNC_RTN_U32 ( au8LinkRxBuffer, 13);
                    sNotificationPayload.pu8LogData            =  &au8LinkRxBuffer[17];
                    u8Status =  eCLD_ASCLogNotificationSend ( au8LinkRxBuffer [ 3 ],     // u8SourceEndPointId,
                                                             au8LinkRxBuffer [ 4 ],     // u8DestinationEndPointId,
                                                             &sAddress,                 // *psDestinationAddress,
                                                             &u8SeqNum,                 // *pu8TransactionSequenceNumber,
                                                             &sNotificationPayload);    // *psPayload);)
                }
                break;
#endif
                
#ifdef NETWORK_RECOVERY
            case (E_SL_MSG_NWK_RECOVERY_EXTRACT_REQ):
                {
                    tsNwkRecovery    sNwkRecovery = { { 0 } };
                    vNetworkRecoveryObtainRecoverData ( &sNwkRecovery );
                    vSL_WriteMessage( E_SL_MSG_NWK_RECOVERY_EXTRACT_RSP,
                                     sizeof(sNwkRecovery),
                                     (uint8 *)&sNwkRecovery );
                }
                break;
                
            case (E_SL_MSG_NWK_RECOVERY_RESTORE_REQ):
                {
                    uint8 u8Success = 0;
                    vNetworkRecoveryInsertRecoverData( ( tsNwkRecovery * ) &au8LinkRxBuffer );
                    vSL_WriteMessage( E_SL_MSG_NWK_RECOVERY_RESTORE_RSP,
                                     sizeof(uint8),
                                     &u8Success );
                }
                break;
#endif
            case E_SL_MSG_BASIC_RESET_TO_FACTORY_DEFAULTS:
                {
                    u8Status    =  APP_eZclBasicResetToFactoryDefaults( au8LinkRxBuffer[3],      //  u8SourceEndPointId
                                                                       au8LinkRxBuffer[4],      //  u8DestinationEndPointId
                                                                       &sAddress,               // *psDestinationAddress,
                                                                       &u8SeqNum );             // *pu8TransactionSequenceNumber
                }
                break;

#ifdef CLD_OTA
            case E_SL_MSG_IMAGE_NOTIFY:
                {
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nE_SL_MSG_IMAGE_NOTIFY" );
                    
                    uint8               u8SrcEndPoint;
                    uint8               u8DstEndPoint;
                    tsOTA_ImageNotifyCommand    sImageNotifyCommand;
                    
                    u8SrcEndPoint                              =  au8LinkRxBuffer[3];
                    u8DstEndPoint                              =  au8LinkRxBuffer[4];
                    sImageNotifyCommand.ePayloadType           =  au8LinkRxBuffer[5];
                    sImageNotifyCommand.u32NewFileVersion      =  ZNC_RTN_U32 ( au8LinkRxBuffer, 6  );
                    sImageNotifyCommand.u16ImageType           =  ZNC_RTN_U16 ( au8LinkRxBuffer, 10 );
                    sImageNotifyCommand.u16ManufacturerCode    =  ZNC_RTN_U16 ( au8LinkRxBuffer, 12 );
                    sImageNotifyCommand.u8QueryJitter          =  au8LinkRxBuffer[14];
                    
                    u8Status    = eOTA_ServerImageNotify ( u8SrcEndPoint,          // u8SourceEndpoint
                                                          u8DstEndPoint,         // u8DestinationEndpoint
                                                          &sAddress,          // *psDestinationAddress
                                                          &sImageNotifyCommand );     // *psImageNotifyCommand
                }
                break;
                
            case E_SL_MSG_UPGRADE_END_RESPONSE:
                {
                    uint8                              u8SrcEndPoint;
                    uint8                              u8DstEndPoint;
                    tsOTA_UpgradeEndResponsePayload    sUpgradeResponsePayload;
                    
                    u8SrcEndPoint                  =  au8LinkRxBuffer[3];
                    u8DstEndPoint                  =  au8LinkRxBuffer[4];
                    sUpgradeResponsePayload.u32UpgradeTime         =  ZNC_RTN_U32 ( au8LinkRxBuffer,  6  );
                    sUpgradeResponsePayload.u32CurrentTime         =  ZNC_RTN_U32 ( au8LinkRxBuffer,  10 );
                    sUpgradeResponsePayload.u32FileVersion         =  ZNC_RTN_U32 ( au8LinkRxBuffer,  14 );
                    sUpgradeResponsePayload.u16ImageType           =  ZNC_RTN_U16 ( au8LinkRxBuffer,  18 );
                    sUpgradeResponsePayload.u16ManufacturerCode    =  ZNC_RTN_U16 ( au8LinkRxBuffer,  20 );
                    
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nE_SL_MSG_UPGRADE_END_RESPONSE");
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nAddr Mode: %x", sAddress.eAddressMode);
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nAddr: %x", sAddress.uAddress.u16DestinationAddress);
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nSrcEndPoint: %x", u8SrcEndPoint);
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nDstEndPoint: %x", u8DstEndPoint);
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nUpgradeTime: %x", sUpgradeResponsePayload.u32UpgradeTime);
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nCurrentTime: %x", sUpgradeResponsePayload.u32CurrentTime);
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nFileVersion: %x", sUpgradeResponsePayload.u32FileVersion);
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nImageType: %x", sUpgradeResponsePayload.u16ImageType);
                    vLog_Printf (TRACE_APP, LOG_DEBUG, "\nManufacturerCode: %x", sUpgradeResponsePayload.u16ManufacturerCode);
                    
                    u8Status    = eOTA_ServerUpgradeEndResponse(u8SrcEndPoint,                //  u8SourceEndpoint
                                                                u8DstEndPoint,                //  u8DestinationEndpoint
                                                                &sAddress,                    // *psDestinationAddress,
                                                                &sUpgradeResponsePayload,     // *psUpgradeResponsePayload,
                                                                au8LinkRxBuffer[5]);          // uint8 u8TransactionSequenceNumber);
                }
                break;
                
            case E_SL_MSG_BLOCK_SEND:
                {
                    uint8                              u8SrcEndPoint;
                    uint8                              u8DstEndPoint;
                    tsOTA_ImageBlockResponsePayload    sImageBlockResponsePayload;
                    
                    u8SrcEndPoint                                                                    =  au8LinkRxBuffer[3];
                    u8DstEndPoint                                                                    =  au8LinkRxBuffer[4];
                    sImageBlockResponsePayload.u8Status                                              =  au8LinkRxBuffer[6];
                    sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u32FileOffset           =  ZNC_RTN_U32 ( au8LinkRxBuffer, 7  );
                    sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u32FileVersion          =  ZNC_RTN_U32 ( au8LinkRxBuffer, 11 );
                    sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u16ImageType            =  ZNC_RTN_U16 ( au8LinkRxBuffer, 15 );
                    sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u16ManufacturerCode     =  ZNC_RTN_U16 ( au8LinkRxBuffer, 17 );
                    sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u8DataSize              =  au8LinkRxBuffer[19];
                    sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.pu8Data                 =  &au8LinkRxBuffer[20];
                    
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nE_SL_MSG_BLOCK_SEND");
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nAddr Mode: %x", sAddress.eAddressMode);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nAddr: %x"     , sAddress.uAddress.u16DestinationAddress);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nSrcEndPoint: %x", u8SrcEndPoint);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nDstEndPoint: %x", u8DstEndPoint);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nStatus: %x", sImageBlockResponsePayload.u8Status);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nFileOffset: %x", sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u32FileOffset);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nFileVersion: %x", sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u32FileVersion);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nImageType: %x", sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u16ImageType);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nManufacturerCode: %x", sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u16ManufacturerCode);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nDataSize: %x", sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u8DataSize);
                    
                    u8Status    = eOTA_ServerImageBlockResponse( u8SrcEndPoint,                                                          //  u8SourceEndpoint
                                                                u8DstEndPoint,                                                          //  u8DestinationEndpoint
                                                                &sAddress,                                                              // *psDestinationAddress
                                                                &sImageBlockResponsePayload,                                            // *psImageBlockResponsePayload
                                                                sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u8DataSize,    //    u8BlockSize
                                                                au8LinkRxBuffer[5] );                                                   //  u8TransactionSequenceNumber
                }
                break;
                
            case E_SL_MSG_LOAD_NEW_IMAGE:
                {
                    tsOTA_CoProcessorOTAHeader    sCoProcessorOTAHeader;
                    uint8                         i = 0;
                    
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u32FileIdentifier       =  ZNC_RTN_U32 ( au8LinkRxBuffer, 3  );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16HeaderVersion        =  ZNC_RTN_U16 ( au8LinkRxBuffer, 7  );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16HeaderLength         =  ZNC_RTN_U16 ( au8LinkRxBuffer, 9  );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16HeaderControlField   =  ZNC_RTN_U16 ( au8LinkRxBuffer, 11 );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16ManufacturerCode     =  ZNC_RTN_U16 ( au8LinkRxBuffer, 13 );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16ImageType            =  ZNC_RTN_U16 ( au8LinkRxBuffer, 15 );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u32FileVersion          =  ZNC_RTN_U32 ( au8LinkRxBuffer, 17 );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16StackVersion         =  ZNC_RTN_U16 ( au8LinkRxBuffer, 21 );
                    do
                    {
                        sCoProcessorOTAHeader.sOTA_ImageHeader[0].stHeaderString [ i ]    = au8LinkRxBuffer[ 23 + i ];
                        i++;
                        
                    }
                    while(i < 32);
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u32TotalImage           =  ZNC_RTN_U32 ( au8LinkRxBuffer, 55 );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u8SecurityCredVersion   =  au8LinkRxBuffer[59] ;
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u64UpgradeFileDest      =  ZNC_RTN_U64 ( au8LinkRxBuffer, 60 );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16MinimumHwVersion     =  ZNC_RTN_U16 ( au8LinkRxBuffer, 68 );
                    sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16MaxHwVersion         =  ZNC_RTN_U16 ( au8LinkRxBuffer, 70 );
                    
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nE_SL_MSG_LOAD_NEW_IMAGE");
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nFileIdentifier: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u32FileIdentifier);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nHeaderVersion: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16HeaderVersion);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nHeaderLength: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16HeaderLength);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nHeaderControlField: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16HeaderControlField);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nManufacturerCode: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16ManufacturerCode);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nImageType: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16ImageType);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nFileVersion: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u32FileVersion);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nStackVersion: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16StackVersion);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nHeaderString: %s", sCoProcessorOTAHeader.sOTA_ImageHeader[0].stHeaderString);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nTotalImage: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u32TotalImage);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nSecurityCredVersion: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u8SecurityCredVersion);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nUpgradeFileDest: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u64UpgradeFileDest);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nMinimumHwVersion: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16MinimumHwVersion);
                    vLog_Printf ( TRACE_APP, LOG_DEBUG, "\nMaxHwVersion: %x", sCoProcessorOTAHeader.sOTA_ImageHeader[0].u16MaxHwVersion);
                    
                    u8Status    =  eOTA_NewImageLoaded(ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT, TRUE, &sCoProcessorOTAHeader);
                }
                break;
                
            case E_SL_MSG_SEND_WAIT_FOR_DATA_PARAMS:
                {
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nE_SL_MSG_SEND_WAIT_FOR_DATA_PARAMS");
                    
                    uint8                              u8SrcEndPoint;
                    uint8                              u8DstEndPoint;
                    tsOTA_ImageBlockResponsePayload    sImageBlockResponsePayload;
                    
                    u8SrcEndPoint                                                           =  au8LinkRxBuffer[3];
                    u8DstEndPoint                                                           =  au8LinkRxBuffer[4];
                    sImageBlockResponsePayload.u8Status                                     =  au8LinkRxBuffer[6];
                    
                    sImageBlockResponsePayload.uMessage.sWaitForData.u32CurrentTime         =  ZNC_RTN_U32 ( au8LinkRxBuffer, 7  );
                    sImageBlockResponsePayload.uMessage.sWaitForData.u32RequestTime         =  ZNC_RTN_U32 ( au8LinkRxBuffer, 11  );
                    sImageBlockResponsePayload.uMessage.sWaitForData.u16BlockRequestDelayMs =  ZNC_RTN_U16 ( au8LinkRxBuffer, 15  );
                    
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nAddr Mode: %x", sAddress.eAddressMode);
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nAddr: %x", sAddress.uAddress.u16DestinationAddress);
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nSrcEndPoint: %x", u8SrcEndPoint);
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nDstEndPoint: %x", u8DstEndPoint);
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nStatus: %x", sImageBlockResponsePayload.u8Status);
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nCurrentTime: %x", sImageBlockResponsePayload.uMessage.sWaitForData.u32CurrentTime);
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nRequestTime: %x", sImageBlockResponsePayload.uMessage.sWaitForData.u32RequestTime);
                    vLog_Printf(TRACE_APP, LOG_DEBUG, "\nBlockDelay: %x", sImageBlockResponsePayload.uMessage.sWaitForData.u16BlockRequestDelayMs);
                    
                    u8Status = eOTA_ServerImageBlockResponse( u8SrcEndPoint,                    /* u8SourceEndpoint */
                                                             u8DstEndPoint,                    /*  u8DestinationEndpoint */
                                                             &sAddress,                        /*  *psDestinationAddress */
                                                             &sImageBlockResponsePayload,      /* *psImageBlockResponsePayload */
                                                             0,                                /*  u8BlockSize           */
                                                             au8LinkRxBuffer[5]);              /*  u8TransactionSequenceNumb */
                }
                break;
                
                
#endif

            default:
                u8Status = E_SL_MSG_STATUS_UNHANDLED_COMMAND;
                break;
            }
        }

#if gFsciIncluded_c
        MEM_BufferFree(pMsg);
#endif
        u8Length    =  0;
        ZNC_BUF_U8_UPD  ( &au8values [ 0 ], u8Status,      u8Length );
        ZNC_BUF_U8_UPD  ( &au8values [ 1 ], u8SeqNum,      u8Length );
        ZNC_BUF_U16_UPD ( &au8values [ 2 ], u16PacketType, u8Length );
        vSL_WriteMessage ( E_SL_MSG_STATUS,
                           u8Length,
                           au8values );
    }

}

/****************************************************************************
 *
 * NAME: APP_eZclBasicResetToFactoryDefaults
 *
 * DESCRIPTION:
 *
 ****************************************************************************/
PRIVATE teZCL_Status APP_eZclBasicResetToFactoryDefaults ( uint8            u8SourceEndPointId,
                                                           uint8            u8DestinationEndPointId,
                                                           tsZCL_Address    *psDestinationAddress,
                                                           uint8            *pu8TransactionSequenceNumber )
{
    teZCL_Status    eStatus;

    eStatus =  eCLD_BasicCommandResetToFactoryDefaultsSend ( u8SourceEndPointId,
                                                             u8DestinationEndPointId,
                                                             psDestinationAddress,
                                                             pu8TransactionSequenceNumber );
    return eStatus;
}

/****************************************************************************
 *
 * NAME: APP_eZclDiscoverCommandGenerated
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE teZCL_Status APP_eZclDiscoverCommandGenerated ( uint8            u8SourceEndPointId,
                                                        uint8            u8DestinationEndPointId,
                                                        uint16           u16ClusterId,
                                                        bool_t           bDirectionIsServerToClient,
                                                        tsZCL_Address    *psDestinationAddress,
                                                        uint8            *pu8TransactionSequenceNumber,
                                                        uint8            u8CommandId,
                                                        bool_t           bIsManufacturerSpecific,
                                                        uint16           u16ManufacturerCode,
                                                        uint8            u8MaximumNumberOfCommands)
{
    teZCL_Status    eStatus;

    eStatus =  eZCL_SendDiscoverCommandGeneratedRequest ( u8SourceEndPointId,
                                                          u8DestinationEndPointId,
                                                          u16ClusterId,
                                                          bDirectionIsServerToClient,
                                                          psDestinationAddress,
                                                          pu8TransactionSequenceNumber,
                                                          u8CommandId,
                                                          bIsManufacturerSpecific,
                                                          u16ManufacturerCode,
                                                          u8MaximumNumberOfCommands );
    return eStatus;
}

/****************************************************************************
 *
 * NAME: APP_eZclDiscoverCommands
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE teZCL_Status APP_eZclDiscoverCommandReceived ( uint8            u8SourceEndPointId,
                                                       uint8            u8DestinationEndPointId,
                                                       uint16           u16ClusterId,
                                                       bool_t           bDirectionIsServerToClient,
                                                       tsZCL_Address*   psDestinationAddress,
                                                       uint8*           pu8TransactionSequenceNumber,
                                                       uint8            u8CommandId,
                                                       bool_t           bIsManufacturerSpecific,
                                                       uint16           u16ManufacturerCode,
                                                       uint8            u8MaximumNumberOfCommands )
{
    teZCL_Status    eStatus;

    eStatus =  eZCL_SendDiscoverCommandReceivedRequest ( u8SourceEndPointId,
                                                         u8DestinationEndPointId,
                                                         u16ClusterId,
                                                         bDirectionIsServerToClient,
                                                         psDestinationAddress,
                                                         pu8TransactionSequenceNumber,
                                                         u8CommandId,
                                                         bIsManufacturerSpecific,
                                                         u16ManufacturerCode,
                                                         u8MaximumNumberOfCommands );
    return eStatus;
}

/****************************************************************************
 *
 * NAME: APP_eZclDiscoverAttributes
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE teZCL_Status APP_eZclDiscoverAttributes ( bool              bIsExtReq,
                                                  uint8             u8SourceEndPointId,
                                                  uint8             u8DestinationEndPointId,
                                                  uint16            u16ClusterId,
                                                  bool_t            bDirectionIsServerToClient,
                                                  tsZCL_Address*    psDestinationAddress,
                                                  uint8 *           pu8TransactionSequenceNumber,
                                                  uint16            u16AttributeId,
                                                  bool_t            bIsManufacturerSpecific,
                                                  uint16            u16ManufacturerCode,
                                                  uint8             u8MaximumNumberOfIdentifiers)
{
    teZCL_Status    eStatus;

    if (TRUE == bIsExtReq)
    {
        eStatus =  eZCL_SendDiscoverAttributesExtendedRequest ( u8SourceEndPointId,
                                                                u8DestinationEndPointId,
                                                                u16ClusterId,
                                                                bDirectionIsServerToClient,
                                                                psDestinationAddress,
                                                                pu8TransactionSequenceNumber,
                                                                u16AttributeId,
                                                                bIsManufacturerSpecific,
                                                                u16ManufacturerCode,
                                                                u8MaximumNumberOfIdentifiers );

    }
    else
    {
        eStatus =  eZCL_SendDiscoverAttributesRequest ( u8SourceEndPointId,
                                                        u8DestinationEndPointId,
                                                        u16ClusterId,
                                                        bDirectionIsServerToClient,
                                                        psDestinationAddress,
                                                        pu8TransactionSequenceNumber,
                                                        u16AttributeId,
                                                        bIsManufacturerSpecific,
                                                        u16ManufacturerCode,
                                                        u8MaximumNumberOfIdentifiers );
    }
    return eStatus;
}

/****************************************************************************
 *
 * NAME: APP_eZdpSystemServerDiscovery
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpSystemServerDiscovery ( uint16    u16ServerMask,
                                                     uint8*    pu8Seq)
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if ( PDUM_INVALID_HANDLE != hAPduInst )
    {
        ZPS_tsAplZdpSystemServerDiscoveryReq    sSystemServerDiscReq;

        sSystemServerDiscReq.u16ServerMask =  u16ServerMask;
        vLog_Printf(TRACE_APP,LOG_DEBUG, "APP_eZdpSystemServerDiscovery Request\n");
        return ZPS_eAplZdpSystemServerDiscoveryRequest ( hAPduInst,
                                                         pu8Seq,
                                                         &sSystemServerDiscReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpMgmtNetworkUpdateReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpMgmtNetworkUpdateReq ( uint16    u16Addr,
                                                    uint32    u32ChannelMask,
                                                    uint8     u8ScanDuration,
                                                    uint8     u8ScanCount,
                                                    uint8*    pu8Seq,
                                                    uint16    u16NwkManagerAddr )
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if ( PDUM_INVALID_HANDLE != hAPduInst )
    {
        ZPS_tsAplZdpMgmtNwkUpdateReq    sMgmtNwkUpdateReq;
        ZPS_tuAddress                   uDstAddr;
        ZPS_tsNwkNib                    *psNib =  ZPS_psAplZdoGetNib();

        uDstAddr.u16Addr                    =  u16Addr;
        sMgmtNwkUpdateReq.u32ScanChannels   =  u32ChannelMask;
        sMgmtNwkUpdateReq.u8ScanDuration    =  u8ScanDuration;
        sMgmtNwkUpdateReq.u8ScanCount       =  u8ScanCount;
        sMgmtNwkUpdateReq.u8NwkUpdateId     =  psNib->sPersist.u8UpdateId + 1;
        sMgmtNwkUpdateReq.u16NwkManagerAddr =  u16NwkManagerAddr;

        return ZPS_eAplZdpMgmtNwkUpdateRequest ( hAPduInst,
                                                 uDstAddr,
                                                 FALSE,
                                                 pu8Seq,
                                                 &sMgmtNwkUpdateReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpMgmtLeave
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpMgmtLeave ( uint16    u16DstAddr,
                                         uint64    u64DeviceAddr,
                                         bool_t    bRejoin,
                                         bool_t    bRemoveChildren,
                                         uint8*    pu8Seq )
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst = PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        ZPS_tsAplZdpMgmtLeaveReq    sMgmtLeaveReq;
        ZPS_tuAddress               uDstAddr;

        uDstAddr.u16Addr = u16DstAddr;

        sMgmtLeaveReq.u64DeviceAddress =  u64DeviceAddr;
        sMgmtLeaveReq.u8Flags          =  bRejoin ? (1 << 7) : 0;
        sMgmtLeaveReq.u8Flags         |=  ((bRemoveChildren ? 1 : 0) << 6);

        return ZPS_eAplZdpMgmtLeaveRequest ( hAPduInst,
                                             uDstAddr,
                                             FALSE,
                                             pu8Seq,
                                             &sMgmtLeaveReq);
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpLeaveReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpLeaveReq ( ZPS_tuAddress    uAddress,
                                        bool             bRemoveChildren,
                                        bool             bRejoin )
{
    if ( uAddress.u64Addr != ZPS_NWK_NULL_EXT_ADDR )
    {
       return ZPS_eAplZdoLeaveNetwork ( uAddress.u64Addr,
                                        bRemoveChildren,
                                        bRejoin );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpRemoveDeviceReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpRemoveDeviceReq ( ZPS_tuAddress    uParentAddress,
                                               ZPS_tuAddress    uChildAddress )
{
    if ( ( uParentAddress.u64Addr != 0 ) &&
         ( uChildAddress.u64Addr != 0 ) )
    {
        return ZPS_eAplZdoRemoveDeviceReq ( uParentAddress.u64Addr,
                                            uChildAddress.u64Addr );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpNodeDescReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpNodeDescReq ( uint16    u16Addr,
                                           uint8*    pu8SeqNum )
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if ( PDUM_INVALID_HANDLE != hAPduInst )
    {
        ZPS_tsAplZdpNodeDescReq    sNodeDescReq;
        ZPS_tuAddress              uDstAddr;

        /* always send to node of interest rather than a cache */

        uDstAddr.u16Addr                  =  u16Addr;
        sNodeDescReq.u16NwkAddrOfInterest =  u16Addr;

        return ZPS_eAplZdpNodeDescRequest ( hAPduInst,
                                            uDstAddr,
                                            FALSE,
                                            pu8SeqNum,
                                            &sNodeDescReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpPowerDescReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpPowerDescReq ( uint16    u16Addr,
                                            uint8*    pu8SeqNum)
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        ZPS_tsAplZdpPowerDescReq    sPowerDescReq;
        ZPS_tuAddress               uDstAddr;

        /* always send to node of interest rather than a cache */
        uDstAddr.u16Addr                   =  u16Addr;
        sPowerDescReq.u16NwkAddrOfInterest =  u16Addr;

        return ZPS_eAplZdpPowerDescRequest ( hAPduInst,
                                             uDstAddr,
                                             FALSE,
                                             pu8SeqNum,
                                             &sPowerDescReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpSimpleDescReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpSimpleDescReq ( uint16    u16Addr,
                                             uint8     u8Endpoint,
                                             uint8*    pu8Seq )
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        ZPS_tsAplZdpSimpleDescReq    sSimpleDescReq;
        ZPS_tuAddress                uDstAddr;

        /* always send to node of interest rather than a cache */

        uDstAddr.u16Addr                    =  u16Addr;
        sSimpleDescReq.u16NwkAddrOfInterest =  u16Addr;
        sSimpleDescReq.u8EndPoint           =  u8Endpoint;

        return ZPS_eAplZdpSimpleDescRequest ( hAPduInst,
                                              uDstAddr,
                                              FALSE,
                                              pu8Seq,
                                              &sSimpleDescReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpActiveEndpointReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpActiveEndpointReq ( uint16    u16Addr,
                                                 uint8*    pu8SeqNum )
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        ZPS_tsAplZdpActiveEpReq    sActiveEpReq;
        ZPS_tuAddress              uDstAddr;

        /* always send to node of interest rather than a cache */

        uDstAddr.u16Addr                  = u16Addr;
        sActiveEpReq.u16NwkAddrOfInterest =  u16Addr;

        return ZPS_eAplZdpActiveEpRequest ( hAPduInst,
                                            uDstAddr,
                                            FALSE,
                                            pu8SeqNum,
                                            &sActiveEpReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpMatchDescReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/

PRIVATE ZPS_teStatus APP_eZdpMatchDescReq ( uint16    u16Addr,
                                            uint16    u16profile,
                                            uint8     u8InputCount,
                                            uint16*   pu16InputList,
                                            uint8     u8OutputCount,
                                            uint16*   pu16OutputList,
                                            uint8*    pu8SeqNum )
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if ( PDUM_INVALID_HANDLE != hAPduInst )
    {
        ZPS_tsAplZdpMatchDescReq    sMatchDescReq;
        ZPS_tuAddress               uDstAddr;

        /* always send to node of interest rather than a cache */

        uDstAddr.u16Addr = u16Addr;
        sMatchDescReq.u16NwkAddrOfInterest =  u16Addr;
        sMatchDescReq.u16ProfileId         =  u16profile;
        sMatchDescReq.u8NumInClusters      =  u8InputCount;
        sMatchDescReq.pu16InClusterList    =  pu16InputList;
        sMatchDescReq.u8NumOutClusters     =  u8OutputCount;
        sMatchDescReq.pu16OutClusterList   =  ( u8OutputCount == 0 )? NULL : pu16OutputList;

        return ZPS_eAplZdpMatchDescRequest ( hAPduInst,
                                             uDstAddr,
                                             FALSE,
                                             pu8SeqNum,
                                             &sMatchDescReq );

    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: APP_eZdpIeeeAddrReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpIeeeAddrReq ( uint16    u16Dst,
                                           uint16    u16Addr,
                                           uint8     u8RequestType,
                                           uint8     u8StartIndex,
                                           uint8*    pu8Seq)
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if ( PDUM_INVALID_HANDLE != hAPduInst )
    {
        ZPS_tsAplZdpIeeeAddrReq    sAplZdpIeeeAddrReq;
        ZPS_tuAddress              uDstAddr;

        /* always send to node of interest rather than a cache */
        uDstAddr.u16Addr                        =  u16Dst;
        sAplZdpIeeeAddrReq.u16NwkAddrOfInterest =  u16Addr;
        sAplZdpIeeeAddrReq.u8RequestType        =  u8RequestType;
        sAplZdpIeeeAddrReq.u8StartIndex         =  u8StartIndex;

        return ZPS_eAplZdpIeeeAddrRequest ( hAPduInst,
                                            uDstAddr,
                                            FALSE,
                                            pu8Seq,
                                            &sAplZdpIeeeAddrReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;

}

/****************************************************************************
 *
 * NAME: APP_eZdpNwkAddrReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpNwkAddrReq ( uint16    u16Dst,
                                          uint64    u64Addr,
                                          uint8     u8RequestType,
                                          uint8     u8StartIndex,
                                          uint8*    pu8Seq )
{
    PDUM_thAPduInstance hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        ZPS_tsAplZdpNwkAddrReq    sAplZdpNwkAddrReq;
        ZPS_tuAddress             uDstAddr;

        /* always send to node of interest rather than a cache */

        uDstAddr.u16Addr                =  u16Dst;
        sAplZdpNwkAddrReq.u64IeeeAddr   =  u64Addr;
        sAplZdpNwkAddrReq.u8RequestType =  u8RequestType;
        sAplZdpNwkAddrReq.u8StartIndex  =  u8StartIndex;

        return ZPS_eAplZdpNwkAddrRequest ( hAPduInst,
                                           uDstAddr,
                                           FALSE,
                                           pu8Seq,
                                           &sAplZdpNwkAddrReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;

}
/****************************************************************************
 *
 * NAME: APP_eZdpPermitJoiningReq
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpPermitJoiningReq ( uint16    u16DstAddr,
                                                uint8     u8PermitDuration,
                                                bool      bTcSignificance,
                                                uint8*    pu8Seq )
{


    if(u16DstAddr != ZPS_u16NwkNibGetNwkAddr(ZPS_pvAplZdoGetNwkHandle()))
    {
        PDUM_thAPduInstance    hAPduInst;

        hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

        if (PDUM_INVALID_HANDLE != hAPduInst)
        {
            ZPS_tsAplZdpMgmtPermitJoiningReq    sAplZdpMgmtPermitJoiningReq;
            ZPS_tuAddress                       uDstAddr;

            /* always send to node of interest rather than a cache */

            uDstAddr.u16Addr                             =  u16DstAddr;
            sAplZdpMgmtPermitJoiningReq.u8PermitDuration =  u8PermitDuration;
            sAplZdpMgmtPermitJoiningReq.bTcSignificance  =  1; /* We should always set this to 1 as per spec*/

            return ZPS_eAplZdpMgmtPermitJoiningRequest ( hAPduInst,
                                                         uDstAddr,
                                                         FALSE,
                                                         pu8Seq,
                                                         &sAplZdpMgmtPermitJoiningReq );
        }
    }
    else
    {
        return ZPS_eAplZdoPermitJoining(u8PermitDuration);
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 *
 * NAME: eBindEntry
 *
 * DESCRIPTION:
 *
 *
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eBindUnbindEntry ( bool_t           bBind,
                                            uint64           u64SrcAddr,
                                            uint8            u8SrcEndpoint,
                                            uint16           u16ClusterId,
                                            ZPS_tuAddress    *puDstAddress,
                                            uint8            u8DstEndpoint,
                                            uint8            u8DstAddrMode,
                                            uint8*           pu8Seq )
{
    ZPS_teStatus                 eReturnCode =  ZPS_APL_APS_E_INVALID_PARAMETER;
    ZPS_tsAplZdpBindUnbindReq    sAplZdpBindReq;

    if ( u8DstAddrMode != 1  &&
         u8DstAddrMode != 3 )
    {
        return eReturnCode;
    }

    if( u8DstAddrMode == 0x1 )
    {
        sAplZdpBindReq.uAddressField.sShort.u16DstAddress =  puDstAddress->u16Addr;
    }
    else
    {
        u8DstAddrMode                                        =  0x3;
        sAplZdpBindReq.uAddressField.sExtended.u64DstAddress =  puDstAddress->u64Addr;
        sAplZdpBindReq.uAddressField.sExtended.u8DstEndPoint =  u8DstEndpoint;
    }

    if ( ZPS_u64NwkNibGetExtAddr ( ZPS_pvAplZdoGetNwkHandle() ) == u64SrcAddr )
    {
        if( u8DstAddrMode == 0x1 )
        {
            if(bBind)
            {
                ZPS_eAplZdoBindGroup ( u16ClusterId,
                                       u8SrcEndpoint,
                                       puDstAddress->u16Addr );
            }
            else
            {
                ZPS_eAplZdoUnbindGroup ( u16ClusterId,
                                         u8SrcEndpoint,
                                         puDstAddress->u16Addr );
            }
        }
        else
        {
            if(bBind)
            {
                eReturnCode = ZPS_eAplZdoBind ( u16ClusterId,
                                                u8SrcEndpoint,
                                                puDstAddress->u16Addr,
                                                puDstAddress->u64Addr,
                                                u8DstEndpoint );
            }
            else
            {
                eReturnCode = ZPS_eAplZdoUnbind ( u16ClusterId,
                                                  u8SrcEndpoint,
                                                  puDstAddress->u16Addr,
                                                  puDstAddress->u64Addr,
                                                  u8DstEndpoint );
            }
        }
    }
    else
    {
        PDUM_thAPduInstance    hAPduInst;

        hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

        if (PDUM_INVALID_HANDLE != hAPduInst)
        {
            ZPS_tuAddress uDstAddr;
            /* always send to node of interest rather than a cache */
            uDstAddr.u64Addr             =  u64SrcAddr;
            sAplZdpBindReq.u64SrcAddress =  u64SrcAddr;
            sAplZdpBindReq.u8SrcEndpoint =  u8SrcEndpoint;
            sAplZdpBindReq.u16ClusterId  =  u16ClusterId;
            sAplZdpBindReq.u8DstAddrMode =  u8DstAddrMode;

            eReturnCode = ZPS_eAplZdpBindUnbindRequest( hAPduInst,
                                                        uDstAddr,
                                                        TRUE,
                                                        pu8Seq,
                                                        bBind,
                                                        &sAplZdpBindReq );
        }
    }
    return eReturnCode;
}
#ifdef FULL_FUNC_DEVICE
/****************************************************************************
 *
 * NAME: vControlNodeScanStart
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * RETURNS:
 *
 * NOTES:
 *
 ****************************************************************************/

PRIVATE void APP_vControlNodeScanStart(void)
{
    uint8    au8Buffer[1];
    uint8    u8Status;

    if( sZllState.eState == FACTORY_NEW )
    {
        /* factory new start up */
        if( sZllState.u8DeviceType == ZPS_ZDO_DEVICE_COORD )
        {
            sBDB.sAttrib.u8bdbCommissioningMode =  BDB_COMMISSIONING_MODE_NWK_FORMATION;
            u8Status =  BDB_eNfStartNwkFormation();
            if ( BDB_E_SUCCESS != u8Status )
            {
                au8Buffer[0] =  u8Status;
                vSL_WriteMessage ( E_SL_MSG_NETWORK_JOINED_FORMED,
                                   sizeof(uint8)  ,
                                   au8Buffer );
            }
            return;
        }
        else if( sZllState.u8DeviceType == ZPS_ZDO_DEVICE_ROUTER )
        {
            sBDB.sAttrib.u8bdbCommissioningMode =  BDB_COMMISSIONING_MODE_TOUCHLINK |
                                                  BDB_COMMISSIONING_MODE_NWK_STEERING|
                                                  BDB_COMMISSIONING_MODE_NWK_FORMATION ;
        }
        else /* ZPS_ZDO_DEVICE_ENDDEVICE */
        {
            sBDB.sAttrib.u8bdbCommissioningMode =     BDB_COMMISSIONING_MODE_NWK_STEERING;
        }

    }
    BDB_vStart();
    if( sZllState.eState == FACTORY_NEW &&
        sBDB.sAttrib.u8bdbCommissioningMode & BDB_COMMISSIONING_MODE_NWK_STEERING)
    {
        BDB_eNsStartNwkSteering();
    }
}
#endif

/****************************************************************************
 *
 * NAME: APP_vControlNodeStartNetwork
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * RETURNS:
 *
 * NOTES:
 *
 ****************************************************************************/

PRIVATE void APP_vControlNodeStartNetwork(void)
{
    ZPS_tsAplAib*    psAib =  ZPS_psAplAibGetAib();
    uint8            au8Buffer[1];
    uint8            u8Status;
#ifdef FULL_FUNC_DEVICE
    if( sZllState.eState == FACTORY_NEW )
    {
#endif
        if( sZllState.u8DeviceType == ZPS_ZDO_DEVICE_COORD )
        {
#if ((defined JENNIC_CHIP_FAMILY) &&  ((JENNIC_CHIP_FAMILY == JN516x) || (JENNIC_CHIP_FAMILY == JN517x)))
            ZPS_vTcInitFlash(&sSet, asTclkStruct);
            ZPS_vTCSetCallback(APP_bSendHATransportKey);
#endif
            if( psAib->u64ApsUseExtendedPanid == 0 )
            {
#ifndef NETWORK_RECOVERY
                psAib->u64ApsUseExtendedPanid   =  RND_u32GetRand(1, 0xffffffff);
                psAib->u64ApsUseExtendedPanid <<=  32;
                psAib->u64ApsUseExtendedPanid  |=  RND_u32GetRand(0, 0xffffffff);
#endif
            }
            sBDB.sAttrib.u8bdbCommissioningMode =  BDB_COMMISSIONING_MODE_NWK_FORMATION;
            u8Status =  BDB_eNfStartNwkFormation();
            if ( BDB_E_SUCCESS != u8Status )
            {
                au8Buffer[0] =  u8Status;
                vSL_WriteMessage ( E_SL_MSG_NETWORK_JOINED_FORMED,
                                   sizeof(uint8)  ,
                                   au8Buffer );
            }
        }
#ifdef FULL_FUNC_DEVICE
        else
        {
            if(sZllState.eNodeState != E_RUNNING )
            {
                APP_tsEvent sAppEvent;
                sAppEvent.eType = APP_E_EVENT_START_ROUTER;
                ZQ_bQueueSend ( &APP_msgAppEvents, &sAppEvent );

            }
        }
    }
#endif
}

/****************************************************************************
 *
 * NAME: APP_vConfigureDevice
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * RETURNS:
 *
 * NOTES:
 *
 ****************************************************************************/
PUBLIC void APP_vConfigureDevice ( uint8 u8DeviceType )
{
    if( u8DeviceType !=  0 )
    {
        ZPS_vAplSecSetInitialSecurityState(ZPS_ZDO_DISTRIBUTED_LINK_KEY,
                                           sBDB.pu8DistributedLinkKey,
                                           0x00,
                                           ZPS_APS_GLOBAL_LINK_KEY);
    }
    else
    {
#if ((defined JENNIC_CHIP_FAMILY) &&  ((JENNIC_CHIP_FAMILY == JN516x) || (JENNIC_CHIP_FAMILY == JN517x)))
        ZPS_vTcInitFlash(&sSet, asTclkStruct);
        ZPS_vTCSetCallback(APP_bSendHATransportKey);
#endif
    }

    ZPS_vNwkSetDeviceType ( ZPS_pvAplZdoGetNwkHandle(),
                           ( u8DeviceType + 1 ) );  /* coordinator is 1 - ED is 3 Router is 2*/
    ZPS_vSetZdoDeviceType ( u8DeviceType );     /* coordinator is 0 - ED is 2 and Router is 1 */
}

#ifdef FULL_FUNC_DEVICE

/****************************************************************************
 *
 * NAME: vSetGroupAddress
 *
 * DESCRIPTION: set and save the group addresses for use by the remote control
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vSetGroupAddress ( uint16    u16GroupStart,
                               uint8     u8NumGroups)
{
    uint8    i;

    /* This passes all the required group addresses for the device
     * if the are morethan one sub devices (endpoints) they need
     * sharing amoung the endpoints
     * In this case there is one the 1 Rc endpoint, so all group addresses
     * are for it
     */
    for ( i = 0; i < NUM_GROUP_RECORDS && i < u8NumGroups; i++ )
    {
        sGroupTable.asGroupRecords[i].u16GroupId  =  u16GroupStart++;
        sGroupTable.asGroupRecords[i].u8GroupType =  0;
    }
    sGroupTable.u8NumRecords =  u8NumGroups;

    PDM_eSaveRecordData ( PDM_ID_APP_GROUP_TABLE,
                          &sGroupTable,
                          sizeof ( tsZllGroupInfoTable ) );

}
/****************************************************************************
 *
 * NAME: APP_vForceStartRouter
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * RETURNS:
 *
 * NOTES:
 *
 ****************************************************************************/
PUBLIC void APP_vForceStartRouter ( uint8*    pu8Buffer )
{
    ZPS_tsAftsStartParamsDistributed    sDistributed;
    ZPS_tsAplAib*                       psAib =  ZPS_psAplAibGetAib();

    if( psAib->u64ApsUseExtendedPanid == 0 )
    {
        sDistributed.u64ExtPanId   =  RND_u32GetRand(1, 0xffffffff);
        sDistributed.u64ExtPanId <<=  32;
        sDistributed.u64ExtPanId  |=  RND_u32GetRand(0, 0xffffffff);
    }
    else
    {
        sDistributed.u64ExtPanId =  psAib->u64ApsUseExtendedPanid;
    }
    sDistributed.pu8NwkKey  =  NULL;
    sDistributed.u16PanId   =  ( uint16 ) RND_u32GetRand(0, 0xfff0);
    sDistributed.u8KeyIndex =  0 ;


    if ( u32ChannelMask ==  0x07fff800 )
    {
        sDistributed.u8LogicalChannel =  DEFAULT_CHANNEL ;
    }
    else
    {
        uint8 u8Value  = 0;
        while ( ( ( u32ChannelMask & ( 1 << u8Value ) ) == 0 ) &&
                ( u8Value < 27 ) )
        {
            u8Value++;
        }

        if ( u32ChannelMask & ( 1 << u8Value ) )
        {
            sDistributed.u8LogicalChannel =  u8Value;
        }
        else
        {
            sDistributed.u8LogicalChannel =  DEFAULT_CHANNEL;
        }
    }

    sDistributed.u16NwkAddr       =  0x0001;
    sZllState.u16FreeAddrLow     +=  1;
    sZllState.u16FreeGroupLow    +=  GROUPS_REQUIRED;
    vSetGroupAddress ( sZllState.u16FreeGroupLow, GROUPS_REQUIRED );

    ZPS_eAplFormDistributedNetworkRouter ( &sDistributed, TRUE );
    sZllState.eState        =  NOT_FACTORY_NEW;
    sZllState.eNodeState  =  E_RUNNING;
    APP_vSendJoinedFormEventToHost ( 1, pu8Buffer );

}
#endif

/****************************************************************************
 *
 * NAME: APP_bSendHATransportKey
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * RETURNS:
 *
 * NOTES:
 *
 ****************************************************************************/
PUBLIC bool APP_bSendHATransportKey ( uint16    u16ShortAddress,
                                      uint64    u64DeviceAddress,
                                      uint64    u64ParentAddress,
                                      uint8     u8Status)
{
    bool_t          bStatus =  TRUE;
    ZPS_teStatus    eStatus;
    if( bBlackListEnable )
    {
        ZPS_teDevicePermissions    eDevicePermissions;
        eStatus =  ZPS_bAplZdoTrustCenterGetDevicePermissions ( u64DeviceAddress,
                                                                &eDevicePermissions );
        if( eStatus == ZPS_E_SUCCESS )
        {
            if ( eDevicePermissions != ZPS_DEVICE_PERMISSIONS_ALL_PERMITED )
            {
                bStatus = FALSE;
            }
            else
            {
                bStatus = TRUE;
            }
        }
        else
        {
            bStatus = FALSE;
        }
        if ( ( sZllState.u8DeviceType >= 2  ) &&
             ( eStatus == ZPS_E_SUCCESS )       &&
             ( eDevicePermissions == ZPS_DEVICE_PERMISSIONS_ALL_PERMITED )
           )
        {
            u64CallbackMacAddress =  u64DeviceAddress;
            ZTIMER_eStart ( u8HaModeTimer , ZTIMER_TIME_MSEC ( 500 ) );
        }
    }
    else
    {
        if(sZllState.u8DeviceType >= 2)
        {
           u64CallbackMacAddress =  u64DeviceAddress;
           ZTIMER_eStart ( u8HaModeTimer, ZTIMER_TIME_MSEC ( 500 ) );
        }
    }
#if ((defined JENNIC_CHIP_FAMILY) &&  ((JENNIC_CHIP_FAMILY == JN516x) || (JENNIC_CHIP_FAMILY == JN517x)))
    AESSW_Block_u   uKey;
    uint16          u16Location;
    if ( ( bSetTclkFlashFeature &&
           !zps_bGetFlashCredential ( u64DeviceAddress,  &uKey, &u16Location, FALSE, FALSE  ) )||
         ( u8Status == 1) )
    {
        extern tsMicroIntStorage    sZpsIntStore;
        extern PUBLIC void* zps_vGetZpsMutex ( void );

        ZPS_u8ReleaseMutexLock ( zps_vGetZpsMutex , &sZpsIntStore );

        ZPS_eAplZdoAddReplaceLinkKey( u64DeviceAddress, sBDB.pu8DefaultTCLinkKey, ZPS_APS_UNIQUE_LINK_KEY);

        ZPS_u8GrabMutexLock ( zps_vGetZpsMutex , &sZpsIntStore );
        bStatus = TRUE;
    }
#endif
    return bStatus;
}

/****************************************************************************
 *
 * NAME: vSendJoinedFormEventToHost
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * RETURNS:
 *
 * NOTES:
 *
 ****************************************************************************/
PUBLIC void APP_vSendJoinedFormEventToHost ( uint8    u8FormJoin,
                                             uint8    *pu8Buffer )
{
    uint32           u32Channel;
    uint16           u16NwkAddr;
    uint64           u64IeeeAddr;
    uint8            *pu8BufferCpy =  pu8Buffer;
    uint8            u8Length = 0;
    static bool_t    bReportSent   =  FALSE;


    if (bReportSent)
    {
        return;
    }

    bReportSent =  TRUE;

    u16NwkAddr  =  ZPS_u16NwkNibGetNwkAddr ( ZPS_pvAplZdoGetNwkHandle ( ) );
    u64IeeeAddr =  ZPS_u64NwkNibGetExtAddr ( ZPS_pvAplZdoGetNwkHandle ( ) );

    eAppApiPlmeGet ( PHY_PIB_ATTR_CURRENT_CHANNEL, &u32Channel );

    *pu8BufferCpy = u8FormJoin;

    ZNC_BUF_U8_UPD  ( &pu8BufferCpy [ 1 ],          u8FormJoin,           u8Length );
    ZNC_BUF_U16_UPD ( &pu8BufferCpy [ u8Length],    u16NwkAddr,           u8Length );
    ZNC_BUF_U64_UPD ( &pu8BufferCpy [ u8Length],    u64IeeeAddr,          u8Length );
    ZNC_BUF_U8_UPD  ( &pu8BufferCpy [ u8Length],    (uint8)u32Channel,    u8Length );
    ZPS_vSaveAllZpsRecords();
    APP_vSaveAllRecords();
    vSL_WriteMessage ( E_SL_MSG_NETWORK_JOINED_FORMED,
                       ( sizeof(uint8) + sizeof(uint16) + sizeof(uint64) + sizeof(uint8) ),
                       pu8Buffer );
    ZTIMER_eStart ( u8TmrToggleLED, s_sLedState.u32LedToggleTime );

    /* Initialise the OTA  */
#ifdef CLD_OTA
    vAppInitOTA();
#endif

}


#ifdef FULL_FUNC_DEVICE

PRIVATE uint8 u8GetFirstPrimaryChannel( void)
{
    uint32 u32Channel = 0;
    uint32 u32ChannelMask = BDB_PRIMARY_CHANNEL_SET;

    while( (u32ChannelMask & 0x01) == 0)
    {
        u32Channel++;
        u32ChannelMask >>= 1;
    };

    return (uint8)u32Channel;


}


/****************************************************************************
 *
 * NAME: APP_vFactoryResetRecords
 *
 * DESCRIPTION: reset application and stack to factory new state
 *          preserving the outgoing nwk frame counter
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vFactoryResetRecords( void)
{
    /* clear out the stack */
    ZPS_vDefaultStack ( );
    (void)ZPS_eAplAibSetApsUseExtendedPanId(ZPS_APS_AIB_INIT_USE_EXTENDED_PANID);
    vZpsGlobalsInit();
    //BDB_vSetKeys ( );

    /* clear out the application */
    sZllState.eState           = FACTORY_NEW;
    sZllState.eNodeState       = E_STARTUP;
    sZllState.u8MyChannel      = u8GetFirstPrimaryChannel();
    sZllState.u16MyAddr        = TL_MIN_ADDR;
    sZllState.u16FreeAddrLow   = TL_MIN_ADDR;
    sZllState.u16FreeAddrHigh  = TL_MAX_ADDR;
    sZllState.u16FreeGroupLow  = TL_MIN_GROUP;
    sZllState.u16FreeGroupHigh = TL_MAX_GROUP;

    FLib_MemSet ( &sEndpointTable,
             0 ,
             sizeof ( tsZllEndpointInfoTable ) );
    FLib_MemSet ( &sGroupTable,
             0,
             sizeof ( tsZllGroupInfoTable ) );

    /* save everything */
    PDM_eSaveRecordData ( PDM_ID_APP_ZLL_CMSSION,
                          &sZllState,
                          sizeof ( tsZllState ) );
    PDM_eSaveRecordData ( PDM_ID_APP_END_P_TABLE,
                          &sEndpointTable,
                          sizeof ( tsZllEndpointInfoTable ) );
    PDM_eSaveRecordData ( PDM_ID_APP_GROUP_TABLE,
                          &sGroupTable,
                          sizeof ( tsZllGroupInfoTable ) );
    ZPS_vSaveAllZpsRecords ( );

    bResetIssued =  TRUE;
    ZTIMER_eStart ( u8IdTimer, ZTIMER_TIME_MSEC ( 1 ) );
    
    #ifdef CPU_MKW41Z512VHT4
        NvCompletePendingOperations();
    #endif   

}
#endif

/****************************************************************************
 **
 ** NAME:       APP_eSendWriteAttributesRequest
 **
 **
 **
 ****************************************************************************/

PUBLIC  teZCL_Status  APP_eSendWriteAttributesRequest ( uint8               u8SourceEndPointId,
                                                        uint8               u8DestinationEndPointId,
                                                        uint16              u16ClusterId,
                                                        bool_t              bDirectionIsServerToClient,
                                                        tsZCL_Address       *psDestinationAddress,
                                                        uint8               *pu8TransactionSequenceNumber,
                                                        bool_t              bIsManufacturerSpecific,
                                                        uint16              u16ManufacturerCode,
                                                        uint8               *pu8AttributeRequestList,
                                                        uint8               u8NumberOfAttrib,
                                                        uint16              u16SizePayload )
{

    uint32                 i;
    uint32                 u32PdumPayloadSize;
    uint16                 u16offset;
    uint16                 u16AttribId;
    PDUM_thAPduInstance    myPDUM_thAPduInstance;
    uint8                  u8FramControl =  0;
    uint8                  u8CommandId   =  0x02;
    uint8                  *pu8Data      =  NULL ;
    uint16                 u16Size;

    /*  handle sequence number pass present value back to user */
    *pu8TransactionSequenceNumber =  u8GetTransactionSequenceNumber();

    /*  get buffer */
    myPDUM_thAPduInstance =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if( myPDUM_thAPduInstance == PDUM_INVALID_HANDLE )
    {
        return(E_ZCL_ERR_ZBUFFER_FAIL);
    }


    if ( bIsManufacturerSpecific )
    {
       u32PdumPayloadSize = ( 5 + u16SizePayload );
    }
    else
    {
       u32PdumPayloadSize = ( 3 + u16SizePayload );
    }


    if ( PDUM_E_OK != PDUM_eAPduInstanceSetPayloadSize(myPDUM_thAPduInstance, ( u32PdumPayloadSize) ) )
    {
        return(E_ZCL_ERR_ZBUFFER_FAIL);
    }

    u8FramControl =  ( 0x00 )                         |
                      ( ( ( bIsManufacturerSpecific ) ?  1 : 0 ) << 2 )   |
                      ( ( ( bDirectionIsServerToClient ) ?  1 : 0 ) << 3) |
                      ( 1 << 4 );

    /*  write command header */

    if( bIsManufacturerSpecific )
    {
        u16offset =  PDUM_u16APduInstanceWriteNBO ( myPDUM_thAPduInstance,
                                                    0,
                                                    "bhbb",
                                                    u8FramControl,
                                                    u16ManufacturerCode,
                                                    *pu8TransactionSequenceNumber,
                                                    u8CommandId );
    }
    else
    {
        u16offset =  PDUM_u16APduInstanceWriteNBO ( myPDUM_thAPduInstance,
                                                    0,
                                                    "bbb",
                                                    u8FramControl,
                                                    *pu8TransactionSequenceNumber,
                                                    u8CommandId );
    }


    i       =  0;
    pu8Data =  ( uint8* ) PDUM_pvAPduInstanceGetPayload ( myPDUM_thAPduInstance );

    while( i < u16SizePayload )
    {
        u16AttribId    =  ZNC_RTN_U16 ( pu8AttributeRequestList, i );
        u16offset     +=  PDUM_u16APduInstanceWriteNBO ( myPDUM_thAPduInstance,
                                                         u16offset,
                                                         "h",
                                                         u16AttribId );
        u16offset     +=  PDUM_u16APduInstanceWriteNBO ( myPDUM_thAPduInstance,
                                                         u16offset,
                                                         "b",
                                                         pu8AttributeRequestList [ i + 2 ] );

        u16Size        =  APP_u16GetAttributeActualSize ( pu8AttributeRequestList [ i + 2 ], 1 );
        if ( u16Size == 0 )
        {
            PDUM_eAPduFreeAPduInstance ( myPDUM_thAPduInstance );
            return ( E_ZCL_ERR_ZTRANSMIT_FAIL );
        }
        u16offset     +=  APP_u16ZncWriteDataPattern ( &pu8Data [ u16offset ],
                                                       pu8AttributeRequestList [ i + 2 ],
                                                       &pu8AttributeRequestList [ i + 3 ],
                                                       u16Size );
        i              =  i + u16Size + 3;
    }

    // transmit the request
    if( ZPS_eAplAfUnicastDataReq ( myPDUM_thAPduInstance,
                                   u16ClusterId,
                                   u8SourceEndPointId,
                                   u8DestinationEndPointId,
                                   psDestinationAddress->uAddress.u16DestinationAddress,
                                   ZPS_E_APL_AF_SECURE_NWK,
                                   0,
                                   NULL ) != E_ZCL_SUCCESS )
    {
        return ( E_ZCL_ERR_ZTRANSMIT_FAIL );
    }

    return(E_ZCL_SUCCESS);
}


/****************************************************************************
 **
 ** NAME:       APP_eZdpMgmtLqiRequest
 **
 **
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpMgmtLqiRequest( uint16    u16Addr,
                                             uint8     u8StartIndex,
                                             uint8*    pu8Seq )
{
    PDUM_thAPduInstance    hAPduInst;

    hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );

    if ( PDUM_INVALID_HANDLE != hAPduInst )
    {
        ZPS_tsAplZdpMgmtLqiReq    sMgmtLqiReq;
        ZPS_tuAddress             uDstAddr;

        uDstAddr.u16Addr         =  u16Addr;
        sMgmtLqiReq.u8StartIndex =  u8StartIndex;

        DBG_vPrintf(TRACE_APP, "\nManagement Lqi Request");

        return ZPS_eAplZdpMgmtLqiRequest ( hAPduInst,
                                           uDstAddr,
                                           FALSE,
                                           pu8Seq,
                                           &sMgmtLqiReq);
    }
    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 **
 ** NAME:       APP_eZdpComplexDescReq
 **
 **
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpComplexDescReq ( uint16    u16Addr,
                                              uint16    u16NwkAddressInterst,
                                              uint8*    pu8Seq )
{
    PDUM_thAPduInstance hAPduInst;

    hAPduInst = PDUM_hAPduAllocateAPduInstance(apduZDP);



    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        ZPS_tsAplZdpComplexDescReq    sComplexDescReq;
        ZPS_tuAddress                 uDstAddr;

        /* always send to node of interest rather than a cache */
        uDstAddr.u16Addr = u16Addr;
        sComplexDescReq.u16NwkAddrOfInterest =  u16NwkAddressInterst;

        return ZPS_eAplZdpComplexDescRequest( hAPduInst,
                                              uDstAddr,
                                              FALSE,
                                              pu8Seq,
                                              &sComplexDescReq );
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}
/****************************************************************************
 *
 * NAME:
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vIdentifyEffect ( void* pvParam )
{
    /* interpan id time out, shut down */
    uint8    u8Status = 1;
    uint16  u16IdentifyTime = *(uint16*)pvParam;

    if(u16IdentifyTime == 0xffff)
    {
        u16IdentifyTime = APP_IDENTIFY_TIME;
    }

    u16IdentifyTime *= 1000;

    ZTIMER_eStop ( u8IdTimer );
    ZTIMER_eStart( u8IdTimer, ZTIMER_TIME_MSEC (u16IdentifyTime));
    vSL_WriteMessage ( E_SL_MSG_IDENTIFY_LOCAL_ACTIVE,
                       sizeof(uint8),
                       &u8Status );
}

/****************************************************************************
 *
 * NAME:
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vIdentifyEffectEnd ( void* pvParam )
{
    uint8 u8Status = 0;
    if( bResetIssued )
    {
        NVIC_SystemReset();
        bResetIssued =  FALSE;
    }
    else
    {
        vSL_WriteMessage ( E_SL_MSG_IDENTIFY_LOCAL_ACTIVE,
                           sizeof(uint8),
                           &u8Status );
    }
}
/****************************************************************************
 **
 ** NAME:       APP_vSaveAllRecords
 **
 **
 ****************************************************************************/
void APP_vSaveAllRecords ( void )
{
    PDM_eSaveRecordData( PDM_ID_APP_ZLL_CMSSION,
                         &sZllState,
                         sizeof ( tsZllState ) );
#ifdef FULL_FUNC_DEVICE
    PDM_eSaveRecordData( PDM_ID_APP_END_P_TABLE,
                         &sEndpointTable,
                         sizeof ( tsZllEndpointInfoTable ) );
    PDM_eSaveRecordData( PDM_ID_APP_GROUP_TABLE,
                         &sGroupTable,
                         sizeof ( tsZllGroupInfoTable ) );
#endif
}


/****************************************************************************
 **
 ** NAME:       APP_eApsProfileDataRequest
 **
 **
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eApsProfileDataRequest ( ZPS_tsAfProfileDataReq*    psProfileDataReq,
                                                  uint8*                     pu8Data,
                                                  uint8                     u8DataLength,
                                                  uint8*                     pu8Seq  )
{
    PDUM_thAPduInstance    hAPduInst = PDUM_INVALID_HANDLE;
    ZPS_teStatus           eStatus =  ZPS_APL_APS_E_INVALID_PARAMETER;

    if (u8DataLength > PDUM_u16APduGetSize(apduZDP) )
    {
        vLog_Printf(TRACE_APP,LOG_DEBUG, "APDU too small \n");
        eStatus =  E_ZCL_ERR_ZBUFFER_FAIL;
    }
    else
    {
        hAPduInst =  PDUM_hAPduAllocateAPduInstance ( apduZDP );
    }

    if ( PDUM_INVALID_HANDLE != hAPduInst )
    {
        uint16    u16LoopCounter =  0;
        uint16    u16Location    =  0;
        for (u16LoopCounter = 0 ; u16LoopCounter < u8DataLength ; u16LoopCounter++)
        {
            APDU_BUF_INC(hAPduInst, u16Location) = pu8Data[u16LoopCounter];
        }
        vLog_Printf(TRACE_APP,LOG_DEBUG, "ZPS_eAplAfApsdeDataReq Request\n");
        if ( PDUM_E_OK != PDUM_eAPduInstanceSetPayloadSize(hAPduInst, ( u16LoopCounter) ) )
        {
            vLog_Printf(TRACE_APP,LOG_DEBUG, "Payload too big  \n");
            eStatus = E_ZCL_ERR_ZBUFFER_FAIL ;
        }
        else
        {
            eStatus =  ZPS_eAplAfApsdeDataReq( hAPduInst,
                                               psProfileDataReq,
                                               pu8Seq );
        }
    }

    return eStatus;
}

/****************************************************************************
 **
 ** NAME:       APP_eSetUserDescriptorReq
 **
 **
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eSetUserDescriptorReq( uint16    u16Addr,
                                                uint16    u16AddrOfInt,
                                                uint8*     pu8Data,
                                                uint8     u8Length,
                                                uint8*    pu8Seq )
{
    PDUM_thAPduInstance hAPduInst;

    hAPduInst = PDUM_hAPduAllocateAPduInstance(apduZDP);



    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        ZPS_tsAplZdpUserDescSet sUserDescSet;
        ZPS_tuAddress uDstAddr;

        /* always send to node of interest rather than a cache */
        uDstAddr.u16Addr = u16Addr;

        sUserDescSet.u16NwkAddrOfInterest = u16AddrOfInt;
        sUserDescSet.u8Length = 16;
        FLib_MemCpy(sUserDescSet.szUserDescriptor, pu8Data, 16);
        vLog_Printf(TRACE_APP,LOG_DEBUG,  "Set User descriptor Req\n");
        return ZPS_eAplZdpUserDescSetRequest(
                        hAPduInst,
                        uDstAddr,
                        FALSE,
                        pu8Seq,
                        &sUserDescSet);
    }
    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 **
 ** NAME:       APP_eZdpUserDescReq
 **
 **
 ****************************************************************************/
PRIVATE ZPS_teStatus APP_eZdpUserDescReq( uint16    u16Addr,
                                          uint16    u16AddrOfInt,
                                          uint8*    pu8Seq )
{
    PDUM_thAPduInstance hAPduInst;

    hAPduInst = PDUM_hAPduAllocateAPduInstance(apduZDP);

    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        ZPS_tsAplZdpUserDescReq sUserDescReq;
        ZPS_tuAddress uDstAddr;

        /* always send to node of interest rather than a cache */
        uDstAddr.u16Addr = u16Addr;

        sUserDescReq.u16NwkAddrOfInterest = u16AddrOfInt;
        vLog_Printf(TRACE_APP,LOG_DEBUG, "User Descriptor Req\n");
        return ZPS_eAplZdpUserDescRequest(
                hAPduInst,
                uDstAddr,
                FALSE,
                pu8Seq,
                &sUserDescReq);
    }

    return ZPS_APL_APS_E_INVALID_PARAMETER;
}

/****************************************************************************
 **
 ** NAME:       vHandleIdentifyRequest
 **
 **
 ****************************************************************************/
PUBLIC void vHandleIdentifyRequest(uint16 u16Duration)
{
    APP_vIdentifyEffect ( &u16Duration );
}


/***    END OF FILE                           ***/
/****************************************************************************/
