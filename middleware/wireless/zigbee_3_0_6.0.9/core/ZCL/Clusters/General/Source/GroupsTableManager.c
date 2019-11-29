/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       GroupsTableManager.c
\brief      Table management functions
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "FunctionLib.h"
#include <jendefs.h>

#include "zps_apl.h"
#include "zps_apl_aib.h"

#include "zcl.h"
#include "zcl_customcommand.h"

#include "Groups.h"
#include "Groups_internal.h"

#ifdef CLD_SCENES
#include "Scenes_internal.h"
#endif

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"

#include "string.h"



#include "dbg.h"

#ifdef DEBUG_CLD_GROUPS
#define TRACE_GROUPS    TRUE
#else
#define TRACE_GROUPS    FALSE
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

PRIVATE bool bCLD_GroupsSearchForGroup(void *pvSearchParam, void *psNodeUnderTest);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       eCLD_GroupsAddGroup
 **
 ** DESCRIPTION:
 ** Adds a group to the table
 **
 ** PARAMETERS:                 Name                    Usage
 ** tsZCL_EndPointDefinition    *psEndPointDefinition
 ** tsZCL_ClusterInstance       *psClusterInstance
 ** tsCLD_Groups_AddGroupRequestPayload *psPayload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_GroupsAddGroup(
                            tsZCL_EndPointDefinition    *psEndPointDefinition,
                            tsZCL_ClusterInstance       *psClusterInstance,
                            tsCLD_Groups_AddGroupRequestPayload *psPayload)
{
    
    return eCLD_GroupsAddGroupCheckRange(psEndPointDefinition, psClusterInstance, psPayload, TRUE);

}

