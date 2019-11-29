/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _TCP_CFG_H
#define _TCP_CFG_H
/*!=================================================================================================
\file       tcp_cfg.h
\brief      This is a header file for the TCP module. It contains macros used to configure the
            TCP module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/
/*! Maximum number of TCP connections */
#ifndef MAX_TCP_CONNECTIONS
    #define MAX_TCP_CONNECTIONS         7
#endif

#ifndef TCP_MIN_PORT_NUMBER
    #define TCP_MIN_PORT_NUMBER      (1024U) /*!< minimum port number available for users(1024) */
#endif

#ifndef TCP_MAX_PORT_NUMBER
    #define TCP_MAX_PORT_NUMBER      (4096U) /*!< maximum port number available for users(49151) */
#endif

/* TCP configurations */
#ifndef TCP_DEFAULT_HOP_LIMIT
#   define TCP_DEFAULT_HOP_LIMIT     (128)   /*!< Default hop limit for IPv6 or TTL for IPv4 */
#endif

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
#endif  /* _TCP_CFG_H */
