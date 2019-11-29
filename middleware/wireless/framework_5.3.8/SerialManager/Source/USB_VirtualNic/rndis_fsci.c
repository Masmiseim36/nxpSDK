/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* This is the source file for the Serial Manager.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "FsciInterface.h"
#include "FsciCommunication.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "rndis_fsci.h"
#include "rndis.h"
#if gNvStorageIncluded_d
#include "NVM_Interface.h"
#endif
/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define mMacAddrLen_c	6
#define mFsciHost_WaitForStatusTimeout_us_c  (gFsciHost_WaitForStatusTimeout_c * 1000)
/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
typedef struct mFsciRegMacAddrReq_tag
{
  rndisMCastMacAddType_t addType; 
  bool_t                 write;
  uint8_t                addr[mMacAddrLen_c];
} mFsciRegMacAddrReq_t;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static void fsciRndisReqHandler(void *pData, void* param, uint32_t interfaceId);
static void RNDIS_Monitor( void* pBuff, uint16_t length );

#if gFsciHost_Rndis_c
static void fsciRndisCnfHandler(void *pData, void* param, uint32_t interfaceId);
static rndisStatus_t Rndis_CmdMonitor(uint8_t opCode, uint8_t* pBuff, uint16_t length);
static rndisStatus_t Rndis_GetCmdStatus(void);
#endif

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

#if gFsciIncluded_c && gFsciHost_Rndis_c
#if gFsciHostSupport_c
  extern clientPacket_t *pFsciHostSyncRsp;

  #if gFsciHostSyncUseEvent_c
    extern osaEventId_t gFsciHostSyncRspEventId;
  #endif
#else
  #error Please enable also gFsciHostSupport_c to allow Host RNDIS module to work.
#endif
#endif

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
static uint32_t mFsciInterface = mFsciInvalidInterface_c;

#if gFsciHost_Rndis_c
static uint32_t mFsciHostInterface = mFsciInvalidInterface_c;
static pfRNDISRxCallback_t mpfFsciRndisRxCb = NULL;
#endif

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief   This function registers a FSCI RNDIS module and request handler
*
* \param[in] fsci interfaceId
*
* \return None.
*
********************************************************************************** */
void fsciRegisterRndis( uint32_t fsciInterfaceId )
{
  mFsciInterface = fsciInterfaceId;
  
  /* Register Handler for requests coming from the serial interface */
  FSCI_RegisterOpGroup( gFSCI_RndisReqOpcodeGroup_c, 
                        gFsciMonitorMode_c, 
                        fsciRndisReqHandler, 
                        NULL, 
                        fsciInterfaceId );	
}

/*! *********************************************************************************
* \brief   This function registers a FSCI RNDIS module as host and confirm handler
*
* \param[in] fsci interfaceId
*
* \return None.
*
********************************************************************************** */
void fsciRegisterRndisToHost( uint32_t fsciInterfaceId )
{
#if gFsciHost_Rndis_c
  mFsciHostInterface = fsciInterfaceId;
  
  /* Register Handler for confirms and indications coming from the serial interface */
  FSCI_RegisterOpGroup( gFSCI_RndisCnfOpcodeGroup_c, 
                        gFsciMonitorMode_c, 
                        fsciRndisCnfHandler, 
                        NULL, 
                        fsciInterfaceId );	
#endif
}

