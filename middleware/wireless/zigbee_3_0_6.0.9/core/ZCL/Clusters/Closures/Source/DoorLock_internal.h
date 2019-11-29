/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/



#ifndef  DOORLOCK_INTERNAL_H_INCLUDED
#define  DOORLOCK_INTERNAL_H_INCLUDED

/*!
\file       DoorLock_internal.h
\brief      The internal API for the Door Lock Cluster
*/
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "DoorLock.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_DoorLockCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);
                    
#ifdef DOOR_LOCK_CLIENT                  
PUBLIC teZCL_Status eCLD_DoorLockCommandLockUnlockResponseReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_DoorLock_LockUnlockResponsePayload *psPayload);                    
#endif 
   
#ifdef DOOR_LOCK_SERVER            
PUBLIC teZCL_Status eCLD_DoorLockCommandLockUnlockRequestReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber);
PUBLIC teZCL_Status eCLD_DoorLockCommandLockUnlockResponseSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    teCLD_DoorLock_CommandID    eCommand,
                    tsCLD_DoorLock_LockUnlockResponsePayload *psPayload);
#endif
                 
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* DOORLOCK_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
