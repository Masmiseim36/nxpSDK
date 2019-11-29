/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _TCP_H
#define _TCP_H
/*!=================================================================================================
\file       tcp.h
\brief      This is a header file for the TCP module. It contains the TCP implementation from
            RFC 793.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "ip.h"
#include "GenericList.h"

#ifndef TCP_ENABLED
#   define TCP_ENABLED              (0)     /*!< Enable/disable module */
#endif

#ifndef TCP_RCV_WND_MANAGEMENT
#   define TCP_RCV_WND_MANAGEMENT   (1)     /*!< Enable window management */
#endif

#ifndef TCP_HANDLE_FULL_QUEUE
#   define TCP_HANDLE_FULL_QUEUE    (0)     /*!< When the RX queue is full block the TCP(IP) Task
                                             with osSignalWait and when one element is extracted
                                             from the queue unblock the TCP(IP) task with
                                             osSignalSet and add the waiting packet */
#endif

#ifndef TCP_CONGESTION
#   define TCP_CONGESTION           (0)     /*!< "Congestion Avoidance and Control," V. Jacobson,
                                            ACM SIGCOMM-88, August 1988 */
#endif

#ifndef TCP_STATS_ENABLE
#   define TCP_STATS_ENABLE         (1)
#endif

#ifndef TCP_OPTIONS_ENABLE
#   define TCP_OPTIONS_ENABLE       (1)
#endif

#ifndef TCP_REUSEADDR
#   define TCP_REUSEADDR            (1)
#endif

#ifndef TCP_TREAT_ICMP_PORT_UNR
#   define TCP_TREAT_ICMP_PORT_UNR  (1)     /*!< treat ICMP Port Unreachable error */
#endif
/* TCP configurations */
#ifndef TCP_DEBUG
#   define TCP_DEBUG                (0)     /*!< TCP debugging messages */
#endif

/*==================================================================================================
Public macros
==================================================================================================*/
#define TCP_MAX_SYN_RETRANSM        (6)     /*!< Maximum number of SYN retransmissions */
#define TCP_MAX_DATA_RETRANSM       (12)    /*!< Maximum number of data retransmissions */

#ifndef TCP_MSS_DEFAULT
    #define TCP_MSS_DEFAULT         (530) /*!< IPv6 (1280 - 40 - 20) */
#endif
#ifndef TCP_WND
    #define TCP_WND                 (530) /*!< SHould be TCP_MSS_DEFAULT * TCP_MAX_RX_PACKETS */
#endif

#define TCP_DEFAULT_TX_BUFF_SIZE    (536)
#define TCP_DEFAULT_RX_BUFF_SIZE    (536)
#define TCP_DEFAULT_TTL             (255)   /*!< Time to live  */
#define TCP_SND_BUFF                (12 * TCP_MSS_DEFAULT)
#define TCP_SND_Q_SIZE              (40)    /*!< Maximum number of segments to send */

#define TCP_MAX_SEG_LIFETIME        (120000)/*!< Maximum segment lifetime(ms): 2 minutes */

/* TCP timer settings */
#define TCP_FAST_TIMER_PERIOD       (250)   /*!< [ms] */
#define TCP_SLOW_TIMER_PERIOD       (500)   /*!< [ms] */
#define TCP_CONN_EXPIRE_TIMEOUT     (75)    /*!< connection expire timeout[s] */
#define TCP_DELAYED_ACK_IMEOUT      (200)   /*!< [ms]  */
#define TCP_DEFAULT_RTO             (3)     /*!< [s]  */

/* Events for TCP */
#define TCP_TX_DONE                 (0x10U) /* Event to be used for TCP TX blocking */
#define TCP_QUEUE_NOT_FULL          (0x20U)

#define TCP_HEADER_SIZE             (20)    /*!< TCP header size in bytes */
#define TCP_HEADER_SIZE_W           (5)     /*!< TCP header size in 32bit words */

