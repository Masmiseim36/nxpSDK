/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
/* getsockopt available options and parameter sizes
SOL_SOCKET -> SO_TYPE -> uint32_t (4 bytes)
SOL_SOCKET -> SO_SOCKETBLOCK -> bool_t (1 byte)
SOL_SOCKET -> SO_RCVBUF -> uint32_t (4 bytes)

SOL_IP -> IP_PKTINFO -> recvOptions_t** (4 bytes-double pointer to recvOptions_t structure) */

/* setsockopt available options and parameter sizes
SOL_SOCKET -> SO_BINDTODEVICE -> uint32_t (4 bytes)
SOL_SOCKET -> SO_SOCKETBLOCK -> bool_t (1 byte)
SOL_SOCKET -> SO_REUSESOCK ->  uint32_t (4 bytes)

SOL_IP -> IPV6_JOIN_GROUP -> ipMreq_t (sizeof(ipMreq_t) - 32 bytes)
SOL_IP -> IPV6_LEAVE_GROUP -> ipMreq_t (sizeof(ipMreq_t) - 32 bytes)
SOL_IP -> IP_ADD_MEMBERSHIP -> ipMreq_t (sizeof(ipMreq_t) - 32 bytes)
SOL_IP -> IP_DROP_MEMBERSHIP -> ipMreq_t (sizeof(ipMreq_t) - 32 bytes)

SOL_IP -> IPV6_MULTICAST_HOPS -> uint8_t (1 byte)
SOL_IP -> IPV6_UNICAST_HOPS -> uint8_t (1 byte)
SOL_IP -> IP_MULTICAST_TTL -> uint8_t (1 byte)
SOL_IP -> IP_TTL -> uint8_t (1 byte)

SOL_IP -> IP_TOS -> uint8_t (1 byte)
SOL_IP -> IPV6_TCLASS -> uint8_t (1 byte) */

/*-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
  INFORMATIVE MACROS THAT SHOULD NOT BE CHANGED BY THE USER
  (THESE ARE USED BY THREAD LIBRARY AT COMPILE TIME)
-------------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
#ifndef BSDS_DATAGRAM_SUPPORT
    #define BSDS_DATAGRAM_SUPPORT       (TRUE)  /*!< Enable datagram sockets(using UDP) */
#endif
#ifndef BSDS_STREAM_SUPPORT
    #define BSDS_STREAM_SUPPORT         (FALSE) /*!< Enable stream sockets(using TCP) */
#endif
#ifndef BSDS_BLOCKING_SOCKET
    #define BSDS_BLOCKING_SOCKET        (FALSE) /*!< Enable blocking sockets */
#endif
#ifndef BSDS_SELECT_SUPPORT
    #define BSDS_SELECT_SUPPORT         (FALSE) /*!<Enable sockets select functionality */
#endif
#if (TCP_ENABLED) && (!BSDS_STREAM_SUPPORT)
    #undef BSDS_STREAM_SUPPORT
    #define BSDS_STREAM_SUPPORT         (TRUE)  /*!< Enable sockets support for TCP */
#endif

#ifndef BSDS_OPTIONS_SUPPORT
    #define BSDS_OPTIONS_SUPPORT        (TRUE)  /*!< Enable socket options support */
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

#define MSG_DONTWAIT          0x40      /*!< Nonblocking socket send/receive flag. Blocking
                                             send is not currently supported. For blocking receive,
                                             if flag is not set and no data is present in the receive
                                             queue receive function will block waiting for data to
                                             read from the transport layer. */

#define MSG_SEND_WITH_MEMBUFF 0x40000   /*!< If set the application data MUST be located in a memory
                                             buffer. If the memory buffer was allocated from an
                                             application memory pool that buffer will not be
                                             available to the application until the packet is sent
                                             but the speed will be increased as the memory doesn't
                                             need to be copied. Don't include if transport layer
                                             should allocate memory and copy the application data
                                             payload from msg pointer. */

