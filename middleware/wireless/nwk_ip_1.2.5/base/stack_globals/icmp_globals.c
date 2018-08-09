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

/*!
\file       icmp_globals.c
\brief      This is the source file that contains parameters for the ICMP module that can be
            configured by the application 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "stack_config.h"
#include "icmp.h"
#include "mld.h"
#include "ip.h"

extern const icmpProtMsgTypeHandler_t gaIcmpProtMsgTypeHandlerTbl6[];
extern const uint8_t gIcmpProtMsgTypeHandlerTbl6Size;

extern const icmpProtMsgTypeHandler_t gaThrRouterImcpErrorHandler[];
extern const uint8_t gThrRouterImcpErrorHandlerSize;

extern const icmpProtMsgTypeHandler_t gaUdpMsgTypeHandlerTbl6[];
extern const uint8_t gUdpMsgTypeHandlerTbl6Size;

extern const icmpProtMsgTypeHandler_t gaShellProtMsgTypeHandlerTbl6[];
extern const uint8_t gShellProtMsgTypeHandlerTbl6Size;

extern const icmpProtMsgTypeHandler_t gaShellProtMsgTypeHandlerTbl4[];
extern const uint8_t gShellProtMsgTypeHandlerTbl4Size;

#if IP_IP4_ENABLE
    extern const icmpProtMsgTypeHandler_t gaIcmpProtMsgTypeHandlerTbl4[];
    extern const uint8_t gIcmpProtMsgTypeHandlerTbl4Size;
    #if UDP_TREAT_ICMP_PORT_UNR
        extern const icmpProtMsgTypeHandler_t gaUdpMsgTypeHandlerTbl4[];
        extern const uint8_t gUdpMsgTypeHandlerTbl4Size;
    #endif
#endif


#if ND_ENABLED
    extern const icmpProtMsgTypeHandler_t gaNdIcmpProtMsgTypeHandlerTbl6[];
    extern const uint8_t gNdIcmpProtMsgTypeHandlerTbl6Size;
#endif

#if PMTU_REGISTER_ICMP_PKT_TOO_BIG_HANDLER ||  PMTU_REGISTER_TCP_PKT_TOO_BIG_HANDLER || PMTU_REGISTER_UDP_PKT_TOO_BIG_HANDLER
   extern const icmpProtMsgTypeHandler_t gaPmtuIcmpProtMsgTypeHandlerTbl6[];
   extern const uint8_t gPmtuIcmpProtMsgTypeHandlerTbl6Size;
#endif

#if MLD_ENABLED
    extern const icmpProtMsgTypeHandler_t gaMldProtMsgTypeHandlerTbl[];
    extern const uint8_t gMldProtMsgTypeHandlerTblSize;
#endif

/*==================================================================================================
Private macros
==================================================================================================*/

/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
/* Register ICMP receive callbacks */
const icmpMsgTypeHandler_t gaIcmpMsgTypeHandlerTable[]=
{
    /*ICMP*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaIcmpProtMsgTypeHandlerTbl6,IPPROTO_ICMPV6, (uint8_t*)&gIcmpProtMsgTypeHandlerTbl6Size},

    /*THR*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaThrRouterImcpErrorHandler,IPPROTO_ICMPV6, (uint8_t*)&gThrRouterImcpErrorHandlerSize},
    {NULL, (icmpProtMsgTypeHandler_t*)&gaThrRouterImcpErrorHandler,IPPROTO_UDP, (uint8_t*)&gThrRouterImcpErrorHandlerSize},

    /*UDP*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaUdpMsgTypeHandlerTbl6, IPPROTO_UDP , (uint8_t*)&gUdpMsgTypeHandlerTbl6Size},

#if ND_ENABLED
    /*ND*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaNdIcmpProtMsgTypeHandlerTbl6, IPPROTO_ICMPV6 , (uint8_t*)&gNdIcmpProtMsgTypeHandlerTbl6Size},

   #if PMTU_REGISTER_ICMP_PKT_TOO_BIG_HANDLER
    {NULL, (icmpProtMsgTypeHandler_t*)&gaPmtuIcmpProtMsgTypeHandlerTbl6, IPPROTO_ICMPV6 , (uint8_t*)&gPmtuIcmpProtMsgTypeHandlerTbl6Size},
   #endif

   #if PMTU_REGISTER_TCP_PKT_TOO_BIG_HANDLER
    {NULL, (icmpProtMsgTypeHandler_t*)&gaPmtuIcmpProtMsgTypeHandlerTbl6, IPPROTO_TCP , (uint8_t*)&gPmtuIcmpProtMsgTypeHandlerTbl6Size},
   #endif

   #if PMTU_REGISTER_UDP_PKT_TOO_BIG_HANDLER
    {NULL, (icmpProtMsgTypeHandler_t*)&gaPmtuIcmpProtMsgTypeHandlerTbl6, IPPROTO_UDP , (uint8_t*)&gPmtuIcmpProtMsgTypeHandlerTbl6Size},
   #endif

#endif

#if IP_IP4_ENABLE
    /* ICMP */
    {NULL, (icmpProtMsgTypeHandler_t*)&gaIcmpProtMsgTypeHandlerTbl4, IPPROTO_ICMP , (uint8_t*)&gIcmpProtMsgTypeHandlerTbl4Size},

#if UDP_TREAT_ICMP_PORT_UNR
    /*UDP*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaUdpMsgTypeHandlerTbl4, IPPROTO_UDP , (uint8_t*)&gUdpMsgTypeHandlerTbl4Size},
#endif
#endif

#if THREAD_USE_SHELL || THREAD_USE_THCI    
    /*SHELL_APP OR THCI*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaShellProtMsgTypeHandlerTbl6, IPPROTO_ICMPV6, (uint8_t*)&gShellProtMsgTypeHandlerTbl6Size},

    /*SHELL_APP OR THCI*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaShellProtMsgTypeHandlerTbl4, IPPROTO_ICMP, (uint8_t*)&gShellProtMsgTypeHandlerTbl4Size},
#endif    

#if MLD_ENABLED
    /*MLD*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaMldProtMsgTypeHandlerTbl, IPPROTO_ICMPV6, (uint8_t*)&gMldProtMsgTypeHandlerTblSize},
#endif
};

const uint8_t gIcmpMsgTypeHandlerSize = sizeof(gaIcmpMsgTypeHandlerTable)/ sizeof(icmpMsgTypeHandler_t);
/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

