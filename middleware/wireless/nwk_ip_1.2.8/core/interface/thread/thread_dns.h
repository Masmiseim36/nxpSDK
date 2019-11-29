/*! *********************************************************************************
 * \defgroup DNS  Network IP RFC6106 Interface
 * @{
 ***********************************************************************************/
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP.
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
\fn     void THR_DNS_DataReqHandler(uint8_t *pParams)
\brief  Function used to call the DNS ND Data Req.

\param  [in]    pParams    Pointer to function parameters
***************************************************************************************************/
void THR_DNS_DataReqHandler (uint8_t *pParams);

/*!*************************************************************************************************
\fn void THR_DNS_ND_DataReq(instanceId_t thrInstanceID, coapCallback_t pfAppReturn)
\brief Sends a ND_DATA.req to obtain RDNSS/DNSSL information from the border routers for which
        P_nd_dns bit is True.

\param  [in] thrInstanceID      Thread instance ID
\param  [in] pfAppReturn        Pointer to the function to be called after the response is received
                                or timeout occurs
***************************************************************************************************/
void THR_DNS_ND_DataReq(instanceId_t thrInstanceID, coapCallback_t pfAppReturn);

/*!*************************************************************************************************
\fn     void THR_DNS_ND_DataService(coapSessionStatus_t sessionStatus, uint8_t *pData,
        coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for the ND_DATA.req message.

\param  [in]    sessionStatus    Status of the session
\param  [in]    pData            Pointer to data
\param  [in]    pSession         Pointer to CoAP session
\param  [in]    dataLen          Length of data
***************************************************************************************************/
void THR_DNS_ND_DataService(coapSessionStatus_t sessionStatus, uint8_t *pData, 
                                        coapSession_t *pSession, uint32_t dataLen);


#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _THREAD_DNS_H */
