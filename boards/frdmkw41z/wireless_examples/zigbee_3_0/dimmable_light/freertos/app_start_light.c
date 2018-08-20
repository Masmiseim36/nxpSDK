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
\file       app_start_light.c
\brief      ZLO Demo: Light Node Initialisation -Implementation
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "pwrm.h"
#include "pdum_nwk.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#ifdef CPU_MKW41Z512VHT4
#include "fsl_cop.h"
#include "fsl_rcm.h"
#include "board.h"
#include "PWR_Interface.h"
#include "NVM_Interface.h"
#include "TimersManager.h"
#include "Keyboard.h"
#else
#include "fsl_wwdt.h"
#include "fsl_iocon.h"
#endif
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "app_cfg.h"


#include "app_zcl_cfg.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "string.h"

#ifndef CPU_MKW41Z512VHT4
#include "AppApi.h"
#endif

#include "app_zlo_light_node.h"


#include "zcl_options.h"

#include "app_common.h"
#include "app_main.h"
#include "app_light_interpolation.h"
#include "DriverBulb_Shim.h"

#ifdef APP_NTAG
#include "ntag_nwk.h"
#include "app_ntag.h"
#endif


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef DEBUG_APP
#define TRACE_APP   TRUE
#else
#define TRACE_APP   FALSE
#endif

#ifdef DEBUG_START_UP
#define TRACE_START TRUE
#else
#define TRACE_START FALSE
#endif


#define HALT_ON_EXCEPTION      FALSE

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/


PRIVATE void APP_vInitialise(void);

#if (defined PDM_EEPROM)
#if TRACE_APP
PRIVATE void vPdmEventHandlerCallback(uint32 u32EventNumber, PDM_eSystemEventCode eSystemEventCode);
#endif
#endif

void vfExtendedStatusCallBack (ZPS_teExtendedStatus eExtendedStatus);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern void *_stack_low_water_mark;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern void OSA_TimeInit(void);
#ifdef PDM_EEPROM
    extern uint8 u8PDM_CalculateFileSystemCapacity();
    extern uint8 u8PDM_GetFileSystemOccupancy();
#endif
#if 0
/**
 * Power manager callback.
 * Called just before the device is put to sleep.
 */
PWRM_CALLBACK(PreSleep)
{
    DBG_vPrintf(TRACE_START, "Going to sleep ...\n");
}

/**
 * Power manager callback.
 * Called when the device wakes up.
 */
PWRM_CALLBACK(Wakeup)
{
    DBG_vReset();
    DBG_vPrintf(TRACE_START, "Woken up\n");
}

#endif
/**
 * Entry point for application from a cold start.
 */
PUBLIC void vAppMain(void)
{
#ifndef CPU_MKW41Z512VHT4
    wwdt_config_t  config;
WWDT_GetDefaultConfig(&config);

    if (((PMC->RESETCAUSE) & PMC_RESETCAUSE_WDTRESET_MASK) == PMC_RESETCAUSE_WDTRESET_MASK)
    {
    	DBG_vPrintf(TRACE_APP, "APP: Watchdog timer has reset device!\n");
#if HALT_ON_EXCEPTION
        /* Enable the WWDT clock */
        CLOCK_EnableClock(kCLOCK_WdtOsc);
        RESET_PeripheralReset(kWWDT_RST_SHIFT_RSTn);
    	WWDT_Deinit(WWDT);
    	PMC->RESETCAUSE = (PMC->RESETCAUSE);
    	while (1);
#endif
    }
#endif
    /* idle task commences here */
    DBG_vPrintf(TRUE, "***********************************************\n");
    DBG_vPrintf(TRUE, "* LIGHT NODE RESET                            *\n");
    DBG_vPrintf(TRUE, "***********************************************\n");


    DBG_vPrintf(TRACE_APP, "APP: Entering APP_vInitResources()\n");
    APP_vInitResources();



    
    /* Early call to Bulb initialisation to enable fast start up    */
    vBULB_Init();

    /*
     * Start the bulb in Off state
     * ZCL start up will turn it on if required
     */
#if (defined CLD_COLOUR_CONTROL)
    vLI_SetCurrentValues(0,0,0,0,0);
#if  !((defined DR1221) || (defined DR1221_Dimic) || (defined OM15045))
    vRGBLight_SetLevels(0,0,0,0,0);
#endif
#if  ((defined DR1221) || (defined DR1221_Dimic) || (defined OM15045))
    vTunableWhiteLightSetLevels(0,0,CLD_COLOURCONTROL_COLOUR_TEMPERATURE_PHY_MAX);
#endif
#elif ( defined MONO_WITH_LEVEL)
    vLI_SetCurrentValues(0, 0, 0, 0, 0 );
    vSetBulbState( 0, 0);
#else
    vSetBulbState( 0);
#endif

    APP_vInitialise();

#ifdef APP_NTAG
    DBG_vPrintf(TRACE_APP, "\nAPP: Entering APP_vNtagPdmLoad()");
    /* Didn't start BDB using PDM data ? */
    if (FALSE == APP_bNtagPdmLoad())
#endif
    {
        DBG_vPrintf(TRACE_APP, "APP: Entering BDB_vStart()\n");
        BDB_vStart();

#ifdef APP_NTAG
        DBG_vPrintf(TRACE_APP, "\nAPP: Entering APP_vNtagStart()");
        APP_vNtagStart(NFC_NWK_LIGHT);
#endif
    }

#ifndef CPU_MKW41Z512VHT4
    SYSCON -> MAINCLKSEL       = 2;  // PATCH FOR LL MEMORY ACCESS : SYSTEM AT 32 MHz
    SystemCoreClockUpdate();
    OSA_TimeInit();
#endif
}

