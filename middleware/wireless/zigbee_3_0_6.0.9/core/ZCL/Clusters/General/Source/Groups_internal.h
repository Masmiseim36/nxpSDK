/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  GROUPS_INTERNAL_H_INCLUDED
#define  GROUPS_INTERNAL_H_INCLUDED

/*!
\file       Groups_internal.h
\brief      The internal API for the Groups Cluster
*/     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "Groups.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_GroupsCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC teZCL_Status eCLD_GroupsSetIdentifying(
                    uint8 u8SourceEndPointId,
                    bool_t bIsIdentifying);

PUBLIC teZCL_Status eCLD_GroupsCommandAddGroupResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_Groups_AddGroupResponsePayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCommandViewGroupResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_Groups_ViewGroupResponsePayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCommandViewGroupResponseReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_Groups_ViewGroupResponsePayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCommandGetGroupMembershipResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_Groups_GetGroupMembershipResponsePayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCommandRemoveGroupResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_Groups_RemoveGroupResponsePayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsAddGroup(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    tsCLD_Groups_AddGroupRequestPayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsAddGroupCheckRange(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    tsCLD_Groups_AddGroupRequestPayload *psPayload,
                    bool_t bCheckGroupIdRange);

PUBLIC teZCL_Status eCLD_GroupsFindGroup(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                      u16GroupId,
                    tsCLD_GroupTableEntry       **ppsGroupTableEntry);

PUBLIC int iCLD_GroupsListGroups(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    int16 *pai16List);

PUBLIC int iCLD_GroupsCountGroups(tsZCL_EndPointDefinition    *psEndPointDefinition);

PUBLIC int iCLD_GroupsListMatchingGroups(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    int16 *pai16MatchList,
                    uint8 u8ItemsInMatchList,
                    int16 *pai16List);

PUBLIC teZCL_Status eCLD_GroupsRemoveGroup(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    tsCLD_Groups_RemoveGroupRequestPayload *psPayload,
                    uint64                      u64SrcIEEEAddress);

PUBLIC teZCL_Status eCLD_GroupsRemoveAllGroups(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint64                      u64SrcIEEEAddress);


/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* GROUPS_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
