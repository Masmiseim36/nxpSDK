/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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

