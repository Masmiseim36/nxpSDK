/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_sockets_utils.c
\brief      This is a public source file for the socket server demo application.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include <string.h>
#include <stdio.h>

#include "shell.h"
#include "FunctionLib.h"

#include "network_utils.h"

#include "app_stack_config.h"
#include "app_socket_utils.h"
#include "app_temp_sensor.h"

/*==================================================================================================
Private macros
==================================================================================================*/

#define SOCK_DEMO_BLOCKING_MODE      0
#define SOCK_DEMO_THROUGHPUT         0
/*==================================================================================================
Private global variables declarations
==================================================================================================*/
#if SOCK_DEMO_THROUGHPUT
/*!< Benchmark information */
static throughputBench_t benchInfo;
#endif

static taskMsgQueue_t * pmAppSockThreadMsgQueue = NULL;
userSock_t gaUserSockID[gUSER_MAX_SOCKETS]; /* User socket list */

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/* None */

/*==================================================================================================
Private prototypes
==================================================================================================*/
static void App_SocketHandleSendAsync(uint8_t* pAppSockCmdParams);

static void App_SocketOpenBind(appSockCmdParams_t* pSockCmdParams);
static void App_SocketConnect(appSockCmdParams_t* pSockCmdParams);
static void App_SocketClose(appSockCmdParams_t* pSockCmdParams);
static void App_SocketSend(appSockCmdParams_t* pSockCmdParams);

#if TCP_ENABLED
    static void App_SocketAccept(appSockCmdParams_t* pSockCmdParams);
    static void App_SocketShutdown(appSockCmdParams_t* pSockCmdParams);
    static void APP_SockTcpConn(void *pParam);

#if !SOCK_DEMO_BLOCKING_MODE
static void APP_SocketConnectCallback(uint8_t *param);
static void APP_SocketAcceptCallback(uint8_t *param);
#endif

#endif /* TCP_ENABLED */

#if SOCK_DEMO_THROUGHPUT
static void App_ThroughputRxCallback(uint8_t *pPacket);
static void APP_ThroughputBenchRx(appSockCmdParams_t* pSockCmdParams);
static void App_ThroughputEndTestCb(sessionPacket_t *pSessionPacket);
static void APP_ThroughputAcceptCb(uint8_t *param);
#endif

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     void APP_InitUserSock(void)
\brief  This function is used to initialize user sockets

\param  [in]    void

\return         void
***************************************************************************************************/
void APP_InitUserSockets
(
    taskMsgQueue_t * pAppSockThreadMsgQueue
)
{
    uint32_t idx;

    if (NULL == pmAppSockThreadMsgQueue)
    {
        pmAppSockThreadMsgQueue = pAppSockThreadMsgQueue;

        /* Initialize the user sockets array */
        for(idx =0;idx<gUSER_MAX_SOCKETS;idx++)
        {
            gaUserSockID[idx].userSockFd = -1;
#if TCP_ENABLED
            gaUserSockID[idx].pRemoteAddr = NULL;
#endif
        }
    }
}

/*!*************************************************************************************************
\fn     void APP_SocketClientRxCallback(uint8_t *param)
\brief  This function is called in an endless loop.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
void APP_SocketClientRxCallback(uint8_t *param)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)param;
    sockaddrIn6_t *pRemAddr = (sockaddrIn6_t*)(&pSessionPacket->remAddr);
    char addrStr[INET6_ADDRSTRLEN];

    shell_write("\r");
    shell_writeN((char *)pSessionPacket->pData, (uint16_t)pSessionPacket->dataLen);
    ntop(AF_INET6, &pRemAddr->sin6_addr, addrStr, INET6_ADDRSTRLEN);
    shell_printf("\tFrom IPv6 Address: %s\n\r", addrStr);
    shell_refresh();

    MEM_BufferFree(pSessionPacket->pData);
    MEM_BufferFree(pSessionPacket);
}

/*!*************************************************************************************************
\fn     bool_t App_SocketSendAsync(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for sending a generic Socket application request to App socket utils.

\param [in]   pAppSockCmdParams   Socket application parameters.

\retval       bool_t    Return status. TRUE if the message was sent successfully, FALSE if not.
***************************************************************************************************/
bool_t App_SocketSendAsync
(
    appSockCmdParams_t* pAppSockCmdParams
)
{
    /* Send Message to task */
    return NWKU_SendMsg(App_SocketHandleSendAsync, pAppSockCmdParams, pmAppSockThreadMsgQueue);
}

