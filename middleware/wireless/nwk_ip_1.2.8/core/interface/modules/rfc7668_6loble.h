/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _RFC7668_6LOWPAN_H
#define _RFC7668_6LOWPAN_H
/*!=================================================================================================
\file       rfc7668_6lowpan.h
\brief      This is a header interface file for the RFC7668 (IPv6 over BLUETOOTH(R) Low Energy) module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "ble_abs_types.h"
#include "network_utils.h"
#include "sixloble.h"
#include "sixloble_interface.h"
#include "sixlowpan_tbl.h"

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

adpStatus_t RFC7668_GetLinkLayerInfo(sloBlePktInfo_t * pSloPktInfo, ipAddr_t * pIpDstAddr);

uint32_t RFC7668_GetTotalPcktSize(sloBlePktInfo_t * pSloPktInfo);

bool_t RFC7668_ProcessIPv6Packet(bleDataInd_t * pBleDataInd, uint8_t ** ppCrtPos);

/*!*************************************************************************************************
\fn     void RFC7668_GetCompressionIID(sloBlePktInfo_t * pSloPktInfo, iidInfo_t * pIID)
\brief  Interface function for the 6LoWPAN module. It returns the Link-Layer address from the IID.

\param  [in]     pSloPktInfo    Pointer to 6LoWPAN packet structure
\param  [in,out] pIID            Pointer to the IID
***************************************************************************************************/
void RFC7668_GetCompressionIID(sloBlePktInfo_t * pSloPktInfo, iidInfo_t * pIID);


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _RFC7668_6LOWPAN_H */
