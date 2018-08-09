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
\file       coap_globals.c
\brief      This is the source file that contains parameters for the COAP module that can
            be configured by the application 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "coap.h"
#include "coap_cfg.h"

/*==================================================================================================
Private macros
==================================================================================================*/

/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*! CoAP Sessions table */
const uint32_t gCoapMaxSessions = COAP_MAX_SESSIONS;

/*! Table of resources and their callbacks */
coapCallbackStruct_t gCoapResourceCbs[COAP_MAX_CALLBACKS];
const uint32_t gCoapMaxCallbacks = COAP_MAX_CALLBACKS;

/*! Table of tokens and their callbacks, for non-piggybacked responses */
coapTokenCbStruct_t gCoapTokenCbs[COAP_MAX_NON_PIGGYBACKED_RSP];
const uint32_t gCoapMaxNonPiggybackedResponses = COAP_MAX_NON_PIGGYBACKED_RSP;


coapInstance_t gCoapInstances[COAP_MAX_INSTANCES];
const uint32_t gCoapMaxInstances = COAP_MAX_INSTANCES;

/*! Keep track of the last  msg IDs to avoid duplicates */
uint16_t gaCoapLastMsgIds[COAP_MAX_MSG_IDs] = {0};
const uint32_t  gCoapMaxMsgIds = COAP_MAX_MSG_IDs;

const uint32_t gCoapMaxOptions = COAP_MAX_OPTIONS;
const uint32_t gCoapTokenLen = COAP_TOKEN_LENGTH;

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

