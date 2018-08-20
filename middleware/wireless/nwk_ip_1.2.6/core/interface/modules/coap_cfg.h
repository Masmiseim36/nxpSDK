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

/*!< Maximum number of active CoAP sessions at a give moment, per one CoAP instance */
#ifndef COAP_MAX_SESSIONS
    #define COAP_MAX_SESSIONS                   (5U)
#endif

/*!< Maximum number of CoAP instances */
#ifndef COAP_MAX_INSTANCES
    #define COAP_MAX_INSTANCES                  (5U)
#endif

/*! Used for keeping track of duplicate CoAP messages */
#ifndef COAP_MAX_MSG_IDs
    #define COAP_MAX_MSG_IDs                    (20U)
#endif

/*! Maximum number of options included in one CoAP message. 
   Here are not included URI-path options which MAY be added with COAP_SetUriPath() function */
#ifndef COAP_MAX_OPTIONS
    #define COAP_MAX_OPTIONS                    (5U) 
#endif

/*! Token length used by default in CoAP messages. */
#ifndef COAP_TOKEN_LENGTH
    #define COAP_TOKEN_LENGTH                   (4U)
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
