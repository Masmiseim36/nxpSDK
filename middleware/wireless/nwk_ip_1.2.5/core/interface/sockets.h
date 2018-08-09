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

/*! *********************************************************************************
 * \defgroup SOCKETS  Network IP Sockets Interface
 * @{
 ***********************************************************************************/

#ifndef _SOCKETS_H
#define _SOCKETS_H


/*!
\file       sockets.h
\brief      This is a header file for the Sockets module. It contains the sockets implementation
            using the BSD interface.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "network_utils.h"
#include "udp.h"
#if TCP_ENABLED
#include "tcp.h"
#endif

/*==================================================================================================
Public macros
==================================================================================================*/
/*-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
  INFORMATIVE MACROS THAT SHOULD NOT BE CHANGED BY THE USER
  (THESE ARE USED BY THREAD LIBRARY AT COMPILE TIME)
-------------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
#ifndef BSDS_DATAGRAM_SUPPORT
    #define BSDS_DATAGRAM_SUPPORT       (TRUE)  /*!< Support datagram sockets(using UDP) */
#endif
#ifndef BSDS_STREAM_SUPPORT
    #define BSDS_STREAM_SUPPORT         (FALSE) /*!< Support stream sockets(using TCP) */
#endif
#ifndef BSDS_BLOCKING_SOCKET
    #define BSDS_BLOCKING_SOCKET        (FALSE) /*!< Support blocking sockets */
#endif
#ifndef BSDS_SELECT_SUPPORT
    #define BSDS_SELECT_SUPPORT         (FALSE) /*!< Sockets module support select functionality */
#endif
#if (TCP_ENABLED) && (!BSDS_STREAM_SUPPORT)
    #undef BSDS_STREAM_SUPPORT
    #define BSDS_STREAM_SUPPORT         (1)     /*!< Whether sockets support TCP */
#endif

#ifndef BSDS_OPTIONS_SUPPORT
    #define BSDS_OPTIONS_SUPPORT        (TRUE)  /*!< Support socket options */
#endif

#define BSDS_RECV_EVENT             (0x04U)   /*!< Event to be used for Socket receive blocking */
#define BSDS_CANCEL_SELECT_EVENT    (0x08U)   /*!< Event to be used for Socket select blocking */
#define BSDS_CONN_DONE_EVENT        (0x10U)   /*!< Event to be used for receiving a connection */

#define SOCK_DGRAM                  (0)     /*!< Datagram socket type */
#define SOCK_STREAM                 (1)     /*!< Stream socket type */

/* Protocol families, same as address families. */
#define PF_INET     AF_INET     /*!< IPv4 family */
#define PF_INET6    AF_INET6    /*!< IPv6 family */

/* Flags we can use with send/ and recv.
   Added those for 1003.1g not all are supported yet
 */

#define MSG_DONTWAIT          0x40      /*!< Nonblocking IO */
#define MSG_SEND_WITH_MEMBUFF 0x40000   /*!< socket send app data is located in a memory buffer
                                            allocated from memory pools and will be freed by
                                            the stack */
#define MSG_GET              0x80000000 /*!< Message get */

#define IPV6_UNICAST_HOPS       (16)    /*!< Set the unicast hop limit for the socket */
#define IPV6_MULTICAST_HOPS     (18)    /*!< Set the multicast hop limit for the socket */
#define IPV6_ADD_MEMBERSHIP     (20)    /*!< Joins the IPv6 multicast group specified */
#define IPV6_DROP_MEMBERSHIP    (21)    /*!< Leaves the IPv6 multicast group specified */
#define IPV6_MTU                (24)    /*!< Set/Get MTU only for connected socket */
#define IPV6_JOIN_ANYCAST       (27)    /*!< Joins the anycast group specified */
#define IPV6_TCLASS             (67)    /*!< Set the traffic class field of IPv6 header */

#define IP_TOS               (1)    /*!< Sets the TOS field from IPv4 header */
#define IP_TTL               (2)    /*!< Sets the Time To Live (TTL) in the IP header for unicast 
                                         packets */
#define IP_ADD_MEMBERSHIP   (35)    /*!< Joins the multicast group specified */
#define IP_DROP_MEMBERSHIP  (36)    /*!< Leaves the multicast group specified */
#define IP_MULTICAST_IF     (32)    /*!< Sets the interface over which outgoing multicast datagrams
                                         are sent */
