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
#ifndef _APP_ECHO_UDP_H
#define _APP_ECHO_UDP_H
/*!=================================================================================================
\file       app_echo_udp.h
\brief      This is a header file for the echo protocol application.

==================================================================================================*/

/*==================================================================================================
 Include Files
 ==================================================================================================*/

#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
 Public macros
 ==================================================================================================*/

/*==================================================================================================
 Public type definitions
 ==================================================================================================*/

/*==================================================================================================
 Public global variables declarations
 ==================================================================================================*/

/*==================================================================================================
 Public function prototypes
 ==================================================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif


#if UDP_ECHO_PROTOCOL
/*!*************************************************************************************************
\fn     void ECHO_ProtocolInit(void)
\brief  This function is used to initialize the server for echo protocol.

\param  [in]    pMsgQueue     task message queue
\return         void
***************************************************************************************************/
void ECHO_ProtocolInit(taskMsgQueue_t * pMsgQueue);

/*!*************************************************************************************************
\fn     void ECHO_UdpServerService(void *pInData)
\brief  This function is used to initialize the server for echo protocol.

\param  [in]    pInData     pointer to the received packet
***************************************************************************************************/
void ECHO_UdpServerService(void *pInData);


void ECHO_TcpServerService(void *pInData);

/*!*************************************************************************************************
\fn     int8_t SHELL_EchoUdp(uint8_t argc, char *argv[])
\brief  This function is used for "echoudp" shell command.

\param  [in]    argc    number of arguments the command was called with
\param  [in]    argv    pointer to a list of pointers to the arguments

\return    void
***************************************************************************************************/
int8_t ECHO_ShellUdp(uint8_t argc, char *argv[]);

#if THREAD_USE_SHELL
/*!*************************************************************************************************
\fn     void ECHO_resumeShell(void* pInData)
\brief  This function is used to resume shell if no reply received after echo udp message is sent.

\param  [in]    pInData    not used

\return    void
***************************************************************************************************/
void ECHO_resumeShell(void* pInData);
#endif
/*!*************************************************************************************************
\fn     void ECHO_service(void* pInData)
\brief  This function is used to receive message from the server and resume shell.

\param  [in]    pInData    socket used for receiving

\return    void
***************************************************************************************************/
void ECHO_service(void *pInData);

#if THREAD_USE_THCI
/*!*************************************************************************************************
\fn     void ECHO_ThciUdp(uint32_t payloadSize, uint16_t timeout, uint8_t count, ipAddr_t* pDestIpAddr,
                          ipAddr_t* pSourceIpAddr)
\brief  This function is used to send an echo UDP message.

\param  [in]    payloadSize    size of message payload
\param  [in]    timeout        timeout 
\param  [in]    security       if the message is secured at MAC layer 
\param  [in]    pDestIpAddr    pointer to the destination ip address 
\param  [in]    pSourceIpAddr  pointer to the source ip address

\return    void
***************************************************************************************************/
void ECHO_ThciUdp(uint32_t payloadSize, uint16_t timeout, uint8_t count, ipAddr_t* pDestIpAddr, ipAddr_t* pSourceIpAddr);
#endif


#endif

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _APP_ECHO_UDP_H */
