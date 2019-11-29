/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

 /*! *********************************************************************************
 * \defgroup UDP Interface
 * @{
 ***********************************************************************************/
#ifndef _UDP_H
#define _UDP_H

/*!
\file       udp.h
\brief      This is a public header file for the UDP module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "ip_if_management.h"
#include "ip.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#define UDP_MIN_PORT_NUMBER      (1024U) /*!< minimum port number available for users(1024) */
#define UDP_MAX_PORT_NUMBER      (4096U) /*!< maximum port number available for users(49151) */

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    gUdpStatusCallOK_c          = 0,    /*!< UDP OK */
    gUdpStatusCallError_c       = 1,    /*!< UDP connection error */
    gUdpStatusNoFreePorts_c     = 2,    /*!< No more free ports */
    gUdpStatusPortInUse_c       = 3,    /*!< Port is used by another connection */
} udpStatus_t;

typedef enum udpConnFlags_tag
{
    gUdpConnFlagUsedEntry_c     = 0x01, /*!< Used entry */
    gUdpConnFlagReusePort_c     = 0x02, /*!< Reuse port */
    gUdpConnFlagConnected_c     = 0x04, /*!< Udp has connection information */
}udpConnFlags_t;

typedef enum udpSearchStatus_tag
{
    gUdpSearchStatusPortUnUsed_c            = 0,    /*!< Port is unused */
    gUdpSearchStatusPortInUse_c             = 1,    /*!< Port is unused */
    gUdpSearchStatusTableFull_c             = 2,    /*!< Port is unused */
    gUdpSearchStatusConnIdxNotNull_c        = 3,    /*!< Port is unused */
    gUdpSearchStatusConnIdxOutOfRange_c     = 4,    /*!< Port is unused */
}udpSearchStatus_t;

typedef struct udpHeader_tag
{
    uint16_t    srcPort;                /*!< Source port number */
    uint16_t    dstPort;                /*!< Destination port number */
    uint16_t    length;                 /*!< UDP header + UDP data */
    uint16_t    checksum;               /*!< Checksum */
} udpHeader_t; /*!< UDP header */

typedef struct udpPacket_tag
{
    ipAddr_t        srcIpAddr;          /*!< Source IP address */
    ipAddr_t        remIpAddr;          /*!< Remote IP address */
    uint8_t         *udpPayload;        /*!< The UDP data buffer */
    uint8_t         *pStartPayload;     /*!< Pointer in the UDP data buffer which is used if
                                             multiple reads are required for getting this packet */
    uint16_t        dataSize;           /*!< The length of the UDP data buffer(w/o the UDP header) */
    uint16_t        remPort;            /*!< Remote port number */
    recvOptions_t   recvPktOptions;     /*!< Information about received IP packet*/
} udpPacket_t; /*!< UDP data including payload */

typedef struct udpConn_tag
{
    uint8_t         addrFam;            /* AF_INET for IPv4 or AF_INET6 for IPv6 */
    ipAddr_t        localIPAddr;
    ipAddr_t        remIPAddr;
    list_t          rxList;             /*!< receive queue */
    uint16_t        localPort;          /*!< Local port number */
    uint16_t        remPort;            /*!< Remote port number */
    tspDataIndCb_t  pUdpDataIndCb;      /*!< function pointer to be called when we have RX data */
    uint8_t         flags;              /*!< Flags (udpConnFlags_t) */
    uint8_t         hopLimit;           /*!< Hop limit for IPv6 or TTL for IPv4 */
    ipIfUniqueId_t  ipIfId;              /*!< ID of the interface */
} udpConn_t; /*!< Structure containing one UDP connection data */

