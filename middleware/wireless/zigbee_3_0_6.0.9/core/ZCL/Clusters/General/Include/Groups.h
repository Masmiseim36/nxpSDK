/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef GROUPS_H
#define GROUPS_H

/*!
\file       Groups.h
\brief      Header for Groups Cluster
*/
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/     
#include <jendefs.h>
#include "dlist.h"
#include "zcl.h"
#include "zcl_customcommand.h"
#include "zcl_options.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Cluster ID's */
#define GENERAL_CLUSTER_ID_GROUPS                   0x0004

/* Set this to the size used for the group addressing table in the .zpscfg file */
#ifndef CLD_GROUPS_MAX_NUMBER_OF_GROUPS
#define CLD_GROUPS_MAX_NUMBER_OF_GROUPS             (8)
#endif

/* 3.6.2.2.1 */
/* Set this to configure the maximum length of the group name */
#ifndef CLD_GROUPS_MAX_GROUP_NAME_LENGTH
  #ifdef CLD_GROUPS_DISABLE_NAME_SUPPORT
    #define CLD_GROUPS_MAX_GROUP_NAME_LENGTH            (0)
  #else
    #define CLD_GROUPS_MAX_GROUP_NAME_LENGTH            (16)
  #endif
#endif

/* Name Support */
#ifndef CLD_GROUPS_DISABLE_NAME_SUPPORT
#define CLD_GROUPS_NAME_SUPPORT                     (1 << 7)
#else
#define CLD_GROUPS_NAME_SUPPORT                     (0)
#endif

#ifndef CLD_GROUPS_CLUSTER_REVISION
    #define CLD_GROUPS_CLUSTER_REVISION                         1
#endif 

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Return values - Extensions to teZCL_Status*/
typedef enum 
{
    E_CLD_GROUPS_TABLE_SIZE_MISMATCH = 0x80,
    E_CLD_GROUPS_DUPLICATE_EXISTS,
    E_CLD_GROUPS_GROUP_HAS_NO_TEXT
} teCLD_GroupsStatus;


/* Command codes */
typedef enum 
{
    E_CLD_GROUPS_CMD_ADD_GROUP               = 0x00,    /* Mandatory */
    E_CLD_GROUPS_CMD_VIEW_GROUP,                        /* Mandatory */
    E_CLD_GROUPS_CMD_GET_GROUP_MEMBERSHIP,              /* Mandatory */
    E_CLD_GROUPS_CMD_REMOVE_GROUP,                      /* Mandatory */
    E_CLD_GROUPS_CMD_REMOVE_ALL_GROUPS,                 /* Mandatory */
    E_CLD_GROUPS_CMD_ADD_GROUP_IF_IDENTIFYING           /* Mandatory */
} teCLD_Groups_Command;


/* Attribute ID's */
typedef enum 
{
    /* Groups attribute set attribute ID's (3.6.2.2) */
    E_CLD_GROUPS_ATTR_ID_NAME_SUPPORT        = 0x0000,   /* Mandatory */
} teCLD_Groups_ClusterID;


/* Command status values */
typedef enum 
{
    E_CLD_GROUPS_CMD_STATUS_SUCCESS             = 0x00,
    E_CLD_GROUPS_CMD_STATUS_FAILURE,
    E_CLD_GROUPS_CMD_STATUS_INVALID_FIELD       = 0x85,
    E_CLD_GROUPS_CMD_STATUS_INSUFFICIENT_SPACE  = 0x89,
    E_CLD_GROUPS_CMD_STATUS_DUPLICATE_EXISTS,
    E_CLD_GROUPS_CMD_STATUS_NOT_FOUND
} teCLD_Groups_ResponseStatus;

#if (defined(ZCLIP_ENABLED) && defined(ZCLIP_GROUPS_SERVER_CMD_EXTENSIONS))
/* TLV tag command Extensions */
typedef enum
{
    E_CLD_GROUPS_CMD_TLV_TAG_ADDR_ASSIGNMENT_MODE       = 0x01,
    E_CLD_GROUPS_CMD_TLV_TAG_MULTICAST_IPV6_ADDR        = 0x02,
    E_CLD_GROUPS_CMD_TLV_TAG_PORT                       = 0x03
} teCLD_Groups_Command_Extension_Tls_Tag;
#endif

/* Groups Cluster */
typedef struct
{
#ifdef GROUPS_SERVER    
    zbmap8                  u8NameSupport;
#endif    
    zuint16                 u16ClusterRevision;
} tsCLD_Groups;


/* Payloads for custom commands */

/* Add Group Request Payload */
typedef struct
{
    zuint16                 u16GroupId;
    tsZCL_CharacterString   sGroupName;
#if (defined(ZCLIP_ENABLED) && defined(ZCLIP_GROUPS_SERVER_CMD_EXTENSIONS))
    zuint8                  u8TlvCount;
    tsZCL_Tlv              *psTlvList;
#endif
} tsCLD_Groups_AddGroupRequestPayload;


