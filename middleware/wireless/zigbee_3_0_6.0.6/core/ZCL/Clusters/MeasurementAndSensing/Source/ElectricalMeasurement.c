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

/*!=============================================================================
\file       ElectricalMeasurement.c
\brief      Electrical Measurement cluster definition
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
#include "ElectricalMeasurement.h"


#ifdef CLD_ELECTRICAL_MEASUREMENT
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
const tsZCL_AttributeDefinition asCLD_ElectricalMeasurementClusterAttributeDefinitions[] = {
#ifdef ELECTRICAL_MEASUREMENT_SERVER
        {E_CLD_ELECTMEAS_ATTR_ID_MEASUREMENT_TYPE,                      E_ZCL_AF_RD,                    E_ZCL_BMAP32,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u32MeasurementType),      0},   /* Mandatory */

    #ifdef CLD_ELECTMEAS_ATTR_AC_FREQUENCY
        {E_CLD_ELECTMEAS_ATTR_ID_AC_FREQUENCY,                          E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ACFrequency),          0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_RMS_VOLTAGE
        {E_CLD_ELECTMEAS_ATTR_ID_RMS_VOLATGE,                           E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16RMSVoltage),           0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_RMS_CURRENT
        {E_CLD_ELECTMEAS_ATTR_ID_RMS_CURRENT,                           E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16RMSCurrent),           0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_ACTIVE_POWER
        {E_CLD_ELECTMEAS_ATTR_ID_ACTIVE_POWER,                          E_ZCL_AF_RD,                    E_ZCL_INT16,     (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->i16ActivePower),          0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_REACTIVE_POWER
        {E_CLD_ELECTMEAS_ATTR_ID_REACTIVE_POWER,                        E_ZCL_AF_RD,                    E_ZCL_INT16,     (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->i16ReactivePower),        0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_APPARENT_POWER
        {E_CLD_ELECTMEAS_ATTR_ID_APPARENT_POWER,                        E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ApparentPower),        0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_POWER_FACTOR
        {E_CLD_ELECTMEAS_ATTR_ID_POWER_FACTOR,                          E_ZCL_AF_RD,                    E_ZCL_INT8,      (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->i8PowerFactor),           0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_AC_VOLTAGE_MULTIPLIER
        {E_CLD_ELECTMEAS_ATTR_ID_AC_VOLTAGE_MULTIPLIER,                 E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ACVoltageMultiplier),  0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_AC_VOLTAGE_DIVISOR
        {E_CLD_ELECTMEAS_ATTR_ID_AC_VOLTAGE_DIVISOR,                    E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ACVoltageDivisor),     0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_AC_CURRENT_MULTIPLIER
        {E_CLD_ELECTMEAS_ATTR_ID_AC_CURRENT_MULTIPLIER,                 E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ACCurrentMultiplier),  0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_AC_CURRENT_DIVISOR
        {E_CLD_ELECTMEAS_ATTR_ID_AC_CURRENT_DIVISOR,                    E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ACCurentDivisor),      0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_AC_POWER_MULTIPLIER
        {E_CLD_ELECTMEAS_ATTR_ID_AC_POWER_MULTIPLIER,                   E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ACPowerMultiplier),    0},   /* Optional */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_AC_POWER_DIVISOR
        {E_CLD_ELECTMEAS_ATTR_ID_AC_POWER_DIVISOR,                      E_ZCL_AF_RD,                    E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ACPowerDivisor),       0},   /* Optional */
    #endif
    
    /*Manufacturer Specific Attributes*/
    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_APPARENT_POWER   
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_APPARENT_POWER,               (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_UINT32,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u32ManSpecificApparentPower),       0},   /* Manu Specific */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_NON_ACTIVE_POWER
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_NON_ACTIVE_POWER,             (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_UINT32,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u32NonActivePower),       0},   /* Manu Specific */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_FNDMTL_REACTIVE_POWER
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_FNDMTL_REACTIVE_POWER,        (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_INT32,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->i32FundamentalReactivePower),       0},   /* Manu Specific */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_FNDMTL_APPARENT_POWER
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_FNDMTL_APPARENT_POWER,        (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_UINT32,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u32FundamentalApparentPower),       0},   /* Manu Specific */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_FNDMTL_POWER_FACTOR
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_FNDMTL_POWER_FACTOR,          (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16FundamentalPowerFactor),       0},   /* Manu Specific */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_NON_FNDMTL_APPARENT_POWER
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_NON_FNDMTL_APPARENT_POWER,    (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_UINT32,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u32NonFundamentalApparentPower),       0},   /* Manu Specific */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_TOTAL_HARMONIC_DISTORTION
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_TOTAL_HARMONIC_DISTORTION,    (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_UINT32,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u32TotalHarmonicDistortion),       0},   /* Manu Specific */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_VBIAS
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_VBIAS,                        (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_UINT32,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u32VBias),       0},   /* Manu Specific */
    #endif

    #ifdef CLD_ELECTMEAS_ATTR_MAN_SPEC_DIVISOR
        {E_CLD_ELECTMEAS_ATTR_ID_MAN_SPEC_DIVISOR,                      (E_ZCL_AF_RD|E_ZCL_AF_MS),      E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ManSpecDivisor),       0},   /* Manu Specific */
    #endif

