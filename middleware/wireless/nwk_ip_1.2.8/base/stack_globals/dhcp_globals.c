/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
const uint32_t dhcp6ClientMaxRetryNb = DHCP6_CLIENT_MAX_RETRY_NB;
#endif

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

