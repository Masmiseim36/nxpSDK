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

#ifndef APP_COMMON_H_
#define APP_COMMON_H_


/*!
\file       app_common.h
\brief      This file is a for application configuration of demo applications
*/
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "ZQueue.h"
#include "ZTimer.h"
#include "zlo_device_id.h"
#include "control_bridge.h"
#include "portmacro.h"
#include "bdb_api.h"
#ifdef FULL_FUNC_DEVICE
#include "bdb_tl.h"
#endif
#include "PDM_IDs.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/



/* Macros take the buffer pointer and return the data */
#define ZNC_RTN_U64( BUFFER, i )  ( ( ( uint64 ) ( BUFFER )[ i ]  <<  56) |\
    ( ( uint64 ) ( BUFFER )[ i + 1 ]  << 48) |\
    ( ( uint64 ) ( BUFFER )[ i + 2 ]  << 40) |\
    ( ( uint64 ) ( BUFFER )[ i + 3 ]  << 32) |\
    ( ( uint64 ) ( BUFFER )[ i + 4 ]  << 24) |\
    ( ( uint64 ) ( BUFFER )[ i + 5 ]  << 16) |\
    ( ( uint64 ) ( BUFFER )[ i + 6 ]  << 8) |\
    ( ( uint64 ) ( BUFFER )[ i + 7 ] & 0xFF))\

#define ZNC_RTN_U32( BUFFER, i ) ( ( ( uint32 ) ( BUFFER )[ i ] << 24) |\
    ( ( uint32 ) ( BUFFER )[ i + 1 ]  << 16) |\
    ( ( uint32 ) ( BUFFER )[ i + 2 ]  << 8) |\
    ( ( uint32 ) ( BUFFER )[ i + 3 ] & 0xFF))\

#define ZNC_RTN_U16( BUFFER, i ) ( ( ( uint16 ) (BUFFER)[ i ] << 8) |\
    ( ( uint16 ) (BUFFER)[ i + 1 ] & 0xFF))\

/* Macros take the value and put it into a buffer */
#define ZNC_BUF_U8_UPD( BUFFER, U8VALUE, LEN)    (  ( *( (uint8*)( ( BUFFER ) ) ) = ( ( ( ( uint8 ) ( U8VALUE ) ) & 0xFF ) ) ) ,\
     ( ( LEN ) += sizeof( uint8 ) ) )

#define ZNC_BUF_U64_UPD( BUFFER, U64VALUE, LEN)    ( ( *(uint8*) ( BUFFER ) = (uint8) ( ( ( ( uint64 ) ( U64VALUE ) ) >> 56) & 0xFF)),\
    ( *(uint8*) ( ( BUFFER ) + 1) = (uint8) ( ( ( ( uint64 ) ( U64VALUE ) ) >> 48) & 0xFF ) ),\
    ( *(uint8*) ( ( BUFFER ) + 2) = (uint8) ( ( ( ( uint64 ) ( U64VALUE ) ) >> 40) & 0xFF ) ),\
    ( *(uint8*) ( ( BUFFER ) + 3) = (uint8) ( ( ( ( uint64 ) ( U64VALUE ) ) >> 32) & 0xFF ) ),\
    ( *(uint8*) ( ( BUFFER ) + 4) = (uint8) ( ( ( ( uint64 ) ( U64VALUE ) ) >> 24) & 0xFF ) ),\
    ( *(uint8*) ( ( BUFFER ) + 5) = (uint8) ( ( ( ( uint64 ) ( U64VALUE ) ) >> 16) & 0xFF ) ),\
    ( *(uint8*) ( ( BUFFER ) + 6) = (uint8) ( ( ( ( uint64 ) ( U64VALUE ) ) >>  8) & 0xFF ) ),\
    ( *(uint8*) ( ( BUFFER ) + 7) = (uint8) ( ( ( ( uint64 ) ( U64VALUE ) ) & 0xFF ) ) ),\
    ( ( LEN ) += sizeof( uint64 ) ) )

#define ZNC_BUF_U32_UPD( BUFFER, U32VALUE, LEN )     ( ( *( uint8* )( BUFFER )   =  ( uint8 ) ( ( ( ( uint32 ) ( U32VALUE ) ) >> 24 ) & 0xFF ) ),\
    ( *( uint8* )( ( BUFFER ) + 1) =  ( uint8 ) ( ( ( ( uint32 ) ( U32VALUE ) ) >> 16 ) & 0xFF ) ),\
    ( *( uint8* )( ( BUFFER ) + 2) =  ( uint8 ) ( ( ( ( uint32 ) ( U32VALUE ) ) >> 8 ) & 0xFF ) ),\
    ( *( uint8* )( ( BUFFER ) + 3) =  ( uint8 ) ( ( ( ( uint32 ) ( U32VALUE ) ) & 0xFF ) ) ) ,\
    ( ( LEN ) += sizeof ( uint32 ) ) )


#define ZNC_BUF_U16_UPD( BUFFER, U16VALUE, LEN )     ( ( *( uint8* )( BUFFER )   =  ( uint8 )  ( ( ( ( uint16)( U16VALUE ) ) >> 8 ) & 0xFF ) ),\
    ( *( uint8* ) ( ( BUFFER ) + 1 )  =  ( uint8 )  ( ( ( ( uint16 )( U16VALUE ) ) ) & 0xFF ) ) ,\
    ( ( LEN ) += sizeof( uint16 ) ) )


