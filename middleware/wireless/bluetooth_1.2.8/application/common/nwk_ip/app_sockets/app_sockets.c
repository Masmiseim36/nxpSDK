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
#include "app_sockets.h"

/*==================================================================================================
Private macros
==================================================================================================*/



/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/*!< Benchmark information */
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
                         pSockCmdParams->sin_port, gNoIPv6FlowInfo_c, gIpIfUndef_c);

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
