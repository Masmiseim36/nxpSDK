/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*! *********************************************************************************
 * \defgroup SOCKETS  Network IP Sockets Interface
 * @{
 ***********************************************************************************/

#ifndef _SOCKETS_CFG_H
#define _SOCKETS_CFG_H

/*!
\file       sockets_cfg.h
\brief      This is a header file for the Sockets module. It contains the configuration file for the
            sockets implementation. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*! Maximum sockets available */
#ifndef BSDS_MAX_SOCKETS
    #define BSDS_MAX_SOCKETS            8
#endif

/*! Session Task Stack Size - in case session task is enabled*/
#ifndef SESSION_TASK_STACK_SIZE
    #define SESSION_TASK_STACK_SIZE     (1000U)
#endif
 
/*! Maximum number of socket file descriptors for
      select (only if supported by the library)*/
#ifndef BSDS_SELECT_MAX_FDS
    #define BSDS_SELECT_MAX_FDS         (20)   
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
#endif  /* _SOCKETS_CFG_H */