#define MSG_GET               0x80000000 /*!< Get memory buffer where data is stored without needing to
                                              provide a separate memory location where data is copied.
                                              Warning, this memory buffer must be freed by the
                                              applicaiton and must not be kept for a long time as
                                              it reduces stack memory. This feature increases receive
                                              speed by not requiring an extra memory copy. */

#define IPV6_UNICAST_HOPS       (16)    /*!< Set the unicast hop limit for the socket. */
#define IPV6_MULTICAST_HOPS     (18)    /*!< Set the multicast hop limit for the socket. */
#define IPV6_ADD_MEMBERSHIP     (20)    /*!< Joins the IPv6 multicast group specified. */
#define IPV6_DROP_MEMBERSHIP    (21)    /*!< Leaves the IPv6 multicast group specified. */
#define IPV6_JOIN_ANYCAST       (27)    /*!< Joins the anycast group specified. */
#define IPV6_TCLASS             (67)    /*!< Set the traffic class field of IPv6 header. */

#define IP_TOS               (1)    /*!< Sets the TOS field from IPv4 header. */
#define IP_TTL               (2)    /*!< Sets the Time To Live (TTL) in the IP header for unicast
                                         packets. */
#define IP_ADD_MEMBERSHIP   (35)    /*!< Joins the multicast group specified. */
#define IP_DROP_MEMBERSHIP  (36)    /*!< Leaves the multicast group specified. */
#define IP_MULTICAST_TTL    (33)    /*!< Sets the Time To Live (TTL) in the IP header for outgoing
                                         multicast datagrams. */

#define IP_PKTINFO          (35)    /*!< Only for getsockopt, returns IP packet info structure for
                                         the first packet in the socket queue for both IPv4 and IPv6. */

#ifndef IPV6_JOIN_GROUP     /* APIv0 compatibility */
#define IPV6_JOIN_GROUP     IPV6_ADD_MEMBERSHIP     /*!< Joins the IPv6 multicast group specified. */
#endif

#ifndef IPV6_LEAVE_GROUP
#define IPV6_LEAVE_GROUP    IPV6_DROP_MEMBERSHIP    /*!< Leaves the IPv6 multicast group specified. */
#endif

/*! Set default socket flow info flag of the sockaddrIn6 struture */
#define BSDS_DEFAULT_FLOW_FLAGS                             ((uint32_t)-1)

/*! Set MAC security flags in the flow info field of the sockaddrIn6 struture. These settings are
    only relevant for 802.15.4 */
#define BSDS_SET_TX_SEC_FLAGS(macKeyIdMode, macSecLevel)    (((macKeyIdMode) << 4) | (macSecLevel))

#ifndef FD_SETSIZE
#define FD_SETSIZE          0   /*!< File descriptor list size to select/poll on */
#endif

#define SOCK_STATUS_SUCCESS 0 /*!< socket success return value 0 */

/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef enum
{
    gBsdsSockUnbound_c,             /*!< Socket is not in use */
    gBsdsSockBound_c,               /*!< Socket is bound to an address/port combination */
    gBsdsSockListening_c,           /*!< Socket is in listening state */
    gBsdsSockUnConnected_c,         /*!< Socket is not connected */
    gBsdsSockConnected_c            /*!< Socket is connected */
} sockStateErr_t;                 /*!< Socket states */

