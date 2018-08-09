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

#ifndef ZLO_DEVICE_ID_H
#define ZLO_DEVICE_ID_H

/*!
\file       zlo_device_id.h
\brief      Header for ZigBee Lighting and Occupancy Device Id file
*/
     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/    
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* ZLO device types  */
#define ZLO_ON_OFF_LIGHT_DEVICE_ID                  0x0100
#define ZLO_DIMMABLE_LIGHT_DEVICE_ID                0x0101
#define ZLO_COLOUR_DIMMABLE_LIGHT_DEVICE_ID         0x0102
#define ZLO_ON_OFF_LIGHT_SWITCH_DEVICE_ID           0x0103
#define ZLO_DIMMER_SWITCH_DEVICE_ID                 0x0104
#define ZLO_COLOUR_DIMMER_SWITCH_DEVICE_ID          0x0105
#define ZLO_LIGHT_SENSOR_DEVICE_ID                  0x0106
#define ZLO_OCCUPANCY_SENSOR_DEVICE_ID              0x0107
#define ZLO_ON_OFF_BALLAST_DEVICE_ID                0x0108
#define ZLO_DIMMABLE_BALLAST_DEVICE_ID              0x0109
#define ZLO_ON_OFF_PLUG_DEVICE_ID                   0x010A
#define ZLO_DIMMABLE_PLUG_DEVICE_ID                 0x010B
#define ZLO_COLOUR_TEMPERATURE_LIGHT_DEVICE_ID      0x010C
#define ZLO_EXTENDED_COLOUR_LIGHT_DEVICE_ID         0x010D
#define ZLO_LIGHT_LEVEL_SENSOR_DEVICE_ID            0x010E

#define ZLO_COLOUR_CONTROLLER_DEVICE_ID             0x0800
#define ZLO_COLOUR_SCENE_CONTROLLER_DEVICE_ID       0x0810
#define ZLO_NON_COLOUR_CONTROLLER_DEVICE_ID         0x0820
#define ZLO_NON_COLOUR_SCENE_CONTROLLER_DEVICE_ID   0x0830
#define ZLO_CONTROL_BRIDGE_DEVICE_ID                0x0840
#define ZLO_ON_OFF_SENSOR_DEVICE_ID                 0x0850

/* Legacy ZLL Device IDs */
#define ON_OFF_LIGHT_DEVICE_ID             0x0000
#define ON_OFF_PLUG_DEVICE_ID              0x0010
#define DIMMABLE_LIGHT_DEVICE_ID           0x0100
#define DIMMABLE_PLUG_DEVICE_ID            0x0110
#define COLOUR_LIGHT_DEVICE_ID             0x0200
#define EXTENDED_COLOUR_LIGHT_DEVICE_ID     0x0210
#define COLOUR_TEMPERATURE_LIGHT_DEVICE_ID  0x0220

#define COLOUR_REMOTE_DEVICE_ID            0x0800
#define COLOUR_SCENE_REMOTE_DEVICE_ID      0x0810
#define NON_COLOUR_REMOTE_DEVICE_ID        0x0820
#define NON_COLOUR_SCENE_REMOTE_DEVICE_ID  0x0830
#define CONTROL_BRIDGE_DEVICE_ID           0x0840
#define ON_OFF_SENSOR_DEVICE_ID            0x0850

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ZLO_DEVICE_ID_H */
