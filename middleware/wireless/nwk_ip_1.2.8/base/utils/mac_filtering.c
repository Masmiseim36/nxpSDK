/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

 /*!
\file       mac_filtering.c
\brief      This is a public source file for the mac filtering  module. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* Framework includes */
#include "Panic.h"
#include "MemManager.h"

/* Network includes */
#include "mac_filtering_cfg.h"
#include "mac_filtering.h"

#include "nvm_adapter.h"
#include "thread_attributes.h"
#include "thread_utils.h"

/*==================================================================================================
Private macros
==================================================================================================*/

/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/
#if MAC_FILTERING_ENABLED
    static macFilteringNeighborData_t** MacFiltering_GetEntry(macAbsAddrModeType_t addressMode,
                                                              uint64_t address, bool_t bAcceptFreeEntry);
    static bool_t MacFiltering_IsTableEmpty(void);
#endif   /* MAC_FILTERING_ENABLED */  
/*==================================================================================================
Private global variables declarations
==================================================================================================*/

macFilteringDefault_t mbDefaultSetting = mMacFilteringDisabled_c;
uint8_t   globalLinkIndicator = 0xFF;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     thrStatus_t MacFiltering_UpdateNeighbor(instanceId_t thrInstID, uint64_t extendedAddress,
                                  uint16_t shortAddress)
\brief  This function is used to update an entry in Mac filtering table

\param  [in]    thrInstID       thread instance id
\param  [in]    extendedAddress extended address
\param  [in]    shortAddress    short address

\return thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t MacFiltering_UpdateNeighbor
(
    instanceId_t thrInstID, 
    uint64_t extendedAddress,
    uint16_t shortAddress
)
{
    thrStatus_t status = gThrStatus_EmptyEntry_c;
    
#if MAC_FILTERING_ENABLED
    
    macFilteringNeighborData_t** ppNeighborData = NULL;
    
    ppNeighborData = MacFiltering_GetEntry(gMacAbsAddrModeExtendedAddress_c, extendedAddress, FALSE);

    if (NULL != ppNeighborData)
    {
         NVNG_MoveToRam((void**)ppNeighborData);
         (*ppNeighborData)->shortAddress     = shortAddress;
         /* Save to NVM */
        NVNG_Save((void**)ppNeighborData);
        status = gThrStatus_Success_c;
    }
    
#endif
    
    return status;
}

/*!*************************************************************************************************
\fn     thrStatus_t MacFiltering_AddNeighbor(instanceId_t thrInstID, uint64_t extendedAddress,
                                  uint16_t shortAddress, uint8_t linkIndicator, bool_t  blockNeighbor)
\brief  This function is used to add an entry in Mac filtering table

\param  [in]    thrInstID        thread instance id
\param  [in]    extendedAddress  extended address
\param  [in]    shortAddress     short address
\param  [in]    linkIndicator    link indicator
\param  [in]    blockNeighbor    boolean. If true - drop received packets from neighbor

\return thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t MacFiltering_AddNeighbor
(
    instanceId_t thrInstID, 
    uint64_t extendedAddress, 
    uint16_t shortAddress, 
    uint8_t linkIndicator,
    bool_t  blockNeighbor
)
{
    thrStatus_t status = gThrStatus_NoMem_c;
    
#if MAC_FILTERING_ENABLED

    macFilteringNeighborData_t** ppNeighborData = MacFiltering_GetEntry(gMacAbsAddrModeExtendedAddress_c, extendedAddress, TRUE);

    if(ppNeighborData)
    {
        if(NULL == *ppNeighborData)
        {   
            /* If it is a dummy entry, do not add it to table, keep only the link indicator value */
            if ((extendedAddress == THR_ALL_FFs64) && (shortAddress == THR_ALL_FFs16) && (blockNeighbor == FALSE))
            {
                globalLinkIndicator = linkIndicator;
                return gThrStatus_Success_c;
            }
            else
            {
                *ppNeighborData = (macFilteringNeighborData_t*)NWKU_MEM_BufferAlloc(sizeof(macFilteringNeighborData_t));
                if(NULL == *ppNeighborData)
                {
                    return gThrStatus_NoMem_c;  
                }
                FLib_MemSet(*ppNeighborData, 0, sizeof(macFilteringNeighborData_t));
                (*ppNeighborData)->shortAddress = 0xFFFF;
            }
        }
        else
        {
            NVNG_MoveToRam((void**)ppNeighborData);
        }

        (*ppNeighborData)->extendedAddress  = extendedAddress;
        (*ppNeighborData)->blockNeighbor    = blockNeighbor;
        if(0xFFFF != shortAddress)
        {
            (*ppNeighborData)->shortAddress     = shortAddress;
        }
        /* Check if neighbor is in neighbor table and update its short address */
        else
        {   
           
            (*ppNeighborData)->shortAddress     = THR_NeighborGetShortByExtAddr(&extendedAddress);
            
        }
        if (linkIndicator)
        {
            (*ppNeighborData)->linkIndicator    = linkIndicator;
        }
        
            /* Save to NVM */
        NVNG_Save((void**)ppNeighborData);
        status = gThrStatus_Success_c;
    }
