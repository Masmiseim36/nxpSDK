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
