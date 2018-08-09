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

/*
\file       app_serial_tun.h
\brief      This is a header file for the Serialtun interface configuration module.
*/

#ifndef _APP_SERIAL_TUN_H
#define _APP_SERIAL_TUN_H



/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "network_utils.h"

#if THR_SERIAL_TUN_ROUTER
/*==================================================================================================
Public macros
==================================================================================================*/
/* Enable/disable ND host behavior -> TUN transforms to a TAP interface */
#ifndef THR_SERIAL_TUN_ENABLE_ND_HOST
    #define THR_SERIAL_TUN_ENABLE_ND_HOST       TRUE
#endif

/* Enable/disable ND router behavior -> TUN transforms to a TAP interface */
/* Various other configurations are required on the host side, e.g. net.ipv6.conf.intf.accept_ra = 1 */
#ifndef THR_SERIAL_TUN_ENABLE_ND_ROUTER
    #define THR_SERIAL_TUN_ENABLE_ND_ROUTER     FALSE
#endif

/* When DHCPv6-PD is turned off, local static configuration is used, even in TAP mode */
#ifndef THR_SERIAL_TUN_USE_DHCP6_ADDR
    #define THR_SERIAL_TUN_USE_DHCP6_ADDR       TRUE
#endif

#if THR_SERIAL_TUN_ENABLE_ND_HOST && THR_SERIAL_TUN_ENABLE_ND_ROUTER
    #error "Serial TUN: Cannot act as both Host and Router!"
#endif

#if THR_SERIAL_TUN_USE_DHCP6_ADDR && !THR_SERIAL_TUN_ENABLE_ND_HOST
    #error "Serial TUN: DHCPv6-PD can only be used in ND Host mode!"
#endif

/* On the serial TUN, if ND or DHCPv6-PD are disabled, an IPv6 static address will be registered
 * for the external interface */
#define THR_SERIAL_TUN_DEFAULT_ADDRESS {0xFD, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}

/* Thread 1.1 - 8.4.1.1.2 - Table 8-5 Border Agent State Bitmap */
/* Connection Mode:
 * - bits 0-2:
 *      0: DTLS connection to Border Agent is not allowed
 *      1: DTLS connection to Border Agent allowed with a user chosen network
 *      Commissioner Credential and shared PSKc for the active Thread Network Partition
 *      2: DTLS connection to Border Agent allowed using the Border Agent Device
 *      Passphrase (PSKd) as the Commissioner Credential
 *      3: DTLS connection to Border Agent allowed using a vendor defined Commissioner
 *      Credential
 *      4-7: Reserved
 *
 * Thread Interface Status
 * - bits 3-4:
 *      0: Thread interface is not active and is not initialized with a set of valid
 *      operational network parameters
 *      1: Thread interface is initialized with a set of valid operational parameters,
 *      but is not actively participating in a Thread Network Partition
 *      2: Thread interface is initialized with a set of valid operational parameters
 *      and is actively part of a Thread Network Partition
 *      3: Reserved
 *
 * Availability
 * - bits 5-6:
 *      0: Infrequent availability - Thread interface may become inactive when the
 *      Border Agent device is not in
 *      1: High availability - The Border Agent device and its Thread interface are
 *      part of stable, always-on network infrastructure (such as a wireless Access
 *      Point, Home Router, or Cable Modem) use
 *      2-3: Reserved
 * */
#define STATE_BITMAP      uint32_t
#define TWOBITS_MASK      0x0003
#define THREEBITS_MASK    0x0007

#define CONNECTION_MODE_TYPE            0
#define THREAD_INTERFACE_STATUS_TYPE    1
#define AVAILABILITY_TYPE               2

#define SerialTun_AgentCheck_ConnectionMode(var)            (var & THREEBITS_MASK)
#define SerialTun_AgentCheck_ThreadInterfaceStatus(var)     ((var >> 3) & TWOBITS_MASK)
#define SerialTun_AgentCheck_Availability(var)              ((var >> 5) & TWOBITS_MASK)

#define SerialTun_AgentSet_ConnectionMode(var, val)         if(val < 4)                            \
                                                            {                                      \
                                                                var &= (~THREEBITS_MASK);          \
                                                                var |= val;                        \
                                                            }

#define SerialTun_AgentSet_ThreadInterfaceStatus(var, val)  if(val < 3)                            \
                                                            {                                      \
                                                                var &= ((~(TWOBITS_MASK << 3)));   \
                                                                var |= (val<<3);                   \
                                                            }

#define SerialTun_AgentSet_Availability(var, val)           if(val < 2)                            \
                                                            {                                      \
                                                                var &= ((~(TWOBITS_MASK << 5)));   \
                                                                var |= (val<<5);                   \
                                                            }

/*==================================================================================================
Public type definitions
==================================================================================================*/
/* Connection mode */
typedef enum NetAgent_ConnectionMode_tag
{
    NetAgent_DtlsNotAllowed_c      = 0,
    NetAgent_DtlsUserAllowed_c     = 1,
    NetAgent_DtlsAllowed_c         = 2,
    NetAgent_DtlsVendorAllowed_c   = 3,
    NetAgent_ConnectionModeLast_c  = 4
    /* values 4-7 - reserved */
} NetAgentConnectionMode_t;

/* Thread Interface Status */
typedef enum brAgThrInterfaceStatus_tag
{
    NetAgent_ThrNotActive_c              = 0,
    NetAgent_ThrInitializedNotActive_c   = 1,
    NetAgent_ThrInitializedActive_c      = 2,
    NetAgent_ThrInterfaceStatusLast_c    = 3
    /* value 3 - reserved */
} NetAgentThrInterfaceStatus_t;

/* Availability */
typedef enum brAgAvailability_tag
{
    NetAgent_InfrequentAvailability_c    = 0,
    NetAgent_HighAvailability_c          = 1,
    NetAgent_AvailabilityLast_c          = 2
    /* values 2-3 - reserved */
} NetAgentAvailability_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern taskMsgQueue_t *gpSerialTunTaskMsgQueue;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
/*!*************************************************************************************************
\fn     void Serialtun_Start(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID)
\brief  This is a public function used to start the Serialtun interface.

\param  [in] pTaskMsgQueue      pointer to the message queue of the application task
\param  [in] thrInstanceID      Thread instance ID

\return void
***************************************************************************************************/
void Serialtun_Start(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID);

/*!*************************************************************************************************
\fn     void Serialtun_ThreadStarted(uint32_t thrInstanceId)
\brief  This is a public function that handles the steps that should be done after the Thread
        stack is started.

\param  [in] thrInstanceId      Thread instance ID

\return void
***************************************************************************************************/
void Serialtun_ThreadStarted(uint32_t thrInstanceId);

/*!*************************************************************************************************
\fn     void Serialtun_RaReceived(void *pEvent)
\brief  This is a function that is called when a Prefix Information Option is present in an
        ND Router Advertisement packet.

\param  [in] pEvent     pointer to the event structure

\return void
***************************************************************************************************/
void Serialtun_RaReceived(void *pEvent);

/*!*************************************************************************************************
\fn     void Serialtun_RaRouteInfoReceived(void *pEvent)
\brief  This is a function that is called when a Route Information option is present in an
        ND Router Advertisement packet.

\param  [in] pEvent     pointer to the event structure

\return void
***************************************************************************************************/
void Serialtun_RaRouteInfoReceived(void *pEvent);

#endif /* THR_SERIAL_TUN_ROUTER */

#endif /* _APP_SERIAL_TUN_H */