/* Macros for comparing sequence numbers(in modulo 32 math) */
#define TCP_SEQ_LT(a,b)             ((int32_t)((uint32_t)(a) - (uint32_t)(b)) < 0)
#define TCP_SEQ_LEQ(a,b)            ((int32_t)((uint32_t)(a) - (uint32_t)(b)) <= 0)
#define TCP_SEQ_GT(a,b)             ((int32_t)((uint32_t)(a) - (uint32_t)(b)) > 0)
#define TCP_SEQ_GEQ(a,b)            ((int32_t)((uint32_t)(a) - (uint32_t)(b)) >= 0)

#define TCP_NODELAY                 (1)     /*!< Turn off Nagle's algorithm. */
#define TCP_DONTWAIT                (0x01U) /*!< Blocking TX flag */

#define TCP_DEFAULT_KEEP_IDLE       (7200000U)  /*!< RFC1122 keep Idle time[ms] */

#if TCP_STATS_ENABLE
#   define TCP_STATS_INC(x)         (++x)
#else
#   define TCP_STATS_INC(x)         ((void)x)
#endif

#define TF_ACK_DELAY        ((uint8_t)0x01U)    /*!< Delayed ACK. */
#define TF_ACK_NOW          ((uint8_t)0x02U)    /*!< Immediate ACK. */
#define TF_INFR             ((uint8_t)0x04U)    /*!< In fast recovery. */
#define TF_TIMESTAMP        ((uint8_t)0x08U)    /*!< Timestamp option enabled */
#define TF_RXCLOSED         ((uint8_t)0x10U)    /*!< rx closed by tcp_shutdown */
#define TF_FIN              ((uint8_t)0x20U)    /*!< Connection was closed locally (FIN segment
                                                     enqueued). */
#define TF_NODELAY          ((uint8_t)0x40U)    /*!< Disable Nagle algorithm */
#define TF_NAGLEMEMERR      ((uint8_t)0x80U)    /*!< nagle enabled, memerr, try to output to prevent
                                                     delayed ACK to happen */

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    mTcpOK_g                   = 0x0000U, /*!<  */
    mTcpConnEstablisehd_g,                /*!<  */
    mTcpError_g,                          /*!<  */

    mTcpErrInsufRes_g,                    /*!<  */
    mTcpErrForSockUnspec_g,               /*!<  */
    mTcpErrConnClosing_g,                 /*!< Connection is being closed by peer */
    mTcpErrConnClosed_g,                  /*!< Connection closed */
    mTcpErrConnReset_g,                   /*!< Connection reset by peer */
    mTcpErrConnDoesNotExist_g,            /*!<  */
    mTcpErrConnInUse_g,                   /*!<  */
    mTcpErrPortInUse_g,                   /*!<  */
    mTcpErrNoFreePorts_g,                 /*!<  */
    mTcpErrWrongState_g,                  /*!<  */
    mTcpErrAbortTimeout_g,                /*!< Connection aborted due to user timeout */

    mTcpErrIpSend_g,                      /*!< Error in calling IP_Send() */
} tcpErrorCode_t;

typedef enum
{
    mTcpStateClosed_g = 0U, /*!< Connection is closed */
    mTcpStateListen_g,      /*!< waiting for a connection request from any remote TCP and port */
    mTcpStateSynSent_g,     /*!< have sent SYN(active open) */
    mTcpStateSynRecvd_g,    /*!< have sent and received SYN, waiting for ACK */
    mTcpStateEstab_g,       /*!< connection is open, data received can be delivered to the user */
    mTcpStateFinWait1_g,    /*!< have closed, FIN was sent, waiting for ACK and FIN */
    mTcpStateFinWait2_g,    /*!< close, waiting for FIN */
    mTcpStateCloseWait_g,   /*!< received FIN, waiting for application close */
    mTcpStateClosing_g,     /*!< close on both peers, waiting for ACK */
    mTcpStateLastAck_g,     /*!< received FIN was closed, waiting for ACK */
    mTcpStateTimeWait_g     /*!< 2*MSL wait state after active close */
}tcpStates_t;

