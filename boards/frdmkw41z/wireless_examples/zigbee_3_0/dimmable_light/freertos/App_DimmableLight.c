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
\file       App_DimmableLight.c
\brief      ZLO Demo: Dimmable Light Implementation
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "app_zcl_cfg.h"

#include "app_reporting.h"
#include "App_DimmableLight.h"
#include "app_zcl_light_task.h"
#include "app_common.h"
#include "dbg.h"
#include <string.h>
#include "app_light_interpolation.h"
#
#include "DriverBulb_Shim.h"
#include "bdb_options.h"

#include "zlo_device_id.h"

#ifdef DEBUG_LIGHT_TASK
#define TRACE_LIGHT_TASK  TRUE
#else
#define TRACE_LIGHT_TASK FALSE
#endif
#include "FunctionLib.h"


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

tsZLO_DimmableLightDevice sLight;
tsIdentifyWhite sIdEffect;
tsCLD_ZllDeviceTable sDeviceTable = { ZLO_NUMBER_DEVICES,
                                      {
                                          { 0,
                                            HA_PROFILE_ID,
                                            ZLO_DIMMABLE_LIGHT_DEVICE_ID,
                                            DIMMABLELIGHT_LIGHT_ENDPOINT,
                                            APPLICATION_DEVICE_VERSION,
                                            0,
                                            0}
                                      }
};

/* define the default reports */
tsReports asDefaultReports[ZLO_NUMBER_OF_REPORTS] = \
{\
    {GENERAL_CLUSTER_ID_ONOFF,           {0, E_ZCL_BOOL,     E_CLD_ONOFF_ATTR_ID_ONOFF,                         ZLO_MIN_REPORT_INTERVAL,ZLO_MAX_REPORT_INTERVAL,0,{0}}},\
    {GENERAL_CLUSTER_ID_LEVEL_CONTROL,   {0, E_ZCL_UINT8,    E_CLD_LEVELCONTROL_ATTR_ID_CURRENT_LEVEL,          ZLO_MIN_REPORT_INTERVAL,ZLO_MAX_REPORT_INTERVAL,0,{10}}}\
};

/*
 * If overriding the ZLL Master Key in the appilation,
 * define BDB_APPLICATION_DEFINED_TL_MASTER_KEY in bdb_options.h
 * otherwize the key defined in BDB\Source\touchlink\bdb_tl_common.c will be used.
 */
#ifdef BDB_APPLICATION_DEFINED_TL_MASTER_KEY
PUBLIC tsReg128 sTLMasterKey = {0x11223344, 0x55667788, 0x99aabbcc, 0xddeeff00 };
#endif

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE bool bAllowInterPanEp(uint8 u8Ep, uint16 u16ProfileId);

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: eApp_ZLO_RegisterEndpoint
 *
 * DESCRIPTION:
 * Register ZLO endpoints
 *
 * PARAMETER
 * Type                        Name                  Descirption
 * tfpZCL_ZCLCallBackFunction  fptr                  Pointer to ZCL Callback function
 *
 *
 * RETURNS:
 * teZCL_Status
 *
 ****************************************************************************/
PUBLIC teZCL_Status eApp_ZLO_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr)
{

    zps_vSetIgnoreProfileCheck();
    ZPS_vAplZdoRegisterInterPanFilter((void *)&bAllowInterPanEp);

    return eZLO_RegisterDimmableLightEndPoint(DIMMABLELIGHT_LIGHT_ENDPOINT,
                                              fptr,
                                              &sLight);
}


/****************************************************************************
*
* NAME: bAllowInterPanEp
*
* DESCRIPTION: Allows the application to decide which end point receive
* inter pan messages
*
*
* PARAMETER: the end point receiving the inter pan
*
* RETURNS: True to allow reception, False otherwise
*
****************************************************************************/
PRIVATE bool bAllowInterPanEp(uint8 u8Ep, uint16 u16ProfileId) {

    if ( (u8Ep == DIMMABLELIGHT_LIGHT_ENDPOINT) &&
          ( u16ProfileId == ZLL_PROFILE_ID))
    {
        return TRUE;
    }
    return FALSE;
}


/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_Init
 *
 * DESCRIPTION:
 * ZLO Device Specific initialization
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_ZCL_DeviceSpecific_Init()
{
    /* Initialise the strings in Basic */
    uint8    au8PCode[CLD_BAS_PCODE_SIZE] = BAS_PCODE_STRING;
    FLib_MemCpy(sLight.sBasicServerCluster.au8ManufacturerName, BAS_MANUF_NAME_STRING, CLD_BAS_MANUF_NAME_SIZE);
    FLib_MemCpy(sLight.sBasicServerCluster.au8ModelIdentifier, BAS_MODEL_ID_STRING, CLD_BAS_MODEL_ID_SIZE);
    FLib_MemCpy(sLight.sBasicServerCluster.au8DateCode, BAS_DATE_STRING, CLD_BAS_DATE_SIZE);
    FLib_MemCpy(sLight.sBasicServerCluster.au8SWBuildID, BAS_SW_BUILD_STRING, CLD_BAS_SW_BUILD_SIZE);
    FLib_MemCpy(sLight.sBasicServerCluster.au8ProductURL, BAS_URL_STRING, CLD_BAS_URL_SIZE);
    FLib_MemCpy(sLight.sBasicServerCluster.au8ProductCode, au8PCode, CLD_BAS_PCODE_SIZE);

    sIdEffect.u8Effect = E_CLD_IDENTIFY_EFFECT_STOP_EFFECT;
    sIdEffect.u8Tick = 0;

}

