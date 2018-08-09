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

#ifndef _SIXLOWPAN_FWD_THREAD_H
#define _SIXLOWPAN_FWD_THREAD_H

/*!=================================================================================================
\file       sixlowpan_fwd_thread.h
\brief      This is a header file for the sixlowpan forwarding functionality for THREAD.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "ip_if_management.h"
#include "network_utils.h"
#include "mac_abs_types.h"


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
\fn     llAddr_t adpThreadGetParent(llAddr_t* pMyAddr, llAddr_t* pFinalAddr, llAddr_t *pMacSrcAddr,
        instanceId_t instanceId)
\brief  Function used to get the parent node Link-Layer address.

\param  [in]    pMyAddr         Pointer to device Link-Layer address.
\param  [in]    pFinalAddr      Pointer to final destination Link-Layer address.
\param  [in]    pMacSrclAddr    Pointer to neighbor mac source Link-Layer address.
\param  [in]    instanceId      Instance ID.

\return         llAddr_t        Parent node Link-Layer address.
***************************************************************************************************/
llAddr_t adpThreadGetParent(llAddr_t* myAddr, llAddr_t* finalAddr, llAddr_t *pMacSrcAddr,
                                      instanceId_t instanceId);


/*!*************************************************************************************************
\fn     bool_t adpThreadIsAddrOnLink(ipAddr_t *pIpAddr, uint32_t interfaceId)
\brief  Function used to determine the on link status of a destination address.

\param  [in]    pIpAddr         Pointer to the IPv6 address.
\param  [in]    ifHandle        Pointer to media interface struct.

\retval         TRUE            Address is ON LINK.
\retval         FALSE           Address is OFF LINK.
***************************************************************************************************/
bool_t adpThreadIsAddrOnLink(ipAddr_t *pIpAddr, ifHandle_t ifHandle);


/*!*************************************************************************************************
\fn     bool_t adpThreadIsMeshNeeded(void * param)
\brief  Function used to determine if the mesh header is required for forwarding the packet.

\param  [in]    param         Pointer to the 6LoWPAN info structure.

\retval         TRUE            Mesh Header is needed.
\retval         FALSE           Mesh Header is not needed.
***************************************************************************************************/
bool_t adpThreadIsMeshNeeded(void * param);

#if THR_ROUTING_ENABLE
/*!*************************************************************************************************
\fn     llAddr_t adpThreadGetNextHop(llAddr_t* pMyAddr, llAddr_t* pFinalAddr, llAddr_t *pMacSrcAddr,
                                     instanceId_t instanceId)
\brief  Function used to get the next hop Link-Layer address.

\param  [in]    pMyAddr         Pointer to device Link-Layer address.
\param  [in]    pFinalAddr      Pointer to final destination Link-Layer address.
\param  [in]    pMacSrclAddr    Pointer to neighbor mac source Link-Layer address.
\param  [in]    instanceId      Instance ID.

\return         llAddr_t        Next hop Link-Layer address.
***************************************************************************************************/
llAddr_t adpThreadGetNextHop(llAddr_t* myAddr, llAddr_t* finalAddr, llAddr_t *pMacSrcAddr,
                                        instanceId_t instanceId);

/*!*************************************************************************************************
\fn     void adpThreadSetRoutingTable(void * pMeshRouteTbl, instanceId_t instanceId)
\brief  Function used to set the whole routing table.

\param  [in]    pMeshRouteTbl   Pointer to routing table.
\param  [in]    instanceId      Instance ID.
***************************************************************************************************/
void adpThreadSetRoutingTable(void * pMeshRouteTbl, instanceId_t instanceId);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/

#endif
#endif  /* _SIXLOWPAN_FWD_THREAD_H */
