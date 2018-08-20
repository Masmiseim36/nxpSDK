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


#ifndef DEVICE_TEMPERATURE_CONFIGURATION_H
#define DEVICE_TEMPERATURE_CONFIGURATION_H

/*!
\file       DeviceTemperatureConfiguration.h
\brief      Header for Device Temperature Configuration Cluster
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

/****************************************************************************/
/* DeviceTemperatureConfiguration Cluster - Optional Attributes             */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the cluster.                                               */
/****************************************************************************/

/* Enable the optional Minimum Temperature Experienced attribute */
//#define CLD_DEVTEMPCFG_ATTR_ID_MIN_TEMP_EXPERIENCED

/* Enable the optional Maximum Temperature Experienced attribute */
//#define CLD_DEVTEMPCFG_ATTR_ID_MAX_TEMP_EXPERIENCED

/* Enable the optional Over Temperature Total Dwell attribute */
//#define CLD_DEVTEMPCFG_ATTR_ID_OVER_TEMP_TOTAL_DWELL

/* Enable the optional Device Temperature Alarm Mask attribute */
//#define CLD_DEVTEMPCFG_ATTR_ID_DEVICE_TEMP_ALARM_MASK

/* Enable the optional Low Temperature Threshold attribute */
//#define CLD_DEVTEMPCFG_ATTR_ID_LOW_TEMP_THRESHOLD

/* Enable the optional High Temperature Threshold attribute */
//#define CLD_DEVTEMPCFG_ATTR_ID_HIGH_TEMP_THRESHOLD

/* Enable the optional Low Temperature Dwell Trip Point attribute */
//#define CLD_DEVTEMPCFG_ATTR_ID_LOW_TEMP_DWELL_TRIP_POINT

/* Enable the optional High Temperature Dwell Trip Point attribute */
//#define CLD_DEVTEMPCFG_ATTR_ID_HIGH_TEMP_DWELL_TRIP_POINT

/* End of optional attributes */


/* Cluster ID's */
#define GENERAL_CLUSTER_ID_DEVICE_TEMPERATURE_CONFIGURATION     0x0002

#ifndef CLD_DEVTEMPCFG_CLUSTER_REVISION
    #define CLD_DEVTEMPCFG_CLUSTER_REVISION                         1
#endif 

/* Device Temperature Alarm Mask */
#define CLD_DEVTEMPCFG_BITMASK_DEVICE_TEMP_TOO_LOW              (1 << 0)
#define CLD_DEVTEMPCFG_BITMASK_DEVICE_TEMP_TOO_HIGH             (1 << 1)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum 
{
    /* Device Temperature Information attribute set attribute ID's (3.4.2.2.1) */
    E_CLD_DEVTEMPCFG_ATTR_ID_CURRENT_TEMPERATURE      = 0x0000, /* Mandatory */
    E_CLD_DEVTEMPCFG_ATTR_ID_MIN_TEMP_EXPERIENCED,
    E_CLD_DEVTEMPCFG_ATTR_ID_MAX_TEMP_EXPERIENCED,
    E_CLD_DEVTEMPCFG_ATTR_ID_OVER_TEMP_TOTAL_DWELL,

    /* Device Temperature settings attribute set attribute ID's (3.4.2.2.2) */
    E_CLD_DEVTEMPCFG_ATTR_ID_DEVICE_TEMP_ALARM_MASK   = 0x0010,
    E_CLD_DEVTEMPCFG_ATTR_ID_LOW_TEMP_THRESHOLD,
    E_CLD_DEVTEMPCFG_ATTR_ID_HIGH_TEMP_THRESHOLD,
    E_CLD_DEVTEMPCFG_ATTR_ID_LOW_TEMP_DWELL_TRIP_POINT,
    E_CLD_DEVTEMPCFG_ATTR_ID_HIGH_TEMP_DWELL_TRIP_POINT,
} teCLD_DEVTEMPCFG_AttributeId;

/* Device Temperature Configuration Cluster */
typedef struct
{

#ifdef DEVICE_TEMPERATURE_CONFIGURATION_SERVER
    zint16                  i16CurrentTemperature;

#ifdef CLD_DEVTEMPCFG_ATTR_ID_MIN_TEMP_EXPERIENCED
    zint16                  i16MinTempExperienced;
#endif

#ifdef CLD_DEVTEMPCFG_ATTR_ID_MAX_TEMP_EXPERIENCED
    zint16                  i16MaxTempExperienced;
#endif

#ifdef CLD_DEVTEMPCFG_ATTR_ID_OVER_TEMP_TOTAL_DWELL
    zuint16                 u16OverTempTotalDwell;
#endif

#ifdef CLD_DEVTEMPCFG_ATTR_ID_DEVICE_TEMP_ALARM_MASK
    zbmap8                  u8DeviceTempAlarmMask;
#endif

#ifdef CLD_DEVTEMPCFG_ATTR_ID_LOW_TEMP_THRESHOLD
    zint16                  i16LowTempThreshold;
#endif

#ifdef CLD_DEVTEMPCFG_ATTR_ID_HIGH_TEMP_THRESHOLD
    zint16                  i16HighTempThreshold;
#endif

#ifdef CLD_DEVTEMPCFG_ATTR_ID_LOW_TEMP_DWELL_TRIP_POINT
    zuint24                 u24LowTempDwellTripPoint;
#endif

#ifdef CLD_DEVTEMPCFG_ATTR_ID_HIGH_TEMP_DWELL_TRIP_POINT
    zuint24                 u24HighTempDwellTripPoint;
#endif
#endif
    zuint16                 u16ClusterRevision;

} tsCLD_DeviceTemperatureConfiguration;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_DeviceTemperatureConfigurationCreateDeviceTemperatureConfiguration(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                                        *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern tsZCL_ClusterDefinition sCLD_DeviceTemperatureConfiguration;
extern uint8 au8DeviceTempConfigClusterAttributeControlBits[];
extern const tsZCL_AttributeDefinition asCLD_DeviceTemperatureConfigurationClusterAttributeDefinitions[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* DEVICE_TEMPERATURE_CONFIGURATION_H */
