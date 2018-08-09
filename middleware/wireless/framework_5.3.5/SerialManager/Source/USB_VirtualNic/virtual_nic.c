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
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdlib.h>
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_cdc_rndis.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "virtual_nic.h"
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#include "usb_phy.h"
#endif
#include <stdbool.h>
#include "SerialManager.h"      
#include "VirtualNicInterface.h"
#include "fsl_os_abstraction.h"      
#include "FunctionLib.h"
/*******************************************************************************
* Definitions
******************************************************************************/
/* Base unit for ENIT layer is 1Mbps while for RNDIS its 100bps*/
#define ENET_CONVERT_FACTOR (10000)
#if gVirtualNIC_ReceiveDirectlyFromUSB_d
#define RNDIS_MIN_PACKET_SIZE (RNDIS_USB_OVERHEAD_SIZE+ RNDIS_ETHER_HDR_SIZE)
#else
#define RNDIS_MIN_PACKET_SIZE (RNDIS_USB_OVERHEAD_SIZE)
#endif
/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
typedef enum
{
  vnicRecvStatus_WaitForHeader_c = 0,
  vnicRecvStatus_WaitForData_c,
  vnicRecvStatus_ReceiveLocked_c
} vnicRecvStatus_t; 
typedef struct rndisConfigParamInfo_tag
{
  uint32_t paramNameOffset;
  uint32_t paramNameLength;
  uint32_t paramType;
  uint32_t paramValueOffset;
  uint32_t paramValueLength;
}rndisConfigParamInfo_t;
/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
extern void SerialManager_TxNotify( uint32_t i );
extern uint16_t SerialManager_VirtualNicRxNotify(uint8_t* pData, uint16_t dataSize, uint8_t interface);   
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
usb_status_t USB_DeviceCdcVnicCallback(class_handle_t handle, uint32_t event, void *param);
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
static bool_t VNIC_ValidateNDISPacketHeader(uint8_t* pdata, uint32_t size);
static bool_t VNIC_CompareNDISParamName(uint8_t* pParamNameInfoBuff, const uint8_t* pParamName, uint32_t size);
static bool_t VNIC_MacAddressIsUninitialized(void);
static void VNIC_InitMacAddress(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTask(void *handle);
#endif
/*******************************************************************************
* Variables
******************************************************************************/
extern usb_device_endpoint_struct_t g_cdcVnicDicEp[];
extern usb_device_class_struct_t g_cdcVnicClass;
/* Data structure of virtual nic device. */
usb_cdc_vnic_t g_cdcVnic;
/* USB device class information */
static usb_device_class_config_struct_t s_cdcAcmConfig[1] = {{
    USB_DeviceCdcVnicCallback, 0, &g_cdcVnicClass,
}};
/* USB device class configuraion information */
static usb_device_class_config_list_struct_t s_cdcAcmConfigList = {
    s_cdcAcmConfig, USB_DeviceCallback, 1,
};
static uint8_t mSerialIndex;

uint8_t nic_mac_addr[MAC_ADDR_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00};
#define mfsbSize_c 64
#define mRecvBuffSize_c 64
static uint8_t first_send_buff[mfsbSize_c];
static uint16_t mRestOfBytesToSend;
static uint8_t* mpToRestOfBytesToSend;
static bool_t  mSendInProgress;
static uint8_t maRecvDataBuff[mRecvBuffSize_c];
static uint32_t mRestOfBytesToReceive;
vnicRecvStatus_t gVnicRecvStatus = vnicRecvStatus_WaitForHeader_c;
static const vnicParamStruct_t* mpVNICParamStruct;
#if gVirtualNIC_ReceiveDirectlyFromUSB_d
static const vnicReceiveCallbacksStruct_t* mpReceiveCallbacksStruct = NULL;
static const vnicReceiveCallbacksStruct_t* mpRCSAlias;
static uint8_t* mpReceiveBuffer;
static uint32_t mRxPayloadLength;
static uint32_t mBytesAlreadyReceived;
#else
static uint32_t mBytesToSMRxIn;
static uint32_t mBytesToSMRxOut;
static bool_t  mArmVnicRxOnReadNotify;
static bool_t  mSMReadNotifyInProgress;
#endif
static bool mDeviceConfigured;
static uint8_t g_zero_send = 0x00;
#if USB_DEVICE_CONFIG_USE_TASK
OSA_TASK_DEFINE(USB_DeviceTask, gVirtualNIC_USBTaskPriority_d, 1, gVirtualNIC_USBTaskStackSize_d, 0);
#endif
/*****************************************************************************
 * Local Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  VNIC_MacAddressIsUninitialized(void)
 *
 * @brief The function 
 *
 *
 * @return TRUE                              mac address uninitialized
 *         FALSE                             mac address initialized
 *****************************************************************************/
static bool_t VNIC_MacAddressIsUninitialized(void)
{
  uint8_t i;
  for(i=0;i<MAC_ADDR_SIZE;i++)
  {
    if(nic_mac_addr[i] != 0)
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
static void VNIC_InitMacAddress(void)
{
  uint8_t i;
  uint8_t nxpMacPrefix[] = {0x00,0x60,0x37}; 
  uint32_t simUIdL = SIM->UIDL + 1;
  for(i=0; i < (MAC_ADDR_SIZE >> 1) ;i++)
  {
    nic_mac_addr[i] = nxpMacPrefix[i];
  }
  while(i < MAC_ADDR_SIZE)
  {
    nic_mac_addr[i] = (simUIdL >> ((MAC_ADDR_SIZE - i -1 )<< 3))&0xff;
    i++;
  }
  
  
}

/**************************************************************************//*!
 *
 * @name  VNIC_CompareNDISParamName(uint8_t* pdata, uint)
 *
 * @brief The function 
 *
 * @param pParamNameInfoBuff: pointer to param name in information buffer of REMOTE_NDIS_SET_MSG                
 * @param pParamName:         pointer to a constant string representing parameter name 
 * @param size:               
 *
 * @return TRUE                              match
 *         FALSE                             no match
 *****************************************************************************/
static bool_t VNIC_CompareNDISParamName(uint8_t* pParamNameInfoBuff, const uint8_t* pParamName, uint32_t size)
{
  while(size)
  {
    if(*pParamNameInfoBuff++ != *pParamName++)
    {
      return FALSE;
    }
    if(*pParamNameInfoBuff++ != 0)
    {
      return FALSE;
    }
    size--;
  }
  return TRUE;
}

/**************************************************************************//*!
 *
 * @name  VNIC_ValidateNDISPacketHeader(uint8_t* pdata, uint)
 *
 * @brief The function returns the Line Coding/Configuration
 *
 * @param handle:        handle
 * @param interface:     interface number
 * @param coding_data:   output line coding data
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
static bool_t VNIC_ValidateNDISPacketHeader(uint8_t* pdata, uint32_t size)
{
  if(size <= RNDIS_MIN_PACKET_SIZE)
  {
    return FALSE;
  }
  if(USB_LONG_TO_LITTLE_ENDIAN(((rndis_packet_msg_format_t*)pdata)->messageType) != RNDIS_PACKET_MSG)
  {
    return FALSE;
  }
  if(USB_LONG_TO_LITTLE_ENDIAN(((rndis_packet_msg_format_t*)pdata)->messageLen) <= RNDIS_MIN_PACKET_SIZE)
  {
    return FALSE;
  }
  if(USB_LONG_TO_LITTLE_ENDIAN(((rndis_packet_msg_format_t*)pdata)->dataOffset) == 0)
  {
    return FALSE;
  }
  if(USB_LONG_TO_LITTLE_ENDIAN(((rndis_packet_msg_format_t*)pdata)->dataOffset)%4)
  {
    return FALSE;
  }
  if(USB_LONG_TO_LITTLE_ENDIAN(((rndis_packet_msg_format_t*)pdata)->dataOffset) <  RNDIS_DATA_OFFSET)
  {
    return FALSE;
  }
  return TRUE;
}

///*******************************************************************************
//* Code
//******************************************************************************/

/*!
* @brief Callback for RNDIS specific requests.
*
* @param handle The class handle of the CDC ACM class.
* @param event The event for the RNDIS device.
* @param param The pointer to parameter of the callback.
* @return A USB error code or kStatus_USB_Success.
*/
usb_status_t USB_DeviceCdcRndisCallback(class_handle_t handle, uint32_t event, void *param)
{
  usb_status_t error = kStatus_USB_Success;
  usb_device_cdc_rndis_request_param_struct_t *rndisParam;
  rndisParam = (usb_device_cdc_rndis_request_param_struct_t *)param;
  
  switch (event)
  {
  case kUSB_DeviceCdcEventAppGetLinkSpeed:
    if (1 == mDeviceConfigured)
    {
      *((uint32_t *)rndisParam->buffer) = 100;//VNIC_EnetGetSpeed();
      *((uint32_t *)rndisParam->buffer) *= ENET_CONVERT_FACTOR;
    }
    break;
  case kUSB_DeviceCdcEventAppGetSendPacketSize:
    *((uint32_t *)rndisParam->buffer) = USB_LONG_TO_LITTLE_ENDIAN(g_cdcVnicDicEp[0].maxPacketSize);
    break;
  case kUSB_DeviceCdcEventAppGetRecvPacketSize:
    *((uint32_t *)rndisParam->buffer) = USB_LONG_TO_LITTLE_ENDIAN(g_cdcVnicDicEp[1].maxPacketSize);
    break;
  case kUSB_DeviceCdcEventAppGetMacAddress:
    memcpy(rndisParam->buffer, nic_mac_addr, MAC_ADDR_SIZE);
    break;
  case kUSB_DeviceCdcEventAppGetMaxFrameSize:
    *((uint32_t *)rndisParam->buffer) = (ENET_FRAME_MAX_FRAMELEN + RNDIS_ETHER_HDR_SIZE);
    break;
  case kUSB_DeviceCdcEventAppGetLinkStatus:
    if (1 == mDeviceConfigured)
    {
      *((uint32_t *)rndisParam->buffer) = 1;//VNIC_EnetGetLinkStatus();
    }
    break;
  case kUSB_DeviceCdcEventAppConfigParameter:
    {
      uint8_t* pInfoBuff;
      uint32_t infoBuffSize, paramNameOffset,paramNameLength,paramValueOffset,paramValueLength;
      vnicParamType_t paramType ;
      const vnicParamStruct_t* pParamStruct;
      pParamStruct = mpVNICParamStruct;
      if(!pParamStruct)
      {
        break;
      }
      pInfoBuff = rndisParam->buffer;
      infoBuffSize = rndisParam->length; 
      if(infoBuffSize < sizeof(rndisConfigParamInfo_t))
      {
        break;
      }
      paramNameOffset = ((rndisConfigParamInfo_t*)pInfoBuff)->paramNameOffset;
      paramNameLength = ((rndisConfigParamInfo_t*)pInfoBuff)->paramNameLength;
      paramValueOffset = ((rndisConfigParamInfo_t*)pInfoBuff)->paramValueOffset;
      paramValueLength = ((rndisConfigParamInfo_t*)pInfoBuff)->paramValueLength;
      paramType = (((rndisConfigParamInfo_t*)pInfoBuff)->paramType)? vnicParamType_String_c : vnicParamType_Numeric_c;  
      if(infoBuffSize < paramNameOffset + paramNameLength)
      {
        break;
      }
      if(infoBuffSize<paramValueOffset + paramValueLength)
      {
        break;
      }
      
      while(pParamStruct->pfVNICConfigParamCallback && pParamStruct->psParamName)
      {
        if(paramNameLength >> 1 == FLib_StrLen((char*)pParamStruct->psParamName))
        {
          if( VNIC_CompareNDISParamName(pInfoBuff + paramNameOffset, pParamStruct->psParamName, paramNameLength >> 1))
          {
            pParamStruct->pfVNICConfigParamCallback(paramType, pInfoBuff + paramValueOffset, paramValueLength);
          } 
        }
        pParamStruct++;
      }
      
    }
    break;
  default:
    break;
  }
  return error;
}

/*!
 * @brief Callback for CDC RNDIS class specific requests.
 *
 * @param handle The class handle of the CDC ACM class.
 * @param event The event for the RNDIS device.
 * @param param The pointer to parameter of the callback.
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVnicCallback(class_handle_t handle, uint32_t event, void *param)
{
  usb_status_t error = kStatus_USB_Error;
  usb_device_cdc_acm_request_param_struct_t *acmReqParam;
  usb_device_endpoint_callback_message_struct_t *epCbParam;
  acmReqParam = (usb_device_cdc_acm_request_param_struct_t *)param;
  epCbParam = (usb_device_endpoint_callback_message_struct_t *)param;
  switch (event)
  {
  case kUSB_DeviceCdcEventSendResponse:
    {
      
      bool_t txNotify = FALSE;
      if (TRUE == mDeviceConfigured)
      {
        
        if(mRestOfBytesToSend)
        {
          error = USB_DeviceCdcAcmSend(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_IN_ENDPOINT,mpToRestOfBytesToSend,mRestOfBytesToSend);
          mRestOfBytesToSend =  0;
          if(error != kStatus_USB_Success)
          {
            txNotify = TRUE; 
          }
          
        }
        else
        {
          if(epCbParam->length % FS_CDC_VNIC_BULK_IN_PACKET_SIZE)
          {
            txNotify = TRUE; 
          }              
          else
          {
            error = USB_DeviceCdcAcmSend(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_IN_ENDPOINT,&g_zero_send,sizeof(uint8_t));
            if(error != kStatus_USB_Success)
            {
              txNotify = TRUE;
            }
          }  
          
        }
        if(txNotify)
        {
          mSendInProgress = FALSE;
          SerialManager_TxNotify((uint32_t)mSerialIndex);
        }
        
      }
    }
    break;
  case kUSB_DeviceCdcEventRecvResponse:
    {
      uint32_t size = epCbParam->length;
      error = kStatus_USB_Success;
      
      
      if (FALSE == mDeviceConfigured)
      {
        break;
      }
      if (size == 0xFFFFFFFF)
      {
        break;
      }
#if gVirtualNIC_ReceiveDirectlyFromUSB_d
      if (gVnicRecvStatus == vnicRecvStatus_WaitForHeader_c)
      {
    	rndis_packet_msg_format_t* pRndisHeader;
        if (!VNIC_ValidateNDISPacketHeader(maRecvDataBuff, size)) 
        {
          error = USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, maRecvDataBuff, mRecvBuffSize_c);
          break;
        }
        pRndisHeader = (rndis_packet_msg_format_t*)maRecvDataBuff;
        mRestOfBytesToReceive = USB_LONG_TO_LITTLE_ENDIAN(pRndisHeader->messageLen);
        mRxPayloadLength = mRestOfBytesToReceive - RNDIS_USB_OVERHEAD_SIZE;
        mpRCSAlias = mpReceiveCallbacksStruct;
        mpReceiveBuffer = NULL;
        if(mpRCSAlias)
        {
          mpReceiveBuffer = mpRCSAlias->pfEthernetHeaderReceived(maRecvDataBuff + RNDIS_USB_OVERHEAD_SIZE, mRestOfBytesToReceive - RNDIS_USB_OVERHEAD_SIZE);
        }
        
        if(mRestOfBytesToReceive >= size)
        {
          mRestOfBytesToReceive -= size;
        }
        else
        {
          size = mRestOfBytesToReceive;
          mRestOfBytesToReceive = 0;
        }
        if(size % FS_CDC_VNIC_BULK_OUT_PACKET_SIZE )
        {
          mRestOfBytesToReceive = 0;
        }
        mBytesAlreadyReceived = size;
        if(mpReceiveBuffer)
        {
          if(mpReceiveBuffer == gVirtualNIC_ReceiveLockBuffer_d)
          {
            gVnicRecvStatus = vnicRecvStatus_ReceiveLocked_c ;
            break;
          }
          for(mRxPayloadLength = 0; mRxPayloadLength < mBytesAlreadyReceived - RNDIS_USB_OVERHEAD_SIZE; mRxPayloadLength++ )
          {
            mpReceiveBuffer[mRxPayloadLength] = maRecvDataBuff[RNDIS_USB_OVERHEAD_SIZE + mRxPayloadLength];
          }
        }
      }
      
      else if((gVnicRecvStatus == vnicRecvStatus_WaitForData_c)) //vnicRecvStatus_WaitForData_c
      {
        if(mRestOfBytesToReceive >= size)
        {
          mRestOfBytesToReceive -= size;
        }
        else
        {
          size = mRestOfBytesToReceive;
          mRestOfBytesToReceive = 0;
        }
        if(size % FS_CDC_VNIC_BULK_OUT_PACKET_SIZE )
        {
          mRestOfBytesToReceive = 0;
        }
        if(mpReceiveBuffer)
        {
          mRxPayloadLength += size;
          mRestOfBytesToReceive = 0;
        }
        
      }
      else
      {}
      
      
      if(mRestOfBytesToReceive)
      {
        gVnicRecvStatus = vnicRecvStatus_WaitForData_c;
        if(mpReceiveBuffer)
        {
          
          error = USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, mpReceiveBuffer + mRxPayloadLength , mRestOfBytesToReceive); 
        } 
        else
        {
          error = USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, maRecvDataBuff, mRecvBuffSize_c);
        }
        
      } 
      else
      {
        gVnicRecvStatus = vnicRecvStatus_WaitForHeader_c;
        if(mpReceiveBuffer)
        {
          mpRCSAlias->pfEthernetPacketReceived(mpReceiveBuffer, mRxPayloadLength);
        }
        error = USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, maRecvDataBuff, mRecvBuffSize_c);
      }
      
#else //gVirtualNIC_ReceiveDirectlyFromUSB_d         
      
      if (gVnicRecvStatus == vnicRecvStatus_WaitForHeader_c)
      {
        if (!VNIC_ValidateNDISPacketHeader(maRecvDataBuff, size)) 
        {
          error = USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, maRecvDataBuff, mRecvBuffSize_c);
          break;
        }
        mRestOfBytesToReceive = USB_LONG_TO_LITTLE_ENDIAN(((rndis_packet_msg_format_t *)maRecvDataBuff)->messageLen);
        mBytesToSMRxOut = RNDIS_USB_OVERHEAD_SIZE ;  
      }
      
      else
      {
        //vnicRecvStatus_WaitForData_c
        mBytesToSMRxOut = 0;
      }
      if(mRestOfBytesToReceive >= size)
      {
        mRestOfBytesToReceive -= size;
      }
      else
      {
        size = mRestOfBytesToReceive;
        mRestOfBytesToReceive = 0;
      }
      mBytesToSMRxIn = size;
      if(size % FS_CDC_VNIC_BULK_OUT_PACKET_SIZE )
      {
        mRestOfBytesToReceive = 0;
      }
      while(mBytesToSMRxIn != mBytesToSMRxOut)
      {
        uint16_t bytesCount;
        OSA_InterruptDisable();
        Serial_RxBufferByteCount( mSerialIndex, &bytesCount );
        if(bytesCount == gSerialMgrRxBufSize_c )
        {
          // SM receive buffer is full  
          mArmVnicRxOnReadNotify = TRUE;
          OSA_InterruptEnable();
          return error;
        }
        else
        {
          OSA_InterruptEnable();
          mBytesToSMRxOut += SerialManager_VirtualNicRxNotify(maRecvDataBuff + mBytesToSMRxOut, mBytesToSMRxIn - mBytesToSMRxOut , mSerialIndex);
        } 
      } 
      if(mRestOfBytesToReceive)
      {
        gVnicRecvStatus = vnicRecvStatus_WaitForData_c;
      } 
      else
      {
        gVnicRecvStatus = vnicRecvStatus_WaitForHeader_c;
      }
      error = USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, maRecvDataBuff, mRecvBuffSize_c);
#endif // gVirtualNIC_ReceiveDirectlyFromUSB_d         
    }
    break;
  case kUSB_DeviceCdcEventSerialStateNotif:
    ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 0;
    break;
  case kUSB_DeviceCdcEventSendEncapsulatedCommand:
    if (1 == acmReqParam->isSetup)
    {
      *(acmReqParam->buffer) = (g_cdcVnic.rndisHandle)->rndisCommand;
    }
    else
    {
      USB_DeviceCdcRndisMessageSet(g_cdcVnic.rndisHandle, acmReqParam->buffer, acmReqParam->length);
      *(acmReqParam->length) = 0;
    }
    error = kStatus_USB_Success;
    break;
  case kUSB_DeviceCdcEventGetEncapsulatedResponse:
    USB_DeviceCdcRndisMessageGet(g_cdcVnic.rndisHandle, acmReqParam->buffer, acmReqParam->length);
    error = kStatus_USB_Success;
    break;
  default:
    break;
  }
  
  return error;
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
  usb_status_t error = kStatus_USB_Error;
  uint16_t *temp16 = (uint16_t *)param;
  uint8_t *temp8 = (uint8_t *)param;
  
  switch (event)
  {
  case kUSB_DeviceEventBusReset:
    {
      uint8_t *message;
      uint32_t len;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
      if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &g_cdcVnic.speed))
      {
        USB_DeviceSetSpeed(handle, g_cdcVnic.speed);
      }
#endif
      USB_DeviceCdcRndisHaltCommand(g_cdcVnic.rndisHandle);
      USB_DeviceCdcRndisResetCommand(g_cdcVnic.rndisHandle, &message, &len);
      mDeviceConfigured = FALSE;
      mSendInProgress = FALSE; 
      gVnicRecvStatus = vnicRecvStatus_WaitForHeader_c;
#if (gVirtualNIC_ReceiveDirectlyFromUSB_d == 0)      
      mSMReadNotifyInProgress = 0;
      mArmVnicRxOnReadNotify = 0;
#endif      
    }
    break;
  case kUSB_DeviceEventSetConfiguration:
    if (param)
    {
      g_cdcVnic.currentConfiguration = *temp8;
      if (USB_CDC_VNIC_CONFIGURE_INDEX == (*temp8))
      {
#if (gVirtualNIC_ReceiveDirectlyFromUSB_d == 0)              
        mArmVnicRxOnReadNotify = 0;
        mSMReadNotifyInProgress = 0;
#endif        
        mDeviceConfigured = TRUE;
        mSendInProgress = FALSE; 
        gVnicRecvStatus = vnicRecvStatus_WaitForHeader_c;
        (void)USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, maRecvDataBuff, mRecvBuffSize_c);
      }
    }
    break;
  case kUSB_DeviceEventSetInterface:
    if (mDeviceConfigured)
    {
      uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
      uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
      if (interface < USB_CDC_VNIC_INTERFACE_COUNT)
      {
        g_cdcVnic.currentInterfaceAlternateSetting[interface] = alternateSetting;
      }
    }
    break;
  case kUSB_DeviceEventGetConfiguration:
    break;
  case kUSB_DeviceEventGetInterface:
    break;
  case kUSB_DeviceEventGetDeviceDescriptor:
    if (param)
    {
      error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
    }
    break;
  case kUSB_DeviceEventGetConfigurationDescriptor:
    if (param)
    {
      error = USB_DeviceGetConfigurationDescriptor(handle,
                                                   (usb_device_get_configuration_descriptor_struct_t *)param);
    }
    break;
  case kUSB_DeviceEventGetStringDescriptor:
    if (param)
    {
      /* Get device string descriptor request */
      error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
    }
    break;
  default:
    break;
  }
  
  return error;
}

