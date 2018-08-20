/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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

macFilteringDefault_t mbDefaultSetting = mMacFilteringDefaultPolicyAccept_c;

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
            *ppNeighborData = (macFilteringNeighborData_t*)NWKU_MEM_BufferAlloc(sizeof(macFilteringNeighborData_t));
            if(NULL == *ppNeighborData)
            {
                return gThrStatus_NoMem_c;  
            }
            FLib_MemSet(*ppNeighborData, 0, sizeof(macFilteringNeighborData_t));
            (*ppNeighborData)->shortAddress = 0xFFFF;
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
    macFilteringNeighborData_t**    ppDummyEntry = NULL;
    
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

                if(address == neighborAddress)
                {
                    ppNeighborData = &gaMacFilteringTable[iCount];
                    break;
                }                
                else if ((gaMacFilteringTable[iCount]->extendedAddress == THR_ALL_FFs64) &&
                         (gaMacFilteringTable[iCount]->shortAddress == THR_ALL_FFs16))
                {   
                    ppDummyEntry = &gaMacFilteringTable[iCount];
                }
            }
            else if((TRUE == bAcceptFreeEntry) &&
                    (NULL == ppNeighborData))
            {
                ppNeighborData = &gaMacFilteringTable[iCount];
            }
        }
    }

    /* Check if previously was added a dummy entry and update it with the actual address */
    if ((ppNeighborData == NULL) && (ppDummyEntry != NULL))
    {
        NVNG_MoveToRam((void**)ppDummyEntry);
        if(addressMode == gMacAbsAddrModeExtendedAddress_c)
        {
            (*ppDummyEntry)->extendedAddress = address;
        }
        else
        {
            address &= 0xFFFF;
            (*ppDummyEntry)->shortAddress = address;
        }
        /* Save to NVM */
        NVNG_Save((void**)ppDummyEntry);
        ppNeighborData = ppDummyEntry;
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