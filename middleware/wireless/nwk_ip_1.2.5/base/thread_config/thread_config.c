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
\file       thread_config.c
\brief      This is a public source file for the thread_config module. It contains
            Thread configurations.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* FSL Framework */
/* 802.15.4 PHY-MAC */
/* IP Stack Lite */
/* Communication Interfaces */
/* Application */
#include "app_thread_config.h"

#if STACK_THREAD

#include "sixlowpan_fwd_thread.h"
#include "mac_filtering.h"
#include "thread_attributes.h"
#include "thread_meshcop.h"

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

CONST macFilteringNeighborData_t mThreadICanHearYouTbl[]    = {0, 0, 0};

CONST adpIb_t mThreadAdpIb                                  = {THR_ADP_IB};

const thrAttr_t mThrDefaultAttr                = {THR_DEFAULT_ATTR};
const thrStringAttr_t mThrStringAttr           = {THR_STRING_ATTR};
const thrActiveAttr_t mThrDefaultActiveAttr    = {THR_DEFAULT_ACTIVE_ATTR};

CONST stackConfig_t mThreadCfg                 = {THR_CONFIGURATION};

/* Active Stack Configuration */
CONST stackConfig_t* pStackCfg[] = {&mThreadCfg,
                                    NULL
                                   };

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Private debug functions
==================================================================================================*/

#endif /* STACK_THREAD */
