/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* This is a source file which implements the FSCI commands received from the host.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "EmbeddedTypes.h"
#include "FsciInterface.h"
#include "FsciCommands.h"
#include "FsciCommunication.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "ModuleInfo.h"
#include "Flash_Adapter.h"

#if gFSCI_IncludeMacCommands_c
    #include "FsciMacCommands.h"
#endif

#if gFSCI_IncludeLpmCommands_c
    #include "PWR_Interface.h"
#endif

#include "fsl_device_registers.h"

#if gBeeStackIncluded_d
#include "ZigbeeTask.h"
#endif

#if gFsciIncluded_c
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
extern uint32_t _RAM_START_[];
extern uint32_t _RAM_END_[];
#define mRamStartAddress_c   (uint32_t)(_RAM_START_)
#define mRamEndAddress_c     (uint32_t)(_RAM_END_)
/* Flash memory limits */
#define mFlashStartAddress_c (0x00000000)
#define mFlashEndAddress_c   (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE * FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT)

/* Weak function. */
#if defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined( __CC_ARM )
#define __WEAK_FUNC __weak
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
extern void ResetMCU(void);
extern void Mac_GetExtendedAddress(uint8_t *pAddr, instanceId_t instanceId);
extern void Mac_SetExtendedAddress(uint8_t *pAddr, instanceId_t instanceId);
extern uint8_t PhyGetLastRxLqiValue(void);
extern gFsciOpGroup_t *FSCI_GetReqOpGroup(opGroup_t OG, uint8_t fsciInterface);

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef PACKED_STRUCT FsciWakeUpConfig_tag
{
    bool_t   signalWhenWakeUpFlag; /* Flag used to send or not a WakeUp.Ind message */
    uint32_t deepSleepDuration;    /* The deep sleep duration in 802.15.4 phy symbols (16 us) */
}FsciWakeUpConfig_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
/* Set to TRUE when FSCI_Error() is called. */
uint8_t mFsciErrorReported;
bool_t (*pfFSCI_OtaSupportCalback)(clientPacket_t* pPacket) = NULL;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
extern uint8_t gNumberOfOG;
extern gFsciOpGroup_t gReqOpGroupTable[];
extern uint16_t gFreeMessagesCount;

/* FSCI Error message */
static gFsciErrorMsg_t mFsciErrorMsg = {
    {
        gFSCI_StartMarker_c,
        gFSCI_CnfOpcodeGroup_c,
        mFsciMsgError_c,
        sizeof(clientPacketStatus_t),
    },
    gFsciSuccess_c,
    0,
    0
};

/* FSCI Ack message */
#if gFsciTxAck_c
static gFsciAckMsg_t mFsciAckMsg = {
    {
        gFSCI_StartMarker_c,
        gFSCI_CnfOpcodeGroup_c,
        mFsciMsgAck_c,
        sizeof(uint8_t)
    },
    0,
    0,
    0
};
#endif