typedef enum
{
    mTcpEmptySegAck,        /*!< Send segment with ACK flag set and no payload */
    mTcpNonEmptySeg         /*!< Send segment containing payload */
}tcpSegType_t;

typedef enum
{
    mTcpOpenPassive_g,      /*!< Open connection in passive mode */
    mTcpOpenActive_g        /*!< Open connection in active mode */
}tcpOpenMode_t;

typedef enum
{
    mTcpFlagFIN_g = 0x01U,
    mTcpFlagSYN_g = 0x02U,
    mTcpFlagRST_g = 0x04U,
    mTcpFlagPSH_g = 0x08U,
    mTcpFlagACK_g = 0x10U,
    mTcpFlagURG_g = 0x20U
}tcpFlags_t; /*!< Flags used for the TCP header */

typedef enum
{
    mTcpConnFlagACKNow_g        = 0x0001U,      /*!< Immediate ACK */
    mTcpConnFlagACKDelayed_g    = 0x0002U,      /*!< Delayed ACK */
    mTcpConnFlagTS              = 0x0004U,      /*!< Time stamp enabled */
    mTcpConnFlagFin_g           = 0x0008U,      /*!< Connection was closed locally(FIN segment
                                                     enqueued) */
    mTcpConnFlagNoDelay_g       = 0x0010U,      /*!< Disable Nagle algorithm */

    mTcpConnFlagTxBlock_g       = 0x0020U,      /*!< TX is blocking */
    mTcpConnFlagSoReuseaddr_g   = 0x0040U,      /*!< SO_REUSEADDR socket option */
    mTcpConnFlagIFR_g           = 0x0080U,      /*!< In fast recovery */

    mTcpConnFlagActive_g        = 0x0100U,      /*!< Active connection(has initiated a connect) */
    mTcpConnFlagGetFin_g        = 0x0200U,      /*!< Send connection close event to upper layer after reading data */
    mTcpConnFlagReset_g         = 0x0400U,      /*!< This connection needs to send a reset segment */
    mTcpConnFlagGetData_g       = 0x0800U,      /*!< Move the data into upper layer after processing the packet */
}tcpConnFlags_t; /*!< Flags used for the connection */

typedef enum
{
    mTcpConnOptMSS_g = 0x01U,           /*!< MSS */
    mTcpConnOptTimestamp_g = 0x02U     /*!< timestamp */
}tcpConnOpt_t; /*!< TCP connection options */

typedef enum tcpOpt_tag
{
    mTcpOptEnd_g = 0x00U,       /*!< End of options */
    mTcpOptNoOp_g = 0x01U,      /*!< No operation/padding */
    mTcpOptMss_g = 0x02U,       /*!< Maximum segment size */
    mTcpOptTimestamp_g = 0x08U, /*!< Timestamp */

}tcpOpt_t; /*!< TCP header options */

/*
    0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |          Source Port          |       Destination Port        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        Sequence Number                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Acknowledgment Number                      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Data |           |U|A|P|R|S|F|                               |
   | Offset| Reserved  |R|C|S|S|Y|I|            Window             |
   |       |           |G|K|H|T|N|N|                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Checksum            |         Urgent Pointer        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Options                    |    Padding    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                             data                              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct tcpHeader_tag
{
    uint8_t srcPort[2];        /*!< Source port number */
    uint8_t dstPort[2];        /*!< Destination port number */
    uint8_t seqNumber[4];      /*!< Sequence number */
    uint8_t ackNumber[4];      /*!< Acknowledgment number */
    uint8_t dataAndFlags[2];   /*!< Data offset(4 bits) + Reserved(6 bits) + Control bits(6 bits) */
    uint8_t windowSize[2];     /*!< Window */
    uint8_t checksum[2];       /*!< Checksum */
    uint8_t urgPointer[2];
} tcpHeader_t; /*!< TCP header */

