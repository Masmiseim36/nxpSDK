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


#ifndef  SCENES_INTERNAL_H_INCLUDED
#define  SCENES_INTERNAL_H_INCLUDED

/*!
\file       Scenes_internal.h
\brief      The internal API for the Scenes Cluster
*/     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "Scenes.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC teZCL_Status eCLD_ScenesCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

#ifdef SCENES_SERVER
PUBLIC void vCLD_ScenesGenerateIndividualAttributeCallback(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16 u16AttributeId);
                    
PUBLIC teZCL_Status eCLD_ScenesCommandAddSceneResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesAddSceneResponsePayload *psPayload);
                    
PUBLIC teZCL_Status eCLD_ScenesCommandViewSceneResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesViewSceneResponsePayload *psPayload);
                    
PUBLIC teZCL_Status eCLD_ScenesCommandRemoveSceneResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesRemoveSceneResponsePayload *psPayload);

PUBLIC teZCL_Status eCLD_ScenesCommandRemoveAllScenesResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesRemoveAllScenesResponsePayload *psPayload);


PUBLIC teZCL_Status eCLD_ScenesCommandStoreSceneResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesStoreSceneResponsePayload *psPayload);

PUBLIC teZCL_Status eCLD_ScenesCommandGetSceneMembershipResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesGetSceneMembershipResponsePayload *psPayload);

#ifdef  CLD_SCENES_CMD_ENHANCED_ADD_SCENE                    
PUBLIC teZCL_Status eCLD_ScenesCommandEnhancedAddSceneResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesEnhancedAddSceneResponsePayload *psPayload);
#endif

#ifdef CLD_SCENES_CMD_ENHANCED_VIEW_SCENE
PUBLIC teZCL_Status eCLD_ScenesCommandEnhancedViewSceneResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesEnhancedViewSceneResponsePayload *psPayload);
#endif

#ifdef CLD_SCENES_CMD_COPY_SCENE
PUBLIC teZCL_Status eCLD_ScenesCommandCopySceneResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_ScenesCopySceneResponsePayload *psPayload);
#endif

PUBLIC teZCL_Status eCLD_ScenesAddScene(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    tsCLD_ScenesAddSceneRequestPayload *psPayload,
                    bool_t                      bEnhanced);

PUBLIC teZCL_Status eCLD_ScenesStoreScene(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                      u16GroupId,
                    uint8                       u8SceneId,
                    tsCLD_ScenesTableEntry      **ppsTableEntry);

PUBLIC teZCL_Status eCLD_ScenesFindScene(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                      u16GroupId,
                    uint8                       u8SceneId,
                    tsCLD_ScenesTableEntry      **ppsTableEntry);

PUBLIC teZCL_Status eCLD_ScenesFindActiveScene(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    tsCLD_ScenesTableEntry      **ppsTableEntry);

PUBLIC teZCL_Status eCLD_ScenesListScenes(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                      u16GroupId,
                    uint8                       *pu8NumberOfScenes,
                    uint8                       *pau8ListOfScenes);

PUBLIC teZCL_Status eCLD_ScenesCountScenes(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint8                       *pu8NumberOfScenes);

PUBLIC teZCL_Status eCLD_ScenesRemoveScene(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    tsCLD_ScenesRemoveSceneRequestPayload *psPayload);

PUBLIC teZCL_Status eCLD_ScenesRemoveAllScenes(
                    uint8                        u8SourceEndPointId,
                    uint16                       u16GroupId,
                    uint64                       u64LastConfiguredBy);

#ifdef CLD_SCENES_CMD_COPY_SCENE
PUBLIC teZCL_Status eCLD_ScenesCopyScene(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                      u16FromGroupId,
                    uint8                       u8FromSceneId,
                    uint16                      u16ToGroupId,
                    uint8                       u8ToSceneId);

PUBLIC teZCL_Status eCLD_ScenesCopyAllScenes(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                      u16FromGroupId,
                    uint16                      u16ToGroupId);
#endif

PUBLIC teZCL_Status eCLD_ScenesSaveClusters(
                    tsZCL_EndPointDefinition *psEndPointDefinition,
                    uint16 u16MaxLength,
                    uint16 *pu16Length,
                    uint8 *pu8Buffer,
                    bool_t bIsTempory );

PUBLIC teZCL_Status eCLD_ScenesRestoreClusters(
                    tsZCL_EndPointDefinition *psEndPointDefinition,
                    uint16 u16Length,
                    uint8 *pu8Buffer,
                    uint32 u32RemainingTime);
#endif /* SCENES_SERVER */

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* SCENES_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