/*****************************************************************************
 * Public Functions
 *****************************************************************************/
/*!
 * @brief USB Interrupt service routine.
 *
 * This function serves as the USB interrupt service routine.
 *
 * @return None.
 */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_cdcVnic.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_cdcVnic.deviceHandle);
}
#endif

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
void* VirtualNic_Init(uint8_t param)
{
  static bool_t vNicInit = FALSE;
  usb_device_cdc_rndis_config_struct_t rndisConfig;
  uint8_t irqNo;
  if( vNicInit ==  FALSE )
  {
    mSerialIndex = param;
    if(VNIC_MacAddressIsUninitialized())
    {
      VNIC_InitMacAddress();
    }
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
    uint8_t ehciIrq[] = USBHS_IRQS;
    irqNo = ehciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
    
    CLOCK_EnableUsbhs0Clock(kCLOCK_UsbSrcPll0,  CLOCK_GetFreq(kCLOCK_PllFllSelClk));
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
    uint8_t khciIrq[] = USB_IRQS;
    irqNo = khciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];
    
    SystemCoreClockUpdate();
    
#if ((defined FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED) && (FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED))
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
#else
#if (defined(FSL_FEATURE_SOC_SCG_COUNT) && (FSL_FEATURE_SOC_SCG_COUNT > 0U))
    CLOCK_EnableUsbfs0Clock(kCLOCK_IpSrcFircAsync, CLOCK_GetFreq(kCLOCK_ScgFircAsyncDiv1Clk));
#else
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
#endif
#endif /* FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED */
#endif
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
    
    /*
    * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
    * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
    * RAM can not be used and the memory can't be accessed.
    */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
      ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */
    
    g_cdcVnic.speed = USB_SPEED_FULL;
    g_cdcVnic.cdcAcmHandle = (class_handle_t)NULL;
    g_cdcVnic.deviceHandle = NULL;
    if (kStatus_USB_Success != USB_DeviceClassInit(CONTROLLER_ID, &s_cdcAcmConfigList, &g_cdcVnic.deviceHandle))
    {
      return NULL;
    }
    else
    {
      g_cdcVnic.cdcAcmHandle = s_cdcAcmConfigList.config->classHandle;
    }
    rndisConfig.devMaxTxSize = ENET_FRAME_MAX_FRAMELEN + RNDIS_ETHER_HDR_SIZE + RNDIS_USB_HEADER_SIZE;
    rndisConfig.rndisCallback = USB_DeviceCdcRndisCallback;
    if (kStatus_USB_Success != USB_DeviceCdcRndisInit(g_cdcVnic.cdcAcmHandle, &rndisConfig, &(g_cdcVnic.rndisHandle)))
    {
      USB_DeviceClassDeinit(CONTROLLER_ID);
      return NULL;
    }
    
    NVIC_SetPriority((IRQn_Type)irqNo, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irqNo);
    
    USB_DeviceRun(g_cdcVnic.deviceHandle);
#if USB_DEVICE_CONFIG_USE_TASK    
    if( NULL == OSA_TaskCreate(OSA_TASK(USB_DeviceTask),g_cdcVnic.deviceHandle))
    {
      USB_DeviceStop(g_cdcVnic.deviceHandle);
      NVIC_DisableIRQ((IRQn_Type)irqNo);
      USB_DeviceCdcRndisDeinit(g_cdcVnic.rndisHandle);
      USB_DeviceClassDeinit(CONTROLLER_ID);
      return NULL;
    }
#endif      
    vNicInit = TRUE;
  }
  
  
  return (void*)g_cdcVnic.cdcAcmHandle;    
}

