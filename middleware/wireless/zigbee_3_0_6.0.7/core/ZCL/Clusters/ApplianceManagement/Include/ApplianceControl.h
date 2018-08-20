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

#ifndef APPLIANCE_CONTROL_H
#define APPLIANCE_CONTROL_H

/*!
\file       ApplianceControl.h
\brief      Header for Appliance Control Cluster
*/

#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"
#include "zcl_customcommand.h"
#include "dlist.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/*             Appliance Control Cluster - Optional Attributes                          */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Appliance Control  cluster.                                         */
/****************************************************************************/

/* Remaining Time attribute */
//#ifdef CLD_APPLIANCE_CONTROL_REMAINING_TIME

/* Optional Global Attribute Reporting Status */
//#ifdef CLD_APPLIANCE_CONTROL_ATTRIBUTE_REPORTING_STATUS

/* Cluster ID's */
#define APPLIANCE_MANAGEMENT_CLUSTER_ID_APPLIANCE_CONTROL                                 0x001b

#ifndef CLD_APPLIANCE_CONTROL_CLUSTER_REVISION
    #define CLD_APPLIANCE_CONTROL_CLUSTER_REVISION                     1
#endif   

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum 
{
    E_CLD_APPLIANCE_CONTROL_ATTR_ID_START_TIME                           = 0x0000, /* Mandatory */
    E_CLD_APPLIANCE_CONTROL_ATTR_ID_FINISH_TIME,                                   /* Mandatory */
    E_CLD_APPLIANCE_CONTROL_ATTR_ID_REMAINING_TIME,                                /* Optional */
} teCLD_ApplianceControl_Cluster_AttrID;

/* Client Command codes */
typedef enum 
{
    E_CLD_APPLIANCE_CONTROL_CMD_EXECUTION_OF_COMMAND                = 0x00,     /* Mandatory */
    E_CLD_APPLIANCE_CONTROL_CMD_SIGNAL_STATE,                                   /* Mandatory */
} teCLD_ApplianceControl_ClientCommandId;

/* Server Command codes */
typedef enum 
{
    E_CLD_APPLIANCE_CONTROL_CMD_SIGNAL_STATE_RESPONSE                = 0x00,     /* Mandatory */
    E_CLD_APPLIANCE_CONTROL_CMD_SIGNAL_STATE_NOTIFICATION                        /* Mandatory */
} teCLD_ApplianceControl_ServerCommandId;

/* Appliance Control Cluster */
typedef struct
{
#ifdef APPLIANCE_CONTROL_SERVER    
    zuint16                         u16StartTime;
    zuint16                         u16FinishTime;
#ifdef CLD_APPLIANCE_CONTROL_REMAINING_TIME
    zuint16                         u16RemainingTime;
#endif
#ifdef CLD_APPLIANCE_CONTROL_ATTRIBUTE_REPORTING_STATUS
    zenum8                          u8AttributeReportingStatus;
#endif
#endif
    zuint16                         u16ClusterRevision; 
} tsCLD_ApplianceControl;

/* Execution of Command Payload */
typedef struct
{
    zenum8                         eExecutionCommandId;
} tsCLD_AC_ExecutionOfCommandPayload;

/* Signal State Response  & Signal State Notifivcation Payload */
typedef struct
{
    zenum8                         eApplianceStatus;
    zuint8                         u8RemoteEnableFlagAndDeviceStatusTwo;
    zuint24                        u24ApplianceStatusTwo;
} tsCLD_AC_SignalStateResponseORSignalStateNotificationPayload;

typedef struct
{
    uint8                                       u8CommandId;
    bool                                        *pbApplianceStatusTwoPresent;
    union
    {
        tsCLD_AC_ExecutionOfCommandPayload                               *psExecutionOfCommandPayload;
        tsCLD_AC_SignalStateResponseORSignalStateNotificationPayload     *psSignalStateResponseORSignalStateNotificationPayload;
    } uMessage;
} tsCLD_ApplianceControlCallBackMessage;

