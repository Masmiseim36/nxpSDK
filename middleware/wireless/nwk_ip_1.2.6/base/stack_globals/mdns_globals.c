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
