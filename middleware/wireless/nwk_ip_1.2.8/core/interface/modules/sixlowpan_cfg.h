/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef  _SIXLOWPAN_CFG_H
#define  _SIXLOWPAN_CFG_H

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"

/*==================================================================================================
Public macros
==================================================================================================*/
/*! Number of 6LoWPAN Instances */
#ifndef SLWPCFG_INSTANCES_NB
   #define SLWPCFG_INSTANCES_NB                 1
#endif

/*! Enables support for handling indirect transmissions to Sleepy end devices and other Router 
    specific functionalities */
#ifndef SLWPCFG_ROUTER_ENABLE
   #define SLWPCFG_ROUTER_ENABLE                1
#endif

#ifndef SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE
    #define SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE  16
#endif

#ifndef SLWPCFG_STACK_SIZE
   #define SLWPCFG_STACK_SIZE  768U
#endif

#ifndef SLWPCFG_MSG_QUEUE_SIZE
   #define SLWPCFG_MSG_QUEUE_SIZE  10
#endif

#ifndef SLWPCFG_MAX_6LOWPAN_RETRANSMISSIONS
    #define SLWPCFG_MAX_6LOWPAN_RETRANSMISSIONS 1
#endif

#ifndef SLWPCFG_RFC4944_MAX_FRAGMENTS
    #define SLWPCFG_RFC4944_MAX_FRAGMENTS 16
#endif

#ifndef SLWPCFG_MAX_FRAMENT_LENGTH
    #define SLWPCFG_MAX_FRAGMENT_LENGTH                 96
#endif

#ifndef RFC4944_REASSEMBLY_TIMEOUT_GUARD_TIME_MS
    #define RFC4944_REASSEMBLY_TIMEOUT_GUARD_TIME_MS    10
#endif

#ifndef SLWPCFG_RFC4944_TX_PACKET_TIMEOUT_SEC
    #define SLWPCFG_RFC4944_TX_PACKET_TIMEOUT_SEC       1
#endif

#ifndef SLWPCFG_RFC4944_FRAG_QUEUE_SIZE
    #define SLWPCFG_RFC4944_FRAG_QUEUE_SIZE             5
#endif

#ifndef SLWPCFG_TX_NORMAL_QUEUE_SIZE
    #define SLWPCFG_TX_NORMAL_QUEUE_SIZE                15
#endif

#ifndef SLWPCFG_TX_PRIORITY_QUEUE_SIZE
    #define SLWPCFG_TX_PRIORITY_QUEUE_SIZE              3
#endif

/*! The number of SED devices a parent can handle for unfragmented packets */
#ifndef SLWPCFG_UNFRAG_SED_TRACK_NB
    #define SLWPCFG_UNFRAG_SED_TRACK_NB 6
#endif

/*! The number of unfragmented packets a parent can hold for a SED */
#ifndef SLWPCFG_UNFRAG_SED_TRACK_PKT_NB
    #define SLWPCFG_UNFRAG_SED_TRACK_PKT_NB 1
#endif

/*! The number of SED fragmented packets a parent can hold for transmission */
#ifndef SLWPCFG_SED_IND_QUEUE_SIZE
    #define SLWPCFG_SED_IND_QUEUE_SIZE 1
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
#endif