typedef enum
{
    NOERROR,
    EBADF,                          /*!< Sockfd is not a valid file descriptor */
    EADDRINUSE,                     /*!< The given address is already in use(combination of port
                                         and source IP address) */
    ENOFREEPORT,                    /*!< No more free ports to allocate when port value is
                                         auto-select(port value 0) */
    EISCONN,                        /*!< The socket is connected or a connection is in progress */
    EINVAL,                         /*!< addrlen is wrong, or addr is not a valid address(IP or port)
                                         for this socket's domain. For getsockopt/setsockopt optVal or optLen
                                         are invalid(invalid value or NULL pointer).
                                         For accept(), socket is not listening for connections */
    ENOPROTOOPT,                     /*!< The option is unknown at the level indicated. */
    ENOMEM,                          /*!< Insufficient entries available for allocating a resource.
                                          For accept(), a new socket could not be created because
                                          no free entries are available */
    ENOBUFS,                         /*!< Insufficient memory buffers or no memmory buffer of the
                                          required size available. */

    ENOTCONN,                        /*!< The socket is not connected, and no target has been given. */
    ESOCKBOUND,                      /*!< The socket is allready bound. */
    ESOCKNOTBOUND,                   /*!< The socket is not bound and socket function requires it. */
    EALREADY,                        /*!< The socket is nonblocking and a previous connection attempt
                                          has not yet been completed. */
    EOPNOTSUPP,                      /*!< The referenced socket is not of type that supports the
                                          socket fucntion. */
    EADDRNOTAVAIL,                   /*!< A nonexistent interface was requested or the requested
                                          address   was not local. */
    EMSGSIZE,                        /*!< The socket type requires that message be sent atomically, and
                                          the size of the message to be sent made this impossible. */

} sockErrno_t; /*!< Socket error number values */

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
    SO_TYPE         = 0x0000,       /*!< Socket type(Datagram or Stream). Compatible name for SO_STYLE */
    SO_BINDTODEVICE = 0x0019,       /*!< Bind this socket to an interface */
    SO_RCVBUF       = 0x1002,       /*!< Last received buffer size */
    SO_SOCKETBLOCK  = 0x1003,       /*!< Enable/Disable blocking mode of operation on socket */
    SO_REUSESOCK    = 0x1004        /*!< Allows rebinding same UDP socket with a different address/port
                                         combination. This is mostly usefull for changing the source
                                         address of an UDP packet */
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
    mSockFlagUsed_c         = 0x01,  /*!< The socket is allocated */
    mSockFlagBlock_c        = 0x02,  /*!< The socket is blocking */
    mSockFlagStatic_c       = 0x04,  /*!< The socket was allocated from a static pool */
    mSockFlagReuse_c        = 0x08   /*!< The socket allows rebinding (UDP only) */
} sockFlags_t;
#endif /* BSDS_OPTIONS_SUPPORT */

typedef uint32_t socklen_t;

#if BSDS_SELECT_SUPPORT
typedef struct fdSet_tag{
    int32_t fds[BSDS_SELECT_MAX_FDS];
} fdSet_t; /*!< Structure containing file descriptors set information */
#endif /* BSDS_SELECT_SUPPORT */

typedef struct ipMreq
{
    ipAddr_t imrMultiaddr;   /*!< IP multicast group address */
    ipAddr_t imrInterface;   /*!< IP address of local interface -> one of the source addresses of
                                  interface we want to use for the multicast group address. Must be
                                  a valid source IP address of one interface */
} ipMreq_t; /*!< Structure used for multicast group join/leave setsockopt() function */

typedef struct socketCallback_tag
{
    /*! Socket bind callback */
    int32_t(*SocketBind)(int32_t sockfd, sockaddrStorage_t *pLocalAddr, uint32_t addrlen);
    /*! Socket connect callback */
    int32_t(*SocketConnect)(int32_t sockfd, sockaddrStorage_t *serv_addr, uint32_t addrLen);
    /*! Socket listen (TCP) callback */
    int32_t(*SocketListen)(int32_t sockfd, uint32_t backlog);
    /*! Socket accept (TCP) callback */
    int32_t(*SocketAccept)(int32_t sockfd, sockaddrStorage_t *addr, uint32_t addrLen);
    /*! Socket recv callback */
    int32_t(*SocketRecv)(int32_t sockfd, uint8_t *msg, uint32_t msgLen, uint32_t flags);
    /*! Socket recvfrom (UDP) callback */
    int32_t(*SocketRecvFrom)(int32_t sockfd, uint8_t *msg, uint32_t msgLen, uint32_t flags,
                             sockaddrStorage_t *from, socklen_t fromLen);
    /*! Socket send callback */
    int32_t(*SocketSend)(int32_t sockfd, uint8_t *msg, uint32_t msgLen, uint32_t flags);
    /*! Socket sendto (UDP) callback */
    int32_t(*SocketSendto)(int32_t sockfd, ipAddr_t* pAddr, uint8_t *msg, uint32_t msgLen, uint32_t flags,
                           sockaddrStorage_t *to, socklen_t toLen);
    /*! Socket shutdown (TCP) callback */
    int32_t(*SocketShutdown)(int32_t sockfd, uint32_t how);
}socketCallback_t; /*!< Structure for holding pointer to functions for each transport module */