#define IP_MULTICAST_TTL    (33)    /*!< Sets the Time To Live (TTL) in the IP header for outgoing
                                         multicast datagrams */
#define IP_MULTICAST_LOOP   (34)    /*!< Specifies whether or not a copy of an outgoing multicast
                                         datagram is delivered to the sending host as long as it is 
                                         a member of the multicast group */

#ifndef IPV6_JOIN_GROUP     /* APIv0 compatibility */
#define IPV6_JOIN_GROUP     IPV6_ADD_MEMBERSHIP     /*!< Joins the IPv6 multicast group specified */
#endif

#ifndef IPV6_LEAVE_GROUP
#define IPV6_LEAVE_GROUP    IPV6_DROP_MEMBERSHIP    /*!< Leaves the IPv6 multicast group specified */
#endif

/*!< Get default socket flow flag */
#define BSDS_DEFAULT_FLOW_FLAGS                             ((uint32_t)-1)
/*!< Set MAC security flags */
#define BSDS_SET_TX_SEC_FLAGS(macKeyIdMode, macSecLevel)    (((macKeyIdMode) << 4) | (macSecLevel))

#ifndef FD_SETSIZE
#define FD_SETSIZE          0   /*!< File descriptor list size to select/poll on */
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    gBsdsSockSuccess_c      = 0U,   /*!< Success */
    gBsdsSockUnavailable_c  = 1U,   /*!< Socket to be removed is not available */
    gBsdsSockAdded_c        = 2U,   /*!< Socket was successfully added to list */
    gBsdsSockRemoved_c      = 3U,   /*!< Socket was successfully removed from list */
    gBsdsSockListFull_c     = 4U,   /*!< The sockets list is full */
    gBsdsSockError_g        = 5U,   /*!< Error */
    gBsdsSockPortInUse_c    = 6U,   /*!< Port was already used */
    gBsdsNoMoreFreePorts_c  = 7U,   /*!< There are no more free ports */
    gBsdsSockFound_c        = 8U,   /*!< Socket was found in list */
    gBsdsSockInvalid_c      = THR_ALL_FFs32   /*!< Invalid */
} sockFuncErr_t; /*!< Socket functions error codes */

typedef enum
{
    gBsdsSockUnbound_c,             /*!< Socket is not in use */
    gBsdsSockBound_c,               /*!< Socket is bound to an address/port combination */
    gBsdsSockListening_c,           /*!< Socket is in listening state */
    gBsdsSockUnConnected_c,         /*!< Socket is not connected */
    gBsdsSockConnected_c            /*!< Socket is connected */
} sockStateErr_t; /*!< Socket states */

#if BSDS_OPTIONS_SUPPORT
typedef enum
{
    SOL_SOCKET,                     /*!< Sockets layer */
    SOL_IP,                         /*!< IP layer */
    SOL_UDP,                        /*!< UDP layer */
    SOL_TCP,                        /*!< TCP layer */
    SOL_MAC                         /*!< MAC layer */
} sockOptLayer_t; /*!< Socket options layers */

typedef enum
{
    SO_TYPE,                        /*!< Compatible name for SO_STYLE */
    SO_REUSEADDR    = 0x0004,       /*!< Reuse IP address before previous connection to the same
                                         address is closed */
    SO_BINDTODEVICE = 0x0019,       /*!< Bind this socket to an interface */
    SO_PEERNAME     = 0x001C,       /*!< sockAddrStorage of the peer */
    SO_PEERSEC      = 0x001F,       /*!< Peer security */
    SO_REUSEPORT    = 0x0200,       /*!< Allow local address and port reuse */
    SO_SNDBUF       = 0x1001,       /*!< Send buffer size */
    SO_RCVBUF       = 0x1002        /*!< Last received buffer size */
} sockOptGeneric_t; /*!< Generic socket options */

typedef enum
{
    UDP_PKTOPT = 0
}udpSockOpt_t;

typedef enum
{
    TCP_PKTOPT = 0
}tcpSockOpt_t;

typedef enum
{
    MAC_SECURITY_KEY_ID_MODE = 1
}macSockOpt_t;

typedef enum
{
    mSockFlagUsed_c         = 0x01, /*!< The socket is allocated */
    mSockFlagBlock_c        = 0x02, /*!< The socket is blocking */
    mSockFlagStatic_c       = 0x04  /*!< The socket was allocated from a static pool */
} sockFlags_t;
#endif /* BSDS_OPTIONS_SUPPORT */

