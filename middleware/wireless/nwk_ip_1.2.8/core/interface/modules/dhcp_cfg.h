/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _DHCP_CFG_H
#define _DHCP_CFG_H

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Private macros
==================================================================================================*/

/* Default values */

#ifndef DHCP4_SERVER_MAX_CLIENTS
    #define DHCP4_SERVER_MAX_CLIENTS         5
#endif

/*! timer period */
#ifndef DHCP4_EXPIRATION_TIMER_PERIOD_SEC
    #define DHCP4_EXPIRATION_TIMER_PERIOD_SEC       0xE10    /* one hour in seconds */ //10
#endif

/*! DHCP4 Client Request in milliseconds */
#ifndef DHCP4_CLIENT_REQ_TIMER
    #define DHCP4_CLIENT_REQ_TIMER                 10000U
#endif

/*! number of retries*/
#ifndef DHCP4_CLIENT_DEF_RETRY_NB
    #define DHCP4_CLIENT_DEF_RETRY_NB            4U
#endif

#endif  /*_DHCP_CFG_H */
