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

