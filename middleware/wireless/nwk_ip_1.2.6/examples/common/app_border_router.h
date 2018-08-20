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
 
#ifndef _APP_BORDER_ROUTER_H
#define _APP_BORDER_ROUTER_H
/*!=================================================================================================
\file       app_border_router.h
\brief      This is a header file for the echo protocol application.

==================================================================================================*/

/*==================================================================================================
 Include Files
 ==================================================================================================*/

#include "EmbeddedTypes.h"
#include "app_stack_config.h"
#include "thread_utils.h"

/*==================================================================================================
 Public macros
 ==================================================================================================*/

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
extern "C"
{
#endif

/*!*************************************************************************************************
\fn     BR_Start
\brief  This function will initiate the Ethernet or USB Ethernet or Seriat Tun components.

\param  [in] pTaskMsgQueue  task message queue
\param  [in] thrInstanceID  instance ID of the Thread interface

\return void
***************************************************************************************************/
void BR_Start(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID);

/*!*************************************************************************************************
\fn     BR_PrefixSetup
\brief  This is a public function used to configure the prefixes on the External IP Media Interface.
\return void
***************************************************************************************************/
void BR_PrefixSetup(instanceId_t thrInstanceID);

/*!*************************************************************************************************
\fn     BR_ThreadStartedCb
\brief  This function shall be called when the device becomes a leader and wants
        to propagate different IP prefixes (e.g. global on-mesh prefix, external prefix)

\param  [in] thrInstanceId  instance ID of the Thread interface

\return void
***************************************************************************************************/
void BR_ThreadStartedCb(instanceId_t thrInstanceID);



#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _APP_APP_BORDER_ROUTER_H */