/*! *********************************************************************************
* \brief   This function monitors RNDIS data indications and sends them over the serial interface
*
* \param[in] buffer received
* \param[in] length of buffer received
*
* \return None.
*
********************************************************************************** */
void RNDIS_Monitor(void* pBuff, uint16_t length)
{
#if gFsciIncluded_c
  clientPacket_t *pFsciPacket = NULL;
  uint8_t        *p = NULL;
  uint16_t       size = sizeof(clientPacketHdr_t) + sizeof(uint16_t) + length + 2 * sizeof(uint8_t);
  uint16_t       buffSize = MEM_BufferGetSize(pBuff);
  
  if( buffSize > size )
  {
    pFsciPacket = pBuff;
    p = pFsciPacket->structured.payload;
    FLib_MemInPlaceCpy(p+sizeof(uint16_t), pBuff, length);
    *p++ = length & 0xFF;
    *p++ = (length >> 8) & 0xFF;
  }
  else
  {
    pFsciPacket = MEM_BufferAlloc(size);
    if( pFsciPacket )
    {
        p = pFsciPacket->structured.payload;
        *p++ = length & 0xFF;
        *p++ = (length >> 8) & 0xFF;
        FLib_MemCpy(p, pBuff, length);
    }
  }
  
  if( pFsciPacket )
  {  
    pFsciPacket->structured.header.opGroup = gFSCI_RndisCnfOpcodeGroup_c;
    pFsciPacket->structured.header.opCode = mFsciRndisDataIndOpcode_c;
    pFsciPacket->structured.header.len = sizeof(uint16_t) + length;
    
    FSCI_transmitFormatedPacket(pFsciPacket, mFsciInterface);
  }
  
  if( pBuff != pFsciPacket )
  {
    MEM_BufferFree(pBuff);
  }
#endif
}

#if gFsciHost_Rndis_c
/*! *********************************************************************************
* \brief   Host implementation of RNDIS API
*
* \return staus of the operation.
*
********************************************************************************** */
rndisStatus_t RNDIS_Init()
{
    rndisStatus_t status = gRndis_Success_c;
    
    FSCI_HostSyncLock(mFsciHostInterface, gFSCI_RndisCnfOpcodeGroup_c, mFsciRndisInitCnfOpcode_c);
#if gNvStorageIncluded_d
    NvSetCriticalSection();
#endif                
    status = Rndis_CmdMonitor(mFsciRndisInitReqOpcode_c, NULL, 0);
    if( gRndis_Success_c == status )
    {
        status = Rndis_GetCmdStatus();
    }
#if gNvStorageIncluded_d
    NvClearCriticalSection();
#endif                                                      
    FSCI_HostSyncUnlock(mFsciHostInterface);
    
    return status;
}

/*! *********************************************************************************
* \brief   Host implementation of RNDIS API
*
* \return staus of the operation.
*
********************************************************************************** */
rndisStatus_t RNDIS_Send(uint8_t* pBuff, uint16_t length)
{
    rndisStatus_t status = gRndis_Success_c;
    
    FSCI_HostSyncLock(mFsciHostInterface, gFSCI_RndisCnfOpcodeGroup_c, mFsciRndisSendCnfOpcode_c);
#if gNvStorageIncluded_d
    NvSetCriticalSection();
#endif                
    status = Rndis_CmdMonitor(mFsciRndisSendReqOpcode_c, pBuff, length);
    if( gRndis_Success_c == status )
    {
        status = Rndis_GetCmdStatus();
    }
#if gNvStorageIncluded_d
    NvClearCriticalSection();
#endif                                                      
    FSCI_HostSyncUnlock(mFsciHostInterface);
    
    MEM_BufferFree(pBuff);
    
    return status;   
}

/*! *********************************************************************************
* \brief   Host implementation of RNDIS API
*
* \return staus of the operation.
*
********************************************************************************** */
rndisStatus_t RNDIS_SetDestMacAddress(uint8_t* pBuff)
{
    rndisStatus_t status = gRndis_Success_c;
    
    FSCI_HostSyncLock(mFsciHostInterface, gFSCI_RndisCnfOpcodeGroup_c, mFsciRndisSetDestMacAddrCnfOpcode_c);
#if gNvStorageIncluded_d
    NvSetCriticalSection();
#endif                
    status = Rndis_CmdMonitor(mFsciRndisSetDestMacAddrReqOpcode_c, pBuff, mMacAddrLen_c);
    if( gRndis_Success_c == status )
    {
        status = Rndis_GetCmdStatus();
    }
#if gNvStorageIncluded_d
    NvClearCriticalSection();
#endif                                                      
    FSCI_HostSyncUnlock(mFsciHostInterface);
    
    return status;
}

