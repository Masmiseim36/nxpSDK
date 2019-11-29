/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       tcp_globals.c
\brief      This is the source file that contains parameters for the TCP module that can
            be configured by the application 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "stack_config.h"
#include "tcp.h"
#include "tcp_cfg.h"
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

/*! TCP Connections table */
tcpConn_t *gaTcpConnections[MAX_TCP_CONNECTIONS];
/*! Maximum number of TCP connections */
const uint32_t gTcpConnectionsSize = MAX_TCP_CONNECTIONS;

uint32_t gTcpMssDefault = TCP_MSS_DEFAULT;
uint32_t gTcpRcvWindow = TCP_WND;

const uint16_t gTcpMinPort = TCP_MIN_PORT_NUMBER; /*!< minimum port number available for users(1024) */
const uint16_t gTcpMaxPort = TCP_MAX_PORT_NUMBER; /*!< maximum port number available for users(49151) */
const uint8_t  gTcpHopLimit = TCP_DEFAULT_HOP_LIMIT; /*!< Default hop limit for IPv6 or TTL for IPv4 */

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

