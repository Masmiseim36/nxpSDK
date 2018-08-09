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

#ifndef _VIRTUAL_COM_INTERFACE_H
#define _VIRTUAL_COM_INTERFACE_H  1



/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/

 #ifndef gVirtualCOM_USBTaskPriority_d
  #define gVirtualCOM_USBTaskPriority_d                       2
 #endif
 #ifndef gVirtualCOM_USBTaskStackSize_d
  #define gVirtualCOM_USBTaskStackSize_d                       512
 #endif
/* if gVirtualCOMPortSerialNoEnable_d == TRUE the Virtual Com device contains a Serial Number String Descriptor */

 #ifndef gVirtualCOMPortSerialNoEnable_d
  #define gVirtualCOMPortSerialNoEnable_d                       1
 #endif

#ifndef gDefaultValueOfVirtualCOMPortSerialNo_c
                                                            /*00000001*/
 #define gDefaultValueOfVirtualCOMPortSerialNo_c            0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x31,0x00
#endif   

#ifndef gVirtualCOMPort_DiscardTxOnCOMClose_d
  #define gVirtualCOMPort_DiscardTxOnCOMClose_d    1
#endif


#if gVirtualCOMPort_DiscardTxOnCOMClose_d   
  #define gVirtualCOMPort_LineStateCOMOpen_d     0x3
  #define gVirtualCOMPort_LineStateCOMClose_d    0x2
#endif

#ifndef gVirtualCOMPort_EndTxWithEmptyPacket_d      
  #define gVirtualCOMPort_EndTxWithEmptyPacket_d         1
#endif   
/*****************************************************************************
 * Global variables
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
extern void* VirtualCom_Init(uint8_t);
extern serialStatus_t VirtualCom_Write(void* interface, uint8_t* pData, uint16_t dataSize);
extern void VirtualCom_SMReadNotify(void* interface);

#endif 


/* EOF */