/* FSCI OpCodes and coresponding handler functions */
static const gFsciOpCode_t FSCI_ReqOCtable[] =
{
    {mFsciMsgModeSelectReq_c,                FSCI_MsgModeSelectReqFunc},
    {mFsciMsgGetModeReq_c,                   FSCI_MsgGetModeReqFunc},
    {mFsciMsgResetCPUReq_c,                  FSCI_MsgResetCPUReqFunc},

#if gBeeStackIncluded_d
    {mFsciMsgAFResetReq_c,                   ZbFsciHandler},
    {mFsciMsgAPSResetReq_c,                  ZbFsciHandler},
    {mFsciMsgAPSReadyReq_c,                  ZbFsciHandler},
    {mFsciMsgDeregisterEndPoint_c,           ZbFsciHandler},
    {mFsciMsgRegisterEndPoint_c,             ZbFsciHandler},
    {mFsciMsgGetNumberOfEndPoints_c,         ZbFsciHandler},
    {mFsciMsgGetEndPointDescription_c,       ZbFsciHandler},
    {mFsciMsgGetEndPointIdList_c,            ZbFsciHandler},
    {mFsciMsgSetEpMaxWindowSize_c,           ZbFsciHandler},

    {mFsciMsgGetICanHearYouList_c,           ZbFsciHandler},
    {mFsciMsgSetICanHearYouList_c,           ZbFsciHandler},

    {mFsciMsgGetChannelReq_c,                ZbFsciHandler},
    {mFsciMsgSetChannelReq_c,                ZbFsciHandler},
    {mFsciMsgGetPanIDReq_c,                  ZbFsciHandler},
    {mFsciMsgSetPanIDReq_c,                  ZbFsciHandler},
    {mFsciMsgGetPermissionsTable_c,          ZbFsciHandler},
    {mFsciMsgSetPermissionsTable_c,          ZbFsciHandler},
    {mFsciMsgRemoveFromPermissionsTable_c,   ZbFsciHandler},
    {mFsciMsgAddDeviceToPermissionsTable_c,  ZbFsciHandler},
    {mFsciMsgGetNumOfMsgsReq_c,              ZbFsciHandler},

    {mFsciMsgApsmeGetIBReq_c,                ZbFsciHandler},
    {mFsciMsgApsmeSetIBReq_c,                ZbFsciHandler},
    {mFsciMsgNlmeGetIBReq_c,                 ZbFsciHandler},
    {mFsciMsgNlmeSetIBReq_c,                 ZbFsciHandler},
    {mFsciMsgFreeDiscoveryTablesReq_c,       ZbFsciHandler},
    {mFsciMsgSetJoinFilterFlagReq_c,         ZbFsciHandler},
    {mFsciMsgGetMaxApplicationPayloadReq_c,  ZbFsciHandler},

    {mFsciMsgGetApsDeviceKeyPairSet_c,       ZbFsciHandler},
    {mFsciMsgGetApsDeviceKey_c,              ZbFsciHandler},
    {mFsciMsgSetApsDeviceKey_c,              ZbFsciHandler},
    {mFsciMsgSetApsDeviceKeyPairSet_c,       ZbFsciHandler},
    {mFsciMsgClearApsDeviceKeyPairSet_c,     ZbFsciHandler},
     /* TBD */
    {mFsciMsgSetApsDeviceKeyPairSetKeyInfo,  ZbFsciHandler}, //not used by MAC
    {mFsciMsgSetApsOverrideApsEncryption,    ZbFsciHandler},
    {mFsciMsgSetPollRate,                    ZbFsciHandler},
    {mFsciMsgSetRejoinConfigParams,          ZbFsciHandler},
    {mFsciMsgSetFaMaxIncomingErrorReports_c, ZbFsciHandler},
    {mFsciMsgSetHighLowRamConcentrator,      ZbFsciHandler},
    
    {mFsciMsgEZModeComissioningStart,                    ZbFsciHandler}, //not used by MAC  
    {mFsciMsgZllTouchlinkCommissioningStart_c,           ZbFsciHandler},
    {mFsciMsgZllTouchlinkCommissioningConfigure_c,       ZbFsciHandler},
    {mFsciMsgZllTouchlinkGetListOfCommissionedDevices_c, ZbFsciHandler},
    {mFsciMsgZllTouchlinkRemoveEntry_c,                  ZbFsciHandler},    
    
    {mFsciMsgClearNeighborTableEntry_c,      ZbFsciHandler},
    
    {mFsciMsgAddToAddressMapPermanent_c,     ZbFsciHandler},
    {mFsciMsgRemoveFromAddressMap_c,         ZbFsciHandler},

    {mFsciOtaSupportQueryImageReq_c,         ZbFsciHandler}, //not used by MAC
    {mFsciOtaSupportQueryImageRsp_c,         ZbFsciHandler},
    {mFsciOtaSupportImageNotifyReq_c,        ZbFsciHandler},

    {mFsciMsgReadNwkMngAddressReq_c,         ZbFsciHandler},
    {mFsciMsgWriteNwkMngAddressReq_c,        ZbFsciHandler},
    {mFsciMsgStopNwkReq_c,                   ZbFsciHandler},
    {mFsciMsgStartNwkReq_c,                  ZbFsciHandler},
    {mFsciMsgRestartNwkReq_c,                ZbFsciHandler},
    {mFsciMsgStartNwkExReq_c,                ZbFsciHandler},
    {mFsciMsgStopNwkExReq_c,                 ZbFsciHandler},
    {mFsciMsgReadExtendedAdrReq_c,           ZbFsciHandler},
    {mFsciMsgWriteExtendedAdrReq_c,          ZbFsciHandler},
#else
    {mFsciMsgReadExtendedAdrReq_c,           FSCI_MsgReadExtendedAdrReqFunc},
    {mFsciMsgWriteExtendedAdrReq_c,          FSCI_MsgWriteExtendedAdrReqFunc},
#endif

    {mFsciLowLevelMemoryWriteBlock_c,        FSCI_WriteMemoryBlock},
    {mFsciLowLevelMemoryReadBlock_c,         FSCI_ReadMemoryBlock},
    {mFsciLowLevelPing_c,                    FSCI_Ping},

    {mFsciOtaSupportImageNotifyReq_c,        FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportStartImageReq_c,         FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportSetModeReq_c,            FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportQueryImageRsp_c,         FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportPushImageChunkReq_c,     FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportCommitImageReq_c,        FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportCancelImageReq_c,        FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportSetFileVerPoliciesReq_c, FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportAbortOTAUpgradeReq_c,    FSCI_OtaSupportHandlerFunc},
    {mFsciOtaSupportGetClientInfo_c,         FSCI_OtaSupportHandlerFunc},

/* Bootloader cmd */
    {mFsciEnableBootloaderReq_c,             FSCI_EnableBootloaderFunc},

#if gFSCI_IncludeLpmCommands_c
    {mFsciMsgAllowDeviceToSleepReq_c,        FSCI_MsgAllowDeviceToSleepReqFunc},
    {mFsciMsgGetWakeUpReasonReq_c,           FSCI_MsgGetWakeUpReasonReqFunc},
#endif

    {mFsciGetUniqueId_c,                     FSCI_ReadUniqueId},
    {mFsciGetMcuId_c,                        FSCI_ReadMCUId},
    {mFsciGetSwVersions_c,                   FSCI_ReadModVer},
};

