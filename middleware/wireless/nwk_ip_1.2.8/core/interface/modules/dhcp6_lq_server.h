/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _DHCP6_LQ_SERVER_H_
#define _DHCP6_LQ_SERVER_H_
/*!=================================================================================================
\file       dhcp6_lq_server.h
\brief      This is a header file for the DHCPv6 Leasequery server module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "dhcp6_server.h"
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
\fn     void DHCP6_Server_LeasequeryService(uint8_t * pOptionsField, dhcp6ClientTempBindingTbl_t *pClientTempTbl,
                                            dhcp6ServerCfg_t* pServerStruct, uint32_t optionsSize)
\brief  Handles an incoming leasequery packet.

\param  [in]  pOptionsField   pointer to options field
\param  [in]  pClientTempTbl  pointer to client table
\param  [in]  pServerStruct   pointer to server's configuration table
\param  [in]  optionsSize     the size of the option field
***************************************************************************************************/
void DHCP6_Server_LeasequeryService(uint8_t * pOptionsField, dhcp6ClientTempBindingTbl_t *pClientTempTbl,
                                    dhcp6ServerCfg_t* pServerStruct, uint32_t optionsSize);


#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /*_DHCP6_LQ_SERVER_H_ */