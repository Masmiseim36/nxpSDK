/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
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
