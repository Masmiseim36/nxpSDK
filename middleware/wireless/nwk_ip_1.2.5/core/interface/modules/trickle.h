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

#ifndef _TRICKLE_H_
#define _TRICKLE_H_

/*!=================================================================================================
\file       trickle.h
\brief      This is a header file for the trickle timer module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

/* Network Includes */
#include "network_utils.h"

/*! Inform the the trickle timer is enable or disabled */
#ifndef TRICKLE_ENABLED
  #define TRICKLE_ENABLED              0
#endif

#if TRICKLE_ENABLED

/*==================================================================================================
Public macros
==================================================================================================*/

#define TRICKlE_K_INFINITE 0x00

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! Prototype of the function called by Trickle timer to handle associated message */
typedef void (*trickleMsgHandler_t)(void* pMsg);

/*! Trickle timer instance configuration structure */
typedef struct trickleInstanceConfig_tag
{
    void*               pUpperLayerHandle;  /*!< Upper layer handle to identify the Trickle instance */
    trickleMsgHandler_t msgHandler;         /*!< Function to be called when Trickle timer expires */
    uint16_t            Imin;               /*!< Minimum interval size */
    uint16_t            Imax;               /*!< Maximum interval size */
    uint8_t             k;                  /*!< Redundancy constant */
    bool_t              useFullInterval;    /*!< use interval [0, I) or [I/2, I) as defined by RFC 6206*/
}trickleInstanceConfig_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void Trickle_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  Public interface function for Trickle module. This function initializes the Trickle module.

\param  [in]    pTaskMsgQueue               Pointer to the task message queue.
***************************************************************************************************/
void Trickle_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     nwkStatus_t* Trickle_CreateInstance(trickleInstanceConfig_t* pTrickleInstanceConfig)
\brief  Public interface function for Trickle module. This function creates a new Trickle timer
        instance.

\param  [in]    pTrickleInstanceConfig      Pointer to the configuration information.

\return         nwkStatus_t                 Returned status of the function.
***************************************************************************************************/
nwkStatus_t Trickle_CreateInstance(trickleInstanceConfig_t* pTrickleInstanceConfig);

/*!*************************************************************************************************
\fn     void* Trickle_Alloc(void* pUpperLayerHandle, void* pMsg)
\brief  Public interface function for Trickle module. This function allocates a trickle timer and
        returns a handle to it.

\param  [in]    pUpperLayerHandle           Upper layer handle to identify the Trickle instance.
\param  [in]    pMsg                        Message to be used as argument for function handler when
                                            Trickle timer expires.

\return         void*                       Handle to the allocated Trickle timer.
***************************************************************************************************/
void* Trickle_Alloc(void* pTrickleInstanceHandle, void* pMsg);

/*!*************************************************************************************************
\fn     void Trickle_Free(void** ppTrickleHandle)
\brief  Public interface function for Trickle module. This function frees a trickle timer and sets
        its handle to NULL.

\param  [in,out]    ppTrickleHandle         Address of the Trickle timer handle to be freed (is set
                                            to NULL by the function).

***************************************************************************************************/
void Trickle_Free(void** ppTrickleHandle);

/*!*************************************************************************************************
\fn     void Trickle_ConsistentRx(void* pTrickleHandle)
\brief  Public interface function for Trickle module. This function increments consistency counter
        for the Trickle timer.

\param  [in]    pTrickleHandle              Trickle timer handle for the consistent message.
***************************************************************************************************/
void Trickle_ConsistentRx(void* pTrickleHandle);

/*!*************************************************************************************************
\fn     void Trickle_InconsistentRx(void* pTrickleHandle)
\brief  Public interface function for Trickle module. This function resets the Trickle timer (due an
        inconsistent message).

\param  [in]    pTrickleHandle              Trickle timer handle for the inconsistent message.
***************************************************************************************************/
void Trickle_InconsistentRx(void* pTrickleHandle);

#ifdef __cplusplus
}
#endif


#endif /* TRICKLE_ENABLED */

/*================================================================================================*/

#endif /* _TRICKLE_H_ */
