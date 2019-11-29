/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _THREAD_STACK_CONFIG_H
#define _THREAD_STACK_CONFIG_H


/*!=================================================================================================
\file       thread_stack_config.h
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
#ifndef STACK_THREAD
    #define STACK_THREAD                        1
#endif

/* Thread stack Configuration */
#ifndef THREAD_BORDER_ROUTER_CONFIG
    #define THREAD_BORDER_ROUTER_CONFIG         0
#endif

#ifndef THREAD_ROUTER_CONFIG
    #define THREAD_ROUTER_CONFIG                0
#endif

#ifndef THREAD_ED_CONFIG
    #define THREAD_ED_CONFIG                    0
#endif

#ifndef TCP_ENABLED
    #define TCP_ENABLED                         0
#endif

/* Thread border router configuration(Read Only) */
#if THREAD_BORDER_ROUTER_CONFIG
    /* Modules */
    #define IP_IP6_ENABLE               1
    #define IP_IP6_ROUTING_ENABLE       1
    #define IP_IP6_MCAST_FW_ENABLE      1
    #define IP_IP6_FIREWALL_ENABLE      1
    #define DHCP6_CLIENT_ENABLED        1
    #define DHCP6_SERVER_ENABLED        1
    #define COAP_ENABLED                1
    #define MPL_ENABLED                 1
    #define TRICKLE_ENABLED             1
    #define THR_ROUTING_ENABLE          1
    #define MDNS_ENABLED                1
    #define SLWPCFG_ROUTER_ENABLE       1
    #define IP_IP4_ENABLE               1
    #define IP_IP4_ROUTING_ENABLE       1
    #define DHCP4_CLIENT_ENABLED        1
    #define DHCP4_SERVER_ENABLED        1
#ifndef DNS_ENABLED
    #define DNS_ENABLED                 1
#endif
#ifndef ND_ENABLED
    #define ND_ENABLED                  1
#endif
#ifndef MLD_ENABLED
    #define MLD_ENABLED                 1
#endif

    #define TCP_MSS_DEFAULT             1220
    #define TCP_WND                     7320
#endif

 /* Thread router configuration(Read Only) */
#if THREAD_ROUTER_CONFIG
    /* Modules */
    #define IP_IP6_ENABLE               1
    #define IP_IP6_ROUTING_ENABLE       1
    #define IP_IP6_FIREWALL_ENABLE      1
    #define DHCP6_CLIENT_ENABLED        1
    #define DHCP6_SERVER_ENABLED        0
    #define COAP_ENABLED                1
    #define MPL_ENABLED                 1
    #define TRICKLE_ENABLED             1
    #define THR_ROUTING_ENABLE          1
    #define IP_IP4_ENABLE               0
    #define IP_IP4_ROUTING_ENABLE       0
    #define DHCP4_CLIENT_ENABLED        0
    #define ND_ENABLED                  0
    #define SLWPCFG_ROUTER_ENABLE       1
    #define MLD_ENABLED                 0
#ifndef DNS_ENABLED
    #define DNS_ENABLED                 1
#endif
#endif

 /* Thread end device configuration(Read Only) */
#if THREAD_ED_CONFIG
    /* Modules */
    #define IP_IP6_ENABLE               1
    #define IP_IP6_ROUTING_ENABLE       1
    #define IP_IP6_FIREWALL_ENABLE      1
    #define DHCP6_CLIENT_ENABLED        1
    #define DHCP6_SERVER_ENABLED        0
    #define COAP_ENABLED                1
    #define MPL_ENABLED                 1
    #define TRICKLE_ENABLED             1
    #define THR_ROUTING_ENABLE          0
    #define IP_IP4_ENABLE               0
    #define IP_IP4_ROUTING_ENABLE       0
    #define DHCP4_CLIENT_ENABLED        0
    #define ND_ENABLED                  0
    #define SLWPCFG_ROUTER_ENABLE       0
    #define MLD_ENABLED                 0
#ifndef DNS_ENABLED
    #define DNS_ENABLED                 1
#endif
#endif

/* Module Settings */
#define DHCP6_RAPID_COMMIT              1

#define BLACK_BOX_ENABLED               1

#if BLACK_BOX_ENABLED
    #define CONST
#else
   #define CONST const
#endif

#ifndef DTLS_ENABLED
    #define DTLS_ENABLED                1
#endif

/* Select MLD version */
#if MLD_ENABLED
    #define MLDv1_ENABLED   (0)
    #define MLDv2_ENABLED   (1)

    #if (MLDv1_ENABLED == 0) && (MLDv2_ENABLED == 0)
        #error "Cannot disable both MLDv1 and MLDv2"
    #endif
#endif

#if ((DHCP4_CLIENT_ENABLED == 1) || (DHCP4_SERVER_ENABLED)) && (IP_IP4_ENABLE == 0)
      #error "IPv4 must be enabled for DHCP"
#endif

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
#endif  /* _THREAD_STACK_CONFIG_H */
