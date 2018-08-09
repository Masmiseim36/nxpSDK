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
\file       bdb_failure_recovery.c
\brief      BDB failure recovery implementation
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "bdb_api.h"
#include "bdb_start.h"
#include "dbg.h"
#include "PDM.h"
#include <string.h>
#include <stdlib.h>
#if (defined BDB_SUPPORT_NWK_STEERING)
#include "bdb_ns.h"
#endif
#include "bdb_fr.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

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
teFR_State eFR_State = E_FR_IDLE;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: BDB_vFrStateMachine
 *
 * DESCRIPTION:
 * Upon Poll Failure on ZED or Leave with Rejoin on ZED/ZR, stack will
 * kick off a rejoin internally.
 * 1. If this rejoin was successful BDB generates BDB_EVENT_REJOIN_SUCCESS
 *    event to application.
 * 2. If this rejoin was failure BDB enters into failure recovery state
 *    and generates BDB_EVENT_FAILURE_RECOVERY_FOR_REJOIN event to application.
 *    In failure recovery state, BDB performs rejoin cycles same as in
 *    BDB_vStart.
 *    Number of rejoin cycles are controlled by BDBC_IMP_MAX_REJOIN_CYCLES
 *    macro defined in bdb_options.h file in application.
 *
 *    a. If the rejoin was success BDB_EVENT_REJOIN_SUCCESS is generated.
 *    b. If the rejoin was unsuccessful BDB_EVENT_REJOIN_FAILURE is generated.
 *
 *
 * RETURNS:
 *
 * NOTES:
 *
 ****************************************************************************/
PUBLIC void BDB_vFrStateMachine(BDB_tsZpsAfEvent *psZpsAfEvent)
{
    BDB_tsBdbEvent sBdbEvent;

    if((eNS_State != E_NS_IDLE) || (eInitState != E_INIT_IDLE))
    {
        return;
    }

    switch(eFR_State)
    {
        case E_FR_IDLE:
            switch(psZpsAfEvent->sStackEvent.eType)
            {
                case ZPS_EVENT_NWK_FAILED_TO_JOIN:
                    DBG_vPrintf(TRACE_BDB,"BDB: FR Starting rejoin cycles!\n");
                    eFR_State = E_FR_RECOVERY;
                    sBDB.sAttrib.ebdbCommissioningStatus = E_BDB_COMMISSIONING_STATUS_IN_PROGRESS;

                    sBdbEvent.eEventType = BDB_EVENT_FAILURE_RECOVERY_FOR_REJOIN;
                    APP_vBdbCallback(&sBdbEvent);

                    BDB_vRejoinCycle(TRUE);
                    break;

                case ZPS_EVENT_NWK_JOINED_AS_ROUTER:
                case ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE:
                    DBG_vPrintf(TRACE_BDB,"BDB: FR Rejoined --- \n");
                    BDB_vRejoinSuccess();
                    break;
                default:
                    break;
            }
            break;

        case E_FR_RECOVERY:
            switch(psZpsAfEvent->sStackEvent.eType)
            {
                case ZPS_EVENT_NWK_FAILED_TO_JOIN:
                    BDB_vRejoinCycle(TRUE);
                    break;
                case ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE:
                case ZPS_EVENT_NWK_JOINED_AS_ROUTER:
                    DBG_vPrintf(TRACE_BDB,"BDB: FR Rejoined --- \n");
                    BDB_vRejoinSuccess();
                    break;
                default:
                    break;
            }
            break;

        case E_FR_GIVE_UP:
            break;

        default:
            break;
    }
}


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
