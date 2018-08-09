/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the source file for the Serial Manager.
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


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "SerialManager.h"
#include "Panic.h"
#include "MemManager.h"
#include "Messaging.h"
#include "FunctionLib.h"
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_common.h"
#include <string.h>
#include "VirtualNicInterface.h"
#include "TimersManager.h"
#include "rndis.h"

#if( gVirtualNIC_ReceiveDirectlyFromUSB_d == 0 )
#error "config error: please set gVirtualNIC_ReceiveDirectlyFromUSB_d = 1 "
#endif
#if( gSerialMgrUseUSB_VNIC_c == 0 )
#error "config error: please set gSerialMgrUseUSB_VNIC_c = 1 "
#endif
/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define MAC_ADDR_SIZE                      (6)
#define mMCastMacAddrTableSize_c           (10)                
/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

typedef enum
{
  gIpProtv4_c = 0x0800,
  gIpProtv6_c = 0x86DD,
  gIpProtArp_c = 0x0806,
  gFsciInProt_c = 0x88B5
}ipProtocolType_t;

typedef struct usbenetHdr_tag
{
   uint8_t    dest[6];
   uint8_t    source[6];
   uint8_t    type[2];
}usbenetHdr_t;

typedef struct mCastMacAddrStruct_t
{
    bool_t inUse;
    uint8_t address[6];
}mCastMacAddrStruct_t;
/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static void* RNDIS_EthernetHeaderReceived(void* pEthernetHeader, uint32_t packetLength);
static void RNDIS_EthernetPacketReceived(void* pEthernetPacket, uint32_t packetLength);
static bool_t RNDIS_HeaderPassesFiltering(void* pEthernetHeader);
static void RNDIS_TimerCallback ( void * param );
static void RNDIS_SerialCallBack(void* param);
static bool_t RNDIS_DestMacAddressIsUninitialized(void);
static void RNDIS_InitDestMacAddress(void);
/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
static uint8_t mRndisInterfaceId = gSerialMgrInvalidIdx_c;
const vnicReceiveCallbacksStruct_t mVNICReceiveCallbacks = {RNDIS_EthernetHeaderReceived, RNDIS_EthernetPacketReceived};
tmrTimerID_t mRndisTimerID = gTmrInvalidTimerID_c;
pfRNDISRxCallback_t mpfRNDISRxCallback;
uint32_t mPendingEthPacketLength;
uint8_t maDestMacAddr[MAC_ADDR_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00};
bool_t mIPv6Enabled = TRUE;
bool_t mIPv4Enabled = FALSE;
mCastMacAddrStruct_t maIPv4MCastMacAddrTable[mMCastMacAddrTableSize_c];    
mCastMacAddrStruct_t maIPv6MCastMacAddrTable[mMCastMacAddrTableSize_c];    
/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */


/*! *********************************************************************************
* \brief   
*
********************************************************************************** */
rndisStatus_t RNDIS_Init()
{
  serialStatus_t serialStatus;
  rndisStatus_t rndisStatus = gRndis_InitError;
  static bool_t rndisInit = FALSE;
  if(rndisInit == FALSE)
  {
    rndisInit = TRUE;
    if(RNDIS_DestMacAddressIsUninitialized())
    {
      RNDIS_InitDestMacAddress();
    }
    TMR_Init();
    SerialManager_Init();
    serialStatus =  Serial_InitInterface(&mRndisInterfaceId,gSerialMgrUSB_VNIC_c, 0);
    if ((gSerial_Success_c == serialStatus) && (mRndisInterfaceId != gSerialMgrInvalidIdx_c))
    {
      mRndisTimerID = TMR_AllocateTimer();
      if(mRndisTimerID != gTmrInvalidTimerID_c)
      {
        VirtualNic_RegisterReceiveDirecltyFromUSBCallbacks(&mVNICReceiveCallbacks);  
        rndisStatus = gRndis_Success_c;
      }
    }
  }
  
  return rndisStatus;
}



/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return 
*
********************************************************************************** */

rndisStatus_t RNDIS_Send(uint8_t* pBuff, uint16_t length)
{
  rndisStatus_t rndisStatus = gRndis_InitError;  
  serialStatus_t serialStatus;
  
  if(mRndisInterfaceId != gSerialMgrInvalidIdx_c)
  {
    serialStatus = Serial_AsyncWrite (mRndisInterfaceId, pBuff, length, RNDIS_SerialCallBack, pBuff);  
    if(serialStatus == gSerial_Success_c)
    {
      rndisStatus = gRndis_Success_c;
    }
    else
    {
      MEM_BufferFree(pBuff);
      if(serialStatus == gSerial_OutOfMemory_c)
      {
        rndisStatus = gRndis_OutOfMemory_c;
      }
      else 
      {
        rndisStatus = gRndis_UsbError;
      }
    }
  }
  return rndisStatus;   
}
/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[in] 
* \param[in] 
* \param[in] 
*            
*
* \return The status of the operation
*
********************************************************************************** */
rndisStatus_t RNDIS_SetDestMacAddress(uint8_t* pBuff)
{
  uint8_t i;
  OSA_InterruptDisable();
  for(i=0; i< MAC_ADDR_SIZE ; i++)
  {
      maDestMacAddr[i] = pBuff[i];
  }
  
  OSA_InterruptEnable();
  return gRndis_Success_c;
  
}


