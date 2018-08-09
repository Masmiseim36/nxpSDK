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

/*!=============================================================================
\file       ApplianceEventsAndAlerts.c
\brief      Appliance Events And Alerts Cluster definition
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
#include "ApplianceEventsAndAlerts.h"
#include "ApplianceEventsAndAlerts_internal.h"
#include "zcl_options.h"

#include "zcl.h"
#include "string.h"

#ifdef CLD_APPLIANCE_EVENTS_AND_ALERTS

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
#ifdef ZCL_COMMAND_DISCOVERY_SUPPORTED
    const tsZCL_CommandDefinition asCLD_ApplianceEventsAndAlertsClusterCommandDefinitions[] = {
        {E_CLD_APPLIANCE_EVENTS_AND_ALERTS_CMD_GET_ALERTS,                E_ZCL_CF_RX|E_ZCL_CF_TX},     /* Mandatory */
        {E_CLD_APPLIANCE_EVENTS_AND_ALERTS_CMD_ALERTS_NOTIFICATION,       E_ZCL_CF_TX},     /* Mandatory */
        {E_CLD_APPLIANCE_EVENTS_AND_ALERTS_CMD_EVENT_NOTIFICATION,        E_ZCL_CF_TX}
        
    };
#endif

const tsZCL_AttributeDefinition asCLD_ApplianceEventsAndAlertsClusterAttributeDefinitions[] = {
    {E_CLD_GLOBAL_ATTR_ID_CLUSTER_REVISION,            (E_ZCL_AF_RD|E_ZCL_AF_GA),                E_ZCL_UINT16,     (uint32)(&((tsCLD_ApplianceEventsAndAlerts*)(0))->u16ClusterRevision),      0},   /* Mandatory  */    
};

tsZCL_ClusterDefinition sCLD_ApplianceEventsAndAlerts = {
        APPLIANCE_MANAGEMENT_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS,
        FALSE,
        E_ZCL_SECURITY_NETWORK,
        (sizeof(asCLD_ApplianceEventsAndAlertsClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition)),
        (tsZCL_AttributeDefinition*)asCLD_ApplianceEventsAndAlertsClusterAttributeDefinitions,
        NULL
        #ifdef ZCL_COMMAND_DISCOVERY_SUPPORTED
            ,
            (sizeof(asCLD_ApplianceEventsAndAlertsClusterCommandDefinitions) / sizeof(tsZCL_CommandDefinition)),
            (tsZCL_CommandDefinition*)asCLD_ApplianceEventsAndAlertsClusterCommandDefinitions
        #endif
};
uint8 au8ApplianceEventsAndAlertsAttributeControlBits[(sizeof(asCLD_ApplianceEventsAndAlertsClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition))];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:       eCLD_ApplianceEventsAndAlertsCreateApplianceEventsAndAlerts
 *
 * DESCRIPTION:
 * Creates a basic cluster
 *
 * PARAMETERS:  Name                         Usage
 *              psClusterInstance            Cluster structure
 *              bIsServer                    Server/Client Flag
 *              psClusterDefinition          Cluster Definitation
 *              pvEndPointSharedStructPtr    EndPoint Shared Structure Pointer
 *              pu8AttributeEventsAndAlertsBits      Attribute EventsAndAlerts Bits
 * RETURN:
 * teZCL_Status
 *
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ApplianceEventsAndAlertsCreateApplianceEventsAndAlerts(
                tsZCL_ClusterInstance                                  *psClusterInstance,
                bool_t                                                  bIsServer,
                tsZCL_ClusterDefinition                                *psClusterDefinition,
                void                                                   *pvEndPointSharedStructPtr,
                uint8                                                  *pu8AttributeEventsAndAlertsBits,
                tsCLD_ApplianceEventsAndAlertsCustomDataStructure      *psCustomDataStructure)
{

    #ifdef STRICT_PARAM_CHECK 
        /* Parameter check */
        if((psClusterInstance==NULL) ||
           (psClusterDefinition==NULL) ||
           (psCustomDataStructure==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif
    
    /* Create an instance of an appliance events and alerts cluster */
    vZCL_InitializeClusterInstance(
                               psClusterInstance, 
                               bIsServer,
                               psClusterDefinition,
                               pvEndPointSharedStructPtr,
                               pu8AttributeEventsAndAlertsBits,
                               psCustomDataStructure,
                               eCLD_ApplianceEventsAndAlertsCommandHandler);
                     
    psCustomDataStructure->sCustomCallBackEvent.eEventType = E_ZCL_CBET_CLUSTER_CUSTOM;
    psCustomDataStructure->sCustomCallBackEvent.uMessage.sClusterCustomMessage.u16ClusterId = psClusterDefinition->u16ClusterEnum;
    psCustomDataStructure->sCustomCallBackEvent.uMessage.sClusterCustomMessage.pvCustomData = (void *)&psCustomDataStructure->sCallBackMessage;
    psCustomDataStructure->sCustomCallBackEvent.psClusterInstance = psClusterInstance; 
    
    /* Initialise attributes defaults */
    if(pvEndPointSharedStructPtr != NULL)
    {
        ((tsCLD_ApplianceEventsAndAlerts*)pvEndPointSharedStructPtr)->u16ClusterRevision = CLD_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_REVISION;
    }    
    
    return E_ZCL_SUCCESS;
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

