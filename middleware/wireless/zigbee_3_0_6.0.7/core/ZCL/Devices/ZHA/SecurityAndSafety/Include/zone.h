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


#ifndef ZONE_H
#define ZONE_H

/*!
\file       zone.h
\brief      Header for ZigBee IAS Zone profile functions
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

/*Mandatory General Clusters*/
#include "Basic.h"
#include "Identify.h"

/*Optional Servers */
#ifdef CLD_POWER_CONFIGURATION
    #include "PowerConfiguration.h"
#endif

#ifdef CLD_DEVICE_TEMPERATURE_CONFIGURATION
    #include "DeviceTemperatureConfiguration.h"
#endif

#ifdef CLD_ALARMS
    #include "Alarms.h"
#endif

#ifdef CLD_POLL_CONTROL
    #include "PollControl.h"
#endif

/*Optional Clients */
#ifdef CLD_OTA
    #include "OTA.h"
#endif

#ifdef CLD_TIME
    #include "ZclTime.h"
#endif


/*EZ Mode Commisioning supported Clusters */
#include "Groups.h"
#include "Scenes.h"

/*IAS Zone cluster*/
#include "IASZone.h"
#ifdef CLD_ELECTRICAL_MEASUREMENT
	#include "ElectricalMeasurement.h"
