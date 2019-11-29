/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_NWK_EVENT_HANDLER_H_
#define APP_NWK_EVENT_HANDLER_H_

/*!
\file       app_nwk_event_handler.h
\brief      ZLO Demo: Handles all network events like network join/leave
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <zcl.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vHandlePollResponse(ZPS_tsAfEvent* psStackEvent);
PUBLIC void vHandleNetworkLeave(ZPS_tsAfEvent* psStackEvent);
PUBLIC void vHandleNetworkLeaveConfirm(ZPS_tsAfEvent* psStackEvent);
PUBLIC void vHandleFailedRejoin(void);
PUBLIC void vHandleNetworkJoinEndDevice(void);
PUBLIC void vHandleNetworkJoinAndRejoin(void);
PUBLIC void APP_cbTimerPoll( void * pvParam);
PUBLIC void vStopPollTimerTask();
PUBLIC void vHandleFailedToJoin(void);
PUBLIC void vStartPollTimer(uint32 u32PollInterval);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_NWK_EVENT_HANDLER_H_ */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
