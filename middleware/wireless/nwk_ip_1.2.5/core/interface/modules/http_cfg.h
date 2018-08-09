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


#ifndef  _HTTP_CFG_H
#define  _HTTP_CFG_H


/*!=================================================================================================
\file       http_cfg.h
\brief      This is a configuration header file for the HTTP module.

\details    This file contains the folowing configuration options:
            HTTP_TASK_MSG_QUEUE_SIZE         configurable value
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/

#ifndef HTTP_TASK_MSG_QUEUE_SIZE
   #define HTTP_TASK_MSG_QUEUE_SIZE 20U
#endif

#ifndef HTTP_ACCEPT_TASK_MSG_QUEUE_SIZE
   #define HTTP_ACCEPT_TASK_MSG_QUEUE_SIZE 20U
#endif

#ifndef HTTP_TASK_STACK_SIZE
   #define HTTP_TASK_STACK_SIZE 896U
#endif

#ifndef HTTP_SERVER_CONNECTION_TABLE_SIZE
   #define HTTP_SERVER_CONNECTION_TABLE_SIZE 2U
#endif

#ifndef HTTP_QUERY_SIZE
   #define HTTP_QUERY_SIZE 5U
#endif

#ifndef HTTP_URI_SIZE
   #define HTTP_URI_SIZE 2U
#endif

#ifndef HTTP_DISABLE_PERSISTENT_CONNECTIONS
   #define HTTP_DISABLE_PERSISTENT_CONNECTIONS 1U
#endif

#ifndef HTTP_CLIENT_TIMEOUT_INTERVAL
    #define HTTP_CLIENT_TIMEOUT_INTERVAL 1000U /* ms */
#endif





/* Implemented methods number */
#define HTTP_METHODS_NB 6

/* Http versions */
#define gHttpStrVersionsNumber_c 3

#define HTTP_PROTOCOL_NB 4





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
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /*  _HTTP_CFG_H */
