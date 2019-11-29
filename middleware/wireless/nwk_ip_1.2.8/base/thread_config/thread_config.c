/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