/****************************************************************************
 **
 ** NAME:       eCLD_GroupsAddGroupCheckRange
 **
 ** DESCRIPTION:
 ** Adds a group to the table
 **
 ** PARAMETERS:                 Name                    Usage
 ** tsZCL_EndPointDefinition    *psEndPointDefinition
 ** tsZCL_ClusterInstance       *psClusterInstance
 ** tsCLD_Groups_AddGroupRequestPayload *psPayload
 ** bool_t                      bCheckGroupIdRange
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_GroupsAddGroupCheckRange(
                            tsZCL_EndPointDefinition    *psEndPointDefinition,
                            tsZCL_ClusterInstance       *psClusterInstance,
                            tsCLD_Groups_AddGroupRequestPayload *psPayload,
                            bool_t bCheckGroupIdRange)
{

    tsCLD_GroupTableEntry *psTableEntry;
    tsCLD_GroupsCustomDataStructure *psCommon;

#if (defined(ZCLIP_ENABLED) && defined(ZCLIP_GROUPS_SERVER_CMD_EXTENSIONS))
    bool_t addGroupExtensionsFound = FALSE;
    uint8 addrAssignmentMode;
    uint8 *multicastIPv6Addr = NULL;
    uint16 groupPort;
#endif

    ZPS_teStatus eZpsStatus;
    #ifdef STRICT_PARAM_CHECK
        /* Parameter check */
        if((psEndPointDefinition==NULL) || (psClusterInstance==NULL) || (psPayload==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif
    psCommon = (tsCLD_GroupsCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    /* group range is 1 - fff7 */
    if(bCheckGroupIdRange && ((psPayload->u16GroupId < 1) || (psPayload->u16GroupId > 0xfff7)))
    {
        return E_CLD_GROUPS_CMD_STATUS_INVALID_FIELD;
    }
    /* Search list for any existing entry to avoid duplication */
    psTableEntry = (tsCLD_GroupTableEntry*)psDLISTsearchFromHead(&psCommon->lGroupsAllocList, (boDLISTtest*)bCLD_GroupsSearchForGroup, (void*)&psPayload->u16GroupId);
    if(psTableEntry != NULL)
    {
        DBG_vPrintf(TRACE_GROUPS, "Existing group entry in table\r\n");

        // release EP
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif


        return E_CLD_GROUPS_DUPLICATE_EXISTS;
    }

    if(eCLD_GroupsCheckGroupExists(psEndPointDefinition, psPayload->u16GroupId) == E_ZCL_SUCCESS)
    {
        // release EP
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif
        return E_CLD_GROUPS_DUPLICATE_EXISTS;
    }


    // Add group to zigbee stack
    DBG_vPrintf(TRACE_GROUPS, "Adding group %04x to endpoint %d\r\n", psPayload->u16GroupId, psEndPointDefinition->u8EndPointNumber);

#if (defined(ZCLIP_ENABLED) && defined(ZCLIP_GROUPS_SERVER_CMD_EXTENSIONS))
    /* This section implements TLV extension for Group address assignment - introduced for ZCLIP */
    if (psPayload->u8TlvCount == 3)
    {
        /* Try to extract the address assignment mode if any */
        tsZCL_Tlv *zclTlv = &psPayload->psTlvList[0];
        if (zclTlv->eTag == E_CLD_GROUPS_CMD_TLV_TAG_ADDR_ASSIGNMENT_MODE
                && zclTlv->u16Length == 1)
        {
            addrAssignmentMode = zclTlv->pu8Data[0];
            addGroupExtensionsFound = TRUE;
        }
        /* Try to extract the multicast IPv6 addr if any */
        zclTlv = &psPayload->psTlvList[1];
        if (zclTlv->eTag == E_CLD_GROUPS_CMD_TLV_TAG_MULTICAST_IPV6_ADDR
                && zclTlv->u16Length > 0)
        {
            zclTlv->pu8Data[zclTlv->u16Length] = '\0';
            multicastIPv6Addr = zclTlv->pu8Data;
        }
        /* Try to extract the group port if any */
        zclTlv = &psPayload->psTlvList[2];
        if (zclTlv->eTag == E_CLD_GROUPS_CMD_TLV_TAG_PORT
                && zclTlv->u16Length == 2)
        {
            FLib_MemCpy(&groupPort, zclTlv->pu8Data, zclTlv->u16Length);
        }
    }

    if (addGroupExtensionsFound)
    {
        eZpsStatus = ZPS_eAplZdoGroupEndpointAddWithMulticastExtensions(psPayload->u16GroupId,
                    psEndPointDefinition->u8EndPointNumber,
                    addrAssignmentMode,
                    multicastIPv6Addr,
                    groupPort);
    }
    else
#endif
    {
        eZpsStatus = ZPS_eAplZdoGroupEndpointAdd(psPayload->u16GroupId, psEndPointDefinition->u8EndPointNumber);
    }

#ifdef ZCLIP_ENABLED
    /* This section for return code mapping is also probably valid for regular zigbee, not just ZCLIP */
    if(eZpsStatus == ZPS_APL_APS_E_INVALID_PARAMETER)
    {
        return E_ZCL_ERR_PARAMETER_RANGE;
    }
    else if(eZpsStatus == ZPS_APL_APS_E_TABLE_FULL)
    {
        return E_ZCL_ERR_INSUFFICIENT_SPACE;
    }
    else if(eZpsStatus != ZPS_E_SUCCESS)
    {
        DBG_vPrintf(TRACE_GROUPS, "Error: Failed to add group\r\n");
        return E_ZCL_FAIL;
    }
#else
    if(eZpsStatus != ZPS_E_SUCCESS)
    {
        DBG_vPrintf(TRACE_GROUPS, "Error: Failed to add group\r\n");
    }
#endif /* ZCLIP_ENABLED */

    /* Get a free table entry */
    psTableEntry = (tsCLD_GroupTableEntry*)psDLISTgetHead(&psCommon->lGroupsDeAllocList);
    if(psTableEntry == NULL)
    {
        /* Delete the group we just added */
        ZPS_eAplZdoGroupEndpointRemove(psPayload->u16GroupId, psEndPointDefinition->u8EndPointNumber);

        // release EP
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif


        DBG_vPrintf(TRACE_GROUPS, "Error: Insufficient space\r\n");

        return E_ZCL_ERR_INSUFFICIENT_SPACE;
    }

    /* Remove from list of free table entries */
    psDLISTremove(&psCommon->lGroupsDeAllocList, (DNODE*)psTableEntry);

    /* Add to allocated list */
    vDLISTaddToTail(&psCommon->lGroupsAllocList, (DNODE*)psTableEntry);

    /* Fill in table entry */
    psTableEntry->u16GroupId = psPayload->u16GroupId;
    FLib_MemSet(psTableEntry->au8GroupName, 0, sizeof(psTableEntry->au8GroupName));
    strncpy((char*)psTableEntry->au8GroupName, (char*)psPayload->sGroupName.pu8Data, psPayload->sGroupName.u8Length);

    // release EP
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif


    return E_ZCL_SUCCESS;

}

/****************************************************************************
 **
 ** NAME:       PUBLIC eCLD_GroupsFindGroup
 **
 ** DESCRIPTION:
 ** Searches both tables for a matching group
 **
 ** PARAMETERS:                 Name                           Usage
 ** tsZCL_EndPointDefinition    *psEndPointDefinition
 ** tsZCL_ClusterInstance       *psClusterInstance
 ** uint16                      *u16GroupId                     group entry to find
 ** tsCLD_GroupTableEntry       **ppsGroupTableEntry            Returned pointer
 **                                                             to matching group
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_GroupsFindGroup(
                            tsZCL_EndPointDefinition    *psEndPointDefinition,
                            tsZCL_ClusterInstance       *psClusterInstance,
                            uint16                      u16GroupId,
                            tsCLD_GroupTableEntry       **ppsGroupTableEntry)
{

    tsCLD_GroupTableEntry *psTableEntry;
    tsCLD_GroupsCustomDataStructure *psCommon;

    teZCL_Status eStatus = E_ZCL_FAIL;

    #ifdef STRICT_PARAM_CHECK
        /* Parameter check */
        if((psEndPointDefinition==NULL) || (psClusterInstance==NULL) || (ppsGroupTableEntry==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif

    psCommon = (tsCLD_GroupsCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif


	eStatus = eCLD_GroupsCheckGroupExists(psEndPointDefinition, u16GroupId);


    /* Search table for an entry containing text */
    psTableEntry = (tsCLD_GroupTableEntry*)psDLISTsearchFromHead(&psCommon->lGroupsAllocList, (boDLISTtest*)bCLD_GroupsSearchForGroup, (void*)&u16GroupId);
    if(psTableEntry != NULL)
    {
        DBG_vPrintf(TRACE_GROUPS, "Found matching group entry in table\r\n");
        *ppsGroupTableEntry = psTableEntry;
    }

    // release EP
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif


    return eStatus;

}


/****************************************************************************
 **
 ** NAME:       PUBLIC iCLD_GroupsListGroups
 **
 ** DESCRIPTION:
 ** Generates a list of groups
 **
 ** PARAMETERS:                 Name                           Usage
 ** tsZCL_EndPointDefinition    *psEndPointDefinition
 ** int16                       *pai16List
 **
 ** RETURN:
 ** int
 **
 ****************************************************************************/
PUBLIC  int iCLD_GroupsListGroups(tsZCL_EndPointDefinition    *psEndPointDefinition,
                                 int16 *pai16List)
{
    uint32 n;
    int iNumGroups = 0;
    ZPS_tsAplAib *psAib;
    uint8 u8ByteOffset;
    uint8 u8BitOffset;

    /* Calculate endpoint bit position in group table array */
    u8ByteOffset = (psEndPointDefinition->u8EndPointNumber - 1) / 8;
    u8BitOffset  = (psEndPointDefinition->u8EndPointNumber - 1) % 8;

    /* Get AIB and search group table for group entries */
    psAib = ZPS_psAplAibGetAib();

    for(n = 0; n < psAib->psAplApsmeGroupTable->u32SizeOfGroupTable; n++)
    {
        if((psAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[n].au8Endpoint[u8ByteOffset] & (1 << u8BitOffset)) != 0)
        {
            pai16List[iNumGroups] = psAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[n].u16Groupid;
            iNumGroups++;
        }
    }

    return iNumGroups;

}

/****************************************************************************
 **
 ** NAME:       PUBLIC iCLD_GroupsCountGroups
 **
 ** DESCRIPTION:
 ** Returns the number of groups
 **
 ** PARAMETERS:                 Name                           Usage
 ** tsZCL_EndPointDefinition    *psEndPointDefinition
 **
 ** RETURN:
 ** int
 **
 ****************************************************************************/
PUBLIC  int iCLD_GroupsCountGroups(tsZCL_EndPointDefinition    *psEndPointDefinition)
{
    uint32 n;
    int iNumGroups = 0;
    ZPS_tsAplAib *psAib;
    uint8 u8ByteOffset;
    uint8 u8BitOffset;

    /* Calculate endpoint bit position in group table array */
    u8ByteOffset = (psEndPointDefinition->u8EndPointNumber - 1) / 8;
    u8BitOffset  = (psEndPointDefinition->u8EndPointNumber - 1) % 8;

    /* Get AIB and search group table for group entries */
    psAib = ZPS_psAplAibGetAib();

    for(n = 0; n < psAib->psAplApsmeGroupTable->u32SizeOfGroupTable; n++)
    {
        if((psAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[n].au8Endpoint[u8ByteOffset] & (1 << u8BitOffset)) != 0)
        {
            iNumGroups++;
        }
    }

    return iNumGroups;

}


/****************************************************************************
 **
 ** NAME:       PUBLIC iCLD_GroupsListMatchingGroups
 **
 ** DESCRIPTION:
 ** Searches both tables for a matching group
 **
 ** PARAMETERS:                 Name                           Usage
 ** tsZCL_EndPointDefinition    *psEndPointDefinition
 ** int16                       *pai16MatchList,
 ** uint8                       u8ItemsInMatchList,
 ** int16                       *pai16List
 **
 ** RETURN:
 ** int
 **
 ****************************************************************************/
PUBLIC  int iCLD_GroupsListMatchingGroups(tsZCL_EndPointDefinition    *psEndPointDefinition,
                                                         int16 *pai16MatchList,
                                                         uint8 u8ItemsInMatchList,
                                                         int16 *pai16List)
{
    uint32 n, x;
    int iNumGroups = 0;
    ZPS_tsAplAib *psAib;
    uint8 u8ByteOffset;
    uint8 u8BitOffset;

    /* Parameter check */
    if(pai16List==NULL)
    {
        return 0;
    }

    /* Calculate endpoint bit position in group table array */
    u8ByteOffset = (psEndPointDefinition->u8EndPointNumber - 1) / 8;
    u8BitOffset  = (psEndPointDefinition->u8EndPointNumber - 1) % 8;

    /* Get AIB and search group table for a matching group entry */
    psAib = ZPS_psAplAibGetAib();

    for(x = 0; x < u8ItemsInMatchList; x++)
    {

        for(n = 0; n < psAib->psAplApsmeGroupTable->u32SizeOfGroupTable; n++)
        {
            if((psAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[n].u16Groupid == (uint16)pai16MatchList[x]) &&
               ((psAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[n].au8Endpoint[u8ByteOffset] & (1 << u8BitOffset)) != 0)
              )
            {
                DBG_vPrintf(TRACE_GROUPS, "\nMatch: Group %d", psAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[n].u16Groupid);
                pai16List[iNumGroups] = psAib->psAplApsmeGroupTable->psAplApsmeGroupTableId[n].u16Groupid;
                iNumGroups++;
            }
        }

    }

    DBG_vPrintf(TRACE_GROUPS, "\nNumGroups %d", iNumGroups);

    return iNumGroups;

}

/****************************************************************************
 **
 ** NAME:       eCLD_GroupsRemoveGroup
 **
 ** DESCRIPTION:
 ** Removes a group from the table
 **
 ** PARAMETERS:                 Name                           Usage
 ** tsZCL_EndPointDefinition    *psEndPointDefinition
 ** tsZCL_ClusterInstance       *psClusterInstance
 ** tsCLD_Groups_AddGroupRequestPayload *psPayload
 ** uint64                      u64SrcIEEEAddress
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_GroupsRemoveGroup(
                            tsZCL_EndPointDefinition    *psEndPointDefinition,
                            tsZCL_ClusterInstance       *psClusterInstance,
                            tsCLD_Groups_RemoveGroupRequestPayload *psPayload,
                            uint64                       u64SrcIEEEAddress)
{
    teZCL_Status eStatus = E_ZCL_SUCCESS;
    tsCLD_GroupTableEntry *psTableEntry;
    tsCLD_GroupsCustomDataStructure *psCommon;
    ZPS_teStatus eZpsStatus;
    #ifdef STRICT_PARAM_CHECK
        /* Parameter check */
        if((psEndPointDefinition==NULL) || (psClusterInstance==NULL) || (psPayload==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif
    psCommon = (tsCLD_GroupsCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

#ifdef ZCLIP_ENABLED
    /* group range is 1 - fff7 */
    if((psPayload->u16GroupId < 1) || (psPayload->u16GroupId > 0xfff7))
    {
        return E_ZCL_ERR_PARAMETER_RANGE;
    }
#endif

    DBG_vPrintf(TRACE_GROUPS, "Removing group %04x from endpoint %d\r\n", psPayload->u16GroupId, psEndPointDefinition->u8EndPointNumber);

    /* Search list for a matching entry */
    psTableEntry = (tsCLD_GroupTableEntry*)psDLISTsearchFromHead(&psCommon->lGroupsAllocList, (boDLISTtest*)bCLD_GroupsSearchForGroup, (void*)&psPayload->u16GroupId);
    if(psTableEntry != NULL)
    {
        DBG_vPrintf(TRACE_GROUPS, "Found matching entry in group table\r\n");

        /* Remove Scenes if associated with this group. */
        #ifdef SCENES_SERVER
            eStatus = eCLD_ScenesRemoveAllScenes(psEndPointDefinition->u8EndPointNumber,
                                                 psTableEntry->u16GroupId,
                                                 u64SrcIEEEAddress);
            if(eStatus == E_ZCL_SUCCESS)
            {
                 vSaveScenesNVM();
            }
#ifdef ZCLIP_ENABLED
            if (eStatus == E_ZCL_ERR_CLUSTER_NOT_FOUND)
            {
                eStatus = E_ZCL_SUCCESS;
            }
#endif
        #endif  // SCENES_SERVER

        /* Remove from list of allocated table entries */
        psDLISTremove(&psCommon->lGroupsAllocList, (DNODE*)psTableEntry);

        /* Add to deallocated list */
        vDLISTaddToTail(&psCommon->lGroupsDeAllocList, (DNODE*)psTableEntry);

    }

    // Remove group from zigbee stack
    eZpsStatus = ZPS_eAplZdoGroupEndpointRemove(psPayload->u16GroupId, psEndPointDefinition->u8EndPointNumber);
    if(eZpsStatus != ZPS_E_SUCCESS)
    {
        DBG_vPrintf(TRACE_GROUPS, "Error: Failed to remove group\r\n");
        eStatus = E_ZCL_FAIL;
    }

    // release EP
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif


    return eStatus;

}


/****************************************************************************
 **
 ** NAME:       eCLD_GroupsRemoveAllGroups
 **
 ** DESCRIPTION:
 ** Removes all groups from the table
 **
 ** PARAMETERS:                 Name                           Usage
 ** tsZCL_EndPointDefinition    *psEndPointDefinition
 ** tsZCL_ClusterInstance       *psClusterInstance
 ** uint64                      u64SrcIEEEAddress
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_GroupsRemoveAllGroups(
                            tsZCL_EndPointDefinition    *psEndPointDefinition,
                            tsZCL_ClusterInstance       *psClusterInstance,
                            uint64                      u64SrcIEEEAddress)
{
    int n;
    teZCL_Status eStatus = E_ZCL_SUCCESS;
    tsCLD_GroupTableEntry *psTableEntry;
    tsCLD_GroupsCustomDataStructure *psCommon;
    ZPS_teStatus eZpsStatus;

    /* Parameter check */
    #ifdef STRICT_PARAM_CHECK
        if((psEndPointDefinition==NULL) || (psClusterInstance==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif
    
    psCommon = (tsCLD_GroupsCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif


    DBG_vPrintf(TRACE_GROUPS, "Removing all groups from endpoint %d\r\n", psEndPointDefinition->u8EndPointNumber);

    /* Move all allocated table entries to the unallocated list */
    for(n = 0; n < CLD_GROUPS_MAX_NUMBER_OF_GROUPS; n++)
    {
        /* Get an entry from the allocated list, exit if there are none */
        psTableEntry = (tsCLD_GroupTableEntry*)psDLISTgetHead(&psCommon->lGroupsAllocList);

#ifdef ZCLIP_ENABLED
        /* Do not remove the broadcast group */
        if(psTableEntry != NULL && psTableEntry->u16GroupId == 0xffff)
        {
            psTableEntry = (tsCLD_GroupTableEntry*)psDLISTgetNext((DNODE *)psTableEntry);
        }
#endif

        if(psTableEntry == NULL)
        {
            break;
        }

        /* Remove All Scenes if associated with this group. */
        #ifdef SCENES_SERVER
            eStatus = eCLD_ScenesRemoveAllScenes(psEndPointDefinition->u8EndPointNumber,
                                                 psTableEntry->u16GroupId,
                                                 u64SrcIEEEAddress);
            if(eStatus == E_ZCL_SUCCESS)
            {
                 vSaveScenesNVM();
            }
        #endif  // SCENES_SERVER

        /* Remove from list of allocated table entries */
        psDLISTremove(&psCommon->lGroupsAllocList, (DNODE*)psTableEntry);

        /* Add to deallocated list */
        vDLISTaddToTail(&psCommon->lGroupsDeAllocList, (DNODE*)psTableEntry);
    }

    // Remove group from zigbee stack

    eZpsStatus = ZPS_eAplZdoGroupAllEndpointRemove(psEndPointDefinition->u8EndPointNumber);
    if(eZpsStatus != ZPS_E_SUCCESS)
    {
        DBG_vPrintf(TRACE_GROUPS, "Error: Failed to remove all groups (zps error %x\r\n", eZpsStatus);
        eStatus = E_ZCL_FAIL;
    }

    // release EP
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif


    return eStatus;

}

/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       bCLD_GroupsSearchForGroup
 **
 ** DESCRIPTION:
 ** This function checks if group id in node under test is same as search parameter.
 **
 ** PARAMETERS:                 Name                           Usage
 ** void                        *pvSearchParam
 ** DNODE                       *psNodeUnderTest
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PRIVATE  bool bCLD_GroupsSearchForGroup(void *pvSearchParam, void *psNodeUnderTest)
{
    uint16 u16SearchParameter;
    uint16 u16GroupId;

    FLib_MemCpy(&u16SearchParameter, pvSearchParam, sizeof(uint16));
    FLib_MemCpy(&u16GroupId, &((tsCLD_GroupTableEntry*)psNodeUnderTest)->u16GroupId, sizeof(uint16));

    DBG_vPrintf(TRACE_GROUPS, "Search: %04x:%04x\r\n", u16GroupId, u16SearchParameter);

    if(u16SearchParameter == u16GroupId)
    {
        return TRUE;
    }

    return FALSE;

}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
