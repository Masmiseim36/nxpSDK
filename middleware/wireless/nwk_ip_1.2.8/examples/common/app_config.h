/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!
**  @addtogroup thread_app_config
**  @{
*/
#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

/*!=================================================================================================
\file       app_config.h
\brief      This file is a for a general configuration of all thread demo applications.
            If it is required to configure just one application use the appllication config. file.
            Ex: for thread router application use thread_router_config.h
==================================================================================================*/


/*! Enable UART for FCSI */
#ifndef THCI_UART_ENABLE
    #define THCI_UART_ENABLE            0
#endif

/*! Enable USB for FCSI */
#ifndef THCI_USB_ENABLE
    #define THCI_USB_ENABLE             0
#endif

/*! Enable Usb Ethernet interface for FCSI */
#ifndef THCI_USBENET_ENABLE
    #define THCI_USBENET_ENABLE         0
#endif

/*! Enable Non-Volatile Memory (NVM) module */
#ifndef NVM_NG_ENABLED
    #define NVM_NG_ENABLED              1
#endif

/*! Select the UART channel for FCSI */
#ifndef THCI_UART_CHANNEL
    #define THCI_UART_CHANNEL           1
#endif

/*! Enable WDOG for CPU_MKW24D512VHA5 and CPU_MK64FN1M0VMD12 */ 
#ifndef WDOG_ENABLE
    #define WDOG_ENABLE                 0
#endif

#ifndef DEBUG_LOG
    #define DEBUG_LOG                   0
#endif

#if DEBUG_LOG
  #define gUsePanic_c                   1
  #define MEM_TRACKING                  1
  #define MEM_STATISTICS                0
  #if MEM_STATISTICS == 0
    #undef MEM_STATISTICS
  #endif
  #define gKbdVeryLongKeyIterations_c   (60)
#endif

#ifndef __ALIGNED
  #define __ALIGNED
#endif

/*! IPV6 realm local multicast 3ead address */
#ifndef MCAST_3EAD_ADDRESS
    #define MCAST_3EAD_ADDRESS \
            {0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0xAD}
#endif

/************************************************************************************/

#endif /* _APP_CONFIG_H_  */
/*!
** @}
*/