/*! *********************************************************************************
* \brief 
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return 
*
********************************************************************************** */

rndisStatus_t RNDIS_SetRndisMacAddress(uint8_t* pBuff)
{
  VirtualNic_SetMacAddress(pBuff);
  return gRndis_Success_c;
}

/*! *********************************************************************************
* \brief 
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return 
*
********************************************************************************** */

rndisStatus_t RNDIS_RegisterMulticastMacAddress(rndisMCastMacAddType_t addType , bool_t write_erase, uint8_t* pBuff)
{
  //mCastMacAddrStruct_t maIPv4MCastMacAddrTable[mMCastMacAddrTableSize_c];    
  mCastMacAddrStruct_t* pMCastAddrTable;
  mCastMacAddrStruct_t* pFirstFreeLocation = NULL;
  mCastMacAddrStruct_t* pAddressMatchLocation = NULL;
  rndisStatus_t status = gRndis_Success_c;
  uint8_t i;
  if(addType == rndisMCastMacAddType_IPv4)
  {
    pMCastAddrTable = maIPv4MCastMacAddrTable;
  }
  else
  {
    pMCastAddrTable = maIPv6MCastMacAddrTable;
  }
  OSA_InterruptDisable();
  for(i=0;i<mMCastMacAddrTableSize_c;i++)
  {
    if(pMCastAddrTable[i].inUse)
    {
      if(FLib_MemCmp (pMCastAddrTable[i].address, pBuff, MAC_ADDR_SIZE))
      {
        pAddressMatchLocation = pMCastAddrTable + i;
        break;
      }
    }
    else
    {
      if(pFirstFreeLocation == NULL)
      {
        pFirstFreeLocation = pMCastAddrTable + i;
      }
    }
  }
  if(write_erase)// write
  {
    if(pAddressMatchLocation == NULL)
    {
      if(pFirstFreeLocation)
      {
       FLib_MemCpy (pFirstFreeLocation->address, pBuff, MAC_ADDR_SIZE);
       pFirstFreeLocation->inUse = TRUE;
      }
      else
      {
        status = gRndis_OutOfMemory_c;
      }
    }
  }
  else//erase
  {
    if(pAddressMatchLocation)
    {
      pAddressMatchLocation->inUse = FALSE;
    }
  }
  OSA_InterruptEnable();
  return status;
}
/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[out] 
* \param[in] 
* \param[out] 
*
* \return 
*
********************************************************************************** */

rndisStatus_t RNDIS_EnableIPv6(bool_t en)
{
  mIPv6Enabled = en;
  return gRndis_Success_c;
}

/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[out] 
*
* \return The status of the operation
*
********************************************************************************** */

rndisStatus_t RNDIS_EnableIPv4(bool_t en)
{
  mIPv4Enabled = en;
  return gRndis_Success_c;
}

/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return The status of the operation
*
********************************************************************************** */
rndisStatus_t RNDIS_RegisterRxCallback(pfRNDISRxCallback_t pCallback)
{
  mpfRNDISRxCallback = pCallback;
  return gRndis_Success_c;
}


/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************* */
/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return The status of the operation
*
********************************************************************************** */
static void* RNDIS_EthernetHeaderReceived(void* pEthernetHeader, uint32_t packetLength)
{
  void* pBuff = NULL;
  if(mpfRNDISRxCallback)
  {
    if(RNDIS_HeaderPassesFiltering(pEthernetHeader))
    {
      pBuff = MEM_BufferAllocWithId(packetLength, gRNDIS_MemPoolId_c, (void*)__get_LR());
      if(pBuff == NULL)
      {
        mPendingEthPacketLength = packetLength;
        TMR_StartIntervalTimer(mRndisTimerID, 5, RNDIS_TimerCallback, &mRndisTimerID);
        pBuff = gVirtualNIC_ReceiveLockBuffer_d;
      }
    }
  }
  
  return pBuff  ;
}
/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return The status of the operation
*
********************************************************************************** */
static void RNDIS_EthernetPacketReceived(void* pEthernetPacket, uint32_t packetLength)
{
  pfRNDISRxCallback_t pfRNDISRxCallback = mpfRNDISRxCallback;
  if(pfRNDISRxCallback)
  {
    pfRNDISRxCallback(pEthernetPacket,(uint16_t)packetLength);
  }
  else
  {
    MEM_BufferFree(pEthernetPacket);
  }
}

