/*! *********************************************************************************
 * \defgroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file is the interface file for the echo protocol application
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _APP_ECHO_UDP_H
#define _APP_ECHO_UDP_H

#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

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
void ECHO_UdpServerService(uint8_t *pInData);

/*!*************************************************************************************************
\fn     int8_t SHELL_EchoUdp(uint8_t argc, char *argv[])
\brief  This function is used for "echoudp" shell command.

\param  [in]    argc    number of arguments the command was called with
\param  [in]    argv    pointer to a list of pointers to the arguments

\return    void
***************************************************************************************************/
int8_t ECHO_ShellUdp(uint8_t argc, char *argv[]);

#if SHELL_ENABLED
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
void ECHO_service(uint8_t *pInData);

#endif

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _APP_ECHO_UDP_H */
