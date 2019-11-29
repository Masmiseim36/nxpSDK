/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
**  @addtogroup COAP
**  @{
*/

#ifndef  _COAP_CFG_H
#define  _COAP_CFG_H

/*!
\file       coap_cfg.h
\brief      This is a configuration header file for the CoAP module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
  MACROS THAT CAN BE SET AT COMPILE TIME (BY THE USER)
-------------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

/*!< Maximum number of callbacks */
#ifndef COAP_MAX_CALLBACKS
    #define COAP_MAX_CALLBACKS                  (60U)
#endif

/*!< Maximum number of callbacks registered for non-piggybacked responses */
#ifndef COAP_MAX_NON_PIGGYBACKED_RSP
    #define COAP_MAX_NON_PIGGYBACKED_RSP        (10U)
#endif

/*!< Maximum number of active CoAP sessions at a given moment, per one CoAP instance */
#ifndef COAP_MAX_SESSIONS
    #define COAP_MAX_SESSIONS                   (5U)
#endif

/*!< Maximum number of CoAP instances */
#ifndef COAP_MAX_INSTANCES
    #if THREAD_BORDER_ROUTER_CONFIG
        #define COAP_MAX_INSTANCES              (7U)
    #else
        #define COAP_MAX_INSTANCES              (6U)
    #endif
#endif

/*! Used for keeping track of duplicate CoAP messages */
#ifndef COAP_MAX_MSG_IDs
    #define COAP_MAX_MSG_IDs                    (20U)
#endif

/*! Maximum number of options included in one CoAP message.
   Here are not included URI-path options which MAY be added with COAP_SetUriPath() function */
#ifndef COAP_MAX_OPTIONS
    #define COAP_MAX_OPTIONS                    (10U)
#endif

/*! Token length used by default in CoAP messages. */
#ifndef COAP_TOKEN_LENGTH
    #define COAP_TOKEN_LENGTH                   (4U)
#endif

/*! The default block size used in block-wise transfer
    Possible values are 16, 32, 64, 128, 256, 512, 1024 */
#ifndef COAP_BLOCK_SIZE
  #define COAP_BLOCK_SIZE                       (64U)
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
#endif  /*  _COAP_CFG_H */