/* Add Group Response Payload */
typedef struct
{
    zenum8                  eStatus;
    zuint16                 u16GroupId;
} tsCLD_Groups_AddGroupResponsePayload;


/* View Group Request Payload */
typedef struct
{
    zuint16                 u16GroupId;
} tsCLD_Groups_ViewGroupRequestPayload;


/* View Group Response Payload */
typedef struct
{
    zenum8                  eStatus;
    zuint16                 u16GroupId;
    tsZCL_CharacterString   sGroupName;
} tsCLD_Groups_ViewGroupResponsePayload;


/* Get Group Membership Request Payload */
typedef struct
{
    zuint8                  u8GroupCount;
    zint16                 *pi16GroupList;
} tsCLD_Groups_GetGroupMembershipRequestPayload;


/* Get Group Membership Response Payload */
typedef struct
{
    zuint8                  u8Capacity;
    zuint8                  u8GroupCount;
    zint16                 *pi16GroupList;
} tsCLD_Groups_GetGroupMembershipResponsePayload;


/* Remove Group Request Payload */
typedef struct
{
    zuint16                 u16GroupId;
} tsCLD_Groups_RemoveGroupRequestPayload;


/* Remove Group Response Payload */
typedef struct
{
    zenum8                  eStatus;
    zuint16                 u16GroupId;
} tsCLD_Groups_RemoveGroupResponsePayload;


/* Definition of On-Off Call back Event Structure */
typedef struct
{
    uint8                                       u8CommandId;
    union
    {
        tsCLD_Groups_AddGroupRequestPayload             *psAddGroupRequestPayload;
        tsCLD_Groups_AddGroupResponsePayload            *psAddGroupResponsePayload;
        tsCLD_Groups_ViewGroupRequestPayload            *psViewGroupRequestPayload;
        tsCLD_Groups_ViewGroupResponsePayload           *psViewGroupResponsePayload;
        tsCLD_Groups_GetGroupMembershipRequestPayload   *psGetGroupMembershipRequestPayload;
        tsCLD_Groups_GetGroupMembershipResponsePayload  *psGetGroupMembershipResponsePayload;
        tsCLD_Groups_RemoveGroupRequestPayload          *psRemoveGroupRequestPayload;
        tsCLD_Groups_RemoveGroupResponsePayload         *psRemoveGroupResponsePayload;
    } uMessage;
} tsCLD_GroupsCallBackMessage;


/* Group entry */
typedef struct
{
    DNODE   dllGroupNode;
    uint16  u16GroupId;
    uint8   au8GroupName[CLD_GROUPS_MAX_GROUP_NAME_LENGTH + 1];
} tsCLD_GroupTableEntry;


/* Custom data structure */
typedef struct
{
    DLIST   lGroupsAllocList;
    DLIST   lGroupsDeAllocList;
    bool    bIdentifying;
    tsZCL_ReceiveEventAddress sReceiveEventAddress;
    tsZCL_CallBackEvent sCustomCallBackEvent;
    tsCLD_GroupsCallBackMessage sCallBackMessage;
#if (defined CLD_GROUPS) && (defined GROUPS_SERVER)
    tsCLD_GroupTableEntry asGroupTableEntry[CLD_GROUPS_MAX_NUMBER_OF_GROUPS];
#endif
} tsCLD_GroupsCustomDataStructure;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_GroupsCreateGroups(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits,
                tsCLD_GroupsCustomDataStructure    *psCustomDataStructure,
                tsZCL_EndPointDefinition           *psEndPointDefinition);

PUBLIC teZCL_Status eCLD_GroupsAdd(uint8 u8SourceEndPointId,
                                   uint16 u16GroupId,
                                   uint8 *pu8GroupName);

PUBLIC teZCL_Status eCLD_GroupsCommandAddGroupRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Groups_AddGroupRequestPayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCommandViewGroupRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Groups_ViewGroupRequestPayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCommandGetGroupMembershipRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Groups_GetGroupMembershipRequestPayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCommandRemoveGroupRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Groups_RemoveGroupRequestPayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCommandRemoveAllGroupsRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber);

PUBLIC teZCL_Status eCLD_GroupsCommandAddGroupIfIdentifyingRequestSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Groups_AddGroupRequestPayload *psPayload);

PUBLIC teZCL_Status eCLD_GroupsCheckGroupExists(
                tsZCL_EndPointDefinition    *psEndPointDefinition,
                uint16                      u16GroupId);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern tsZCL_ClusterDefinition sCLD_Groups;
extern const tsZCL_AttributeDefinition asCLD_GroupsClusterAttributeDefinitions[];
extern uint8 au8GroupsAttributeControlBits[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* GROUPS_H */
