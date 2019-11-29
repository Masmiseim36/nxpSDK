/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MDNS_CFG_H
#define _MDNS_CFG_H
/*!=================================================================================================
\file       mdns_cfg.h
\brief      This is a header file for the mDNS module. It contains macros used to configure the
            mDNS module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "app_stack_config.h"
/*==================================================================================================
Public macros
==================================================================================================*/

/* Default Values */

/*! Maximum mDns services */
#ifndef MDNS_MAX_SERVICES_NO
    #define MDNS_MAX_SERVICES_NO        5
#endif

#ifndef MDNS_CACHE_ENTRIES
    #define MDNS_CACHE_ENTRIES          5
#endif

#ifndef MDNS_IP6_ADDRESS
    #define MDNS_IP6_ADDRESS            {0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB}
#endif

#ifndef MDNS_IP4_ADDRESS
    #define MDNS_IP4_ADDRESS            0xE00000FB
#endif

#define MDNS6_SERVER_PORT              (5353U)
#define MDNS4_SERVER_PORT              (MDNS6_SERVER_PORT)

#define MDNS_PROBING_TIME              (250U)
#define MDNS_ANNOUNCING_TIME           (1000U)
#define MDNS_ANNOUNCING_STEPS          (2U)
#define MDNS_TTL                       (120U)


/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _MDNS_CFG_H */