/* Used for maintaining backward compatibillity */
static const opGroup_t mFsciModeSelectSAPs[] =
{
    gFSCI_McpsSapId_c,
    gFSCI_MlmeSapId_c,
    gFSCI_AspSapId_c,
    gFSCI_NldeSapId_c,
    gFSCI_NlmeSapId_c,
    gFSCI_AspdeSapId_c,
    gFSCI_AfdeSapId_c,
    gFSCI_ApsmeSapId_c,
    gFSCI_ZdpSapId_c,
};

#if gFSCI_IncludeLpmCommands_c
uint8_t mFsciInterfaceToSendWakeUp;
static FsciWakeUpConfig_t  mFsciWakeUpConfig =
{
    FALSE,       /* WakeUp.Ind message is NOT sent when wake up */
    0x0003D090   /* deep sleep duration 4 seconds */
};
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief   This is the handler function for the FSCI OpGroup.
*          It calls the handler function for the received OpCode.
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
********************************************************************************** */
void fsciMsgHandler( void* pData, uint32_t fsciInterface )
{
    uint32_t i;

    /* Call the handler function for the received OpCode */
    for ( i = 0; i < NumberOfElements(FSCI_ReqOCtable); i++ )
    {
        if( FSCI_ReqOCtable[i].opCode == ((clientPacket_t*)pData)->structured.header.opCode )
        {
            if( FSCI_ReqOCtable[i].pfOpCodeHandle( pData, fsciInterface ) )
            {
                /* Reuse received message */
                ((clientPacket_t*)pData)->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
                FSCI_transmitFormatedPacket( pData, fsciInterface );
            }
            break;
        }
    }

    /* If handler function was not found, send error message */
    if( i >= NumberOfElements(FSCI_ReqOCtable) )
    {
        MEM_BufferFree( pData );
        FSCI_Error( gFsciUnknownOpcode_c, fsciInterface );
    }
}

/*! *********************************************************************************
* \brief  Send an error message back to the external client.
*         This function should not block even if there is no more dynamic memory available
*
* \param[in] errorCode the erros encountered
* \param[in] fsciInterface the interface on which the packet was received
*
*
********************************************************************************** */
void FSCI_Error(uint8_t errorCode, uint32_t fsciInterface)
{
    uint32_t virtInterface = FSCI_GetVirtualInterface(fsciInterface);
    uint8_t size = sizeof(mFsciErrorMsg)-1;

    /* Don't cascade error messages. */
    if( !mFsciErrorReported )
    {
        mFsciErrorMsg.status = errorCode;
        mFsciErrorMsg.checksum = FSCI_computeChecksum( &mFsciErrorMsg.header.opGroup, size-2 );
        
        if( virtInterface )
        {
#if gFsciMaxVirtualInterfaces_c
            mFsciErrorMsg.checksum2  = mFsciErrorMsg.checksum;
            mFsciErrorMsg.checksum  += virtInterface;
            mFsciErrorMsg.checksum2 ^= mFsciErrorMsg.checksum;
            size++;
#else
            (void)virtInterface;
#endif
        }
        
        Serial_SyncWrite( gFsciSerialInterfaces[fsciInterface], (uint8_t*)&mFsciErrorMsg, size );
        
        mFsciErrorReported = TRUE;
    }
}