/* Socket structures */
typedef struct sockaddrIn
{
    ipAddr_t      sin_addr;     /*!< Internet address     */
    uint16_t      sin_family;   /*!< Address family       */
    uint16_t      sin_port;     /*!< Port number          */
} sockaddrIn_t; /*!< Structure containing socket address information(IPV4) */

typedef struct sockaddrIn6
{
    ipAddr_t    sin6_addr;      /*!< IPV6 address */
    uint16_t    sin6_family;    /*!< The address family we used when we set up the socket (AF_INET6) */
    uint16_t    sin6_port;      /*!< The port number (the transport address) */
    uint32_t    sin6_flowinfo;  /*!< IPV6 flow information
                                    (LSB= (MAC key id mode) | (MAC security level) ) */
    uint32_t    sin6_scope_id;  /*!< set of interfaces for a scope (RFC2553) or media interface
                                    handle */
} sockaddrIn6_t; /*!< Structure containing socket address information(IPV6) */

typedef struct sockaddrStorage
{
    ipAddr_t    ss_addr;        /*!< Internet address */
    uint16_t    ss_family;      /*!< Address family */
    uint8_t     _data[sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t)];  /*!< Storage large enough and aligned
                                for storing the socket address data structure of any family */
} sockaddrStorage_t; /*!< Structure containing socket address information */

typedef uint32_t socklen_t;

#if BSDS_SELECT_SUPPORT
typedef struct fdSet_tag{
    int32_t fds[BSDS_SELECT_MAX_FDS];
} fdSet_t; /*!< Structure containing file descriptors set information */
#endif /* BSDS_SELECT_SUPPORT */

struct timeval
{
  uint32_t tv_sec;      /*!< seconds */
  uint32_t tv_usec;     /*!< microseconds */
}; /*!< Structure containing time information */

typedef struct ipMreq
{
    ipAddr_t imrMultiaddr;   /*!< IP multicast group address */
    ipAddr_t imrInterface;   /*!< IP address of local interface */
} ipMreq_t; /*!< Request structure for multicast socket options */

typedef struct socketCallback_tag
{
    /*!< Socket bind callback */
    int32_t(*SocketBind)(int32_t sockfd, sockaddrStorage_t *pLocalAddr, uint32_t addrlen);
    /*!< Socket connect (TCP) callback */
    int32_t(*SocketConnect)(int32_t sockfd, sockaddrStorage_t *serv_addr, int addrLen);
    /*!< Socket listen (TCP) callback */
    int32_t(*SocketListen)(int32_t sockfd, uint32_t backlog);
    /*!< Socket accept (TCP) callback */
    int32_t(*SocketAccept)(int32_t sockfd, sockaddrStorage_t *addr, int addrLen);
    /*!< Socket recv (TCP) callback */
    int32_t(*SocketRecv)(int32_t sockfd, void *msg, uint32_t msgLen, uint32_t flags);
    /*!< Socket recvfrom (UDP) callback */
    int32_t(*SocketRecvFrom)(int32_t sockfd, void *msg, uint32_t msgLen, uint32_t flags,
                             sockaddrStorage_t *from, socklen_t fromLen);
    /*!< Socket send (TCP) callback */
    int32_t(*SocketSend)(int32_t sockfd, void *msg, uint32_t msgLen, uint32_t flags);
    /*!< Socket sendto (UDP) callback */
    int32_t(*SocketSendto)(int32_t sockfd, ipAddr_t* pAddr, void *msg, uint32_t msgLen, uint32_t flags,
                           sockaddrStorage_t *to, socklen_t toLen);
    /*!< Socket close (UDP/TCP) callback */
    int32_t(*SocketClose)(int32_t sockfd, int how);
}socketCallback_t; /*!< Structure for holding pointer to functions for each transport module */