/*==================================================================================================
Private functions
==================================================================================================*/

/*!*************************************************************************************************
\private
\fn     void App_SocketHandleSendAsync(uint8_t *pAppSockCmdParams)
\brief  This function is called for sending a generic Socket Shell request to App module.

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketHandleSendAsync
(
    uint8_t *pAppSockCmdParams
)
{
   appSockCmdParams_t* pSockCmdParams = (appSockCmdParams_t*)pAppSockCmdParams;

    switch(pSockCmdParams->appSocketsCmd)
    {
        case gSockOpenUdp:
        case gSockOpenTcp:
            App_SocketOpenBind(pSockCmdParams);
            break;
        case gSockConnect:
            App_SocketConnect(pSockCmdParams);
            break;
        case gSockClose:
            App_SocketClose(pSockCmdParams);
            break;
        case gSockSend:
            App_SocketSend(pSockCmdParams);
            break;

#if TCP_ENABLED
        case gSockAccept:
            App_SocketAccept(pSockCmdParams);
            break;
        case gSockShutdown:
            App_SocketShutdown(pSockCmdParams);
            break;
#endif

#if SOCK_DEMO_THROUGHPUT            
        case gSockBenchRxTcp:
        case gSockBenchRxUdp:
            APP_ThroughputBenchRx(pSockCmdParams);
            break;

        case gSockBenchTxTcp:
        case gSockBenchTxUdp:
           break;
#endif
           
        default:
            shell_write("Invalid command");
            break;
    }

    MEM_BufferFree(pSockCmdParams->pData);
    MEM_BufferFree(pSockCmdParams);
    shell_refresh();
}

/*!*************************************************************************************************
\private
\fn     void App_SocketOpenBind(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for sending a generic Socket Shell request to App module.

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketOpenBind
(
    appSockCmdParams_t* pSockCmdParams
)
{
    int32_t sockFd = - 1;
    uint32_t iSock;
    int32_t result = -1;

    sockaddrStorage_t localInfo = {0};
    shell_write("Opening Socket... ");

    NWKU_SetSockAddrInfo(&localInfo, &pSockCmdParams->sin_addr,(uint16_t)pSockCmdParams->ipVersion,
                         pSockCmdParams->sin_port, gNoIPv6FlowInfo_c, gIpIfSlp0_c);

    /* Create socket */
    if(gSockUdp == pSockCmdParams->appSocketsTrans)
    {
        sockFd = socket((uint8_t)pSockCmdParams->ipVersion, SOCK_DGRAM, IPPROTO_UDP);
    }
    if(gSockTcp == pSockCmdParams->appSocketsTrans)
    {
        sockFd = socket((uint8_t)pSockCmdParams->ipVersion, SOCK_STREAM, IPPROTO_TCP);
    }

    if(sockFd != -1)
    {
        /* Add socket to the list */
        for(iSock = 0; iSock < gUSER_MAX_SOCKETS; iSock++)
        {
            if(-1 == gaUserSockID[iSock].userSockFd)
            {
                gaUserSockID[iSock].userSockFd = sockFd;
                gaUserSockID[iSock].index = iSock;
                gaUserSockID[iSock].sockType = pSockCmdParams->appSocketsTrans;
                result = 0;
                break;
            }
        }

        if(0 == result)
        {
            uint32_t bindErr;

#if SOCK_DEMO_BLOCKING_MODE
            /* Enable blocking functionality on opened socket */
            bool_t bBlockSocket = TRUE;
            setsockopt(sockFd, SOL_SOCKET, SO_SOCKETBLOCK, &bBlockSocket, sizeof(bBlockSocket));
#endif
            /* Bind socket to local information */
            bindErr = bind(sockFd, &localInfo, sizeof(sockaddrStorage_t));

            switch(bindErr)
            {
                case 0:

                    /* Register socket for receive */
                    Session_RegisterCb(sockFd, APP_SocketClientRxCallback, pmAppSockThreadMsgQueue,
                                       mSessCbData_c);

                    shell_write("OK");
                    SHELL_NEWLINE();
                    shell_write("Socket id is: ");
                    shell_writeDec(iSock);

                    break;
                default:
                    (void)closesock(sockFd);
                    gaUserSockID[iSock].userSockFd = -1;

                    shell_write("ERROR\n\rBind went wrong");
                    break;
            }
         } /* Socket was successfully added */
         else
         {
            (void)closesock(sockFd);
            shell_write("ERROR\n\rAdding socket to the users list failed");
         }
     } /* socket was created successfully */
     else
     {
        shell_write("ERROR\n\rCreating socket failed");
     }
}

