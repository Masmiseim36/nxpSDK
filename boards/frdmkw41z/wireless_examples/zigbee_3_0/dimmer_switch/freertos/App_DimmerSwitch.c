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
\file       App_DimmerSwitch.c
\brief      ZLO Controller & Switch Demo: Dimmer Switch Implementation
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "app_zcl_cfg.h"
#include "App_DimmerSwitch.h"
#include "app_main.h"
#include "LED.h"
#include <string.h>
#include "dbg.h"
#include "FunctionLib.h"
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
tsZLO_DimmerSwitchDevice sSwitch;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
const uint8 u8MyEndpoint = DIMMERSWITCH_SWITCH_ENDPOINT;

/****************************************************************************
 **
 ** NAME: eApp_ZCL_RegisterEndpoint
 **
 ** DESCRIPTION:
 ** Register ZLO endpoints
 **
 ** PARAMETER
 ** Type                                Name                    Descirption
 ** tfpZCL_ZCLCallBackFunction          fptr                    Pointer to ZCL Callback function
 **
 **
 **
 ** RETURNS:
 ** teZCL_Status
 *
 ****************************************************************************/
teZCL_Status eApp_ZCL_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr)

{
    return eZLO_RegisterDimmerSwitchEndPoint(DIMMERSWITCH_SWITCH_ENDPOINT,
                                                fptr,
                                                &sSwitch);
}

/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_Init
 *
 * DESCRIPTION:
 * ZCL Device Specific initialization
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
void vAPP_ZCL_DeviceSpecific_Init()
{
    /* Initialise the strings in Basic */
    uint8   au8PCode[CLD_BAS_PCODE_SIZE] = { 1, 2, 3, 4};
    FLib_MemCpy(sSwitch.sBasicServerCluster.au8ManufacturerName, "NXP", CLD_BAS_MANUF_NAME_SIZE);
    FLib_MemCpy(sSwitch.sBasicServerCluster.au8ModelIdentifier, "ZLO-DimmerSwitch", CLD_BAS_MODEL_ID_SIZE);
    FLib_MemCpy(sSwitch.sBasicServerCluster.au8DateCode, "20160301", CLD_BAS_DATE_SIZE);
    FLib_MemCpy(sSwitch.sBasicServerCluster.au8SWBuildID, "2000-0001", CLD_BAS_SW_BUILD_SIZE);
    sSwitch.sBasicServerCluster.eGenericDeviceType = E_CLD_BAS_GENERIC_DEVICE_TYPE_WALL_SWITCH;
    FLib_MemCpy(sSwitch.sBasicServerCluster.au8ProductURL, "www.nxp.com", CLD_BAS_URL_SIZE);
    FLib_MemCpy(sSwitch.sBasicServerCluster.au8ProductCode, au8PCode, CLD_BAS_PCODE_SIZE);

}
/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_Identify
 *
 * DESCRIPTION:
 * ZCL Device Specific initialization
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_ZCL_DeviceSpecific_SetIdentifyTime(uint16 u16Time)
{
    sSwitch.sIdentifyServerCluster.u16IdentifyTime=u16Time;
}
/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_UpdateIdentify
 *
 * DESCRIPTION:
 * ZCL Device Specific initialization
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_ZCL_DeviceSpecific_UpdateIdentify(void)
{
    APP_vSetLED(LED1, sSwitch.sIdentifyServerCluster.u16IdentifyTime%2);
}
/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_IdentifyOff
 *
 * DESCRIPTION:
 * ZCL Device Specific initialization
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_ZCL_DeviceSpecific_IdentifyOff(void)
{
    vAPP_ZCL_DeviceSpecific_SetIdentifyTime(0);
    APP_vSetLED(LED1, 0);
}

/****************************************************************************
 *
 * NAME: app_u8GetDeviceEndpoint
 *
 * DESCRIPTION:
 * Return the application endpoint
 *
 * PARAMETER: uint8
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC uint8 app_u8GetDeviceEndpoint( void)
{
    return DIMMERSWITCH_SWITCH_ENDPOINT;
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
