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


#ifndef ONOFF_H
#define ONOFF_H

/*!
\file       OnOff.h
\brief      Header for On/Off Cluster
*/
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/     
#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Cluster ID's */
#define GENERAL_CLUSTER_ID_ONOFF                        0x0006

/****************************************************************************/
/*             On/Off Control Cluster - Attributes             */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the level control cluster.                                 */
/****************************************************************************/
/* global scene control attribute */
//#define CLD_ONOFF_ATTR_GLOBAL_SCENE_CONTROL

/* on time attribute */
//#define CLD_ONOFF_ATTR_ON_TIME

/* global scene control attribute */
//#define CLD_ONOFF_ATTR_OFF_WAIT_TIME

/* Enable the optional Attribute Reporting Status attribute */
//#define CLD_ONOFF_ATTR_ATTRIBUTE_REPORTING_STATUS

/* Enable the optional Attribute StartUpOnOff  for ZLO extension*/
//#define CLD_ONOFF_ATTR_STARTUP_ONOFF

/* OnOff command dependencies on the attributes */
#ifdef CLD_ONOFF_CMD_ON_WITH_RECALL_GLOBAL_SCENE
    #ifndef CLD_ONOFF_ATTR_GLOBAL_SCENE_CONTROL
        #define CLD_ONOFF_ATTR_GLOBAL_SCENE_CONTROL
    #endif    
#endif

#ifdef CLD_ONOFF_CMD_ON_WITH_TIMED_OFF
    #ifndef CLD_ONOFF_ATTR_ON_TIME
        #define CLD_ONOFF_ATTR_ON_TIME
    #endif   
    #ifndef CLD_ONOFF_ATTR_OFF_WAIT_TIME
        #define CLD_ONOFF_ATTR_OFF_WAIT_TIME
    #endif   
#endif

/* Bit definitions in On with Timed Off command On/Off field */
#define CLD_ONOFF_OWTO_BIT_ACCEPT_ONLY_WHEN_ON  (1 << 0)

#ifndef CLD_ONOFF_CLUSTER_REVISION
    #define CLD_ONOFF_CLUSTER_REVISION                         1
#endif 

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* On/Off Command - Payload */
typedef enum 
{

    E_CLD_ONOFF_CMD_OFF                      = 0x00,     /* Mandatory */
    E_CLD_ONOFF_CMD_ON,                                  /* Mandatory */
    E_CLD_ONOFF_CMD_TOGGLE,                              /* Mandatory */

    E_CLD_ONOFF_CMD_OFF_EFFECT               = 0x40,
    E_CLD_ONOFF_CMD_ON_RECALL_GLOBAL_SCENE,
    E_CLD_ONOFF_CMD_ON_TIMED_OFF
} teCLD_OnOff_Command;


typedef enum 
{
    /* On/Off attribute set attribute ID's (3.8.2.2) */
    E_CLD_ONOFF_ATTR_ID_ONOFF                           = 0x0000,    /* Mandatory */
    E_CLD_ONOFF_ATTR_ID_GLOBAL_SCENE_CONTROL            = 0x4000,    /* Optional */
    E_CLD_ONOFF_ATTR_ID_ON_TIME,                                     /* Optional */
    E_CLD_ONOFF_ATTR_ID_OFF_WAIT_TIME,                               /* Optional */
    /* ZLO extension for OnOff Cluster    */             
    E_CLD_ONOFF_ATTR_ID_STARTUP_ONOFF,                               /* Optional */
} teCLD_OnOff_ClusterID;

/* StartUpOnOff */
typedef enum 
{
    E_CLD_ONOFF_STARTUPONOFF_OFF             = 0x00,
    E_CLD_ONOFF_STARTUPONOFF_ON,
    E_CLD_ONOFF_STARTUPONOFF_TOGGLE,
    E_CLD_ONOFF_STARTUPONOFF_PREVIOUS_VALUE  = 0xFF
} teCLD_OnOff_StartUpOnOff;