/*!*************************************************************************************************
\private
\fn     void App_SocketClose(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for closing an Socket.

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketClose(appSockCmdParams_t* pSockCmdParams)
{

    uint32_t sock32 = pSockCmdParams->sock32;
    int32_t result = -1;


    if(sock32 < gUSER_MAX_SOCKETS)
    {
        /* Close socket */
        result = closesock(gaUserSockID[sock32].userSockFd);

        /* Clear any allocated memory for this user socket(if exist) */
#if TCP_ENABLED
        if(gaUserSockID[sock32].pRemoteAddr)
        {
            MEM_BufferFree(gaUserSockID[sock32].pRemoteAddr);
            gaUserSockID[sock32].pRemoteAddr = NULL;
        }
#endif
        /* Remove socket from list */
        gaUserSockID[sock32].userSockFd = -1;

        if(0 == result)
        {
            shell_write("Socket ");
            shell_writeDec(sock32);
            shell_write(" was closed");
        }
        else
        {
            shell_write("Wrong socket ID");
        }
    }
}

/*!*************************************************************************************************
\private
\fn     void App_SocketSend(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for closing an Socket.

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketSend(appSockCmdParams_t* pSockCmdParams)
{
    uint32_t sock32 = pSockCmdParams->sock32;

    if((sock32 < gUSER_MAX_SOCKETS) && (gaUserSockID[sock32].userSockFd != -1))
    {
        (void)send(gaUserSockID[sock32].userSockFd, (uint8_t*)pSockCmdParams->pData,
                   pSockCmdParams->dataLen, 0);
        shell_write("Socket Data Sent");
    }
    else
    {
        shell_write("Socket was not previously opened");
    }
}

/*!*************************************************************************************************
\private
\fn     void App_SocketConnect(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for Socket connect.

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketConnect(appSockCmdParams_t* pSockCmdParams)
{
    uint32_t sockIndex = pSockCmdParams->sock32;

    if(pSockCmdParams->sock32 < gUSER_MAX_SOCKETS)
    {
        gaUserSockID[sockIndex].pRemoteAddr = NWKU_MEM_BufferAlloc(sizeof(sockaddrStorage_t));
        if (NULL != gaUserSockID[sockIndex].pRemoteAddr)
        {
            if (-1 != gaUserSockID[sockIndex].userSockFd)
            {
                /* Set remote information for easier send */
                NWKU_SetSockAddrInfo(gaUserSockID[sockIndex].pRemoteAddr, &pSockCmdParams->sin_addr,
                                    (uint16_t)pSockCmdParams->ipVersion,pSockCmdParams->sin_port,
                                     gNoIPv6FlowInfo_c, gIpIfSlp0_c);

                if (SOCK_DGRAM == gaUserSockID[sockIndex].sockType)
                {
                    (void)connect(gaUserSockID[sockIndex].userSockFd, gaUserSockID[sockIndex].pRemoteAddr,
                                  sizeof(sockaddrStorage_t));
                    shell_write("Socket is configured with remote peer information");
                }
#if TCP_ENABLED
                else
                {
                    (void)NWKU_SendMsg(APP_SockTcpConn, &gaUserSockID[sockIndex], pmAppSockThreadMsgQueue);
                }
#endif
            }
            else
            {
                shell_write("Wrong socket ID");
            }
        }
        else
        {
            shell_write("Memory alloc error");
        }
    }
    else
    {
        shell_write("Wrong socket ID");
    }
}