/******************************************************************************
 *
 *    @name       VirtualNic_Write
 *
 *    @brief
 *
 *    @param      interface
 *    @param      nic_data_ptr
 *    @param      length
 *
 *    @return     None
 *
 *****************************************************************************/
serialStatus_t VirtualNic_Write (void* interface, uint8_t * nic_data_ptr, uint16_t length)
{
  
  serialStatus_t status = gSerial_InternalError_c;
  uint32_t usb_tx_len = length + RNDIS_USB_OVERHEAD_SIZE;
  uint8_t return_status = kStatus_USB_Success;
  rndis_packet_msg_format_t* pRndisHeader;
  uint32_t i;
  bool_t proceed = FALSE;
  (void)interface;
  if(FALSE == mDeviceConfigured)
  {
    SerialManager_TxNotify((uint32_t)mSerialIndex);  
    return gSerial_Success_c;
  }
  OSA_InterruptDisable();
  if(!mSendInProgress)
  {
    proceed = mSendInProgress = TRUE;
  }
  OSA_InterruptEnable();
  if(!proceed)
  {
    return status;
  }
  mRestOfBytesToSend = length;
  mpToRestOfBytesToSend = nic_data_ptr;
  pRndisHeader = (rndis_packet_msg_format_t*)first_send_buff;
  /* Prepare USB Header */
  pRndisHeader->messageType = USB_LONG_TO_LITTLE_ENDIAN(RNDIS_PACKET_MSG);
  pRndisHeader->messageLen = USB_LONG_TO_LITTLE_ENDIAN(usb_tx_len);
  pRndisHeader->dataOffset = USB_LONG_TO_LITTLE_ENDIAN(RNDIS_DATA_OFFSET);
  pRndisHeader->dataLen = USB_LONG_TO_LITTLE_ENDIAN(length);
  
  /* fill rest of first_send_buff buffers with payload as much as possible */
  for( i = RNDIS_USB_OVERHEAD_SIZE; (i < mfsbSize_c) && mRestOfBytesToSend ; i++,mRestOfBytesToSend--)
  {
    first_send_buff[i] =  *mpToRestOfBytesToSend++;
  }
  
#if gVNIC_AddPaddingForPacketsShorterThan14_d
  /* It seems that Linux RNDIS driver rejects all packets smaller than 58 bytes(44(packet header) + 14). So, for those packets 
  padding bytes(zero bytes) are added */
  while(i < 58)
  {
    first_send_buff[i++] = 0;
  }
#endif    
  return_status = USB_DeviceCdcAcmSend(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_IN_ENDPOINT, first_send_buff,i);
  
  if (return_status == kStatus_USB_Success)
  {
    status = gSerial_Success_c; 
  }
  return status;
}
/*!
 * @brief USB task function.
 *
 * This function runs the task for USB device.
 *
 * @return None.
 */