typedef struct socket_tag
{
    uint8_t addrFam;                /*!< Address family */
    uint8_t prot;                   /*!< Protocol */
    uint8_t type;                   /*!< Socket type(datagram or stream) */
    uint8_t state;                  /*!< Socket status of the connection */

#if BSDS_OPTIONS_SUPPORT
    /* Socket options */
    uint32_t ipIfId;                /*!< ID of the interface used by this socket */
    uint16_t optMtu;                /*!< MTU value for this socket(only for connected sockets) */
    uint8_t optHopLimit;            /*!< Multicast hop limit for the socket(0-255) */
    uint8_t security;               /*!< Mac security level */
    uint8_t macSecKeyIdMode;        /*!< Mac security key ID mode */
    uint8_t optQos;                 /*!< Ip packet quality of service -> DSCP field in ip header */
#endif /* BSDS_OPTIONS_SUPPORT */

    uint8_t flags;                  /*!< Socket flags */
    uint8_t tspConnIndex;           /*!< Transport connection index */
    uint8_t sockPad;                /*!< padding */

    socketCallback_t *pCallback;    /*!< Pointer to socket callbacks */
}sock_t; /*!< BSD socket structure(information about local and remote is found in the transport
connection) */
/* 20 bytes without UDP pointer */

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern const socketCallback_t sockDgramCallback;
extern const socketCallback_t sockStreamCallback;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     int32_t socket(uint8_t domain, uint8_t type, uint8_t protocol)
\brief  This function creates a socket structure(and initialize its values with default) using a
        specific domain, type and protocol.

\param  [in]    domain      Domain which can be PF_INET or PF_INET6
\param  [in]    type        Type of socket(SOCK_DGRAM or SOCK_STREAM)
\param  [in]    protocol    Transport protocol to be used(IPPROTO_UDP or IPPROTO_TCP)

\return         int32_t     Socket file descriptor or -1 in case of error
***************************************************************************************************/
int32_t socket
(
    uint8_t domain,
    uint8_t type,
    uint8_t protocol
);

/*!*************************************************************************************************
\fn     int32_t shutdown(int32_t sockfd, int how)
\brief  This function close a socket connection.

\param  [in]    sockfd  Socket descriptor
\param  [in]    how     (UNUSED)parameter which specifies how the socket will be closed

\return         0       On success
\return         -1      On failure
***************************************************************************************************/
int32_t shutdown
(
    int32_t sockfd,
    int how
);

/*!*************************************************************************************************
\fn     int32_t bind(int32_t sockfd, sockaddrStorage_t *pAddr, uint32_t addrLen)
\brief  Public interface function for Sockets module. This function is used to bind a local IP
        address and a local port to an existing socket.

\param  [in]    sockfd   Socket descriptor
\param  [in]    pAddr    Pointer to the socket address structure
\param  [in]    addrLen  Size of the pAddr structure

\return         0        On success
\return         -1       On failure
***************************************************************************************************/
int32_t bind
(
    int32_t sockfd,
    sockaddrStorage_t *pLocalAddr,
    uint32_t addrlen
);

/*!*************************************************************************************************
\fn     int32_t send(int32_t sockfd, void *msg, uint32_t msgLen, uint32_t flags)
\brief  This function is used to send data to a connected socket.

\param  [in]    sockfd    Socket descriptor
\param  [in]    msg       Pointer to the data which needs to be send
\param  [in]    msgLen    Length of the data which needs to be send
\param  [in]    flags     Flags used for sending

\return         int32_t   Length of the data sent, -1 on failure
***************************************************************************************************/
int32_t send
(
    int32_t sockfd,
    void *msg,
    uint32_t msgLen,
    uint32_t flags
);

/*!*************************************************************************************************
\fn     int32_t sendmsg(int32_t sockfd, void *msg, uint32_t msgLen, uint32_t flags,
                        sockaddrStorage_t *pTo, socklen_t toLen)
\brief  This function is used to send data to a specific socket.

\param  [in]    sockfd   Socket descriptor
\param  [in]    pSrc     Pointer to local address
\param  [in]    msg      Pointer to the data which needs to be send
\param  [in]    msgLen   Length of the data which needs to be send
\param  [in]    flags    Flags used for sending
\param  [in]    pTo      Pointer to the remote socket address structure
\param  [in]    toLen    Size of the remote address structure

\return         int32_t  Length of the data sent, -1 on failure
***************************************************************************************************/
int32_t sendmsg
(
    int32_t sockfd,
    ipAddr_t *pSrc,
    void *msg,
    uint32_t msgLen,
    uint32_t flags,
    sockaddrStorage_t *pTo,
    socklen_t toLen
);

