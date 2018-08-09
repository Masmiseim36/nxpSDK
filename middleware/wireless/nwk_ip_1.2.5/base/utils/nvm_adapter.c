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
\file       nvm_adapter.c
\brief      This is a public source file for the non volatile adapter module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "app_stack_config.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "stack_config.h"
#include "NVM_Interface.h"
#include "fsl_os_abstraction.h"
#include "nvm_adapter.h"
#include "nv_data.h"
#include "thread_app_callbacks.h"

#include "thread_types.h"

#if MAC_FILTERING_ENABLED
    #include "mac_filtering.h"
#endif

#if (gNvStorageIncluded_d == TRUE)
    #include "Panic.h"
#endif

/*! Thread neighbors data set for nvm */
extern thrNeighbor_t gpaThrNeighbors[];

/*! Thread child addr reg table set for nvm */
extern thrChildAddrRegEntry_t gaThrAddrRegTbl[];

/*! Thread child version table set for nvm */
extern childVersNbSet_t gaChildVersNbTbl[];

/*! Thread children multicast address register table set for nvm */
extern thrChildMcastAddrRegEntry_t gaThrMcastAddrRegTbl[];

/*! Mle outgoing security frame counter */
extern uint32_t mMleOutgoingSecFrameCounter;

/*! Mac outgoing security frame counter */
extern uint32_t mMacOutgoingSecFrameCounter;

/*! Mpl outgoing sequence number*/
extern uint8_t  mMplSequenceNumber;

/* Server Data tables */
extern ipAddr_t gaServerDataPrefixTbl[];
extern uint8_t gaServerDataPrefixLenTbl[];
extern borderRouterSet_t gaThrServerDataBrSetTbl[];
extern externalRouteSet_t gaServerDataExtRouteTbl[];

#if MAC_FILTERING_ENABLED
    /*! Mac Filtering Default policy */
    extern macFilteringDefault_t mbDefaultSetting;
#endif
/*==================================================================================================
Private macros
==================================================================================================*/
#if NVM_NG_ENABLED
    #if (gNvStorageIncluded_d == FALSE)
        #error "*** ERROR: NVM module is not enabled"
    #endif
    #if (gUnmirroredFeatureSet_d != TRUE)
        #error "*** ERROR: gUnmirroredFeatureSet_d from NVM module is not enabled"
    #endif
#endif
/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/*! mutex used to ensure NVNG functions thread switch safety */
osaMutexId_t mNVNGMutex;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn      nvngErrCode_t NVNG_Init(void)
\brief   Interface function for the NVNG module. It performs the initialization of the module.

\return  nvngErrCode_t  Status: gNVNGSuccess_c or gNVNGFail_c
***************************************************************************************************/
nvngErrCode_t NVNG_Init
(
    void
)
{
    nvngErrCode_t nvngInitStatus = gNVNGSuccess_c;

#if NVM_NG_ENABLED

    static bool_t bNvngInitialized = FALSE;
    nvngInitStatus = gNVNGSuccess_c;

    if (FALSE == bNvngInitialized)
    {
        mNVNGMutex  = OSA_MutexCreate();

        if (NULL != mNVNGMutex)
        {
            bNvngInitialized = TRUE;
        }
        else
        {
            nvngInitStatus = gNVNGFail_c;
            /* Critical issue with nvm - mcu reset / halt in panic*/
            APP_CriticalExitCb((uint32_t)NVNG_Init, nvngInitStatus);
        }
    }

#endif /* NVM_NG_ENABLED */

    return nvngInitStatus;
}

/*!*************************************************************************************************
\fn     nvngErrCode_t NVNG_ThreadRestore(bool_t startWithNVM)
\brief  Interface function for the NVNG module. It performs the restore of NVM data.

\param [in]     startWithNVM TRUE start with NVM, false do not restore from nvm

\return         nvngErrCode_t  Status: gNVNGSuccess_c or gNVNGFail_c
***************************************************************************************************/
nvngErrCode_t NVNG_ThreadRestore
(
    bool_t startWithNVM
)
{
#if NVM_NG_ENABLED
    nvngErrCode_t nvngInitStatus = gNVNGSuccess_c;
    NVM_Status_t status = gNVM_Error_c;
    uint32_t i = 0;
    void *aThrMirroredDataSets[] = {gpaThrNeighbors, &mMacOutgoingSecFrameCounter,
                                    &mMleOutgoingSecFrameCounter, &mMplSequenceNumber
#if !THREAD_ED_CONFIG
                                    , &gaThrAddrRegTbl, &gaChildVersNbTbl
                                    , &gaServerDataPrefixTbl, &gaServerDataPrefixLenTbl
                                    , &gaThrServerDataBrSetTbl, &gaServerDataExtRouteTbl
#endif
#if MAC_FILTERING_ENABLED
                                    , &mbDefaultSetting
#endif
                                   };

    if (startWithNVM)
    {
        /* Restore Thread Mirrored nvm */
        for (i = 0; i < sizeof(aThrMirroredDataSets) / sizeof(void *); i++)
        {
            status = NvRestoreDataSet(aThrMirroredDataSets[i], TRUE);

            if (!((gNVM_MetaNotFound_c == status) || (gNVM_OK_c == status)))
            {
                nvngInitStatus = gNVNGFail_c;
                break;
            }
        }

        if (gNVNGFail_c ==  nvngInitStatus)
        {
            panic(0, (uint32_t)NVNG_ThreadRestore, 0, 0);
        }
    }
    else
    {
        /* Delete Unmirrored Data */
        i = 0;

        while (gNVM_TABLE_entries_c != i)
        {
            if ((gNVM_NotMirroredInRamAutoRestore_c == pNVM_DataTable[i].DataEntryType) && (NULL != pNVM_DataTable[i].pData))
            {
                /* for debug */
                switch (pNVM_DataTable[i].DataEntryID)
                {
                    case nvmId_ThreadAttr_c:
                    case nvmId_ThreadStringAttr_c:
                    case nvmId_ThrIdAssignTbl_c:
                    case nvmId_mDhcp6ServerBindingTbl_c:
                    case nvmId_Dhcp6ServerCfg_c:
                    case nvmId_Dhcp6ClientParams_c:

                    /// Needed for interop Leader reboot.
                    case nvmId_macFilteringTable_c:
                    case nvmId_GlobalAddrTable6_c:
                    case nvmId_ThrSlaacTempAddrTbl_c:
                    {
                        for (uint32_t nvIdx = 0; nvIdx < pNVM_DataTable[i].ElementsCount; nvIdx++)
                        {
                            ((void **)pNVM_DataTable[i].pData)[nvIdx] = NULL;
                        }
                    }

                    default:
                        break;
                }
            }

            i++;
        }
    }

    return nvngInitStatus;
#else
    (void)startWithNVM;
    return gNVNGSuccess_c;
#endif
}

