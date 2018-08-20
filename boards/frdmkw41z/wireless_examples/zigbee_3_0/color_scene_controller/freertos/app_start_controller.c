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
#include "zps_apl_af.h"
#include "ZTimer.h"
#include "zlo_controller_node.h"
#ifdef DR1159
#include "DriverCapTouch.h"
#endif
#ifdef DR1199
#include "app_buttons.h"
#endif
#include "low_bat_indicator.h"
#include "app_led_control.h"
#include "app_main.h"
#include <string.h>
#include "bdb_api.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define RAM_HELD 2

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

#ifdef DEBUG_SLEEP
#define TRACE_SLEEP TRUE
#else
#define TRACE_SLEEP FALSE
#endif

#define HALT_ON_EXCEPTION   FALSE

#define POWER_BTN (1)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void APP_vInitialise(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern void *_stack_low_water_mark;

bool_t bDeepSleep = FALSE;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Functions                                            ***/
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
 * Wakeup call back by the power manager after the controller wakes up from sleep.
 *
 * PARAMETERS:      Name            RW  Usage
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PWRM_CALLBACK(Wakeup)
{
}

/****************************************************************************
 *
 * NAME: vAppMain
 *
 * DESCRIPTION:
 *
 * Entry point for application from a cold start.
 *
 * PARAMETERS:      Name            RW  Usage
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppMain(void)
{
#ifdef DR1159
    /* Enable the Low Battery Detection function */
     vLowBatIndicationInit( (LHS_LED_DIO | RHS_LED_DIO), E_BO_TRIP_2V4 );
#endif

    /* idle task commences here */
    DBG_vPrintf(TRACE_APP, "\n***********************************************\n");
    DBG_vPrintf(TRACE_APP, "REMOTE NODE RESET                              \n");
    DBG_vPrintf(TRACE_APP, "***********************************************\n");

    DBG_vPrintf(TRACE_APP, "APP: Entering APP_vInitResources()\n");
    APP_vInitResources();

    DBG_vPrintf(TRACE_APP, "APP: Entering APP_vInitialise()\n");
    APP_vInitialise();

    DBG_vPrintf(TRACE_APP, "APP: Entering BDB_vStart()\n");
    BDB_vStart();
}


/****************************************************************************
 *
 * NAME: vAppMain
 *
 * DESCRIPTION:
 *
 * Power manager callback.
 * Called to allow the application to register
 * sleep and wake callbacks.
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
 *
 * Initialises Zigbee stack, hardware and application.
 *
 * PARAMETERS:      Name            RW  Usage
 *
 *
 * RETURNS:
 * void
 *
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

    /* Initialise application */
    APP_vInitialiseNode();
}

/****************************************************************************
 *
 * NAME: vDebug
 *
 * DESCRIPTION:
 * prints a string
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/

PUBLIC void vDebug(char *pcMessage)
{
    DBG_vPrintf(TRACE_START, "%s",pcMessage);
}


/****************************************************************************
 *
 * NAME: vDebugHex
 *
 * DESCRIPTION:
 * prints a hex
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vDebugHex(uint32 u32Data, int iSize)
{
    DBG_vPrintf(TRACE_START, "%x",u32Data);
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
