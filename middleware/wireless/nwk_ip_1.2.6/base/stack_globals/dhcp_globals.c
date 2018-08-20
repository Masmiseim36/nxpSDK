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
\file       dhcp_globals.h
\brief      This is a public header file for the DHCPv4 and DHCPv6 module. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "dhcp_client.h"
#include "dhcp_server.h"

#include "dhcp6_client.h"
#include "dhcp6_server.h"

#include "dhcp_cfg.h"
#include "dhcp6_cfg.h"

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

/*! DHCP4 Server*/
#if DHCP4_SERVER_ENABLED && IP_IP4_ENABLE
dhcpServerCfgStruct_t   *aServerCfgStruct[IP_IF_NB] = {NULL};
dhcpServerData_t        *aServerBindingTbl[DHCP4_SERVER_MAX_CLIENTS] = {NULL};
const uint32_t dhcp4ServerMaxClients = DHCP4_SERVER_MAX_CLIENTS;
#endif

#if (DHCP4_SERVER_ENABLED || DHCP4_CLIENT_ENABLED) && IP_IP4_ENABLE
const uint32_t dhcp4ExpirationTimerPeriod = DHCP4_EXPIRATION_TIMER_PERIOD_SEC;
#endif

/*! DHCP4 Client*/
#if DHCP4_CLIENT_ENABLED && IP_IP4_ENABLE
dhcpClientData_t        *aClientParamsTbl[IP_IF_NB] = {NULL};
const uint32_t dhcp4ClientReqTime = DHCP4_CLIENT_REQ_TIMER;
const uint32_t dhcp4ClientRetryNb = DHCP4_CLIENT_DEF_RETRY_NB;
#endif


/*! DHCP6 Server*/
#if DHCP6_SERVER_ENABLED
dhcp6ServerCfg_t*        aDhcp6ServerCfgStruct[DHCP6_SERVER_MAX_INSTANCES];
dhcp6ServerBindingTbl_t *mDhcp6ServerBindingTbl[DHCP6_SERVER_MAX_CLIENTS] = {NULL};
const uint32_t dhcp6ServerMaxClients = DHCP6_SERVER_MAX_CLIENTS;
const uint32_t dhcp6ServerMaxInstances = DHCP6_SERVER_MAX_INSTANCES;
const uint32_t dhcp6PreferredLifetime = DHCP6_DEFAULT_PREFERRED_LIFETIME;
const uint32_t dhcp6ValidLifetime =  DHCP6_DEFAULT_VALID_LIFETIME;
const uint32_t dhcp6T1 =  DHCP6_DEFAULT_T1;
const uint32_t dhcp6T2 =   DHCP6_DEFAULT_T2;
#endif



/*! DHCP6 Client*/
#if DHCP6_CLIENT_ENABLED
dhcp6ClientData_t       *aDhcp6ClientParams[DHCP6_CLIENT_MAX_INSTANCES] = {NULL};
dhcp6ClientRamData_t    gDhcp6ClientRamData[DHCP6_CLIENT_MAX_INSTANCES]; /* RAM stucture */
const uint32_t dhcp6ClientMaxInstances = DHCP6_CLIENT_MAX_INSTANCES;
#endif

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

