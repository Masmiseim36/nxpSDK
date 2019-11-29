/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _DHCP6_LQ_CLIENT_H_
#define _DHCP6_LQ_CLIENT_H_


/*!=================================================================================================
\file       dhcp6_lq_client.h
\brief      This is a header file for the DHCPv6 Leasequery client module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "dhcp6_client.h"
#include "ip.h"

/*==================================================================================================
Public macros
==================================================================================================*/


/*==================================================================================================
Public type definitions
==================================================================================================*/



/*==================================================================================================
Public global variables declarations
==================================================================================================*/



/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void DHCP6_Client_SendLeaseQuery(ifHandle_t* pIfHandle, uint8_t* queryAddr,
                                         dhcp6LqNotifyCb_t pfNotifyData, ipAddr_t* pIpSrcAddr,
                                         uint8_t queryType)
\brief  This function sends a DHCP LeaseQuery message.

\param  [in]  pIfHandle     Pointer to the interface handle.
\param  [in]  queryAddr     Pointer to the searched device's address
\param  [in]  pfNotifyData  Callback function for lease query replay notification.
\param  [in]  pIpSrcAddr    Lease query source address or NULL for IP source address select
\param  [in]  queryType     Query type
***************************************************************************************************/
void DHCP6_Client_SendLeaseQuery(ifHandle_t* pIfHandle, uint8_t* queryAddr,
                                 dhcp6LqNotifyCb_t pfNotifyData, ipAddr_t* pIpSrcAddr,
                                 uint8_t queryType);

/*!*************************************************************************************************
\fn     void DHCP6_Client_Lq_Service(uint8_t * pOptionsField,dhcp6ClientData_t* pClientParams, 
                                     uint32_t optionsSize)
\brief  This function handles a DHCP LeaseQuery Reply message.

\param  [in]  pOptionsField  Pointer to the beginning of option field of the DHCP message.
\param  [in]  pClientParams  Pointer to the client data table.
\param  [in]  optionsSize    Size of the options.
***************************************************************************************************/
void DHCP6_Client_Lq_Service(uint8_t * pOptionsField, dhcp6ClientData_t* pClientParams,
                             uint32_t optionsSize);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /*_DHCP6_LQ_CLIENT_H_ */