/*!*************************************************************************************************
\fn     int32_t sendto(int32_t sockfd, void *msg, uint32_t msgLen, uint32_t flags,
        sockaddrStorage_t *pTo, uint32_t toLen)
\brief  This function is used to send data to a specific socket.

\param  [in]    sockfd   Socket descriptor
\param  [in]    msg      Pointer to the data which needs to be send
\param  [in]    msgLen   Length of the data which needs to be send
\param  [in]    flags    Flags used for sending
\param  [in]    pTo      Pointer to the remote socket address structure
\param  [in]    toLen    Size of the remote address structure

\return         int32_t  Length of the data sent, -1 on failure
***************************************************************************************************/
int32_t sendto
(
    int32_t sockfd,
    void *msg,
    uint32_t msgLen,
    uint32_t flags,
    sockaddrStorage_t *pTo,
    uint32_t toLen
);

/*!*************************************************************************************************
\fn     int32_t recv(int32_t sockfd, void *msg, uint32_t msgLen, uint32_t flags)
\brief  This function is used to get data from a socket RX queue.

\param  [in]    sockfd   Socket descriptor
\param  [out]   msg      Pointer to the buffer responsible for holding received data
\param  [in]    msgLen   Length of the buffer allocated for receiving data
\param  [in]    flags    Flags used for receiving

\return         int32_t  Length of the data received, -1 on failure
***************************************************************************************************/
int32_t recv
(
    int32_t sockfd,
    void *msg,
    uint32_t msgLen,
    uint32_t flags
);

/*!*************************************************************************************************
\fn     int32_t recvfrom(int32_t sockfd, void *msg, uint32_t msgLen, uint32_t flags,
                         sockaddrStorage_t *from, socklen_t *fromLen)
\brief  This function is used to get data from a specific socket from the RX queue. The remote
        information will be placed in the from structure.

\param  [in]    sockfd   Socket descriptor
\param  [out]   msg      Pointer to the buffer responsible for holding received data
\param  [in]    msgLen   Length of the buffer allocated for receiving data
\param  [in]    flags    Flags used for receiving
\param  [out]   from     Pointer to the remote socket address structure
\param  [in]    fromLen  Pointer to the size of the remote address structure

\return         int32_t  Length of the data received, -1 on failure
***************************************************************************************************/
int32_t recvfrom
(
    int32_t sockfd,
    void *msg,
    uint32_t msgLen,
    uint32_t flags,
    sockaddrStorage_t *from,
    socklen_t *fromLen
);

/*!*************************************************************************************************
\fn     int32_t connect(int32_t sockfd, sockaddrStorage_t *serv_addr, uint32_t addrLen)
\brief  This function is used to connect to a remote server.

\param  [in]    sockfd      Socket descriptor
\param  [in]    serv_addr   Address structure for the server to connect to
\param  [in]    addrLen     Address structure length

\return         0           On success
\return         -1          On error
***************************************************************************************************/
int32_t connect
(
    int32_t sockfd,
    sockaddrStorage_t *serv_addr,
    uint32_t addrLen
);

#if BSDS_STREAM_SUPPORT
/*!*************************************************************************************************
\fn     int32_t listen(int32_t sockfd, uint32_t backlog)
\brief  This function is used to listen to a stream socket.

\param  [in]    sockfd   Socket descriptor
\param  [in]    backlog  (UNUSED)number of connections allowed on the incoming queue

\return         0        On success
\return         -1       On failure
***************************************************************************************************/
int32_t listen
(
    int32_t sockfd,
    uint32_t backlog
);

/*!*************************************************************************************************
\fn     int32_t accept(int32_t sockfd, sockaddrStorage_t *pAddr, uint32_t addrLen)
\brief  This function is used to accept a connection from a client.

\param  [in]    sockfd   Socket descriptor
\param  [out]   pAddr    Address structure for remote client
\param  [in]    addrLen  Address structure length

\return         int32_t  Pointer to the newly accepted socket or -1 on failure
***************************************************************************************************/
int32_t accept
(
    int32_t sockfd,
    sockaddrStorage_t *pAddr,
    uint32_t addrLen
);
#endif /* BSDS_STREAM_SUPPORT */


#if BSDS_SELECT_SUPPORT
/*!*************************************************************************************************
\fn     bool_t FD_ZERO(fdSet_t *pSet)
\brief  This function is used to clear all file descriptors in a file descriptor set.

\param  [in]    pSet    Pointer to the set of file descriptors

\return         TRUE    The call was successful
\return         FALSE   The call failed
***************************************************************************************************/
bool_t FD_ZERO(fdSet_t *pSet);

