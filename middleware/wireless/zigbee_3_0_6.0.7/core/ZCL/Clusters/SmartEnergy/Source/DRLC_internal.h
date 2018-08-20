/*
* The Clear BSD License
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


#ifndef  DRLC_INTERNAL_H_INCLUDED
#define  DRLC_INTERNAL_H_INCLUDED

/*!
\file       DRLC_internal.h
\brief      The Internal API for the  DRLC Cluster
*/     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "DRLC.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teSE_DRLCStatus eSE_DRLCLoadControlEventSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address              *psDestinationAddress,
                    tsSE_DRLCLoadControlEvent  *psLoadControlEvent,
                    bool_t                      bSendWithTimeNow,
                    uint8                      *pu8TransactionSequenceNumber);

PUBLIC teSE_DRLCStatus eSE_DRLCLoadControlEventReceive(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsSE_DRLCLoadControlEvent   *psLoadControlEvent,
                    uint8                       *pu8TransactionSequenceNumber);

PUBLIC teSE_DRLCStatus eSE_DRLCCancelLoadControlEventSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address              *psDestinationAddress,
                    tsSE_DRLCCancelLoadControlEvent *psCancelLoadControlEvent,
                    uint8                      *pu8TransactionSequenceNumber);

PUBLIC teSE_DRLCStatus eSE_DRLCCancelLoadControlEventReceive(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsSE_DRLCCancelLoadControlEvent *psCancelLoadControlEvent,
                    uint8                       *pu8TransactionSequenceNumber);

PUBLIC teSE_DRLCStatus eSE_DRLCCancelAllLoadControlEventsSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address              *psDestinationAddress,
                    teSE_DRLCCancelControl     eSE_CancelEventsControl,
                    uint8                      *pu8TransactionSequenceNumber);

PUBLIC teSE_DRLCStatus eSE_DRLCCancelAllLoadControlEventsReceive(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsSE_DRLCCancelLoadControlAllEvent *pEvent,
                    uint8                       *pu8TransactionSequenceNumber);

PUBLIC teSE_DRLCStatus eSE_DRLCGetScheduledEventsReceive(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsSE_DRLCGetScheduledEvents *psGetScheduledEvents,
                    uint8                       *pu8TransactionSequenceNumber);

PUBLIC teSE_DRLCStatus eSE_DRLCReportEventStatusReceive(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsSE_DRLCReportEvent        *psReportEvent,
                    uint8                       *pu8TransactionSequenceNumber);

