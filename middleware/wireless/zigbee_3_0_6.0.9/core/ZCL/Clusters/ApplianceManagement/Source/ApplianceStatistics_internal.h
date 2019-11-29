/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef  APPLIANCE_STATISTICS_INTERNAL_H_INCLUDED
#define  APPLIANCE_STATISTICS_INTERNAL_H_INCLUDED

/*!
\file       ApplianceStatistics_internal.h
\brief      The internal API for the Appliance Statistics Cluster
*/
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "ApplianceStatistics.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_ApplianceStatisticsCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

#ifdef APPLIANCE_STATISTICS_SERVER
PUBLIC teZCL_Status eCLD_ASCLogRequestReceive(
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_ASC_LogRequestPayload                     *psPayload);                                        

PUBLIC teZCL_Status eCLD_ASCLogQueueRequestReceive(
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);
#endif

#ifdef APPLIANCE_STATISTICS_CLIENT
PUBLIC teZCL_Status eCLD_ASCLogNotificationORLogResponseReceive(
                    ZPS_tsAfEvent                                               *pZPSevent,
                    uint8                                                       *pu8TransactionSequenceNumber,
                    tsCLD_ASC_LogNotificationORLogResponsePayload               *psPayload);

PUBLIC teZCL_Status eCLD_ASCLogQueueResponseORStatisticsAvailableReceive(
                    ZPS_tsAfEvent                                               *pZPSevent,
                    uint8                                                       *pu8TransactionSequenceNumber,
                    tsCLD_ASC_LogQueueResponseORStatisticsAvailablePayload       *psPayload);

#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* APPLIANCE_STATISTICS_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
