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

