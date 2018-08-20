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


#ifndef DIMMABLE_BALLAST_H
#define DIMMABLE_BALLAST_H

/*!
\file       dimmable_ballast.h
\brief      Header for ZigBee Dimmable Ballast functions
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/    
#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"
#include "Basic.h"
#include "Identify.h"
#include "OnOff.h"
#include "PowerConfiguration.h"
#include "DeviceTemperatureConfiguration.h"
#include "LevelControl.h"
#include "Groups.h"
#include "Scenes.h"
#include "IlluminanceLevelSensing.h"
#include "IlluminanceMeasurement.h"
#include "OccupancySensing.h"
#ifdef CLD_BALLAST_CONFIGURATION
    #include "BallastConfiguration.h"
#endif

#ifdef CLD_ZLL_COMMISSION
    #include "zll_commission.h"
#endif

#ifdef CLD_OTA
    #include "OTA.h"
#endif
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Holds cluster instances */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma pack(push, 4)
typedef struct 
{
    /* All ZLO devices have 2 mandatory clusters - Basic(server) and Identify(server) */
    #if (defined CLD_BASIC) && (defined BASIC_SERVER)
        tsZCL_ClusterInstance sBasicServer;
    #endif

    #if (defined CLD_IDENTIFY) && (defined IDENTIFY_SERVER)
        tsZCL_ClusterInstance sIdentifyServer;
    #endif

    /* On/Off ballast has other 7 mandatory clusters */
    #if (defined CLD_ONOFF) && (defined ONOFF_SERVER)
        tsZCL_ClusterInstance sOnOffServer;
    #endif
    
    #if (defined CLD_LEVEL_CONTROL) && (defined LEVEL_CONTROL_SERVER)
        tsZCL_ClusterInstance sLevelControlServer;
    #endif
    
    #if (defined CLD_SCENES) && (defined SCENES_SERVER)
        tsZCL_ClusterInstance sScenesServer;
    #endif

    #if (defined CLD_GROUPS) && (defined GROUPS_SERVER)
        tsZCL_ClusterInstance sGroupsServer;
    #endif

    /* Optional server clusters */
    #if (defined CLD_POWER_CONFIGURATION) && (defined POWER_CONFIGURATION_SERVER)
        tsZCL_ClusterInstance sPowerConfigurationServer;
    #endif

    #if (defined CLD_DEVICE_TEMPERATURE_CONFIGURATION) && (defined DEVICE_TEMPERATURE_CONFIGURATION_SERVER)
        tsZCL_ClusterInstance sDeviceTemperatureConfigurationServer;
    #endif

    #if (defined CLD_BALLAST_CONFIGURATION) && (defined BALLAST_CONFIGURATION_SERVER)
        tsZCL_ClusterInstance sBallastConfigurationServer;
    #endif

    /* Recommended Optional Server clusters */
    #if (defined CLD_ILLUMINANCE_LEVEL_SENSING) && (defined ILLUMINANCE_LEVEL_SENSING_SERVER)
        tsZCL_ClusterInstance sIlluminanceLevelSensingServer;
    #endif
    
    #if (defined CLD_ZLL_COMMISSION) && (defined ZLL_COMMISSION_SERVER)
        tsZCL_ClusterInstance sZllCommissionServer;
    #endif
    
    /* Recommended Optional Client clusters */
    #if (defined CLD_OTA) && (defined OTA_CLIENT)
        tsZCL_ClusterInstance sOTAClient;
    #endif
    
    #if (defined CLD_ILLUMINANCE_MEASUREMENT) && (defined ILLUMINANCE_MEASUREMENT_CLIENT)
        tsZCL_ClusterInstance sIlluminanceMeasurementClient;
    #endif
    
    #if (defined CLD_ILLUMINANCE_LEVEL_SENSING) && (defined ILLUMINANCE_LEVEL_SENSING_CLIENT)
        tsZCL_ClusterInstance sIlluminanceLevelSensingClient;
    #endif
       
    #if (defined CLD_OCCUPANCY_SENSING) && (defined OCCUPANCY_SENSING_CLIENT)
        tsZCL_ClusterInstance sOccupancySensingClient;
    #endif
} tsZLO_DimmableBallastDeviceClusterInstances;
#pragma pack(pop)
#else  
typedef struct 
{
    /* All ZLO devices have 2 mandatory clusters - Basic(server) and Identify(server) */
    #if (defined CLD_BASIC) && (defined BASIC_SERVER)
        tsZCL_ClusterInstance sBasicServer;
    #endif

    #if (defined CLD_IDENTIFY) && (defined IDENTIFY_SERVER)
        tsZCL_ClusterInstance sIdentifyServer;
    #endif

    /* On/Off ballast has other 7 mandatory clusters */
    #if (defined CLD_ONOFF) && (defined ONOFF_SERVER)
        tsZCL_ClusterInstance sOnOffServer;
    #endif
    
    #if (defined CLD_LEVEL_CONTROL) && (defined LEVEL_CONTROL_SERVER)
        tsZCL_ClusterInstance sLevelControlServer;
    #endif
    
    #if (defined CLD_SCENES) && (defined SCENES_SERVER)
        tsZCL_ClusterInstance sScenesServer;
    #endif

    #if (defined CLD_GROUPS) && (defined GROUPS_SERVER)
        tsZCL_ClusterInstance sGroupsServer;
    #endif

    /* Optional server clusters */
    #if (defined CLD_POWER_CONFIGURATION) && (defined POWER_CONFIGURATION_SERVER)
        tsZCL_ClusterInstance sPowerConfigurationServer;
    #endif

    #if (defined CLD_DEVICE_TEMPERATURE_CONFIGURATION) && (defined DEVICE_TEMPERATURE_CONFIGURATION_SERVER)
        tsZCL_ClusterInstance sDeviceTemperatureConfigurationServer;
    #endif

    #if (defined CLD_BALLAST_CONFIGURATION) && (defined BALLAST_CONFIGURATION_SERVER)
        tsZCL_ClusterInstance sBallastConfigurationServer;
    #endif

    /* Recommended Optional Server clusters */
    #if (defined CLD_ILLUMINANCE_LEVEL_SENSING) && (defined ILLUMINANCE_LEVEL_SENSING_SERVER)
        tsZCL_ClusterInstance sIlluminanceLevelSensingServer;
    #endif
    
    #if (defined CLD_ZLL_COMMISSION) && (defined ZLL_COMMISSION_SERVER)
        tsZCL_ClusterInstance sZllCommissionServer;
    #endif
    
    /* Recommended Optional Client clusters */
    #if (defined CLD_OTA) && (defined OTA_CLIENT)
        tsZCL_ClusterInstance sOTAClient;
    #endif
    
    #if (defined CLD_ILLUMINANCE_MEASUREMENT) && (defined ILLUMINANCE_MEASUREMENT_CLIENT)
        tsZCL_ClusterInstance sIlluminanceMeasurementClient;
    #endif
    
    #if (defined CLD_ILLUMINANCE_LEVEL_SENSING) && (defined ILLUMINANCE_LEVEL_SENSING_CLIENT)
        tsZCL_ClusterInstance sIlluminanceLevelSensingClient;
    #endif
       
    #if (defined CLD_OCCUPANCY_SENSING) && (defined OCCUPANCY_SENSING_CLIENT)
        tsZCL_ClusterInstance sOccupancySensingClient;
    #endif
} tsZLO_DimmableBallastDeviceClusterInstances __attribute__ ((aligned(4)));
#endif


