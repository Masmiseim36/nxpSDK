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

#ifndef _RIPNG_H_
#define _RIPNG_H_
/*!
\file       ripng.h

\copyright  (c) Copyright 2014, Freescale, Inc.  All rights reserved.

\brief      This is a header file for the RIPng routing module.

*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "ip.h"

#ifndef RIPNG_ENABLE
   #define RIPNG_ENABLE 0
#endif


#if RIPNG_ENABLE
/*==================================================================================================
Public macros
==================================================================================================*/

#define RIPNG_VERSION 1U
#define RIPNG_UDP_PORT 521


/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    gRipNgReqCommand_c = 0x01,
    gRipNgRspCommand_c = 0x02,
}ripNgCommandType_t;

typedef enum
{
    gRipNgRegularUpdate_c,
    gRipNgTriggeredUpdate_c,
    gRipNgReceivedRequest_c
}ripNgOutputMsgType_t;

/*! RIPng routing table */
typedef struct ripNgRoutingTblEntry_tag
{
    /*! common part of a routing table entry */
    ip6RoutingTblEntry_t ip6TblEntry;
    /*! total cost of route to destination */
    uint8_t routeCost;
    /*! route index in IP6 Routing table */
    uint8_t routeIndex;
    /*! flag that signales that a route has changed */
    uint8_t routeChangeFlag;
    /*! flag that signales that a route is in the deletion phase */
    uint8_t routeDelFlag;
    /*! timestamp in seconds when the route was created */
    //uint32_t routeTimestampSec;
}ripNgRoutingTblEntry_t;

/*! RIPng packet header */
typedef struct ripNgPktHdr_tag
{
    uint8_t command;
    uint8_t version;
    uint16_t notUsed;
}ripNgPktHdr_t;

typedef struct ripNgOutputDataInfo_tag
{
    void* outputData;
    uint32_t outputDataSize;
    ifHandle_t outputInterface;
}ripNgOutputDataInfo_t;


/*! RIPng output message struct */
typedef struct ripNgOutputMsg_tag
{
    ipAddr_t dstAddr;
    ripNgOutputDataInfo_t outputInfo;
    uint16_t msgType;
    uint16_t dstPort;
}ripNgOutputMsg_t;


/*! RIPng packet route table entry */
typedef struct ripNgPktRouteEntry_tag
{
    uint8_t ipv6Prefix[16];
    uint8_t routeTag[2];
    uint8_t prefixLen;
    uint8_t metric;
}ripNgPktRouteEntry_t;

/*! RipNg LQY to link cost table entry */
typedef struct ripNgLqiToLinkQEntry_tag
{
    uint8_t min;
    uint8_t max;
    uint16_t ripLink;
}ripNgLqiToLinkQEntry_t;


/*==================================================================================================
Public global variables declarations
==================================================================================================*/



/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*!*************************************************************************************************
\fn void RIPNG_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief This function initializes the RIPng  module.

\param [in]  pTaskMsgQueue     Pointer to the task message queue

\retval      none
***************************************************************************************************/
void RIPNG_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn void RIPNG_RegisterIf(ifHandle_t* pIfHandle)
\brief This function registers an interface to the RipNg module.

\param [in]  pIfHandle     Double pointer to the interface handle.

\retval      none
***************************************************************************************************/
void RIPNG_RegisterIf(ifHandle_t* pIfHandle);

/*!*************************************************************************************************
\fn ripNgRoutingTblEntry_t** RIPNG_GetRouteTblEntry(ipAddr_t* reqAddr)
\brief Searches the routing table for the provided IPv6 address.

\param [in]  reqAddr                 the address to seach for in the routing table

\retval      ripNgRoutingTblEntry_t** double pointer to routing table entry that matches the provided
                                      IPv6 address
***************************************************************************************************/
ripNgRoutingTblEntry_t** RIPNG_GetRouteTblEntry(ipAddr_t* reqAddr);

/*!*************************************************************************************************
\fn void RIPNG_SetRouteTblEntry(ipAddr_t* setAddr, uint8_t prefixLen, ipAddr_t* nextHopAddr,
         ifHandle_t* pIfHandle,uint8_t metric)
\brief Sets a new RIPng entry in the routing table.

\param [in]  setAddr       the destination address to set in the routing table
\param [in]  prefixLen     prefix length of the address to set in the routing table
\param [in]  nextHopAddr   the address of the next hop router to destination
\param [in]  pIfHandle     double pointer to the interface of the next hop router to destination
\param [in]  metric        route metric

\retval      none
***************************************************************************************************/
void RIPNG_SetRouteTblEntry(ipAddr_t* setAddr,uint8_t prefixLen,ipAddr_t* nextHopAddr,
                                       ifHandle_t* pIfHandle,uint8_t metric);


#ifdef __cplusplus
}
#endif

#endif /* RIPNG_ENALBE */

/*================================================================================================*/
#endif  /*_RIPNG_H_ */
