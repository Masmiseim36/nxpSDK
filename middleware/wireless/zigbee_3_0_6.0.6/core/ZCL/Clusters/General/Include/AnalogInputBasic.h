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


#ifndef ANALOG_INPUT_BASIC_H
#define ANALOG_INPUT_BASIC_H

/*!
\file       AnalogInputBasic.h
\brief      Header for Analog Input Basic Cluster
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
/*             Analog Input Basic Cluster - Optional Attributes             */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Analog input basic cluster.                            */
/****************************************************************************/

/* Enable the optional Stack Version attribute*/
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_DESCRIPTION

/* Enable the optional Max Present Value attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_MAX_PRESENT_VALUE

/* Enable the optional Min Present Value attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_MIN_PRESENT_VALUE

/* Enable the optional Reliability attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_RELIABILITY

/* Enable the optional Resolution attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_RESOLUTION

/* Enable the optional Engineering Units attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_ENGINEERING_UNITS

/* Enable the optional Application Type attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_APPLICATION_TYPE

/* Enable the optional Attribute Reporting Status attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_ATTRIBUTE_REPORTING_STATUS

/* End of optional attributes */


/* Cluster ID's */
#define GENERAL_CLUSTER_ID_ANALOG_INPUT_BASIC                            0x000C


#ifndef CLD_ANALOG_INPUT_BASIC_CLUSTER_REVISION
    #define CLD_ANALOG_INPUT_BASIC_CLUSTER_REVISION                     1
#endif    

/* Status Flags - bit definitions and bit masks */
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_IN_ALARM                              0
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_FAULT                                 1
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_OVERRIDDEN                            2
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_OUT_OF_SERVICE                        3

#define ANALOG_INPUT_BASIC_STATUS_FLAGS_IN_ALARM_MASK                             (1 << ANALOG_INPUT_BASIC_STATUS_FLAGS_IN_ALARM)
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_FAULT_MASK                                (1 << ANALOG_INPUT_BASIC_STATUS_FLAGS_FAULT)
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_OVERRIDDEN_MASK                           (1 << ANALOG_INPUT_BASIC_STATUS_FLAGS_OVERRIDDEN)
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_OUT_OF_SERVICE_MASK                       (1 << ANALOG_INPUT_BASIC_STATUS_FLAGS_OUT_OF_SERVICE)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Command codes */


typedef enum
{
    /* Analog Input (Basic) Device Information attribute set attribute ID's (3.14.1.1.2) */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_DESCRIPTION                    =                0x001C, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_MAX_PRESENT_VALUE              =                0x0041, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_MIN_PRESENT_VALUE              =                0x0045, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_OUT_OF_SERVICE                 =                0x0051, /*Mandatory */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_PRESENT_VALUE                  =                0x0055, /* Mandatory */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_RELIABILITY                    =                0x0067, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_RESOLUTION                     =                0x006A, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_STATUS_FLAGS                   =                0x006F, /* Mandatory */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_ENGINEERING_UNITS              =                0x0075, /* Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_APPLICATION_TYPE               =                0x0100, /*Optional */
} teCLD_AnalogInputBasicCluster_AttrID;



/* Polarity attribute */
typedef enum
{
    E_CLD_ANALOG_INPUT_BASIC_OUT_OF_SERVICE_FALSE                       =                0x0000,
    E_CLD_ANALOG_INPUT_BASIC_OUT_OF_SERVICE_TRUE
}teCLD_AnalogInputBasic_OutOfService;


/* Reliability attribute */
typedef enum
{
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_NO_FAULT_DETECTED         =                0x0000,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_NO_SENSOR,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_OVER_RANGE,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_UNDER_RANGE,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_OPEN_LOOP,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_SHORTED_LOOP,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_NO_OUTPUT,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_UNRELIABLE_OTHER,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_PROCESS_ERROR,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_CONFIGURATION_ERROR       =                0x000A
 }teCLD_AnalogInputBasic_Reliability;


/* Analog Input Basic Cluster */
typedef struct
{

#ifdef ANALOG_INPUT_BASIC_SERVER
#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_DESCRIPTION
    tsZCL_CharacterString       sDescription;
    zuint8                      au8Description[16];
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_MAX_PRESENT_VALUE
    zsingle                     fMaxPresentValue;
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_MIN_PRESENT_VALUE
    zsingle                     fMinPresentValue;
#endif

    zbool                       bOutOfService;
    
    zsingle                     fPresentValue;

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_RELIABILITY
    zenum8                      u8Reliability;
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_RESOLUTION
    zsingle                     fResolution;
#endif

    zbmap8                      u8StatusFlags;

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_ENGINEERING_UNITS
    zenum16                     u16EngineeringUnits;
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_APPLICATION_TYPE
    zuint32                     u32ApplicationType;
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_ATTRIBUTE_REPORTING_STATUS
    zenum8                      u8AttributeReportingStatus;
#endif
    
#endif
    zuint16                     u16ClusterRevision;
    
} tsCLD_AnalogInputBasic;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_AnalogInputBasicCreateAnalogInputBasic(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZCL_ClusterDefinition sCLD_AnalogInputBasic;
extern tsZCL_AttributeDefinition asCLD_AnalogInputBasicClusterAttributeDefinitions[];
extern uint8 au8AnalogInputBasicAttributeControlBits[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* BASIC_H */
