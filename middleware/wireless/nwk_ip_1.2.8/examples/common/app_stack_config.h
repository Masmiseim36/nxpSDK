/*! *********************************************************************************
 * \defgroup TAC  Thread Application Configuration Interface
 * @{
 ***********************************************************************************/
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_STACK_CONFIG_H_
#define _APP_STACK_CONFIG_H_

/*!
\file       app_stack_config.h
\brief      This file is used for the stack configuration of all thread demo applications.
            If required to configure just one application, please use the application configuration file.
            Ex: for the thread router application, use thread_router_eligible_device_config.h
*/

#include "stack_config.h"

/*! Thread APP uses SHELL commands */
#ifndef THREAD_USE_SHELL
    #define THREAD_USE_SHELL          TRUE
#endif
/*! Thread APP uses FCSI commands */
#ifndef THREAD_USE_THCI
    #define THREAD_USE_THCI           FALSE
#endif

#if (THR_DEFAULT_CAN_CREATE_NEW_NETWORK && THR_DEFAULT_IS_POLLING_END_DEVICE) || \
    (THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER && THR_DEFAULT_IS_POLLING_END_DEVICE) || \
    (THR_DEFAULT_IS_FULL_END_DEVICE && THR_DEFAULT_IS_POLLING_END_DEVICE)
    #error There are included two not compatible configurations. Please select only one configuration
#endif

#if (THREAD_USE_SHELL && THREAD_USE_THCI)
    #error There are included two not compatible methods. Please select only one method.
#endif

#if (THREAD_BORDER_ROUTER_CONFIG + THREAD_ROUTER_CONFIG + THREAD_ED_CONFIG) != 1
    #error "Please select one configuration."
#endif

/*=================================================================================================
   Stack Globals Configuration
==================================================================================================*/

/*=================================================================================================
  THREAD
==================================================================================================*/
/*! The maximum number of Thread Router / REED (Router Eligible Devices) radio range neighbors */
#ifndef THR_MAX_REED_ROUTERS_NEIGHBORS
    #define THR_MAX_REED_ROUTERS_NEIGHBORS      50
#endif

/*! The maximum number of Thread Sleepy End Device radio range neighbors */
#ifndef THR_MAX_SLEEPY_ED_NEIGHBORS
    #define THR_MAX_SLEEPY_ED_NEIGHBORS         10
#endif

/*! The maximum number of radio range neighbors with which the Thread device can communicate */
#ifndef THR_MAX_NEIGHBORS
    #define THR_MAX_NEIGHBORS         (THR_MAX_REED_ROUTERS_NEIGHBORS + THR_MAX_SLEEPY_ED_NEIGHBORS)
#endif

/*! The maximum number of simultaneous 802.15.4 transmissions */
#ifndef THR_MAX_DATA_REQS
    #define THR_MAX_DATA_REQS                   8
#endif

/*! Number of consecutive  failed 802.15.4 transmissions for a link to be considered down and a
    child device to reattach */
#ifndef THR_FAILED_CHILD_TRANSMISSIONS
    #define THR_FAILED_CHILD_TRANSMISSIONS      4
#endif

/*! Number of consecutive  failed 802.15.4 transmissions for a Router-to-Router link to be
    considered broken. */
#ifndef THR_FAILED_ROUTER_TRANSMISSIONS
    #define THR_FAILED_ROUTER_TRANSMISSIONS     4
#endif

/*=================================================================================================
   DHCPv6
==================================================================================================*/

/*! The maximum number of DHCPv6 servers that can be started on the device */
#ifndef DHCP6_SERVER_MAX_INSTANCES
    #define DHCP6_SERVER_MAX_INSTANCES          3
#endif

/*! The maximum number of DHCPv6 clients that the device can service as a DHCPv6 server */
#ifndef DHCP6_SERVER_MAX_CLIENTS
    #define DHCP6_SERVER_MAX_CLIENTS            100
#endif

/*! The maximum number of DHCPv6 clients that can be started on the device */
#ifndef DHCP6_CLIENT_MAX_INSTANCES
    #define DHCP6_CLIENT_MAX_INSTANCES          5
#endif

/*=================================================================================================
   COAP
==================================================================================================*/

/*! The maximum number of COAP sessions that can be established at one time */
#ifndef COAP_MAX_SESSIONS
    #define COAP_MAX_SESSIONS                   5
#endif

/*=================================================================================================
   MLE
==================================================================================================*/

/*=================================================================================================
   SOCKETS
==================================================================================================*/

