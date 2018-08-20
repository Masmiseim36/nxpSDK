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
    #define SLWPCFG_TX_NORMAL_QUEUE_SIZE                7
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
