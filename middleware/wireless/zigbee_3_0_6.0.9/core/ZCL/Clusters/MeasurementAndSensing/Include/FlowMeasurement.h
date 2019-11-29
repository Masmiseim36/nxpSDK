/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef FLOW_MEASUREMENT_H
#define FLOW_MEASUREMENT_H

/*!
\file       FlowMeasurement.h
\brief      Header for Flow Measurement Cluster
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
#ifndef CLD_FLOWMEAS_CLUSTER_REVISION
    #define CLD_FLOWMEAS_CLUSTER_REVISION        1
#endif 
/****************************************************************************/
/*          Flow Measurement Cluster - Optional Attributes                  */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Flow measurement cluster.                       */
/****************************************************************************/

/* Enable the optional Tolerance attribute */
//#define CLD_FLOWMEAS_ATTR_TOLERANCE

/* Enable the optional Attribute Reporting Status attribute */
//#define CLD_FLOWMEAS_ATTR_ATTRIBUTE_REPORTING_STATUS

/* End of optional attributes */


/* Cluster ID's */
#define MEASUREMENT_AND_SENSING_CLUSTER_ID_FLOW_MEASUREMENT  0x0404


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum 
{
    /* Flow Measurement Information attribute set attribute ID's (4.6.2.2.1) */
    E_CLD_FLOWMEAS_ATTR_ID_MEASURED_VALUE               = 0x0000,   /* Mandatory */
    E_CLD_FLOWMEAS_ATTR_ID_MIN_MEASURED_VALUE,                      /* Mandatory */
    E_CLD_FLOWMEAS_ATTR_ID_MAX_MEASURED_VALUE,                      /* Mandatory */
    E_CLD_FLOWMEAS_ATTR_ID_TOLERANCE,
} teCLD_FlowMeasurement_AttributeID;

/* Flow Measurement Cluster */
typedef struct
{
#ifdef FLOW_MEASUREMENT_SERVER    
    zint16                 i16MeasuredValue;

    zint16                 i16MinMeasuredValue;

    zint16                 i16MaxMeasuredValue;

#ifdef CLD_FLOWMEAS_ATTR_TOLERANCE
    zuint16                u16Tolerance;
#endif

#ifdef CLD_FLOWMEAS_ATTR_ATTRIBUTE_REPORTING_STATUS
    zenum8                 u8AttributeReportingStatus;
#endif
#endif
    zuint16                u16ClusterRevision;
   
} tsCLD_FlowMeasurement;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_FlowMeasurementCreateFlowMeasurement(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8              *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZCL_ClusterDefinition sCLD_FlowMeasurement;
extern uint8 au8FlowMeasurementAttributeControlBits[];
extern const tsZCL_AttributeDefinition asCLD_FlowMeasurementClusterAttributeDefinitions[];
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* FLOW_MEASUREMENT_H */