PUBLIC void vSE_DRLCHandleReportEventStatus(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC void vSE_DRLCHandleGetScheduledEvents(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC void vSE_DRLCHandleLoadControlEvent(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC void vSE_DRLCHandleLoadControlEventCancel(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC void vSE_DRLCHandleLoadControlEventCancelAll(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC teSE_DRLCStatus eSE_DRLCGetScheduledEventsReceive(
                    ZPS_tsAfEvent               *pZPSevent, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsSE_DRLCGetScheduledEvents *psGetScheduledEvents,
                    uint8                       *pu8TransactionSequenceNumber);

PUBLIC teSE_DRLCStatus eSE_DRLCAddLoadControlEventNoMutex(
                    tsZCL_EndPointDefinition     *psEndPointDefinition,
                    tsZCL_ClusterInstance        *psClusterInstance,
                    tsSE_DRLCCustomDataStructure *psDRLCCustomDataStructure,
                    tsSE_DRLCLoadControlEvent    *psLoadControlEvent);

PUBLIC void vSetRandomizationValues(
                    tsZCL_EndPointDefinition     *psEndPointDefinition, 
                    tsZCL_ClusterInstance        *psClusterInstance,
                    tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord);

PUBLIC bool_t boInEffectiveTimeExtendedEventOperation(
                    tsSE_DRLCLoadControlEvent   *psLoadControlEvent, 
                    uint32                       u32UTCtime);

PUBLIC bool boEffectiveExpiredTimeCheck(
                    tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint32                       u32UTCtime);

PUBLIC bool boInEffectiveActiveTimeCheck(
                    tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint32                       u32UTCtime);

PUBLIC uint32 u32EffectiveDuration(
                    tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord, 
                    tsZCL_EndPointDefinition    *psEndPointDefinition, 
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC uint32 boCancelTimeCheck(
                    tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord, 
                    tsZCL_EndPointDefinition   *psEndPointDefinition, 
                    tsZCL_ClusterInstance      *psClusterInstance,
                    uint32                      u32UTCtime);

PUBLIC teSE_DRLCStatus eSE_DRLCFindDRLCCluster(
                    uint8                        u8SourceEndPointId,
                    bool_t                       bIsServer,
                    tsZCL_EndPointDefinition     **ppsEndPointDefinition,
                    tsZCL_ClusterInstance        **ppsClusterInstance,
                    tsSE_DRLCCustomDataStructure **ppsDRLCCustomDataStructure);

PUBLIC teSE_DRLCStatus eSE_DRLCAddToSchedulerList(
                    tsZCL_EndPointDefinition           *psEndPointDefinition, 
                    tsZCL_ClusterInstance              *psClusterInstance,
                    tsSE_DRLCCustomDataStructure       *psDRLCCustomDataStructure,
                    tsSE_DRLCLoadControlEventRecord    *psLoadControlEventRecord, 
                    teSE_DRLCEventList                  eFromEventList,
                    teSE_DRLCEventList                  eToEventList);

PUBLIC teSE_DRLCStatus eSE_DRLCOverlapAddEventToScheduler(
                    tsZCL_EndPointDefinition           *psEndPointDefinition, 
                    tsZCL_ClusterInstance              *psClusterInstance,
                    tsSE_DRLCLoadControlEventRecord    *psLoadControlEventAddRecord, 
                    tsSE_DRLCCustomDataStructure       *psDRLCCustomDataStructure,
                    teSE_DRLCEventList                  eFromEventList,
                    teSE_DRLCEventList                  eToEventList);

PUBLIC teSE_DRLCStatus eSE_DRLCCancelLoadControlEventNoMutex(
                    tsZCL_EndPointDefinition           *psZCL_EndPointDefinition, 
                    tsZCL_ClusterInstance              *psClusterInstance,
                    tsSE_DRLCCustomDataStructure       *psDRLCCustomDataStructure,
                    tsSE_DRLCCancelLoadControlEvent    *psCancelLoadControlEvent);

PUBLIC tsSE_DRLCLoadControlEventRecord * psGetListHead(
                    tsSE_DRLCCustomDataStructure       *psDRLCCustomDataStructure, 
                    teSE_DRLCEventList                  eEventList,
                    DLIST                             **pplEventList);

PUBLIC teSE_DRLCStatus eSE_DRLCGenerateReportEventStatusMessage(
                    tsZCL_ClusterInstance              *psClusterInstance,
                    tsSE_DRLCReportEvent               *psReportEvent);

PUBLIC teSE_DRLCStatus eSE_DRLCFindDRLCCluster(
                    uint8                               u8SourceEndPointId,
                    bool_t                              bIsServer,
                    tsZCL_EndPointDefinition         **ppsEndPointDefinition,
                    tsZCL_ClusterInstance            **ppsClusterInstance,
                    tsSE_DRLCCustomDataStructure     **ppsDRLCCustomDataStructure);

PUBLIC teSE_DRLCStatus eSE_CancelAllLoadControlEventsReceive(
                    ZPS_tsAfEvent                      *pZPSevent, 
                    tsZCL_EndPointDefinition           *psEndPointDefinition, 
                    teSE_DRLCCancelControl             *peCancelEventControl,
                    uint8                              *pu8TransactionSequenceNumber);

PUBLIC void vRemoveFromSchedulerList(
                    tsSE_DRLCCustomDataStructure       *psDRLCCustomDataStructure,
                    tsSE_DRLCLoadControlEventRecord    *psLoadControlEventRecord,
                    teSE_DRLCEventList                  eEventList);

PUBLIC void vSimpleAddEventToScheduler(
                    tsSE_DRLCCustomDataStructure       *psDRLCCustomDataStructure,
                    tsSE_DRLCLoadControlEventRecord    *psLoadControlEventRecord,
                    teSE_DRLCEventList                  eFromEventList,
                    teSE_DRLCEventList                  eToEventList);

PUBLIC tsSE_DRLCLoadControlEventRecord * psCheckToSeeIfEventExists(
                   tsSE_DRLCCustomDataStructure        *psDRLCCustomDataStructure, 
                   uint32                               u32IssuerId,
                   teSE_DRLCEventList                  *peEventList);

PUBLIC teSE_DRLCStatus eSE_DRLCSchedulerUpdate(
                   uint8                                u8SourceEndPointId, 
                   bool_t                               bIsServer, 
                   uint32                               u32UTCtime);

PUBLIC teSE_DRLCStatus eDRLC_SetCallBackMessage(
                   tsDRLC_Common                       *psDRLC_Common,
                   teSE_DRLCCallBackEventType           eEventType,
                   uint8                                u8CommandId,
                   teSE_DRLCStatus                      eDRLCStatus);

PUBLIC teZCL_Status eSE_DRLCCommandHandler(
                    ZPS_tsAfEvent                      *pZPSevent, 
                    tsZCL_EndPointDefinition           *psEndPointDefinition, 
                    tsZCL_ClusterInstance              *psClusterInstance);

PUBLIC teSE_DRLCStatus eSE_DRLCInitialiseReportStructure(
                    tsSE_DRLCReportEvent               *psReportEvent, 
                    uint32                              u32IssuerId,
                    uint8                               u8CriticalityLevel,
                    uint8                               u8EventControl);


/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* DRLC_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