/*!*************************************************************************************************
\fn     bool_t FD_SET(int32_t fd, fdSet_t *pSet)
\brief  This function is used to set a file descriptor in a set of file descriptors.

\param  [in]    fd      File descriptor
\param  [in]    pSet    Pointer to the set of file descriptors

\return         TRUE    The file descriptor could be set
\return         FALSE   The file descriptor could not be set
***************************************************************************************************/
bool_t FD_SET(int32_t fd, fdSet_t *pSet);

/*!*************************************************************************************************
\fn     bool_t FD_ISSET(int32_t fd, fdSet_t *pSet)
\brief  This function is used to check if a file descriptor is set in a set of file descriptors.

\param  [in]    fd      File descriptor
\param  [in]    pSet    Pointer to the set of file descriptors

\return         TRUE    The file descriptor is set
\return         FALSE   The file descriptor is not set
***************************************************************************************************/
bool_t FD_ISSET(int32_t fd, fdSet_t *pSet);

/*!*************************************************************************************************
\fn     bool_t FD_CLR(int32_t fd, fdSet_t *pSet)
\brief  This function is used to clear a file descriptor from a set of file descriptors.

\param  [in]    fd      File descriptor
\param  [in]    pSet    Pointer to the set of file descriptors

\return         TRUE    The file descriptor could be cleared
\return         FALSE   The file descriptor could not be cleared
***************************************************************************************************/
bool_t FD_CLR(int32_t fd, fdSet_t *pSet);

#endif /* BSDS_SELECT_SUPPORT */

/*!*************************************************************************************************
\fn     int32_t getsockopt(int32_t sockfd, int32_t level, int32_t optName, void *optVal,
                           int32_t *optLen)
\brief  This function retrieves information about a specified socket.

\param  [in]    sockfd              Socket file descriptor
\param  [in]    level               Layer for operation
\param  [in]    optName             Option
\param  [out]   optVal              Pointer to the value for the option
\param  [out]   optLen              Pointer to the length of the option

\retval         gBsdsSockSuccess_c  If the option was set
\retval         gBsdsSockError_g    If the option cannot be set
***************************************************************************************************/
int32_t getsockopt
(
    int32_t sockfd,
    int32_t level,
    int32_t optName,
    void *optVal,
    int32_t *optLen
);

/*!*************************************************************************************************
\fn     int32_t setsockopt(int32_t sockfd, int32_t level, int32_t optName, void *optVal,
                           int32_t optLen)
\brief  This function sets information for a specified socket.

\param  [in]    sockfd              Socket file descriptor
\param  [in]    level               Layer for operation
\param  [in]    optName             Option
\param  [in]    optVal              Pointer to the value for the option
\param  [in]    optLen              The length of the option

\retval         gBsdsSockSuccess_c  If the option was set
\retval         gBsdsSockError_g    If the option cannot be set
***************************************************************************************************/
int32_t setsockopt
(
    int32_t sockfd,
    int32_t level,
    int32_t optName,
    void *optVal,
    int32_t optLen
);

/*!*************************************************************************************************
\fn     int32_t getsockname(int32_t sockfd, sockaddrStorage_t *pAddr, socklen_t *addrlen)
\brief  This function retrieves information about the local address and port of a socket.

\param  [in]    sockfd              Socket file descriptor
\param  [out]   pAddr               A pointer to a structure containing the local information
\param  [out]   addrlen             Pointer to the size of the pAddr structure

\retval         gBsdsSockSuccess_c  If the name can be retrieved
\retval         gBsdsSockError_g    If the name cannot be retrieved
***************************************************************************************************/
int32_t getsockname
(
    int32_t sockfd,
    sockaddrStorage_t *pAddr,
    socklen_t *addrlen
);

/*!*************************************************************************************************
\fn     sock_t *getsocket(int32_t sockFd)
\brief  This function gets a pointer to the socket structure.

\param  [in]    sockFd    Socket descriptor

\return         sock_t*   Pointer to the socket structure or NULL if it doesn't exist
***************************************************************************************************/
sock_t *getsocket
(
    int32_t sockFd
);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _SOCKETS_H */
/*!
** @}
*/