typedef struct socket_tag
{
    socketCallback_t *pCallback;    /*!< Pointer to socket callbacks */
    uint8_t addrFam;                /*!< Address family */
    uint8_t type;                   /*!< Socket type(datagram or stream) */
    uint8_t state;                  /*!< Socket status of the connection */
    uint8_t errno;                  /*!< Error number - set by the socket layer in case a socket function
                                         returns -1. Use getsockerrno() to get error code */
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
}sock_t; /*!< BSD socket structure(information about local and remote is found in the transport
                                   connection) */

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

\param  [in]    domain      Domain which can be AF_INET or AF_INET6
\param  [in]    type        Type of socket(SOCK_DGRAM or SOCK_STREAM)
\param  [in]    protocol    Transport protocol to be used(IPPROTO_UDP or IPPROTO_TCP)

\return         int32_t     Socket file descriptor or -1 in case the socket could not be created
***************************************************************************************************/
int32_t socket
(
    uint8_t domain,
    uint8_t type,
    uint8_t protocol
);

/*!*************************************************************************************************
\fn     int32_t shutdown(int32_t sockfd, int32_t how)
\brief  This function shuts down part of a full-duplex connection(only for TCP). Calling shutdown
        tells the TCP peer that we have no more data to send and await their end command to fully
        close the connection. Until FIN is received from the peer, receiving is still possible on the
        socket. After full closure, call closesock() to free socket file descriptor.

\param  [in]    sockfd  socket descriptor
\param  [in]    how     (UNUSED)parameter which specifies how the connection will be closed

\retval         0       on success
\retval        -1       on failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t shutdown
(
    int32_t sockfd,
    int32_t how
);
/*!*************************************************************************************************
\fn     int32_t closesock(int32_t sockfd)
\brief  This function closes a socket file descriptor and frees all socket allocated resources

\param  [in]    sockfd  socket descriptor

\retval         0       on success
\retval        -1       on failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t closesock
(
    int32_t sockfd
);

/*!*************************************************************************************************
\fn     int32_t bind(int32_t sockfd, sockaddrStorage_t *pAddr, uint32_t addrLen)
\brief  Public interface function for Sockets module. This function is used to bind a local IP
        address and a local port to an existing socket. SO_REUSEADDR allows binding of the same port
        with 2 sockets, one with in6addr_any and the other one a specific source address. This
        option is always enabled and cannot be disabled. SO_REUSEPORT allows binding of the same
        source address/port pair on 2 sockets. This option is always disabled an cannot be enabled
        as the network layer does not support load balancing on 2 identical sockets.

\param  [in]    sockfd  socket descriptor
\param  [in]    pAddr   pointer to the socket address structure
\param  [in]    addrLen size of the pAddr structure

\retval         0       on success
\retval        -1       on failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t bind
(
    int32_t sockfd,
    const sockaddrStorage_t *pLocalAddr,
    uint32_t addrlen
);

/*!*************************************************************************************************
\fn     int32_t send(int32_t sockfd, const uint8_t *msg, uint32_t msgLen, uint32_t flags)
\brief  This function is used to send data to a connected socket.

\param  [in]    sockfd  socket descriptor
\param  [in]    msg     pointer to the data that needs sending
\param  [in]    msgLen  length of the data that needs sending
\param  [in]    flags   flags used for sending
                        - MSG_DONTWAIT - non blocking send, blocking is not yet supported
                        - MSG_SEND_WITH_MEMBUFF - don't include if transport layer should allocate
                        memory and copy the application data payload from msg pointer. If set the
                        application data MUST be located in a memory buffer. If the memory buffer
                        was allocated from an application memory pool that buffer will not be
                        available to the application until the packet is sent but the speed will be
                        increased as the memory doesn't need to be copied

\return         int32_t length of the data sent, -1 on failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t send
(
    int32_t sockfd,
    uint8_t *msg,
    uint32_t msgLen,
    uint32_t flags
);

/*!*************************************************************************************************
\fn     int32_t sendmsg(int32_t sockfd, uint8_t *msg, uint32_t msgLen, uint32_t flags,
                        sockaddrStorage_t *pTo, socklen_t toLen)
\brief  This function is used to send data to a specific destination IP address and port with a
        specific IP source address. Only available for UDP sockets as TCP sockets need to be connected.

\param  [in]    sockfd   Socket descriptor
\param  [in]    pSrc     Pointer to local address
\param  [in]    msg      Pointer to the data that needs sending
\param  [in]    msgLen   Length of the data that needs sending
\param  [in]    flags    Flags used for sending
                         - MSG_DONTWAIT - non blocking send, blocking is not supported
                         - MSG_SEND_WITH_MEMBUFF - don't include if transport layer should allocate
                         memory and copy the application data payload from msg pointer. If set the
                         application data MUST be located in a memory buffer. If the memory buffer
                         was allocated from an application memory pool that buffer will not be
                         available to the application until the packet is sent but the speed will be
                         increased as the memory doesn't need to be copied
\param  [in]    pTo      Pointer to the remote socket address structure. For sending encrypted
                         IPv6 packets inside a Thread network, flowInfo field must be initialized
                         with  BSDS_SET_TX_SEC_FLAGS(1,5) (macKeyIdMode 1 and macSecLevel 5 ->
                         standard Thread MAC security). This field can be populated directly with
                         the macro or as a parameter of NWKU_SetSockAddrInfo() function.
\param  [in]    toLen    Size of the remote address structure

\return         int32_t  Length of the data sent, -1 on failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t sendmsg
(
    int32_t sockfd,
    const ipAddr_t *pSrc,
    uint8_t *msg,
    uint32_t msgLen,
    uint32_t flags,
    const sockaddrStorage_t *pTo,
    socklen_t toLen
);

/*!*************************************************************************************************
\fn     int32_t sendto(int32_t sockfd, uint8_t *msg, uint32_t msgLen, uint32_t flags,
        sockaddrStorage_t *pTo, uint32_t toLen)
\brief  This function is used to send data to a specific destination IP address and port. Only
        available for UDP sockets as TCP sockets need to be connected..

\param  [in]    sockfd   Socket descriptor
\param  [in]    msg      Pointer to the data that needs sending
\param  [in]    msgLen   Length of the data that needs sending
\param  [in]    flags    Flags used for sending
                         - MSG_DONTWAIT - non blocking send, blocking is not supported
                         - MSG_SEND_WITH_MEMBUFF - don't include if transport layer should allocate
                         memory and copy the application data payload from msg pointer. If set the
                         application data MUST be located in a memory buffer. If the memory buffer
                         was allocated from an application memory pool that buffer will not be
                         available to the application until the packet is sent but the speed will be
                         increased as the memory doesn't need to be copied
\param  [in]    pTo      Pointer to the remote socket address structure. For sending encrypted
                         IPv6 packets inside a Thread network, flowInfo field must be initialized
                         with  BSDS_SET_TX_SEC_FLAGS(1,5) (macKeyIdMode 1 and macSecLevel 5 ->
                         standard Thread MAC security). This field can be populated directly with
                         the macro or as a parameter of NWKU_SetSockAddrInfo() function.
\param  [in]    toLen    Size of the remote address structure

\return         int32_t  Length of the data sent, -1 on failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t sendto
(
    int32_t sockfd,
    uint8_t *msg,
    uint32_t msgLen,
    uint32_t flags,
    const sockaddrStorage_t *pTo,
    uint32_t toLen
);

/*!*************************************************************************************************
\fn     int32_t recv(int32_t sockfd, uint8_t *msg, uint32_t msgLen, uint32_t flags)
\brief  This function is used to get data from a socket receive queue. If non blocking mode is used
        see Session.h on how to register a data event callback that will trigger when new data is
        received on the socket.

\param  [in]    sockfd   Socket descriptor
\param  [out]   msg      Pointer to the buffer responsible for holding received data. If MSG_GET is
                         used it must be a double pointer so that receive funcion can return
                         location of dynamic memory buffer where data is stored
\param  [in]    msgLen   Length of the buffer allocated for receiving data. Only relevant if MSG_GET
                         is not used
\param  [in]    flags    Flags used for receiving
                         - MSG_DONTWAIT - non blocking receive, if not set, Socket blocking
                         functionality is enabled and no data is present in the receive queue
                         receive function will block waiting for data to read from the transport layer
                         - MSG_GET - get memory buffer where data is stored without needing to
                         provide a separate memory location where data is copied. Warning, this
                         memory buffer must be freed by the applicaiton and must not be kept for a
                         long time as it reduces stack memory. This feature increases receive speed
                         by not requiring an extra memory copy.

\return         int32_t  Length of the data received, -1 on failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t recv
(
    int32_t sockfd,
    uint8_t *msg,
    uint32_t msgLen,
    uint32_t flags
);

/*!*************************************************************************************************
\fn     int32_t recvfrom(int32_t sockfd, uint8_t *msg, uint32_t msgLen, uint32_t flags,
                         sockaddrStorage_t *from, socklen_t *fromLen)
\brief  This function is used to get data from a socket receive queue. The remote information will
        be placed in the from structure. If non blocking mode is used see Session.h on how to
        register a data event callback that will trigger when new data is
        received on the socket.

\param  [in]    sockfd   Socket descriptor
\param  [out]   msg      Pointer to the buffer responsible for holding received data. If MSG_GET is
                         used it must be a double pointer so that receive funcion can return
                         location of dynamic memory buffer where data is stored
\param  [in]    msgLen   Length of the buffer allocated for receiving data. Only relevant if MSG_GET
                         is not used
\param  [in]    flags    Flags used for receiving
                         - MSG_DONTWAIT - non blocking receive, if not set, Socket blocking
                         functionality is enabled and no data is present in the receive queue
                         receive function will block waiting for data to read from the transport layer
                         - MSG_GET - get memory buffer where data is stored without needing to
                         provide a separate memory location where data is copied. Warning, this
                         memory buffer must be freed by the applicaiton and must not be kept for a
                         long time as it reduces stack memory. This feature increases receive speed
                         by not requiring an extra memory copy.
\param  [out]   from     Pointer to the remote socket address structure
\param  [in]    fromLen  Pointer to the size of the remote address structure

\return         int32_t  Length of the data received, -1 on failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t recvfrom
(
    int32_t sockfd,
    uint8_t *msg,
    uint32_t msgLen,
    uint32_t flags,
    sockaddrStorage_t *from,
    socklen_t *fromLen
);

/*!*************************************************************************************************
\fn     int32_t connect(int32_t sockfd, sockaddrStorage_t *serv_addr, uint32_t addrLen)
\brief  This function is used to connect to a remote server. If socket is STREAM(TCP) and blocking
        sockets functionality is enabled and socket is blocking, this function will block
        until the socket is connected with the remote peer. If non blocking mode is used, function
        will return immediately with status success. For this case see Session.h on how to register
        a connection event callback that will trigger when socket is connected. For DATAGRAM(UDP)
        sockets this function retunrs immediately and only configures remote information in socket
        layer.

\param  [in]    sockfd      Socket descriptor
\param  [in]    serv_addr   Address structure for the server to connect to
\param  [in]    addrLen     Address structure length

\return         0           On success
\return        -1           On error (use getsockerrno to get error val)
***************************************************************************************************/
int32_t connect
(
    int32_t sockfd,
    const sockaddrStorage_t *serv_addr,
    uint32_t addrLen
);