#if TCP_ENABLED
/*!*************************************************************************************************
\private
\fn     void App_SocketAccept(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for Socket accept.

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketAccept(appSockCmdParams_t* pSockCmdParams)
{
    uint32_t sock32 = pSockCmdParams->sock32;

    if((sock32 < gUSER_MAX_SOCKETS) && (gaUserSockID[sock32].userSockFd != -1))
    {
        sockaddrStorage_t localAddr = {0};
        uint16_t localPort;
        socklen_t len = sizeof(sockaddrStorage_t);

        (void)getsockname(gaUserSockID[sock32].userSockFd, &localAddr, &len);
        if(localAddr.ss_family == AF_INET6)
        {
            localPort = ((sockaddrIn6_t*)&localAddr)->sin6_port;
        }
        else
        {
            localPort = ((sockaddrIn_t*)&localAddr)->sin_port;
        }

        shell_printf("Waiting for one connection on port %d\n\r", localPort);

        (void)NWKU_SendMsg(APP_SockTcpConn, &gaUserSockID[sock32], pmAppSockThreadMsgQueue);

    }
    else
    {
        shell_write("Wrong socket ID");
    }
}

/*!*************************************************************************************************
\private
\fn     void App_SocketShutdown(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for shuting down part of a TCP connection

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketShutdown(appSockCmdParams_t* pSockCmdParams)
{

    uint32_t sock32 = pSockCmdParams->sock32;
    int32_t result = -1;


    if(sock32 < gUSER_MAX_SOCKETS)
    {
        /* Close socket */
        result = shutdown(gaUserSockID[sock32].userSockFd, 0);

        if(0 == result)
        {
            shell_write("Socket ");
            shell_writeDec(sock32);
            shell_write(" has shutdown its connection with peer");
        }
        else
        {
            shell_write("Wrong socket ID");
        }
    }
}

/*!*************************************************************************************************
\private
\fn     void APP_SockTcpConn(void *pParam)
\brief  Sockets demo application function used for tcp sockets. This function is used for blocking
        calls like connect/accept, which runs in the main task.

\param  [in]    pParam  pointer to the user socket structure

\return         void
***************************************************************************************************/
static void APP_SockTcpConn
(
    void *pParam
)
{
    userSock_t *pUserSock = (userSock_t*)pParam;
    int32_t sockFd;
    int32_t result = -1;
    sockaddrStorage_t remoteInfo = {0};

    /* Remote address was set -> initiate a TCP Client connection */
    if(pUserSock->pRemoteAddr)
    {
        /* Wait until a server accepts our connection */
        (void)connect(pUserSock->userSockFd, pUserSock->pRemoteAddr, sizeof(sockaddrStorage_t));

#if SOCK_DEMO_BLOCKING_MODE
        shell_write("The socket was connected to the remote Server\n\r");
#else
        Session_RegisterCb(pUserSock->userSockFd, APP_SocketConnectCallback, pmAppSockThreadMsgQueue,
                           mSessCbEvent_c);
#endif

    }
    /* Remote address was not set -> initiate a TCP Server connection */
    else
    {
        uint32_t iSock;
        char addrString[INET6_ADDRSTRLEN];
        uint16_t port;

        /* Initiate a TCP Server instance */
        (void)listen(pUserSock->userSockFd, 1);
        sockFd = accept(pUserSock->userSockFd, &remoteInfo, sizeof(sockaddrStorage_t));

        /* Register socket for receive */
        Session_RegisterCb(sockFd, APP_SocketClientRxCallback, pmAppSockThreadMsgQueue, mSessCbData_c);

#if SOCK_DEMO_BLOCKING_MODE

        if(IP_IsAddrIPv6((ipAddr_t*)pUserSock->pRemoteAddr->ss_addr))
        {
            ntop(AF_INET6, remoteInfo.ss_addr, addrString, INET6_ADDRSTRLEN);
            port = ((sockaddrIn6_t*)&remoteInfo)->sin6_port;
        }
        else
        {
            ntop(AF_INET, &remoteInfo.ss_addr, addrString, INET6_ADDRSTRLEN);
            port = ((sockaddrIn6_t*)&remoteInfo)->sin6_port;
        }

        /* Inform about the accepted connection */
        shell_write("Accepted connection from ");
        shell_printf("%s:%d\n\r", addrString, port);
#else
        Session_RegisterCb(sockFd, APP_SocketAcceptCallback, pmAppSockThreadMsgQueue, mSessCbEvent_c);
#endif

        /* Add socket to the list */
        for(iSock= 0; iSock < gUSER_MAX_SOCKETS; iSock++)
        {
            if(gaUserSockID[iSock].userSockFd == -1)
            {
                gaUserSockID[iSock].userSockFd = sockFd;
                gaUserSockID[iSock].index = iSock;
                gaUserSockID[iSock].sockType = SOCK_STREAM;
                result = 0;
                break;
            }
        }

        if(0 == result)
        {
#if SOCK_DEMO_BLOCKING_MODE
            shell_printf("Socket %d was connected with the remote TCP Client\r\n", iSock);
#endif
        }
        else
        {
            /* Close the accepted socket because it couldn't be added to the user sockets list */
            (void)closesock(sockFd);
            shell_write("A new socket could not be allocated to accept the connection\n\r");
        }

    }
}
#if !SOCK_DEMO_BLOCKING_MODE
/*!*************************************************************************************************
\fn     void APP_SocketConnectCallback(uint8_t *param)
\brief  This function is called

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void APP_SocketConnectCallback(uint8_t *param)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)param;
    sockaddrIn6_t *pRemAddr = (sockaddrIn6_t*)(&pSessionPacket->remAddr);
    char addrStr[INET6_ADDRSTRLEN];
    uint32_t iSock;

    ntop(AF_INET6, &pRemAddr->sin6_addr, addrStr, INET6_ADDRSTRLEN);

    /* Find socket in the list */
    for(iSock= 0; iSock < gUSER_MAX_SOCKETS; iSock++)
    {
        if(gaUserSockID[iSock].userSockFd == pSessionPacket->sockFd)
        {
            break;
        }
    }

    switch (pSessionPacket->sessStatus)
    {
        case gSessSuccess_c:
            shell_printf("Socket %d is connected to the remote Server %s\n\r", iSock, addrStr);
            break;

        case gSessSockConnClosing_c:
            shell_printf("The connection has been closed by the remote Server %s\n\r", addrStr);
            break;

        case gSessSockConnReset_c:
            shell_printf("The connection has been reset by the remote Server %s\n\r", addrStr);
            break;

        default:
            shell_printf("Unknown error \n\r");
            break;
    }

    shell_refresh();

    MEM_BufferFree(pSessionPacket);
}

