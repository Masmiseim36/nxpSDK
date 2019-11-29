/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef  APPLIANCE_EVENTS_AND_ALERTS_INTERNAL_H_INCLUDED
#define  APPLIANCE_EVENTS_AND_ALERTS_INTERNAL_H_INCLUDED

/*!
\file       ApplianceEventsAndAlerts_internal.h
\brief      The internal API for for Appliance Event and Alert Cluster
*/
     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "ApplianceEventsAndAlerts.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_ApplianceEventsAndAlertsCommandHandler(
                ZPS_tsAfEvent               *pZPSevent,
                tsZCL_EndPointDefinition    *psEndPointDefinition,
                tsZCL_ClusterInstance       *psClusterInstance);

#ifdef APPLIANCE_EVENTS_AND_ALERTS_SERVER
PUBLIC teZCL_Status eCLD_AEAAGetAlertsReceive(
                ZPS_tsAfEvent                                   *pZPSevent,
                uint8                                           *pu8TransactionSequenceNumber);
#endif

#ifdef APPLIANCE_EVENTS_AND_ALERTS_CLIENT
PUBLIC teZCL_Status eCLD_AEAAGetAlertsResponseORAlertsNotificationReceive(
                ZPS_tsAfEvent                                                               *pZPSevent,
                uint8                                                                       *pu8TransactionSequenceNumber,
                tsCLD_AEAA_GetAlertsResponseORAlertsNotificationPayload                     *psPayload);

PUBLIC teZCL_Status eCLD_AEAAEventNotificationReceive(
                ZPS_tsAfEvent                                                               *pZPSevent,
                uint8                                                                       *pu8TransactionSequenceNumber,
                tsCLD_AEAA_EventNotificationPayload                                         *psPayload);
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* APPLIANCE_EVENTS_AND_ALERTS_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
