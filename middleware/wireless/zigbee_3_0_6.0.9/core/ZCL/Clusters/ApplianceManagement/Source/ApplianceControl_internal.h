/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  APPLIANCE_CONTROL_INTERNAL_H_INCLUDED
#define  APPLIANCE_CONTROL_INTERNAL_H_INCLUDED

/*!
\file       ApplianceControl_internal.h
\brief      The internal API for the Appliance Control Cluster
*/
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "ApplianceControl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_ApplianceControlCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

#ifdef APPLIANCE_CONTROL_SERVER
PUBLIC teZCL_Status eCLD_ACExecutionOfCommandReceive(
                    ZPS_tsAfEvent                                                       *pZPSevent,
                    uint8                                                               *pu8TransactionSequenceNumber,
                    tsCLD_AC_ExecutionOfCommandPayload                                  *psPayload);
                    
PUBLIC teZCL_Status eCLD_ACSignalStateReceive(
                    ZPS_tsAfEvent                                                       *pZPSevent,
                    uint8                                                               *pu8TransactionSequenceNumber);
#endif

#ifdef APPLIANCE_CONTROL_CLIENT
PUBLIC teZCL_Status eCLD_ACSignalStateResponseORSignalStateNotificationReceive(
                    ZPS_tsAfEvent                                                       *pZPSevent,
                    uint8                                                               *pu8TransactionSequenceNumber,
                    tsCLD_AC_SignalStateResponseORSignalStateNotificationPayload        *psPayload);
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* APPLIANCE_CONTROL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