#if gFsciTxAck_c
/*! *********************************************************************************
* \brief  Send an ack message back to the external client.
*
* \param[in] checksum of the packet received
* \param[in] fsciInterface the interface on which the packet was received
*
*
********************************************************************************** */
void FSCI_Ack(uint8_t checksum, uint32_t fsciInterface)
{
    uint32_t virtInterface = FSCI_GetVirtualInterface(fsciInterface);
    uint8_t size = sizeof(mFsciAckMsg)-1;

    mFsciAckMsg.checksumPacketReceived = checksum;
    mFsciAckMsg.checksum = FSCI_computeChecksum( &mFsciAckMsg.header.opGroup, size-2 );

    if( virtInterface )
    {
#if gFsciMaxVirtualInterfaces_c
        mFsciAckMsg.checksum2  = mFsciAckMsg.checksum;
        mFsciAckMsg.checksum  += virtInterface;
        mFsciAckMsg.checksum2 ^= mFsciAckMsg.checksum;
        size++;
#else
        (void)virtInterface;
#endif
    }

    Serial_SyncWrite( gFsciSerialInterfaces[fsciInterface], (uint8_t*)&mFsciAckMsg, size );
}
#endif 

#if gFsciHostSupport_c
/*! *********************************************************************************
* \brief  Send a CPU-Reset Request to a FSCI black box
*
* \param[in] fsciInterface the interface to send the packet on
*
********************************************************************************** */
gFsciStatus_t FSCI_ResetReq(uint32_t fsciInterface)
{
    clientPacket_t *pFsciPacket = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + 2);
    gFsciStatus_t  status = gFsciSuccess_c;
    uint8_t        checksum = 0;
    uint8_t        size = 0;
    
    if( NULL == pFsciPacket ) 
    {
        status = gFsciOutOfMessages_c;
    }
    else
    {
        pFsciPacket->structured.header.startMarker = gFSCI_StartMarker_c;
        pFsciPacket->structured.header.opGroup = gFSCI_ReqOpcodeGroup_c;
        pFsciPacket->structured.header.opCode = mFsciMsgResetCPUReq_c;
        pFsciPacket->structured.header.len = 0;
        size = sizeof(clientPacketHdr_t) + 1;
        checksum = FSCI_computeChecksum(pFsciPacket->raw+1, size - 2);
        pFsciPacket->structured.payload[0] = checksum;
        Serial_SyncWrite(gFsciSerialInterfaces[fsciInterface], pFsciPacket->raw, size);    
        MEM_BufferFree(pFsciPacket);
    }
    
    return status;
}
#endif

/*! *********************************************************************************
* \brief   Set FSCI operating mode for certain OpGroups
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_MsgModeSelectReqFunc(void* pData, uint32_t fsciInterface)
{
    uint8_t i;
    uint8_t payloadIndex = 0;
    gFsciOpGroup_t *p;

    fsciLen_t dataLen = ((clientPacket_t*)pData)->structured.header.len;

    if( dataLen > 0 )
    {
        /* gFsciTxBlocking = ((clientPacket_t*)pData)->structured.payload[payloadIndex] */
        payloadIndex++;
        dataLen--;
    }

    for( i = 0; i < dataLen; i++ )
    {
        p = FSCI_GetReqOpGroup(mFsciModeSelectSAPs[i], fsciInterface);
        if( NULL != p )
        {
            p->mode= ((clientPacket_t*)pData)->structured.payload[payloadIndex + i];
        }
    }

    ((clientPacket_t*)pData)->structured.payload[0] = gFsciSuccess_c;
    ((clientPacket_t*)pData)->structured.header.len = sizeof(uint8_t);
    return TRUE;
}

