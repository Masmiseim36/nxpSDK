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

