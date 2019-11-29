/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  POLLCONTROL_INTERNAL_H_INCLUDED
#define  POLLCONTROL_INTERNAL_H_INCLUDED

/*!
\file       PollControl_internal.h
\brief      The internal API for the Poll Control
*/     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "PollControl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_PollControlCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

#ifdef POLL_CONTROL_CLIENT
PUBLIC teZCL_Status eCLD_PollControlCheckinReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber);
					
PUBLIC teZCL_Status eCLD_PollControlCheckinResponseSend(
                    uint8                                         u8SourceEndPointId,
                    uint8                                         u8DestinationEndPointId,
                    tsZCL_Address                                 *psDestinationAddress,
                    uint8                                         *pu8TransactionSequenceNumber,
					tsCLD_PollControl_CheckinResponsePayload	  *psPayload);					
#endif

#ifdef POLL_CONTROL_SERVER
PUBLIC teZCL_Status eCLD_PollControlCheckinResponseReceive(
                    ZPS_tsAfEvent               				*pZPSevent,
                    uint8                       				*pu8TransactionSequenceNumber,
					tsCLD_PollControl_CheckinResponsePayload    *psPayload);
					
PUBLIC teZCL_Status eCLD_PollControlFastPollStopReceive(
                    ZPS_tsAfEvent               				*pZPSevent,
                    uint8                       				*pu8TransactionSequenceNumber);
					
#ifdef CLD_POLL_CONTROL_CMD_SET_LONG_POLL_INTERVAL
PUBLIC teZCL_Status eCLD_PollControlSetLongPollIntervalReceive(
                    ZPS_tsAfEvent               					*pZPSevent,
                    uint8                       					*pu8TransactionSequenceNumber,
					tsCLD_PollControl_SetLongPollIntervalPayload    *psPayload);
#endif

#ifdef CLD_POLL_CONTROL_CMD_SET_SHORT_POLL_INTERVAL
PUBLIC teZCL_Status eCLD_PollControlSetShortPollIntervalReceive(
                    ZPS_tsAfEvent               					*pZPSevent,
                    uint8                       					*pu8TransactionSequenceNumber,
					tsCLD_PollControl_SetShortPollIntervalPayload   *psPayload);
#endif
				
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* POLLCONTROL_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
