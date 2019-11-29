/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
#ifndef DTLS_ENABLED
    #define DTLS_ENABLED            0
#endif

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
