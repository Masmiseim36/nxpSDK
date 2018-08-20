/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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

#ifndef __SEMIHOST_HARDFAULT_DISABLE
    #define __SEMIHOST_HARDFAULT_DISABLE
#endif

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
