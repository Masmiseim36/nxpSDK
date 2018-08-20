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

/*!=================================================================================================
\file       app_zb_utils.c
\brief      This is a public source file for the zigbee utils for demo application.
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>

#include "fsl_cop.h"
#include "PWR_Interface.h"
#include "LED.h"

#include "pwrm.h"
#include "pdum_nwk.h"
#include "pdum_apl.h"
#include "PDM.h"
#include "dbg.h"
#include "app_pdum_cfg.h"
#include "app_zcl_cfg.h"
#include "zps_apl_af.h"
#include "app_zlo_sensor_node.h"
#include "app_main.h"
#include "app_sensor_buttons.h"
#include <string.h>
#ifdef APP_NTAG_ICODE
#include "ntag_nwk.h"
#include "app_ntag_icode.h"
#endif
#ifdef APP_NTAG_AES
#include "ntag_nwk.h"
#include "app_ntag_aes.h"
#endif
#include "bdb_api.h"
#include "App_LightTemperatureOccupancySensor.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_START_UP
    #define TRACE_START TRUE
#else
    #define TRACE_START FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vInitialiseApp(void);
PRIVATE void vfExtendedStatusCallBack (ZPS_teExtendedStatus eExtendedStatus);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern void *_stack_low_water_mark;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/**
 * Power manager Callback.
 * Called just before the device is put to sleep
 */

static PWRM_DECLARE_CALLBACK_DESCRIPTOR(PreSleep);
/**
 * Power manager Callback.
 * Called just after the device wakes up from sleep
 */
static PWRM_DECLARE_CALLBACK_DESCRIPTOR(Wakeup);
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


/****************************************************************************
 *
 * NAME: vAppMain
 *
 * DESCRIPTION:
 * Entry point for application from a cold start.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppMain(void)
{
    /* idle task commences here */
    DBG_vPrintf(TRUE, "\n***********************************************\n");
    DBG_vPrintf(TRUE, "* LIGHT TEMPERATURE OCCUPANCY SENSOR RESET    *\n");
    DBG_vPrintf(TRUE, "***********************************************\n");
    
    DBG_vPrintf(TRACE_START, "\nAPP Entering APP_vInitResources()");
    APP_vInitResources();

    DBG_vPrintf(TRACE_START, "APP: Entering APP_vInitialise()\n");
    vInitialiseApp();

#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    DBG_vPrintf(TRACE_START, "\nAPP: Entering APP_vNtagPdmLoad()");
    /* Didn't start BDB using PDM data ? */
    if (FALSE == APP_bNtagPdmLoad())
#endif
    {
        DBG_vPrintf(TRACE_START, "APP: Entering BDB_vStart()\n");
        BDB_vStart();

        #ifdef APP_NTAG_AES
            DBG_vPrintf(TRACE_START, "\nAPP: Entering APP_vNtagStart()");
            APP_vNtagStart(NFC_NWK_NSC_DEVICE_CLIMATE_SENSOR_DEVICE);
        #endif
        if (FALSE == APP_bNodeIsInRunningState()) BDB_eNsStartNwkSteering();
    }

#ifdef APP_NTAG_ICODE
    /* Not waking from deep sleep ? */
    if (0 == (u16AHI_PowerStatus() & (1 << 11)))
    {
        DBG_vPrintf(TRACE_START, "\nAPP: Entering APP_vNtagStart()");
        APP_vNtagStart(LIGHTTEMPERATUREOCCUPANCYSENSOR_SENSOR_ENDPOINT);
    }
#endif
}

/****************************************************************************
 *
 * NAME: vAppRegisterPWRMCallbacks
 *
 * DESCRIPTION:
 * Power manager callback.
 * Called to allow the application to register  sleep and wake callbacks.
 *
 * PARAMETERS:      Name            RW  Usage
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void vAppRegisterPWRMCallbacks(void)
{
    PWRM_vRegisterPreSleepCallback(PreSleep);
    PWRM_vRegisterWakeupCallback(Wakeup);
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: vInitialiseApp
 *
 * DESCRIPTION:
 * Initialises Zigbee stack, hardware and application.
 *
 *
 * RETURNS:
 * void
 ****************************************************************************/
PRIVATE void vInitialiseApp(void)
{
#if cPWR_UsePowerDownMode
    /* Initialise Power Manager even on non-sleeping nodes as it allows the
     * device to doze when in the idle task */
    PWRM_vInit(3);
#endif

    PDM_eInitialise(700, 63);

    /* Initialise Protocol Data Unit Manager */
    PDUM_vInit();

    ZPS_vExtendedStatusSetCallback(vfExtendedStatusCallBack);

    /* Initialise application */
    APP_vInitialiseNode();
}

/****************************************************************************
 *
 * NAME: vfExtendedStatusCallBack
 *
 * DESCRIPTION:
 *
 * ZPS extended error callback .
 *
 * PARAMETERS:      Name            RW  Usage
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vfExtendedStatusCallBack (ZPS_teExtendedStatus eExtendedStatus)
{
    DBG_vPrintf(TRACE_START,"ERROR: Extended status %x\n", eExtendedStatus);
}

/****************************************************************************
 *
 * NAME: PreSleep
 *
 * DESCRIPTION:
 *
 * PreSleep call back by the power manager before the controller put into sleep.
 *
 * PARAMETERS:      Name            RW  Usage
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PWRM_CALLBACK(PreSleep)
{
}

/****************************************************************************
 *
 * NAME: Wakeup
 *
 * DESCRIPTION:
 *
 * Wakeup call back by  power manager after the controller wakes up from sleep.
 *
 ****************************************************************************/
PWRM_CALLBACK(Wakeup)
{
#ifndef CPU_MKW41Z512VHT4
    /* Initialise RGB LED */
    bRGB_LED_Enable();
    bRGB_LED_Off();
    bRGB_LED_SetGroupLevel(LED_LEVEL);


    /* Always initialise the light sensor and the RGB Led
     * the iic interface runs slower (100kHz) than that used
     * by the RGB driver (400KHz)
    */

    bool_t bStatus= bTSL2550_Init();
    DBG_vPrintf(TRACE_START,"bTSL2550_Init = %d\n",bStatus);

    DBG_vPrintf(TRACE_START, "\nAPP: Woken up (CB)");
    DBG_vPrintf(TRACE_START, "\nAPP Start: APP: Warm Waking powerStatus = 0x%04x", u16AHI_PowerStatus());

    /* If the power status is OK and RAM held while sleeping, restore the MAC settings */
    if(u16AHI_PowerStatus() & (1 << 1))
    {
        /* Restore Mac settings (turns radio on) */
        vMAC_RestoreSettings();
        DBG_vPrintf(TRACE_START, "\nAPP Start: APP: MAC settings restored");

        /* Define HIGH_POWER_ENABLE to enable high power module */
        #ifdef HIGH_POWER_ENABLE
            vAHI_HighPowerModuleEnable(TRUE, TRUE);
        #endif

        DBG_vPrintf(TRACE_START, "\nAPP: Restarting , Tick Timer = %d", u32AHI_TickTimerRead());
        APP_vSetUpHardware();

        ZTIMER_vWake();
    }
#endif
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