/****************************************************************************
 *
 * NAME: app_u8GetDeviceEndpoint
 *
 * DESCRIPTION:
 * Return the application endpoint
 *
 * PARAMETER: void
 *
 * RETURNS: uint8
 *
 ****************************************************************************/
PUBLIC uint8 app_u8GetDeviceEndpoint( void)
{
    return DIMMABLELIGHT_LIGHT_ENDPOINT;
}

/****************************************************************************
 *
 * NAME: vApp_ZCL_ResetDeviceStructure
 *
 * DESCRIPTION:
 * Resets the device structure
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vApp_ZCL_ResetDeviceStructure(void)
{
    FLib_MemSet(&sLight,0,sizeof(tsZLO_DimmableLightDevice));
}

#if 1
/****************************************************************************
 *
 * NAME: vUpdateBulbFromZCL
 *
 * DESCRIPTION:
 * Copys the ZCL bulb state to the bulb driver
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vUpdateBulbFromZCL(bool_t bResetInterpolation)
{
    /*
     * Monochrome bulb with level control
     */
    if (bResetInterpolation)
    {
        vLI_SetCurrentValues(sLight.sLevelControlServerCluster.u8CurrentLevel,
                            0, 0, 0, 0 );
    }

    vSetBulbState(sLight.sOnOffServerCluster.bOnOff, sLight.sLevelControlServerCluster.u8CurrentLevel);

    u8StateChangeTick = BULB_SAVE_DELAY_SEC;
}
#endif

/****************************************************************************
 *
 * NAME: APP_vHandleIdentify
 *
 * DESCRIPTION:
 * ZLO Device Specific identify
 *
 * PARAMETER: the identify time
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void APP_vHandleIdentify(uint16 u16Time) {
static bool bActive = FALSE;

    if (sIdEffect.u8Effect < E_CLD_IDENTIFY_EFFECT_STOP_EFFECT) {
        /* do nothing */

    }
    else if (u16Time == 0)
    {
        /*
         * Restore to off/off state
         */
        vSetBulbState( sLight.sOnOffServerCluster.bOnOff, sLight.sLevelControlServerCluster.u8CurrentLevel);
        bActive = FALSE;
    }
    else
    {
        /* Set the Identify levels */
        if (!bActive) {
            bActive = TRUE;
            sIdEffect.u8Level = 250;
            sIdEffect.u8Count = 5;
            vSetBulbState( TRUE, CLD_LEVELCONTROL_MAX_LEVEL );
        }
    }
}

/****************************************************************************
 *
 * NAME: vIdEffectTick
 *
 * DESCRIPTION:
 * ZLO Device Specific identify tick
 *
 * PARAMETER: uint8 End Point Identifier
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vIdEffectTick(uint8 u8Endpoint) {

    if (u8Endpoint != DIMMABLELIGHT_LIGHT_ENDPOINT) {
        return;
    }

    if (sIdEffect.u8Effect < E_CLD_IDENTIFY_EFFECT_STOP_EFFECT)
    {
        if (sIdEffect.u8Tick > 0)
        {
            sIdEffect.u8Tick--;
            /* Set the light parameters */

            vSetBulbState(TRUE, sIdEffect.u8Level);

            /* Now adjust parameters ready for for next round */
            switch (sIdEffect.u8Effect) {
                case E_CLD_IDENTIFY_EFFECT_BLINK:
                    break;

                case E_CLD_IDENTIFY_EFFECT_BREATHE:
                    if (sIdEffect.bDirection) {
                        if (sIdEffect.u8Level >= 250) {
                            sIdEffect.u8Level -= 50;
                            sIdEffect.bDirection = 0;
                        } else {
                            sIdEffect.u8Level += 50;
                        }
                    } else {
                        if (sIdEffect.u8Level == 0) {
                            // go back up, check for stop
                            sIdEffect.u8Count--;
                            if ((sIdEffect.u8Count) && ( !sIdEffect.bFinish)) {
                                sIdEffect.u8Level += 50;
                                sIdEffect.bDirection = 1;
                            } else {
                                /* lpsw2773 - stop the effect on the next tick */
                                sIdEffect.u8Tick = 0;
                            }
                        } else {
                            sIdEffect.u8Level -= 50;
                        }
                    }
                    break;
                case E_CLD_IDENTIFY_EFFECT_OKAY:
                    if ((sIdEffect.u8Tick == 10) || (sIdEffect.u8Tick == 5)) {
                        sIdEffect.u8Level ^= 254;
                        if (sIdEffect.bFinish) {
                            sIdEffect.u8Tick = 0;
                        }
                    }
                    break;
                case E_CLD_IDENTIFY_EFFECT_CHANNEL_CHANGE:
                    if ( sIdEffect.u8Tick == 74) {
                        sIdEffect.u8Level = 1;
                        if (sIdEffect.bFinish) {
                            sIdEffect.u8Tick = 0;
                        }
                    }
                    break;
                default:
                    if ( sIdEffect.bFinish ) {
                        sIdEffect.u8Tick = 0;
                    }
                }
        } else {
            /*
             * Effect finished, restore the light
             */
            sIdEffect.u8Effect = E_CLD_IDENTIFY_EFFECT_STOP_EFFECT;
            sIdEffect.bDirection = FALSE;
            APP_ZCL_vSetIdentifyTime(0);
            vSetBulbState( sLight.sOnOffServerCluster.bOnOff, sLight.sLevelControlServerCluster.u8CurrentLevel);
        }
    } else {
        if (sLight.sIdentifyServerCluster.u16IdentifyTime) {
            sIdEffect.u8Count--;
            if (0 == sIdEffect.u8Count) {
                sIdEffect.u8Count = 5;
                if (sIdEffect.u8Level) {
                    sIdEffect.u8Level = 0;
                    vSetBulbState( FALSE, 0);
                }
                else
                {
                    sIdEffect.u8Level = 250;
                    vSetBulbState( TRUE, CLD_LEVELCONTROL_MAX_LEVEL);
                }
            }
        }
    }
}

