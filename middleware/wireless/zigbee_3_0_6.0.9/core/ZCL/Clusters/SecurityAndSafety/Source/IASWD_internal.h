/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  IASWD_INTERNAL_H_INCLUDED
#define  IASWD_INTERNAL_H_INCLUDED

/*!
\file       IASWD_internal.h
\brief      The internal API for the IAS WD Cluster
*/     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "IASWD.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC teZCL_Status eCLD_IASWDCommandHandler(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance);

#ifdef IASWD_SERVER
PUBLIC teZCL_Status eCLD_IASWDStartWarningReqReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_IASWD_StartWarningReqPayload *psPayload);
                    
PUBLIC teZCL_Status eCLD_IASWDSquawkReqReceive(
                    ZPS_tsAfEvent                *pZPSevent,
                    uint8                        *pu8TransactionSequenceNumber,
                    tsCLD_IASWD_SquawkReqPayload *psPayload);                    
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* IASWD_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