/*! *********************************************************************************
* \brief   Returns FSCI operating mode for certain OpGroups
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_MsgGetModeReqFunc(void* pData, uint32_t fsciInterface)
{
    uint8_t i;
    uint8_t payloadIndex = 0;
    gFsciOpGroup_t *p;

    ((clientPacket_t*)pData)->structured.payload[payloadIndex++] = gFsciSuccess_c;
    ((clientPacket_t*)pData)->structured.payload[payloadIndex++] = gFsciTxBlocking;

    for( i = 0; i < NumberOfElements(mFsciModeSelectSAPs); i++ )
    {
        p = FSCI_GetReqOpGroup(mFsciModeSelectSAPs[i], fsciInterface);
        if( NULL != p )
        {
            ((clientPacket_t*)pData)->structured.payload[payloadIndex++] = p->mode;
        }
        else
        {
            ((clientPacket_t*)pData)->structured.payload[payloadIndex++] = gFsciInvalidMode;
        }
    }

    ((clientPacket_t*)pData)->structured.header.len = payloadIndex;
    return TRUE;
}

/*! *********************************************************************************
* \brief   Function used for writing to RAM memory.
*          Payload contains the packet received over the serial interface
*          bytes 0-3 --> start address for writing
*          byte  4   --> number of bytes to be written
*          bytes 5+  --> data to be written starting with start address.
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_WriteMemoryBlock(void* pData, uint32_t fsciInterface)
{
    uint16_t len;
    uint8_t *addr;

    FLib_MemCpy(&addr, ((clientPacket_t*)pData)->structured.payload, sizeof(uint8_t*));
    len = ((clientPacket_t*)pData)->structured.payload[sizeof(uint8_t*)];

    /* Check RAM boundaries */
    if( (mRamStartAddress_c <= (uint32_t)addr) && (mRamEndAddress_c >= (uint32_t)addr))
    {
        FLib_MemCpy(addr, &((clientPacket_t*)pData)->structured.payload[sizeof(uint8_t*) + 1], len);
    }
    else
    {
        FSCI_Error(gFsciError_c, fsciInterface);
        len = 0;
    }

    ((clientPacket_t*)pData)->structured.header.len = sizeof(len);
    ((clientPacket_t*)pData)->structured.payload[0] = len;
    return TRUE;
}

/*! *********************************************************************************
* \brief   Function used for reading from RAM memory.
*          Payload contains the packet received over the serial interface
*          bytes 0-3 --> start address for reading
*          byte  4   --> number of bytes to read
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_ReadMemoryBlock(void* pData, uint32_t fsciInterface)
{
    clientPacket_t *pPkt;
    uint16_t len;
    uint8_t *addr;

    FLib_MemCpy(&addr, ((clientPacket_t*)pData)->structured.payload, sizeof(uint8_t*));
    len = ((clientPacket_t*)pData)->structured.payload[sizeof(uint8_t*)];

    if( MEM_BufferGetSize(pData) >= (sizeof(clientPacketHdr_t) + len + 2) )
    {
        pPkt = pData;
    }
    else
    {
        pPkt = MEM_BufferAlloc( sizeof(clientPacketHdr_t) + len + 2 );
    }

    if( !pPkt )
    {
        FSCI_Error( gFsciOutOfMessages_c, fsciInterface );
        MEM_BufferFree(pData);
        return FALSE;
    }

    /* Check RAM and FLASH boundaries */
    if (((mRamStartAddress_c <= (uint32_t)addr) && ((uint32_t)addr <= mRamEndAddress_c)) ||
        ((mFlashStartAddress_c < (uint32_t)addr) && ((uint32_t)addr <= mFlashEndAddress_c)))
    {
        FLib_MemCpy(pPkt->structured.payload, addr, len);
    }
    else
    {
        FSCI_Error(gFsciError_c, fsciInterface);
        MEM_BufferFree(pData);
        return FALSE;
    }

    pPkt->structured.header.len = len;

    /* Check if the received buffer was reused. */
    if( pPkt == pData )
    {
        return TRUE;
    }

    /* A new buffer was allocated. Fill with aditional information */
    pPkt->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
    pPkt->structured.header.opCode = mFsciLowLevelMemoryReadBlock_c;
    FSCI_transmitFormatedPacket( pPkt, fsciInterface );
    MEM_BufferFree(pData);

    return FALSE;
}

/*! *********************************************************************************
* \brief  This function simply echoes back the payload
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
* \remarks Remarks: if USB communication is used, the connection will be lost
*
********************************************************************************** */
bool_t  FSCI_Ping(void* pData, uint32_t fsciInterface)
{
    /* Nothing to do here */
    return TRUE;
}

/*! *********************************************************************************
* \brief  This function resets the MCU
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
* \remarks Remarks: if USB communication is used, the connection will be lost
*
********************************************************************************** */
bool_t FSCI_MsgResetCPUReqFunc(void* pData, uint32_t fsciInterface)
{
#if gFSCI_IncludeMacCommands_c && gFsciHost_802_15_4_c    
    /* Get Host FSCI interface for MAC instance and forward packet */
    clientPacket_t *pFsciPacket = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + 9);
    uint8_t checksum;
    uint8_t size;
    
    if( NULL == pFsciPacket ) 
    {
        FSCI_Error( gFsciOutOfMessages_c, fsciInterface );
        return FALSE;
    }
    else
    {
        FLib_MemCpy(pFsciPacket, pData, sizeof(clientPacketHdr_t));
        pFsciPacket->structured.header.startMarker = gFSCI_StartMarker_c;
        size = sizeof(clientPacketHdr_t) + pFsciPacket->structured.header.len + 1 /* CRC */;
        
        /* Compute Checksum */
        checksum = FSCI_computeChecksum(pFsciPacket->raw+1, size - 2);
        pFsciPacket->structured.payload[pFsciPacket->structured.header.len] = checksum;
        
        Serial_SyncWrite(gFsciSerialInterfaces[fsciHostGetMacInterfaceId(fsciGetMacInstanceId(fsciInterface))], 
                         pFsciPacket->raw, 
                         size+9); /* additional bytes to allow last byte reception */
        
        MEM_BufferFree(pFsciPacket);
    }