/*!*************************************************************************************************
\fn     void APP_SocketAcceptCallback(uint8_t *param)
\brief  This function is

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void APP_SocketAcceptCallback(uint8_t *param)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)param;
    sockaddrIn6_t *pRemAddr = (sockaddrIn6_t*)(&pSessionPacket->remAddr);
    char addrStr[INET6_ADDRSTRLEN];
    //uint16_t port;
    uint32_t iSock;

    if(IP_IsAddrIPv6((ipAddr_t*)pSessionPacket->remAddr.ss_addr))
    {
        ntop(AF_INET6, (ipAddr_t*)pSessionPacket->remAddr.ss_addr, addrStr, INET6_ADDRSTRLEN);
        //port = ((sockaddrIn6_t*)&pSessionPacket->remAddr)->sin6_port;
    }
    else
    {
        ntop(AF_INET, (ipAddr_t*)pSessionPacket->remAddr.ss_addr, addrStr, INET6_ADDRSTRLEN);
        //port = ((sockaddrIn6_t*)&pSessionPacket->remAddr)->sin6_port;
    }

    /* Find socket in the list */
    for(iSock= 0; iSock < gUSER_MAX_SOCKETS; iSock++)
    {
        if(gaUserSockID[iSock].userSockFd == pSessionPacket->sockFd)
        {
            break;
        }
    }

    switch (pSessionPacket->sessStatus)
    {
        case gSessSuccess_c:
            shell_printf("Socket %d is connected to the remote Server %s\n\r", iSock, addrStr);
            break;

        case gSessSockConnClosing_c:
            shell_printf("The connection has been closed by the remote Server %s\n\r", addrStr);
            break;

        case gSessSockConnReset_c:
            shell_printf("The connection has been reset by the remote Server %s\n\r", addrStr);
            break;

        default:
            shell_printf("Unknown errort \n\r");
            break;
    }

    MEM_BufferFree(pSessionPacket);
}
#endif /* SOCK_DEMO_BLOCKING_MODE */
#endif /* TCP_ENABLED */