typedef struct tcpPacket_tag
{
    uint8_t         *pPayload;          /*!< The TCP data buffer */
    uint32_t        dataSize;           /*!< The length of the TCP data buffer(w/o the TCP header) */
    recvOptions_t   recvTcpPktOptions;  /*!< Information about received IP packet*/
    struct tcpPacket_tag *next;
} tcpPacket_t; /*!< TCP data including payload */

typedef struct tcpSeg_tag
{
    uint8_t     *payload;           /*!< TCP header + data buffer(SHOULD BE FREED!) */
    uint32_t    dataSize;           /*!< The length of the TCP segment including header and
                                         payload */
    tcpHeader_t *pTcpHdr;           /*!< Pointer to the header(DO NOT FREE THIS!) */
    uint8_t     flags;              /*!< Flags for this segment */
    uint8_t     reserved;           /*!< Used for alignment */
    uint16_t    rTime;              /*!< Retransmission time */
    struct tcpSeg_tag *next;
} tcpSeg_t; /*!< TCP segment representation */


typedef struct tcpConn_tag
{
    ipAddr_t        localIPAddr;    /*!< Local IP address */
    ipAddr_t        remIPAddr;      /*!< Remote IP address */

    list_t          rxList;

    uint16_t        localPort;      /*!< Local port number */
    uint16_t        remPort;        /*!< Remote port number */

    uint32_t        mss;            /*!< Maximum segment size  */
    uint16_t        sndBuffSize;    /*!< Maximum space available for sending[B] */

    /* Fast retransmit/recovery */
    uint8_t         dupAcks;        /*!< count of how many ACKs has been received for the sequence
                                        number in lastack */

    uint8_t         lastTimer;
    uint32_t        tcpTmr;

    ipIfUniqueId_t  tcpConIfUniqueId;    /*!< Interface unique Id */

    /* Send Variables */
    uint32_t        lastAck;        /*!< SND.UNA The sequence number acknowledged by the last ACK
                                         received */
    uint32_t        sndNxt;         /*!< The next sequence number to be sent */
    uint32_t        sndWnd;         /*!< The receiver's advertised window */
    uint32_t        sndWndMax;      /*!< Send window maximum size */
    uint32_t        sndUp;          /*!< Send urgent pointer */
    uint32_t        sndWl1;         /*!< Segment sequence number used for last window update */
    uint32_t        sndWl2;         /*!< Segment acknowledgment number used for last window update */
    uint32_t        iss;            /*!< Initial send sequence number */
    uint32_t        sndLbb;         /*!< Sequence number of next byte to be buffered(or the sequence
                                         number of the last byte queued for transmission ). */

    /* Receive Variables */
    uint32_t        rcvNxt;         /*!< Next sequence number expected(used when sending ACKs)
                                        It is the left or lower edge of the receive window */
    uint32_t        rcvWnd;         /*!< Receive window */
    //uint32_t        rcvUp;          /*!< Receive urgent pointer */
    uint32_t        rcvAnnRight;    /*!< Announced right edge of the receive window
                                        RCV.NXT + RCV.WND - 1 */
    uint16_t        rcvAnnWnd;      /*!< Receive window to announce */
    /* Congestion control */
#if TCP_CONGESTION
    uint16_t        cWnd;           /*!< The current congestion window[packets] */
    uint16_t        ssThreshold;    /*!< The slow start threshold*/
#endif /* TCP_CONGESTION */

    uint16_t        acked;          /*!< Number of acknowledged bytes */
    uint32_t        lastTimeStamp;  /*!< Recent timestamp */

    /* Retransmission timeout variables for Karn and Jacobson algorithm(max value is 2*MSL=240s) */
    uint32_t        rtSeqNb;        /*!< Sequence number for RTT estimation */
    int32_t         rttAvg;         /*!< RTT average */
    int32_t         rttVar;         /*!< RTT variance */
    uint32_t        rto;            /*!< Retransmission timeout[s] */
    uint16_t        rtt;            /*!< Round trip time estimation[s] */
    uint8_t         nbRetransm;     /*!< Number of retransmissions */
    int32_t         retransmTime;   /*!< Retransmission timer value for this connection. If it is >
                                        0 it is running */

    /* Segment queues */
    tcpSeg_t        *lastUnsent;    /*!< Pointer to the last unsent segment in the unsent Q */
    tcpSeg_t        *unsent;        /*!< Pointer to the start of the unsent Q(data from the APP
                                        which was not yet sent) */
    tcpSeg_t        *unackd;        /*!< Pointer to the start of the unack'd Q(data that was sent
                                        but not yet acknowledged) */
    uint16_t        unsentWaste;    /*!< Size of the data free at the end of the last unsent
                                            segment */

    uint16_t        flags;          /*!< Flags used by this connection(tcpConnFlags_t) */

    /* Events processing */
    tspDataIndCb_t  pTcpDataIndCb;  /*!< Pointer to the callback to be executed after some data
                                        was received on this connection */

    tspDataIndCb_t  pTcpConnIndCb;  /*!< Pointer to the callback to be executed after a connection
                                         event was detected, after connect() or accept(): */

    osaEventId_t    pTcpEventId;    /*!< Rx event: Pointer to the message queue of the thread to be woke up */

    uint32_t        keepIdle;       /*!< Time to wait until the next keep alive */

    struct tcpConn_tag* pActiveConn;/*!< Pointer to the active connection for this connection. This
                                            is only available for listening connections */


    tcpStates_t     state;          /*!< Connection's state */
    uint8_t         persTmrBackoff; /*!< Persistent timer back off  */
    uint8_t         persTmrCounter; /*!< Persistent timer counter  */

#if TCP_DEBUG
    uint32_t        firstSeqNb;
    uint32_t        firstAckNb;
#endif
    uint8_t         hopLimit;       /*!< Hop limit for IPv6 or TTL for IPv4 */

    uint8_t         tos;            /*!< Type of service */
    uint8_t         cAck;           /*!< Number of ACKs(used for delayed ACK mechanism) */
    uint16_t        sndSegQSize;    /*!< Number of segments in the send queue[segments] */

    tcpErrorCode_t  tcpConnStatus;  /*!< Tcp connection status that is signaled to the socket layer */
    uint32_t        tcpConnIndex;   /*!< Tcp connection index */
} tcpConn_t;  /*!< Structure containing one TCP connection data */

