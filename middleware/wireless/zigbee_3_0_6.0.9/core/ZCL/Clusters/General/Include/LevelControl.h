/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef LEVEL_CONTROL_H
#define LEVEL_CONTROL_H

/*!
\file       LevelControl.h
\brief      Header for Level Control Cluster
*/
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/     
#include <jendefs.h>
#include "dlist.h"
#include "zcl.h"
#include "zcl_customcommand.h"
#include "zcl_options.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Cluster ID's */
#define GENERAL_CLUSTER_ID_LEVEL_CONTROL            0x0008

/****************************************************************************/
/*             Level Control Cluster - Optional Attributes                  */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the level control cluster.                                 */
/****************************************************************************/

/* Enable the optional Remaining Time attribute */
//#define CLD_LEVELCONTROL_ATTR_REMAINING_TIME

/* Enable the optional On/Off Transition Time attribute */
//#define CLD_LEVELCONTROL_ATTR_ON_OFF_TRANSITION_TIME

/* Enable the optional On Level attribute */
//#define CLD_LEVELCONTROL_ATTR_ON_LEVEL

/* Enable the optional On Transition Time attribute */
//#define CLD_LEVELCONTROL_ATTR_ON_TRANSITION_TIME

/* Enable the optional Off Transition Time attribute */
//#define CLD_LEVELCONTROL_ATTR_OFF_TRANSITION_TIME

/* Enable the optional Default Move Rate attribute */
//#define CLD_LEVELCONTROL_ATTR_DEFAULT_MOVE_RATE

/* Enable the optional Attribute reporting status attribute */
//#define CLD_LEVELCONTROL_ATTR_ATTRIBUTE_REPORTING_STATUS

/* Enable the optional Attribute StartUpCurrentLevel  for ZLO extension*/
//#define CLD_LEVELCONTROL_ATTR_STARTUP_CURRENT_LEVEL

#ifndef CLD_LEVELCONTROL_MIN_LEVEL
#define CLD_LEVELCONTROL_MIN_LEVEL                  (1)
#endif

#ifndef CLD_LEVELCONTROL_MAX_LEVEL
#define CLD_LEVELCONTROL_MAX_LEVEL                  (0xfe)
#endif

#ifndef CLD_LEVELCONTROL_CLUSTER_REVISION
    #define CLD_LEVELCONTROL_CLUSTER_REVISION         1
#endif 

#ifndef CLD_LEVELCONTROL_MIN_FREQ
    #define CLD_LEVELCONTROL_MIN_FREQ             (0x0001)
#endif

#ifndef CLD_LEVELCONTROL_MAX_FREQ
    #define CLD_LEVELCONTROL_MAX_FREQ              (0xFFFE)
#endif

#define LEVELCONTROL_EXECUTE_IF_OFF_BIT              (1<<0)
#define LEVELCONTROL_COUPLE_COLOUR_TEMP_TO_LEVEL_BIT (1<<1)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Command codes */
typedef enum 
{
    E_CLD_LEVELCONTROL_CMD_MOVE_TO_LEVEL            = 0x00,    /* Mandatory */
    E_CLD_LEVELCONTROL_CMD_MOVE,                            /* Mandatory */
    E_CLD_LEVELCONTROL_CMD_STEP,                            /* Mandatory */
    E_CLD_LEVELCONTROL_CMD_STOP,                            /* Mandatory */
    E_CLD_LEVELCONTROL_CMD_MOVE_TO_LEVEL_WITH_ON_OFF,       /* Mandatory */
    E_CLD_LEVELCONTROL_CMD_MOVE_WITH_ON_OFF,                /* Mandatory */
    E_CLD_LEVELCONTROL_CMD_STEP_WITH_ON_OFF,                /* Mandatory */
    E_CLD_LEVELCONTROL_CMD_STOP_WITH_ON_OFF,                /* Mandatory */
    E_CLD_LEVELCONTROL_CMD_MOVE_TO_CLOSEST_FREQ,            /*Optional*/

    /* These are not actual commands, they are used as extra states in the level control update function */
    E_CLD_LEVELCONTROL_CMD_NONE
} teCLD_LevelControl_Command;


/* Attribute ID's */
typedef enum 
{
    /* LevelControl attribute set attribute ID's (3.10.2.2) */
    E_CLD_LEVELCONTROL_ATTR_ID_CURRENT_LEVEL                    = 0x0000,  /* Mandatory */
    E_CLD_LEVELCONTROL_ATTR_ID_REMAINING_TIME,
    E_CLD_LEVELCONTROL_ATTR_ID_MIN_LEVEL,
    E_CLD_LEVELCONTROL_ATTR_ID_MAX_LEVEL,
    E_CLD_LEVELCONTROL_ATTR_ID_CURRENT_FREQUENCY,
    E_CLD_LEVELCONTROL_ATTR_ID_MIN_FREQUENCY,
    E_CLD_LEVELCONTROL_ATTR_ID_MAX_FREQUENCY,
    E_CLD_LEVELCONTROL_ATTR_ID_OPTIONS                          = 0x000F,  /* Mandatory */
    E_CLD_LEVELCONTROL_ATTR_ID_ON_OFF_TRANSITION_TIME           = 0x010,
    E_CLD_LEVELCONTROL_ATTR_ID_ON_LEVEL,
    E_CLD_LEVELCONTROL_ATTR_ID_ON_TRANSITION_TIME,
    E_CLD_LEVELCONTROL_ATTR_ID_OFF_TRANSITION_TIME,
    E_CLD_LEVELCONTROL_ATTR_ID_DEFAULT_MOVE_RATE,
    E_CLD_LEVELCONTROL_ATTR_ID_STARTUP_CURRENT_LEVEL           = 0x4000, /* Optional */
} teCLD_LevelControl_ClusterID;