/*! The maximum number of sockets that can be opened at one time. MUST be correlated to
 * MAX_UDP_CONNECTIONS */
#ifndef BSDS_MAX_SOCKETS
    #define BSDS_MAX_SOCKETS                    14
#endif

/*=================================================================================================
   UDP
==================================================================================================*/

/*! The maximum number of UDP connections that can be opened at one time. MUST not be greater than
 * BSDS_MAX_SOCKETS */
#ifndef MAX_UDP_CONNECTIONS
    #define MAX_UDP_CONNECTIONS                 10
#endif

/*=================================================================================================
   IP
==================================================================================================*/

/*! The maximum number of IP route entries */
#ifndef IP_IP6_ROUTING_TBL_SIZE
    #define IP_IP6_ROUTING_TBL_SIZE             10
#endif

/*! The maximum number of dynamic firewall entries */
#ifndef IP_IP6_FIREWALL_TBL_SIZE
    #define IP_IP6_FIREWALL_TBL_SIZE            10
#endif

#ifndef IP_IP4_ROUTING_TBL_SIZE
    #define IP_IP4_ROUTING_TBL_SIZE             5
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
    #define IP_TRANSPORT_SERVICE_NB             4
#endif

/*! Number representing how many IP packet fragments can be stored at one time */
#ifndef IP_IP_REASSEMBLY_QUEUE_SIZE
    #define IP_IP_REASSEMBLY_QUEUE_SIZE         3
#endif

/*! The maximum number of IPv4 addresses. This is regardless of how many interfaces are available */
#ifndef IP_IF_IP4_ADDR_NB
   #define IP_IF_IP4_ADDR_NB                    3
#endif

/*=================================================================================================
   MPL
==================================================================================================*/

/*! The maximum number of MPL instances. This must be correlated to IP_IF_NB. */
#ifndef MPL_INSTANCE_SET_SIZE
    #define MPL_INSTANCE_SET_SIZE               2
#endif

/*! The maximum number of seeds the MPL module can store at one time */
#ifndef MPL_SEED_SET_SIZE
    #define MPL_SEED_SET_SIZE                   5
#endif

/*! The maximum number of MPL transmitted messages that can be buffered at one time */
#ifndef MPL_BUFFERED_MESSAGE_SET_SIZE
    #define MPL_BUFFERED_MESSAGE_SET_SIZE       5
#endif

/*=================================================================================================
   TRICKLE
==================================================================================================*/

/*! The maximum number of TRICKLE instances. This must be correlated to IP_IF_NB */
#ifndef TRICKLE_INSTANCE_SET_SIZE
    #define TRICKLE_INSTANCE_SET_SIZE           2
#endif

/*! The maximum number of Trickle events */
#ifndef TRICKLE_LIST_SIZE
    #define TRICKLE_LIST_SIZE                   5
#endif

/*=================================================================================================
   SIXLOWPAN
==================================================================================================*/

/*! The maximum number of 6LoWPAN instances. MUST not be greater than IP_IF_NB */
#ifndef SLWPCFG_INSTANCES_NB
    #define SLWPCFG_INSTANCES_NB                1
#endif

/*! The maximum number of 6LoWPAN contexts that can be stored */
#ifndef SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE
    #define SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE  16
#endif

/*! The number of SED devices a router can handle for unfragmented packets */
#ifndef SLWPCFG_UNFRAG_SED_TRACK_NB
    #define SLWPCFG_UNFRAG_SED_TRACK_NB 6
#endif

/*! The number of unfragmented packets a parent can hold for a SED */
#ifndef SLWPCFG_UNFRAG_SED_TRACK_PKT_NB
    #define SLWPCFG_UNFRAG_SED_TRACK_PKT_NB 1
#endif

/*! The number of SED fragmented packets a parent can hold for transmission */
#ifndef SLWPCFG_SED_IND_QUEUE_SIZE
    #define SLWPCFG_SED_IND_QUEUE_SIZE      (THR_MAX_SLEEPY_ED_NEIGHBORS)
#endif

/*=================================================================================================
   MAC
==================================================================================================*/

/*! Enables/Disables the MAC Filtering */
#ifndef MAC_FILTERING_ENABLED
  #define MAC_FILTERING_ENABLED                 1
#endif

/*! The maximum number of entries in the MAC filtering table */
#ifndef MAC_FILTERING_TABLE_SIZE
    #define MAC_FILTERING_TABLE_SIZE            10
#endif

#endif /* _APP_STACK_CONFIG_H_ */
/*!
** @}
*/