#if SOCK_DEMO_THROUGHPUT
/*!*************************************************************************************************
\fn    void APP_ThroughputBenchRx(char Control[3])

\brief Function for throughput Transmit.

\param [in]   Control           Receive Type [TCP/UDP]
\param [in]   Delay             Delay between 1 packet Receive

\return     void
 ***************************************************************************************************/
static void APP_ThroughputBenchRx
(
    appSockCmdParams_t* pSockCmdParams
)
{
    int32_t retVal = -1;
    sockaddrStorage_t servAddrInfo;
#if TCP_ENABLED
    sockaddrStorage_t clientAddrInfo;
#endif

    shell_write("Throughput Rx Test Starts now!\n\r");

    NWKU_SetSockAddrInfo(&servAddrInfo, (ipAddr_t*)&in6addr_any,AF_INET6,APP_BENCH_PORT,gNoIPv6FlowInfo_c,
                         gIpIfEth0_c);

    /* Socket Creation */
    if(gSockTcp == pSockCmdParams->appSocketsTrans)
    {
        benchInfo.foreignSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    }
    else
    {
        benchInfo.foreignSocket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    }

    benchInfo.appSocketsTrans = pSockCmdParams->appSocketsTrans;

    /* Check status of Socket Creation */
    if (-1 == benchInfo.foreignSocket)
    {
       /* Free Memory Buffer */
        shell_write ("[FAIL] Socket creation\n\r");
        return;
    }

    shell_write ("[SUCCESS] Socket creation\n\r");

    /* Socket Binding */
    retVal = bind(benchInfo.foreignSocket, &servAddrInfo, sizeof(sockaddrStorage_t));

    if (-1 == retVal)
    {
        shell_write ("[FAIL] bind\n\r");
        closesock(benchInfo.foreignSocket);
        return;
    }
    shell_write ("[SUCCESS] bind\n\r");

    if(gSockTcp == pSockCmdParams->appSocketsTrans)
    {

#if TCP_ENABLED
        retVal = listen(benchInfo.foreignSocket, 5);

        if(retVal == -1)
        {
             shell_write ("[FAIL] listen\n\r");
             closesock(benchInfo.foreignSocket);
             return;
        }

        shell_write ("[SUCCESS] listen\n\r");

        benchInfo.clientSocket = accept(benchInfo.foreignSocket, &clientAddrInfo, sizeof(sockaddrStorage_t));

        if (-1 == benchInfo.clientSocket)
        {
            shell_write ("[FAIL] accept\n\r");
            closesock(benchInfo.foreignSocket);
            return;
        }

        shell_printf ("[SUCCESS] accept \n\r");
        shell_refresh();
#endif
    }

    /* reset counter */
    benchInfo.cbytesRecv = 0;

    if (gSockTcp == benchInfo.appSocketsTrans)
    {
        Session_RegisterCb(benchInfo.clientSocket, App_ThroughputRxCallback, pmAppSockThreadMsgQueue,
                           mSessCbData_c);
        Session_RegisterCb(benchInfo.clientSocket, APP_ThroughputAcceptCb, pmAppSockThreadMsgQueue,
                           mSessCbEvent_c);
    }
    else
    {
        Session_RegisterCb(benchInfo.foreignSocket, App_ThroughputRxCallback, pmAppSockThreadMsgQueue,
                           mSessCbData_c);
    }
}

/*!*************************************************************************************************
\fn     void App_ThroughputRxCallback(uint8_t *pPacket)
\brief  This callback function is used to receive data from TCP/UDP client [F-Bench] in a non-block manner
***************************************************************************************************/
static void App_ThroughputRxCallback
(
    uint8_t *pPacket
)
{
    /* Process data in pSessionPacket->pData */
    sessionPacket_t *pSessionPacket = (sessionPacket_t *)pPacket;

    if (pSessionPacket->dataLen > 0)
    {
        if (gSockUdp == benchInfo.appSocketsTrans)
        {
            if(pSessionPacket->dataLen > APP_BENCH_END_BUFFER_LENGTH)
            {
                if (APP_BENCH_START_BUFFER_LENGTH == pSessionPacket->dataLen)
                {
                    /* Record Start Time */
                    benchInfo.firstTime = TMR_GetTimestamp();
                }
                else
                {
                    /* Sum of received bytes */
                    benchInfo.cbytesRecv += pSessionPacket->dataLen;
                }
            }
            else if(APP_BENCH_END_BUFFER_LENGTH == pSessionPacket->dataLen)
            {
                App_ThroughputEndTestCb(pSessionPacket);
            }
        }
        else
        {
            /* for tcp just record the number of bytes received */
            benchInfo.cbytesRecv += pSessionPacket->dataLen;
        }
    }
    else
    {
        shell_write ("NIL\n\r");
    }

    /* must free the packets after processing */
    MEM_BufferFree(pSessionPacket->pData);
    MEM_BufferFree(pSessionPacket);
}

