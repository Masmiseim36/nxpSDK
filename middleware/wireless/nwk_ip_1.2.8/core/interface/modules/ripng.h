/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