typedef struct udpMsgData_tag
{
    ipAddr_t localIPAddr;               /*!< local IP address */
    ipAddr_t remIPAddr;                 /*!< remote IP address */
    uint16_t localPort;                 /*!< local port */
    uint16_t remPort;                   /*!< remote port */
    ipIfUniqueId_t ipIfId;              /*!< ID of the interface used to send this packet */

    uint8_t* appData;                    /*!< pointer to application data */
    uint32_t appDataSize;               /*!< application data size */

    uint8_t udpConIndex;                /*!< the index of the udp connection */
    uint8_t hopLimit;                   /*!< socket hop limit */
    uint8_t security;                   /*!< MAC security level */
    uint8_t macSecKeyIdMode;            /*!<  */
    uint8_t qos;                        /*!< packet Quality Of Service */
    uint8_t channel;                    /*!< channel */
    uint16_t destPanId;                 /*!< destination pan id*/
}udpMsgData_t; /*!< Structure used for message exchange between Upper Layers and Transport Layer */

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
\fn     void UDP_Init(taskMsgQueue_t *pTranspMsgQueue)
\brief  This function initialize the UDP module.

\param  [in]  pTranspMsgQueue  Pointer to the transport task message queue
***************************************************************************************************/
void UDP_Init(taskMsgQueue_t *pTranspMsgQueue);

/*!*************************************************************************************************
\fn     uint8_t UDP_Open(uint8_t domain)
\brief  This is function is used to open a new UDP connection. External UDP connections must be
        reserved by upper layer (if exist).

\param  [in]  domain   AF_INET for IPv4 or AF_INET6 for IPv6

\return       uint8_t  Index in the upd connection table of the new connection or THR_ALL_FFs8
                       in case of no memory
***************************************************************************************************/
uint8_t UDP_Open(uint8_t domain);

/*!*************************************************************************************************
\fn     void UDP_Close(uint8_t udpConnIndex)
\brief  This function is used to close an UDP connection.

\param  [in]  udpConnIndex  Pointer to the UDP connection
***************************************************************************************************/
void UDP_Close(uint8_t udpConnIndex);

/*!*************************************************************************************************
\fn     udpStatus_t UDP_Bind(uint8_t udpConnIndex, ipAddr_t *pIPAddr, uint16_t localPort, ipIfUniqueId_t ipIfId)
\brief  This function is used to bind a local IP and port combination to an opened UDP connection.

\param  [in]    udpConnIndex    UDP connection index
\param  [in]    pIPAddr         pointer to the local ip address
\param  [in]    localPort       local port number
\param  [in]    ipIfId          interface id

\retval         mUdpCallError   if the open call failed
\retval         mUdpCallOk      if the open call succeeded
***************************************************************************************************/
udpStatus_t UDP_Bind(uint8_t udpConnIndex, ipAddr_t *pIPAddr, uint16_t localPort, ipIfUniqueId_t ipIfId);

/*!*************************************************************************************************
\fn     int32_t UDP_Send(udpMsgData_t *pUdpParams, bool_t bUseAppDataAsBuffer, uint8_t *pErrno)
\brief  This function is used to Send data over an opened UDP connection.

\param  [in]    pUdpParams           pointer to the UDP parameters structure
\param  [in]    bUseAppDataAsBuffer  FALSE if UDP should allocate memory and copy the application
                                     data payload from pUdpParams and TRUE if the application data
                                     is located in a memory buffer
\param  [in]    pErrno               pointer to store the error code

\return         uint32_t             the size of the data sent or -1 in case of error
***************************************************************************************************/
int32_t UDP_Send(udpMsgData_t *pUdpParams,bool_t bUseAppDataAsBuffer, uint8_t *pErrno);

/*!*************************************************************************************************
\fn     udpPacket_t *UDP_Receive(uint8_t udpConnIndex)
\brief  This function is used to get data from the RX queue for a specific UDP connection.

\param  [in]  udpConnIndex  UDP connection index

\return       udpPacket_t*  Pointer to an udpPacket_t* structure or NULL in case no data is available
***************************************************************************************************/
udpPacket_t *UDP_Receive(uint8_t udpConnIndex);