#else
    status = gThrStatus_NotPermitted_c;
    (void)extendedAddress;
    (void)shortAddress;
    (void)linkIndicator;
    (void)thrInstID;
    (void)blockNeighbor;
#endif /* MAC_FILTERING_ENABLED */
    
    return status;
}

/*!*************************************************************************************************
\fn     thrStatus_t MacFiltering_RemoveNeighbor(instanceId_t thrInstID, uint64_t extendedAddress)
\brief  This function is used to remove an entry from Mac filtering table

\param  [in]    thrInstID       thread instance id
\param  [in]    extendedAddress extended address

\return thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t MacFiltering_RemoveNeighbor
(
    instanceId_t thrInstID, 
    uint64_t extendedAddress
)
{
    thrStatus_t status = gThrStatus_NotPermitted_c;
#if MAC_FILTERING_ENABLED
    macFilteringNeighborData_t** ppNeighborData = MacFiltering_GetEntry(gMacAbsAddrModeExtendedAddress_c, extendedAddress, FALSE);

    if(NULL != ppNeighborData)
    {
        NVNG_Erase((void **)ppNeighborData);
        status = gThrStatus_Success_c;
    }
#else
    (void)extendedAddress;
#endif /* MAC_FILTERING_ENABLED */
    
    return status;
}

/*!*************************************************************************************************
\fn       bool_t MacFiltering_KeepPacket(instanceId_t thrInstID,  macAbsAddrModeType_t addressMode,
                                         uint64_t address,uint8_t *pLinkIndicator)
\brief    This function returns TRUE if the packet should be keep 

\param  [in]     addressMode     address mode
\param  [in]     address         address 
\param  [in]     pLinkIndicator  pointer to link indicator

\return  bool_t  boolean. If true - keep the packet
***************************************************************************************************/
bool_t MacFiltering_KeepPacket
(
    macAbsAddrModeType_t addressMode, 
    uint64_t address,
    uint8_t *pLinkIndicator
)
{
    bool_t bKeepPacket = TRUE;

#if MAC_FILTERING_ENABLED
    if (mbDefaultSetting != mMacFilteringDisabled_c)
    {
        if(FALSE == MacFiltering_IsTableEmpty())
        {
            macFilteringNeighborData_t** ppNeighborData = MacFiltering_GetEntry(addressMode, address, FALSE);

            if (NULL != ppNeighborData)
            {   
                if ((*ppNeighborData)->blockNeighbor == TRUE)
                {
                    bKeepPacket = FALSE;
                }
                else if (pLinkIndicator)
                {
                    *pLinkIndicator = (*ppNeighborData)->linkIndicator;
                }
            }
            else
            {   
                if (mbDefaultSetting == mMacFilteringDefaultPolicyReject_c)
                {
                    bKeepPacket = FALSE;
                }
                else
                {
                     bKeepPacket = TRUE;
                     if (pLinkIndicator)
                     {
                        *pLinkIndicator = globalLinkIndicator;
                     }
                }
            }
        }
        else
        {
            if (mbDefaultSetting == mMacFilteringDefaultPolicyReject_c)
            {
                bKeepPacket = FALSE;
            }
            else
            {
                 bKeepPacket = TRUE;
                 if (pLinkIndicator)
                 {
                    *pLinkIndicator = globalLinkIndicator;
                 }
            }        
        }
    }
#else
    (void)addressMode;
    (void)address;
    (void)pLinkIndicator;
#endif /* MAC_FILTERING_ENABLED */

    return bKeepPacket;
}