/* structure type used for message exchange between Upper Layers and Transport Layer */
typedef struct tcpMsgData_tag
{
    nwkBuffer_t     *pNwkBuff;      /*!< pointer to network buffer */
    tcpConn_t       *pTcpConn;      /*!< Pointer to the TCP connection */
    uint32_t        flags;          /*!< various flags */
    uint16_t        sndSegQSize;    /*!< Number of segments in the send queue[segments] */
}tcpMsgData_t;

typedef struct tcpStats_tag
{
    uint16_t xmit;                  /*!< Transmitted packets. */
    uint16_t recv;                  /*!< Received packets. */
    uint16_t fw;                    /*!< Forwarded packets. */
    uint16_t drop;                  /*!< Dropped packets. */
    uint16_t chkErr;                /*!< Checksum error. */
    uint16_t lenErr;                /*!< Invalid length error. */
    uint16_t memErr;                /*!< Out of memory error. */
    uint16_t rtErr;                 /*!< Routing error. */
    uint16_t protErr;               /*!< Protocol error/invalid packet */
    uint16_t optErr;                /*!< Error in options. */
    uint16_t err;                   /*!< Misc error. */
    uint16_t cacheHit;

    uint16_t resetRX;               /*!< Number of resets sent back from RX processing */
    uint16_t badAck;                /*!< Bad ACK count */
}tcpStats_t; /*!< Structure used to hold TCP statistics */

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
\fn    uint8_t TCP_Create(uint8_t domain)
\brief This function allocates a new TCP connection.

