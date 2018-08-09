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

#ifndef IP_IP6_FIREWALL_TBL_SIZE
   #define IP_IP6_FIREWALL_TBL_SIZE     2     /*!< IPv6 firewall table size */
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
   #define IP_TRANSPORT_SERVICE_NB      3     /*!< IP transport service number */
#endif

#ifndef IP_IP_REASSEMBLY_QUEUE_SIZE
    #define IP_IP_REASSEMBLY_QUEUE_SIZE 3     /*!< IP reassembly queue size */
#endif

#ifndef IP_TASK_STACK_SIZE
   #define IP_TASK_STACK_SIZE           1024U  /*!< IP task stack size */
#endif

#ifndef IP_TASK_MSG_QUEUE_SIZE
   #define IP_TASK_MSG_QUEUE_SIZE       20U    /*!< IP task message queue size */
#endif

#endif  /*_IP_CFG_H */
