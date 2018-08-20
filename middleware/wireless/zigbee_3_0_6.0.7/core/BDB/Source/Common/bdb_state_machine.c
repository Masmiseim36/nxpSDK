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
\file       bdb_state_machine.c
\brief      BDB handling of APP_vGenCallback and bdb_taskBDB
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "FunctionLib.h"
#include "jendefs.h"
#include "bdb_api.h"
#include "bdb_start.h"
#if ((defined BDB_SUPPORT_FIND_AND_BIND_INITIATOR) || (defined BDB_SUPPORT_FIND_AND_BIND_TARGET))
#include "bdb_fb_api.h"
#endif
#if (defined BDB_SUPPORT_TOUCHLINK)
#include "bdb_tl.h"
#endif
#if (defined BDB_SUPPORT_NWK_STEERING)
#include "bdb_ns.h"
#endif
#if (defined BDB_SUPPORT_NWK_FORMATION)
#include "bdb_nf.h"
#endif
#if (defined BDB_SUPPORT_OOBC)
#include "bdb_DeviceCommissioning.h"
#endif
#include "dbg.h"
#include "bdb_fr.h"
#include <string.h>
#include <stdlib.h>
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
PUBLIC tsBDB sBDB;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vGenCallback
 *
 * DESCRIPTION:
 * Event handler called by ZigBee PRO Stack
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vGenCallback(uint8 u8Endpoint, ZPS_tsAfEvent *psStackEvent)
{
    BDB_tsZpsAfEvent sZpsAfEvent;

    DBG_vPrintf(TRACE_BDB, "BDB: APP_vGenCallback [%d %d] \n", u8Endpoint, psStackEvent->eType);

    /* Before passing to queue; copy is required to combine two arguments from stack */
    sZpsAfEvent.u8EndPoint = u8Endpoint;
    FLib_MemCpy(&sZpsAfEvent.sStackEvent, psStackEvent, sizeof(ZPS_tsAfEvent));

    /* Post the task to break stack context */
    if(ZQ_bQueueSend(sBDB.hBdbEventsMsgQ, &sZpsAfEvent) == FALSE)
    {
        DBG_vPrintf(TRACE_BDB, "BDB: Failed to post zpsEvent %d \n", psStackEvent->eType);
    }
}
/****************************************************************************
 *
 * NAME: bdb_taskBDB
 *
 * DESCRIPTION:
 * Processes events from the bdb event queue
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void bdb_taskBDB(void)
{
    BDB_tsZpsAfEvent sZpsAfEvent;
    BDB_tsBdbEvent sBDBEvent = {0};

    while(ZQ_bQueueReceive(sBDB.hBdbEventsMsgQ, &sZpsAfEvent))
    {
        /* Do not transmit link key for know node rejoins and BDB_JOIN_USES_INSTALL_CODE_KEY is TRUE */
        #if ((BDB_SET_DEFAULT_TC_POLICY == TRUE) &&  (BDB_JOIN_USES_INSTALL_CODE_KEY == TRUE))
            if((ZPS_ZDO_DEVICE_COORD == ZPS_eAplZdoGetDeviceType()) &&
               (ZPS_EVENT_TC_STATUS == sZpsAfEvent.sStackEvent.eType) &&
               (0x00 == sZpsAfEvent.sStackEvent.uEvent.sApsTcEvent.u8Status))
            {
                uint64 u64DeviceAddr;

                u64DeviceAddr = ZPS_u64NwkNibGetMappedIeeeAddr(ZPS_pvAplZdoGetNwkHandle(),
                                                               sZpsAfEvent.sStackEvent.uEvent.sApsTcEvent.uTcData.pKeyDesc->u16ExtAddrLkup);
                DBG_vPrintf(TRACE_BDB,"ZPS_EVENT_TC_STATUS %016llx \n",u64DeviceAddr);
                ZPS_bAplZdoTrustCenterSetDevicePermissions(u64DeviceAddr,
                                                           ZPS_DEVICE_PERMISSIONS_JOIN_DISALLOWED);
            }
        #endif
        /* Call BDB state machines */
        BDB_vFrStateMachine(&sZpsAfEvent); // Failure Recovery

        BDB_vInitStateMachine(&sZpsAfEvent);

        #if (defined BDB_SUPPORT_NWK_STEERING)
            BDB_vNsStateMachine(&sZpsAfEvent);
        #endif

        #if (defined BDB_SUPPORT_NWK_FORMATION)
            BDB_vNfStateMachine(&sZpsAfEvent);
        #endif

        #if (defined BDB_SUPPORT_FIND_AND_BIND_INITIATOR)
            vFbZdpHandler(&sZpsAfEvent.sStackEvent);
        #endif

        #if (defined BDB_SUPPORT_OOBC)
            BDB_vOutOfBandCommissionHandleEvent(&sZpsAfEvent);
        #endif

        /* Call application */
        sBDBEvent.eEventType = BDB_EVENT_ZPSAF;
        FLib_MemCpy(&sBDBEvent.uEventData.sZpsAfEvent,&sZpsAfEvent,sizeof(BDB_tsZpsAfEvent));
        APP_vBdbCallback(&sBDBEvent);
    }
}

/****************************************************************************
 *
 * NAME: BDB_vZclEventHandler
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:      Name            RW  Usage
 *
 *
 * RETURNS:
 *
 *
 * NOTES:
 *
 ****************************************************************************/
PUBLIC void BDB_vZclEventHandler(tsBDB_ZCLEvent *psEvent)
{
    DBG_vPrintf(TRACE_BDB, "BDB ZCL Event %d\n", psEvent->eType);
    if ( (psEvent->eType > BDB_E_ZCL_EVENT_NONE)  &&
         (psEvent->eType < BDB_E_ZCL_EVENT_IDENTIFY_QUERY))
    {
        #if (defined BDB_SUPPORT_TOUCHLINK)
            BDB_vTlStateMachine(psEvent);
        #endif
    }
    else if (psEvent->eType >= BDB_E_ZCL_EVENT_IDENTIFY_QUERY)
    {
        #if ((defined BDB_SUPPORT_FIND_AND_BIND_INITIATOR) || (defined BDB_SUPPORT_FIND_AND_BIND_TARGET))
            BDB_vFbZclHandler(psEvent->eType,psEvent->psCallBackEvent);// identify query response or identify
        #endif
    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