/*****************************************************************************
*  
*	@name		 VirtualNic_SMReadNotify
* 
*	@brief		 This function is called from SerialRead function in serial manager. It arms a new 
*                        receive on the cdc bulk out endpoint if it isn't already armed and if there is room in 
*                        the receive buffer for bulk out endpoint size characters       
*	@param		 
* 
*	@return 	 None
**				  
*****************************************************************************/
void VirtualNic_SMReadNotify(void* interface)
{
#if (gVirtualNIC_ReceiveDirectlyFromUSB_d == 0)        
  bool_t proceed = FALSE;
  OSA_InterruptDisable();
  if(!mSMReadNotifyInProgress && mArmVnicRxOnReadNotify)
  {
    proceed = mSMReadNotifyInProgress = TRUE;
  }
  OSA_InterruptEnable();
  if(!proceed)
  {
    return;
  }
  while(mBytesToSMRxIn != mBytesToSMRxOut)
  {
    uint16_t bytesCount;
    OSA_InterruptDisable();
    Serial_RxBufferByteCount( mSerialIndex, &bytesCount );
    if(gSerialMgrRxBufSize_c == bytesCount)
    {
      mSMReadNotifyInProgress = FALSE;
      OSA_InterruptEnable();
      return;
    } 
    else
    {
      OSA_InterruptEnable();
      mBytesToSMRxOut += SerialManager_VirtualNicRxNotify(maRecvDataBuff + mBytesToSMRxOut, mBytesToSMRxIn - mBytesToSMRxOut, mSerialIndex);
    }
  }
  mArmVnicRxOnReadNotify = FALSE;
  if(mRestOfBytesToReceive)
  {
    gVnicRecvStatus = vnicRecvStatus_WaitForData_c;
  } 
  else
  {
    gVnicRecvStatus = vnicRecvStatus_WaitForHeader_c;
  }
  (void)USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, maRecvDataBuff, mRecvBuffSize_c);
  mSMReadNotifyInProgress = FALSE;
