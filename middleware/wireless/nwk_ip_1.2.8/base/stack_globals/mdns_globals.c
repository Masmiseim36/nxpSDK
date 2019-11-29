/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       mdns_globals.c
\brief      This is the source file that contains parameters for the MDNS module that can
            be configured by the application 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "mdns.h"
#include "mdns_cfg.h"
#include "dns_client.h"

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

/*! UDP Connections table */
mDnsService_t *aMdnsServices[MDNS_MAX_SERVICES_NO];
const uint32_t gMdnsServicesNo = MDNS_MAX_SERVICES_NO;

dnsCacheEntry_t *aMdnsCache[MDNS_CACHE_ENTRIES];
const uint32_t gMdnsCacheEntriesNo = MDNS_CACHE_ENTRIES;

const ipAddr_t mdnsIp6Addr = MDNS_IP6_ADDRESS;
const uint32_t mdnsIp4Addr = MDNS_IP4_ADDRESS;

const uint16_t mdnsIp6Port = MDNS6_SERVER_PORT;
const uint16_t mdnsIp4Port = MDNS4_SERVER_PORT;

const uint16_t mdnsProbingTime = MDNS_PROBING_TIME;
const uint16_t mdnsAnnouncingTime = MDNS_ANNOUNCING_TIME;
const uint16_t mdnsAnnouncingSpets = MDNS_ANNOUNCING_STEPS;
const uint16_t msdnTLL = MDNS_TTL;

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/