/*!*************************************************************************************************
\fn     udpPacket_t *UDP_Peek(uint8_t udpConnIndex)
\brief  This function is used to find if a connection has any packet in the RX queue and to get
        information about that packet like interface source or destination.

\param  [in]  udpConnIndex  UDP connection index

\return       udpPacket_t   Pointer to the first available packet in the RX queue
***************************************************************************************************/
udpPacket_t *UDP_Peek(uint8_t udpConnIndex);

/*!*************************************************************************************************
\fn     udpConn_t *UDP_GetConnInfo(uint8_t udpConnIndex)
\brief  This function is used to get udp connection info

\param  [in]  udpConnIndex    Index to the UDP connection

\return       udpConn_t*      Pointer to udp connection structure
***************************************************************************************************/
udpConn_t *UDP_GetConnInfo(uint8_t udpConnIndex);

/*!*************************************************************************************************
\fn     void UDP_RegisterDataIndCb(uint8_t udpConnIndex, tspDataIndCb_t pDataIndCb)
\brief  This function is used to register a callback to be called by each layer after any data was
        received.

\param  [in]  udpConnIndex  Index to the UDP connection
\param  [in]  pDataIndCb    Pointer to the callback to be registered
***************************************************************************************************/
void UDP_RegisterDataIndCb(uint8_t udpConnIndex,tspDataIndCb_t  pDataIndCb);

/*!*************************************************************************************************
\fn     void UDP_RegisterDataIndCb(uint8_t udpConnIndex)
\brief  This function is used to unregister the callback that is called after data is received on a
        specific connection

\param  [in]  udpConnIndex  Pointer to the UDP connection
***************************************************************************************************/
void UDP_UnRegisterDataIndCb(uint8_t udpConnIndex);

/*!*************************************************************************************************
\fn     udpStatus_t UDP_Connect(uint8_t udpConnIndex, ipAddr_t *pDstIPAddr, uint16_t dstPort)
\brief  This function is used to set the remote peer.

\param  [in]  udpConnIndex  index to the UDP connection
\param  [in]  pDstIPAddr    pointer to the destination IP address
\param  [in]  dstPort       destination port number

\retval         mUdpCallOK             if the call succeeded
\retval         gUdpStatusCallError_c  if the call failed
***************************************************************************************************/
udpStatus_t UDP_Connect(uint8_t udpConnIndex, ipAddr_t *pDstIPAddr, uint16_t dstPort);

/*!*************************************************************************************************
\fn     uint8_t UDP_GetConnStatus(ipAddr_t *pIPAddr, uint16_t portNumber, ipIfUniqueId_t ipIfId,
                                  uint8_t *idx)
\brief  This function is used to search an opened UDP connection by IP address and port.

\param  [in]        pIPAddr       pointer to the IP address
\param  [in]        portNumber    the UDP port number
\param  [in]        ipIfId        interface Id
\param  [in/out]    idx           Returned pointer to UDP connection index

\return         udpSearchStatus_t     status of the connection
***************************************************************************************************/
udpSearchStatus_t UDP_GetConnStatus(ipAddr_t *pIPAddr, uint16_t portNumber,
                                                  ipIfUniqueId_t ipIfId, uint8_t* idx);

/*!*************************************************************************************************
\fn     void UDP4_RecomputeChecksum(ipPktInfo_t *pIpPktInfo)
\brief  This function is used to recompute the UDP checksum contained in an IPv4 packet

\param  [in]    pIpPktInfo      IPv4 packet
***************************************************************************************************/
void UDP4_RecomputeChecksum(ipPktInfo_t *pIpPktInfo);

/*!*************************************************************************************************
\fn     void UDP6_RecomputeChecksum(ipPktInfo_t *pIpPktInfo)
\brief  This function is used to recompute the UDP checksum contained in an IPv6 packet

\param  [in]    pIpPktInfo      IPv6 packet
***************************************************************************************************/
void UDP6_RecomputeChecksum(ipPktInfo_t *pIpPktInfo);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _UDP_H */
