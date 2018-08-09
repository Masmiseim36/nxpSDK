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
#include "app_temp_sensor.h" 

#include "sockets.h"
#include "session.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#define UDP_PORT                        (1234)

#ifndef gUSER_MAX_SOCKETS
    #define gUSER_MAX_SOCKETS           (5U)
#endif


/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum {
    gSockOpenUdp,    
    gSockOpenTcp,
    gSockConnect,
    gSockAccept,
    gSockClose,
    gSockSend,
    gSockSelect,
    gSockPost
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
    #if TCP_ENABLED
        sockaddrStorage_t* pSockAddr;
    #endif
    uint32_t sock32;
    uint8_t* pData;
    uint8_t  dataLen;
    uint32_t timeMs;
    bool_t   bSelectedFlag;
}appSockCmdParams_t;

typedef struct userSock_tag
{
    int32_t userSockFd;                      /* Socket file descriptor */
    tmrTimerID_t timerID;                    /* ID for the timer used to poll on this socket */
    uint32_t index;                          /* Index of the file descriptor in the global file descriptors array */
#if TCP_ENABLED
    sockaddrStorage_t *pRemoteAddr;          /* Pointer to sockAddrStorage_t structure(only in tcp active mode) */
#endif
} userSock_t;

typedef struct pollInfo_tag
{
    uint8_t *pRequest;
    uint32_t delay;
    userSock_t *pSockList;
    tmrTimerID_t timerId;
    uint32_t currentSock;
    bool_t stop;            /*!< Used to force stop a poll and notify the timer callback to stop and
                                 free this structure */
}pollInfo_t;

typedef enum
{
    gSockDemoDevTypeConcentrator_c = 0x00U,  
    gSockDemoDevTypeNode_c = 0x01U
} sockDemoDevType_t;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern userSock_t       gaUserSockID[]; /* User socket list */
extern sockaddrIn6_t    ssTx;


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

/*!*************************************************************************************************
\fn     void App_SocketShellResume(appSockCmdParams_t* pAppSockCmdParams)                                         
\brief  This function is called from shell to resume a socket post or poll.

\param [in]   none

\retval       none
***************************************************************************************************/

void App_ShellSocketResume(void);
#ifdef __cplusplus
}
#endif
/*================================================================================================*/


#endif /* _APP_SOCKET_UTILS_H */