/*! *********************************************************************************
* \brief   Host implementation of RNDIS API
*
* \return staus of the operation.
*
********************************************************************************** */
rndisStatus_t RNDIS_SetRndisMacAddress(uint8_t* pBuff)
{
    rndisStatus_t status = gRndis_Success_c;
    
    FSCI_HostSyncLock(mFsciHostInterface, gFSCI_RndisCnfOpcodeGroup_c, mFsciRndisSetRndisMacAddrCnfOpcode_c);
#if gNvStorageIncluded_d
    NvSetCriticalSection();
#endif                
    status = Rndis_CmdMonitor(mFsciRndisSetRndisMacAddrReqOpcode_c, pBuff, mMacAddrLen_c);
    if( gRndis_Success_c == status )
    {
        status = Rndis_GetCmdStatus();
    }
#if gNvStorageIncluded_d
    NvClearCriticalSection();
#endif                                                      
    FSCI_HostSyncUnlock(mFsciHostInterface);
    
    return status;
}

/*! *********************************************************************************
* \brief   Host implementation of RNDIS API
*
* \return staus of the operation.
*
********************************************************************************** */
rndisStatus_t RNDIS_EnableIPv6(bool_t en)
{
    rndisStatus_t status = gRndis_Success_c;
    bool_t        enable = en;
    
    FSCI_HostSyncLock(mFsciHostInterface, gFSCI_RndisCnfOpcodeGroup_c, mFsciRndisEnableIpv6CnfOpcode_c);
#if gNvStorageIncluded_d
    NvSetCriticalSection();
#endif                
    status = Rndis_CmdMonitor(mFsciRndisEnableIpv6ReqOpcode_c, &enable, sizeof(bool_t));
    if( gRndis_Success_c == status )
    {
        status = Rndis_GetCmdStatus();
    }
#if gNvStorageIncluded_d
    NvClearCriticalSection();
#endif                                                      
    FSCI_HostSyncUnlock(mFsciHostInterface);
    
    return status;
}

/*! *********************************************************************************
* \brief   Host implementation of RNDIS API
*
* \return staus of the operation.
*
********************************************************************************** */
rndisStatus_t RNDIS_EnableIPv4(bool_t en)
{
    rndisStatus_t status = gRndis_Success_c;
    bool_t        enable = en;
    
    FSCI_HostSyncLock(mFsciHostInterface, gFSCI_RndisCnfOpcodeGroup_c, mFsciRndisEnableIpv4CnfOpcode_c);
#if gNvStorageIncluded_d
    NvSetCriticalSection();
#endif                
    status = Rndis_CmdMonitor(mFsciRndisEnableIpv4ReqOpcode_c, &enable, sizeof(bool_t));
    if( gRndis_Success_c == status )
    {
        status = Rndis_GetCmdStatus();
    }
#if gNvStorageIncluded_d
    NvClearCriticalSection();
#endif                                                      
    FSCI_HostSyncUnlock(mFsciHostInterface);
    
    return status;
}

/*! *********************************************************************************
* \brief   Host implementation of RNDIS API
*
* \return staus of the operation.
*
********************************************************************************** */
rndisStatus_t RNDIS_RegisterRxCallback(pfRNDISRxCallback_t pCallback)
{
  mpfFsciRndisRxCb = pCallback;
  
  return gRndis_Success_c;
}

/*! *********************************************************************************
* \brief   Host implementation of RNDIS API
*
* \return staus of the operation.
*
********************************************************************************** */
rndisStatus_t RNDIS_RegisterMulticastMacAddress(rndisMCastMacAddType_t addType, bool_t write, uint8_t* pBuff)
{
    rndisStatus_t         status = gRndis_Success_c;
    mFsciRegMacAddrReq_t  req;
    
    FSCI_HostSyncLock(mFsciHostInterface, gFSCI_RndisCnfOpcodeGroup_c, mFsciRndisRegisterMulticastMacAddrCnfOpCode_c);
    req.addType = addType;
    req.write = write;
    FLib_MemCpy(req.addr, pBuff, mMacAddrLen_c);
#if gNvStorageIncluded_d
    NvSetCriticalSection();
#endif                
    status = Rndis_CmdMonitor(mFsciRndisRegisterMulticastMacAddrReqOpCode_c, (uint8_t*)&req, 0);
    if( gRndis_Success_c == status )
    {
        status = Rndis_GetCmdStatus();
    }
#if gNvStorageIncluded_d
    NvClearCriticalSection();
#endif                                                      
    FSCI_HostSyncUnlock(mFsciHostInterface);
    
    return status;
}
#endif

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************* */