#endif    
        {E_CLD_GLOBAL_ATTR_ID_CLUSTER_REVISION,                         (E_ZCL_AF_RD|E_ZCL_AF_GA),      E_ZCL_UINT16,    (uint32)(&((tsCLD_ElectricalMeasurement*)(0))->u16ClusterRevision),      0},   /* Mandatory  */

    };

tsZCL_ClusterDefinition sCLD_ElectricalMeasurement = {
        MEASUREMENT_AND_SENSING_CLUSTER_ID_ELECTRICAL_MEASUREMENT,
        FALSE,
        E_ZCL_SECURITY_NETWORK,
        (sizeof(asCLD_ElectricalMeasurementClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition)),
        (tsZCL_AttributeDefinition*)asCLD_ElectricalMeasurementClusterAttributeDefinitions,
        NULL
};

uint8 au8ElectricalMeasurementAttributeControlBits[(sizeof(asCLD_ElectricalMeasurementClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition))];
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 **
 ** NAME:       eCLD_ElectricalMeasurementCreateElectricalMeasurement
 **
 ** DESCRIPTION:
 ** Creates a temperature measurement cluster
 **
 ** PARAMETERS:                 Name                        Usage
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ElectricalMeasurementCreateElectricalMeasurement(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits)
{
    #ifdef STRICT_PARAM_CHECK 
        /* Parameter check */
        if((psClusterInstance==NULL) ||
           (psClusterDefinition==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif

    /* Create an instance of a temperature measurement cluster */
    vZCL_InitializeClusterInstance(
                   psClusterInstance, 
                   bIsServer,
                   psClusterDefinition,
                   pvEndPointSharedStructPtr,
                   pu8AttributeControlBits,
                   NULL,
                   NULL);

        if(pvEndPointSharedStructPtr != NULL)
        {
#ifdef ELECTRICAL_MEASUREMENT_SERVER            
            /* Initializing these values to invalid as ZCL spec does not mention the defualt */ 
            #ifdef CLD_ELECTMEAS_ATTR_AC_FREQUENCY
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ACFrequency = 0xFFFF;
            #endif
            
            #ifdef CLD_ELECTMEAS_ATTR_RMS_VOLTAGE
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16RMSVoltage = 0xFFFF;
            #endif
            
            #ifdef CLD_ELECTMEAS_ATTR_RMS_CURRENT
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16RMSCurrent = 0xFFFF;
            #endif
            
            #ifdef CLD_ELECTMEAS_ATTR_ACTIVE_POWER
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->i16ActivePower = 0xFFFF;
            #endif

            #ifdef CLD_ELECTMEAS_ATTR_REACTIVE_POWER
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->i16ReactivePower = 0xFFFF;
            #endif
            
            #ifdef CLD_ELECTMEAS_ATTR_APPARENT_POWER
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ApparentPower = 0xFFFF;
            #endif
            
            #ifdef CLD_ELECTMEAS_ATTR_AC_VOLTAGE_MULTIPLIER
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ACVoltageMultiplier = 0x0001;
            #endif
            
            #ifdef CLD_ELECTMEAS_ATTR_AC_VOLTAGE_DIVISOR
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ACVoltageDivisor = 0x0001;
            #endif
    
            #ifdef CLD_ELECTMEAS_ATTR_AC_CURRENT_MULTIPLIER
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ACCurrentMultiplier = 0x0001;
            #endif
    
            #ifdef CLD_ELECTMEAS_ATTR_AC_CURRENT_DIVISOR
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ACCurentDivisor = 0x0001;
            #endif
    
            #ifdef CLD_ELECTMEAS_ATTR_AC_POWER_MULTIPLIER
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ACPowerMultiplier = 0x0001;
            #endif
    
            #ifdef CLD_ELECTMEAS_ATTR_AC_POWER_DIVISOR
                ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ACPowerDivisor = 0x0001;
            #endif
#endif            
            ((tsCLD_ElectricalMeasurement*)pvEndPointSharedStructPtr)->u16ClusterRevision = CLD_ELECTMEAS_CLUSTER_REVISION;
        }

    return E_ZCL_SUCCESS;

}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#endif/* CLD_ELECTRICAL_MEASUREMENT */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

