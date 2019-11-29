/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "fsl_device_registers.h"
#include "board.h"

#include <stdio.h>
#include <stdlib.h>

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "fsl_debug_console.h"

#include "usb_device_descriptor.h"
#include "virtual_com.h"
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#include "usb_phy.h"
#endif
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
#include "fsl_smc.h"
#endif
#include <stdbool.h>
#include "SerialManager.h"      
#include "VirtualComInterface.h"      
#include "fsl_os_abstraction.h"      
/*******************************************************************************
* Definitions
******************************************************************************/
#ifndef gVirtualComDebug_d
#define gVirtualComDebug_d 0
#endif      
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param);
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
extern void SerialManager_TxNotify( uint32_t i );
extern void SerialManager_VirtualComRxNotify(uint8_t* pData, uint16_t dataSize, uint8_t interface);
static void ReceiveArmingInCriticalSection(class_handle_t handle);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTask(void *handle);
#endif
/*******************************************************************************
* Variables
******************************************************************************/
extern usb_device_endpoint_struct_t g_UsbDeviceCdcVcomDicEndpoints[];
extern usb_device_class_struct_t g_UsbDeviceCdcVcomConfig;
/* Data structure of virtual com device */
static usb_cdc_vcom_struct_t s_cdcVcom;

/* Line codinig of cdc device */
static uint8_t s_lineCoding[LINE_CODING_SIZE] = {
    /* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
    (LINE_CODING_DTERATE >> 0U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 16U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 24U) & 0x000000FFU,
    LINE_CODING_CHARFORMAT,
    LINE_CODING_PARITYTYPE,
    LINE_CODING_DATABITS};

/* Abstract state of cdc device */
static uint8_t s_abstractState[COMM_FEATURE_DATA_SIZE] = {(STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU,
                                                          (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU};

/* Country code of cdc device */
static uint8_t s_countryCode[COMM_FEATURE_DATA_SIZE] = {(COUNTRY_SETTING >> 0U) & 0x00FFU,
                                                        (COUNTRY_SETTING >> 8U) & 0x00FFU};

/* CDC ACM information */
static usb_cdc_acm_info_t s_usbCdcAcmInfo = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, 0};

/* USB device class information */
static usb_device_class_config_struct_t s_cdcAcmConfig[1] = {{
    USB_DeviceCdcVcomCallback, 0, &g_UsbDeviceCdcVcomConfig,
}};

/* USB device class configuraion information */
static usb_device_class_config_list_struct_t s_cdcAcmConfigList = {
    s_cdcAcmConfig, USB_DeviceCallback, 1,
};

static uint8_t mSerialIndex;
static bool_t mReceiveArmed = FALSE;
static bool_t mDeviceConfigured = FALSE;
#if gVirtualCOMPort_DiscardTxOnCOMClose_d
static bool mComOpen = FALSE;
#endif
#if(gSerialMgrRxBufSize_c < DATA_BUFF_SIZE)
#error "gSerialMgrRxBufSize_c  must be at least bulk in endpoint size "
#endif
static uint8_t g_curr_recv_buf[DATA_BUFF_SIZE];
#if USB_DEVICE_CONFIG_USE_TASK
OSA_TASK_DEFINE(USB_DeviceTask, gVirtualCOM_USBTaskPriority_d, 1, gVirtualCOM_USBTaskStackSize_d, 0);
#endif
/*******************************************************************************
* Code
******************************************************************************/
/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle          The CDC ACM class handle.
 * @param event           The CDC ACM class event type.
 * @param param           The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
#if gVirtualComDebug_d
volatile uint32_t sentChar;
volatile uint32_t writeChar;
volatile uint32_t receivedChar;
#endif

usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param)
{
  uint32_t len;
  uint16_t *uartBitmap;
  usb_device_cdc_acm_request_param_struct_t *acmReqParam;
  usb_device_endpoint_callback_message_struct_t *epCbParam;
  usb_status_t error = kStatus_USB_Error;
  usb_cdc_acm_info_t *acmInfo = &s_usbCdcAcmInfo;
  acmReqParam = (usb_device_cdc_acm_request_param_struct_t *)param;
  epCbParam = (usb_device_endpoint_callback_message_struct_t *)param;
  switch (event)
  {
  case kUSB_DeviceCdcEventSendResponse:
    {
#if gVirtualComDebug_d
      sentChar += epCbParam->length;
#endif
#if gVirtualCOMPort_EndTxWithEmptyPacket_d
      if ((epCbParam->length != 0) && (!(epCbParam->length % g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize)))
      {
        /* If the last packet is the size of endpoint, then send also zero-ended packet,
        ** meaning that we want to inform the host that we do not have any additional
        ** data, so it can flush the output.
        */
        USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_BULK_IN_ENDPOINT, NULL, 0);
      }
      
      else
      {
        SerialManager_TxNotify( (uint32_t)mSerialIndex );
      }
      
#else                 
      
      SerialManager_TxNotify( (uint32_t)mSerialIndex );
#endif                     
      
    }
    break;
  case kUSB_DeviceCdcEventRecvResponse:
    {
      if(epCbParam->length != 0xFFFFFFFF)      
      {
#if gVirtualComDebug_d
        receivedChar += epCbParam->length;
#endif
        SerialManager_VirtualComRxNotify(epCbParam->buffer, epCbParam->length, mSerialIndex);             
        mReceiveArmed = FALSE;
        ReceiveArmingInCriticalSection(handle);        
      }
    }
    break;
  case kUSB_DeviceCdcEventSerialStateNotif:
    ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 0;
    break;
  case kUSB_DeviceCdcEventSendEncapsulatedCommand:
    break;
  case kUSB_DeviceCdcEventGetEncapsulatedResponse:
    break;
  case kUSB_DeviceCdcEventSetCommFeature:
    if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
    {
      if (1 == acmReqParam->isSetup)
      {
        *(acmReqParam->buffer) = s_abstractState;
      }
      else
      {
        *(acmReqParam->length) = 0;
      }
    }
    else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
    {
      if (1 == acmReqParam->isSetup)
      {
        *(acmReqParam->buffer) = s_countryCode;
      }
      else
      {
        *(acmReqParam->length) = 0;
      }
    }
    else
    {
    }
    error = kStatus_USB_Success;
    break;
  case kUSB_DeviceCdcEventGetCommFeature:
    if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
    {
      *(acmReqParam->buffer) = s_abstractState;
      *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
    }
    else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
    {
      *(acmReqParam->buffer) = s_countryCode;
      *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
    }
    else
    {
    }
    error = kStatus_USB_Success;
    break;
  case kUSB_DeviceCdcEventClearCommFeature:
    break;
  case kUSB_DeviceCdcEventGetLineCoding:
    *(acmReqParam->buffer) = s_lineCoding;
    *(acmReqParam->length) = LINE_CODING_SIZE;
    error = kStatus_USB_Success;
    break;
  case kUSB_DeviceCdcEventSetLineCoding:
    {
      if (1 == acmReqParam->isSetup)
      {
        *(acmReqParam->buffer) = s_lineCoding;
      }
      else
      {
        *(acmReqParam->length) = 0;
      }
    }
    error = kStatus_USB_Success;
    break;
  case kUSB_DeviceCdcEventSetControlLineState:
    {
      s_usbCdcAcmInfo.dteStatus = acmReqParam->setupValue;
      /* activate/deactivate Tx carrier */
      if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
      {
        acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
      }
      else
      {
        acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
      }
      
      /* activate carrier and DTE */
      if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
      {
        acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
      }
      else
      {
        acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
      }
      
      /* Indicates to DCE if DTE is present or not */
      acmInfo->dtePresent = (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ? true : false;
      
      /* Initialize the serial state buffer */
      acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE;                /* bmRequestType */
      acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_NOTIF_SERIAL_STATE; /* bNotification */
      acmInfo->serialStateBuf[2] = 0x00;                              /* wValue */
      acmInfo->serialStateBuf[3] = 0x00;
      acmInfo->serialStateBuf[4] = 0x00; /* wIndex */
      acmInfo->serialStateBuf[5] = 0x00;
      acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE; /* wLength */
      acmInfo->serialStateBuf[7] = 0x00;
      /* Notifiy to host the line state */
      acmInfo->serialStateBuf[4] = acmReqParam->interfaceIndex;
      /* Lower byte of UART BITMAP */
      uartBitmap = (uint16_t *)&acmInfo->serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE - 2];
      *uartBitmap = acmInfo->uartState;
      len = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
      if (0 == ((usb_device_cdc_acm_struct_t *)handle)->hasSentState)
      {
        error = USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT, acmInfo->serialStateBuf, len);
        if (kStatus_USB_Success != error)
        {
          usb_echo("kUSB_DeviceCdcEventSetControlLineState error!");
        }
        ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 1;
      }
      
      /* Update status */
      if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
      {
        /*  To do: CARRIER_ACTIVATED */
      }
      else
      {
        /* To do: CARRIER_DEACTIVATED */
      }
      
#if gVirtualCOMPort_DiscardTxOnCOMClose_d        
      if(mDeviceConfigured)
      {
        
        if(acmInfo->dteStatus  == gVirtualCOMPort_LineStateCOMOpen_d)
        {
          mComOpen = TRUE; 
#if gVirtualComDebug_d          
          sentChar = 0;
          writeChar = 0;
          receivedChar = 0;
#endif          
        }
        else
        {
          mComOpen = FALSE; 
        }
        
      }
#endif      
      
    }
    break;
  case kUSB_DeviceCdcEventSendBreak:
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
      mDeviceConfigured = FALSE;
#if gVirtualCOMPort_DiscardTxOnCOMClose_d        
      mComOpen = FALSE;
#endif        
      mReceiveArmed = FALSE;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
      if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &s_cdcVcom.speed))
      {
        USB_DeviceSetSpeed(handle, s_cdcVcom.speed);
      }