#else
  (void)interface;
#endif  
}

/*****************************************************************************
 *
 *   @name        VirtualNic_RegisterParamStruct
 *
 *   @brief       Calls for all types of Initializations
 *               
 *   @param       None
 *
 *   @return      None
 **
 *****************************************************************************/
void VirtualNic_RegisterParamStruct(const vnicParamStruct_t* pParamStruct)
{
  mpVNICParamStruct = pParamStruct;
}

/*****************************************************************************
 *
 *   @name        VirtualNic_IsDeviceConfigured
 *
 *   @brief       Calls for all types of Initializations
 *               
 *   @param       None
 *
 *   @return      None
 **
 *****************************************************************************/
bool_t VirtualNic_IsDeviceConfigured(void)
{
  return mDeviceConfigured;
}
/*****************************************************************************
 *
 *   @name        VirtualNic_SetMacAddress
 *
 *   @brief       Calls for all types of Initializations
 *               
 *   @param       None
 *
 *   @return      None
 **
 *****************************************************************************/
void VirtualNic_SetMacAddress(uint8_t* pMacAddress)
{
  uint8_t i;
  OSA_InterruptDisable();
  for(i=0; i< MAC_ADDR_SIZE ; i++)
  {
      nic_mac_addr[i] = pMacAddress[i];
  }
  
  OSA_InterruptEnable();
}
/*****************************************************************************
 *
 *   @name        VirtualNic_RegisterReceiveDirecltyFromUSBCallbacks
 *
 *   @brief       Calls for all types of Initializations
 *               
 *   @param       None
 *
 *   @return      None
 **
 *****************************************************************************/