/* Holds everything required to create an instance of a door lock */
typedef struct
{
    tsZCL_EndPointDefinition sEndPoint;

    /* Cluster instances */
    tsZLO_DimmableBallastDeviceClusterInstances sClusterInstance;

    /* Mandatory server clusters */
    #if (defined CLD_BASIC) && (defined BASIC_SERVER)
        /* Basic Cluster - Server */
        tsCLD_Basic sBasicServerCluster;
    #endif

    #if (defined CLD_IDENTIFY) && (defined IDENTIFY_SERVER)
        /* Identify Cluster - Server */
        tsCLD_Identify sIdentifyServerCluster;
        tsCLD_IdentifyCustomDataStructure sIdentifyServerCustomDataStructure;
    #endif

    #if (defined CLD_ONOFF) && (defined ONOFF_SERVER)
        /* On/Off Cluster - Server */
        tsCLD_OnOff sOnOffServerCluster;
        tsCLD_OnOffCustomDataStructure     sOnOffServerCustomDataStructure;
    #endif
    
    #if (defined CLD_LEVEL_CONTROL) && (defined LEVEL_CONTROL_SERVER)
        /* LevelControl Cluster - Server */
        tsCLD_LevelControl sLevelControlServerCluster;
        tsCLD_LevelControlCustomDataStructure sLevelControlServerCustomDataStructure;
    #endif
    
    #if (defined CLD_SCENES) && (defined SCENES_SERVER)
        /* Scenes Cluster - Server */
        tsCLD_Scenes sScenesServerCluster;
        tsCLD_ScenesCustomDataStructure sScenesServerCustomDataStructure;
    #endif

    #if (defined CLD_GROUPS) && (defined GROUPS_SERVER)
        /* Groups Cluster - Server */
        tsCLD_Groups sGroupsServerCluster;
        tsCLD_GroupsCustomDataStructure sGroupsServerCustomDataStructure;
    #endif

    #if (defined CLD_POWER_CONFIGURATION) && (defined POWER_CONFIGURATION_SERVER)
        /* Power Configuration Cluster - Server */
        tsCLD_PowerConfiguration sPowerConfigServerCluster;
    #endif

    #if (defined CLD_DEVICE_TEMPERATURE_CONFIGURATION) && (defined DEVICE_TEMPERATURE_CONFIGURATION_SERVER)
        /* Device Temperature Configuration Cluster - Server */
        tsCLD_DeviceTemperatureConfiguration sDeviceTemperatureConfigurationServerCluster;
    #endif

    #if (defined CLD_BALLAST_CONFIGURATION) && (defined BALLAST_CONFIGURATION_SERVER)
        tsCLD_BallastConfiguration sBallastConfigurationServerCluster;
    #endif

    /* Recommended Optional server clusters */
    #if (defined CLD_ILLUMINANCE_LEVEL_SENSING) && (defined ILLUMINANCE_LEVEL_SENSING_SERVER)
        tsCLD_IlluminanceLevelSensing sIlluminanceLevelSensingServerCluster;
    #endif
    
    #if (defined CLD_ZLL_COMMISSION) && (defined ZLL_COMMISSION_SERVER)
        tsCLD_ZllCommission                         sZllCommissionServerCluster;
        tsCLD_ZllCommissionCustomDataStructure      sZllCommissionServerCustomDataStructure;
    #endif
    
    /*Recommended Optional client clusters */
    #if (defined CLD_OTA) && (defined OTA_CLIENT)
        /* OTA cluster - Client */
        tsCLD_AS_Ota sCLD_OTA;
        tsOTA_Common sCLD_OTA_CustomDataStruct;
    #endif
    
    #if (defined CLD_ILLUMINANCE_MEASUREMENT) && (defined ILLUMINANCE_MEASUREMENT_CLIENT)
        /* Illuminance Measurement Cluster - Client */
        tsCLD_IlluminanceMeasurement sIlluminanceMeasurementClientCluster;
    #endif

    #if (defined CLD_ILLUMINANCE_LEVEL_SENSING) && (defined ILLUMINANCE_LEVEL_SENSING_CLIENT)
        tsCLD_IlluminanceLevelSensing sIlluminanceLevelSensingClientCluster;
    #endif
    
    #if (defined CLD_OCCUPANCY_SENSING) && (defined OCCUPANCY_SENSING_CLIENT)
        /* Occupancy Sensing Cluster - Client */
        tsCLD_OccupancySensing sOccupancySensingClientCluster;
    #endif
    
} tsZLO_DimmableBallastDevice;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eZLO_RegisterDimmableBallastEndPoint(uint8 u8EndPointIdentifier,
                                              tfpZCL_ZCLCallBackFunction cbCallBack,
                                              tsZLO_DimmableBallastDevice *psDeviceInfo);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* DIMMABLE_BALLAST_H */
