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
