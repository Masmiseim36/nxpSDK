/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _VNIC_INTERFACE_H
#define _VNIC_INTERFACE_H  1



/******************************************************************************
* Constants - None
*****************************************************************************/

/******************************************************************************
* Macro's
*****************************************************************************/

#ifndef gVNIC_PID_d
#define gVNIC_PID_d (0x0301)
#endif


#ifndef gVirtualNIC_USBTaskPriority_d
  #define gVirtualNIC_USBTaskPriority_d                       2
#endif
 #ifndef gVirtualNIC_USBTaskStackSize_d
  #define gVirtualNIC_USBTaskStackSize_d                       1024
 #endif


#ifndef gVNIC_AddPaddingForPacketsShorterThan14_d
#define gVNIC_AddPaddingForPacketsShorterThan14_d 1
#endif

/* if gVirtualCOMPortSerialNoEnable_d == TRUE the VNIC device contains a Serial Number String Descriptor */
#ifndef gVNICSerialNoEnable_d
#define gVNICSerialNoEnable_d                       1
#endif

#ifndef gDefaultValueOfVNICSerialNo_c
/*00000001*/
#define gDefaultValueOfVNICSerialNo_c            0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x31,0x00
#endif   
#ifndef gVirtualNIC_ReceiveDirectlyFromUSB_d
  #define gVirtualNIC_ReceiveDirectlyFromUSB_d                       1
#endif
#define gVirtualNIC_ReceiveLockBuffer_d       ((void*)0xf0f0f0f0)
/*****************************************************************************
******************************************************************************
* Public type definitions
******************************************************************************
*****************************************************************************/
typedef enum
{
  vnicParamType_Numeric_c,
  vnicParamType_String_c
} vnicParamType_t;

typedef void (*pfVNICConfigParamCallback_t)( vnicParamType_t paramType, uint8_t* pParamValue, uint32_t paramLength);
typedef struct vnicParamStruct_tag
{
  uint8_t* const psParamName;
  pfVNICConfigParamCallback_t pfVNICConfigParamCallback;
}vnicParamStruct_t;

/*This callback is called from VNIC to inform application that a ethernet header was received. Application must 
decide weather it wants the packet or not. If application wants the packet, it must provide a buffer for its storage,
otherwise it must return NULL*/
typedef void*(*pfVNICEthernetHeaderReceivedCallback_t)(void* pEthernetHeader, uint32_t packetLength);
/*This callback is called from VNIC to inform application that a complete ethernet packet was received.*/
typedef void(*pfVNICEthernetPacketReceivedCallback_t)(void* pEthernetPacket, uint32_t packetLength);

typedef struct vnicReceiveCallbacksStruct_tag
{
  pfVNICEthernetHeaderReceivedCallback_t pfEthernetHeaderReceived;
  pfVNICEthernetPacketReceivedCallback_t pfEthernetPacketReceived;
}vnicReceiveCallbacksStruct_t;
/*****************************************************************************
* Global variables
*****************************************************************************/

/*****************************************************************************
* Global Functions
*****************************************************************************/
void VirtualNic_RegisterParamStruct(const vnicParamStruct_t* pParamStruct);  
extern void* VirtualNic_Init(uint8_t);
extern serialStatus_t VirtualNic_Write(void* interface, uint8_t* pData, uint16_t dataSize);
extern void VirtualNic_SMReadNotify(void* interface);
extern void VirtualNic_RegisterReceiveDirecltyFromUSBCallbacks(const vnicReceiveCallbacksStruct_t* pReceiveCallbacksStruct);
extern void VirtualNic_ReceiveUnlock (void* pBuff);
extern void VirtualNic_SetMacAddress(uint8_t* pMacAddress);
extern bool_t VirtualNic_IsDeviceConfigured(void);
#endif 


/* EOF */
