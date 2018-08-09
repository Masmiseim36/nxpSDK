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
\file       app_start.c
\brief      This is a public source file for the Base Device Demo: 
            Coordinator Initialisation
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "fsl_cop.h"
#include "PWR_Interface.h"
#include "RNG_Interface.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "app_cfg.h"
#include "pwrm.h"
#include "pdum_nwk.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "app_zps_cfg.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "zps_tsv.h"
#include "app_coordinator.h"
#include "app_main.h"
#include "portmacro.h"
#ifdef APP_NCI_ICODE
#include "nci_nwk.h"
#include "app_nci_icode.h"
#endif
#ifdef APP_NCI_AES
#include "nci_nwk.h"
#include "app_nci_aes.h"
#endif
#ifdef APP_NTAG_I2C_PLUS
#include "app_ntag_i2c_plus.h"
#endif
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef DEBUG_APP
    #define TRACE_APP   TRUE
#else
    #define TRACE_APP   FALSE
#endif

#define HALT_ON_EXCEPTION   FALSE

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void APP_vInitialise(void);
PRIVATE void vfExtendedStatusCallBack(ZPS_teExtendedStatus eExtendedStatus);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern void OSA_TimeInit(void);

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

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
 * Never returns.
 *
 ****************************************************************************/
PUBLIC void vAppMain(void)
{
    /* idle task commences here */
    DBG_vPrintf(TRUE,"\r\n");
    DBG_vPrintf(TRUE, "***********************************************\r\n");
    DBG_vPrintf(TRUE, "* COORDINATOR RESET                           *\r\n");
    DBG_vPrintf(TRUE, "***********************************************\r\n");

    DBG_vPrintf(TRACE_APP, "APP: Entering APP_vInitResources()\r\n");
    APP_vInitResources();

    DBG_vPrintf(TRACE_APP, "APP: Entering APP_vInitialise()\r\n");
    APP_vInitialise();

    DBG_vPrintf(TRACE_APP, "APP: Entering BDB_vStart()\r\n");
    BDB_vStart();

#ifdef APP_NCI_ICODE
    DBG_vPrintf(TRACE_APP, "\r\nAPP: Entering APP_vNciStart()");
    APP_vNciStart(COORDINATOR_APPLICATION_ENDPOINT);
#endif
#ifdef APP_NCI_AES
    DBG_vPrintf(TRACE_APP, "\r\nAPP: Entering APP_vNciStart()");
    APP_vNciStart(NFC_NWK_NSC_IOT_GATEWAY_DEVICE);
#endif
#ifdef APP_NTAG_I2C_PLUS
    APP_NTAG_Init();
#endif
}

/****************************************************************************
 *
 * NAME: vAppRegisterPWRMCallbacks
 *
 * DESCRIPTION:
 * Power manager callback.
 * Called to allow the application to register
 * sleep and wake callbacks.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void vAppRegisterPWRMCallbacks(void)
{
    /* nothing to register as device does not sleep */
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
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_vInitialise(void)
{
    /* Initialise Power Manager even on non-sleeping nodes as it allows the
     * device to doze when in the idle task */
    //PWRM_vInit(E_AHI_SLEEP_OSCON_RAMON);

    /* Initialise the Persistent Data Manager */
    PDM_eInitialise(700, 63);

    /* Initialise Protocol Data Unit Manager */
    PDUM_vInit();
    ZPS_vExtendedStatusSetCallback(vfExtendedStatusCallBack);

    /* Initialise application */
    APP_vInitialiseCoordinator();
}

/****************************************************************************
 *
 * NAME: vfExtendedStatusCallBack
 *
 * DESCRIPTION:
 * Callback from stack on extended error situations.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vfExtendedStatusCallBack (ZPS_teExtendedStatus eExtendedStatus)
{
    DBG_vPrintf(TRUE,"ERROR: Extended status 0x%02x\r\n", eExtendedStatus);
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