#endif  
  
#if gFSCI_ResetCpu_c
    ResetMCU();
#endif

    MEM_BufferFree(pData);
    return FALSE;
}

/*! *********************************************************************************
* \brief  This function writes the MAC Extended Address into the MAC layer
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
* \remarks Remarks: this function is legacy
*
********************************************************************************** */
bool_t FSCI_MsgWriteExtendedAdrReqFunc(void* pData, uint32_t fsciInterface)
{
#if gFSCI_IncludeMacCommands_c

#if gFsciHost_802_15_4_c
    /* Get Host FSCI interface for MAC instance and forward packet */
    clientPacket_t *pFsciPacket = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + sizeof(uint64_t) + 2);
    if( NULL == pFsciPacket ) 
    {
        FSCI_Error( gFsciOutOfMessages_c, fsciInterface );
        ((clientPacket_t*)pData)->structured.payload[0] = gFsciOutOfMessages_c;
        ((clientPacket_t*)pData)->structured.header.len = sizeof(clientPacketStatus_t);
        return FALSE;
    }
    else
    {
        FLib_MemCpy(pFsciPacket, pData, sizeof(clientPacketHdr_t) + sizeof(uint64_t));
        FSCI_transmitFormatedPacket( pFsciPacket, fsciHostGetMacInterfaceId(fsciGetMacInstanceId(fsciInterface)) );
    }
#else
    Mac_SetExtendedAddress( ((clientPacket_t*)pData)->structured.payload, fsciGetMacInstanceId(fsciInterface) );
#endif
    
#if gRF4CEIncluded_d
    extern uint8_t aExtendedAddress[8];
    FLib_MemCpy(aExtendedAddress,((clientPacket_t*)pData)->structured.payload, 8);
#endif

    ((clientPacket_t*)pData)->structured.payload[0] = gFsciSuccess_c;
#else
    ((clientPacket_t*)pData)->structured.payload[0] = gFsciRequestIsDisabled_c;
#endif /* gFSCI_IncludeMacCommands_c */
    ((clientPacket_t*)pData)->structured.header.len = sizeof(clientPacketStatus_t);
    return TRUE;
}

/*! *********************************************************************************
* \brief  This function sends the MAC Extended Address over the serial interface
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
* \remarks Remarks: this function is legacy
*
********************************************************************************** */
bool_t FSCI_MsgReadExtendedAdrReqFunc(void* pData, uint32_t fsciInterface)
{
#if gFSCI_IncludeMacCommands_c && !gFsciHost_802_15_4_c
    Mac_GetExtendedAddress( &((clientPacket_t*)pData)->structured.payload[sizeof(clientPacketStatus_t)], fsciGetMacInstanceId(fsciInterface) );

    ((clientPacket_t*)pData)->structured.payload[0] = gFsciSuccess_c;
    ((clientPacket_t*)pData)->structured.header.len = sizeof(clientPacketStatus_t)
                                                      + sizeof(uint64_t);
#else
    ((clientPacket_t*)pData)->structured.payload[0] = gFsciRequestIsDisabled_c;
    ((clientPacket_t*)pData)->structured.header.len = sizeof(clientPacketStatus_t);
#endif
    return TRUE;
}