/*! *********************************************************************************
* \brief This function is the FSCI request handler for a black box running RNDIS
*
* \param[in] fsci packet received
* \param[in] parameter registered
* \param[in] fsci interface on which the packet was received
*
* \return
*
********************************************************************************** */
static void fsciRndisReqHandler(void *pData, void* param, uint32_t interfaceId)
{
  clientPacket_t *pFsciPacket = (clientPacket_t*)pData;
  uint8_t 		 *p = pFsciPacket->structured.payload;
  uint8_t	     *pBuff = NULL;
  uint8_t	     aMacAddr[mMacAddrLen_c];
  uint16_t		 length = 0;
  rndisStatus_t  status = gRndis_Success_c;
  bool_t         write = FALSE;
  rndisMCastMacAddType_t addType = rndisMCastMacAddType_IPv4;
  
  switch(pFsciPacket->structured.header.opCode)
  {
  case mFsciRndisInitReqOpcode_c:
    status = RNDIS_Init();
    if( gRndis_Success_c == status )
    {
      status = RNDIS_RegisterRxCallback(RNDIS_Monitor);
    }
    break;
    
  case mFsciRndisSendReqOpcode_c:
    length = *p++;
    length += *p++ << 8;
    pBuff = MEM_BufferAlloc(length);
    if( pBuff )
    {
      FLib_MemCpy(pBuff, p, length);
      status = RNDIS_Send(pBuff, length);
    }
    else
    {
      status = gRndis_OutOfMemory_c;
    }
    break;
    
  case mFsciRndisSetDestMacAddrReqOpcode_c:
    FLib_MemCpy(aMacAddr, p, mMacAddrLen_c);
    status = RNDIS_SetDestMacAddress(aMacAddr);
    break;
    
  case mFsciRndisSetRndisMacAddrReqOpcode_c:
    FLib_MemCpy(aMacAddr, p, mMacAddrLen_c);
    status = RNDIS_SetRndisMacAddress(aMacAddr);		
    break;
    
  case mFsciRndisEnableIpv6ReqOpcode_c:
    status = RNDIS_EnableIPv6((bool_t)*p);
    break;
    
  case mFsciRndisEnableIpv4ReqOpcode_c:
    status = RNDIS_EnableIPv4((bool_t)*p);
    break;
    
  case mFsciRndisRegisterMulticastMacAddrReqOpCode_c:
    addType = (rndisMCastMacAddType_t)*p++;
    write = (bool_t)*p++;
    FLib_MemCpy(aMacAddr, p, mMacAddrLen_c);
    status = RNDIS_RegisterMulticastMacAddress(addType, write, aMacAddr);
    break;
  }
  
  /* Reuse request FSCI message for confirm, op code is the same as the request */
  pFsciPacket->structured.header.opGroup = gFSCI_RndisCnfOpcodeGroup_c;
  pFsciPacket->structured.header.len = sizeof(rndisStatus_t);
  pFsciPacket->structured.payload[0] = status;
  
  FSCI_transmitFormatedPacket(pFsciPacket, interfaceId);
}

#if gFsciHost_Rndis_c
/*! *********************************************************************************
* \brief This function is the FSCI confirm handler(data indication) for a RNDIS host
*
* \param[in] fsci packet
* \param[in] parameter registered
* \param[in] fsci interface on which the packet was received
*
* \return
*
********************************************************************************** */
static void fsciRndisCnfHandler(void *pData, void* param, uint32_t interfaceId)
{
  uint8_t  *p = ((clientPacket_t*)pData)->structured.payload;
  uint16_t length = 0;
  
  switch(((clientPacket_t*)pData)->structured.header.opCode)
  {
  case mFsciRndisDataIndOpcode_c:
    length = *p++;
    length += *p++ << 8;
    FLib_MemInPlaceCpy(pData, p, length);
    mpfFsciRndisRxCb(pData, length);
    break;
  default:
    MEM_BufferFree(pData);
    break;
  }
}

