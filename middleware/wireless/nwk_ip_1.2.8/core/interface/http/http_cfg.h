/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
#ifndef HTTP_ENABLED
    #define HTTP_ENABLED  0
#endif    

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

#if HTTP_ENABLED && !TCP_ENABLED
   #error "HTTP requires TCP and STREAM Socket enabled"
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