/*! *********************************************************************************
* \brief  This function sends the LQI value for the last received packet over
*         the serial interface
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_GetLastLqiValue(void* pData, uint32_t fsciInterface)
{
    ((clientPacket_t*)pData)->structured.payload[0] = PhyGetLastRxLqiValue();
    ((clientPacket_t*)pData)->structured.header.len = sizeof(clientPacketStatus_t);
    return TRUE;
}

#if gFSCI_IncludeLpmCommands_c

__WEAK_FUNC bool_t PWR_ChangeDeepSleepMode(uint8_t dsMode)
{
  (void)dsMode;
  return TRUE;
}
/*! *********************************************************************************
* \brief Allow the MCU to enter LowPower
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_MsgAllowDeviceToSleepReqFunc(void* pData, uint32_t fsciInterface)
{
    uint8_t dsMode = PWR_GetDeepSleepMode();
    mFsciInterfaceToSendWakeUp = (uint8_t)fsciInterface;
    /* Set the new configuration */
    FLib_MemCpy(&mFsciWakeUpConfig,
                ((clientPacket_t*)pData)->structured.payload,
                sizeof(mFsciWakeUpConfig));
    if(((clientPacket_t*)pData)->structured.header.len > sizeof(mFsciWakeUpConfig))
    {
      dsMode = ((clientPacket_t*)pData)->structured.payload[sizeof(mFsciWakeUpConfig)];
    }
    if( mFsciWakeUpConfig.deepSleepDuration < 10 )
        ((clientPacket_t*)pData)->structured.payload[0] = gFsciError_c;
    else
        ((clientPacket_t*)pData)->structured.payload[0] = gFsciSuccess_c;

    ((clientPacket_t*)pData)->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
    ((clientPacket_t*)pData)->structured.header.len = sizeof(clientPacketStatus_t);

    // Perform a Sync Tx
    gFsciTxBlocking = TRUE;
    FSCI_transmitFormatedPacket(pData, fsciInterface);
    gFsciTxBlocking = FALSE;

    if( mFsciWakeUpConfig.deepSleepDuration >= 10 )
    {
        PWR_ChangeDeepSleepMode(dsMode);
        PWR_SetDeepSleepTimeInSymbols(mFsciWakeUpConfig.deepSleepDuration);
        PWR_AllowDeviceToSleep();  /* Allow device to sleep */
    }

    return FALSE;
}
/*! *********************************************************************************
* \brief Read the MCU wake-up reason
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_MsgGetWakeUpReasonReqFunc(void* pData, uint32_t fsciInterface)
{
    MEM_BufferFree(pData);
    FSCI_transmitPayload( gFSCI_CnfOpcodeGroup_c, mFsciMsgGetWakeUpReasonReq_c, (void*)&PWRLib_MCU_WakeupReason, (uint16_t)sizeof(PWRLib_WakeupReason_t), fsciInterface);
    return FALSE;
}

/*! *********************************************************************************
* \brief Notify that the MCU has exit LowPower mode
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
void FSCI_SendWakeUpIndication( void )
{
    clientPacket_t *pPkt;

    PWR_DisallowDeviceToSleep();  /* Disallow device to sleep */

    if( mFsciWakeUpConfig.signalWhenWakeUpFlag )
    {
        pPkt = MEM_BufferAlloc(sizeof(clientPacketHdr_t) +
                               sizeof(clientPacketStatus_t) + 2);

        if( !pPkt )
            return;

        pPkt->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
        pPkt->structured.header.opCode = mFsciMsgWakeUpIndication_c;
        pPkt->structured.header.len = sizeof(clientPacketStatus_t);
        pPkt->structured.payload[0] = gFsciSuccess_c;
        FSCI_transmitFormatedPacket( pPkt, mFsciInterfaceToSendWakeUp );
    }
}
#endif /* #if gFSCI_IncludeLpmCommands_c */

/*! *********************************************************************************
* \brief  This function sends the content of the SIM_UID registers over the
*         serial interface
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_ReadUniqueId(void* pData, uint32_t fsciInterface)
{
    clientPacket_t *pPkt;
    uint8_t *p;
    bool_t status; 
    uint32_t size = sizeof(clientPacketHdr_t) + 4*sizeof(uint32_t) + 2*sizeof(uint8_t);
    
    /* Check if the received buffer is large enough to be reused */
    if( MEM_BufferGetSize(pData) >= size )
    {
        pPkt = pData;
    }
    else
    {
        pPkt = MEM_BufferAlloc(size);
    }

    if( !pPkt )
    {
        FSCI_Error(gFsciOutOfMessages_c, fsciInterface);
        MEM_BufferFree(pData);
        status = FALSE;
    }
    else
    {
        p = pPkt->structured.payload;
        pPkt->structured.header.len = 4*sizeof(uint32_t);
#if defined(SIM_UIDH_UID_MASK)
        FLib_MemCpy(p, (void*)&SIM->UIDH, sizeof(uint32_t));
#else
        FLib_MemSet(p, 0, sizeof(uint32_t));
#endif
        p += sizeof(uint32_t);
        FLib_MemCpy(p, (void*)&SIM->UIDMH, sizeof(uint32_t));
        p += sizeof(uint32_t);
        FLib_MemCpy(p, (void*)&SIM->UIDML, sizeof(uint32_t));
        p += sizeof(uint32_t);
        FLib_MemCpy(p, (void*)&SIM->UIDL, sizeof(uint32_t));

        /* Check if the received buffer was reused. */
        if( pPkt != pData )
        {
            /* A new buffer was allocated. Fill with aditional information */
            pPkt->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
            pPkt->structured.header.opCode = mFsciGetUniqueId_c;
            FSCI_transmitFormatedPacket(pPkt, fsciInterface);
            MEM_BufferFree(pData);
            status = FALSE;
        }
        else
        {
            status = TRUE;
        }
    }
    
    return status;
}

