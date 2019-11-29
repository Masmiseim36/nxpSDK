/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       mle_globals.c
\brief      This is the source file that contains parameters for the mle module that can 
            be configured by the application 
*/
     
/*==================================================================================================
Include Files
==================================================================================================*/

#include "app_stack_config.h"
#include "mle_security.h"

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

/* MLE key descriptor table */
mleKeyDescriptor_t  mMleKeyDescriptorTable[MLE_KEY_DESCRIPTOR_TABLE_SIZE]   = {0};
/* MLE key descriptor table size */
const uint32_t      mMleKeyDescriptorTableSize                              = MLE_KEY_DESCRIPTOR_TABLE_SIZE;
/* MLE incoming security frame counters table */
uint32_t            mMleIncomingSecFrameCntTable[MLE_KEY_DESCRIPTOR_TABLE_SIZE * THR_MAX_NEIGHBORS] = {0};


uint32_t            mMleOutgoingSecFrameCounter;
uint32_t            mMacOutgoingSecFrameCounter;
const uint32_t      mMleNvmSaveThreshold = 512;

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

