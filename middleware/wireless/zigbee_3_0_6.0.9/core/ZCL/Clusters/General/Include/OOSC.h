/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef OOSC_H
#define OOSC_H

/*!
\file       OOSC.h
\brief      Header for On/Off Switch Configuration Cluster
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
#define GENERAL_CLUSTER_ID_ONOFF_SWITCH_CONFIGURATION   0x0007

#ifndef CLD_OOSC_CLUSTER_REVISION
    #define CLD_OOSC_CLUSTER_REVISION                         1
#endif 

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* On/Off switch configuration attribute ID's (3.9.2.2) */
typedef enum 
{
    E_CLD_OOSC_ATTR_ID_SWITCH_TYPE          = 0x0000,   /* Mandatory */
    E_CLD_OOSC_ATTR_ID_SWITCH_ACTIONS       = 0x0010,   /* Mandatory */
} teCLD_OOSC_ClusterID;


/* On/Off switch types */
typedef enum 
{
    E_CLD_OOSC_TYPE_TOGGLE,
    E_CLD_OOSC_TYPE_MOMENTARY,
    E_CLD_OOSC_TYPE_MULTI_FUNCTION
} teCLD_OOSC_SwitchType;


/* On/Off switch actions */
typedef enum 
{
    E_CLD_OOSC_ACTION_S2ON_S1OFF,
    E_CLD_OOSC_ACTION_S2OFF_S1ON,
    E_CLD_OOSC_ACTION_TOGGLE
} teCLD_OOSC_SwitchAction;


/* On/Off Switch Configuration Cluster */
typedef struct
{
#ifdef OOSC_SERVER    
    zenum8                  eSwitchType;                /* Mandatory */
    zenum8                  eSwitchActions;             /* Mandatory */
#endif    
    zuint16                 u16ClusterRevision;
} tsCLD_OOSC;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_OOSCCreateOnOffSwitchConfig(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8              *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZCL_ClusterDefinition sCLD_OOSC;
extern uint8 au8OOSCAttributeControlBits[];
extern const tsZCL_AttributeDefinition asCLD_OOSCClusterAttributeDefinitions[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* OOSC_H */
