/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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

