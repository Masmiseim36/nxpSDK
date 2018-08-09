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


#ifndef ILLUMINANCE_MEASUREMENT_H
#define ILLUMINANCE_MEASUREMENT_H

/*!
\file       IlluminanceMeasurement.h
\brief      Header for Illuminance Measurement Cluster
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
#ifndef CLD_ILLMEAS_CLUSTER_REVISION
    #define CLD_ILLMEAS_CLUSTER_REVISION        1
#endif
/****************************************************************************/
/*             Illuminance Measurement Cluster - Optional Attributes        */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the illuminance measurement cluster.                       */
/****************************************************************************/

/* Enable the optional Tolerance attribute */
//#define CLD_ILLMEAS_ATTR_TOLERANCE

/* Enable the optional Light Sensor Type attribute */
//#define CLD_ILLMEAS_ATTR_LIGHT_SENSOR_TYPE

/* Enable the optional Attribute Reporting Status attribute */
//#define CLD_ILLMEAS_ATTR_ATTRIBUTE_REPORTING_STATUS

/* End of optional attributes */


/* Cluster ID's */
#define MEASUREMENT_AND_SENSING_CLUSTER_ID_ILLUMINANCE_MEASUREMENT  0x0400


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum 
{
    /* Illuminance Level Sensing Information attribute set attribute ID's (4.2.2.2.1) */
    E_CLD_ILLMEAS_ATTR_ID_MEASURED_VALUE          = 0x0000,  /* Mandatory */
    E_CLD_ILLMEAS_ATTR_ID_MIN_MEASURED_VALUE,                /* Mandatory */
    E_CLD_ILLMEAS_ATTR_ID_MAX_MEASURED_VALUE,                /* Mandatory */
    E_CLD_ILLMEAS_ATTR_ID_TOLERANCE,
    E_CLD_ILLMEAS_ATTR_ID_LIGHT_SENSOR_TYPE,
} teCLD_IM_ClusterID;


/* Light sensor types */
typedef enum 
{
    E_CLD_ILLMEAS_LST_PHOTODIODE = 0,
    E_CLD_ILLMEAS_LST_CMOS
} teCLD_IMEAS_LightSensorType;


/* Illuminance Measurement Cluster */
typedef struct
{
#ifdef ILLUMINANCE_MEASUREMENT_SERVER    
    zuint16                u16MeasuredValue;

    zuint16                u16MinMeasuredValue;

    zuint16                u16MaxMeasuredValue;

#ifdef CLD_ILLMEAS_ATTR_TOLERANCE
    zuint16                u16Tolerance;
#endif

#ifdef CLD_ILLMEAS_ATTR_LIGHT_SENSOR_TYPE
    zenum8                  eLightSensorType;
#endif
    
#ifdef CLD_ILLMEAS_ATTR_ATTRIBUTE_REPORTING_STATUS
    zenum8      u8AttributeReportingStatus;
#endif
#endif
    zuint16     u16ClusterRevision;

} tsCLD_IlluminanceMeasurement;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_IlluminanceMeasurementCreateIlluminanceMeasurement(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern tsZCL_ClusterDefinition sCLD_IlluminanceMeasurement;
extern const tsZCL_AttributeDefinition asCLD_IlluminanceMeasurementClusterAttributeDefinitions[];
extern uint8 au8IlluminanceMeasurementAttributeControlBits[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ILLUMINANCE_MEASUREMENT_H */