/* Macros take buffer and return data and the next offset of within the buffer */
#define ZNC_RTN_U16_OFFSET(BUFFER, i, OFFSET )   ( ZNC_RTN_U16 (BUFFER, i) );\
( ( OFFSET ) += sizeof (uint16) )

#define ZNC_RTN_U32_OFFSET(BUFFER, i, OFFSET )   (  ZNC_RTN_U32 (BUFFER, i) );\
( ( OFFSET ) += sizeof (uint32) )

#define ZNC_RTN_U64_OFFSET(BUFFER, i, OFFSET )  (  ZNC_RTN_U64 (BUFFER, i) );\
( ( OFFSET ) += sizeof (uint64) )



#define ZCL_TICK_TIME           ZTIMER_TIME_MSEC(1000)
#define MAX_PACKET_SIZE         256
#define DEFAULT_CHANNEL         15
#define APP_IDENTIFY_TIME       10  /* Application specific identify time specified in seconds */
#define ZNC_MAX_TCLK_DEVICES    20
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
    E_STARTUP,
#ifdef FULL_FUNC_DEVICE
    E_NFN_START,
#endif
    E_RUNNING,
} teNODE_STATES;

typedef struct {
    enum {
        FACTORY_NEW = 0,
        NOT_FACTORY_NEW = 0xff
    }eState;
    teNODE_STATES eNodeState;
    uint8 u8DeviceType;
    uint8 u8MyChannel;
    uint16 u16MyAddr;
#ifdef FULL_FUNC_DEVICE
    uint16 u16FreeAddrLow;
    uint16 u16FreeAddrHigh;
    uint16 u16FreeGroupLow;
    uint16 u16FreeGroupHigh;
#endif
#ifdef CLD_OTA
    bool bValid;
    uint64 u64IeeeAddrOfServer;
    uint16 u16NwkAddrOfServer;
    uint8 u8OTAserverEP;
#endif
}tsZllState;

typedef struct {
   uint32 u32LedState;
   uint32 u32LedToggleTime;
   bool_t bIncreaseTime;
} tsLedState;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vConfigureDevice( uint8 u8DeviceType );
PUBLIC void APP_vFormatAndSendUpdateLists(void);
PUBLIC uint16 APP_u16GetAttributeActualSize( uint32 u32Type,
                                             uint16 u16NumberOfItems);
PUBLIC uint16 APP_u16ZncWriteDataPattern( uint8 *pu8Data,
                                          teZCL_ZCLAttributeType eAttributeDataType,
                                          uint8 *pu8Struct,
                                         uint32 u32Size);
PUBLIC void APP_vSendDataIndicationToHost( ZPS_tsAfEvent *psStackEvent,
                                           uint8* pau8StatusBuffer);
PUBLIC void APP_vSaveAllRecords(void);
PUBLIC void APP_vSendJoinedFormEventToHost ( uint8    u8FormJoin,
                                             uint8    *pu8Buffer );
PUBLIC void APP_vHandleStackEvents ( ZPS_tsAfEvent*    psStackEvent );
PUBLIC void APP_vHandleAppEvents ( void );
PUBLIC void APP_vHandleZclEvents ( ZPS_tsAfEvent*    psStackEvent );
PUBLIC void APP_ZCL_vInitialise ( void );
PUBLIC void APP_vIdentifyEffect ( void* pvParam );
PUBLIC void APP_vIdentifyEffectEnd ( void* pvParam );
PUBLIC void vHandleIdentifyRequest(uint16 u16Duration);
#ifdef FULL_FUNC_DEVICE
PUBLIC void APP_vFactoryResetRecords ( void ) ;
PUBLIC void APP_vForceStartRouter ( uint8*    pu8Buffer );
PUBLIC void APP_vAppAddGroup ( uint16    u16GroupId,
                               bool_t    bBroadcast );
PUBLIC void APP_vSetAddress ( tsZCL_Address*    psAddress,
                              bool_t            bBroadcast );
#endif
extern PUBLIC void App_TransportKeyCallback ( void*   pvParam );
extern void zps_taskZPS ( void );
extern void APP_vProcessRxData ( void );
extern bool_t APP_vProfileWideCommandSupportedForCluster ( uint16 u16Clusterid );
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
#ifdef FULL_FUNC_DEVICE
extern tsZllEndpointInfoTable     sEndpointTable;
extern tsZllGroupInfoTable        sGroupTable;
extern tsCLD_ZllDeviceTable       sDeviceTable;
extern bool_t                     bSendFactoryResetOverAir;
#endif
extern tsZllState                 sZllState;
extern tszQueue                   APP_msgSerialRx;
extern tszQueue                   APP_msgSerialTx;
extern tszQueue                   APP_msgAppEvents;
extern tszQueue                   zps_msgMlmeDcfmInd;
extern tszQueue                   zps_msgMcpsDcfmInd;
extern tszQueue                   zps_TimeEvents;
extern tszQueue                   APP_msgBdbEvents;
extern uint8                      u8IdTimer;
extern uint8                      u8TmrToggleLED;
extern uint8                      u8HaModeTimer;
extern uint8                      u8TickTimer;
#if (defined JENNIC_CHIP_FAMILY)|| (defined CPU_MKW41Z512VHT4)
extern uint32                     _heap_location;
#endif
extern bool_t                     bSetPermitJoinForever;
extern uint64                     u64CallbackMacAddress;
#if (defined JENNIC_CHIP_FAMILY)|| (defined CPU_MKW41Z512VHT4)
extern ZPS_tsAfFlashInfoSet       sSet;
#endif
extern uint8                      au8LinkRxBuffer[256];
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_COMMON_H_*/
