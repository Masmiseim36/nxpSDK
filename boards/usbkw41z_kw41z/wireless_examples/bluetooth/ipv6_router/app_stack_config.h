/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
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
    #define IP_IF_NB                            1
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

/*! The maximum number of IPv4 addresses. This is regardless of how many interfaces are available */
#ifndef IP_IF_IP4_ADDR_NB
   #define IP_IF_IP4_ADDR_NB                    3
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