/* Move Modes */
typedef enum 
{
    E_CLD_LEVELCONTROL_MOVE_MODE_UP = 0x0,
    E_CLD_LEVELCONTROL_MOVE_MODE_DOWN
} teCLD_LevelControl_MoveMode;


typedef enum 
{
    E_CLD_LEVELCONTROL_TRANSITION_MOVE_TO_LEVEL = 0,
    E_CLD_LEVELCONTROL_TRANSITION_MOVE,
    E_CLD_LEVELCONTROL_TRANSITION_STEP,
    E_CLD_LEVELCONTROL_TRANSITION_STOP,
    E_CLD_LEVELCONTROL_TRANSITION_ON,
    E_CLD_LEVELCONTROL_TRANSITION_OFF,
    E_CLD_LEVELCONTROL_TRANSITION_OFF_WITH_EFFECT_DIM_DOWN_FADE_OFF,
    E_CLD_LEVELCONTROL_TRANSITION_OFF_WITH_EFFECT_DIM_UP_FADE_OFF,
    E_CLD_LEVELCONTROL_TRANSITION_NONE,
} teCLD_LevelControl_Transition;

#ifdef LEVEL_CONTROL_SERVER
/* Level Control Cluster */
typedef struct
{
    zuint8                  u8CurrentLevel;
   
#ifdef CLD_LEVELCONTROL_ATTR_REMAINING_TIME
    zuint16                 u16RemainingTime;
#endif

#ifdef CLD_LEVELCONTROL_ATTR_MIN_LEVEL
    zuint8                  u8MinLevel;
#endif
    
#ifdef CLD_LEVELCONTROL_ATTR_MAX_LEVEL
    zuint8                  u8MaxLevel;
#endif

#ifdef CLD_LEVELCONTROL_ATTR_CURRENT_FREQUENCY
    zuint16                 u16CurrentFrequency;
#endif    
    
#ifdef CLD_LEVELCONTROL_ATTR_CURRENT_FREQUENCY
    zuint16                 u16MinFrequency;
#endif 

#ifdef CLD_LEVELCONTROL_ATTR_CURRENT_FREQUENCY
    zuint16                 u16MaxFrequency;
#endif     
    
    zbmap8                  u8Options;
    
#ifdef CLD_LEVELCONTROL_ATTR_ON_OFF_TRANSITION_TIME
    zuint16                 u16OnOffTransitionTime;
#endif

#ifdef CLD_LEVELCONTROL_ATTR_ON_LEVEL
    zuint8                  u8OnLevel;
#endif

#ifdef CLD_LEVELCONTROL_ATTR_ON_TRANSITION_TIME
    zuint16                 u16OnTransitionTime;
#endif

#ifdef CLD_LEVELCONTROL_ATTR_OFF_TRANSITION_TIME
    zuint16                 u16OffTransitionTime;
#endif

#ifdef CLD_LEVELCONTROL_ATTR_DEFAULT_MOVE_RATE
    zuint8                  u8DefaultMoveRate;
#endif

#ifdef CLD_LEVELCONTROL_ATTR_STARTUP_CURRENT_LEVEL
    zuint8                  u8StartUpCurrentLevel;
#endif

#ifdef CLD_LEVELCONTROL_ATTR_ATTRIBUTE_REPORTING_STATUS
    zenum8                  u8AttributeReportingStatus;
#endif
    zuint16                 u16ClusterRevision;

} tsCLD_LevelControl;
#endif

#ifdef LEVEL_CONTROL_CLIENT
/* Level Control Cluster */
typedef struct
{
    zuint16                 u16ClusterRevision;
} tsCLD_LevelControlClient;
#endif

/* Move to level command payload */
typedef struct
{
    uint8                   u8Level;
    uint16                  u16TransitionTime;
    zbmap8                  u8OptionsMask;
    zbmap8                  u8OptionsOverride;
} tsCLD_LevelControl_MoveToLevelCommandPayload;

/* Move command payload */
typedef struct
{
    uint8                   u8MoveMode;
    uint8                   u8Rate;
    zbmap8                  u8OptionsMask;
    zbmap8                  u8OptionsOverride;    
} tsCLD_LevelControl_MoveCommandPayload;


