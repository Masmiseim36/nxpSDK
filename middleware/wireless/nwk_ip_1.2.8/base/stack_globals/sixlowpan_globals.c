/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       sixlowpan_globals.c
\brief      This is the source file that contains parameters for the 6LOWPAN module that can
            be configured by the application
*/
/*==================================================================================================
Include Files
==================================================================================================*/
#include "sixlowpan.h"
#include "sixlowpan_tbl.h"
#include "sixlowpan_cfg.h"

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

taskMsgQueue_t   mSlwpMsgQueue = {0};
taskMsgQueue_t*  pSlwpMsgQueue = NULL;

slwpStruct_t    aSlwpStruct[SLWPCFG_INSTANCES_NB] = {0};
adpIb_t         aAdpIb[SLWPCFG_INSTANCES_NB];
const uint32_t sixlowpanInstancesNb = SLWPCFG_INSTANCES_NB;

#if SLWPCFG_ROUTER_ENABLE
uint64_t aSlwpSedAddrTbl[SLWPCFG_UNFRAG_SED_TRACK_NB];
adpQos_t aSlwpSedPktPriorityTbl[SLWPCFG_UNFRAG_SED_TRACK_NB * SLWPCFG_UNFRAG_SED_TRACK_PKT_NB];
#endif

/*! Context Table */
ndContextEntry_t* aContextTable[SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE] = {NULL};

/*! Context Table Size */
const uint32_t slwpContextTableSize = SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE;

const uint8_t  slwpMaxRetransmissions = SLWPCFG_MAX_6LOWPAN_RETRANSMISSIONS;

const uint8_t  slwpMaxFragments  = SLWPCFG_RFC4944_MAX_FRAGMENTS;

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

