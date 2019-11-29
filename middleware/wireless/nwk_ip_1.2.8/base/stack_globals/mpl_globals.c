/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       mpl_globals.c
\brief      This is the source file that contains parameters for the mpl module that can 
            be configured by the application
*/
 
/*==================================================================================================
Include Files
==================================================================================================*/
#include "mpl.h"
#include "mpl_cfg.h"
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

#if TRICKLE_ENABLED && MPL_ENABLED
/*! MPL instance table */
mplInstanceSetEntry_t*  mMplInstanceSet[MPL_INSTANCE_SET_SIZE]  = {NULL};
/*! MPL instance table size */
const uint32_t          mMplInstanceSetSize                     = MPL_INSTANCE_SET_SIZE;

/*! MPL seed table */
mplSeedSetEntry_t       mMplSeedSet[MPL_SEED_SET_SIZE];
/*! MPL seed table size */
const uint32_t          mMplSeedSetSize                         = MPL_SEED_SET_SIZE;

/*! MPL buffered message list maximum size */
const uint32_t          mMplBufferedMessageSetSize              = MPL_BUFFERED_MESSAGE_SET_SIZE;

/*! Sequence number for the originated multicast packets sent using MPL */
uint8_t                 mMplSequenceNumber  = 0;

const uint8_t           mMplNvmSaveThreshold = 100;
#endif

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

