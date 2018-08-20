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

#ifndef _SESSION_H
#define _SESSION_H
/*!=================================================================================================
\file       session.h
\brief      This is a header file for the Session module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "network_utils.h"
#include "sockets.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*!< Structure used to keep information about a received packet */
typedef struct sessionPacket_tag
{
    int32_t sockFd;
    sockaddrStorage_t remAddr;
    sockaddrStorage_t localAddr;
    uint32_t dataLen;
    uint8_t *pData;
    recvOptions_t packetOpt;
} sessionPacket_t;

/*!< Structure used to keep information about a socket handler */
typedef struct sessEntity_tag
{
    int32_t sockFd;
    taskMsgQueue_t *pMsgQueue;
    nwkMsgHandler pHandler;
} sessEnt_t;

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
\fn    void Session_Init()
\brief Public interface function for Session module. This function initializes the Session task.
       This function must be called from the upper layer task directly.
***************************************************************************************************/
void Session_Init(void);

/*!*************************************************************************************************
\fn    void Session_RegisterCb(int32_t sockFd, nwkMsgHandler pCallback)
\brief Public interface function for Session module. This function registers a callback for the
       corresponding socket descriptor and update the select sockets read list.

\param [in]  sockFd     socket file descriptor
\param [in]  pCallback  pointer to the callback
\param [in]  pMsgQueue  pointer to task queue
***************************************************************************************************/
void Session_RegisterCb(int32_t sockFd, nwkMsgHandler pHandler, taskMsgQueue_t *pMsgQueue);

/*!*************************************************************************************************
\fn    void Session_UnRegisterCb(int32_t sockFd)
\brief Public interface function for Session module. This function unregisters a socket and its
       corresponding callback from the select list.

\param [in]  sockFd     socket file descriptor
***************************************************************************************************/
void Session_UnRegisterCb(int32_t sockFd);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _SESSION_H */
