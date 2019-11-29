/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       app_start_switch.c
\brief      ZLO Dimmmer Switch Application Initialisation and Startup
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>

#include "fsl_cop.h"
#include "PWR_Interface.h"
#include "MemManager.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "app_cfg.h"
#include "pwrm.h"
#include "pdum_nwk.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "app_zcl_cfg.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
//#include "AppApi.h"
#include "app_events.h"
#include "app_zlo_switch_node.h"
#include "app_buttons.h"
#include <string.h>
#include "ZTimer.h"
#ifdef CLD_OTA
    #include "app_ota_client.h"
#endif
#include "zcl_options.h"
#include "app_common.h"
#include "app_main.h"
#include "portmacro.h"
#ifdef APP_NTAG_ICODE
#include "ntag_nwk.h"
#include "app_ntag_icode.h"
#endif
#ifdef APP_NTAG_AES
#include "ntag_nwk.h"
#include "app_ntag_aes.h"
#endif
#include "bdb_api.h"
#include "SerialManager.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_START_UP
    #define TRACE_START TRUE
#else
    #define TRACE_START FALSE
#endif

#define RAM_HELD 2

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void APP_vInitialise(void);
void vfExtendedStatusCallBack (ZPS_teExtendedStatus eExtendedStatus);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

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
     DBG_vPrintf( TRACE_START,"APP: Dimmer-Switch Power Up\r\n" );

    /*
     * Catch resets due to watchdog timer expiry. Comment out to harden code.
     */
    DBG_vPrintf(TRACE_START,"\r\n");
    DBG_vPrintf(TRACE_START, "***********************************************\r\n");
    DBG_vPrintf(TRACE_START, "* Dimmer Switch RESET                         *\r\n");
    DBG_vPrintf(TRACE_START, "***********************************************\r\n");

    DBG_vPrintf( TRACE_START,"APP: Entering APP_vInitResources()\r\n");
    APP_vInitResources();

    DBG_vPrintf( TRACE_START,"APP: Entering APP_vInitialise()\r\n" );
    APP_vInitialise();

#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    DBG_vPrintf(TRACE_START, "\nAPP: Entering APP_vNtagPdmLoad()");
    /* Didn't start BDB using PDM data ? */
    if (FALSE == APP_bNtagPdmLoad())
#endif
    {		  
        DBG_vPrintf(TRACE_START,"APP: Entering BDB_vStart()\r\n" );
        BDB_vStart();

#ifdef APP_NTAG_AES
        DBG_vPrintf(TRACE_START, "\nAPP: Entering APP_vNtagStart()");
        APP_vNtagStart(NFC_NWK_NSC_DEVICE_ZIGBEE_ROUTER_DEVICE);
#endif
    }

#ifdef APP_NTAG_ICODE
    /* Not waking from deep sleep ? */
    if (0 == (u16AHI_PowerStatus() & (1 << 11)))
    {
        DBG_vPrintf(TRACE_START, "\nAPP: Entering APP_vNtagStart()");
        APP_vNtagStart(DIMMERSWITCH_SWITCH_ENDPOINT);
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
 * NAME: APP_vInitialise
 *
 * DESCRIPTION:
 * Initialises Zigbee stack, hardware and application.
 *
 *
 * RETURNS:
 * void
 ****************************************************************************/
PRIVATE void APP_vInitialise(void)
{
#if cPWR_UsePowerDownMode
    /* Initialise Power Manager even on non-sleeping nodes as it allows the
     * device to doze when in the idle task */
    PWRM_vInit(3);
#endif

    /* Initialise the Persistent Data Manager */
    PDM_eInitialise(700, 63);
	
    /* Initialise Protocol Data Unit Manager */
    PDUM_vInit();

    ZPS_vExtendedStatusSetCallback(vfExtendedStatusCallBack);
    /* Initialise application */
    APP_vInitialiseNode();
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
void vfExtendedStatusCallBack (ZPS_teExtendedStatus eExtendedStatus)
{
    DBG_vPrintf(TRACE_START,"ERROR: Extended status %x\r\n", eExtendedStatus);
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
