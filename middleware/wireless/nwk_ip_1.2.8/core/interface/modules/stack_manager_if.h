/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