#endif
      
    }
    break;
  case kUSB_DeviceEventSetConfiguration:
    if (param)
    {
      mReceiveArmed = FALSE;
#if gVirtualCOMPort_DiscardTxOnCOMClose_d      
      mComOpen = FALSE;
#endif      
      mDeviceConfigured = *temp8;
      if(mDeviceConfigured)
      {
        ReceiveArmingInCriticalSection(s_cdcVcom.cdcAcmHandle);
      }
    }
    break;
  case kUSB_DeviceEventSetInterface:
    if (mDeviceConfigured)
    {
      uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
      uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
      if (interface < USB_CDC_VCOM_INTERFACE_COUNT)
      {
        s_cdcVcom.currentInterfaceAlternateSetting[interface] = alternateSetting;
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

/*!
 * @brief USB Interrupt service routine.
 *
 * This function serves as the USB interrupt service routine.
 *
 * @return None.
 */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U) && (gSerialMgrUseUSB_c)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_cdcVcom.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)  && (gSerialMgrUseUSB_c)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(s_cdcVcom.deviceHandle);
}
#endif


/*!
 * @brief USB task function.
 *
 * This function runs the task for USB device.
 *
 * @return None.
 */
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

/*****************************************************************************
*  
*	@name		 VirtualCom_Init
* 
*	@brief		 This function starts cdc device.
* 
*	@param		 None
* 
*	@return 	 None
**				  
*****************************************************************************/
void* VirtualCom_Init(uint8_t param)
{
  uint8_t irqNo;
  mSerialIndex = param;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
  uint8_t ehciIrq[] = USBHS_IRQS;
  irqNo = ehciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
  
  CLOCK_EnableUsbhs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
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
  
  s_cdcVcom.speed = USB_SPEED_FULL;
  s_cdcVcom.cdcAcmHandle = (class_handle_t)NULL;
  s_cdcVcom.deviceHandle = NULL;
  
  if (kStatus_USB_Success == USB_DeviceClassInit(CONTROLLER_ID, &s_cdcAcmConfigList, &s_cdcVcom.deviceHandle))
  {
    usb_echo("USB device CDC virtual com demo\r\n");
    s_cdcVcom.cdcAcmHandle = s_cdcAcmConfigList.config->classHandle;
  }
  
  NVIC_SetPriority((IRQn_Type)irqNo, USB_DEVICE_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ((IRQn_Type)irqNo);
  
  USB_DeviceRun(s_cdcVcom.deviceHandle);
#if USB_DEVICE_CONFIG_USE_TASK    
  OSA_TaskCreate(OSA_TASK(USB_DeviceTask),s_cdcVcom.deviceHandle);
#endif    
  return (void*)s_cdcVcom.cdcAcmHandle;
}

/*****************************************************************************
*  
*	@name		 VirtualCom_Write
* 
*	@brief		 This function arms a send operation on cdc bulk in endpoint  
* 
*	@param		 None
* 
*	@return 	 None
**				  
*****************************************************************************/
serialStatus_t VirtualCom_Write(void* interface, uint8_t* pData, uint16_t dataSize)
{
    serialStatus_t status = gSerial_InternalError_c;
    usb_status_t usbStatus;
#if gVirtualCOMPort_DiscardTxOnCOMClose_d
    if(mComOpen)
#else
        if(mDeviceConfigured)    
#endif    
        {
            usbStatus = USB_DeviceCdcAcmSend((class_handle_t)interface, USB_CDC_VCOM_BULK_IN_ENDPOINT, pData, dataSize);      
            if(usbStatus == kStatus_USB_Success)
            {
                status = gSerial_Success_c;
#if gVirtualComDebug_d
                writeChar += dataSize;
#endif
            }
            
        }
#if gVirtualCOMPort_DiscardTxOnCOMClose_d
        else
        {
            SerialManager_TxNotify( (uint32_t)mSerialIndex );
            status = gSerial_Success_c;
        }
#endif
    return status;
}
/*****************************************************************************
*  
*	@name		 VirtualCom_SMReadNotify
* 
*	@brief		 This function is called from SerialRead function in serial manager. It arms a new 
*                        receive on the cdc bulk out endpoint if it isn't already armed and if there is room in 
*                        the receive buffer for bulk out endpoint size characters       
*	@param		 
* 
*	@return 	 None
**				  
*****************************************************************************/
void VirtualCom_SMReadNotify(void* interface)
{
  if(mDeviceConfigured)
  {
    ReceiveArmingInCriticalSection((class_handle_t)interface);
  }
}
/*****************************************************************************
*  
*	@name		 ReceiveArmingInCriticalSection
* 
*	@brief		 manage to arm securely a receive on cdc bulk out endpoint
* 
*	@param		 None
* 
*	@return 	 None
**				  
*****************************************************************************/
static void ReceiveArmingInCriticalSection(class_handle_t handle)
{
  uint16_t bytesCount;
  uint16_t bytesFree;
  bool_t rx = FALSE;
  
  OSA_InterruptDisable();
  if(!mReceiveArmed)
  {
    Serial_RxBufferByteCount( mSerialIndex, &bytesCount );
    bytesFree = gSerialMgrRxBufSize_c - bytesCount;  
    if(bytesFree >= g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize)
    {
      rx = TRUE;
      mReceiveArmed = TRUE;
    }  
  }
  OSA_InterruptEnable();
  if(rx)
  {
    USB_DeviceCdcAcmRecv(handle, USB_CDC_VCOM_BULK_OUT_ENDPOINT, g_curr_recv_buf,
                         g_UsbDeviceCdcVcomDicEndpoints[1].maxPacketSize);
  }
}                          


