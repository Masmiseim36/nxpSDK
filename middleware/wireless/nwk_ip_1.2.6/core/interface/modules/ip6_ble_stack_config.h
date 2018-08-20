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

#ifndef _IP6_BLE_STACK_CONFIG_H
#define _IP6_BLE_STACK_CONFIG_H


/*!=================================================================================================
\file       IP6_BLE_STACK_config.h
\brief      This is a header file for thread library configuration. This file should not be
            by  the user.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/


/*==================================================================================================
Public macros
==================================================================================================*/

/* Stacks */
#ifndef STACK_BLE
    #define STACK_BLE               1
#endif
   
#ifndef STACK_THREAD
    #define STACK_THREAD            0
#endif   

/* Modules */
#define IP_IP6_ENABLE               1
#define IP_IP6_ROUTING_ENABLE       1
#define DHCP6_CLIENT_ENABLED        0
#define DHCP6_SERVER_ENABLED        0
#define COAP_ENABLED                1
#define MPL_ENABLED                 0
#define TRICKLE_ENABLED             0
#define THR_ROUTING_ENABLE          0
#define IP_IP4_ENABLE               0
#define DHCP4_CLIENT_ENABLED        0
#define ND_ENABLED                  1
#define MDNS_ENABLED                0
#define DNS_ENABLED                 0
#define MLE_SECURITY_ENABLED        0
#define DTLS_ENABLED                0

#define ND_DAD_ENABLED                  0
#define ND_OPT_CONTEXT_RCV_ENABLED      1
#define ND_OPT_CONTEXT_SEND_ENABLED     1



/* Module Settings */
#define DHCP6_RAPID_COMMIT              1

#define SLWPCFG_BLE_ENABLED              1
#define SLWPCFG_802154_ENABLED          0
#define SLWPCFG_SERVER_ENABLED          0
#define SLWPCFG_NODE_ENABLED            0
#define SLWPCFG_MESH_ENABLED            1
#define SLWPCFG_FRAGMENTATION_ENABLED   0

#define SLWPCFG_HEADER_COMPRESSION_ENABLED  1
#define SLWPCFG_RFC6282_COMPRESSION_ENABLED 1
#define SLWPCFG_RFC4944_COMPRESSION_ENABLED 0

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#define NVNG_MoveToRam(...)
#define NVNG_Save(...)
#define NVNG_Erase(...)
#define NVNG_SyncSave(...)

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _IP6_BLE_STACK_CONFIG_H */
