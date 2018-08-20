/*! *********************************************************************************
 * \defgroup DNS  Network IP RFC6106 Interface
 * @{
 ***********************************************************************************/
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

#ifndef _THREAD_DNS_H
#define _THREAD_DNS_H
/*!
\file       thread_dns.h
\brief      This is a header file for the Thread DNS services module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* Framework */
#include "EmbeddedTypes.h"

/* Application */
#include "coap.h"

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
\fn void THR_DNS_BRInit(void)
\brief Registers the BR network services. For now just provider of RDNSS/DNSSL information.
***************************************************************************************************/
void THR_DNS_BRInit(void);

/*!*************************************************************************************************
\fn void THR_DNS_ND_DataReq(instanceId_t thrInstanceID, coapCallback_t pfAppReturn)
\brief Sends a ND_DATA.req to obtain RDNSS/DNSSL information from the border routers for which
        P_nd_dns bit is True.

\param  [in] thrInstanceID      Thread instance ID
\param  [in] pfAppReturn        Pointer to the function to be called after the response is received
                                or timeout occurs
***************************************************************************************************/
void THR_DNS_ND_DataReq(instanceId_t thrInstanceID, coapCallback_t pfAppReturn);


#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _THREAD_DNS_H */
