/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef ILLUMINANCE_LEVEL_SENSING_H
#define ILLUMINANCE_LEVEL_SENSING_H

/*!
\file       IlluminanceLevelSensing.h
\brief      Header for Illuminance Level Sensing Cluster
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
#ifndef CLD_ILS_CLUSTER_REVISION
    #define CLD_ILS_CLUSTER_REVISION        1
#endif
/****************************************************************************/
/*           Illuminance Level Sensing Cluster - Optional Attributes        */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the illuminance Level Sensing cluster.                     */
/****************************************************************************/

/* Enable the optional Light Sensor Type attribute */
//#define CLD_ILS_ATTR_LIGHT_SENSOR_TYPE

/* Enable the optional Attribute Reporting Status attribute */
//#define CLD_ILS_ATTR_ATTRIBUTE_REPORTING_STATUS

/* End of optional attributes */


/* Cluster ID's */
#define MEASUREMENT_AND_SENSING_CLUSTER_ID_ILLUMINANCE_LEVEL_SENSING  0x0401


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum 
{
    /* Illuminance Level Sensing Information attribute set attribute ID's (4.3.2.3) */
    E_CLD_ILS_ATTR_ID_LEVEL_STATUS              = 0x0000,  /* Mandatory */
    E_CLD_ILS_ATTR_ID_LIGHT_SENSOR_TYPE,

    /* Illuminance Level Sensing Settings attribute set attribute ID's (4.3.2.4) */
    E_CLD_ILS_ATTR_ID_ILLUMINANCE_TARGET_LEVEL  = 0x0010,  /* Mandatory */
} teCLD_ILS_ClusterID;


/* Light sensor types */
typedef enum 
{
    E_CLD_ILS_LST_PHOTODIODE = 0,
    E_CLD_ILS_LST_CMOS
} teCLD_ILS_LightSensorType;


/* Light level status */
typedef enum 
{
    E_CLD_ILS_LLS_ON_TARGET,
    E_CLD_ILS_LLS_BELOW_TARGET,
    E_CLD_ILS_LLS_ABOVE_TARGET,
} teCLD_ILS_LightLevelStatus;


/* Illuminance Level Sensing Cluster */
typedef struct
{
#ifdef ILLUMINANCE_LEVEL_SENSING_SERVER    
    zenum8      u8LevelStatus;

#ifdef CLD_ILS_ATTR_LIGHT_SENSOR_TYPE
    zenum8      eLightSensorType;
#endif

    zuint16     u16IlluminanceTargetLevel;
    
#ifdef CLD_ILS_ATTR_ATTRIBUTE_REPORTING_STATUS
    zenum8      u8AttributeReportingStatus;
#endif
#endif
 
    zuint16     u16ClusterRevision;    
} tsCLD_IlluminanceLevelSensing;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_IlluminanceLevelSensingCreateIlluminanceLevelSensing(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8              *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZCL_ClusterDefinition sCLD_IlluminanceLevelSensing;
extern uint8 au8IlluminanceLevelSensingAttributeControlBits[];
extern const tsZCL_AttributeDefinition asCLD_IlluminanceLevelSensingClusterAttributeDefinitions[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ILLUMINANCE_LEVEL_SENSING_H */
