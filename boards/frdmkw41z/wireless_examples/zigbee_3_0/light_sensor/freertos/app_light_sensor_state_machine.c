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
\file       app_sensor_state_machine.c
\brief      ZLO Demo: Processes all stack events depending on it's state
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "dbg.h"
#include "app_light_sensor_state_machine.h"
#include "app_common.h"
#include "pwrm.h"
#include "App_LightSensor.h"

#include "app_nwk_event_handler.h"
#include "app_zbp_utilities.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "bdb_api.h"
#include "app_zlo_sensor_node.h"
#include "app_zcl_cfg.h"
#ifdef APP_NTAG_ICODE
#include "app_ntag_icode.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_SENSOR_STATE
    #define TRACE_SENSOR_STATE   TRUE
#else
    #define TRACE_SENSOR_STATE   FALSE
#endif
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
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vAppHandleRunning
 *
 * DESCRIPTION:
 * Is called to handle stack events when we are currently in the network
 *
 ****************************************************************************/
PUBLIC void vAppHandleRunning(ZPS_tsAfEvent* psStackEvent)
{

    DBG_vPrintf(TRACE_SENSOR_STATE, "\nAPP State Event: Running");

    switch( psStackEvent->eType)
    {

    case ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE:
        vHandleNetworkJoinEndDevice();
        #ifdef APP_NTAG_ICODE
        {
            /* Not a rejoin ? */
            if (FALSE == psStackEvent->uEvent.sNwkJoinedEvent.bRejoin)
            {
                /* Write network data to tag */
                APP_vNtagStart(LIGHTSENSOR_SENSOR_ENDPOINT);
            }
        }
        #endif
        break;

    case ZPS_EVENT_NWK_LEAVE_INDICATION:
        vHandleNetworkLeave(psStackEvent);
        break;

    case ZPS_EVENT_NWK_POLL_CONFIRM:
        vHandlePollResponse(psStackEvent);
        break;

    case ZPS_EVENT_NWK_LEAVE_CONFIRM:
        vHandleNetworkLeaveConfirm(psStackEvent);
        break;

    default:
        break;

    }

}

/****************************************************************************
 *
 * NAME: vAppHandleStartup
 *
 * DESCRIPTION:
 * Is called to handle stack events when we are currently not in the network
 *
 ****************************************************************************/
PUBLIC void vAppHandleStartup(void)
{
    DBG_vPrintf(TRACE_SENSOR_STATE, "\nAPP State Event: StartUp");
    BDB_eNsStartNwkSteering();
    sDeviceDesc.eNodeState = E_JOINING_NETWORK;
}

/****************************************************************************
 *
 * NAME: vStartWakeTimer
 *
 * DESCRIPTION:
 * Starts wake timer 0
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vStartWakeTimer(uint16 u16Tick)
{
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