/*! *********************************************************************************
* \brief This function sends a RNDIS command to the black box
*
* \param[in] op code of the RNDIS request
* \param[in] buffer with the request parameters
* \param[in] length of the buffer
*
* \return
*
********************************************************************************** */
static rndisStatus_t Rndis_CmdMonitor(uint8_t opCode, uint8_t* pBuff, uint16_t length)
{
  clientPacket_t  *pFsciPacket = NULL;
  uint8_t         *p = NULL;
  uint16_t        size = sizeof(clientPacketHdr_t) + 2 * sizeof(uint8_t);
  uint16_t        payloadLen = 0;
  rndisStatus_t   status = gRndis_Success_c;
  
  switch(opCode)
  {
  case mFsciRndisSendReqOpcode_c:
    payloadLen = sizeof(uint16_t) + length;
    break;
    
  case mFsciRndisSetDestMacAddrReqOpcode_c:
  case mFsciRndisSetRndisMacAddrReqOpcode_c:
    payloadLen = mMacAddrLen_c;
    break;
    
  case mFsciRndisEnableIpv6ReqOpcode_c:
  case mFsciRndisEnableIpv4ReqOpcode_c:
    payloadLen = sizeof(bool_t);
    break;
    
  case mFsciRndisRegisterMulticastMacAddrReqOpCode_c:
    payloadLen = sizeof(rndisMCastMacAddType_t) + sizeof(bool_t) + mMacAddrLen_c;
    break;
  }
  
  pFsciPacket = MEM_BufferAlloc(size + payloadLen);
  if( pFsciPacket )
  {
    pFsciPacket->structured.header.opGroup = gFSCI_RndisReqOpcodeGroup_c;
    pFsciPacket->structured.header.opCode = opCode;
    pFsciPacket->structured.header.len = payloadLen;
    p = pFsciPacket->structured.payload;
    
    switch(opCode)
    {
    case mFsciRndisSendReqOpcode_c:
      *p++ = length & 0xFF;
      *p++ = length >> 8;
      FLib_MemCpy(p, pBuff, length);
      break;
      
    case mFsciRndisSetDestMacAddrReqOpcode_c:
    case mFsciRndisSetRndisMacAddrReqOpcode_c:
      FLib_MemCpy(p, pBuff, mMacAddrLen_c);
      break;
      
    case mFsciRndisEnableIpv6ReqOpcode_c:
    case mFsciRndisEnableIpv4ReqOpcode_c:
      *p = *pBuff;
      break;
      
    case mFsciRndisRegisterMulticastMacAddrReqOpCode_c:
      *p++ = ((mFsciRegMacAddrReq_t*)pBuff)->addType;
      *p++ = ((mFsciRegMacAddrReq_t*)pBuff)->write;
      FLib_MemCpy(p, ((mFsciRegMacAddrReq_t*)pBuff)->addr, mMacAddrLen_c);
      break;
    }
    
    FSCI_transmitFormatedPacket(pFsciPacket, mFsciHostInterface);
  }
  else
  {
    status = gRndis_OutOfMemory_c;
  }
  
  return status;
}

/*! *********************************************************************************
* \brief This function gets a RNDIS command result from the black box
*
* \return status of the operation
*
********************************************************************************** */
static rndisStatus_t Rndis_GetCmdStatus(void)
{
    rndisStatus_t status = gRndis_Success_c;
#if gFsciHostSyncUseEvent_c    
    osaEventFlags_t fsciHostSyncRspEventFlags; 
#else
    uint64_t tStamp;
#endif

    /* Wait for result from the serial interface */
#if gFsciHostSyncUseEvent_c  
    OSA_EventWait(gFsciHostSyncRspEventId, 0x00FFFFFF, FALSE, gFsciHost_WaitForStatusTimeout_c, &fsciHostSyncRspEventFlags);
#else
    tStamp = TMR_GetTimestamp();
    while( !pFsciHostSyncRsp )
    {
        FSCI_receivePacket((void*)mFsciHostInterface);
        if(TMR_GetTimestamp() - tStamp > mFsciHost_WaitForStatusTimeout_us_c)
        {
            break;
        }    
    }
#endif  
    if( pFsciHostSyncRsp )
    {
        status = (rndisStatus_t)pFsciHostSyncRsp->structured.payload[0];
        /* Free FSCI packet */
        MEM_BufferFree(pFsciHostSyncRsp);
    }
    else
    {
        status = gRndis_Timeout;
    }
    return status;    
}
#endif