#endif
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define SECURITY_AND_SAFETY_IAS_ZONE_DEVICE_ID                          0x0402
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Holds cluster instances */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma pack(push, 4)
typedef struct 
{
    /* All HA devices have 2 mandatory clusters - Basic(server) and Identify(server) */
#if (defined CLD_BASIC) && (defined BASIC_SERVER)
    tsZCL_ClusterInstance sBasicServer;
#endif

#if (defined CLD_IDENTIFY) && (defined IDENTIFY_SERVER)
    tsZCL_ClusterInstance sIdentifyServer;
#endif

#if (defined CLD_IASZONE) && (defined IASZONE_SERVER)
    tsZCL_ClusterInstance sIASZoneServer;
#endif

    /* Optional Clusters Common to All HA Server Devices */
#if (defined CLD_POWER_CONFIGURATION) && (defined POWER_CONFIGURATION_SERVER)
    tsZCL_ClusterInstance sPowerConfigurationServer;
#endif

#if (defined CLD_DEVICE_TEMPERATURE_CONFIGURATION) && (defined DEVICE_TEMPERATURE_CONFIGURATION_SERVER)
    tsZCL_ClusterInstance sDeviceTemperatureConfigurationServer;
#endif

#if (defined CLD_ALARMS) && (defined ALARMS_SERVER)
    tsZCL_ClusterInstance sAlarmsServer;
#endif

#if (defined CLD_POLL_CONTROL) && (defined POLL_CONTROL_SERVER)
    tsZCL_ClusterInstance sPollControlServer;
#endif

#if (defined CLD_SCENES) && (defined SCENES_SERVER)
    tsZCL_ClusterInstance sScenesServer;
#endif

#if (defined CLD_GROUPS) && (defined GROUPS_SERVER)
    tsZCL_ClusterInstance sGroupsServer;
#endif

    /* Optional Clusters Common to All HA Client Devices */
#if (defined CLD_SCENES) && (defined SCENES_CLIENT)
    tsZCL_ClusterInstance sScenesClient;
#endif

#if (defined CLD_GROUPS) && (defined GROUPS_CLIENT)
    tsZCL_ClusterInstance sGroupsClient;
#endif
    
    /* Optional Clusters supported by Server Device */

    /* Optional Clusters supported by Client Device */
#ifdef CLD_OTA
    #ifdef OTA_CLIENT
    /* Add  cluster instance for the OTA cluster */
    tsZCL_ClusterInstance sOTAClient;
    #endif
    
    #ifdef OTA_SERVER
    /* Add  server instance for the OTA cluster */
    tsZCL_ClusterInstance sOTAServer;    
    #endif
#endif

#if (defined CLD_TIME) && (defined TIME_CLIENT)
    tsZCL_ClusterInstance sTimeClient;
#endif

    /* Optional client clusters */
  
#if (defined CLD_IDENTIFY) && (defined IDENTIFY_CLIENT)
    tsZCL_ClusterInstance sIdentifyClient;
#endif

#if (defined CLD_POLL_CONTROL) && (defined POLL_CONTROL_CLIENT)
    tsZCL_ClusterInstance sPollControlClient;
#endif


#if (defined CLD_ELECTRICAL_MEASUREMENT && defined ELECTRICAL_MEASUREMENT_SERVER)
    tsZCL_ClusterInstance sElectricalMeasurementServer;
#endif
} tsHA_IASZoneDeviceClusterInstances;
#pragma pack(pop)
#else  
typedef struct 
{
    /* All HA devices have 2 mandatory clusters - Basic(server) and Identify(server) */
#if (defined CLD_BASIC) && (defined BASIC_SERVER)
    tsZCL_ClusterInstance sBasicServer;
#endif

#if (defined CLD_IDENTIFY) && (defined IDENTIFY_SERVER)
    tsZCL_ClusterInstance sIdentifyServer;
#endif

#if (defined CLD_IASZONE) && (defined IASZONE_SERVER)
    tsZCL_ClusterInstance sIASZoneServer;
#endif

    /* Optional Clusters Common to All HA Server Devices */
#if (defined CLD_POWER_CONFIGURATION) && (defined POWER_CONFIGURATION_SERVER)
    tsZCL_ClusterInstance sPowerConfigurationServer;
#endif

#if (defined CLD_DEVICE_TEMPERATURE_CONFIGURATION) && (defined DEVICE_TEMPERATURE_CONFIGURATION_SERVER)
    tsZCL_ClusterInstance sDeviceTemperatureConfigurationServer;
#endif

#if (defined CLD_ALARMS) && (defined ALARMS_SERVER)
    tsZCL_ClusterInstance sAlarmsServer;
#endif

#if (defined CLD_POLL_CONTROL) && (defined POLL_CONTROL_SERVER)
    tsZCL_ClusterInstance sPollControlServer;
#endif

#if (defined CLD_SCENES) && (defined SCENES_SERVER)
    tsZCL_ClusterInstance sScenesServer;
#endif

#if (defined CLD_GROUPS) && (defined GROUPS_SERVER)
    tsZCL_ClusterInstance sGroupsServer;
#endif

    /* Optional Clusters Common to All HA Client Devices */
#if (defined CLD_SCENES) && (defined SCENES_CLIENT)
    tsZCL_ClusterInstance sScenesClient;
#endif

#if (defined CLD_GROUPS) && (defined GROUPS_CLIENT)
    tsZCL_ClusterInstance sGroupsClient;
#endif
    
    /* Optional Clusters supported by Server Device */

    /* Optional Clusters supported by Client Device */
#ifdef CLD_OTA
    #ifdef OTA_CLIENT
    /* Add  cluster instance for the OTA cluster */
    tsZCL_ClusterInstance sOTAClient;
    #endif
    
    #ifdef OTA_SERVER
    /* Add  server instance for the OTA cluster */
    tsZCL_ClusterInstance sOTAServer;    
    #endif
#endif

#if (defined CLD_TIME) && (defined TIME_CLIENT)
    tsZCL_ClusterInstance sTimeClient;
#endif

    /* Optional client clusters */
  
#if (defined CLD_IDENTIFY) && (defined IDENTIFY_CLIENT)
    tsZCL_ClusterInstance sIdentifyClient;
#endif

#if (defined CLD_POLL_CONTROL) && (defined POLL_CONTROL_CLIENT)
    tsZCL_ClusterInstance sPollControlClient;
#endif


#if (defined CLD_ELECTRICAL_MEASUREMENT && defined ELECTRICAL_MEASUREMENT_SERVER)
    tsZCL_ClusterInstance sElectricalMeasurementServer;
#endif
} tsHA_IASZoneDeviceClusterInstances __attribute__ ((aligned(4)));
#endif

