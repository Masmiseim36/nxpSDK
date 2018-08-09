/*! *********************************************************************************
 * \defgroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* \brief 	This is a public source file for the Sockets application.
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

/* General Includes */
#include "EmbeddedTypes.h"
#include <string.h>
#include <stdio.h>

#include "shell.h"
#include "FunctionLib.h"

#include "network_utils.h"

#include "app_stack_config.h"
#include "app_sockets.h"

/*************************************************************************************
**************************************************************************************
* Private macros
**************************************************************************************
*************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void APP_InitSocketServer(void);
static void APP_SocketClientRxCallback(void *param);
static void App_SocketHandleSendAsync(void* pAppSockCmdParams);

static void App_SocketOpenBindUdp(appSockCmdParams_t* pSockCmdParams);
static void App_SocketClose(appSockCmdParams_t* pSockCmdParams);
static void App_SocketSend(appSockCmdParams_t* pSockCmdParams);

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static taskMsgQueue_t * pmAppSockThreadMsgQueue = NULL;
userSock_t      gaUserSockID[gUSER_MAX_SOCKETS]; /* User socket list */

/* Server sockets */
static int32_t mSockfd = -1;


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
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
            gaUserSockID[idx].userSockFd = gBsdsSockInvalid_c;
            gaUserSockID[idx].timerID = gTmrInvalidTimerID_c;
        }
        APP_InitSocketServer();
    }
}
/*!************************************************************************************************
*
\private
\fn     void APP_InitSocketServer(void)
\brief  This function is used to initialize the socket server demo.

\param  [in]    void

\return         void
***************************************************************************************************/
static void APP_InitSocketServer(void)
{
    /* Socket storage information used for RX */
    sockaddrIn6_t mSsRx = {{{0}}};
    
    if(mSockfd != -1)
    {
        /* socket already initialised */
        return;
    }

    /* Create a socket for global IP address */
    /* Set local information */

    mSsRx.sin6_family = AF_INET6;
    mSsRx.sin6_port = UDP_PORT;
    IP_AddrCopy(&mSsRx.sin6_addr, &in6addr_any);

    /* Create socket */
    mSockfd = socket(mSsRx.sin6_family, SOCK_DGRAM, IPPROTO_UDP);

    /* Bind socket to local information */
    bind(mSockfd, (sockaddrStorage_t*)&mSsRx, sizeof(sockaddrStorage_t));

    /* Initializes the Session task*/
    Session_Init();

    Session_RegisterCb(mSockfd, APP_SocketClientRxCallback, pmAppSockThreadMsgQueue);

}
/*!************************************************************************************************
*
\private
\fn     void APP_SocketClientRxCallback(void *param)
\brief  This function is called in an endless loop.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void APP_SocketClientRxCallback(void *param)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)param;
    sockaddrIn6_t *pRemAddr = (sockaddrIn6_t*)(&pSessionPacket->remAddr);
    char addrStr[INET6_ADDRSTRLEN];

    shell_write("\r\n");
    shell_write((char *)pSessionPacket->pData);
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

/*!*************************************************************************************************
\fn     void App_SocketShellResume(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called from shell to resume a socket post or poll.

\param [in]   none

\retval       none
***************************************************************************************************/
void App_ShellSocketResume
(
    void
)
{
    uint32_t iSock;

    /* Stop and free all timers */
    for(iSock =0; iSock < gUSER_MAX_SOCKETS; iSock++)
    {
        if((gaUserSockID[iSock].userSockFd) && (gaUserSockID[iSock].timerID != gTmrInvalidTimerID_c))
        {
            TMR_FreeTimer(gaUserSockID[iSock].timerID);
            gaUserSockID[iSock].timerID = gTmrInvalidTimerID_c;
        }
    }

}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*!*************************************************************************************************
\private
\fn     void App_SocketHandleSendAsync(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for sending a generic Socket Shell request to App module.

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketHandleSendAsync
(
    void* pAppSockCmdParams
)
{
   appSockCmdParams_t* pSockCmdParams =  pAppSockCmdParams;

    switch(pSockCmdParams->appSocketsCmd)
    {
        case gSockOpenUdp:
            App_SocketOpenBindUdp(pSockCmdParams);
            break;
        case gSockClose:
            App_SocketClose(pSockCmdParams);
            break;
        case gSockSend:
            App_SocketSend(pSockCmdParams);
            break;
        default:
            break;
    }

    MEM_BufferFree(pSockCmdParams->pData);
    MEM_BufferFree(pSockCmdParams);
    shell_refresh();
}

/*!*************************************************************************************************
\private
\fn     void App_SocketUdpOpen(appSockCmdParams_t* pAppSockCmdParams)
\brief  This function is called for sending a generic Socket Shell request to App module.

\param [in]   pAppSockCmdParams   Socket Shell request parameters.

\retval       none
***************************************************************************************************/
static void App_SocketOpenBindUdp(appSockCmdParams_t* pSockCmdParams)
{
    int32_t sockFd;
    uint32_t iSock;
    uint32_t result = gBsdsSockListFull_c;

    sockaddrIn6_t   localInfo;
    sockaddrIn6_t   remoteInfo;
    
    shell_write("Opening Socket... ");

    if(gSockUdp == pSockCmdParams->appSocketsTrans)
    {

        if(pSockCmdParams->ipVersion == AF_INET6)
        {
            localInfo.sin6_family = pSockCmdParams->ipVersion;
            localInfo.sin6_port = 0;
            IP_AddrCopy(&localInfo.sin6_addr, &in6addr_any);

            /* Create socket */
            sockFd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
        }
        else
        {
            ((sockaddrIn_t*)&localInfo)->sin_family = pSockCmdParams->ipVersion;
            ((sockaddrIn_t*)&localInfo)->sin_port = 0;
             IP_AddrCopy(&((sockaddrIn_t*)&localInfo)->sin_addr, &in6addr_any);

             /* Create socket */
             sockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        }

        if(sockFd)
        {
            /* Add socket to the list */
            for(iSock=0;iSock<gUSER_MAX_SOCKETS;iSock++)
            {
                if(gaUserSockID[iSock].userSockFd == gBsdsSockInvalid_c)
                {
                    gaUserSockID[iSock].userSockFd = sockFd;
                    gaUserSockID[iSock].index = iSock;
                    result = gBsdsSockAdded_c;
                    break;
                }
            }
            if(result == gBsdsSockAdded_c)
            {
                uint32_t bindErr;

                /* Bind socket to local information */
                bindErr = bind(sockFd, (sockaddrStorage_t*)&localInfo, sizeof(sockaddrStorage_t));

                switch(bindErr)
                {
                    case gBsdsSockSuccess_c:
                        /* Set remote information for easier send */
                        if(pSockCmdParams->ipVersion == AF_INET6)
                        {
                            remoteInfo.sin6_port = pSockCmdParams->sin6_port;
                            remoteInfo.sin6_addr = pSockCmdParams->sin6_addr;
                        }
                        else
                        {
                            ((sockaddrIn_t*)&remoteInfo)->sin_port = pSockCmdParams->sin_port;
                            ((sockaddrIn_t*)&remoteInfo)->sin_addr = pSockCmdParams->sin_addr;
                        }
                        /* Set Remote information */
                        connect(sockFd, (sockaddrStorage_t*)&remoteInfo, sizeof(sockaddrStorage_t));

                        shell_write("OK");
                        SHELL_NEWLINE();
                        shell_write("Socket id is: ");
                        shell_writeDec(iSock);

                        break;
                    default:
                        shell_write("ERROR\n\rBind went wrong");
                        break;
                }
             } /* Socket was successfully added */
             else
             {
                shell_write("ERROR\n\rAdding socket to the users list failed");
             }
         } /* socket was created successfully */
         else
         {
            shell_write("ERROR\n\rCreating socket failed");
         }
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
    uint32_t result = gBsdsSockError_g;


    if(sock32 < gUSER_MAX_SOCKETS)
    {
        /* Close socket */
        result = shutdown(gaUserSockID[sock32].userSockFd, 0);
    
        /* Remove socket from list */
        gaUserSockID[sock32].userSockFd = gBsdsSockInvalid_c;

        if(result == gBsdsSockSuccess_c)
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

    if((sock32 < gUSER_MAX_SOCKETS) && (gaUserSockID[sock32].userSockFd != gBsdsSockInvalid_c))
    {
        send(gaUserSockID[sock32].userSockFd,
                pSockCmdParams->pData,
                pSockCmdParams->dataLen,
                0);
        shell_write("Socket Data Sent");
    }
    else
    {
        shell_write("Socket was not previously opened");
    }

}
