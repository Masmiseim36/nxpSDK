/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_STACK_CONFIG_H_
#define _APP_STACK_CONFIG_H_

/*!=================================================================================================
\file       app_stack_config.h
\brief      This file is a for stack configuration of all thread demo applications.
            If it is required to configure just one application use the appllication config. file.
            Ex: for thread router application use thread_router_config.h
==================================================================================================*/

/*!=================================================================================================
   SOCKETS
==================================================================================================*/

/*! The maximum number of sockets that can be opened at one time. MUST be correlated to
 * MAX_UDP_CONNECTIONS */
#ifndef BSDS_MAX_SOCKETS
    #define BSDS_MAX_SOCKETS                    10
#endif

/*!=================================================================================================
   UDP
==================================================================================================*/

/*! The maximum number of UDP connections that can be opened at one time. MUST not be greater than
 * BSDS_MAX_SOCKETS */
#ifndef MAX_UDP_CONNECTIONS
    #define MAX_UDP_CONNECTIONS                 10
#endif

/*!=================================================================================================
   IP
==================================================================================================*/

/*! The maximum number of IP route entries */
#ifndef IP_IP6_ROUTING_TBL_SIZE
    #define IP_IP6_ROUTING_TBL_SIZE             10
#endif

/*! The maximum supported number of IP interfaces */
#ifndef IP_IF_NB
    #define IP_IF_NB                            2
#endif

/*! The maximum number of IPv6 addresses. This is regardless of how many interfaces are available */
#ifndef IP_IF_IP6_ADDR_NB
    #define IP_IF_IP6_ADDR_NB                   9
#endif

/*! The maximum number of supported multicast addresses */
#ifndef IP_IF_IP6_MULTICAST_ADDR_NB
    #define IP_IF_IP6_MULTICAST_ADDR_NB         15
#endif

/*! The maximum number of IP transport services that can be supported. Ex. UDP, TCP. */
#ifndef IP_TRANSPORT_SERVICE_NB
    #define IP_TRANSPORT_SERVICE_NB             3
#endif

/*! Number representing how many IP packet fragments can be stored at one time */
#ifndef IP_IP_REASSEMBLY_QUEUE_SIZE
    #define IP_IP_REASSEMBLY_QUEUE_SIZE         3
#endif

/*!=================================================================================================
   SIXLOWPAN
==================================================================================================*/

/*! The maximum number of 6LoWPAN instances. MUST not be greater than IP_IF_NB. */
#ifndef SLWPCFG_INSTANCES_NB
    #define SLWPCFG_INSTANCES_NB                1
#endif

/*! The maximum number of 6LoWPAN contexts that can be stored */
#ifndef SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE
    #define SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE  16
#endif

#endif /* _APP_STACK_CONFIG_H_ */
/*!
** @}
*/