/*!*************************************************************************************************
\fn     void APP_ThroughputAcceptCb(uint8_t *param)
\brief  This function is

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void APP_ThroughputAcceptCb
(
    uint8_t *param
)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)param;

    if (FALSE == benchInfo.bTestRunning)
    {
        /* Record Start Time */
        benchInfo.firstTime = TMR_GetTimestamp();
        benchInfo.bTestRunning = TRUE;
    }
    else
    {
        /* test has finished */
        App_ThroughputEndTestCb(pSessionPacket);
    }

    MEM_BufferFree(pSessionPacket);
}

/*!*************************************************************************************************
\fn     void App_ThroughputEndTestCb(sessionPacket_t *pSessionPacket)
\brief
***************************************************************************************************/
static void App_ThroughputEndTestCb
(
    sessionPacket_t *pSessionPacket
)
{
    uint64_t tempTimestamp;
    uint64_t tempTimestampSec;
    uint64_t bitsPerSec;

    /* test has complited */
    benchInfo.bTestRunning = FALSE;

    /* Record Close Time */
    benchInfo.lastTime = TMR_GetTimestamp();

    /* Calculation of Throughput */
    tempTimestamp = benchInfo.lastTime - benchInfo.firstTime;
    /* Convert to miliseconds and seconds */
    tempTimestamp = tempTimestamp / 1000;
    tempTimestampSec = tempTimestamp / 1000;

    bitsPerSec = (benchInfo.cbytesRecv << 3) / tempTimestampSec;

    shell_printf("%d bytes received in %d.%03d seconds = %d.%d Kbits/sec \n\r",
                 (uint32_t)benchInfo.cbytesRecv, (uint32_t)tempTimestampSec, (uint32_t)(tempTimestamp % 1000),
                 (uint32_t)(bitsPerSec /1024), (uint32_t)(bitsPerSec % 1024));

#if 0
    /* End of Receiving */
    uint32_t ackBytesReceived = htonl(benchInfo.cbytesRecv);

    /* Send acknoledgement */
    if (gSockTcp == benchInfo.appSocketsTrans)
    {
        send(benchInfo.clientSocket,(void*)&ackBytesReceived, sizeof(ackBytesReceived),0);
    }
    else
    {
        sendto(pSessionPacket->sockFd, (void*)&ackBytesReceived, sizeof(ackBytesReceived), 0,
               &pSessionPacket->remAddr, sizeof(sockaddrStorage_t));
    }
#endif

    /* reset counter */
    benchInfo.cbytesRecv = 0;

    if (gSockTcp == benchInfo.appSocketsTrans)
    {
        /* remove session callbacks for TCP connection */
        Session_UnRegisterCb(benchInfo.clientSocket);

        /* shutdown client socket (TCP - Accept) if connection was not reset */
        if (gSessSockConnClosing_c == pSessionPacket->sessStatus)
        {
            shutdown(benchInfo.clientSocket, 0);
        }

        closesock(benchInfo.clientSocket);
    }
    else
    {
        /* remove session callback for UDP connection */
        Session_UnRegisterCb(benchInfo.foreignSocket);
    }

    /* closesock main socket */
    if(!closesock(benchInfo.foreignSocket))
    {
        shell_write ("[SUCCESS] Socket Closed\n\r");
    }
    else
    {
        shell_write ("[FAILURE] Socket Closed\n\r");
    }

    shell_printf ("Throughput Test Finished!\n\r");
    shell_refresh();

}
#endif /* SOCK_DEMO_THROUGHPUT */

/*==================================================================================================
Private debug functions
==================================================================================================*/

