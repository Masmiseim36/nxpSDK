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
\file       nd_globals.c
\brief      This is the source file that contains parameters for the ND module that can 
            be configured by the application.
*/     

/*==================================================================================================
Include Files
==================================================================================================*/

#include "nd.h"
#include "nd_cfg.h"

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
/*! ND Configs Table */
ndCfg_t* aNdCfg[IP_IF_NB] = {NULL};
ndRuntimeVariables_t aNdVariables[IP_IF_NB];
/*! ND Configs Table Size */
const uint32_t ndCfgTblSize = IP_IF_NB;
/*! Neighbor Cache */
ndNeighborEntry_t* aNeighborCache[ND_NEIGHBOR_CACHE_SIZE] = {NULL};
/*! Neighbor Cache Size */
const uint32_t ndNeighborCacheSize = ND_NEIGHBOR_CACHE_SIZE;
/*! Prefix List */
ndPrefixEntry_t* aPrefixList[ND_PREFIX_LIST_SIZE] = {NULL};
/*! Prefix List Size */
const uint32_t ndPrefixListSize = ND_PREFIX_LIST_SIZE;
/*! Route Information List Size */
const uint8_t ndRouteInfoListSize = ND_ROUTE_INFO_LIST_SIZE;
/*! Route Information */
ndOptionRouteInfo_t ndRouteInfoOption = { 0 };
/*! RDNSS List Size */
const uint32_t ndRDNSSListSize = ND_RDNSS_LIST_SIZE;
/*! Destination Cache */
ndDstCacheEntry_t* aDestinationCache[ND_DESTINATION_CACHE_SIZE] = {NULL};
/*! Destination Cache Size */
const uint32_t ndDestinationCacheSize = ND_DESTINATION_CACHE_SIZE;

const bool_t ndDADTransmits = ND_DAD_TRANSMITS;
const uint8_t ndLifeTimeGuardInterval = ND_LIFETIME_GUARD_INTERVAL;
const uint8_t ndRegAddrGuardTime = ND_REG_ADDRESS_GUARD_TIME;

#if ND_OPT_DNS_RCV_ENABLED
    ndOptionRDNSS_t ndRDNSSOption = { 0 };
    ndOptionDNSSL_t ndDNSSLOption = { 0 };
#endif

/*==================================================================================================
Public functions
==================================================================================================*/