#if BSDS_STREAM_SUPPORT
/*!*************************************************************************************************
\fn     int32_t listen(int32_t sockfd, uint32_t backlog)
\brief  This function is used to listen to a stream socket.

\param  [in]    sockfd   Socket descriptor
\param  [in]    backlog  (UNUSED)number of connections allowed on the incoming queue

\return         0        On success
\return        -1        On failure (use getsockerrno to get error val)
***************************************************************************************************/
int32_t listen
(
    int32_t sockfd,
    uint32_t backlog
);

/*!*************************************************************************************************
\fn     int32_t accept(int32_t sockfd, sockaddrStorage_t *pAddr, uint32_t addrLen)
\brief  This function is used to accept a connection from a client only for STREAM(TCP) sockets. If
        blocking sockets functionality is enabled and socket is blocking, this function will block
        until the socket is connected with a remote peer. If non blocking mode is used, function
        will return immediately with a new socket descritor that cand be used when the socket is
        connected. For this case see Session.h on how to register a connection event callback that
        will trigger when a connection is accepted.

\param  [in]    sockfd   Socket descriptor
\param  [out]   pAddr    Address structure for remote client
\param  [in]    addrLen  pAddr structure size

\return         int32_t  Socket descriptor of the newly accepted socket or -1 on failure (use
                         getsockerrno to get error val).
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
\fn     int32_t getsockopt(int32_t sockfd, int32_t level, int32_t optName, uint8_t *optVal,
        int32_t *optLen)
\brief  This function retrieves information about a specified socket.

\param  [in]    sockfd              Socket file descriptor
\param  [in]    level               Layer for operation
\param  [in]    optName             Option
\param  [out]   optVal              Pointer to the value for the option
\param  [out]   optLen              Pointer to the length of the option

\retval         0                   if the option was set
\retval        -1                   if the option cannot be set (use getsockerrno to get error val)
***************************************************************************************************/
int32_t getsockopt
(
    int32_t sockfd,
    int32_t level,
    int32_t optName,
    uint8_t *optVal,
    int32_t *optLen
);