/* On/Off Cluster */
typedef struct
{

#ifdef ONOFF_SERVER
    zbool                   bOnOff;

#ifdef CLD_ONOFF_ATTR_GLOBAL_SCENE_CONTROL
    zbool                   bGlobalSceneControl;
#endif

#ifdef CLD_ONOFF_ATTR_ON_TIME
    zuint16                 u16OnTime;
#endif

#ifdef CLD_ONOFF_ATTR_OFF_WAIT_TIME
    zuint16                 u16OffWaitTime;
#endif
    
#ifdef CLD_ONOFF_ATTR_STARTUP_ONOFF
    /* ZLO extension for OnOff Cluster    */             
    zenum8                  eStartUpOnOff;
#endif

#ifdef CLD_ONOFF_ATTR_ATTRIBUTE_REPORTING_STATUS
    zenum8                  u8AttributeReportingStatus;
#endif

#endif
    zuint16                 u16ClusterRevision;

} tsCLD_OnOff;


#ifdef  CLD_ONOFF_CMD_OFF_WITH_EFFECT
/* off effect request command payload */
typedef struct
{
    zuint8                 u8EffectId;
    zuint8                 u8EffectVariant;
} tsCLD_OnOff_OffWithEffectRequestPayload;
#endif

#ifdef CLD_ONOFF_CMD_ON_WITH_TIMED_OFF
typedef struct
{
    zuint8                 u8OnOff;
    zuint16                u16OnTime;
    zuint16                u16OffTime;
} tsCLD_OnOff_OnWithTimedOffRequestPayload;
#endif

/* Custom data structure */
typedef struct
{
    uint8                   u8Dummy;
} tsCLD_OnOffCustomDataStructure;


/* Definition of On/Off Callback Event Structure */
typedef struct
{
    uint8                           u8CommandId;
    union
    {
    #ifdef  CLD_ONOFF_CMD_OFF_WITH_EFFECT
        tsCLD_OnOff_OffWithEffectRequestPayload     *psOffWithEffectRequestPayload;
    #endif 
    #ifdef CLD_ONOFF_CMD_ON_WITH_TIMED_OFF
        tsCLD_OnOff_OnWithTimedOffRequestPayload    *psOnWithTimedOffRequestPayload;
    #endif
#if (!defined CLD_ONOFF_CMD_OFF_WITH_EFFECT) && (!defined CLD_ONOFF_CMD_ON_WITH_TIMED_OFF)
        uint8 u8DummyByte;
#endif
    } uMessage;
} tsCLD_OnOffCallBackMessage;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_OnOffCreateOnOff(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits,
                tsCLD_OnOffCustomDataStructure     *psCustomDataStructure);

#ifdef ONOFF_CLIENT
PUBLIC teZCL_Status eCLD_OnOffCommandSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                teCLD_OnOff_Command   eCommand);

#ifdef CLD_ONOFF_CMD_OFF_WITH_EFFECT
PUBLIC teZCL_Status eCLD_OnOffCommandOffWithEffectSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_OnOff_OffWithEffectRequestPayload *psPayload);
#endif

#ifdef CLD_ONOFF_CMD_ON_WITH_TIMED_OFF
PUBLIC teZCL_Status eCLD_OnOffCommandOnWithTimedOffSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_OnOff_OnWithTimedOffRequestPayload *psPayload);
#endif
#endif /* ONOFF_CLIENT */

#if (defined ONOFF_SERVER) && (defined CLD_ONOFF_CMD_ON_WITH_TIMED_OFF)
PUBLIC teZCL_Status eCLD_OnOffUpdate(uint8 u8SourceEndPointId);
#endif
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern tsZCL_ClusterDefinition sCLD_OnOff;
extern const tsZCL_AttributeDefinition asCLD_OnOffClusterAttributeDefinitions[];
extern uint8 au8OnOffAttributeControlBits[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ONOFF_H */