\param  [in]       Domain AF_INET for IPv4 or AF_INET6 for IPv6

\return uint8_t    TCP connection index
***************************************************************************************************/
uint8_t TCP_Create(uint8_t domain);

/*!*************************************************************************************************
\fn    void TCP_Init(void)
\brief Public interface function for a module. This function gets the destination address.

***************************************************************************************************/
void TCP_Init();

/*!*************************************************************************************************
\fn    tcpErrorCode_t TCP_Open(uint8_t tcpConnIndex, tcpOpenMode_t mode)
\brief Public interface function for the TCP module. This is used to open a new TCP connection.

\param [in]  tcpConnIndex        TCP connection index
\param [out] mode                whether to open the connection in passive mode or active mode

\retval mTcpOk_g                 if the open call succeeded
\retval mTcpErrForSockUnspec_g   if the socket information is not correctly configured
***************************************************************************************************/
tcpErrorCode_t TCP_Open(uint8_t tcpConnIndex, tcpOpenMode_t mode);

/*!*************************************************************************************************
\fn    void TCP_Close(uint8_t tcpConnIndex)
\brief Public interface function for the TCP module. This is used to close TCP connection.

\param [in]  tcpConnIndex   TCP connection index
 ***************************************************************************************************/
void TCP_Close(uint8_t tcpConnIndex);

/*!*************************************************************************************************
\fn    void TCP_Abort(uint8_t tcpConnIndex)
\brief Public interface function for the TCP module. This is used to force close a TCP connection.

\param [in]  tcpConnIndex   TCP connection index

\return      void
 ***************************************************************************************************/
void TCP_Abort(uint8_t tcpConnIndex);

/*!*************************************************************************************************
\fn    tcpErrorCode_t TCP_Bind(uint8_t tcpConnIndex, ipAddr_t *pIPAddr, uint16_t localPort)
\brief Public interface function for the TCP module. This is used to bind a local IP and port
       combination to an opened tcp connection.

\param [in] tcpConnIndex    TCP connection index
\param [in] pIPAddr         Pointer to the local ip address
\param [in] localPort       Local port number

\retval mTcpError           If the open call failed
\retval mTcpOk              If the open call succeeded
\retval mTcpPortInUse_g     If the required port is in use
\retval mTcpNoFreePorts_g   If all the ports were used
 ***************************************************************************************************/
tcpErrorCode_t TCP_Bind(uint8_t tcpConnIndex, ipAddr_t *pIPAddr, uint16_t localPort);

/*!*************************************************************************************************
\fn    int32_t TCP_Send(uint8_t tcpConnIndex, nwkBuffer_t *pAppNwkBufferData, uint32_t flags,
       uint8_t *pErrno)
\brief Public interface function for the TCP module. This is used to Send data on an opened TCP
connection.

Two modes are available: blocking and nonblocking.
If TCP_DONTWAIT flag is set then this function will just queue the packets(they will be sent on the
TCP slow timer timeout function).
If TCP_DONTWAIT flag is not set then this function will queue the data in segments and then the
segments will be sent to IP. After this, the function will be blocked until every byte in this data
buffer will be acknowledged.

This function can be called from every APP task(via sockets send() or directly).

\param [in] tcpConnIndex      TCP connection index
\param [in] pAppNwkBufferData pointer to the APP and network data nwkBUffer
\param [in] flags             flags used for send operation
\param [in] pErrno            pointer to store the error code


\return     uint32_t          the size of the data sent or -1 in case of error
***************************************************************************************************/
int32_t TCP_Send(uint8_t tcpConnIndex, nwkBuffer_t *pAppNwkBufferData, uint32_t flags, uint8_t *pErrno);

