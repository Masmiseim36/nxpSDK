/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IP_IF_USBENET_H
#define _IP_IF_USBENET_H
/*!=================================================================================================
\file       ip_if_usbenet.h
\brief      This is a header file for the Media interface to USB Ethernet (RNDIS)
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include <stdint.h>
#include "ip.h"
#include "nd.h"

/*==================================================================================================
Public macros
==================================================================================================*/


/*==================================================================================================
Public type definitions
==================================================================================================*/


/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern const mediaIfStruct_t* gUsbEnetMediaIfPtr;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
/*!*************************************************************************************************
\fn void USBENET_Initialize(void)
\brief  Initialize RNDIS module and interface

\param [in]  void

\retval      void
***************************************************************************************************/
void USBENET_Initialize(void);


/*!*************************************************************************************************
\fn  void IP_UsbEnetGetMacAddr(uint8_t *pMacAddr, uint8_t addrLen, bool_t internalAddr)

\brief  Function used to retrieve USB ENET MAC addresses

\param [out] pMacAddr      Pointer to the location where the address will be put
\param [in]  addrLen       Length of the address
\param [in]  internalAddr  TRUE: internal demo mac address
                           FALSE: rndis demo mac address

\retval      none
***************************************************************************************************/
void IP_UsbEnetGetMacAddr(uint8_t *pMacAddr, uint8_t addrLen, bool_t internalAddr);

/*!*************************************************************************************************
\fn  void IP_UsbEnetRecv(void *pInData, uint16_t size)

\brief  Services an Ethernet packet

\param [in]  pInData       Pointer to received buffer
\param [in]  size          Length of received buffer

\retval      none
***************************************************************************************************/
void IP_UsbEnetRecv(void *pInData, uint16_t size);

/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetEnable(void)
\brief  Enable Usb enet Interface

\param [in]  void

\retval      void
***************************************************************************************************/
void IP_UsbEnetEnable(void);

/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetDisable(void)
\brief  Disable Usb enet Interface

\param [in]  void

\retval      void
***************************************************************************************************/
void IP_UsbEnetDisable(void);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /*_IP_IF_USBENET_H */