/*!*************************************************************************************************
\fn     int32_t setsockopt(int32_t sockfd, int32_t level, int32_t optName, uint8_t *optVal,
        uint32_t optLen)
\brief  This function sets information for a specified socket.

\param  [in]    sockfd              Socket file descriptor
\param  [in]    level               Layer for operation
\param  [in]    optName             Option
\param  [in]    optVal              Pointer to the value for the option
\param  [in]    optLen              The length of the option

\retval         0                   if the option was set
\retval        -1                   if the option cannot be set (use getsockerrno to get error val)
***************************************************************************************************/
int32_t setsockopt
(
    int32_t sockfd,
    int32_t level,
    int32_t optName,
    uint8_t *optVal,
    uint32_t optLen
);

/*!*************************************************************************************************
\fn     int32_t getsockname(int32_t sockfd, sockaddrStorage_t *pAddr, socklen_t *pAddrlen)
\brief  This function retrieves information about the local address and port of a socket.

\param  [in]    sockfd              socket file descriptor
\param  [out]   pAddr               a pointer to a structure containing the local information
\param  [out]   addrLen             pointer to the size of the pAddr structure

\retval         0                   if the name can be retrieved
\retval        -1                   if the name cannot be retrieved (use getsockerrno to get error val)
***************************************************************************************************/
int32_t getsockname
(
    int32_t sockfd,
    sockaddrStorage_t *pAddr,
    socklen_t *addrlen
);

/*!*************************************************************************************************
\fn     uint8_t getsockerrno(int32_t sockFd)
\brief  This function returns the last error number code generated by socket a function call on that
        socket

\param  [in]    sockFd    Socket descriptor

\return         uint8_t   Socket error number
***************************************************************************************************/
uint8_t getsockerrno
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