extern void VirtualNic_RegisterReceiveDirecltyFromUSBCallbacks(const vnicReceiveCallbacksStruct_t* pReceiveCallbacksStruct)
{
#if gVirtualNIC_ReceiveDirectlyFromUSB_d
  if(pReceiveCallbacksStruct)
  {
    if(pReceiveCallbacksStruct->pfEthernetHeaderReceived && pReceiveCallbacksStruct->pfEthernetPacketReceived)
    {
     mpReceiveCallbacksStruct = pReceiveCallbacksStruct; 
    }
    
  }
  else
  {
    mpReceiveCallbacksStruct = NULL;
  }
  
#endif
}

/******************************************************************************
 *
 *    @name       VirtualNic_ReceiveUnlock
 *
 *    @brief
 *
 *    @param      interface
 *    @param      nic_data_ptr
 *    @param      length
 *
 *    @return     None
 *
 *****************************************************************************/
void VirtualNic_ReceiveUnlock (void* pBuff)
{
#if gVirtualNIC_ReceiveDirectlyFromUSB_d  
  if((gVnicRecvStatus == vnicRecvStatus_ReceiveLocked_c ) &&  pBuff)
  {
    mpReceiveBuffer = pBuff;  
    for(mRxPayloadLength = 0; mRxPayloadLength < mBytesAlreadyReceived - RNDIS_USB_OVERHEAD_SIZE; mRxPayloadLength++ )
    {
      mpReceiveBuffer[mRxPayloadLength] = maRecvDataBuff[RNDIS_USB_OVERHEAD_SIZE + mRxPayloadLength];
    }
    if(mRestOfBytesToReceive)
    {
      gVnicRecvStatus = vnicRecvStatus_WaitForData_c;
      USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, mpReceiveBuffer + mRxPayloadLength , mRestOfBytesToReceive); 
    } 
    else
    {
      gVnicRecvStatus = vnicRecvStatus_WaitForHeader_c;
      mpRCSAlias->pfEthernetPacketReceived(mpReceiveBuffer, mRxPayloadLength);
      USB_DeviceCdcAcmRecv(g_cdcVnic.cdcAcmHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, maRecvDataBuff, mRecvBuffSize_c);
    }
  }
#else
  (void)pBuff;
#endif  
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTask(void *handle)
{
#if USE_RTOS  
    while (1U)
#endif      
    {
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
        USB_DeviceEhciTaskFunction(handle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
        USB_DeviceKhciTaskFunction(handle);
#endif
    }
}
#endif