/* Step command payload */
typedef struct
{
    uint8                   u8StepMode;
    uint8                   u8StepSize;
    uint16                  u16TransitionTime;
    zbmap8                  u8OptionsMask;
    zbmap8                  u8OptionsOverride;    
} tsCLD_LevelControl_StepCommandPayload;

/* Stop command payload */
typedef struct
{
    zbmap8                  u8OptionsMask;
    zbmap8                  u8OptionsOverride;    
} tsCLD_LevelControl_StopCommandPayload;

/*Move to Closest Frequency command payload*/
typedef struct
{
    uint16                  u16Frequency;
} tsCLD_LevelControl_MoveToClosestFreqCommandPayload;


/* Definition of On-Off Call back Event Structure */
typedef struct
{
    uint8                                       u8CommandId;
    union
    {
        tsCLD_LevelControl_MoveToLevelCommandPayload    *psMoveToLevelCommandPayload;
        tsCLD_LevelControl_MoveCommandPayload           *psMoveCommandPayload;
        tsCLD_LevelControl_StepCommandPayload           *psStepCommandPayload;
        tsCLD_LevelControl_StopCommandPayload           *psStopCommandPayload;
        #ifdef CLD_LEVELCONTROL_ATTR_CURRENT_FREQUENCY
        tsCLD_LevelControl_MoveToClosestFreqCommandPayload     *psMoveToClosestFreqCommandPayload;
        #endif
    } uMessage;
} tsCLD_LevelControlCallBackMessage;


/* Transition related data */
typedef struct
{
    teCLD_LevelControl_Transition   eTransition;
    teCLD_LevelControl_MoveMode     eMode;
    bool_t                          bWithOnOff;
    int32_t                         iCurrentLevel;
    int32_t                         iTargetLevel;
    int32_t                         iPreviousLevel;
    int32_t                         iStepSize;
    uint32_t                        u32Time;
    #ifdef CLD_LEVELCONTROL_ATTR_CURRENT_FREQUENCY
    uint16_t                        u16TargetFrequency;
    #endif    
} tsCLD_LevelControl_Transition;


/* Custom data structure */
typedef struct
{
    tsCLD_LevelControl_Transition       sTransition;
    tsZCL_ReceiveEventAddress             sReceiveEventAddress;
    tsZCL_CallBackEvent                 sCustomCallBackEvent;
    tsCLD_LevelControlCallBackMessage   sCallBackMessage;
} tsCLD_LevelControlCustomDataStructure;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_LevelControlCreateLevelControl(
                    tsZCL_ClusterInstance              *psClusterInstance,
                    bool_t                              bIsServer,
                    tsZCL_ClusterDefinition            *psClusterDefinition,
                    void                               *pvEndPointSharedStructPtr,
                    uint8              *pu8AttributeControlBits,
                    tsCLD_LevelControlCustomDataStructure  *psCustomDataStructure);

PUBLIC teZCL_Status eCLD_LevelControlUpdate(
                    uint8                   u8SourceEndPointId);

PUBLIC teZCL_Status eCLD_LevelControlSetLevel(
                    uint8                   u8SourceEndPointId,
                    uint8                   u8Level,
                    uint16                  u16TransitionTime);

PUBLIC teZCL_Status eCLD_LevelControlStopTransition(
        uint8                   u8SourceEndPointId);

PUBLIC teZCL_Status eCLD_LevelControlGetLevel(
                    uint8                   u8SourceEndPointId,
                    uint8                   *pu8Level);

PUBLIC teZCL_Status eCLD_LevelControlClusterIsPresent(
                    uint8                   u8SourceEndPointId);

PUBLIC teZCL_Status eCLD_LevelControlCommandMoveToLevelCommandSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    bool_t                      bWithOnOff,
                    tsCLD_LevelControl_MoveToLevelCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_LevelControlCommandMoveCommandSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    bool_t                      bWithOnOff,
                    tsCLD_LevelControl_MoveCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_LevelControlCommandStepCommandSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    bool_t                      bWithOnOff,
                    tsCLD_LevelControl_StepCommandPayload *psPayload);

PUBLIC  teZCL_Status eCLD_LevelControlCommandStopCommandSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    bool_t                      bWithOnOff,
                    tsCLD_LevelControl_StopCommandPayload *psPayload);

PUBLIC  teZCL_Status eCLD_LevelControlCommandMoveToClosestFreqCommandSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_LevelControl_MoveToClosestFreqCommandPayload *psPayload);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

#ifdef LEVEL_CONTROL_SERVER
    extern tsZCL_ClusterDefinition sCLD_LevelControl;
    extern const tsZCL_AttributeDefinition asCLD_LevelControlClusterAttributeDefinitions[];
    extern uint8 au8LevelControlAttributeControlBits[];
#endif

#ifdef LEVEL_CONTROL_CLIENT
    extern tsZCL_ClusterDefinition sCLD_LevelControlClient;
    extern const tsZCL_AttributeDefinition asCLD_LevelControlClientClusterAttributeDefinitions[];
    extern uint8 au8LevelControlClientAttributeControlBits[];
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* LEVEL_CONTROL_H */
