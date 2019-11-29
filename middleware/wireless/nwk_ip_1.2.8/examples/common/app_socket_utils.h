/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP.
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_SOCKET_UTILS_H
#define _APP_SOCKET_UTILS_H

/*!=================================================================================================
\file       app_socket_utils.h
\brief      This is a header file for the applications sockets utilities.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"
#include "TimersManager.h"

#include "sockets.h"
#include "session.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#ifndef gUSER_MAX_SOCKETS
    #define gUSER_MAX_SOCKETS           (5U)
#endif

/*!< Port used by the benchmark application */
#define APP_BENCH_PORT                          (7007)   
#define APP_BENCH_TX_PACKET_SIZE_DEFAULT        (1472)
#define APP_BENCH_TX_PACKET_NUMBER_DEFAULT      (10000)
#define APP_BENCH_TX_ITERATION_NUMBER_DEFAULT   (1)
#define APP_BENCH_TX_ITERATION_NUMBER_MAX       (10000)
#define APP_BENCH_BUFFER_SIZE                   (200)  
#define APP_BENCH_BUFFER_MAX_SIZE               (210)
#define APP_BENCH_END_BUFFER_LENGTH             (1)
#define APP_BENCH_START_BUFFER_LENGTH           (2)
#define SOCKET_INVALID                          (-1)
#define APP_BENCH_TX_UDP_END_ITERATIONS         (10)

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum {
    gSockInvalidCmd,    
    gSockOpenUdp,    
    gSockOpenTcp,
    gSockConnect,
    gSockAccept,
    gSockClose,
    gSockShutdown,
    gSockSend,
    gSockSelect,
    gSockPost,
    gSockBenchRxTcp,
    gSockBenchRxUdp,
    gSockBenchTxTcp,
    gSockBenchTxUdp,
}appSocketsCmd_t;

typedef enum {
    gSockUdp,
    gSockTcp
}appSocketsTrans_t;

typedef struct appSockCmdParams_tag
{
    appSocketsCmd_t   appSocketsCmd;
    appSocketsTrans_t appSocketsTrans;
    uint32_t          ipVersion;
    /*!< The port number (the transport address) */
    union {
        uint16_t          sin6_port;
        uint16_t          sin_port;
    };
    /*!< IPV4/IPV6 address */
    union {
        ipAddr_t          sin6_addr;
        ipAddr_t          sin_addr;
    };
    uint32_t sock32;
    uint8_t* pData;
    uint8_t  dataLen;
}appSockCmdParams_t;

typedef struct userSock_tag
{
    int32_t userSockFd;                      /* Socket file descriptor */
    uint32_t index;                          /* Index of the file descriptor in the global file descriptors array */
    uint8_t sockType;                        /* SOCK_DGRAM or SOCK_STREAM */
    sockaddrStorage_t *pRemoteAddr;          /* Pointer to sockAddrStorage_t structure(only in tcp active mode) */

} userSock_t;

typedef struct throughputBench_tag 
{
    appSocketsTrans_t appSocketsTrans;
    bool_t bTestRunning;
    int32_t clientSocket;
    int32_t foreignSocket;
    uint64_t cbytesRecv;
    uint64_t cbytesSent;
    uint64_t firstTime;
    uint64_t lastTime;
    uint8_t *pBuffer;              /* Transmit circular buffer */
} throughputBench_t;            /*  Structure containing connection information */ 

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern userSock_t       gaUserSockID[]; /* User socket list */

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void APP_InitUserSock(taskMsgQueue_t * pAppSockThreadMsgQueue)
\brief  This function is used to initialize user sockets

\param  [in]    pAppSockThreadMsgQueue pointer to the message queue

\return         void
***************************************************************************************************/
void APP_InitUserSockets(taskMsgQueue_t * pAppSockThreadMsgQueue);

/*!*************************************************************************************************
\fn     bool_t App_SocketSendAsync(appSockCmdParams_t* pAppSockCmdParams)                                         
\brief  This function is called for sending a generic Socket application request to App socket utils.

\param [in]   pAppSockCmdParams   Socket application parameters.

\retval       bool_t    Return status. TRUE if the message was sent successfully, FALSE if not.
***************************************************************************************************/
bool_t App_SocketSendAsync(appSockCmdParams_t* pAppSockCmdParams);

/*!************************************************************************************************
*
\fn     void APP_SocketClientRxCallback(void *param)
\brief  This function is called in an endless loop.

\param  [in]    param   unused

\return         void
***************************************************************************************************/

void APP_SocketClientRxCallback(uint8_t *param);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/


#endif /* _APP_SOCKET_UTILS_H */
