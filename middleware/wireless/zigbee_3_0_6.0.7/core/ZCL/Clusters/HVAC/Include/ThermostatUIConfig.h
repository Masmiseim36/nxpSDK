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


#ifndef THERMOSTAT_UI_CONFIG_H
#define THERMOSTAT_UI_CONFIG_H

/*!
\file       ThermostatUIConfigUIConfig.h
\brief      Header for ThermostatUIConfigUIConfig Cluster
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
/*       ThermostatUIConfigUIConfig Cluster - Optional Attributes              */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the temperature measurement cluster.                       */
/****************************************************************************/

/* End of optional attributes */


/* Cluster ID's */
#define HVAC_CLUSTER_ID_THERMOSTAT_UI_CONFIG                                             0x0204

#ifndef CLD_THERMOSTAT_UI_CONFIG_CLUSTER_REVISION
    #define CLD_THERMOSTAT_UI_CONFIG_CLUSTER_REVISION                     1
#endif  
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum 
{
    /* ThermostatUIConfigUIConfig attribute set attribute ID's (6.6.2.2) */
    E_CLD_THERMOSTAT_UI_CONFIG_ATTR_ID_TEMPERATURE_DISPLAY_MODE               = 0x0000,         /* Mandatory */
    E_CLD_THERMOSTAT_UI_CONFIG_ATTR_ID_KEYPAD_LOCKOUT,                                          /* Mandatory */
} teCLD_ThermostatUIConfig_AttributeID;

/* Temperature Display */
typedef enum 
{
    E_CLD_THERMOSTAT_UI_CONFIG_TEMPERATURE_DISPLAY_MODE_CELSIUS                 = 0x00,
    E_CLD_THERMOSTAT_UI_CONFIG_TEMPERATURE_DISPLAY_MODE_FAHRENHEIT
} teCLD_ThermostatUIConfig_TemperatureDisplay;

/* Keypad Lockout */
typedef enum 
{
    E_CLD_THERMOSTAT_UI_CONFIG_KEYPAD_LOCKOUT_NO_LOCKOUT                               = 0x00,
    E_CLD_THERMOSTAT_UI_CONFIG_KEYPAD_LOCKOUT_LEVEL_1_LOCKOUT,
    E_CLD_THERMOSTAT_UI_CONFIG_KEYPAD_LOCKOUT_LEVEL_2_LOCKOUT,
    E_CLD_THERMOSTAT_UI_CONFIG_KEYPAD_LOCKOUT_LEVEL_3_LOCKOUT,
    E_CLD_THERMOSTAT_UI_CONFIG_KEYPAD_LOCKOUT_LEVEL_4_LOCKOUT,
    E_CLD_THERMOSTAT_UI_CONFIG_KEYPAD_LOCKOUT_LEVEL_5_LOCKOUT
} teCLD_ThermostatUIConfig_KeyPadLockout;

/* ThermostatUIConfig Cluster */
typedef struct
{
#ifdef THERMOSTAT_UI_CONFIG_SERVER    
    zenum8                 eTemperatureDisplayMode;
    
    zenum8                 eKeypadLockout;
#endif    
    zuint16                u16ClusterRevision;
} tsCLD_ThermostatUIConfig;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_ThermostatUIConfigCreateThermostatUIConfig(
                tsZCL_ClusterInstance                   *psClusterInstance,
                bool_t                                  bIsServer,
                tsZCL_ClusterDefinition                 *psClusterDefinition,
                void                                    *pvEndPointSharedStructPtr,
                uint8                                   *pu8AttributeControlBits);
#ifdef THERMOSTAT_UI_CONFIG_SERVER
PUBLIC teZCL_Status eCLD_ThermostatUIConfigConvertTemp(
                uint8                                   u8SourceEndPointId,
                bool_t                                    bConvertFToC,
                int16                                   *pi16Temperature); 
#endif                
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern const tsZCL_AttributeDefinition asCLD_ThermostatUIConfigClusterAttributeDefinitions[];
extern tsZCL_ClusterDefinition sCLD_ThermostatUIConfig;
extern uint8 au8ThermostatUIConfigAttributeControlBits[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* THERMOSTAT_UI_CONFIG_H */