/**
 * Power manager callback.
 * Called to allow the application to register
 * sleep and wake callbacks.
 */
void vAppRegisterPWRMCallbacks(void)
{
    /* nothing to register as device does not sleep */
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


/**
 * Initialises Zigbee stack, hardware and application.
 */
PRIVATE void APP_vInitialise(void)
{


    /* Initialise the Persistent Data Manager */
    PDM_eInitialise(700, 63);
#if (defined PDM_EEPROM)
#if TRACE_APP
    PDM_vRegisterSystemCallback(vPdmEventHandlerCallback);
#endif
#endif
    /* Initialise Protocol Data Unit Manager */
    PDUM_vInit();

    ZPS_vExtendedStatusSetCallback(vfExtendedStatusCallBack);
    /* initialise application */
    APP_vInitialiseNode();

}

#if (defined PDM_EEPROM)
#if TRACE_APP
/****************************************************************************
 *
 * NAME: vPdmEventHandlerCallback
 *
 * DESCRIPTION:
 * Handles PDM callback, information the application of PDM conditions
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vPdmEventHandlerCallback(uint32 u32EventNumber, PDM_eSystemEventCode eSystemEventCode)
{

    switch (eSystemEventCode) {
        /*
         * The next three events will require the application to take some action
         */
        case E_PDM_SYSTEM_EVENT_WEAR_COUNT_TRIGGER_VALUE_REACHED:
            DBG_vPrintf(TRACE_APP, "PDM: Segment %d reached trigger wear level\n", u32EventNumber);
            break;
        case E_PDM_SYSTEM_EVENT_SYSTEM_INTERNAL_BUFFER_WEAR_COUNT_SWAP:
            DBG_vPrintf(TRACE_APP, "PDM: Segment %d buffer wear count swap\n", u32EventNumber);
            break;
        case E_PDM_SYSTEM_EVENT_SYSTEM_DUPLICATE_FILE_SEGMENT_DETECTED:
            DBG_vPrintf(TRACE_APP, "PDM: Segement %d duplicate selected\n", u32EventNumber);
            break;
        default:
            DBG_vPrintf(TRACE_APP, "PDM: Unexpected call back Code %d Number %d\n", eSystemEventCode, u32EventNumber);
            break;
    }
}
#endif
#endif

#if 0  // TODO
PUBLIC void vDebug(char *pcMessage)
{
    DBG_vPrintf(TRACE_START, "%s",pcMessage);
}


PUBLIC void vDebugHex(uint32 u32Data, int iSize)
{
    DBG_vPrintf(TRACE_START, "%x",u32Data);
}
#endif


void vfExtendedStatusCallBack (ZPS_teExtendedStatus eExtendedStatus)
{
    DBG_vPrintf(TRUE,"ERROR: Extended status %x\n", eExtendedStatus);
}




/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
