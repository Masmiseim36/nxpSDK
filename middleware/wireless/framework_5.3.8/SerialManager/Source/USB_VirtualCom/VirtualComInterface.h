/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
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
