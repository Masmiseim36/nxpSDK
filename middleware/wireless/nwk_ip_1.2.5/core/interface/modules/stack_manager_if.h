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

#ifndef _STACK_MANAGER_IF_H
#define _STACK_MANAGER_IF_H
/*!=================================================================================================
\file       stack_manager_if.h
\brief      This is a header file for stack configuration structures.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "network_utils.h"
#include "sixlowpan_ib.h"
#include "nd.h"
#include "ip_if_management.h"

/*==================================================================================================
Public macros
==================================================================================================*/


/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum stackType_tag
{
    gThread_c,
    gFlip_c,
    gStatic_c
}stackType_t;


typedef struct stackConfig_tag
{
    /* Stack Management */
    stackType_t         stackType;
    ipIfUniqueId_t      ifUniqueId;
    uint8_t             deviceType;

    void *              pDefaultStackAttr;
    void *              pStackStringAttr;
    void *              pDefaultActiveAttr;
    uint8_t             deviceRole;
    /* 802.15.4 */
    adpIb_t             *pAdpIb;
}stackConfig_t;


typedef struct externalIfConfig_tag
{
    /* unique interface Id*/
    ipIfUniqueId_t      ifUniqueId;
    /* mac address */
    llAddr_t            macAddress;
    /* ND */
    ndPib_t *           pNdPib;
}externalIfConfig_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/



/*==================================================================================================
Public function prototypes
==================================================================================================*/

/*================================================================================================*/
#endif  /* _STACK_MANAGER_IF_H */
