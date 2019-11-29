/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _DHCP6_CFG_H
#define _DHCP6_CFG_H

/*!
\file       dhcp6_cfg.h
\brief      This is a header file for a configuration header file. It contains default values for
            module configuration defines.
\details    This file contains the folowing configuration options:

            DHCP6_CLIENT_ENABLED                0 | 1
            DHCP6_SERVER_ENABLED                0 | 1
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Private macros
==================================================================================================*/

#ifndef DHCP6_SERVER_MAX_INSTANCES
    #define DHCP6_SERVER_MAX_INSTANCES      3
#endif

#ifndef DHCP6_SERVER_MAX_CLIENTS
    #define DHCP6_SERVER_MAX_CLIENTS        5
#endif

#ifndef DHCP6_CLIENT_MAX_INSTANCES
    #define DHCP6_CLIENT_MAX_INSTANCES      2
#endif

#ifndef DHCP6_CLIENT_MAX_RETRY_NB
    #define DHCP6_CLIENT_MAX_RETRY_NB       6
#endif    

/* IPv6 address validity periods */
#ifndef DHCP6_DEFAULT_PREFERRED_LIFETIME
    #define DHCP6_DEFAULT_PREFERRED_LIFETIME      86400       /* Seconds */
#endif

#ifndef DHCP6_DEFAULT_VALID_LIFETIME
    #define DHCP6_DEFAULT_VALID_LIFETIME          86400       /* Seconds */
#endif

/* Should be 0.5 times the preferred lifetime */
#ifndef DHCP6_DEFAULT_T1
    #define DHCP6_DEFAULT_T1                      43200       /* Seconds */
#endif

/* Should be 0.8 times the preferred lifetime */
#ifndef DHCP6_DEFAULT_T2
    #define DHCP6_DEFAULT_T2                      69102       /* Seconds */
#endif

#endif  /*_DHCP6_CFG_H */