/*! *********************************************************************************
* \brief  This function sends the content of the SIM_SDID register over the
*         serial interface
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_ReadMCUId(void* pData, uint32_t fsciInterface)
{
    clientPacket_t *pPkt = (clientPacket_t*)pData;

    pPkt->structured.header.len = sizeof(uint32_t);
    FLib_MemCpy(pPkt->structured.payload, (void*)&SIM->SDID, sizeof(uint32_t));

    return TRUE;
}

/*! *********************************************************************************
* \brief  This function reads all module information located in section VERSION_TAGS
*         and sends this information over the serial interface
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_ReadModVer(void* pData, uint32_t fsciInterface)
{
    clientPacket_t *pPkt;
    moduleInfo_t *pInfo = gVERSION_TAGS_startAddr_d;
    uint16_t size = sizeof(clientPacketHdr_t) + 2 +
                    gVERSION_TAGS_entries_d * gVERSION_TAGS_entrySizeNoPadding_d;

    /* Check if the received buffer is large enough to be reused */
    if( MEM_BufferGetSize(pData) >= size )
    {
        pPkt = pData;
    }
    else
    {
        pPkt = MEM_BufferAlloc( size );
    }

    if( !pPkt )
    {
        FSCI_Error( gFsciOutOfMessages_c, fsciInterface );
        MEM_BufferFree(pData);
        return FALSE;
    }

    pPkt->structured.payload[0] = gVERSION_TAGS_entries_d;
    size = sizeof(uint8_t);

    while( pInfo < gVERSION_TAGS_endAddr_d )
    {
        FLib_MemCpy( &pPkt->structured.payload[size],
                     &pInfo->moduleId,
                     gVERSION_TAGS_entrySizeNoPadding_d -
                     GetSizeOfMember(moduleInfo_t, moduleString) );
        size += gVERSION_TAGS_entrySizeNoPadding_d - GetSizeOfMember(moduleInfo_t, moduleString);
        pInfo++;
    }

    pPkt->structured.header.len = (uint8_t)size;

    /* Check if the received buffer was reused. */
    if( pPkt == pData )
    {
        return TRUE;
    }

    /* A new buffer was allocated. Fill with aditional information */
    pPkt->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
    pPkt->structured.header.opCode = mFsciGetSwVersions_c;
    FSCI_transmitFormatedPacket( pPkt, fsciInterface );
    MEM_BufferFree(pData);

    return FALSE;
}

/*! *********************************************************************************
* \brief  This function handles the requests for the OTA OpCodes
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE in order to recycle the received message
*
********************************************************************************** */
bool_t FSCI_OtaSupportHandlerFunc(void* pData, uint32_t fsciInterface)
{
#if gFSCI_IncludeMacCommands_c && gFsciHost_802_15_4_c
    Serial_SyncWrite( gFsciSerialInterfaces[fsciHostGetMacInterfaceId(fsciGetMacInstanceId(fsciInterface))], 
                      pData, sizeof(clientPacketHdr_t) + ((clientPacket_t*)pData)->structured.header.len + 1);
#else
    if( pfFSCI_OtaSupportCalback )
    {
        if( pfFSCI_OtaSupportCalback((clientPacket_t*)pData) )
        {
            return TRUE;
        }
    }
#endif
    MEM_BufferFree(pData);
    return FALSE;
}
/*! *********************************************************************************
* \brief  This function handles the requests for enable the MSD Bootloader
*
* \param[in] pData pointer to location of the received data
* \param[in] fsciInterface the interface on which the packet was received
*
* \return  TRUE if the received message was recycled, FALSE if it must be deleted
*
********************************************************************************** */
bool_t FSCI_EnableBootloaderFunc(void* pData, uint32_t fsciInterface)
{
   #ifdef USE_MSD_BOOTLOADER
       (void)NV_FlashEraseSector(0xBFF8, FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE);
   #else
       uint32_t sectorAddress = 0x3800;
       uint64_t flag = 0;
       (void)NV_FlashProgramUnaligned(sectorAddress, sizeof(flag), (uint8_t*)&flag);
   #endif
   ResetMCU();
   return FALSE;
}


#endif /* gFsciIncluded_c */