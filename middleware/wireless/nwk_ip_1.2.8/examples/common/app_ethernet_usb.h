/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _APP_ETHERNET_USB_H
#define _APP_ETHERNET_USB_H


/*!=================================================================================================
\file       app_ethernet_usb.h
\brief      This is a header file for the ENET Media interface configuration module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "network_utils.h"
#if USB_RNDIS_IF
/*==================================================================================================
Public macros
==================================================================================================*/


#define CONNECTION_MODE_TYPE            0
#define THREAD_INTERFACE_STATUS_TYPE    1
#define AVAILABILITY_TYPE               2

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern taskMsgQueue_t *gpUSBEthTaskMsgQueue;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
/*!*************************************************************************************************
\fn     IFUSBENET_Start
\brief  This is a public function used to start the USB Ethernet IP Media Interface.

\param  [in] pTaskMsgQueue  task message queue
\param  [in] thrInstanceID  Thread Instance ID

\return void
***************************************************************************************************/
void IFUSBENET_Start(taskMsgQueue_t* pTaskMsgQueue, instanceId_t thrInstanceID);

#endif /* USB_RNDIS_IF */    
#endif /* _APP_ETHERNET_USB_H */