/*!*************************************************************************************************
\fn      void NVNG_MoveToRam(void ** ppRam)
\brief   Moves in ram an unmirrored dataset.

\param [in]  ppRam     double pointer to the entity to be moved from flash to RAM
***************************************************************************************************/
void NVNG_MoveToRam
(
    void **ppRam
)
{
#if NVM_NG_ENABLED

    (void)OSA_MutexLock(mNVNGMutex, osaWaitForever_c);
    NVM_Status_t nvmStatus = NvMoveToRam(ppRam);

    if (gNVM_OK_c != nvmStatus)
    {
        /* Critical issue with nvm - mcu reset / halt in panic*/
        APP_CriticalExitCb((uint32_t)NVNG_MoveToRam, nvmStatus);
    }

#endif
}

/*!*************************************************************************************************
\fn     void NVNG_Save(void ** ppRam)
\brief  Saves a structure in NVM in idle loop.

\param [in]  ppRam     double pointer to the entity to be saved
***************************************************************************************************/
void NVNG_Save
(
    void **ppRam
)
{
#if NVM_NG_ENABLED
    NVM_Status_t nvmStatus = NvSaveOnIdle(ppRam, FALSE);

    if (gNVM_OK_c != nvmStatus)
    {
        /* Critical issue with nvm - mcu reset / halt in panic*/
        APP_CriticalExitCb((uint32_t)NVNG_Save, nvmStatus);
    }

    (void)OSA_MutexUnlock(mNVNGMutex);
#endif
}

/*!*************************************************************************************************
\fn void NVNG_SyncSave(void ** ppRam)
\brief  Save synchronously a structure in NVM.

\param [in]  ppRam     double pointer to the entity to be saved
***************************************************************************************************/
void NVNG_SyncSave
(
    void **ppRam
)
{
#if NVM_NG_ENABLED
    NVM_Status_t nvmStatus = NvSyncSave(ppRam, FALSE);

    if ((gNVM_OK_c != nvmStatus) && (gNVM_CriticalSectionActive_c != nvmStatus))
    {
        /* Critical issue with nvm - mcu reset / halt in panic*/
        APP_CriticalExitCb((uint32_t)NvSyncSave, nvmStatus);
    }

    (void)OSA_MutexUnlock(mNVNGMutex);
#endif
}
/*!*************************************************************************************************
\fn void NVNG_SaveOnInterval(void ** ppRam)
\brief  Save on interval a structure in NVM.

\param [in]  ppRam     double pointer to the entity to be saved
***************************************************************************************************/
void NVNG_SaveOnInterval
(
    void **ppRam
)
{
#if NVM_NG_ENABLED
    NVM_Status_t nvmStatus = NvSaveOnInterval(ppRam);

    if (gNVM_OK_c != nvmStatus)
    {
        /* Critical issue with nvm - mcu reset / halt in panic*/
        APP_CriticalExitCb((uint32_t)NVNG_SaveOnInterval, nvmStatus);
    }

    (void)OSA_MutexUnlock(mNVNGMutex);
#endif
}
/*!*************************************************************************************************
\fn     void NVNG_Erase(void ** ppRam)
\brief  Erases an unmirrored dataset.

\param [in]  ppRam     double pointer to the entity to be erased
***************************************************************************************************/
void NVNG_Erase
(
    void **ppRam
)
{
#if !NVM_NG_ENABLED
    /* make a local copy of the pointer */
    void *tempPram = *ppRam;

    OSA_InterruptDisable();
    *ppRam = NULL;
    OSA_InterruptEnable();
    MEM_BufferFree(tempPram);
#else
    NVM_Status_t nvmStatus = NvErase(ppRam);

    if ((gNVM_OK_c != nvmStatus) && (gNVM_CriticalSectionActive_c != nvmStatus))
    {
        /* Critical issue with nvm - mcu reset / halt in panic*/
        APP_CriticalExitCb((uint32_t)NVNG_Erase, nvmStatus);
    }

#endif
}

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Private debug functions
==================================================================================================*/
