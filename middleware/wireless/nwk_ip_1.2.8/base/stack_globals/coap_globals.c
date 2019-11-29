/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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

const uint32_t gCoapBlockSize = COAP_BLOCK_SIZE;
/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

