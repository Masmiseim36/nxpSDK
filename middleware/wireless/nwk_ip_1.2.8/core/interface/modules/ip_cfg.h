/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IP_CFG_H
#define _IP_CFG_H

/*!
\file       ip_cfg.h
\brief      This is a header file for the IP configuration.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Private macros
==================================================================================================*/
/* Default Values */

#ifndef IP_IP6_ROUTING_TBL_SIZE
   #define IP_IP6_ROUTING_TBL_SIZE      15    /*!< IPv6 routing table size */
#endif

#ifndef IP_IP6_MCASTFW_TBL_SIZE
   #define IP_IP6_MCASTFW_TBL_SIZE      5    /*!< IPv6 multicast forwarding table size */
#endif

#ifndef IP_IP6_FIREWALL_TBL_SIZE
   #define IP_IP6_FIREWALL_TBL_SIZE     2     /*!< IPv6 firewall table size */
#endif

#ifndef IP_IP4_ROUTING_TBL_SIZE
   #define IP_IP4_ROUTING_TBL_SIZE      5    /*!< IPv4 routing table size */
#endif

#ifndef IP_IF_NB
   #define IP_IF_NB                     3     /*!< IP interfaces number */
#endif

#ifndef IP_IF_IP6_ADDR_NB
   #define IP_IF_IP6_ADDR_NB            6     /*!< IPv6 interface addresses number */
#endif

#ifndef IP_IF_IP6_MULTICAST_ADDR_NB
   #define IP_IF_IP6_MULTICAST_ADDR_NB  10    /*!< IPv6 multicast addresses number */
#endif

#ifndef IP_IF_IP4_ADDR_NB
   #define IP_IF_IP4_ADDR_NB            3     /*!< IPv4 interface addresses number */
#endif

#ifndef IP_TRANSPORT_SERVICE_NB
   #define IP_TRANSPORT_SERVICE_NB      4     /*!< IP transport service number */
#endif

#ifndef IP_IP_REASSEMBLY_QUEUE_SIZE
    #define IP_IP_REASSEMBLY_QUEUE_SIZE 3     /*!< IP reassembly queue size */
#endif

#ifndef IP_TASK_STACK_SIZE
   #define IP_TASK_STACK_SIZE           1024U  /*!< IP task stack size */
#endif

#ifndef IP_TASK_MSG_QUEUE_SIZE
   #define IP_TASK_MSG_QUEUE_SIZE       30U    /*!< IP task message queue size */
#endif

#endif  /*_IP_CFG_H */