/* Custom data structure */
typedef struct
{
    tsZCL_ReceiveEventAddress                 sReceiveEventAddress;
    tsZCL_CallBackEvent                       sCustomCallBackEvent;
    tsCLD_ApplianceControlCallBackMessage     sCallBackMessage;
} tsCLD_ApplianceControlCustomDataStructure;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_ApplianceControlCreateApplianceControl(
                tsZCL_ClusterInstance                              *psClusterInstance,
                bool_t                                             bIsServer,
                tsZCL_ClusterDefinition                            *psClusterDefinition,
                void                                               *pvEndPointSharedStructPtr,
                uint8                                              *pu8AttributeControlBits,
                tsCLD_ApplianceControlCustomDataStructure          *psCustomDataStructure);
                
#ifdef APPLIANCE_CONTROL_SERVER
PUBLIC INLINE teZCL_Status eCLD_ACSignalStateNotificationSend(                
                uint8                                                         u8SourceEndPointId,
                uint8                                                         u8DestinationEndPointId,
                tsZCL_Address                                                 *psDestinationAddress,
                uint8                                                         *pu8TransactionSequenceNumber,
                bool                                                          bApplianceStatusTwoPresent,
                tsCLD_AC_SignalStateResponseORSignalStateNotificationPayload  *psPayload) ALWAYS_INLINE;
                
PUBLIC teZCL_Status eCLD_ACSignalStateResponseORSignalStateNotificationSend(
                uint8                                                               u8SourceEndPointId,
                uint8                                                               u8DestinationEndPointId,
                tsZCL_Address                                                       *psDestinationAddress,
                uint8                                                               *pu8TransactionSequenceNumber,
                teCLD_ApplianceControl_ServerCommandId                              eCommandId,
                bool                                                                bApplianceStatusTwoPresent,
                tsCLD_AC_SignalStateResponseORSignalStateNotificationPayload       *psPayload);

PUBLIC INLINE teZCL_Status eCLD_ACSignalStateNotificationSend(                
                uint8                                                         u8SourceEndPointId,
                uint8                                                         u8DestinationEndPointId,
                tsZCL_Address                                                 *psDestinationAddress,
                uint8                                                         *pu8TransactionSequenceNumber,
                bool                                                          bApplianceStatusTwoPresent,
                tsCLD_AC_SignalStateResponseORSignalStateNotificationPayload  *psPayload)
{
    // handle sequence number get a new sequence number
    if(pu8TransactionSequenceNumber != NULL)
        *pu8TransactionSequenceNumber = u8GetTransactionSequenceNumber();
    return eCLD_ACSignalStateResponseORSignalStateNotificationSend(
                                                     u8SourceEndPointId,
                                                u8DestinationEndPointId,
                                                   psDestinationAddress,
                                           pu8TransactionSequenceNumber,
                  E_CLD_APPLIANCE_CONTROL_CMD_SIGNAL_STATE_NOTIFICATION,
                                             bApplianceStatusTwoPresent,
                                                              psPayload);
}
                
PUBLIC teZCL_Status eCLD_ACChangeAttributeTime(
                uint8                                             u8SourceEndPointId,
                teCLD_ApplianceControl_Cluster_AttrID             eAttributeTimeId,
                uint16                                            u16TimeValue);        
#endif

#ifdef APPLIANCE_CONTROL_CLIENT                    
PUBLIC teZCL_Status eCLD_ACExecutionOfCommandSend(
                uint8                                               u8SourceEndPointId,
                uint8                                               u8DestinationEndPointId,
                tsZCL_Address                                       *psDestinationAddress,
                uint8                                               *pu8TransactionSequenceNumber,
                tsCLD_AC_ExecutionOfCommandPayload                  *psPayload);
                    
PUBLIC teZCL_Status eCLD_ACSignalStateSend(
                uint8                                               u8SourceEndPointId,
                uint8                                               u8DestinationEndPointId,
                tsZCL_Address                                       *psDestinationAddress,
                uint8                                               *pu8TransactionSequenceNumber);
#endif                    
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern tsZCL_ClusterDefinition sCLD_ApplianceControl;
extern const tsZCL_AttributeDefinition asCLD_ApplianceControlClusterAttributeDefinitions[];
extern uint8 au8ApplianceControlAttributeControlBits[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* APPLIANCE_CONTROL_H */
