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
\file       ScenesClientCommands.c
\brief      Send an Scenes cluster command
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>


#include "zcl.h"
#include "zcl_customcommand.h"

#include "Scenes.h"
#include "Scenes_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"

#include "string.h"

#include "dbg.h"

#ifdef DEBUG_CLD_SCENES
#define TRACE_SCENES    TRUE
#else
#define TRACE_SCENES    FALSE
#endif

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

/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/
#ifdef SCENES_CLIENT
/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandAddSceneRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesAddSceneRequestPayload *psPayload              Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC  teZCL_Status eCLD_ScenesCommandAddSceneRequestSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_ScenesAddSceneRequestPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT16,   &psPayload->u16GroupId},
            {1,                                     E_ZCL_UINT8,    &psPayload->u8SceneId},
            {1,                                     E_ZCL_UINT16,   &psPayload->u16TransitionTime},
            {1,                                     E_ZCL_CSTRING,  &psPayload->sSceneName},
            {psPayload->sExtensionField.u16Length,  E_ZCL_UINT8,    psPayload->sExtensionField.pu8Data},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_ADD,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );

}


/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandViewSceneRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesViewSceneRequestPayload *psPayload             Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ScenesCommandViewSceneRequestSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_ScenesViewSceneRequestPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT16,   &psPayload->u16GroupId},
            {1,                                     E_ZCL_UINT8,    &psPayload->u8SceneId},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_VIEW,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );

}

/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandRemoveSceneRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesRemoveSceneRequestPayload *psPayload           Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ScenesCommandRemoveSceneRequestSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_ScenesRemoveSceneRequestPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT16,   &psPayload->u16GroupId},
            {1,                                     E_ZCL_UINT8,    &psPayload->u8SceneId},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_REMOVE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );

}

/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandRemoveAllScenesRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesRemoveAllScenesRequestPayload *psPayload       Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ScenesCommandRemoveAllScenesRequestSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_ScenesRemoveAllScenesRequestPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT16,   &psPayload->u16GroupId},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_REMOVE_ALL,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );

}


/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandStoreSceneRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesStoreSceneRequestPayload *psPayload            Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ScenesCommandStoreSceneRequestSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_ScenesStoreSceneRequestPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT16,   &psPayload->u16GroupId},
            {1,                                     E_ZCL_UINT8,    &psPayload->u8SceneId},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_STORE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );

}

/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandRecallSceneRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesRecallSceneRequestPayload *psPayload           Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ScenesCommandRecallSceneRequestSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_ScenesRecallSceneRequestPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT16,   &psPayload->u16GroupId},
            {1,                                     E_ZCL_UINT8,    &psPayload->u8SceneId},
            {1,                                     E_ZCL_UINT16,   &psPayload->u16TransitionTime},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_RECALL,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );

}

/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandGetSceneMembershipRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesGetSceneMembershipRequestPayload *psPayload    Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ScenesCommandGetSceneMembershipRequestSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_ScenesGetSceneMembershipRequestPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT16,   &psPayload->u16GroupId},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_GET_SCENE_MEMBERSHIP,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );

}


#ifdef  CLD_SCENES_CMD_ENHANCED_ADD_SCENE
/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandEnhancedAddSceneRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesEnhancedAddSceneRequestPayload *psPayload      Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_ScenesCommandEnhancedAddSceneRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_ScenesEnhancedAddSceneRequestPayload *psPayload)
{
    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT16,   &psPayload->u16GroupId},
            {1,                                     E_ZCL_UINT8,    &psPayload->u8SceneId},
            {1,                                     E_ZCL_UINT16,   &psPayload->u16TransitionTime100ms},
            {1,                                     E_ZCL_CSTRING,  &psPayload->sSceneName},
            {psPayload->sExtensionField.u16Length,  E_ZCL_UINT8,    psPayload->sExtensionField.pu8Data},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_ENHANCED_ADD_SCENE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );
}
#endif

#ifdef  CLD_SCENES_CMD_ENHANCED_VIEW_SCENE
/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandEnhancedViewSceneRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesEnhancedViewSceneRequestPayload *psPayload     Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_ScenesCommandEnhancedViewSceneRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_ScenesEnhancedViewSceneRequestPayload *psPayload)
{
    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,         E_ZCL_UINT16,   &psPayload->u16GroupId},
            {1,         E_ZCL_UINT8,    &psPayload->u8SceneId},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_ENHANCED_VIEW_SCENE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );
}
#endif

#ifdef CLD_SCENES_CMD_COPY_SCENE
/****************************************************************************
 **
 ** NAME:       eCLD_ScenesCommandCopySceneSceneRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends a scenes cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_ScenesCopySceneRequestPayload *psPayload             Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_ScenesCommandCopySceneSceneRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_ScenesCopySceneRequestPayload *psPayload)
{
    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,     E_ZCL_UINT8,    &psPayload->u8Mode},
            {1,     E_ZCL_UINT16,   &psPayload->u16FromGroupId},
            {1,     E_ZCL_UINT8,    &psPayload->u8FromSceneId},
            {1,     E_ZCL_UINT16,   &psPayload->u16ToGroupId},
            {1,     E_ZCL_UINT8,    &psPayload->u8ToSceneId},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_SCENES,
                                  FALSE,
                                  E_CLD_SCENES_CMD_COPY_SCENE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );
}
#endif
#endif /* SCENES_CLIENT */


/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