/*!*************************************************************************************************
\fn    tcpPacket_t *TCP_Peek(uint8_t tcpConnIndex)
\brief Public interface function for the TCP module. This is used to find if a connection has
       something in the RX queue.

\param [in]  tcpConnIndex   TCP connection index

\return      tcpPacket_t *  Pointer to the first available packet in the RX queue
 ***************************************************************************************************/
tcpPacket_t *TCP_Peek(uint8_t tcpConnIndex);

/*!*************************************************************************************************
\fn     tcpConn_t *TCP_GetConnInfo(uint8_t TCPConnIndex)
\brief  This function is used to get tcp connection info

\param  [in]   tcpConnIndex    Index to the TCP connection

\return        tcpConn_t*      Pointer to tcp connection structure
***************************************************************************************************/
tcpConn_t *TCP_GetConnInfo(uint8_t tcpConnIndex);

/*!*************************************************************************************************
\fn    int32_t TCP_Receive(uint8_t tcpConnIndex, uint8_t *buffer, uint32_t size, uint32_t flags)
\brief This function is used to get data from the RX queue for a specific TCP connection.

\param [in] buffer          pointer to the allocated buffer where to put the received data
\param [in] size            size of the buffer where to put the received data
\param [in] flags           flags

\return     uint32_t        the size of the data received
***************************************************************************************************/
int32_t TCP_Receive(uint8_t tcpConnIndex, uint8_t *buffer, uint32_t size, uint32_t flags);

/*!*************************************************************************************************
\fn     void TCP_RegisterDataIndCb(uint8_t tcpConnIndex, tspDataIndCb_t pDataIndCb)
\brief  Public interface function for the TCP module. This is used to register a callback to be
        called by each layer after any data was received.

\param  [in]    tcpConnIndex  TCP connection index
\param  [in]    pDataIndCb    Pointer to the callback
***************************************************************************************************/
void TCP_RegisterDataIndCb(uint8_t tcpConnIndex, tspDataIndCb_t pDataIndCb);

/*!*************************************************************************************************
\fn     void TCP_UnRegisterDataIndCb(uint8_t tcpConnIndex)
\brief  Public interface function for the TCP module. This is used to unregister a callback

\param  [in]    tcpConnIndex  TCP connection index
***************************************************************************************************/
void TCP_UnRegisterDataIndCb(uint8_t tcpConnIndex);

/*!*************************************************************************************************
\fn     void TCP_RegisterConnIndCb(uint8_t tcpConnIndex, tspDataIndCb_t pConnIndCb)
\brief  Public interface function for the TCP module. This is used to register a callback to be
        called by each layer after a connection event is received.

\param  [in]    tcpConnIndex  TCP connection index
\param  [in]    pDataIndCb    pointer to the callback

\return         void
***************************************************************************************************/
void TCP_RegisterConnIndCb(uint8_t tcpConnIndex,tspDataIndCb_t pConnIndCb);

/*!*************************************************************************************************
\fn     void TCP_UnRegisterConnIndCb(uint8_t tcpConnIndex)
\brief  This function is used to unregister the callback that is called after a connection event is
        received on a specific connection

\param  [in]    tcpConnIndex    index to the TCP connection

\return         void
***************************************************************************************************/
void TCP_UnRegisterConnIndCb(uint8_t tcpConnIndex);

/*!*************************************************************************************************
\fn    uint8_t TCP_ClearConnection(tcpConn_t *pTcpConn)
\brief Public function for the TCP module. This function is used to clear any data allocated in
       this connection, including the connection itself.

\param  [in]  tcpConnIndex    TCP connection index
***************************************************************************************************/
void TCP_ClearConnection(uint8_t tcpConnIndex);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _TCP_H */


