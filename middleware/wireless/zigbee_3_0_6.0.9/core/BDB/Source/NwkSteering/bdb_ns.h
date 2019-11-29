/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
#ifndef BDB_NS_INCLUDED
#define BDB_NS_INCLUDED

/*!
\file       bdb_ns.h
\brief      BDB NetworkSteering API
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "bdb_api.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_NS_IDLE,
    E_NS_WAIT_DISCOVERY,
    E_NS_WAIT_JOIN,
    E_NS_TCLK_WAIT_NODE_DESC_RES,
    E_NS_WAIT_AFTER_NWK_JOIN,
    E_NS_TCLK_WAIT_SEND_REQ_KEY,
    E_NS_TCLK_WAIT_LEAVE_CFM,
}teNS_State;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC BDB_teStatus BDB_eNsStartNwkSteering(void);
PUBLIC void BDB_vNsStateMachine(BDB_tsZpsAfEvent *psZpsAfEvent);
PUBLIC void BDB_vNsTimerCb(void *pvParam);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern PUBLIC teNS_State eNS_State;
extern PUBLIC uint8 u8TimerBdbNs;

#if defined __cplusplus
}
#endif

#endif  /* BDB_NS_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/






