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
