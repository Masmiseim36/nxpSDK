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


#ifndef BALLAST_CONFIGURATION_H
#define BALLAST_CONFIGURATION_H

/*!
\file       BallastConfiguration.h
\brief      Header for Ballast Configuration Cluster
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
#define LIGHTING_CLUSTER_ID_BALLAST_CONFIGURATION   0x0301

/****************************************************************************/
/*       Ballast Configuration Cluster - Optional Attributes                */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the simple metering cluster.                               */
/****************************************************************************/

    /* Ballast Information attribute set attribute ID's (5.3.2.2.1) */
#define CLD_BALLASTCONFIGURATION_ATTR_PHYSICAL_MIN_LEVEL
#define CLD_BALLASTCONFIGURATION_ATTR_PHYSICAL_MAX_LEVEL

    /* Ballast Settings attribute attribute ID's set (5.3.2.2.2) */
#define CLD_BALLASTCONFIGURATION_ATTR_MIN_LEVEL
#define CLD_BALLASTCONFIGURATION_ATTR_MAX_LEVEL
#define CLD_BALLASTCONFIGURATION_ATTR_POWER_ON_LEVEL
#define CLD_BALLASTCONFIGURATION_ATTR_POWER_ON_FADE_TIME
#define CLD_BALLASTCONFIGURATION_ATTR_INTRINSIC_BALLAST_FACTOR
#define CLD_BALLASTCONFIGURATION_ATTR_BALLAST_FACTOR_ADJUSTMENT

    /* Lamp Information attribute attribute ID's set (5.3.2.2.3) */
#define CLD_BALLASTCONFIGURATION_ATTR_LAMP_QUANTITY

    /* Lamp Settings attribute ID's set (5.3.2.2.4) */
#define CLD_BALLASTCONFIGURATION_ATTR_LAMP_TYPE
#define CLD_BALLASTCONFIGURATION_ATTR_LAMP_MANUFACTURER
#define CLD_BALLASTCONFIGURATION_ATTR_LAMP_RATED_HOURS
#define CLD_BALLASTCONFIGURATION_ATTR_LAMP_BURN_HOURS
#define CLD_BALLASTCONFIGURATION_ATTR_LAMP_ALARM_MODE
#define CLD_BALLASTCONFIGURATION_ATTR_LAMP_BURN_HOURS_TRIP_POINT

/* End of optional attributes */

/* Ballast Status Bitmask */
#define E_CLD_BC_BALLAST_STATUS_NON_OPERATIONAL_BIT     0
#define E_CLD_BC_BALLAST_STATUS_LAMP_NOT_IN_SOCKET_BIT  1

/* Lamp Alarm Mode Bitmask */
#define E_CLD_BC_LAMP_ALARM_MODE_LAMP_BURN_HOURS_BIT    0

#ifndef CLD_BALLASTCONFIGURATION_CLUSTER_REVISION
    #define CLD_BALLASTCONFIGURATION_CLUSTER_REVISION        1
#endif 

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Attribute ID's */
typedef enum 
{
    /* Ballast Information attribute set attribute ID's (5.3.2.2.1) */
    E_CLD_BALLASTCONFIGURATION_ATTR_PHYSICAL_MIN_LEVEL      = 0x0000,   /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_PHYSICAL_MAX_LEVEL,                 /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_BALLAST_STATUS,                     /* Mandatory */

    /* Ballast Settings attribute attribute ID's set (5.3.2.2.2) */
    E_CLD_BALLASTCONFIGURATION_ATTR_MIN_LEVEL               = 0x0010,   /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_MAX_LEVEL,                          /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_POWER_ON_LEVEL,                     /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_POWER_ON_FADE_TIME,                 /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_INTRINSIC_BALLAST_FACTOR,           /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_BALLAST_FACTOR_ADJUSTMENT,          /* Optional */

    /* Lamp Information attribute attribute ID's set (5.3.2.2.3) */
    E_CLD_BALLASTCONFIGURATION_ATTR_LAMP_QUANTITY           = 0x0020,   /* Optional */

    /* Lamp Settings attribute ID's set (5.3.2.2.4) */
    E_CLD_BALLASTCONFIGURATION_ATTR_LAMP_TYPE               = 0x0030,   /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_LAMP_MANUFACTURER,                  /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_LAMP_RATED_HOURS,                   /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_LAMP_BURN_HOURS,                    /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_LAMP_ALARM_MODE,                    /* Optional */
    E_CLD_BALLASTCONFIGURATION_ATTR_LAMP_BURN_HOURS_TRIP_POINT,         /* Optional */
} teCLD_BallastConfiguration_ClusterID;


/* Ballast Configuration Cluster */
typedef struct
{
#ifdef BALLAST_CONFIGURATION_SERVER
    /* Ballast Information attribute set attribute ID's (5.3.2.2.1) */
#ifdef CLD_BALLASTCONFIGURATION_ATTR_PHYSICAL_MIN_LEVEL
    zuint8                  u8PhysicalMinLevel;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_PHYSICAL_MAX_LEVEL
    zuint8                  u8PhysicalMaxLevel;
#endif

    zbmap8                  u8BallastStatus;

    /* Ballast Settings attribute attribute ID's set (5.3.2.2.2) */
#ifdef CLD_BALLASTCONFIGURATION_ATTR_MIN_LEVEL
    zuint8                  u8MinLevel;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_MAX_LEVEL
    zuint8                  u8MaxLevel;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_POWER_ON_LEVEL
    zuint8                  u8PowerOnLevel;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_POWER_ON_FADE_TIME
    zuint16                 u16PowerOnFadeTime;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_INTRINSIC_BALLAST_FACTOR
    zuint8                  u8IntrinsicBallastFactor;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_BALLAST_FACTOR_ADJUSTMENT
    zuint8                  u8BallastFactorAdjustment;
#endif

    /* Lamp Information attribute attribute ID's set (5.3.2.2.3) */
#ifdef CLD_BALLASTCONFIGURATION_ATTR_LAMP_QUANTITY
    zuint8                  u8LampQuantity;
#endif

    /* Lamp Settings attribute ID's set (5.3.2.2.4) */
#ifdef CLD_BALLASTCONFIGURATION_ATTR_LAMP_TYPE
    tsZCL_CharacterString   sLampType;
    uint8                   au8LampType[16];
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_LAMP_MANUFACTURER
    tsZCL_CharacterString   sLampManufacturer;
    uint8                   au8LampManufacturer[16];
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_LAMP_RATED_HOURS
    zuint24                 u32LampRatedHours;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_LAMP_BURN_HOURS
    zuint24                 u32LampBurnHours;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_LAMP_ALARM_MODE
    zbmap8                  u8LampAlarmMode;
#endif

#ifdef CLD_BALLASTCONFIGURATION_ATTR_LAMP_BURN_HOURS_TRIP_POINT
    zuint24                 u32LampBurnHoursTripPoint;
#endif
#endif
    zuint16                 u16ClusterRevision;

} tsCLD_BallastConfiguration;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_BallastConfigurationCreateBallastConfiguration(
                    tsZCL_ClusterInstance              *psClusterInstance,
                    bool_t                              bIsServer,
                    tsZCL_ClusterDefinition            *psClusterDefinition,
                    void                               *pvEndPointSharedStructPtr,
                    uint8                              *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZCL_ClusterDefinition sCLD_BallastConfiguration;
extern uint8 au8BallastConfigurationAttributeControlBits[];
extern const tsZCL_AttributeDefinition asCLD_BallastConfigurationClusterAttributeDefinitions[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* BALLAST_CONFIGURATION_H */