/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return The status of the operation
*
********************************************************************************** */


static bool_t RNDIS_HeaderPassesFiltering(void* pEthernetHeader)
{
  bool_t destAddressMatch;
  uint16_t protocol;
  usbenetHdr_t* pEnetHdr = pEthernetHeader;
  protocol = pEnetHdr->type[0];
  protocol <<= 8;
  protocol += pEnetHdr->type[1];
  destAddressMatch = FLib_MemCmp(maDestMacAddr,pEnetHdr->dest,MAC_ADDR_SIZE);
  switch(protocol)
  {
  case gIpProtv4_c:
    if(FALSE == mIPv4Enabled)
    {
      return FALSE;
    }
    break;
  case gIpProtv6_c:
    if(FALSE == mIPv6Enabled)
    {
      return FALSE;
    }
    break;
  case gIpProtArp_c:
    if(FALSE == mIPv4Enabled)
    {
      return FALSE;
    }
    break;
  case gFsciInProt_c:
    return destAddressMatch;
    break;
  
  default:
    return FALSE;
  }
  
  if(destAddressMatch)
  {
    return TRUE;
  }
  if ((pEnetHdr->dest[5] == 0xFF) &&(pEnetHdr->dest[4] == 0xFF)&&(pEnetHdr->dest[3] == 0xFF)&&
          (pEnetHdr->dest[2] == 0xFF) &&(pEnetHdr->dest[1] == 0xFF)&&(pEnetHdr->dest[0] == 0xFF))
  {
    return TRUE;
  }
  {
    mCastMacAddrStruct_t* pMCastAddrTable = NULL;
    uint8_t i;
    if(protocol == gIpProtv6_c)
    {
      pMCastAddrTable = maIPv6MCastMacAddrTable;
    }
    else if(protocol == gIpProtv4_c)
    {
      pMCastAddrTable = maIPv4MCastMacAddrTable;
    }
    if(pMCastAddrTable)
    {
      for(i=0;i<mMCastMacAddrTableSize_c;i++)
      {
        if(pMCastAddrTable[i].inUse)
        {
          if(FLib_MemCmp(pMCastAddrTable[i].address, pEnetHdr->dest,MAC_ADDR_SIZE))
          {
            return TRUE;
          }
        }
      }
    }
  }
  
  return FALSE;
}
/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return The status of the operation
*
********************************************************************************** */

static void RNDIS_TimerCallback ( void * param )
{
  void* pBuff;
  pBuff = MEM_BufferAllocWithId(mPendingEthPacketLength, gRNDIS_MemPoolId_c, (void*)__get_LR());
  if(pBuff)
  {
    TMR_StopTimer(*(tmrTimerID_t*)param);
    VirtualNic_ReceiveUnlock (pBuff);
  }  
}

/*! *********************************************************************************
* \brief   
*
* \param[in] 
* \param[in] 
* \param[in] 
*
* \return The status of the operation
*
********************************************************************************** */

static void RNDIS_SerialCallBack(void* param)
{
  MEM_BufferFree(param);
}

/**************************************************************************//*!
 *
 * @name  RNDIS_DestMacAddressIsUninitialized(void)
 *
 * @brief The function 
 *
 *
 * @return TRUE                              mac address uninitialized
 *         FALSE                             mac address initialized
 *****************************************************************************/
static bool_t RNDIS_DestMacAddressIsUninitialized(void)
{
  uint8_t i;
  for(i=0;i<MAC_ADDR_SIZE;i++)
  {
    if(maDestMacAddr[i] != 0)
    {
      return FALSE;
    }
  }
  return TRUE;
}
/**************************************************************************//*!
 *
 * @name  VNIC_InitMacAddress(void)
 *
 * @brief The function 
 *
 *
 * @return TRUE                              mac address uninitialized
 *         FALSE                             mac address initialized
 *****************************************************************************/
static void RNDIS_InitDestMacAddress(void)
{
  uint8_t i;
  uint8_t nxpMacPrefix[] = {0x00,0x60,0x37}; 
  uint32_t simUIdL = SIM->UIDL ;
  for(i=0; i < (MAC_ADDR_SIZE >> 1) ;i++)
  {
    maDestMacAddr[i] = nxpMacPrefix[i];
  }
  while(i < MAC_ADDR_SIZE)
  {
    maDestMacAddr[i] = (simUIdL >> ((MAC_ADDR_SIZE - i -1 )<< 3))&0xff;
    i++;
  }
}