/*!*************************************************************************************************
\fn     thrStatus_t MacFiltering_Active(instanceId_t thrInstID, macFilteringDefault_t filteringType)
\brief  This functions verify sets default filtering policy if mac filtering module is active 

\param  [in]    thrInstID          thread instance id
\param  [in]    filteringType      enable/disable filtering table

\return  thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t MacFiltering_Active
(
    instanceId_t thrInstID,
    macFilteringDefault_t filteringType
)
{
    thrStatus_t status = gThrStatus_NotPermitted_c;
#if MAC_FILTERING_ENABLED
        mbDefaultSetting = filteringType;
        NVNG_SyncSave((void **)&mbDefaultSetting);
    status = gThrStatus_Success_c;
#endif    
    return status;
}

/*!*************************************************************************************************
\fn     macFilteringDefault_t MacFiltering_IsActive(instanceId_t thrInstID)
\brief  This functions returns default filtering policy

\param  [in]    thrInstID       thread instance id

\return  macFilteringDefault_t  Mac filtering default policy
***************************************************************************************************/
macFilteringDefault_t MacFiltering_IsActive
(
    instanceId_t thrInstID   
)
{
    (void)thrInstID;

    return mbDefaultSetting;    
}

/*!*************************************************************************************************
\fn     bool_t MacFiltering_Reset()
\brief  Disables and deletes the mac filtering table from NVM.

\return  bool_t   boolean. If true - reset success
***************************************************************************************************/
bool_t MacFiltering_Reset(void)
{
#if MAC_FILTERING_ENABLED  
    /* remove mac filtering  table */
    for(uint8_t i=0;i< MAC_FILTERING_TABLE_SIZE; i++)
    {
        NVNG_Erase((void **)&gaMacFilteringTable[i]);
    }
    mbDefaultSetting = mMacFilteringDisabled_c;
    NVNG_SyncSave((void **)&mbDefaultSetting);    
    return TRUE;
#else
    return FALSE;
#endif    
}
/*==================================================================================================
Private functions
==================================================================================================*/
#if MAC_FILTERING_ENABLED
/*!*************************************************************************************************
\private
\fn    static macFilteringNeighborData_t** MacFiltering_GetEntry(macAbsAddrModeType_t addressMode,
                                  uint64_t address, bool_t bAcceptFreeEntry)
\brief  Get mac filter entry

\param  [in]    addressMode           mac address mode
\param  [in]    address               mac address
\param  [in]    bAcceptFreeEntry      boolean. If true - return free entry

\return  macFilteringNeighborData_t ** double pointer to mac filter entry
***************************************************************************************************/
static macFilteringNeighborData_t** MacFiltering_GetEntry
(
    macAbsAddrModeType_t addressMode,
    uint64_t address, 
    bool_t bAcceptFreeEntry
)
{
    macFilteringNeighborData_t**    ppNeighborData = NULL;
    uint8_t                         iCount;
    
    if((gMacAbsAddrModeExtendedAddress_c == addressMode) ||
       (gMacAbsAddrModeShortAddress_c == addressMode))
    {
        for(iCount = 0; iCount < MAC_FILTERING_TABLE_SIZE; iCount++)
        {
            if(NULL != gaMacFilteringTable[iCount])
            {

                uint64_t neighborAddress; 
                if(addressMode == gMacAbsAddrModeExtendedAddress_c)
                {
                    neighborAddress = gaMacFilteringTable[iCount]->extendedAddress;
                }
                else
                {
                    address &= 0xFFFF;
                    neighborAddress = gaMacFilteringTable[iCount]->shortAddress;
                }

                if   (address == neighborAddress)                  
                {
                    ppNeighborData = &gaMacFilteringTable[iCount];
                    break;
                }               
            }
            else if((TRUE == bAcceptFreeEntry) &&
                    (NULL == ppNeighborData))
            {
                ppNeighborData = &gaMacFilteringTable[iCount];
            }
        }
    }   
    
    return ppNeighborData;
}

/*!*************************************************************************************************
\private
\fn    static bool_t MacFiltering_IsTableEmpty(void)

\brief  Check if mac filter table is empty

\return  boolean. If true - mac filter table is empty
***************************************************************************************************/
static bool_t MacFiltering_IsTableEmpty(void)
{
    bool_t  bTableEmpty = TRUE;
    uint8_t iCount;

    for(iCount = 0; iCount < MAC_FILTERING_TABLE_SIZE; iCount++)
    {
        if(NULL != gaMacFilteringTable[iCount])
        {
            bTableEmpty = FALSE;
            break;
        }
    }

    return bTableEmpty;
}

#endif /* MAC_FILTERING_ENABLED */