/* Holds everything required to create an instance of a IAS Zone device */
typedef struct
{
    tsZCL_EndPointDefinition sEndPoint;

    /* Cluster instances */
    tsHA_IASZoneDeviceClusterInstances sClusterInstance;

    /* All HA devices have 2 mandatory clusters - Basic(server) and Identify(server) */
#if (defined CLD_BASIC) && (defined BASIC_SERVER)
    /* Basic Cluster - Server */
    tsCLD_Basic sBasicServerCluster;
#endif

#if (defined CLD_IDENTIFY) && (defined IDENTIFY_SERVER)
    /* Identify Cluster - Server */
    tsCLD_Identify sIdentifyServerCluster;
    tsCLD_IdentifyCustomDataStructure sIdentifyServerCustomDataStructure;
#endif
    /* Mandatory Clusters supported by Thermostat Server Device */  
#if (defined CLD_IASZONE) && (defined IASZONE_SERVER)
    /* IAS Zone Cluster - Server */
    tsCLD_IASZone sIASZoneServerCluster;
    tsCLD_IASZone_CustomDataStructure sIASZoneServerCustomDataStructure;
#endif

    /* Optional Clusters Common to All HA Server Devices */
#if (defined CLD_POWER_CONFIGURATION) && (defined POWER_CONFIGURATION_SERVER)
    /* Power Configuration Cluster - Server */
    tsCLD_PowerConfiguration sPowerConfigServerCluster;
#endif

#if (defined CLD_DEVICE_TEMPERATURE_CONFIGURATION) && (defined DEVICE_TEMPERATURE_CONFIGURATION_SERVER)
    /* Device Temperature Configuration Cluster - Server */
    tsCLD_DeviceTemperatureConfiguration sDeviceTemperatureConfigurationServerCluster;
#endif

#if (defined CLD_ALARMS) && (defined ALARMS_SERVER)
    /* Alarms Cluster - Server */
    tsCLD_Alarms sAlarmsServerCluster;
    tsCLD_AlarmsCustomDataStructure sAlarmsServerCustomDataStructure;
#endif

#if (defined CLD_POLL_CONTROL) && (defined POLL_CONTROL_SERVER)
    tsCLD_PollControl sPollControlServerCluster;
    tsCLD_PollControlCustomDataStructure sPollControlServerCustomDataStructure;
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

    /* Optional Clusters Common to All HA Client Devices */
#if (defined CLD_SCENES) && (defined SCENES_CLIENT)
    /* Scenes Cluster - Client */
    tsCLD_Scenes sScenesClientCluster;
    tsCLD_ScenesCustomDataStructure sScenesClientCustomDataStructure;
#endif

#if (defined CLD_GROUPS) && (defined GROUPS_CLIENT)
    /* Groups Cluster - Client */
    tsCLD_Groups sGroupsClientCluster;
    tsCLD_GroupsCustomDataStructure sGroupsClientCustomDataStructure;
#endif

    /* Optional Clusters supported by Thermostat Server Device */

    /* Optional Clusters supported by Thermostat Client Device */

#ifdef CLD_OTA
    /* OTA cluster */
    #ifdef OTA_CLIENT
        tsCLD_AS_Ota sCLD_OTA;
        tsOTA_Common sCLD_OTA_CustomDataStruct;
    #endif
    
    #ifdef OTA_SERVER
        tsCLD_AS_Ota sCLD_ServerOTA;     
        tsOTA_Common sCLD_OTA_ServerCustomDataStruct;
    #endif
#endif

    /* Optional client clusters */

#if (defined CLD_IDENTIFY) && (defined IDENTIFY_CLIENT)
    /* Identify Cluster - Client */
    tsCLD_Identify sIdentifyClientCluster;
    tsCLD_IdentifyCustomDataStructure sIdentifyClientCustomDataStructure;
#endif

#if (defined CLD_POLL_CONTROL) && (defined POLL_CONTROL_CLIENT)
    tsCLD_PollControl sPollControlClientCluster;
    tsCLD_PollControlCustomDataStructure sPollControlClientCustomDataStructure;
#endif
#if (defined CLD_ELECTRICAL_MEASUREMENT && defined ELECTRICAL_MEASUREMENT_SERVER)
        tsCLD_ElectricalMeasurement sCLD_ElectricalMeasurement;
#endif
#if (defined CLD_TIME) && (defined TIME_CLIENT)
    tsCLD_Time sTimeClientCluster;
#endif
} tsHA_IASZoneDevice;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eHA_RegisterIASZoneEndPoint(uint8 u8EndPointIdentifier,
                                                   tfpZCL_ZCLCallBackFunction cbCallBack,
                                                   tsHA_IASZoneDevice *psDeviceInfo);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ZONE_H */
