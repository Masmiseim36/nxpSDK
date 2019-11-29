/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
#include "thread_meshcop.h"
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

/* Thread Router Promotion Jitter */
extern uint32_t gThrRouterSelJitterSec;

#if IP_IP6_MCAST_FW_ENABLE
/* Multicast forwarding table */
extern ip6McastFwTblEntry_t aIp6McastFwTable[];
#endif

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
#if IP_IP6_MCAST_FW_ENABLE
                                    , &aIp6McastFwTable
#endif

#if !THREAD_ED_CONFIG
                                    , &gaThrAddrRegTbl, &gaChildVersNbTbl
                                    , &gaServerDataPrefixTbl, &gaServerDataPrefixLenTbl
                                    , &gaThrServerDataBrSetTbl, &gaServerDataExtRouteTbl
                                    , &gThrRouterSelJitterSec
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
\fn void NVNG_RestoreDataset(void *pDataset, bool_t restoreAll)
\brief  Restore table from NVM.

\param [in]  pDataset     Pointer to dataset
\param [in]  restoreAll   TRUE - restores entire table
                          FALSE - restores one entry
***************************************************************************************************/
void NVNG_RestoreDataset
(
    void *pDataset,
    bool_t restoreAll
)
{
#if NVM_NG_ENABLED
    NVM_Status_t nvmStatus = NvRestoreDataSet(pDataset, restoreAll);

    if ((gNVM_OK_c != nvmStatus) && (gNVM_MetaNotFound_c != nvmStatus))
    {
        /* Critical issue with nvm - mcu reset / halt in panic*/
        APP_CriticalExitCb((uint32_t)NvRestoreDataSet, nvmStatus);
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