/****************************************************************************
 *
 * NAME: vStartEffect
 *
 * DESCRIPTION:
 * ZLO Device Specific identify effect set up
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vStartEffect(uint8 u8Effect) {
    switch (u8Effect) {
        case E_CLD_IDENTIFY_EFFECT_BLINK:
            sIdEffect.u8Effect = E_CLD_IDENTIFY_EFFECT_BLINK;
            if (sLight.sOnOffServerCluster.bOnOff) {
                sIdEffect.u8Level = 0;
            } else {
                sIdEffect.u8Level = 250;
            }
            sIdEffect.bFinish = FALSE;
            APP_ZCL_vSetIdentifyTime(2);
            sIdEffect.u8Tick = 10;
            break;
        case E_CLD_IDENTIFY_EFFECT_BREATHE:
            sIdEffect.u8Effect = E_CLD_IDENTIFY_EFFECT_BREATHE;
            sIdEffect.bDirection = 1;
            sIdEffect.bFinish = FALSE;
            sIdEffect.u8Level = 0;
            sIdEffect.u8Count = 15;
            APP_ZCL_vSetIdentifyTime(17);
            sIdEffect.u8Tick = 200;
            break;
        case E_CLD_IDENTIFY_EFFECT_OKAY:
            sIdEffect.u8Effect = E_CLD_IDENTIFY_EFFECT_OKAY;
            sIdEffect.bFinish = FALSE;
            if (sLight.sOnOffServerCluster.bOnOff) {
                sIdEffect.u8Level = 0;
            } else {
                sIdEffect.u8Level = 254;
            }
            APP_ZCL_vSetIdentifyTime(3);
            sIdEffect.u8Tick = 15;
            break;
        case E_CLD_IDENTIFY_EFFECT_CHANNEL_CHANGE:
            sIdEffect.u8Effect = E_CLD_IDENTIFY_EFFECT_CHANNEL_CHANGE;
            sIdEffect.u8Level = 254;
            sIdEffect.bFinish = FALSE;
            APP_ZCL_vSetIdentifyTime(9);
            sIdEffect.u8Tick = 80;
            break;

        case E_CLD_IDENTIFY_EFFECT_FINISH_EFFECT:
            if (sIdEffect.u8Effect < E_CLD_IDENTIFY_EFFECT_STOP_EFFECT)
            {
                DBG_vPrintf(TRACE_LIGHT_TASK, "\n<FINISH>");
                sIdEffect.bFinish = TRUE;
            }
            break;
        case E_CLD_IDENTIFY_EFFECT_STOP_EFFECT:
            sIdEffect.u8Effect = E_CLD_IDENTIFY_EFFECT_STOP_EFFECT;
            APP_ZCL_vSetIdentifyTime(1);
            break;
    }
}

/****************************************************************************
 *
 * NAME: vSetBulbState
 *
 * DESCRIPTION:
 * ZLL Device Specific build driver interface
 *
 * PARAMETER: the on/off state, the level
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vSetBulbState(bool bOn, uint8 u8Level)
{
    if (bOn)
    {
        vLI_Start(u8Level, 0,0,0,0);
    }
    else
    {
        vLI_Stop();
    }
    vBULB_SetOnOff(bOn);
}



#if 0
#if (!defined OM15081)  && (!defined DR1173) && (!defined OM15045)
PUBLIC void APP_cbTimerButtonScan(void *pvParam)
{

}
#endif
#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
