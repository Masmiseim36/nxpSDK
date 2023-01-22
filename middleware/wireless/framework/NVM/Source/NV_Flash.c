/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019-2021 NXP
* All rights reserved.
*
* \file
*
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "EmbeddedTypes.h"
#include "NV_Flash.h"
#include "fsl_adapter_flash.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"

#if gUnmirroredFeatureSet_d
#include "fsl_component_mem_manager.h"
#endif

#if (gNvUseFlexNVM_d == TRUE)
#include "fsl_adapter_reset.h"
#endif /* gNvUseFlexNVM_d == TRUE */

#if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && (gNvmEnableFSCIRequests_c || gNvmEnableFSCIMonitoring_c))
#include "NV_FsciCommands.h"
#endif

/*****************************************************************************
 *****************************************************************************
 * Private macros
 *****************************************************************************
 *****************************************************************************/
__attribute__((weak))  uint64_t TM_GetTimestamp(void);
__attribute__((weak))  uint64_t TM_GetTimestamp(void)
{
  return 0;
}

__attribute__((weak))  void RNG_GetRandomNo(uint32_t *random);
__attribute__((weak))  void RNG_GetRandomNo(uint32_t *random)
{
  *random = 0x123;
}
#define NV_FlashRead(pSrc, pDest, size) (void)HAL_FlashRead((uint32_t)(pSrc), size, (uint8_t*)(pDest));

#if gNvStorageIncluded_d

 #if (gUnmirroredFeatureSet_d == TRUE)
   #if (gNvUseFlexNVM_d == TRUE)
     #error "*** ERROR: gUnmirroredFeatureSet_d not implemented on FlexNVM"
   #endif
   #if (gNvFragmentation_Enabled_d == FALSE)
     #error "*** ERROR: gNvFragmentation_Enabled_d should be enabled for gUnmirroredFeatureSet_d"
   #endif
 #endif

 #if ((gNvUseFlexNVM_d == TRUE) && (gNvUseExtendedFeatureSet_d == TRUE))
   #error "*** ERROR: gNvUseExtendedFeatureSet_d not available on FlexNVM"
 #endif

/*
 * Name: gNvVirtualPagesCount_c
 * Description: the count of virtual pages used
 */
#define gNvVirtualPagesCount_c         2U /* DO NOT MODIFY */

/*
 * Name: gNvGuardValue_c
 * Description: self explanatory
 */
#define gNvGuardValue_c            0xFFFFFFFFFFFFFFFFuL

/*
 * Name: gNvFirstMetaOffset_c
 * Description: the offset of the first meta
 */
#if gNvUseExtendedFeatureSet_d
    #define gNvFirstMetaOffset_c       (sizeof(NVM_TableInfo_t) + mNvTableSizeInFlash + sizeof(NVM_TableInfo_t))
#else
    #define gNvFirstMetaOffset_c       (sizeof(NVM_TableInfo_t))
#endif

/*
 * Name: gNvErasedFlashCellValue_c
 * Description: self explanatory
 */
#define gNvErasedFlashCellValue_c      0xFF

/*
 * Name: gNvLegacyOffset_c
 * Description: the offset of the old tables, used for upgrading legacy tables to the new format
 */
#define gNvLegacyOffset_c               4U

#if (gNvUseFlexNVM_d == TRUE) /* FlexNVM */
/*
 * Name: gEEPROM_DATA_SET_SIZE_CODE_c
 * Description: EEPROM Data set size code
 */
#define gEEPROM_DATA_SET_SIZE_CODE_c    (0x31U)

/*
 * Name: gFLEXNVM_PARTITION_CODE_c
 * Description: FlexNVM partition code
 */
#define gFLEXNVM_PARTITION_CODE_c       (0x8U)
#endif /* (gNvUseFlexNVM_d == TRUE) */

#endif /* gNvStorageIncluded_d */

#if (!defined(GCOV_DO_COVERAGE) || (GCOV_DO_COVERAGE == 0))
#define NVM_STATIC static
#define NVM_PUBLIC
#else
#define NVM_STATIC __WEAK
#define NVM_PUBLIC __WEAK
#endif

/*****************************************************************************
 *****************************************************************************
 * Private type definitions
 *****************************************************************************
 *****************************************************************************/
/*****************************************************************************
 *****************************************************************************
 * Private prototypes
 *****************************************************************************
 *****************************************************************************/
#if gNvStorageIncluded_d
#if gNvUseExtendedFeatureSet_d && gNvTableKeptInRam_d
/******************************************************************************
 * Name: __NvRegisterTableEntry
 * Description: The function tries to register a new table entry within an
 *              existing NV table. If the NV table contained an erased (invalid)
 *              entry, the entry will be overwritten with a new one (provided
 *              by the mean of this function arguments)
 * Parameter(s): [IN] ptrData - generic pointer to RAM data to be registered
 *                              within the NV storage system
 *               [IN] uniqueId - an unique ID of the table entry
 *               [IN] elemCount - how many elements the table entry contains
 *               [IN] elemSize - the size of an element
 *               [IN] overwrite - if an existing table entry shall be
 *                                overwritten
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvRegisterTableEntry
(
  void* ptrData,
  NvTableEntryId_t uniqueId,
  uint16_t elemCount,
  uint16_t elemSize,
  uint16_t dataEntryType,
  bool_t overwrite
);

/******************************************************************************
 * Name: __NvEraseEntryFromStorage
 * Description: The function removes a table entry within the existing NV
 *              table. The RAM table must be updated previously.
 * Parameter(s): [IN] entryId - the entry id of the entry that is removed
 *               [IN] tableEntryIndex - the index of the entry in the ram table
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvEraseEntryFromStorage
(
  uint16_t entryId,
  uint16_t tableEntryIndex
);

#endif //gNvUseExtendedFeatureSet_d && gNvTableKeptInRam_d

/******************************************************************************
 * Name: InitNVMConfig
 * Description: Initialises the hal driver, and gets the active page.
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
NVM_STATIC void InitNVMConfig
(
  void
);
#endif

/******************************************************************************
 * Name: __NvAtomicSave
 * Description: The function performs an atomic save of the entire NV table
 *              to the storage system. The operation is performed
 *              in place (atomic).
 * Parameter(s): -
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *         gNVM_PointerOutOfRange_c - if the pointer is out of range
 *         gNVM_InvalidTableEntry_c - if the table entry is not valid
 *         gNVM_MetaInfoWriteError_c - meta tag couldn't be written
 *         gNVM_RecordWriteError_c - record couldn't be written
 *         gNVM_CriticalSectionActive_c - the module is in critical section
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvAtomicSave
(
  void
);

/******************************************************************************
 * Name: __NvSyncSave
 * Description: The function saves the pointed element or the entire table
 *              entry to the storage system. The save operation is not
 *              performed on the idle task but within this function call.
 * Parameter(s): [IN] ptrData - a pointer to data to be saved
 *               [IN] saveAll - specifies if the entire table entry shall be
 *                              saved or only the pointed element
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *         gNVM_PointerOutOfRange_c - if the pointer is out of range
 *         gNVM_InvalidTableEntry_c - if the table entry is not valid
 *         gNVM_MetaInfoWriteError_c - meta tag couldn't be written
 *         gNVM_RecordWriteError_c - record couldn't be written
 *         gNVM_CriticalSectionActive_c - the module is in critical section
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvSyncSave
(
  void* ptrData,
  bool_t saveAll
);

/******************************************************************************
 * Name: __NvFormat
 * Description: Format the NV storage system. The function erases both virtual
 *              pages and then writes the page counter to active page.
 * Parameter(s): -
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_FormatFailure_c - if the format operation fails
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_CriticalSectionActive_c - if the system has entered in a
 *                                        critical section
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvFormat
(
  void
);
/******************************************************************************
 * Name: __NvIdle
 * Description: Called from the idle task (bare-metal) or NVM_Task (MQX,
 *              FreeRTOS) to process the pending saves, erase or copy
 *              operations.
 * Parameters: -
 * Return: -
 ******************************************************************************/
NVM_STATIC void __NvIdle
(
  void
);
/******************************************************************************
 * Name: __NvIsDataSetDirty
 * Description: return TRUE if the element pointed by ptrData is dirty
 * Parameters: [IN] ptrData - pointer to data to be checked
 * Return: TRUE if the element is dirty, FALSE otherwise
 ******************************************************************************/
bool_t __NvIsDataSetDirty
(
  void* ptrData
);
/******************************************************************************
 * Name: __NvRestoreDataSet
 * Description: copy the most recent version of the element/table entry pointed
 *              by ptrData from NVM storage system to RAM memory
 * Parameter(s): [IN] ptrData - pointer to data (element) to be restored
 *               [IN] restoreAll - if FALSE restores a single element
 *                               - if TRUE restores an entire table entry
 * Return: status of the restore operation
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvRestoreDataSet
(
  void* ptrData,
  bool_t restoreAll
);

/******************************************************************************
 * Name: __NvTimerTick
 * Description: Called from the idle task to process save-on-interval requests
 * Parameters: [IN] countTick - enable/disable tick count
 * Return: FALSE if the timer tick counters for all data sets have reached
 *         zero. In this case, the timer can be turned off.
 *         TRUE if any of the data sets' timer tick counters have not yet
 *         counted down to zero. In this case, the timer should be active
 ******************************************************************************/
NVM_STATIC bool_t __NvTimerTick
(
  bool_t countTick
);
/******************************************************************************
 * Name: __NvSaveOnCount
 * Description: Decrement the counter. Once it reaches 0, the next call to
 *              NvIdle() will save the entire table entry (all elements).
 * Parameters: [IN] ptrData - pointer to data to be saved
 * Return: NVM_OK_c - if operation completed successfully
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_STATIC NVM_Status_t __NvSaveOnCount
(
  void* ptrData
);
/******************************************************************************
 * Name: __NvSaveOnInterval
 * Description:  save no more often than a given time interval. If it has
 *               been at least that long since the last save,
 *               this function will cause a save the next time the idle
 *               task runs.
 * Parameters: [IN] ptrData - pointer to data to be saved
 * NOTE: this function saves all the element of the table entry pointed by
 *       ptrData
 * Return: NVM_OK_c - if operation completed successfully
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_STATIC NVM_Status_t __NvSaveOnInterval
(
  void* ptrData
);
/******************************************************************************
 * Name: __NvSaveOnIdle
 * Description: Save the data pointed by ptrData on the next call to NvIdle()
 * Parameter(s): [IN] ptrData - pointer to data to be saved
 *               [IN] saveRestoreAll - specify if all the elements from the NVM table
 *                              entry shall be saved
 * Return: gNVM_OK_c - if operation completed successfully
 *         gNVM_Error_c - in case of error(s)
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_STATIC NVM_Status_t __NvSaveOnIdle
(
  void* ptrData,
  bool_t saveAll
);

/******************************************************************************
 * Name: __NvModuleInit
 * Description: Initialize the NV storage module
 * Parameter(s): -
 * Return: gNVM_ModuleAlreadyInitialized_c - if the module is already
 *                                           initialized
 *         gNVM_InvalidSectorsCount_c - if the sector count configured in the
 *                                      project linker file is invalid
 *         gNVM_MetaNotFound_c - if no meta information was found
 *         gNVM_OK_c - module was successfully initialized
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvModuleInit
(
    bool_t flashInit
);

#if gUnmirroredFeatureSet_d

/******************************************************************************
 * Name: __NvmMoveToRam
 * Description: Move from NVM to Ram
 * Parameter(s):  ppData     double pointer to the entity to be moved from flash to RAM
 * Return: pointer to Ram location
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvmMoveToRam
(
  void** ppData
);

/******************************************************************************
 * Name: __NvmErase
 * Description: Erase from NVM an unmirrored dataset
 * Parameter(s):  ppData     double pointer to the entity to be erased
 * Return: pointer to Ram location
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvmErase
(
  void** ppData
);

/******************************************************************************
 * Name: NvIsNVMFlashAddress
 * Description: check if the address is in Flash
 * Parameter(s): [IN] address
 *
 * Return: TRUE if the table entry is in Flash / FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvIsNVMFlashAddress
(
  void* address
);

/******************************************************************************
 * Name: __NvmRestoreUnmirrored
 * Description: Restores all unmirrored entries with gNVM_NotMirroredInRamAutoRestore_c at init
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
NVM_STATIC void __NvmRestoreUnmirrored
(
    void
);
#endif

/******************************************************************************
 * Name: NvInitPendingSavesQueue
 * Description: Initialize the pending saves queue
 * Parameters: [IN] pQueue - pointer to queue
 * Return: TRUE if the pointer is valid, FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvInitPendingSavesQueue
(
  NVM_SaveQueue_t *pQueue
);

/******************************************************************************
 * Name: NvPushPendingSave
 * Description: Add a new pending save to the queue
 * Parameters: [IN] pQueue - pointer to queue
 *             [IN] data - data to be saved
 * Return: TRUE if the push operation succeeded, FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvPushPendingSave
(
  NVM_SaveQueue_t *pQueue,
  NVM_TableEntryInfo_t data
);

/******************************************************************************
 * Name: NvPopPendingSave
 * Description: Retrieves the head element from the pending saves queue
 * Parameters: [IN] pQueue - pointer to queue
 *             [OUT] pData - pointer to the location where data will be placed
 * Return: TRUE if the pop operation succeeded, FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvPopPendingSave
(
  NVM_SaveQueue_t *pQueue,
  NVM_TableEntryInfo_t *pData
);

/******************************************************************************
 * Name: NvGetPendingSavesCount
 * Description: self explanatory
 * Parameters: [IN] pQueue - pointer to queue
 * Return: Number of pending saves
 ******************************************************************************/
NVM_STATIC uint16_t NvGetPendingSavesCount
(
  NVM_SaveQueue_t *pQueue
);

/*****************************************************************
 * The below functions are compiled only if FlexNVM is NOT used
 *****************************************************************/

#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */

/******************************************************************************
 * Name: UpgradeLegacyTable
 * Description: Upgrades an legacy table to the new format
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
NVM_STATIC NVM_Status_t UpgradeLegacyTable
(
  void
);

/******************************************************************************
 * Name: NvUpdateSize
 * Description: Updates the size to be a multiple of 4 or 8 depending on the flash controller
 * Parameter(s): [IN] size - size to be updated
 * Return: the computed size
 *****************************************************************************/
NVM_STATIC uint32_t NvUpdateSize
(
  uint32_t size
);

/******************************************************************************
 * Name: NvEraseVirtualPage
 * Description: erase the specified page
 * Parameter(s): [IN] pageID - the ID of the page to be erased
 * Return: gNVM_InvalidPageID_c - if the page ID is not valid
 *         gNVM_SectorEraseFail_c - if the page cannot be erased
 *         gNVM_OK_c - if operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvEraseVirtualPage
(
  NVM_VirtualPageID_t pageID
);

/******************************************************************************
 * Name: NvInitStorageSystem
 * Description: Initialize the storage system, retrieve the active page and
 *              the page counter. Called once by NvModuleInit() function.
 * Parameter(s): [IN] read_legacy_location - if  set to true, the reads will be made on legacy page counter locations
 * Return: -
 *****************************************************************************/
NVM_STATIC void NvInitStorageSystem
(
  bool_t read_legacy_location
);

/******************************************************************************
 * Name: NvVirtualPageBlankCheck
 * Description: checks if the specified page is blank (erased)
 * Parameter(s): [IN] pageID - the ID of the page to be checked
 * Return: gNVM_InvalidPageID_c - if the page ID is not valid
 *         gNVM_PageIsNotBlank_c - if the page is not blank
 *         gNVM_OK_c - if the page is blank (erased)
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvVirtualPageBlankCheck
(
  NVM_VirtualPageID_t pageID
);

/******************************************************************************
 * Name: NvUpdateLastMetaInfoAddress
 * Description: retrieve and store (update) the last meta information address
 * Parameter(s): -
 * Return: gNVM_MetaNotFound_c - if no meta information has been found
 *         gNVM_OK_c - if the meta was found and stored (updated)
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvUpdateLastMetaInfoAddress
(
    void
);

/******************************************************************************
 * Name: NvGetMetaInfo
 * Description: get meta information based on the meta information address
 * Parameter(s): [IN] pageID - the ID of the page
 *               [IN] metaInfoAddress - meta information address
 *               [OUT] pMetaInfo - a pointer to a memory location where the
 *                                 requested meta information will be stored
 * Return: gNVM_InvalidPageID_c - if the active page is not valid
 *         gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_AddressOutOfRange_c - if the provided address is out of range
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvGetMetaInfo
(
  NVM_VirtualPageID_t pageId,
  uint32_t metaInfoAddress,
  NVM_RecordMetaInfo_t* pMetaInfo
);

/******************************************************************************
 * Name: NvGetPageFreeSpace
 * Description: return the page free space, in bytes
 * Parameter(s): [OUT] ptrFreeSpace - a pointer to a memory location where the
 *                                    page free space will be stored
 * Return: gNVM_InvalidPageID_c - if the active page is not valid
 *         gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_PageIsEmpty_c - if the page is empty
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvGetPageFreeSpace
(
  uint32_t* ptrFreeSpace
);

/******************************************************************************
 * Name: NvIsMemoryAreaAvailable
 * Description: checks if the specified memory area is blank (erased)
 * Parameter(s): [IN] address - start address
 *               [IN] len - length to be verified
 * Return: TRUE if the area is available (blank), FALSE otherwise
 *****************************************************************************/
NVM_STATIC bool_t NvIsMemoryAreaAvailable
(
  uint32_t address,
  uint32_t len
);

/******************************************************************************
 * Name: NvIsRecordCopied
 * Description: Checks if a record or an entire table entry is already copied.
 *              Called by page copy function.
 * Parameter(s): [IN] pageId - the ID of the page where to perform the check
 *               [IN] metaInf - a pointer to source page meta information tag
 * Return: TRUE if the element is already copied, FALSE otherwise
 *****************************************************************************/
NVM_STATIC bool_t NvIsRecordCopied
(
  NVM_VirtualPageID_t pageId,
  NVM_RecordMetaInfo_t* metaInf
);

/******************************************************************************
 * Name: NvInternalCopy
 * Description: Performs a copy of an record / entire table entry
 * Parameter(s): [IN] dstAddress - destination record address
 *               [IN] dstMetaAddress - destination meta address
 *               [IN] srcMetaInfo - source meta information
 *               [IN] srcTblEntryIdx - source table entry index
 *               [IN] size - bytes to copy
 * Return: gNVM_InvalidPageID_c - if the source or destination page is not
 *                                valid
 *         gNVM_MetaInfoWriteError_c - if the meta information couldn't be
 *                                     written
 *         gNVM_RecordWriteError_c - if the record couldn't be written
 *         gNVM_Error_c - in case of error(s)
 *         gNVM_OK_c - page copy completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvInternalCopy
(
  uint32_t dstAddress,
  uint32_t dstMetaAddress,
  NVM_RecordMetaInfo_t* srcMetaInfo,
  uint16_t srcTblEntryIdx,
  uint16_t size
);

/******************************************************************************
 * Name: NvGetTblEntryMetaAddrFromId
 * Description: Gets the table entry meta address based on table entry ID
 * Parameter(s): [IN] searchStartAddress - the search start address
 *               [IN] dataEntryId - table entry ID
 * Return: the value of the meta address
 *****************************************************************************/
#if gNvFragmentation_Enabled_d
NVM_STATIC uint32_t NvGetTblEntryMetaAddrFromId
(
  uint32_t searchStartAddress,
  uint16_t dataEntryId
);

/******************************************************************************
 * Name: NvInternalDefragmentedCopy
 * Description: Performs defragmentation and copy from the source page to
 *              the destination one
 * Parameter(s): [IN] srcMetaAddr - source page meta address
 *               [IN] srcTblEntryIdx - source page table entry index
 *               [IN] dstMetaAddr - destination meta address
 *               [IN] dstRecordAddr - destination record address (to copy to)
 *               [IN] ownerRecordMetaInfo - pointer to the location of a full dataset save
 * Return: the status of the operation
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvInternalDefragmentedCopy
(
  uint32_t srcMetaAddr,
  uint16_t srcTblEntryIdx,
  uint32_t dstMetaAddr,
  uint32_t dstRecordAddr,
  NVM_RecordMetaInfo_t *ownerRecordMetaInfo
);
#endif /* #if gNvFragmentation_Enabled_d */

/******************************************************************************
 * Name: NvCopyPage
 * Description: Copy the active page content to the mirror page. Only the
 *              latest table entries / elements are copied. A merge operation
 *              is performed before copy if an entry has single elements
 *              saved priori and newer than the table entry. If one or more
 *              elements were singular saved and the NV page doesn't has a
 *              full table entry saved, then the elements are copied as they
 *              are.
 * Parameter(s): [IN] skipEntryId - the entry ID to be skipped when page
 *                                  copy is performed
 * Return: gNVM_InvalidPageID_c - if the source or destination page is not
 *                                valid
 *         gNVM_MetaInfoWriteError_c - if the meta information couldn't be
 *                                     written
 *         gNVM_RecordWriteError_c - if the record couldn't be written
 *         gNVM_Error_c - in case of error(s)
 *         gNVM_OK_c - page copy completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvCopyPage
(
  NvTableEntryId_t skipEntryId
);

/******************************************************************************
 * Name: NvInternalFormat
 * Description: Format the NV storage system. The function erases in place both
 *              virtual pages and then writes the page counter value to first
 *              virtual page. The provided page counter value is automatically
 *              incremented and then written to first (active) virtual page.
 * Parameter(s): [IN] pageCounterValue - the page counter value that will
 *                                       be incremented and then written to
 *                                       active page
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_FormatFailure_c - if the format operation fails
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvInternalFormat
(
  uint32_t pageCounterValue
);

/******************************************************************************
 * Name: NvSaveRamTable
 * Description: Saves the NV table
 * Parameter(s): [IN] pageId - the virtual page ID where the table will be
 *                             saved
 * Return: TRUE if table saved successfully, FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvSaveRamTable
(
    NVM_VirtualPageID_t pageId
);

#if gNvUseExtendedFeatureSet_d

/******************************************************************************
 * Name: NvGetFlashTableSize
 * Description: Retrieves the size of the NV table
 * Parameter(s): -
 * Return: the NV table size
 ******************************************************************************/
NVM_STATIC uint32_t NvGetFlashTableSize
(
  void
);

/******************************************************************************
 * Name: NvIsRamTableUpdated
 * Description: Checks if the the NV table from RAM memory has changed since
 *              last system reset (e.g. via an OTA transfer)
 * Parameter(s): -
 * Return: TRUE if the NV RAM table has been changed / FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvIsRamTableUpdated
(
  void
);

/******************************************************************************
 * Name: NvGetTableEntry
 * Description: get the NV table entry information stored on FLASH memory
 * Parameter(s): [IN] tblEntryId - table entry ID
 *               [OUT] pDataEntry - a pointer to a memory location where the
 *                                  entry information will be stored
 * Return: TRUE if the has been found / FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvGetTableEntry
(
  uint16_t tblEntryId,
  NVM_DataEntry_t* pDataEntry
);
#endif /* gNvUseExtendedFeatureSet_d */

#endif /* no FlexNVM */

/******************************************************************************
 * Name: NvGetEntryFromDataPtr
 * Description: get table and element indexes based on a generic pointer address
 * Parameter(s): [IN] pData - a pointer to a NVM RAM table
 *               [OUT] pIndex - a pointer to a memory location where the
 *                              requested indexed will be stored
 * Return: gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_PointerOutOfRange_c - if the provided pointer cannot be founded
 *                                    within the RAM table
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvGetEntryFromDataPtr
(
  void* pData,
  NVM_TableEntryInfo_t* pIndex
);

/******************************************************************************
 * Name: NvGetTableEntryIndexFromDataPtr
 * Description: get table and element indexes based on a generic pointer address
 * Parameter(s): [IN] pData - a pointer to a NVM RAM table
 *               [OUT] pIndex - a pointer to a memory location where the
 *                              requested indexed will be stored
 *               [OUT] pTableEntryIdx - a pointer to a memory location where the
 *                              requested TableEntry Idx will be stored
 * Return: gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_PointerOutOfRange_c - if the provided pointer cannot be founded
 *                                    within the RAM table
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvGetTableEntryIndexFromDataPtr
(
    void* pData,
    NVM_TableEntryInfo_t* pIndex,
    uint16_t *pTableEntryIdx
);
/******************************************************************************
 * Name: NvWriteRecord
 * Description: writes a record
 * Parameter(s): [IN] tblIndexes - a pointer to table and element indexes
 * Return: gNVM_InvalidPageID_c - if the active page is not valid
 *         gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_MetaInfoWriteError_c - if the meta information couldn't be
 *                                     written
 *         gNVM_RecordWriteError_c - if the record couldn't be written
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvWriteRecord
(
  NVM_TableEntryInfo_t* tblIndexes
);

/******************************************************************************
 * Name: NvRestoreData
 * Description: restore an element from NVM storage to its original RAM location
 * Parameter(s): [IN] tblIdx - pointer to table and element indexes
 * Return: gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_PageIsEmpty_c - if page is empty
 *         gNVM_Error_c - in case of error(s)
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvRestoreData
(
  NVM_TableEntryInfo_t* tblIdx
);

/******************************************************************************
 * Name: NvGetTableEntryIndex
 * Description: get the table entry index from the provided ID
 * Parameter(s): [IN] entryId - the ID of the table entry
 * Return: table entry index of gNvInvalidTableEntryIndex_c
 *****************************************************************************/
NVM_STATIC uint16_t NvGetTableEntryIndexFromId
(
  NvTableEntryId_t entryId
);

/******************************************************************************
 * Name: NvAddSaveRequestToQueue
 * Description: Add save request to save requests queue; if the request is
 *              already stored, ignore the current request
 * Parameter(s): [IN] ptrTblIdx - pointer to table index
 * Return: gNVM_OK_c - if operation completed successfully
 *         gNVM_SaveRequestRejected_c - if the request couldn't be queued
 ******************************************************************************/
NVM_STATIC NVM_Status_t NvAddSaveRequestToQueue
(
  NVM_TableEntryInfo_t* ptrTblIdx
);

/******************************************************************************
 * Name: GetRandomRange
 * Description: Returns a random number between 'low' and 'high'
 * Parameter(s): [IN] low, high - generated number range
 * Return: 0..255
 ******************************************************************************/
NVM_STATIC uint8_t GetRandomRange
(
  uint8_t low,
  uint8_t high
);

#if (gNvUseFlexNVM_d == TRUE) /*** FlexNVM ***/

/******************************************************************************
 * Name: NvGetFlexLastMetaInfo
 * Description: Get FlexRAM last meta information address
 * Parameter(s): -
 * Return: the address of the last valid meta information
 ******************************************************************************/
NVM_STATIC uint32_t NvGetFlexLastMetaInfo
(
  void
);

/******************************************************************************
 * Name: NvGetFlexMetaInfoFromId
 * Description: Get FlexRAM meta information tag from table entry ID
 * Parameter(s): [IN] tblEntryId - table entry ID
 *               [OUT] pMetaInfo - a pointer to a memory location where the
 *                                 meta information tag will be stored
 * Return: -
 ******************************************************************************/
NVM_STATIC void NvGetFlexMetaInfoFromId
(
  NvTableEntryId_t tblEntryId,
  NVM_FlexMetaInfo_t* pMetaInfo
);

/******************************************************************************
 * Name: NvCheckNvmTableForFlexRAMUsage
 * Description: Check if the existing NVM table fits within the FlexRAM window
 * Parameter(s): -
 * Return: gNVM_NvTableExceedFlexRAMSize_c - the table exceed the size of
 *                                           FlexRAM window
 *         gNVM_OK_c - the table fits within the size of window FlexRAM window
 ******************************************************************************/
NVM_STATIC NVM_Status_t NvCheckNvmTableForFlexRAMUsage
(
  void
);

#endif /* gNvUseFlexNVM_d */
#endif /* gNvStorageIncluded_d */

/*****************************************************************************
 *****************************************************************************
 * Private memory declarations
 *****************************************************************************
 *****************************************************************************/

#if gNvStorageIncluded_d

#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */

/*
 * Name: mNvActivePageId
 * Description: variable that holds the ID of the active page
 */
#if !gNvDebugEnabled_d
NVM_STATIC
#endif
NVM_VirtualPageID_t mNvActivePageId;

/*
 * Name: mNvPageCounter
 * Description: page counter, used to validate the entire virtual page
 *              and also to provide statistical information about
 *              how many times the virtual page was erased
 */
NVM_STATIC uint32_t mNvPageCounter = 0;

/*
 * Name: mNvVirtualPageProperty
 * Description: virtual page properties
 */
#if !gNvDebugEnabled_d
NVM_STATIC
#endif
NVM_VirtualPageProperties_t mNvVirtualPageProperty[gNvVirtualPagesCount_c];

/*
 * Name: mNvCopyOperationIsPending
 * Description: a flag that a indicates that a page copy operation is requested
 */
NVM_STATIC bool_t mNvCopyOperationIsPending = FALSE;

/*
 * Name: mNvErasePgCmdStatus
 * Description: a data structure used to erase a virtual page. The erase of a
 *              virtual page is performed in idle task, in a sector-by-sector
 *              manner. When the idle task runs, if the erase pending flag is
 *              set, only one flash sector will be erased. Therefore, the
 *              virtual page will be entirely erased after several runs of
 *              idle task
 */
NVM_STATIC NVM_ErasePageCmdStatus_t mNvErasePgCmdStatus;

/*
 * Name: mNvFlashConfigInitialised
 * Description: variable that holds the hal driver and active page initialisation status
 */
NVM_STATIC bool_t mNvFlashConfigInitialised = FALSE;

/*
 * Name: maNvRecordsCpyIdx
 * Description: An array that stores the indexes of the records already copied;
 *              Used by the defragmentation process.
 */
#if gNvFragmentation_Enabled_d
NVM_STATIC uint16_t maNvRecordsCpyOffsets[gNvRecordsCopiedBufferSize_c];
#endif /* gNvFragmentation_Enabled_d */

#if gNvUseExtendedFeatureSet_d
/*
 * Name: mNvTableSizeInFlash
 * Description: the size of the NV table stored in the FLASH memory
 */
#if !gNvDebugEnabled_d
NVM_STATIC
#endif
uint32_t mNvTableSizeInFlash = 0;

/*
 * Name: mNvTableMarker
 * Description: FLASH NV table marker, used only for code readability
 *              (when applying the sizeof() operator to it)
 */
NVM_STATIC uint16_t mNvTableMarker = gNvTableMarker_c;

/*
 * Name: mNvTableMarker
 * Description: FLASH NV application version, used for determining when table upgrade
 *              happened
 */
#if !gNvDebugEnabled_d
NVM_STATIC
#endif
uint16_t mNvFlashTableVersion = gNvFlashTableVersion_c;

/*
 * Name: mNvTableUpdated
 * Description: boolean flag used to mark if the NV table from the RAM memory
 *              has been changed. Set (or left untouched) only at module initialization,
 *              when the existing NV FLASH table (if any) is compared against
 *              the NV RAM table.
 */
NVM_STATIC bool_t mNvTableUpdated;

#endif /* gNvUseExtendedFeatureSet_d */

#endif /* no FlexNVM */

/*
 * Name: mNvModuleInitialized
 * Description: variable that holds the NVM initialisation status
 */
NVM_STATIC bool_t mNvModuleInitialized = FALSE;

/*
 * Name: mNvCriticalSectionFlag
 * Description: If this counter is != 0, do not save to NV Storage
 */
NVM_STATIC uint8_t mNvCriticalSectionFlag = 0;

/*
 * Name: gNvMinimumTicksBetweenSaves
 * Description: Minimum number of calls to NvTimerTick() between saves of a given data set
 */
NVM_STATIC NvSaveInterval_t gNvMinimumTicksBetweenSaves = gNvMinimumTicksBetweenSaves_c;

/*
 * Name: gNvCountsBetweenSaves
 * Description: Minimum number of calls to NvSaveOnIdle() between saves of a given data set
 */
NVM_STATIC NvSaveCounter_t gNvCountsBetweenSaves = gNvCountsBetweenSaves_c;

/*
 * Name: mNvPendingSavesQueue
 * Description: a queue used for storing information about the pending saves
 */
NVM_STATIC NVM_SaveQueue_t mNvPendingSavesQueue;

/*
 * Name: maDatasetInfo
 * Description: Data set info table
 */
NVM_STATIC NVM_DatasetInfo_t maDatasetInfo[gNvTableEntriesCountMax_c];

/*
 * Name: mNvSaveOnIntervalEvent
 * Description: flag used to signal an 'SaveOnInterval' event
 */
NVM_STATIC bool_t mNvSaveOnIntervalEvent;

/*
 * Name: mNvLastTimestampValue
 * Description: the value of the last timestamp used by the Save-On-Interval functionality
 */
NVM_STATIC uint64_t mNvLastTimestampValue = 0;

/*
 * Name: mNVMMutexId
 * Description: mutex used to ensure NVM functions thread switch safety
 */
#if !gNvDebugEnabled_d
NVM_STATIC
#endif
OSA_MUTEX_HANDLE_DEFINE(mNVMMutexId);

/*
 * Name: mNvIdleTaskId
 * Description: stores the Id of the task which hosts NvIdle function.
 */
static osa_task_handle_t mNvIdleTaskId = NULL;



/*
 * Name: eraseNVMFirst
 * Description: byte used to the force the erasure of the first sector of
 *              the first virtual page (thus invalidating the entire page)
 *              via IAR flashloader. Below section must be defined in the
 *              linker configuration file (*.icf)
 */
#if defined ( __IAR_SYSTEMS_ICC__ )
#pragma section = "fEraseNVM"
#pragma location = "fEraseNVM"
NVM_STATIC const uint32_t eraseNVMFirst[4]= {0xffffffff, 0xffffffff, 0xffffffff,  0xffffffff };

/*
 * Name: eraseNVMSecond
 * Description: byte used to the force the erasure of the first sector of
 *              the second virtual page (thus invalidating the entire page)
 *              via IAR flashloader. Below section must be defined in the
 *              linker configuration file (*.icf)
 */
#pragma section = "sEraseNVM"
#pragma location = "sEraseNVM"

NVM_STATIC const uint32_t eraseNVMSecond[4]={0xffffffff, 0xffffffff, 0xffffffff,  0xffffffff };

#endif /* __IAR_SYSTEMS_ICC__  */
#endif /* gNvStorageIncluded_d */

/*****************************************************************************
 *****************************************************************************
 * Public memory declarations
 *****************************************************************************
 *****************************************************************************/

#if gNvStorageIncluded_d

#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
#if (!(defined(__CC_ARM) || defined(__UVISION_VERSION)))
/*
 * Name: NV_STORAGE_START_ADDRESS
 * Description: NV_STORAGE_START_ADDRESS from linker command file is used by this code
 *              as Raw Sector Start Address.
 */
extern uint32_t NV_STORAGE_START_ADDRESS[];

/*
 * Name: NV_STORAGE_SECTOR_SIZE
 * Description: external symbol from linker command file, it represents the size
 *              of a FLASH sector
 */
extern uint32_t NV_STORAGE_SECTOR_SIZE[];

/*
 * Name:  NV_STORAGE_MAX_SECTORS
 * Description: external symbol from linker command file, it represents the sectors
 *              count used by the ENVM storage system; it has to be a multiple of 2
 */
extern uint32_t  NV_STORAGE_MAX_SECTORS[];
#else

extern uint32_t Image$$NVM_region$$ZI$$Base[];
extern uint32_t Image$$NVM_region$$ZI$$Limit[];
extern uint32_t Image$$NVM_region$$Length;

#define NV_STORAGE_START_ADDRESS (Image$$NVM_region$$ZI$$Base)
#define NV_STORAGE_END_ADDRESS (Image$$NVM_region$$ZI$$Limit)
#define NVM_LENGTH ((uint32_t)((uint8_t*)NV_STORAGE_END_ADDRESS)-(uint32_t)((uint8_t*)NV_STORAGE_START_ADDRESS))
#define NV_STORAGE_SECTOR_SIZE FSL_FEATURE_FLASH_PAGE_SIZE_BYTES
#define NV_STORAGE_MAX_SECTORS (NVM_LENGTH/NV_STORAGE_SECTOR_SIZE)
#endif /* __CC_ARM */
#endif /* no FlexNVM */

/*
 * Name:  pNVM_DataTable
 * Description: Pointer to NVM table. The table itself can be stored in FLASH (default)
 *              or in RAM memory. If stored in RAM, the gNVM_TABLE_startAddr_c must be updated
 *              accordingly
 */
NVM_DataEntry_t* const pNVM_DataTable = (NVM_DataEntry_t*)gNVM_TABLE_startAddr_c;

#endif /* gNvStorageIncluded_d */

/*****************************************************************************
 *****************************************************************************
 * Private functions
 *****************************************************************************
 *****************************************************************************/

#if gNvStorageIncluded_d
#if gNvUseExtendedFeatureSet_d
/******************************************************************************
 * Name: __NvRegisterTableEntry
 * Description: The function tries to register a new table entry within an
 *              existing NV table. If the NV table contained an erased (invalid)
 *              entry, the entry will be overwritten with a new one (provided
 *              by the mean of this function arguments)
 * Parameter(s): [IN] ptrData - generic pointer to RAM data to be registered
 *                              within the NV storage system
 *               [IN] uniqueId - an unique ID of the table entry
 *               [IN] elemCount - how many elements the table entry contains
 *               [IN] elemSize - the size of an element
 *               [IN] dataEntryType - the type of the new entry
 *               [IN] overwrite - if an existing table entry shall be
 *                                overwritten
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_RegisterFailure_c - invalid id or unmirrored data set
 *         gNVM_AlreadyRegistered - the id is allready registered in another entry
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *****************************************************************************/
#if gNvTableKeptInRam_d
NVM_STATIC NVM_Status_t __NvRegisterTableEntry
(
void* ptrData,
NvTableEntryId_t uniqueId,
uint16_t elemCount,
uint16_t elemSize,
uint16_t dataEntryType,
bool_t overwrite
)
{
    uint16_t loopCnt = 0;
    uint16_t nullPos = gNvTableEntriesCountMax_c;
    NVM_Status_t status;
    bool_t ret = FALSE;
    uint32_t tableEndAddr = (uint32_t)gNVM_TABLE_endAddr_c;
    uint32_t tableStartAddr = (uint32_t)gNVM_TABLE_startAddr_c;

    if(!mNvModuleInitialized)
    {
        status = gNVM_ModuleNotInitialized_c;
    }
    else
    {
        if(gNvInvalidDataEntry_c == uniqueId)
        {
            status = gNVM_RegisterFailure_c;
        }
        else
        {
            #if gNvFragmentation_Enabled_d
            if (elemCount > gNvRecordsCopiedBufferSize_c)
            {
                status = gNVM_DefragBufferTooSmall_c;
            }
            else
            #endif
            {
                while(loopCnt < (uint16_t)((tableEndAddr - tableStartAddr)/sizeof(NVM_DataEntry_t)))
                {
                    if ((pNVM_DataTable[loopCnt].pData == NULL) && (!overwrite))
                    {
                        nullPos = loopCnt;
                        break;
                    }

                    if(pNVM_DataTable[loopCnt].DataEntryID == uniqueId)
                    {
                        if(overwrite)
                        {
                            /* make sure that the NvWriteRamTable writes the updated values */
                            pNVM_DataTable[loopCnt].pData = ptrData;
                            pNVM_DataTable[loopCnt].ElementsCount = elemCount;
                            pNVM_DataTable[loopCnt].ElementSize = elemSize;
                            pNVM_DataTable[loopCnt].DataEntryType = dataEntryType;
                            /*force page copy first*/
                            status = __NvEraseEntryFromStorage(uniqueId, loopCnt);
                        }
                        else
                        {
                            status = gNVM_AlreadyRegistered;
                        }
                        ret = TRUE;
                        break;
                    }
                    /* increment the loop counter */
                    loopCnt++;
                }

                if(FALSE == ret)
                {
                    if(gNvTableEntriesCountMax_c != nullPos)
                    {
                        pNVM_DataTable[nullPos].pData= ptrData;
                        pNVM_DataTable[nullPos].DataEntryID = uniqueId;
                        pNVM_DataTable[nullPos].ElementsCount = elemCount;
                        pNVM_DataTable[nullPos].ElementSize = elemSize;
                        pNVM_DataTable[nullPos].DataEntryType = dataEntryType;

                        /* postpone the operation */
                        if (mNvCriticalSectionFlag)
                        {
                            mNvCopyOperationIsPending = TRUE;
                            status = gNVM_CriticalSectionActive_c;
                        }
                        else
                        {
                            /*update the flash table*/
                            #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                            FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
                            FSCI_MsgNVVirtualPageMonitoring(FALSE,status=NvCopyPage(gNvCopyAll_c));
                            #else
                            status = NvCopyPage(gNvCopyAll_c);
                            #endif
                        }
                    }
                    else
                    {
                        status = gNVM_RegisterFailure_c;
                    }
                }
            }
        }
    }
    return status;
}
#endif //gNvTableKeptInRam_d

/******************************************************************************
 * Name: GetFlashTableVersion
 * Description: returns the flash table version
 * Parameter(s): -
 * Return: 0 or flash table version
 *****************************************************************************/
uint16_t GetFlashTableVersion
(
    void
)
{
    uint16_t ret = 0U;
    InitNVMConfig();
    if ( gVirtualPageNone_c != mNvActivePageId )
    {
        ret = (*(NVM_TableInfo_t*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress)).fields.NvTableVersion;
    }
    return ret;
}

/******************************************************************************
 * Name: __NvEraseEntryFromStorage
 * Description: The function removes a table entry within the existing NV
 *              table. The RAM table must be updated before this call.
 * Parameter(s): [IN] entryId - the entry id of the entry that is removed
 *               [IN] tableEntryIndex - the index of the entry in the ram table
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *****************************************************************************/
#if gNvTableKeptInRam_d
NVM_STATIC NVM_Status_t __NvEraseEntryFromStorage
(
    uint16_t entryId,
    uint16_t tableEntryIndex
)
{
    uint16_t loopCnt;
    NVM_Status_t status = gNVM_OK_c;
    uint16_t remaining_count;

    if(!mNvModuleInitialized)
    {
        status = gNVM_ModuleNotInitialized_c;
    }
    else
    {
        /* Check if is in pending queue - if yes than remove it */
        if (NvGetPendingSavesCount(&mNvPendingSavesQueue) != 0u)
        {
            /* Start from the queue's head */
            loopCnt = mNvPendingSavesQueue.Head;
            remaining_count = mNvPendingSavesQueue.EntriesCount;

            while (remaining_count)
            {
                if(entryId == mNvPendingSavesQueue.QData[loopCnt].entryId)
                {
                    mNvPendingSavesQueue.QData[loopCnt].entryId = gNvInvalidDataEntry_c;
                }
                remaining_count--;
                /* increment and wrap the loop index */
                if(++loopCnt >= (uint8_t)gNvPendingSavesQueueSize_c)
                {
                    loopCnt=0;
                }
            }
        }
        maDatasetInfo[tableEntryIndex].countsToNextSave = gNvCountsBetweenSaves;
        maDatasetInfo[tableEntryIndex].saveNextInterval = FALSE;

        /* postpone the operation */
        if (mNvCriticalSectionFlag)
        {
            mNvCopyOperationIsPending = TRUE;
            status = gNVM_CriticalSectionActive_c;
        }
        else
        {
            /* erase the table entry by making a copy of the active page to the inactive one,
            * but skipping while copying the table entry to be erased */
            if (gEmptyPageMetaAddress_c != mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress)
            {
                #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
                FSCI_MsgNVVirtualPageMonitoring(FALSE, status = NvCopyPage(entryId));
                #else
                status = NvCopyPage(entryId);
                #endif
            }
        }
    }
    return status;
}
#endif //gNvTableKeptInRam_d

/******************************************************************************
 * Name: RecoverNvEntry
 * Description: Reads a flash entry so that the application can handle dynamic entries.
 * Parameter(s): [IN] index - the ram entry index
 *               [OUT] entry - the flash entry at the specified index
 * Return: gNVM_OK_c - if the operation completes successfully
           gNVM_RestoreFailure_c - if the operation failed
           gNVM_AddressOutOfRange_c - if the index is too large
           gNVM_Error_c - not supported, NVM table is stored in FLASH
 *****************************************************************************/
NVM_Status_t RecoverNvEntry
(
    uint16_t index,
    NVM_DataEntry_t *entry
)
{
#if gNvTableKeptInRam_d

    uint32_t addr;
    NVM_EntryInfo_t entryInfo;
    NVM_Status_t status = gNVM_OK_c;

    entry->pData = NULL;
    InitNVMConfig();
    if (mNvActivePageId == gVirtualPageNone_c)
    {
        status = gNVM_RestoreFailure_c;
    }
    else
    {
        if (index * sizeof(NVM_EntryInfo_t) >= mNvTableSizeInFlash)
        {
            status = gNVM_AddressOutOfRange_c;
        }
        else
        {
            addr = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + sizeof(NVM_TableInfo_t) + index * sizeof(NVM_EntryInfo_t);
            NV_FlashRead(addr, (uint8_t*)&entryInfo, sizeof(NVM_EntryInfo_t));
            entry->DataEntryID   = entryInfo.fields.NvDataEntryID;
            entry->DataEntryType = entryInfo.fields.NvDataEntryType;
            entry->ElementsCount = entryInfo.fields.NvElementsCount;
            entry->ElementSize   = entryInfo.fields.NvElementSize;
        }
    }
    return status;
#else  /*gNvTableKeptInRam_d*/
    return gNVM_Error_c;
#endif /*gNvTableKeptInRam_d*/
}

/******************************************************************************
 * Name: NvIsRamTableUpdated
 * Description: Checks if the the NV table from RAM memory has changed since
 *              last system reset (e.g. via an OTA transfer)
 * Parameter(s): -
 * Return: TRUE if the NV RAM table has been changed / FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvIsRamTableUpdated
(
    void
)
{
    uint16_t idx;
    uint32_t addr;
    uint32_t endAddr;
    bool_t idFound;
    bool_t ret = FALSE;
    NVM_EntryInfo_t entryInfo;

    /* address = page raw sector start address + page counter size + table marker + table version */
    addr = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + sizeof(NVM_TableInfo_t);

    /* compute the search end address */
    endAddr = addr + mNvTableSizeInFlash;

    do
    {
        /* read ID */
        NV_FlashRead((uint32_t*)addr, (uint8_t*)&entryInfo, sizeof(NVM_EntryInfo_t));

        idFound = FALSE;
        idx = 0;

        while(idx < gNVM_TABLE_entries_c)
        {
            if(entryInfo.fields.NvDataEntryID == pNVM_DataTable[idx].DataEntryID)
            {
                idFound = TRUE;
                break;
            }

            /* increment the index */
            idx++;
        }

        if(!idFound)
        {
            ret = TRUE;
            break;
        }

        if ((((uint16_t)gNVM_MirroredInRam_c == entryInfo.fields.NvDataEntryType) ||
            ((uint16_t)gNVM_MirroredInRam_c == pNVM_DataTable[idx].DataEntryType)) && (entryInfo.fields.NvDataEntryType != pNVM_DataTable[idx].DataEntryType))
        {
            ret = TRUE;
            break;
        }
        if ((entryInfo.fields.NvElementsCount != pNVM_DataTable[idx].ElementsCount) ||
            (entryInfo.fields.NvElementSize   != pNVM_DataTable[idx].ElementSize))
        {
            ret = TRUE;
            break;
        }

        /* increment the address */
        addr += sizeof(NVM_EntryInfo_t);

    } while(addr < endAddr);

    return ret;
}
/******************************************************************************
 * Name: NvGetTableEntry
 * Description: get the NV table entry information stored on FLASH memory
 * Parameter(s): [IN] tblEntryId - table entry ID
 *               [OUT] pDataEntry - a pointer to a memory location where the
 *                                  entry information will be stored
 * Return: TRUE if the has been found / FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvGetTableEntry
(
    uint16_t tblEntryId,
    NVM_DataEntry_t* pDataEntry
)
{
    uint32_t addr;
    NVM_EntryInfo_t tmp;
    NVM_TableInfo_t table;
    bool_t ret = FALSE;

    pDataEntry->pData = NULL; /* the data pointer is not saved on FLASH table and
    * shall not be used by the caller of this function */

    addr = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;

    NV_FlashRead((uint32_t*)addr, &tmp, sizeof(NVM_TableInfo_t));
    table.rawValue = tmp.rawValue;
    if ( mNvTableMarker == table.fields.NvTableMarker )
    {
        /* increment address */
        addr += sizeof(NVM_TableInfo_t);

        do
        {
            NV_FlashRead((uint32_t*)addr, &tmp, sizeof(NVM_EntryInfo_t));
            table.rawValue = tmp.rawValue;
            if(table.fields.NvTableMarker == mNvTableMarker)
            {
                /* reached end of table */
                break;
            }
            if(tmp.fields.NvDataEntryID == tblEntryId)
            {
                pDataEntry->DataEntryID   = tmp.fields.NvDataEntryID;
                pDataEntry->DataEntryType = tmp.fields.NvDataEntryType;
                pDataEntry->ElementsCount = tmp.fields.NvElementsCount;
                pDataEntry->ElementSize   = tmp.fields.NvElementSize;
                ret = TRUE;
                break;
            }

            /* continue searching */
            addr += sizeof(NVM_EntryInfo_t);
        } while(addr < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress);
    }

    if( FALSE == ret)
    {
        pDataEntry->DataEntryType = 0;
        pDataEntry->ElementsCount = 0;
        pDataEntry->ElementSize = 0;
        pDataEntry->DataEntryID = gNvInvalidDataEntry_c;
    }
    return ret;
}

/******************************************************************************
 * Name: NvGetFlashTableSize
 * Description: Retrieves the size of the NV table
 * Parameter(s): -
 * Return: the NV table size
 ******************************************************************************/
NVM_STATIC uint32_t NvGetFlashTableSize
(
    void
)
{
    uint32_t addr;
    uint32_t size = 0U;
    NVM_TableInfo_t tableInfo;

    /* compute the size of the table stored in Flash memory */
    addr = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;

    NV_FlashRead((uint32_t*)addr, (uint8_t*)&tableInfo, sizeof(NVM_TableInfo_t));

    if(gNvTableMarker_c == tableInfo.fields.NvTableMarker)
    {
        addr += sizeof(NVM_TableInfo_t);

        do
        {
            NV_FlashRead((uint32_t*)addr, (uint8_t*)&tableInfo, sizeof(NVM_TableInfo_t));

            if(gNvTableMarker_c == tableInfo.fields.NvTableMarker)
            {
                break;
            }
            size += sizeof(NVM_TableInfo_t);
            addr += sizeof(NVM_TableInfo_t);

        } while(addr < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress);
    }
    return size;
}

#endif /* gNvUseExtendedFeatureSet_d */

/******************************************************************************
 * Name: __NvAtomicSave
 * Description: The function performs an atomic save of the entire NV table
 *              to the storage system. The operation is performed
 *              in place (atomic).
 * Parameter(s):  -
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *         gNVM_PointerOutOfRange_c - if the pointer is out of range
 *         gNVM_InvalidTableEntry_c - if the table entry is not valid
 *         gNVM_MetaInfoWriteError_c - meta tag couldn't be written
 *         gNVM_RecordWriteError_c - record couldn't be written
 *         gNVM_CriticalSectionActive_c - the module is in critical section
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvAtomicSave
(
    void
)
{
    NVM_Status_t status = gNVM_OK_c;
    uint16_t loopCnt = 0;
    uint32_t tableEndAddr = (uint32_t)gNVM_TABLE_endAddr_c;
    uint32_t tableStartAddr = (uint32_t)gNVM_TABLE_startAddr_c;
    NVM_TableEntryInfo_t tblIdx;
#if gUnmirroredFeatureSet_d
    uint16_t loopCnt2 = 0;
    uint16_t remaining_count;
    uint16_t tableEntryIdx;
    bool_t skip;
    bool_t ret = FALSE;
#endif

    do
    {
    /* remove all non unmirrored erase operations from the queue */
#if gUnmirroredFeatureSet_d
        if (NvGetPendingSavesCount(&mNvPendingSavesQueue) != 0u)
        {
            /* Start from the queue's head */
            loopCnt = mNvPendingSavesQueue.Head;
            remaining_count = mNvPendingSavesQueue.EntriesCount;

            while (remaining_count != 0u)
            {
                skip = FALSE;
                tableEntryIdx = NvGetTableEntryIndexFromId(mNvPendingSavesQueue.QData[loopCnt].entryId);
                if (gNvInvalidTableEntryIndex_c != tableEntryIdx)
                {
                    if (gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType)
                    {
                        if (NULL == ((void**) pNVM_DataTable[tableEntryIdx].pData)[mNvPendingSavesQueue.QData[loopCnt].elementIndex])
                        {
                            skip = TRUE;
                        }
                    }
                }
                if (FALSE == skip)
                {
                    mNvPendingSavesQueue.QData[loopCnt].entryId = gNvInvalidDataEntry_c;
                }
                remaining_count--;
                /* increment and wrap the loop index */
                if(++loopCnt >= (uint8_t)gNvPendingSavesQueueSize_c)
                {
                    loopCnt=0;
                }
            }
        }
#else  /*gUnmirroredFeatureSet_d*/
        (void)NvInitPendingSavesQueue(&mNvPendingSavesQueue);
#endif /*gUnmirroredFeatureSet_d*/
        /* if critical section, add a special entry in the queue */
        if (mNvCriticalSectionFlag != 0u)
        {
            tblIdx.entryId = gNvCopyAll_c;
            tblIdx.elementIndex = gNvCopyAll_c;
            tblIdx.saveRestoreAll = TRUE;
            status = NvAddSaveRequestToQueue(&tblIdx);
            if ((gNVM_SaveRequestRejected_c != status) && (gNVM_AtomicSaveRecursive_c != status))
            {
                status = gNVM_CriticalSectionActive_c;
            }
        }
        else
        {
            while(loopCnt < (uint16_t)((tableEndAddr - tableStartAddr)/sizeof(NVM_DataEntry_t)))
            {
               #if gUnmirroredFeatureSet_d
                if (gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[loopCnt].DataEntryType)
                {
                    for (loopCnt2 = 0; loopCnt2 < pNVM_DataTable[loopCnt].ElementsCount; loopCnt2++)
                    {
                        status = __NvSyncSave(&((uint8_t**) pNVM_DataTable[loopCnt].pData)[loopCnt2], FALSE);
                        if (gNVM_NullPointer_c == status)
                        {
                            /* skip */
                            continue;
                        }

                        if (gNVM_OK_c != status)
                        {
                            ret = TRUE;
                            break;
                        }
                    }
                    if(TRUE == ret)
                    {
                        break;
                    }
                }
                else
               #endif
                {
                    status = __NvSyncSave(pNVM_DataTable[loopCnt].pData, TRUE);
                    if (gNVM_NullPointer_c == status)
                    {
                        /* skip */
                        loopCnt++;
                        continue;
                    }

                    if (gNVM_OK_c != status)
                    {
                        break;
                    }
                }

                /* increment the loop counter */
                loopCnt++;
            }
        }
    }
    while(status == gNVM_AtomicSaveRecursive_c);
    return status;
}

/******************************************************************************
 * Name: __NvSyncSave
 * Description: The function saves the pointed element or the entire table
 *              entry to the storage system. The save operation is not
 *              performed on the idle task but within this function call.
 * Parameter(s): [IN] ptrData - a pointer to data to be saved
 *               [IN] saveAll - specifies if the entire table entry shall be
 *                              saved or only the pointed element
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *         gNVM_PointerOutOfRange_c - if the pointer is out of range
 *         gNVM_InvalidTableEntry_c - if the table entry is not valid
 *         gNVM_MetaInfoWriteError_c - meta tag couldn't be written
 *         gNVM_RecordWriteError_c - record couldn't be written
 *         gNVM_CriticalSectionActive_c - the module is in critical section
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvSyncSave
(
    void* ptrData,
    bool_t saveAll
)
{
    NVM_TableEntryInfo_t tblIdx;
    NVM_Status_t status;

    if(!mNvModuleInitialized)
    {
        status = gNVM_ModuleNotInitialized_c;
    }
    else
    {
        if(NULL == ptrData)
        {
            status = gNVM_NullPointer_c;
        }
        else
        {
            status = NvGetEntryFromDataPtr(ptrData, &tblIdx);
            if( gNVM_OK_c == status )
            {
                /* write the save all flag */
                #if gNvFragmentation_Enabled_d
                tblIdx.saveRestoreAll = saveAll;
                #else
                tblIdx.saveRestoreAll = TRUE;
                #endif /* gNvFragmentation_Enabled_d */

                if (mNvCriticalSectionFlag != 0u)
                {
                    status = NvAddSaveRequestToQueue(&tblIdx);
                    if (gNVM_SaveRequestRejected_c != status)
                    {
                        status = gNVM_CriticalSectionActive_c;
                    }
                }
                else
                {
                    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
                    status = NvWriteRecord(&tblIdx);
                    if(status == gNVM_PageCopyPending_c)
                    {
                          /* copy active page */
                          #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                          FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
                          FSCI_MsgNVVirtualPageMonitoring(FALSE,status=NvCopyPage(gNvCopyAll_c));
                          #else
                          status = NvCopyPage(gNvCopyAll_c);
                          #endif
                          if(status == gNVM_OK_c)
                          {
                              mNvCopyOperationIsPending = FALSE;

                              /* erase old page */
                              status = NvEraseVirtualPage(mNvErasePgCmdStatus.NvPageToErase);
                              if (gNVM_OK_c == status)
                              {
                                  mNvVirtualPageProperty[mNvErasePgCmdStatus.NvPageToErase].NvLastMetaInfoAddress = gEmptyPageMetaAddress_c;
                                  mNvErasePgCmdStatus.NvErasePending = FALSE;
                                  /* write record */
                                  status = NvWriteRecord(&tblIdx);
                              }
                          }
                    }
                    #else /* FlexNVM */
                    /* write record */
                    status = NvWriteRecord(&tblIdx);
                    #if FSL_FEATURE_FLASH_IS_FTFL
                    /* wait for EEPROM system to be ready (fix@ENGR283453) */
                    while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET,  FTFx_SSD_FCNFG_EEERDY)));
                    #endif
                    #endif
                }
            }
        }
    }

    return status;
}

/******************************************************************************
 * Name: __NvFormat
 * Description: Format the NV storage system. The function erases both virtual
 *              pages and then writes the page counter/ram table to active page.
 * Parameter(s): -
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_FormatFailure_c - if the format operation fails
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_CriticalSectionActive_c - if the system has entered in a
 *                                        critical section
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvFormat
(
    void
)
{
    NVM_Status_t status = gNVM_OK_c;
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    NVM_TableInfo_t tableInfo;
#if gUnmirroredFeatureSet_d
    uint32_t tableEndAddr = (uint32_t)gNVM_TABLE_endAddr_c;
    uint32_t tableStartAddr = (uint32_t)gNVM_TABLE_startAddr_c;
    uint16_t loopCnt;
    uint16_t loopCnt2;
#endif

    if(!mNvModuleInitialized)
    {
        status = gNVM_ModuleNotInitialized_c;
    }
    else
    {
        if(mNvCriticalSectionFlag != 0u)
        {
            status = gNVM_CriticalSectionActive_c;
        }
        else
        {
            NV_FlashRead((uint32_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress, (uint8_t*)&tableInfo, sizeof(NVM_TableInfo_t));

            status = NvInternalFormat((uint32_t)tableInfo.fields.NvPageCounter);
            if(gNVM_OK_c == status)
            {
                #if gUnmirroredFeatureSet_d
                {
                    for(loopCnt = 0; loopCnt < (uint16_t)((tableEndAddr - tableStartAddr)/sizeof(NVM_DataEntry_t)); loopCnt++)
                    {
                        maDatasetInfo[loopCnt].countsToNextSave = gNvCountsBetweenSaves;
                        maDatasetInfo[loopCnt].saveNextInterval = FALSE;
                        if((NVM_DataEntryType_t)pNVM_DataTable[loopCnt].DataEntryType != gNVM_MirroredInRam_c)
                        {
                            for (loopCnt2 = 0; loopCnt2 < pNVM_DataTable[loopCnt].ElementsCount; loopCnt2++)
                            {
                                if (NvIsNVMFlashAddress(((void**)pNVM_DataTable[loopCnt].pData)[loopCnt2]))
                                {
                                    ((void**)pNVM_DataTable[loopCnt].pData)[loopCnt2] = NULL;
                                }
                            }
                        }
                    }
                }
                #endif
                /* clear the save queue */
                (void)NvInitPendingSavesQueue(&mNvPendingSavesQueue);
            }
        }
    }
#else /* FlexNVM */

    uint8_t buff[gNvFlexFormatBufferSize_c];
    uint32_t addr;
    uint16_t size;

    FLib_MemSet(buff, gNvErasedFlashCellValue_c, gNvFlexFormatBufferSize_c);

#if FSL_FEATURE_FLASH_IS_FTFL
    addr = gFlexNvmDriver.ftfxConfig.flexramBlockBase;
    size = gFlexNvmDriver.ftfxConfig.flexramTotalSize;

    while(size)
    {
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET , FTFx_SSD_FCNFG_EEERDY)));

        if(kStatus_FLASH_Success != EEEWrite(&gFlexNvmDriver, addr, sizeof(buff), buff))
        {
            status = gNVM_FormatFailure_c;
            break;
        }
        size -= gNvFlexFormatBufferSize_c;
        addr += gNvFlexFormatBufferSize_c;
    }
#endif /* FSL_FEATURE_FLASH_IS_FTFL */

#if FSL_FEATURE_FLASH_IS_FTFE
    addr = gFlexNvmDriver.ftfxConfig.flexramBlockBase;
    size = gFlexNvmDriver.ftfxConfig.flexramTotalSize;

    while(size)
    {
        if(kStatus_FLASH_Success != FLEXNVM_EepromWrite(&gFlexNvmDriver, addr, buff, sizeof(buff)))
        {
            status = gNVM_FormatFailure_c;
            break;
        }
        size -= gNvFlexFormatBufferSize_c;
        addr += gNvFlexFormatBufferSize_c;
    }
#endif /* FSL_FEATURE_FLASH_IS_FTFE */

#endif /* gNvUseFlexNVM_d */
    return status;
}

/******************************************************************************
 * Name: __NvIdle
 * Description: Called from the idle task (bare-metal) or NVM_Task (MQX,
 *              FreeRTOS) to process the pending saves, erase or copy
 *              operations.
 * Parameters: -
 * Return: -
 ******************************************************************************/
NVM_STATIC void __NvIdle
(
    void
)
{
    NVM_TableEntryInfo_t tblIdx;
    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    NVM_Status_t status;
    bool_t ret = FALSE;
    #endif

    uint64_t currentTimestampValue = 0;

    #if gNvmUseTimerJitter_d
    uint8_t timerJitter = 0;
    #endif

    if (mNvModuleInitialized&& (mNvCriticalSectionFlag== 0u))
    {
        #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
        if(mNvCopyOperationIsPending)
        {
            #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
                FSCI_MsgNVVirtualPageMonitoring(FALSE,status = NvCopyPage(gNvCopyAll_c));
            #else
                status = NvCopyPage(gNvCopyAll_c);
            #endif
            if (gNVM_OK_c == status)
            {
                mNvCopyOperationIsPending = FALSE;
            }
        }

        if(mNvErasePgCmdStatus.NvErasePending)
        {
            if(mNvErasePgCmdStatus.NvSectorAddress >= mNvVirtualPageProperty[mNvErasePgCmdStatus.NvPageToErase].NvRawSectorEndAddress)
            {
                /* all sectors of the page had been erased */
                mNvVirtualPageProperty[mNvErasePgCmdStatus.NvPageToErase].NvLastMetaInfoAddress = gEmptyPageMetaAddress_c;
                mNvErasePgCmdStatus.NvErasePending = FALSE;
                #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                FSCI_MsgNVPageEraseMonitoring(mNvVirtualPageProperty[mNvErasePgCmdStatus.NvPageToErase].NvRawSectorStartAddress, gNVM_OK_c);
                #endif
                ret = TRUE;
            }
            else
            {
                /* erase */
                (void)HAL_FlashEraseSector(mNvErasePgCmdStatus.NvSectorAddress, (uint32_t) ((uint8_t*) NV_STORAGE_SECTOR_SIZE));

                /* blank check */
                if(kStatus_HAL_Flash_Success == HAL_FlashVerifyErase (
                                                                 mNvErasePgCmdStatus.NvSectorAddress,
                                                                (uint32_t)NV_STORAGE_SECTOR_SIZE,
                                                                 kHAL_Flash_MarginValueNormal
                ))

                {
                    mNvErasePgCmdStatus.NvSectorAddress += (uint32_t)((uint8_t*)NV_STORAGE_SECTOR_SIZE);
                    ret = TRUE;
                }
            }
        }
        if(FALSE == ret)
        #endif
        {
            /* process the save-on-interval requests */
            if(mNvSaveOnIntervalEvent)
            {
                #if gNvmUseTimerJitter_d
                timerJitter = GetRandomRange(0,100);
               #endif
               currentTimestampValue = TM_GetTimestamp();
               #if gNvmUseTimerJitter_d
               if(currentTimestampValue - mNvLastTimestampValue >= (uint64_t)((gNvOneSecondInMicros_c + ((uint64_t)timerJitter * (uint64_t)gNvJitterMultiplier_c)) - (gNvJitterDecrement_c)))
               #else
               if(currentTimestampValue - mNvLastTimestampValue >= (uint64_t)gNvOneSecondInMicros_c)
               #endif
                {
                    mNvSaveOnIntervalEvent = __NvTimerTick(TRUE);
                    mNvLastTimestampValue = currentTimestampValue;
                }
            }

            /* process the save-on-idle requests */
            if(NvGetPendingSavesCount(&mNvPendingSavesQueue) != 0u)
            {
                while(NvPopPendingSave(&mNvPendingSavesQueue, &tblIdx))
                {
                    if ((gNvCopyAll_c == tblIdx.entryId) && (gNvCopyAll_c == tblIdx.elementIndex) && (TRUE == tblIdx.saveRestoreAll))
                    {
                        (void)__NvAtomicSave();
                        continue;
                    }
                    else if (gNvInvalidDataEntry_c == tblIdx.entryId)
                    {
                        continue;
                    }
                    else
                    {
                        /*MISRA rule 15.7*/
                    }

                    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
                    if(NvWriteRecord(&tblIdx) == gNVM_PageCopyPending_c)
                    {
                        (void)NvAddSaveRequestToQueue(&tblIdx);
                        break;
                    }
                    #else /* FlexNVM */
                    NvWriteRecord(&tblIdx);
                    #endif
                }
            }
        }
    }
}
/******************************************************************************
 * Name: __NvIsDataSetDirty
 * Description: return TRUE if the element pointed by ptrData is dirty
 * Parameters: [IN] ptrData - pointer to data to be checked
 * Return: TRUE if the element is dirty, FALSE otherwise
 ******************************************************************************/
bool_t __NvIsDataSetDirty
(
    void* ptrData
)
{
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIdx;
    uint16_t loopIdx;
    uint16_t remaining_count;
    bool_t ret = FALSE;

    if(mNvModuleInitialized)
    {
        if(NULL != ptrData)
        {

            if(gNVM_OK_c == NvGetTableEntryIndexFromDataPtr(ptrData, &tblIdx, &tableEntryIdx))
            {
                /* Check if is in pendding queue */
                if (mNvPendingSavesQueue.EntriesCount != 0u)
                {
                    /* Start from the queue's head */
                    loopIdx = mNvPendingSavesQueue.Head;
                    remaining_count = mNvPendingSavesQueue.EntriesCount;

                    while(remaining_count != 0u)
                    {
                        if(mNvPendingSavesQueue.QData[loopIdx].entryId == tblIdx.entryId)
                        {
                            ret = TRUE;
                            break;
                        }
                        remaining_count--;
                        /* increment and wrap the loop index */
                        if(++loopIdx >= (uint8_t)gNvPendingSavesQueueSize_c)
                        {
                            loopIdx=0;
                        }
                    }
                }
                if(FALSE == ret)
                {
                    ret = maDatasetInfo[tableEntryIdx].saveNextInterval;
                }
            }
        }
    }
    return ret;
}
/******************************************************************************
 * Name: __NvRestoreDataSet
 * Description: copy the most recent version of the element/table entry pointed
 *              by ptrData from NVM storage system to RAM memory
 * Parameter(s): [IN] ptrData - pointer to data (element) to be restored
 *               [IN] restoreAll - if FALSE restores a single element
 *                               - if TRUE restores an entire table entry
 * Return: status of the restore operation
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvRestoreDataSet
(
void* ptrData,
bool_t restoreAll
)
{
    NVM_TableEntryInfo_t tblIdx;
    #if gUnmirroredFeatureSet_d
    uint16_t tableEntryIdx;
    #endif
    NVM_Status_t nvmStatus;

    if(!mNvModuleInitialized)
    {
        #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
        FSCI_MsgNVRestoreMonitoring(0, TRUE, (uint8_t)gNVM_ModuleNotInitialized_c);
        #endif
        nvmStatus = gNVM_ModuleNotInitialized_c;
    }
    else
    {
        if(NULL == ptrData)
        {
            #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
            FSCI_MsgNVRestoreMonitoring(0, TRUE, (uint8_t)gNVM_NullPointer_c);
            #endif
            nvmStatus = gNVM_NullPointer_c;
        }
        else
        {
            #if gNvFragmentation_Enabled_d
            tblIdx.saveRestoreAll = restoreAll;
            #else
            tblIdx.saveRestoreAll = TRUE;
            #endif /* gNvFragmentation_Enabled_d */
            /* get table and element indexes */
            if(gNVM_OK_c != NvGetEntryFromDataPtr(ptrData, &tblIdx))
            {
               #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                FSCI_MsgNVRestoreMonitoring(tblIdx.entryId, TRUE, (uint8_t)gNVM_PointerOutOfRange_c);
               #endif
                nvmStatus = gNVM_PointerOutOfRange_c;
            }
            else
            {
                #if gUnmirroredFeatureSet_d
                /*make sure you can't request a full backup for unmirrored data sets*/
                tableEntryIdx = NvGetTableEntryIndexFromId(tblIdx.entryId);

                assert(gNvInvalidTableEntryIndex_c != tableEntryIdx);

                if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType)
                {
                    tblIdx.saveRestoreAll = FALSE;
                }

                #endif

                /* Do Nv Restore Data */
                #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                {
                    FSCI_MsgNVRestoreMonitoring(tblIdx.entryId, TRUE, (uint8_t)gNVM_OK_c);
                    nvmStatus=NvRestoreData(&tblIdx);
                    FSCI_MsgNVRestoreMonitoring(tblIdx.entryId, FALSE, (uint8_t)nvmStatus);
                }
                #else
                nvmStatus = NvRestoreData(&tblIdx);
                #endif
            }
        }
    }
    return nvmStatus;
}

/******************************************************************************
 * Name: __NvTimerTick
 * Description: Called from the idle task to process save-on-interval requests
 * Parameters: [IN] countTick - enable/disable tick count
 * Return: FALSE if the timer tick counters for all data sets have reached
 *         zero. In this case, the timer can be turned off.
 *         TRUE if any of the data sets' timer tick counters have not yet
 *         counted down to zero. In this case, the timer should be active
 ******************************************************************************/
NVM_STATIC bool_t __NvTimerTick
(
    bool_t countTick
)
{
    bool_t fTicksLeft = FALSE;
    NVM_TableEntryInfo_t tblIdx;
    uint16_t idx = 0;

    while(idx < gNVM_TABLE_entries_c)
    {
        if(countTick)
        {
            if(maDatasetInfo[idx].ticksToNextSave != 0u)
            {
                --maDatasetInfo[idx].ticksToNextSave;
            }
        }

        if(maDatasetInfo[idx].saveNextInterval)
        {
            if (maDatasetInfo[idx].ticksToNextSave != 0u)
            {
                fTicksLeft = TRUE;
            }
            else
            {
                tblIdx.entryId = pNVM_DataTable[idx].DataEntryID;
                #if gUnmirroredFeatureSet_d
                if (gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[idx].DataEntryType)
                {
                    tblIdx.elementIndex = maDatasetInfo[idx].elementIndex;
                    tblIdx.saveRestoreAll = FALSE;
                }
                else
                #endif
                {
                    tblIdx.elementIndex = 0;
                    tblIdx.saveRestoreAll = TRUE;
                }
                maDatasetInfo[idx].saveNextInterval = FALSE;
                if(mNvCriticalSectionFlag == 0u)
                {
                    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
                    if(NvWriteRecord(&tblIdx) == gNVM_PageCopyPending_c)
                    {
                        /* retry next time we have a tick */
                        if (NvAddSaveRequestToQueue(&tblIdx) == gNVM_SaveRequestRejected_c)
                        {
                            maDatasetInfo[idx].saveNextInterval = TRUE;
                    }
                    }
                    #else /* FlexNVM */
                    NvWriteRecord(&tblIdx);
                    #endif
                }
                else
                {
                    /* retry next time we have a tick */
                    if (NvAddSaveRequestToQueue(&tblIdx) == gNVM_SaveRequestRejected_c)
                    {
                        maDatasetInfo[idx].saveNextInterval = TRUE;
                    }
                }
            }
        }

        /* increment the loop counter */
        idx++;
    }

    return fTicksLeft;
}/* NvTimerTick() */

/******************************************************************************
 * Name: __NvSaveOnCount
 * Description: Decrement the counter. Once it reaches 0, the next call to
 *              NvIdle() will save the entire table entry (all elements).
 * Parameters: [IN] ptrData - pointer to data to be saved
 * Return: NVM_OK_c - if operation completed successfully
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_STATIC NVM_Status_t __NvSaveOnCount
(
    void* ptrData
)
{
    NVM_Status_t status;
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIdx;

    if(!mNvModuleInitialized)
    {
        status = gNVM_ModuleNotInitialized_c;
    }
    else
    {
        if(NULL == ptrData)
        {
            status = gNVM_NullPointer_c;
        }
        else
        {
            /* get the NVM table entry */
            status = NvGetTableEntryIndexFromDataPtr(ptrData, &tblIdx, &tableEntryIdx);
            if(gNVM_OK_c == status)
            {
                if(maDatasetInfo[tableEntryIdx].countsToNextSave != 0u)
                {
                    --maDatasetInfo[tableEntryIdx].countsToNextSave;
                }
                else
                {
                    /* all the elements of the NVM table entry will be saved */
                    tblIdx.saveRestoreAll = TRUE;
                    maDatasetInfo[tableEntryIdx].countsToNextSave = gNvCountsBetweenSaves;
                    status = NvAddSaveRequestToQueue(&tblIdx);
                }
            }
        }
    }
    return status;
}                                       /* NvSaveOnCount() */

/******************************************************************************
 * Name: __NvSaveOnInterval
 * Description:  save no more often than a given time interval. If it has
 *               been at least that long since the last save,
 *               this function will cause a save the next time the idle
 *               task runs.
 * Parameters: [IN] ptrData - pointer to data to be saved
 * NOTE: this function saves all the element of the table entry pointed by
 *       ptrData
 * Return: NVM_OK_c - if operation completed successfully
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_STATIC NVM_Status_t __NvSaveOnInterval
(
    void* ptrData
)
{
    NVM_Status_t status;
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIdx;

    if(!mNvModuleInitialized)
    {
        status = gNVM_ModuleNotInitialized_c;
    }
    else
    {
        if(NULL == ptrData)
        {
            status = gNVM_NullPointer_c;
        }
        else
        {
            /* get the NVM table entry */
            status = NvGetTableEntryIndexFromDataPtr(ptrData, &tblIdx, &tableEntryIdx);
            if(status == gNVM_OK_c)
            {
                if(maDatasetInfo[tableEntryIdx].saveNextInterval == FALSE)
                {
                    maDatasetInfo[tableEntryIdx].ticksToNextSave = gNvMinimumTicksBetweenSaves;
                    maDatasetInfo[tableEntryIdx].saveNextInterval = TRUE;
                    #if gUnmirroredFeatureSet_d
                    if (gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType)
                    {
                        maDatasetInfo[tableEntryIdx].elementIndex = tblIdx.elementIndex;
                    }
                    #endif
                    mNvSaveOnIntervalEvent = TRUE;
                    mNvLastTimestampValue = TM_GetTimestamp();
                }
            }
        }
    }
    return status;
}

/******************************************************************************
 * Name: __NvSaveOnIdle
 * Description: Save the data pointed by ptrData on the next call to NvIdle()
 * Parameter(s): [IN] ptrData - pointer to data to be saved
 *               [IN] saveRestoreAll - specify if all the elements from the NVM table
 *                              entry shall be saved
 * Return: gNVM_OK_c - if operation completed successfully
 *         gNVM_Error_c - in case of error(s)
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_STATIC NVM_Status_t __NvSaveOnIdle
(
    void* ptrData,
    bool_t saveAll
)
{
    NVM_Status_t status;
    NVM_TableEntryInfo_t tblIdx;

    if(!mNvModuleInitialized)
    {
        status = gNVM_ModuleNotInitialized_c;
    }
    else
    {
        if(NULL == ptrData)
        {
            status = gNVM_NullPointer_c;
        }
        else
        {
            /* get the NVM table entry */
            status = NvGetTableEntryIndexFromDataPtr(ptrData, &tblIdx, NULL);
            if(status == gNVM_OK_c)
            {
                /* write the save all flag */
                #if gNvFragmentation_Enabled_d
                tblIdx.saveRestoreAll = saveAll;
                #else
                tblIdx.saveRestoreAll = TRUE;
                #endif /* gNvFragmentation_Enabled_d */

                status = NvAddSaveRequestToQueue(&tblIdx);
            }
        }
    }
    return status;
}

#if (gNvUseFlexNVM_d == FALSE)
/******************************************************************************
 * Name: __NvModulePostInit
 * Description: Do post Initialize the NV storage module when format failure
 * Parameter(s):
 * Return: gNVM_ModuleAlreadyInitialized_c - if the module is already
 *                                           initialized
 *         gNVM_InvalidSectorsCount_c - if the sector count configured in the
 *                                      project linker file is invalid
 *         gNVM_MetaNotFound_c - if no meta information was found
 *         gNVM_OK_c - module was successfully initialized
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvModulePostInit
(
    void
)
{
    NVM_Status_t status = gNVM_OK_c;
    uint32_t pageFreeSpace;
    /* get the last meta information address */
    status = NvUpdateLastMetaInfoAddress();
    if(gNVM_OK_c == status)
    {
        /* NVM module is now initialized */
        mNvModuleInitialized = TRUE;

        /* get active page free space */
        status = NvGetPageFreeSpace(&pageFreeSpace);
        if(gNVM_OK_c == status)
        {
            if(pageFreeSpace < gNvMinimumFreeBytesCountStart_c )
            {
#if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
                status = NvCopyPage(gNvCopyAll_c);
                FSCI_MsgNVVirtualPageMonitoring(FALSE,status);
#else
                status = NvCopyPage(gNvCopyAll_c);
#endif
            }
#if gUnmirroredFeatureSet_d
            __NvmRestoreUnmirrored();
#endif
        }
        else
        {
            /* format the system */
            status = NvInternalFormat(0);
        }
    }
    return status;
}
#endif

/******************************************************************************
 * Name: __NvModuleInit
 * Description: Initialize the NV storage module
 * Parameter(s): [IN] flashInit - need to Initialize flash adapter
 * Return: gNVM_ModuleAlreadyInitialized_c - if the module is already
 *                                           initialized
 *         gNVM_InvalidSectorsCount_c - if the sector count configured in the
 *                                      project linker file is invalid
 *         gNVM_MetaNotFound_c - if no meta information was found
 *         gNVM_OK_c - module was successfully initialized
 *****************************************************************************/
#if defined ( __IAR_SYSTEMS_ICC__ )
#pragma required=eraseNVMFirst
#pragma required=eraseNVMSecond
#endif /* __IAR_SYSTEMS_ICC__ */
NVM_STATIC NVM_Status_t __NvModuleInit
(
    bool_t flashInit
)
{
    uint16_t loopCnt;
    NVM_Status_t status = gNVM_OK_c;
    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    uint32_t flashEstimateSize = 0U;
    uint32_t pageFreeSpace = 0;
    uint32_t tableEndAddr = (uint32_t)gNVM_TABLE_endAddr_c;
    uint32_t tableStartAddr = (uint32_t)gNVM_TABLE_startAddr_c;
    #if gNvUseExtendedFeatureSet_d
    bool_t ret = FALSE;
    #endif
    #else  /* no FlexNVM */
    ftfx_security_state_t securityStatus;
    status_t result;
    uint32_t eepromTotalSize;
    #endif /* no FlexNVM */

    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    #if defined ( __IAR_SYSTEMS_ICC__ )
    (void)eraseNVMFirst;
    (void)eraseNVMSecond;
    #endif
    #endif

    if((gNVM_TABLE_entries_c == 0U) || (gNVM_TABLE_entries_c >= gNvTableEntriesCountMax_c))
    {
        status = gNVM_InvalidTableEntriesCount_c;
    }
    else
    {
        #if ((gNvUseFlexNVM_d == FALSE) && (gNvFragmentation_Enabled_d == TRUE))
        for(loopCnt = 0; loopCnt < (uint16_t)((tableEndAddr - tableStartAddr)/sizeof(NVM_DataEntry_t)); loopCnt++)
        {
            if (pNVM_DataTable[loopCnt].ElementsCount > gNvRecordsCopiedBufferSize_c)
            {
                status = gNVM_DefragBufferTooSmall_c;
                break;
            }
        }
        if(gNVM_OK_c == status)
        #endif
        {
            /* Initialize the pending saves queue */
            (void)NvInitPendingSavesQueue(&mNvPendingSavesQueue);

            /* Initialize the data set info table */
            for(loopCnt = 0; loopCnt < (uint16_t)gNvTableEntriesCountMax_c; loopCnt++)
            {
                maDatasetInfo[loopCnt].saveNextInterval = FALSE;
                maDatasetInfo[loopCnt].countsToNextSave = gNvCountsBetweenSaves;
            }

            /* initialize the event used by save-on-interval functionality */
            mNvSaveOnIntervalEvent = FALSE;

            #if (gNvUseFlexNVM_d == TRUE) /* FlexNVM */

            /* init the FLASH driver */
            if (flashInit)
            {
                NV_Init();
            }

            #if FSL_FEATURE_FLASH_IS_FTFL && (DEBLOCK_SIZE != 0)
            /* check if the flash registers are configured to support FlexNVM as EEPROM,
            if not enable this feature*/
            if (0 == gFlexNvmDriver.ftfxConfig.flexramTotalSize)
            {
                DEFlashPartition(&gFlexNvmDriver, gNvEepromDatasetSizeCode_c, gNvEepromBackupSizeCode_c, gFlashLaunchCommand);
            }
            /* check if the NVM table fits within the size of the FlexRAM window */
            if(gNVM_OK_c != NvCheckNvmTableForFlexRAMUsage())
            {
                status = gNVM_NvTableExceedFlexRAMSize_c;
            }
            else
            {
                /* check data flash IFR map */
                if(gFlexNvmDriver.ftfxConfig.flexramTotalSize == 0)
                {
                    status = gNVM_NvWrongFlashDataIFRMap_c;
                }
                else
                {
                    /* Enable the EERAM */
                    SetEEEEnable(&gFlexNvmDriver, EEE_ENABLE, FlashCommandSequence);

                    /* NVM module is now initialized */
                    mNvModuleInitialized = TRUE;
                }
            }
            #endif /* FSL_FEATURE_FLASH_IS_FTFL */

            #if FSL_FEATURE_FLASH_IS_FTFE
            securityStatus = kFTFx_SecurityStateNotSecure;

            /* Check security status. */
            result = FLEXNVM_GetSecurityState(&gFlexNvmDriver, &securityStatus);
            if (kStatus_FLASH_Success != result)
            {
                  status = gNVM_Error_c;
            }
            else
            {
                  if (kFTFx_SecurityStateNotSecure != securityStatus)
                  {
                      status = gNVM_FlashMemoryIsSecured_c;
                  }
                  else
                  {
                      /* Get total EEPROM size */
                      FLEXNVM_GetProperty(&gFlexNvmDriver, kFLEXNVM_PropertyEepromTotalSize, &eepromTotalSize);

                      /* If no EEPROM set, partition the device as follows:
                            -> DFLASH size = 0,
                            -> EEPROM backup size = 256KB,
                            -> EEPROM dataset size = 2KB,
                            -> FLEXRAM = 8KB
                      */
                      if(!eepromTotalSize)
                      {
                          uint32_t eepromDataSizeCode = gEEPROM_DATA_SET_SIZE_CODE_c;
                          uint32_t flexnvmPartitionCode = gFLEXNVM_PARTITION_CODE_c;

                          /* Disable interrupts */
                          OSA_InterruptDisable();
                          /* Try to configure FlexNVM block as EEprom */
                          result = FLEXNVM_ProgramPartition(&gFlexNvmDriver, kFTFx_PartitionFlexramLoadOptLoadedWithValidEepromData,
                                                          eepromDataSizeCode, flexnvmPartitionCode);
                          /* Enable interrupts */
                          OSA_InterruptEnable();

                          if (kStatus_FLASH_Success != result)
                          {
                              status = gNVM_FlexNVMPartitioningFail_c;
                          }
                          else
                          {
                              /* Reset the MCU so that the changes become effective */
                              ResetMCU();
                          }
                      }

                      if(gNVM_OK_c == status)
                      {
                          /* check if the NVM table fits within the size of the FlexRAM window */
                          if(gNVM_OK_c != NvCheckNvmTableForFlexRAMUsage())
                          {
                              status = gNVM_NvTableExceedFlexRAMSize_c;
                          }
                          else
                          {
                              /* Enable the EERAM */
                              FLEXNVM_SetFlexramFunction(&gFlexNvmDriver, kFTFx_FlexramFuncOptAvailableForEeprom);

                              /* NVM module is now initialized */
                              mNvModuleInitialized = TRUE;
                          }
                      }
                  }
            }
            #endif /* FSL_FEATURE_FLASH_IS_FTFE */

            #else /* no FlexNVM */

            /* check linker file symbol definition for sector count; it should be multiple of 2 */
            if ((((uint32_t)NV_STORAGE_MAX_SECTORS) & 0x1u) != 0u)
            {
                status = gNVM_InvalidSectorsCount_c;
            }
            else
            {
                /* Init the NVM configuration */
                InitNVMConfig();

                /* both pages are not valid, format the NV storage system */
                if (mNvActivePageId == gVirtualPageNone_c)
                {
                    mNvActivePageId = gFirstVirtualPage_c;
                    status = NvInternalFormat(0);
                    (void)NvGetPageFreeSpace(&pageFreeSpace);
                    for (loopCnt = 0; loopCnt < (uint16_t)((tableEndAddr - tableStartAddr)/sizeof(NVM_DataEntry_t)); loopCnt++)
                    {
                        flashEstimateSize +=  (NvUpdateSize(pNVM_DataTable[loopCnt].ElementSize) + sizeof(NVM_RecordMetaInfo_t)) * pNVM_DataTable[loopCnt].ElementsCount;
                    }
                    if (pageFreeSpace < (flashEstimateSize + gNvMinimumFreeBytesCountStart_c))
                    {
                        assert(0);
                        /* Estimated Flash buffer is too small, Need increase the gNVMSectorCountLink_d */
                        status =  gNVM_ReservedFlashTooSmall_c;
                    }
                }
                if(gNVM_OK_c == status)
                {
                    #if gNvUseExtendedFeatureSet_d
                    /* get the size of the NV table stored in FLASH memory */
                    mNvTableSizeInFlash = NvGetFlashTableSize();

                    if(0u == mNvTableSizeInFlash) /* no NV table found in FLASH, format the system */
                    {
                        status = NvInternalFormat(0); /* will also save the NV table to FLASH memory */
                        if (status != gNVM_OK_c)
                        {
                            ret = TRUE;
                        }
                    }
                    else /* found an valid NV table in FLASH memory */
                    {
                        /* check if the RAM table was updated (e.g. new binary image via OTA) */
                        mNvTableUpdated = (GetFlashTableVersion() != mNvFlashTableVersion) || NvIsRamTableUpdated();
                        if( mNvTableUpdated )
                        {
                            ret = TRUE;
                            if(gNVM_OK_c == NvUpdateLastMetaInfoAddress())
                            {
                                /* copy the new RAM table and the page content */
                                #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                                FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
                                status = NvCopyPage(gNvCopyAll_c);
                                FSCI_MsgNVVirtualPageMonitoring(FALSE,status);
                                #else
                                status = NvCopyPage(gNvCopyAll_c);
                                #endif

                                /* NVM module is now initialised */
                                mNvModuleInitialized = TRUE;
                                mNvTableUpdated = FALSE;

                                #if gUnmirroredFeatureSet_d
                                __NvmRestoreUnmirrored();
                                #endif
                            }
                            else
                            {
                                /* format the system */
                                status = NvInternalFormat(0);
                            }
                        }
                    }
                    if(FALSE == ret)
                    #endif /* gNvUseExtendedFeatureSet_d */
                    {
                        status = __NvModulePostInit();
                    }
                }
            }
            #endif
        }
    }
    return status;
}

/******************************************************************************
 * Name: NvModuleReInit
 * Description: Reinit the NV module , reload from Flash to RAM the latest NVM changes
 *      Usefull for RAM off use case in lowpower
 *    Same than __NvModuleInit without call to NV_Init(); and no mNvModuleInitialized protection
 *  TODO : merge with __NvModuleInit some common processing
 *****************************************************************************/
NVM_Status_t NvModuleReInit
(
    void
)
{
    return __NvModuleInit(FALSE);
}

#if gUnmirroredFeatureSet_d

/******************************************************************************
 * Name: NvIsRecordErased
 * Description: Checks the most recent metas to see if the unmirrored element
 *              was erased or is just uninitialised
 * Parameter(s): [IN] srcTableEntryIdx - the index of the entry to be checked
 *               [IN] srcTableEntryElementIdx - the element index
 *               [IN] srcMetaAddress - the starting address of the search
 * Return: TRUE if the element was erased with NvErase or FALSE otherwise
 *****************************************************************************/
NVM_STATIC bool_t NvIsRecordErased
(
    uint16_t srcTableEntryIdx,
    uint16_t srcTableEntryElementIdx,
    uint32_t srcMetaAddress
)
{
    bool_t status = FALSE;
    NVM_RecordMetaInfo_t srcMetaInfo = {0};
    while(srcMetaAddress < (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress))
    {
        (void)NvGetMetaInfo(mNvActivePageId, srcMetaAddress, &srcMetaInfo);
        if (gNvGuardValue_c == srcMetaInfo.rawValue)
        {
            break;
        }
        if ((srcMetaInfo.fields.NvmRecordOffset == 0u) &&
            (srcMetaInfo.fields.NvmElementIndex == srcTableEntryElementIdx) &&
            (srcMetaInfo.fields.NvmDataEntryID == pNVM_DataTable[srcTableEntryIdx].DataEntryID))
        {
            status = TRUE;
            break;
        }
        srcMetaAddress += sizeof(NVM_RecordMetaInfo_t);
    }
    return status;
}

/******************************************************************************
 * Name: __NvmRestoreUnmirrored
 * Description: Restores all unmirrored entries that should be restored at init
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
NVM_STATIC void __NvmRestoreUnmirrored
(
    void
)
{
    uint32_t metaInfoAddress;
    uint16_t tableEntryIdx;
    NVM_RecordMetaInfo_t metaInfo = {0};
    uint16_t loopCnt = 0;
    uint16_t loopCnt2;
    const uint32_t erasedElement = 0xFFFFFFFFU;

    /* get the last meta information address */
    metaInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress;
    if(metaInfoAddress != gEmptyPageMetaAddress_c)
    {
        /* parse meta info backwards until the element is found */
        while(metaInfoAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
        {
            /* get the meta information */
            (void)NvGetMetaInfo(mNvActivePageId, metaInfoAddress, &metaInfo);
            if ((metaInfo.fields.NvValidationStartByte != metaInfo.fields.NvValidationEndByte) ||
                (metaInfo.fields.NvValidationStartByte != gValidationByteSingleRecord_c))
            {
                metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
                continue;
            }

            /* get table entry information */
            tableEntryIdx = NvGetTableEntryIndexFromId(metaInfo.fields.NvmDataEntryID);
            if ((gNvInvalidTableEntryIndex_c == tableEntryIdx) ||
                (gNVM_NotMirroredInRamAutoRestore_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType))
            {
                metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
                continue;
            }

            /* if it was allready restored, ignore it */
            if (NvIsNVMFlashAddress(((void**)pNVM_DataTable[tableEntryIdx].pData)[metaInfo.fields.NvmElementIndex]) ||
                (erasedElement == (uint32_t)(uint32_t *)((void**)pNVM_DataTable[tableEntryIdx].pData)[metaInfo.fields.NvmElementIndex]))
            {
                metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
                continue;
            }

            /* erased element */
            if (metaInfo.fields.NvmRecordOffset == 0u)
            {
                ((void**)pNVM_DataTable[tableEntryIdx].pData)[metaInfo.fields.NvmElementIndex] = (uint32_t*)erasedElement;
            }
            else
            {
                ((void**)pNVM_DataTable[tableEntryIdx].pData)[metaInfo.fields.NvmElementIndex] =
                    (void*)((uint32_t *)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset));
            }

            /* move to the previous meta info */
            metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
        }

        while(loopCnt < gNVM_TABLE_entries_c)
        {
            if(gNVM_NotMirroredInRamAutoRestore_c == (NVM_DataEntryType_t)pNVM_DataTable[loopCnt].DataEntryType)
            {
                for (loopCnt2=0; loopCnt2<pNVM_DataTable[loopCnt].ElementsCount; loopCnt2++)
                {
                    if (erasedElement == (uint32_t)(uint32_t *)((void**)pNVM_DataTable[loopCnt].pData)[loopCnt2])
                    {
                        ((void**)pNVM_DataTable[loopCnt].pData)[loopCnt2] = NULL;
                    }
                }
            }
            /* increment the loop counter */
            loopCnt++;
        }
    }
}
/******************************************************************************
 * Name: __NvmMoveToRam
 * Description: Move from NVM to Ram an unmirrored dataset
 * Parameter(s):  ppData     double pointer to the entity to be moved from flash to RAM
 * Return: pointer to Ram location
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvmMoveToRam
(
    void** ppData
)
{
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIndex;
    NVM_Status_t status = gNVM_OK_c;
    void* pData=NULL;
    uint16_t loopIdx;
    uint16_t remaining_count;

    /* Get entry from NVM table */
    status = NvGetTableEntryIndexFromDataPtr(ppData, &tblIdx, &tableEntryIndex);
    if(gNVM_OK_c == status)
    {
        if(gNVM_MirroredInRam_c == (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIndex].DataEntryType)
        {
            status = gNVM_IsMirroredDataSet_c;
        }
        else
        {
            /* Check if entry is in ram  */
            if(!NvIsNVMFlashAddress(*ppData)&&(*ppData != NULL))
            {
                /* Check if is in pendding queue - if yes than remove it */
                if (NvGetPendingSavesCount(&mNvPendingSavesQueue) != 0u)
                {
                    /* Start from the queue's head */
                    loopIdx = mNvPendingSavesQueue.Head;
                    remaining_count = mNvPendingSavesQueue.EntriesCount;

                    while(remaining_count != 0u)
                    {
                        if((tblIdx.entryId == mNvPendingSavesQueue.QData[loopIdx].entryId)&&
                           (tblIdx.elementIndex == mNvPendingSavesQueue.QData[loopIdx].elementIndex))
                        {
                            mNvPendingSavesQueue.QData[loopIdx].entryId = gNvInvalidDataEntry_c;
                            break;
                        }
                        remaining_count--;
                        /* increment and wrap the loop index */
                        if(++loopIdx >= (uint8_t)gNvPendingSavesQueueSize_c)
                        {
                            loopIdx=0;
                        }
                    }
                }
                maDatasetInfo[tableEntryIndex].saveNextInterval = FALSE;
                status = gNVM_OK_c;
            }
            else
            {
                /* Allocate a buffer for the data set */
                pData = MEM_BufferAllocWithId(pNVM_DataTable[tableEntryIndex].ElementSize , gNvmMemPoolId_c);
                if(pData == NULL)
                {
                    status = gNVM_NoMemory_c;
                }
                else
                {
                    /* Write from Flash to Ram */
                    if(*ppData != NULL)
                    {
                        FLib_MemCpy(pData, *ppData, pNVM_DataTable[tableEntryIndex].ElementSize);
                    }

                    OSA_InterruptDisable();
                    *ppData = pData;
                    OSA_InterruptEnable();
                    /* Check if the address is in ram */
                    status = gNVM_OK_c;
                }
            }
        }
    }
    return status;
}

/******************************************************************************
 * Name: __NvmErase
 * Description: Erase from NVM an unmirrored dataset
 * Parameter(s):  ppData     double pointer to the entity to be erased
 * Return: pointer to Ram location
 *****************************************************************************/
NVM_STATIC NVM_Status_t __NvmErase
(
    void** ppData
)
{
    NVM_Status_t status;
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIndex;
    uint16_t loopCnt;
    uint16_t remaining_count;

    /* Get entry from NVM table */
    status = NvGetTableEntryIndexFromDataPtr(ppData, &tblIdx, &tableEntryIndex);
    if(gNVM_OK_c == status)
    {
        if(gNVM_MirroredInRam_c == (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIndex].DataEntryType)
        {
            status = gNVM_IsMirroredDataSet_c;
        }
        else
        {
            if(!NvIsNVMFlashAddress(*ppData))
            {
                if(*ppData != NULL)
                {
                    (void)MEM_BufferFree(*ppData);
                }
                OSA_InterruptDisable();
                *ppData = NULL;
                OSA_InterruptEnable();
                status = gNVM_OK_c;
            }
            else
            {
                /* Check if is in pending queue - if yes than remove it */
                if (NvGetPendingSavesCount(&mNvPendingSavesQueue) != 0u)
                {
                    /* Start from the queue's head */
                    loopCnt = mNvPendingSavesQueue.Head;
                    remaining_count = mNvPendingSavesQueue.EntriesCount;
                    while (remaining_count != 0u)
                    {
                        /* if the element is waiting to be saved, cancel the save */
                        if ((tblIdx.entryId == mNvPendingSavesQueue.QData[loopCnt].entryId) && (tblIdx.elementIndex == mNvPendingSavesQueue.QData[loopCnt].elementIndex))
                        {
                            mNvPendingSavesQueue.QData[loopCnt].entryId = gNvInvalidDataEntry_c;
                        }
                        remaining_count--;
                        /* increment and wrap the loop index */
                        if(++loopCnt >= (uint8_t)gNvPendingSavesQueueSize_c)
                        {
                            loopCnt=0;
                        }
                    }
                }
                OSA_InterruptDisable();
                *ppData = NULL;
                OSA_InterruptEnable();
                status = __NvSyncSave(ppData, FALSE);
            }
        }
    }
    return status;
}

/******************************************************************************
 * Name: NvIsNVMFlashAddress
 * Description: check if the address is in Flash
 * Parameter(s): [IN] address
 *
 * Return: TRUE if the table entry is in Flash / FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvIsNVMFlashAddress
(
    void* address
)
{
    bool_t status = FALSE;
    uint8_t idx;
    for(idx=0; idx < gNvVirtualPagesCount_c; idx ++)
    {
        if( ((uint32_t)((uint32_t *)address) > mNvVirtualPageProperty[idx].NvRawSectorStartAddress) &&
           ((uint32_t)((uint32_t *)address) < mNvVirtualPageProperty[idx].NvRawSectorEndAddress))
        {
            status = TRUE;
            break;
        }
    }
    return status;
}
#endif

/******************************************************************************
 * Name: NvInitPendingSavesQueue
 * Description: Initialize the pending saves queue
 * Parameters: [IN] pQueue - pointer to queue
 * Return: TRUE if the pointer is valid, FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvInitPendingSavesQueue
(
    NVM_SaveQueue_t *pQueue
)
{
    bool_t status = FALSE;
    if(NULL != pQueue)
    {
        pQueue->Head = 0;
        pQueue->Tail = 0;
        pQueue->EntriesCount = 0;
        status = TRUE;
    }
    return status;
}

/******************************************************************************
 * Name: NvPushPendingSave
 * Description: Add a new pending save to the queue
 * Parameters: [IN] pQueue - pointer to queue
 *             [IN] data - data to be saved
 * Return: TRUE if the push operation succeeded, FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvPushPendingSave
(
    NVM_SaveQueue_t *pQueue,
    NVM_TableEntryInfo_t data
)
{
    bool_t status = FALSE;
    bool_t ret = FALSE;
    if(NULL != pQueue)
    {
        if((pQueue->Tail == pQueue->Head) && (pQueue->EntriesCount > 0u))
        {
            #if gFifoOverwriteEnabled_c
            /* Increment and wrap the head when it reaches gNvPendingSavesQueueSize_c */
            if(++pQueue->Head >= (uint16_t)gNvPendingSavesQueueSize_c)
            {
                pQueue->Head=0;
            }
            (void)ret;
            #else
            ret = TRUE;
            #endif
        }
        if(FALSE == ret)
        {
            /* Add the item to queue */
            pQueue->QData[pQueue->Tail] = data;

            /* Increment and wrap the tail when it reaches gNvPendingSavesQueueSize_c */
            if(++pQueue->Tail >= (uint16_t)gNvPendingSavesQueueSize_c)
            {
                pQueue->Tail=0;
            }

            /* Increment the entries count */
            if(pQueue->EntriesCount < (uint16_t)(gNvPendingSavesQueueSize_c))
            {
                pQueue->EntriesCount++;
            }

            status = TRUE;
        }
    }
    return status;
}

/******************************************************************************
 * Name: NvPopPendingSave
 * Description: Retrieves the head element from the pending saves queue
 * Parameters: [IN] pQueue - pointer to queue
 *             [OUT] pData - pointer to the location where data will be placed
 * Return: TRUE if the pop operation succeeded, FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvPopPendingSave
(
    NVM_SaveQueue_t *pQueue,
    NVM_TableEntryInfo_t *pData
)
{
    bool_t status = TRUE;
    if( (NULL == pQueue) || (pQueue->EntriesCount <= 0u) || ((NULL == pData)) )
    {
        status = FALSE;
    }
    else
    {
        *pData = pQueue->QData[pQueue->Head];

        /* Increment and wrap the head when it reaches gNvPendingSavesQueueSize_c */
        if(++pQueue->Head >= (uint16_t)gNvPendingSavesQueueSize_c)
        {
            pQueue->Head=0;
        }

        /* Decrement the entries count */
        pQueue->EntriesCount--;
    }
    return status;
}
/******************************************************************************
 * Name: NvGetPendingSavesCount
 * Description: self explanatory
 * Parameters: [IN] pQueue - pointer to queue
 * Return: Number of pending saves
 ******************************************************************************/
NVM_STATIC uint16_t NvGetPendingSavesCount
(
    NVM_SaveQueue_t *pQueue
)
{
    uint16_t status = 0U;
    if( NULL != pQueue )
    {
        status = pQueue->EntriesCount;
    }
    return status;
}

/*****************************************************************
 * The below functions are compiled only if FlexNVM is NOT used
 *****************************************************************/

#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */

/******************************************************************************
 * Name: InitNVMConfig
 * Description: Initialises the hal driver, and gets the active page.
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
NVM_STATIC void InitNVMConfig
(
    void
)
{
    if (FALSE == mNvFlashConfigInitialised)
    {
        /* Initialize flash HAL driver */
        if(kStatus_HAL_Flash_Success != HAL_FlashInit())
        {
            return;
        }

        /* no pending erase operations on system initialisation */
        mNvErasePgCmdStatus.NvErasePending = FALSE;

        /* Initialize the active page ID */
        mNvActivePageId = gVirtualPageNone_c;

        /* First virtual page initialisation */
        mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorStartAddress = (uint32_t)((uint8_t*)NV_STORAGE_START_ADDRESS);
        mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorsCount = (uint8_t)((uint32_t)((uint8_t*) NV_STORAGE_MAX_SECTORS) >> 1);
        mNvVirtualPageProperty[gFirstVirtualPage_c].NvTotalPageSize = mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorsCount *
            (uint32_t)((uint8_t*)NV_STORAGE_SECTOR_SIZE);
        mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorEndAddress = mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorStartAddress +
            mNvVirtualPageProperty[gFirstVirtualPage_c].NvTotalPageSize - 1u;

        /* Second virtual page initialisation */
        mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorStartAddress = mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorEndAddress + 1u;
        mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorsCount = (uint8_t)((uint32_t)((uint8_t*) NV_STORAGE_MAX_SECTORS) >> 1);
        mNvVirtualPageProperty[gSecondVirtualPage_c].NvTotalPageSize = mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorsCount *
            (uint32_t)((uint8_t*)NV_STORAGE_SECTOR_SIZE);
        mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorEndAddress = mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorStartAddress +
            mNvVirtualPageProperty[gSecondVirtualPage_c].NvTotalPageSize - 1u;

        /* Initialize the storage system: get active page and page counter */
        NvInitStorageSystem(FALSE);
        if (mNvActivePageId == gVirtualPageNone_c)
        {
            /* maybe the table is in old format and needs to be updated */
            NvInitStorageSystem(TRUE);
            if (mNvActivePageId != gVirtualPageNone_c)
            {
#if (!defined(gNvLegacyTable_Disabled_d) || (gNvLegacyTable_Disabled_d == 0))
                (void)UpgradeLegacyTable();
#endif
            }
        }
        #if gNvUseExtendedFeatureSet_d
        if (mNvActivePageId != gVirtualPageNone_c)
        {
            mNvTableSizeInFlash = NvGetFlashTableSize();
        }
        #endif
        mNvFlashConfigInitialised = TRUE;
    }
}

/******************************************************************************
 * Name: NvUpdateSize
 * Description: Updates the size to be a multiple of 4 or 8 depending on the flash controller
 * Parameter(s): [IN] size - size to be updated
 * Return: the computed size
 *****************************************************************************/
NVM_STATIC uint32_t NvUpdateSize
(
    uint32_t size
)
{
    /* compute the size that will be actually written on FLASH memory */
    uint8_t paddingBytes = (uint8_t)(size % (uint8_t)PGM_SIZE_BYTE);

    if(paddingBytes != 0u)
    {
        size += (uint8_t)((uint8_t)PGM_SIZE_BYTE-paddingBytes);
    }

    return size;
}

/******************************************************************************
 * Name: NvEraseVirtualPage
 * Description: erase the specified page
 * Parameter(s): [IN] pageID - the ID of the page to be erased
 * Return: gNVM_InvalidPageID_c - if the page ID is not valid
 *         gNVM_SectorEraseFail_c - if the page cannot be erased
 *         gNVM_OK_c - if operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvEraseVirtualPage
(
    NVM_VirtualPageID_t pageID
)
{
    NVM_Status_t status = gNVM_OK_c;

    if(pageID > gSecondVirtualPage_c)
    {
        status = gNVM_InvalidPageID_c;
    }
    else
    {
        /* erase virtual page */
        status = (NVM_Status_t)HAL_FlashEraseSector(mNvVirtualPageProperty[pageID].NvRawSectorStartAddress,
                                     mNvVirtualPageProperty[pageID].NvTotalPageSize);
        if(kStatus_HAL_Flash_Success != (hal_flash_status_t)status)
        {
            status = gNVM_SectorEraseFail_c;
        }
        else
        {
            status = NvVirtualPageBlankCheck(pageID);
        }
        #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
            FSCI_MsgNVPageEraseMonitoring(mNvVirtualPageProperty[pageID].NvRawSectorStartAddress, status);
        #endif
    }
    return status;
}
/******************************************************************************
 * Name: NvSetErasePgCmdStatus
 * Description: Nv Set Erase Page CmdStatus
 * Parameter(s): [IN] PageToErase - the ID of the page to be erased
 * Parameter(s): [IN] read_legacy_location - need use legacy locaton
 * Parameter(s): [IN] doPageBlankCheck - need do page blank check
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
NVM_STATIC void NvSetErasePgCmdStatus(NVM_VirtualPageID_t PageToErase, bool_t read_legacy_location, bool_t doPageBlankCheck)
{
    mNvActivePageId = (PageToErase == gSecondVirtualPage_c) ? gFirstVirtualPage_c : gSecondVirtualPage_c;

    if (read_legacy_location)
    {
        /* check need do BlankCheck */
        if (((gNVM_PageIsNotBlank_c == NvVirtualPageBlankCheck(PageToErase)) && doPageBlankCheck ) || (!doPageBlankCheck))
        {
            /* request the erase of the  page */
            mNvErasePgCmdStatus.NvPageToErase = PageToErase;
            mNvErasePgCmdStatus.NvSectorAddress = mNvVirtualPageProperty[PageToErase].NvRawSectorStartAddress;
            mNvErasePgCmdStatus.NvErasePending = TRUE;
         }
    }
}


NVM_STATIC void NvPostFwUpdateMaintenance(void)
{
    NVM_VirtualPageID_t pageID;
    uint32_t sect_sz = (uint32_t)((uint8_t*)NV_STORAGE_SECTOR_SIZE);

    for (pageID = gFirstVirtualPage_c; pageID <= gSecondVirtualPage_c; pageID++)
    {
        NVM_VirtualPageProperties_t * page_props = &mNvVirtualPageProperty[pageID];
        uint32_t p = page_props->NvRawSectorStartAddress;

        if (*(uint32_t *)p == 0xffffffff)
        {
            /* we read 0xffffffff from the flash: it either means that it is blank / erased or written with 0xffffffff .
             * HAL_FlashVerifyErase does not make the difference!
             */
            for (uint8_t i = 0; i < page_props->NvRawSectorsCount; i++)
            {
                /* Check if sector requires an erase */
                if (HAL_FlashVerifyErase(p, sect_sz,  kHAL_Flash_MarginValueNormal) != kStatus_HAL_Flash_Success)
                {
                    HAL_FlashEraseSector(p, sect_sz);
                }
                p += sect_sz;
            }
          }
    }
}


/******************************************************************************
 * Name: NvInitStorageSystem
 * Description: Initialize the storage system, retrieve the active page and
 *              the page counter. Called once by NvModuleInit() function.
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
NVM_STATIC void NvInitStorageSystem
(
    bool_t read_legacy_location
)
{
    uint32_t value;
    uint32_t firstPageCounterTopValue;
    uint32_t firstPageCounterBottomValue;
    uint32_t secondPageCounterTopValue;
    uint32_t secondPageCounterBottomValue;
    uint8_t location_offset = (read_legacy_location ? gNvLegacyOffset_c : 0u);

    NvPostFwUpdateMaintenance();

    /* read both pages counter values */
    NV_FlashRead((uint8_t*)mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorStartAddress, (uint8_t*)&value,
                 sizeof(value));
    firstPageCounterTopValue = value;
    NV_FlashRead((uint8_t*)mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1u + location_offset,
                (uint8_t*)&value, sizeof(value));
    firstPageCounterBottomValue = value;

    NV_FlashRead((uint8_t*)mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorStartAddress, (uint8_t*)&value,
                 sizeof(value));
    secondPageCounterTopValue = value;
    NV_FlashRead((uint8_t*)mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1u + location_offset,
                (uint8_t*)&value, sizeof(value));
    secondPageCounterBottomValue = value;


    /* get the active page */
    if((firstPageCounterTopValue == firstPageCounterBottomValue) && (gPageCounterMaxValue_c != firstPageCounterTopValue)) /* first page is valid */
    {
        if((secondPageCounterTopValue == secondPageCounterBottomValue) && (gPageCounterMaxValue_c != secondPageCounterTopValue)) /* second page is valid */
        {
            if(firstPageCounterTopValue >= secondPageCounterTopValue)
            {
                /* first page is active */
                mNvPageCounter = firstPageCounterTopValue;
                mNvActivePageId = gFirstVirtualPage_c;
            }
            else
            {
                /* second page is active */
                mNvPageCounter = secondPageCounterTopValue;
                mNvActivePageId = gSecondVirtualPage_c;
            }
        }
        else
        {
            if(secondPageCounterTopValue != secondPageCounterBottomValue)
            {
                /* first page is active */
                mNvPageCounter = firstPageCounterTopValue;
                NvSetErasePgCmdStatus(gSecondVirtualPage_c, read_legacy_location, FALSE);
            }
            else
            {
                /* first page is active */
                mNvPageCounter = firstPageCounterTopValue;
                NvSetErasePgCmdStatus(gSecondVirtualPage_c, read_legacy_location, TRUE);
            }
        }
    }
    else
    {
        if(firstPageCounterTopValue != firstPageCounterBottomValue) /* first page is not valid */
        {
            if((secondPageCounterTopValue == secondPageCounterBottomValue) && (gPageCounterMaxValue_c != secondPageCounterTopValue)) /* second page is valid */
            {
                /* second page is active */
                mNvPageCounter = secondPageCounterTopValue;
                NvSetErasePgCmdStatus(gFirstVirtualPage_c, read_legacy_location, FALSE);
            }
            else
            {
                mNvActivePageId = gVirtualPageNone_c;
            }
        }
        else
        {
            if((secondPageCounterTopValue == secondPageCounterBottomValue) && (gPageCounterMaxValue_c != secondPageCounterTopValue)) /* second page is valid */
            {
                /* second page is active */
                mNvPageCounter = secondPageCounterTopValue;
                NvSetErasePgCmdStatus(gFirstVirtualPage_c, read_legacy_location, TRUE);
            }
            else
            {
                mNvActivePageId = gVirtualPageNone_c;
            }
        }
    }
}

/******************************************************************************
 * Name: NvVirtualPageBlankCheck
 * Description: checks if the specified page is blank (erased)
 * Parameter(s): [IN] pageID - the ID of the page to be checked
 * Return: gNVM_InvalidPageID_c - if the page ID is not valid
 *         gNVM_PageIsNotBlank_c - if the page is not blank
 *         gNVM_OK_c - if the page is blank (erased)
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvVirtualPageBlankCheck
(
    NVM_VirtualPageID_t pageID
)
{
    NVM_Status_t status = gNVM_OK_c;
    /* if pageID > gSecondVirtualPage_c */
    if(pageID > gSecondVirtualPage_c)
    {
        status = gNVM_InvalidPageID_c;
    }
    else
    {
        /* blank check */
        if(kStatus_HAL_Flash_Success != HAL_FlashVerifyErase (
                                                          mNvVirtualPageProperty[pageID].NvRawSectorStartAddress,
                                                          mNvVirtualPageProperty[pageID].NvTotalPageSize,
                                                          kHAL_Flash_MarginValueNormal
        ))
        {
            status = gNVM_PageIsNotBlank_c;
        }
    }
    return status;
}

/******************************************************************************
 * Name: NvUpdateLastMetaInfoAddress
 * Description: retrieve and store (update) the last meta information address
 * Parameter(s): -
 * Return: gNVM_MetaNotFound_c - if no meta information has been found
 *         gNVM_OK_c - if the meta was found and stored (updated)
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvUpdateLastMetaInfoAddress
(
    void
)
{
    NVM_RecordMetaInfo_t metaValue;
    uint32_t readAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c;
    NVM_Status_t status = gNVM_MetaNotFound_c;

    while(readAddress < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
    {
        NV_FlashRead((uint8_t*)readAddress, (uint8_t*)&metaValue, sizeof(metaValue));

        if(gNvGuardValue_c == metaValue.rawValue)
        {
            if(readAddress == (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
            {
                mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress = gEmptyPageMetaAddress_c;
                #if gUnmirroredFeatureSet_d
                    mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress = gEmptyPageMetaAddress_c;
                #endif
                status = gNVM_OK_c;
                break;
            }
            else
            {
                readAddress -= sizeof(NVM_RecordMetaInfo_t);

                while(readAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
                {
                    NV_FlashRead((uint8_t*)readAddress, (uint8_t*)&metaValue, sizeof(metaValue));

                    if((metaValue.fields.NvValidationStartByte == metaValue.fields.NvValidationEndByte) &&
                       ((gValidationByteSingleRecord_c == metaValue.fields.NvValidationStartByte) ||
                        (gValidationByteAllRecords_c == metaValue.fields.NvValidationStartByte)))
                    {
                        mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress = readAddress;
                        #if gUnmirroredFeatureSet_d
                        {
                            while(readAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
                            {
                                if(metaValue.fields.NvmRecordOffset == 0u)
                                {
                                    readAddress -= sizeof(NVM_RecordMetaInfo_t);
                                    NV_FlashRead((uint8_t*)readAddress, (uint8_t*)&metaValue, sizeof(metaValue));
                                }
                                else
                                {
                                    mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress = readAddress;
                                    break;
                                }
                            }
                        }
                        #endif
                        status = gNVM_OK_c;
                        break;
                    }
                    readAddress -= sizeof(NVM_RecordMetaInfo_t);
                }
                if(gNVM_OK_c != status)
                {
                    status = gNVM_MetaNotFound_c;
                }
                break;
            }
        }
        else
        {
            readAddress += sizeof(NVM_RecordMetaInfo_t);
        }
    }
    return status;
}

/******************************************************************************
 * Name: NvGetMetaInfo
 * Description: get meta information based on the meta information address
 * Parameter(s): [IN] pageID - the ID of the page
 *               [IN] metaInfoAddress - meta information address
 *               [OUT] pMetaInfo - a pointer to a memory location where the
 *                                 requested meta information will be stored
 * Return: gNVM_InvalidPageID_c - if the active page is not valid
 *         gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_AddressOutOfRange_c - if the provided address is out of range
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvGetMetaInfo
(
    NVM_VirtualPageID_t pageId,
    uint32_t metaInfoAddress,
    NVM_RecordMetaInfo_t* pMetaInfo
)
{
    NVM_Status_t status;
    /* check address range */
    if(metaInfoAddress < (mNvVirtualPageProperty[pageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c) ||
       metaInfoAddress > mNvVirtualPageProperty[pageId].NvRawSectorEndAddress)
    {
        status = gNVM_AddressOutOfRange_c;
    }
    else
    {
        /* read the meta information tag */
        NV_FlashRead((uint8_t*)metaInfoAddress, (uint8_t*)pMetaInfo, sizeof(NVM_RecordMetaInfo_t));
        status = gNVM_OK_c;
    }
    return status;
}

/******************************************************************************
 * Name: NvGetPageFreeSpace
 * Description: return the page free space, in bytes
 * Parameter(s): [OUT] ptrFreeSpace - a pointer to a memory location where the
 *                                    page free space will be stored
 * Return: gNVM_InvalidPageID_c - if the active page is not valid
 *         gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_PageIsEmpty_c - if the page is empty
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvGetPageFreeSpace
(
    uint32_t* ptrFreeSpace
)
{
    NVM_RecordMetaInfo_t metaInfo;
    NVM_Status_t retVal;
    #if gUnmirroredFeatureSet_d
    NVM_RecordMetaInfo_t metaInfoUndeleted;
    bool_t ret = FALSE;
    #endif

    if(gEmptyPageMetaAddress_c == mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress)
    {
        #if gNvUseExtendedFeatureSet_d
        *ptrFreeSpace = mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize - mNvTableSizeInFlash - 3u*sizeof(NVM_TableInfo_t);
        #else
        *ptrFreeSpace = mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize - 2u*sizeof(NVM_TableInfo_t);
        #endif /* gNvUseExtendedFeatureSet_d */
        retVal = gNVM_OK_c;
    }
    else
    {
        retVal = NvGetMetaInfo(mNvActivePageId, mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress, &metaInfo);

        if(gNVM_OK_c == retVal)
        {
            #if gUnmirroredFeatureSet_d
            {
                if(metaInfo.fields.NvmRecordOffset == 0u)
                {
                    retVal = NvGetMetaInfo(mNvActivePageId, mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress, &metaInfoUndeleted);
                    if(gNVM_OK_c != retVal)
                    {
                        *ptrFreeSpace = 0;
                        ret = TRUE;
                    }
                    else
                    {
                        metaInfo.fields.NvmRecordOffset = metaInfoUndeleted.fields.NvmRecordOffset;
                    }
                }
            }
            if(FALSE == ret)
            #endif
            {
                *ptrFreeSpace = (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset) -
                    (mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress +
                     sizeof(NVM_RecordMetaInfo_t));
            }
        }
        else
        {
            *ptrFreeSpace = 0;
        }
    }
    return retVal;
}

/******************************************************************************
 * Name: NvIsMemoryAreaAvailable
 * Description: checks if the specified memory area is blank (erased)
 * Parameter(s): [IN] address - start address
 *               [IN] len - length to be verified
 * Return: TRUE if the area is available (blank), FALSE otherwise
 *****************************************************************************/
NVM_STATIC bool_t NvIsMemoryAreaAvailable
(
    uint32_t address,
    uint32_t len
)
{
    bool_t status = TRUE;

    if(len % (uint8_t)PGM_SIZE_BYTE != 0u)
    {
        status = FALSE;
    }
    else
    {
        if(address < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress ||
           address > mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
        {
            status = FALSE;
        }
        else
        {
            if((address + len) > mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress + 1U)
            {
                status = FALSE;
            }
            else
            {
                /* blank check */
                if(kStatus_HAL_Flash_Success != HAL_FlashVerifyErase ( address, len, kHAL_Flash_MarginValueNormal))
                {
                    status = FALSE;
                }
            }
        }
    }
    return status;
}

/******************************************************************************
 * Name: NvIsRecordCopied
 * Description: Checks if a record or an entire table entry is already copied.
 *              Called by page copy function.
 * Parameter(s): [IN] pageId - the ID of the page where to perform the check
 *               [IN] metaInf - a pointer to source page meta information tag
 * Return: TRUE if the element is already copied, FALSE otherwise
 *****************************************************************************/
NVM_STATIC bool_t NvIsRecordCopied
(
    NVM_VirtualPageID_t pageId,
    NVM_RecordMetaInfo_t* metaInf
)
{
    uint32_t loopAddress;
    NVM_RecordMetaInfo_t metaValue;
    bool_t retVal;

    loopAddress = mNvVirtualPageProperty[pageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c;
    retVal = FALSE;

    do
    {
        /* read the meta information tag */
        NV_FlashRead((uint8_t*)loopAddress, (uint8_t*)&metaValue, sizeof(NVM_RecordMetaInfo_t));

        if(gNvGuardValue_c == metaValue.rawValue)
        {
            /* reached last meta */
            break;
        }

        if(metaValue.fields.NvValidationStartByte != metaValue.fields.NvValidationEndByte)
        {
            /* invalid meta */
            loopAddress += sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        if(metaInf->fields.NvmDataEntryID == metaValue.fields.NvmDataEntryID)
        {
            if(metaInf->fields.NvValidationStartByte == gValidationByteSingleRecord_c)
            {
                if(metaValue.fields.NvValidationStartByte == gValidationByteSingleRecord_c)
                {
                    if(metaValue.fields.NvmElementIndex == metaInf->fields.NvmElementIndex)
                    {
                        retVal = TRUE;
                        break;
                    }

                    /* skip */
                    loopAddress += sizeof(NVM_RecordMetaInfo_t);
                    continue;
                }
                retVal = TRUE;
                break;
            }

            if(metaInf->fields.NvValidationStartByte == gValidationByteAllRecords_c)
            {
                if(metaValue.fields.NvValidationStartByte == gValidationByteSingleRecord_c)
                {
                    /* skip */
                    loopAddress += sizeof(NVM_RecordMetaInfo_t);
                    continue;
                }
                retVal = TRUE;
                break;
            }

            /* skip */
            loopAddress += sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        loopAddress += sizeof(NVM_RecordMetaInfo_t);

    } while(loopAddress < mNvVirtualPageProperty[pageId].NvRawSectorEndAddress);

    return retVal;
}

/******************************************************************************
 * Name: NvInternalCopy
 * Description: Performs a copy of an record / entire table entry
 * Parameter(s): [IN] dstAddress - destination record address
 *               [IN] dstMetaAddress - destination meta address
 *               [IN] srcMetaInfo - source meta information
 *               [IN] srcTblEntryIdx - source table entry index
 *               [IN] size - bytes to copy
 * Return: gNVM_InvalidPageID_c - if the source or destination page is not
 *                                valid
 *         gNVM_MetaInfoWriteError_c - if the meta information couldn't be
 *                                     written
 *         gNVM_RecordWriteError_c - if the record couldn't be written
 *         gNVM_Error_c - in case of error(s)
 *         gNVM_OK_c - page copy completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvInternalCopy
(
    uint32_t dstAddress,
    uint32_t dstMetaAddress,
    NVM_RecordMetaInfo_t* srcMetaInfo,
    uint16_t srcTblEntryIdx,
    uint16_t size
)
{
    uint16_t innerOffset;
    uint8_t cacheBuffer[gNvCacheBufferSize_c];
    NVM_RecordMetaInfo_t dstMetaInfo;
    uint16_t diffSize = 0;
    uint16_t diffIdx = 0;
    uint16_t ramSize = 0;
    uint8_t misalignedBytes;
    uint8_t loopIdx;
    uint16_t loopEnd;
    NVM_Status_t status = gNVM_OK_c;

    /* Initialize the inner offset*/
    innerOffset = 0;

    /* prepare destination page meta info tag and write if after the record is entirely written.
    * the preparation is made here because the 'dstAddress' may change afterwards
    */
    dstMetaInfo.fields = srcMetaInfo->fields;
    dstMetaInfo.fields.NvmRecordOffset = (uint16_t)((uint32_t)(dstAddress - mNvVirtualPageProperty[((uint8_t)mNvActivePageId+1U)%2U].NvRawSectorStartAddress));

    if (srcMetaInfo->fields.NvValidationStartByte != gValidationByteSingleRecord_c)
    {
        ramSize = pNVM_DataTable[srcTblEntryIdx].ElementsCount * pNVM_DataTable[srcTblEntryIdx].ElementSize;
        /* if the bytes to copy are less then RAM table entry space, the supplementary bytes to write on the destination page
        * will be retrieved from RAM table entry. This is the case when the RAM table has been updated and the new
        * entry's elements count is greater then the one existing in the previous RAM table, now stored in the FLASH active page
        * (source page) */
        if(size < ramSize)
        {
            diffSize = ramSize - size;
            diffIdx = size / pNVM_DataTable[srcTblEntryIdx].ElementSize;
        }
    }

    while(size != 0u)
    {
        if(size > (uint16_t)gNvCacheBufferSize_c)
        {
            /* copy from FLASH to cache buffer */
            NV_FlashRead((uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + srcMetaInfo->fields.NvmRecordOffset + innerOffset,
                         (uint8_t*)&cacheBuffer[0], (uint16_t)gNvCacheBufferSize_c);

            /* write to destination page */
            if(kStatus_HAL_Flash_Success == HAL_FlashProgramUnaligned(dstAddress, (uint16_t)gNvCacheBufferSize_c, cacheBuffer))
            {
                /* update the destination record address copy */
                dstAddress += (uint16_t)gNvCacheBufferSize_c;
                /* update the record size */
                size -= (uint16_t)gNvCacheBufferSize_c;
                /* update the inner offset value */
                innerOffset += (uint16_t)gNvCacheBufferSize_c;

                continue;
            }
            status = gNVM_RecordWriteError_c;
            break;
        }
        else
        {
            /* copy from FLASH to cache buffer */
            NV_FlashRead((uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + srcMetaInfo->fields.NvmRecordOffset + innerOffset,
                         (uint8_t*)&cacheBuffer[0], size);
            break;
        }
    }

    if(gNVM_OK_c == status)
    {
        if(diffSize != 0u)
        {
            /* update the destination record address copy */
            dstAddress += size;

            /* check alignment and adjust it if necessary */
            misalignedBytes = (uint8_t)((dstAddress - (dstAddress & (~((uint32_t)PGM_SIZE_BYTE-1U)))));

            /* initialise the inner offset */
            innerOffset = 0;

            /* check if the destination is longword aligned or not */
            if(misalignedBytes != 0u)
            {
                /* align to previous 32 bit boundary */
                dstAddress &= ((uint32_t)~(uint32_t)((uint32_t)PGM_SIZE_BYTE-1UL));

                /* compute the loop end */
                loopEnd = (uint16_t)PGM_SIZE_BYTE - misalignedBytes;


                /* update with data from RAM */
                for(loopIdx = 0; loopIdx < loopEnd; loopIdx++)
                {
                    cacheBuffer[misalignedBytes] = *((uint8_t*)pNVM_DataTable[srcTblEntryIdx].pData +
                                                     (diffIdx * pNVM_DataTable[srcTblEntryIdx].ElementSize) + innerOffset);
                    innerOffset++;
                    misalignedBytes++;
                    if (innerOffset == diffSize)
                    {
                        break;
                    }
                }

                /* write to Flash destination page */
                if(kStatus_HAL_Flash_Success != HAL_FlashProgramUnaligned(dstAddress, PGM_SIZE_BYTE, cacheBuffer))
                {
                    status = gNVM_RecordWriteError_c;
                }
                else
                {
                    /* align to next 32 bit boundary */
                    dstAddress += (uint32_t)PGM_SIZE_BYTE;
                }
            }

            if(gNVM_OK_c == status)
            {
                /* write to Flash destination page the rest of the aligned data */
                if(kStatus_HAL_Flash_Success != HAL_FlashProgramUnaligned(dstAddress, (uint32_t)diffSize - (uint32_t)innerOffset,
                                                       ((uint8_t*)pNVM_DataTable[srcTblEntryIdx].pData + (diffIdx * pNVM_DataTable[srcTblEntryIdx].ElementSize) + innerOffset)))
                {
                    status = gNVM_RecordWriteError_c;
                }
            }
        }
        else
        {
            /* write to destination page */
            if(kStatus_HAL_Flash_Success == HAL_FlashProgramUnaligned(dstAddress, (uint16_t)size, cacheBuffer))
            {
            }
            else
            {

                status = gNVM_RecordWriteError_c;
            }

        }
        if(gNVM_OK_c == status)
        {
            /* write the associated record meta information */
            if(kStatus_HAL_Flash_Success != HAL_FlashProgramUnaligned(dstMetaAddress, sizeof(NVM_RecordMetaInfo_t), (uint8_t*)(&dstMetaInfo)))
            {
                status = gNVM_MetaInfoWriteError_c;
            }
        }
    }
    return status;
}

/******************************************************************************
 * Name: NvGetTblEntryMetaAddrFromId
 * Description: Gets the table entry meta address based on table entry ID
 * Parameter(s): [IN] searchStartAddress - the search start address
 *               [IN] dataEntryId - table entry ID
 * Return: the value of the meta address
 *****************************************************************************/
#if gNvFragmentation_Enabled_d
NVM_STATIC uint32_t NvGetTblEntryMetaAddrFromId
(
    uint32_t searchStartAddress,
    uint16_t dataEntryId
)
{
    NVM_RecordMetaInfo_t metaInfo = {0};
    uint32_t status = 0U;

    while(searchStartAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
    {
        (void)NvGetMetaInfo(mNvActivePageId, searchStartAddress, &metaInfo);

        if((metaInfo.fields.NvValidationStartByte != gValidationByteAllRecords_c) ||
           (metaInfo.fields.NvValidationStartByte != metaInfo.fields.NvValidationEndByte))
        {
            searchStartAddress -= sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        if(metaInfo.fields.NvmDataEntryID == dataEntryId)
        {
            /* found it */
            status = searchStartAddress;
            break;
        }

        searchStartAddress -= sizeof(NVM_RecordMetaInfo_t);
    }
    return status;
}

/******************************************************************************
 * Name: NvInternalRecordsUpdate
 * Description: Performs to update nv records
 * Parameter(s): [IN] srcMetaAddr - source page meta address
 *               [IN] srcTblEntryIdx - source page table entry index
 *               [IN] ownerRecordMetaInfo - pointer to the location of a full dataset save
 * Return: the status of the operation
 *****************************************************************************/
NVM_STATIC void NvInternalRecordsUpdate(uint32_t srcMetaAddr,uint16_t srcTblEntryIdx, NVM_RecordMetaInfo_t *ownerRecordMetaInfo)
{

    NVM_RecordMetaInfo_t metaInfo = {0};
    uint32_t metaAddress = srcMetaAddr;

    /* clear the records offsets buffer */
    FLib_MemSet(maNvRecordsCpyOffsets, 0, (uint32_t)sizeof(uint16_t)*pNVM_DataTable[srcTblEntryIdx].ElementsCount);

    while(metaAddress > (uint32_t)ownerRecordMetaInfo)
    {
        /* get meta information */
        (void)NvGetMetaInfo(mNvActivePageId, metaAddress, &metaInfo);

        /* skip invalid entries and full table records */
        if((metaInfo.fields.NvValidationStartByte != metaInfo.fields.NvValidationEndByte) ||
           (metaInfo.fields.NvValidationStartByte != gValidationByteSingleRecord_c))
        {
            metaAddress -= sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        /* check if the element still belongs to an valid RAM table entry */
        if(metaInfo.fields.NvmElementIndex >= pNVM_DataTable[srcTblEntryIdx].ElementsCount)
        {
            /* the FLASH element is no longer a current RAM table entry element */
            metaAddress -= sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        /* found a new single record not copied */
        if ((metaInfo.fields.NvmDataEntryID == ownerRecordMetaInfo->fields.NvmDataEntryID) &&
             (0u == maNvRecordsCpyOffsets[metaInfo.fields.NvmElementIndex]))
        {
            maNvRecordsCpyOffsets[metaInfo.fields.NvmElementIndex] = metaInfo.fields.NvmRecordOffset;
        }
        metaAddress -= sizeof(NVM_RecordMetaInfo_t);
    }
}

/******************************************************************************
 * Name: NvInternalDefragmentedCopy
 * Description: Performs defragmentation and copy from the source page to
 *              the destination one
 * Parameter(s): [IN] srcMetaAddr - source page meta address
 *               [IN] srcTblEntryIdx - source page table entry index
 *               [IN] dstMetaAddr - destination meta address
 *               [IN] dstRecordAddr - destination record address (to copy to)
 *               [IN] ownerRecordMetaInfo - pointer to the location of a full dataset save
 * Return: the status of the operation
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvInternalDefragmentedCopy
(
    uint32_t srcMetaAddr,
    uint16_t srcTblEntryIdx,
    uint32_t dstMetaAddr,
    uint32_t dstRecordAddr,
    NVM_RecordMetaInfo_t *ownerRecordMetaInfo
)
{
    NVM_Status_t status = gNVM_OK_c;
    uint32_t size = (uint32_t)pNVM_DataTable[srcTblEntryIdx].ElementSize * pNVM_DataTable[srcTblEntryIdx].ElementsCount;
    uint8_t space_left;
    uint16_t copy_amount;
    uint8_t dstBuffer[PGM_SIZE_BYTE];
    uint16_t element_idx = 0;
    uint8_t element_inner_copied = 0;
    uint16_t written_buffers_count = 0;

    NVM_RecordMetaInfo_t dstMetaInfo;
    #if gNvUseExtendedFeatureSet_d
    NVM_DataEntry_t flashDataEntry;
    bool_t fillFromRAM = FALSE;
    #endif /* gNvUseExtendedFeatureSet_d */

#if gNvUseExtendedFeatureSet_d
    fillFromRAM = FALSE;
    /* RAM table was updated */
    if(mNvTableUpdated)
    {
       (void)NvGetTableEntry(pNVM_DataTable[srcTblEntryIdx].DataEntryID, &flashDataEntry);
       if(pNVM_DataTable[srcTblEntryIdx].ElementsCount > flashDataEntry.ElementsCount)
       {
           /* fill the FLASH destination page with the default RAM value for the missing element(s) */
           fillFromRAM = TRUE;
       }
    }
#endif /* gNvUseExtendedFeatureSet_d */

    NvInternalRecordsUpdate(srcMetaAddr, srcTblEntryIdx, ownerRecordMetaInfo);
    while (size != 0u)
    {
        space_left = PGM_SIZE_BYTE;
        FLib_MemSet(dstBuffer, 0xFF, PGM_SIZE_BYTE);

        /* fill the internal buffer */
        while ((0u != space_left) && (element_idx < pNVM_DataTable[srcTblEntryIdx].ElementsCount))
        {
            /* plenty of space left to copy the rest of the element */
            if (space_left >= (pNVM_DataTable[srcTblEntryIdx].ElementSize - element_inner_copied))
            {
                copy_amount = pNVM_DataTable[srcTblEntryIdx].ElementSize - element_inner_copied;
                #if gNvUseExtendedFeatureSet_d
                /* copy the rest of the data from the RAM entry */
                if (fillFromRAM && element_idx >= flashDataEntry.ElementsCount)
                {
                    FLib_MemCpy(dstBuffer + ((uint8_t)PGM_SIZE_BYTE - space_left),
                                (uint8_t*)((uint32_t)(uint32_t *)pNVM_DataTable[srcTblEntryIdx].pData + (uint32_t)element_idx * pNVM_DataTable[srcTblEntryIdx].ElementSize + element_inner_copied),
                                copy_amount);
                }
                else
                #endif
                /* copy from the owning full record save if no single save offset was found */
                if (0u == maNvRecordsCpyOffsets[element_idx])
                {
                    FLib_MemCpy(dstBuffer + ((uint8_t)PGM_SIZE_BYTE - space_left),
                                (uint8_t*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + ownerRecordMetaInfo->fields.NvmRecordOffset + element_idx * pNVM_DataTable[srcTblEntryIdx].ElementSize + element_inner_copied),
                                copy_amount);
                }
                else
                {
                    FLib_MemCpy(dstBuffer + ((uint8_t)PGM_SIZE_BYTE - space_left),
                                (uint8_t*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + maNvRecordsCpyOffsets[element_idx] + element_inner_copied),
                                copy_amount);
                }
                space_left -= (uint8_t)copy_amount;

                /* move to next element */
                element_idx++;
                element_inner_copied = 0;
                continue;
            }
            else
            {
                copy_amount = space_left;
                #if gNvUseExtendedFeatureSet_d
                /* copy the rest of the data from the RAM entry */
                if (fillFromRAM && element_idx >= flashDataEntry.ElementsCount)
                {
                    FLib_MemCpy(dstBuffer + ((uint8_t)PGM_SIZE_BYTE - space_left),
                                (uint8_t*)((uint32_t)(uint32_t *)pNVM_DataTable[srcTblEntryIdx].pData + (uint32_t)element_idx * pNVM_DataTable[srcTblEntryIdx].ElementSize + element_inner_copied),
                                copy_amount);
                }
                else
                #endif
                /* copy from the owning full record save if no single save offset was found */
                if (0u == maNvRecordsCpyOffsets[element_idx])
                {
                    FLib_MemCpy(dstBuffer + ((uint8_t)PGM_SIZE_BYTE - space_left),
                                (uint8_t*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + ownerRecordMetaInfo->fields.NvmRecordOffset + element_idx * pNVM_DataTable[srcTblEntryIdx].ElementSize + element_inner_copied),
                                copy_amount);
                }
                else
                {
                    FLib_MemCpy(dstBuffer + ((uint8_t)PGM_SIZE_BYTE - space_left),
                                (uint8_t*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + maNvRecordsCpyOffsets[element_idx] + element_inner_copied),
                                copy_amount);
                }
                element_inner_copied += (uint8_t)copy_amount;
                break;
            }
        }
        /* write the record data */
        if(kStatus_HAL_Flash_Success != HAL_FlashProgramUnaligned(dstRecordAddr + written_buffers_count * PGM_SIZE_BYTE, PGM_SIZE_BYTE, dstBuffer))
        {
            status = gNVM_RecordWriteError_c;
            break;
        }
        written_buffers_count++;

        /* copied all the data, exit */
        if (size <= (uint16_t)PGM_SIZE_BYTE)
        {
            break;
        }
        size -= (uint16_t)PGM_SIZE_BYTE;
    }

    if(gNVM_OK_c == status)
    {
        /* write meta information tag */
        dstMetaInfo.fields.NvValidationStartByte = gValidationByteAllRecords_c;
        dstMetaInfo.fields.NvmDataEntryID = ownerRecordMetaInfo->fields.NvmDataEntryID;
        dstMetaInfo.fields.NvmElementIndex = 0;
        dstMetaInfo.fields.NvmRecordOffset = (uint16_t)(uint32_t)(dstRecordAddr - mNvVirtualPageProperty[((uint8_t)mNvActivePageId+1U)%2U].NvRawSectorStartAddress);
        dstMetaInfo.fields.NvValidationEndByte = gValidationByteAllRecords_c;

        /* the offset has to be 4 bytes aligned, an extra check is performed to avoid further hard faults caused by FTFx controller */
        if((dstMetaInfo.fields.NvmRecordOffset & (uint16_t)0x3) != 0u)
        {
            status = gNVM_AlignamentError_c;
        }
        else
        {
            /* write the associated record meta information */
            if(kStatus_HAL_Flash_Success != HAL_FlashProgram(dstMetaAddr, sizeof(NVM_RecordMetaInfo_t), (uint8_t*)(&dstMetaInfo)))
            {
                status = gNVM_MetaInfoWriteError_c;
            }
        }
    }
    return status;
}
#endif /* gNvFragmentation_Enabled_d */

/******************************************************************************
 * Name: NvIsMetaInfoValid
 * Description: Performs to check is the meta is valid
 * Parameter(s): [IN] srcMetaAddress - point to the variable which stored meta info address
 *               [IN] srcMetaInfo - point to the meta info
 *               [IN] srcTableEntryIdx - point to the variable which stored table entry index
 *               [IN] skipEntryId - point to the variable which stored the entry ID that should skip
 *               [IN] dstPageId - point to the variable which stored the target page ID
 * Return: the status of the operation
 *****************************************************************************/
NVM_STATIC bool_t NvIsMetaInfoValid
(
    uint32_t *srcMetaAddress,
    NVM_RecordMetaInfo_t *srcMetaInfo,
    uint16_t *srcTableEntryIdx,
    NvTableEntryId_t skipEntryId,
    NVM_VirtualPageID_t dstPageId
)
{
    bool_t state = TRUE;
    #if gNvUseExtendedFeatureSet_d
    uint16_t idx;
    bool_t entryFound;
    uint32_t tableEndAddr = (uint32_t)gNVM_TABLE_endAddr_c;
    uint32_t tableStartAddr = (uint32_t)gNVM_TABLE_startAddr_c;
    #endif /* gNvUseExtendedFeatureSet_d */

    /* do ... while(FALSE) for MISRA 15.5 control flow problem */
    do
    {
        #if gNvUseExtendedFeatureSet_d
        /* NV RAM table has been updated */
        /* Check if meta info still in RAM table */
        if(mNvTableUpdated)
        {
            idx = 0;
            entryFound = FALSE;

            /* check if the saved entry is still present in the new RAM table */
            while(idx < (uint16_t)((tableEndAddr - tableStartAddr)/sizeof(NVM_DataEntry_t)))
            {
                if(srcMetaInfo->fields.NvmDataEntryID == pNVM_DataTable[idx].DataEntryID)
                {
                    entryFound = TRUE;
                    break;
                }
                idx++;
            }

            if(!entryFound)
            {
                /* move to the next meta info */
                *srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                    state = FALSE;
                    break;
            }
        }
        #endif /* gNvUseExtendedFeatureSet_d */

        /* get table entry index */
        *srcTableEntryIdx = NvGetTableEntryIndexFromId(srcMetaInfo->fields.NvmDataEntryID);
        /* Check if VSB ?= VEB */
        if(NvIsRecordCopied(dstPageId, srcMetaInfo) ||
           (srcMetaInfo->fields.NvValidationStartByte != srcMetaInfo->fields.NvValidationEndByte) ||
           (*srcTableEntryIdx == gNvInvalidDataEntry_c) ||
           (srcMetaInfo->fields.NvmDataEntryID == skipEntryId))
        {
            /* go to the next meta information tag */
            *srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                state = FALSE;
                break;
        }
        /* Check if the  record type is vaild */
        if((srcMetaInfo->fields.NvValidationStartByte != gValidationByteSingleRecord_c) &&
           (srcMetaInfo->fields.NvValidationStartByte != gValidationByteAllRecords_c))
        {
            /* go to the next meta information tag */
            *srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                state = FALSE;
                break;
        }

        #if gUnmirroredFeatureSet_d
        if (gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[*srcTableEntryIdx].DataEntryType)
        {
            /*check if the data was erased using NvErase or is just uninitialised*/
            if (NULL == ((void**)pNVM_DataTable[*srcTableEntryIdx].pData)[srcMetaInfo->fields.NvmElementIndex] &&
                NvIsRecordErased(*srcTableEntryIdx, srcMetaInfo->fields.NvmElementIndex, *srcMetaAddress))
            {
                /* go to the next meta information tag */
                *srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                state = FALSE;
                break;
            }
        }
        #endif
    } while(FALSE);

    return state;
}
/******************************************************************************
 * Name: NvIsNvTableChanged
 * Description: Performs to check if the NvTable is changed
 * Parameter(s): [IN] srcMetaAddress - point to the variable which stored meta info address
 *               [IN] srcMetaInfo - point to the meta info
 *               [IN] srcTableEntryIdx - point to the variable which stored table entry index
 *               [IN] tableUpgraded - point to the variable which stored the state of if a table upgrate is happend
 *               [IN] bytesToCopy - point to the variable which stored how many bytes should copy
 * Return: the status of the operation
 *****************************************************************************/
NVM_STATIC bool_t NvIsNvTableChanged
(
    uint32_t *srcMetaAddress,
    NVM_RecordMetaInfo_t *srcMetaInfo,
    uint16_t *srcTableEntryIdx,
#if gNvUseExtendedFeatureSet_d
    bool_t *tableUpgraded,
#endif /* gNvUseExtendedFeatureSet_d */
    uint32_t *bytesToCopy
)
{
    bool_t status = FALSE;

#if gNvUseExtendedFeatureSet_d
    NVM_DataEntry_t flashDataEntry;

    /* do ... while(FALSE) for MISRA 15.5 control flow problem */
    do
    {
        /* NV RAM table has been updated */
        if(mNvTableUpdated)
        {
            if(NvGetTableEntry(pNVM_DataTable[*srcTableEntryIdx].DataEntryID, &flashDataEntry))
            {
                /* entries changed from mirrored/unmirrored and with different entry size cannot be recovered */
                if ((((gNVM_MirroredInRam_c == (NVM_DataEntryType_t)flashDataEntry.DataEntryType) || (gNVM_MirroredInRam_c == (NVM_DataEntryType_t)pNVM_DataTable[*srcTableEntryIdx].DataEntryType)) && (flashDataEntry.DataEntryType != pNVM_DataTable[*srcTableEntryIdx].DataEntryType))
                    || (flashDataEntry.ElementSize != pNVM_DataTable[*srcTableEntryIdx].ElementSize))
                {
                    *srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                    status = TRUE;
                    break;
                }

                if(flashDataEntry.ElementsCount != pNVM_DataTable[*srcTableEntryIdx].ElementsCount)
                {
                    if (*tableUpgraded)
                    {
                        if (flashDataEntry.ElementsCount < pNVM_DataTable[*srcTableEntryIdx].ElementsCount)
                        {
                            /* copy only the bytes that were previously written to FLASH virtual page */
                            *bytesToCopy = (uint32_t)flashDataEntry.ElementsCount * flashDataEntry.ElementSize;
                        }
                        #if gNvFragmentation_Enabled_d
                        /*ignore if out of bounds*/
                        if (srcMetaInfo->fields.NvValidationStartByte == gValidationByteSingleRecord_c &&
                            srcMetaInfo->fields.NvmElementIndex >= pNVM_DataTable[*srcTableEntryIdx].ElementsCount)
                        {
                            *srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                            status = TRUE;
                            break;
                        }
                        #endif
                    }
                    else
                    {
                        *srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                        status = TRUE;
                        break;
                    }
                }
            }
        }
    } while(FALSE);
#endif /* gNvUseExtendedFeatureSet_d */
    return status;
}
/******************************************************************************
 * Name: NvCopyRecord
 * Description: Performs to copy the record to another page
 * Parameter(s): [IN] dstMetaAddress - point to the variable which stored the target meta info address
 *               [IN] srcMetaAddress - point to the variable which stored meta info address
 *               [IN] srcMetaInfo - point to the meta info
 *               [IN] dstRecordAddress - point to the variable which stored the target record address
 *               [IN] srcTableEntryIdx - point to the variable which stored table entry index
 *               [IN] tblEntryMetaAddress - point to the variable which stored table entry meta address
 *               [IN] tableUpgraded - point to the variable which stored the state of if a table upgrate is happend
 *               [IN] bytesToCopy - point to the variable which stored how many bytes should copy
 * Return: the status of the operation
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvCopyRecord
(
    uint32_t *dstMetaAddress,
    uint32_t *srcMetaAddress,
    NVM_RecordMetaInfo_t *srcMetaInfo,
    uint32_t *dstRecordAddress,
    uint16_t *srcTableEntryIdx,
#if gNvFragmentation_Enabled_d
    uint32_t *tblEntryMetaAddress,
#endif /* gNvFragmentation_Enabled_d */
#if gNvUseExtendedFeatureSet_d
    bool_t *tableUpgraded,
#endif /* gNvUseExtendedFeatureSet_d */
    uint32_t *bytesToCopy
)
{
    NVM_Status_t status;
    /* if the copy operation must take elements from ram */
#if gNvUseExtendedFeatureSet_d
    if(mNvTableUpdated && *tableUpgraded &&
       *bytesToCopy < (uint32_t)pNVM_DataTable[*srcTableEntryIdx].ElementsCount * pNVM_DataTable[*srcTableEntryIdx].ElementSize)
    {
        /* make sure the adress can hold the entire space (+ what is taken from ram) */
        *dstRecordAddress -= NvUpdateSize((uint32_t)pNVM_DataTable[*srcTableEntryIdx].ElementsCount * pNVM_DataTable[*srcTableEntryIdx].ElementSize);
    }
    else
#endif
    {
        /* compute the destination record start address */
        *dstRecordAddress -= NvUpdateSize(*bytesToCopy);
    }

    #if gNvFragmentation_Enabled_d
    /*
    * single element record
    */
    if(srcMetaInfo->fields.NvValidationStartByte == gValidationByteSingleRecord_c)
    {
        status = NvInternalDefragmentedCopy(*srcMetaAddress, *srcTableEntryIdx, *dstMetaAddress, *dstRecordAddress, (NVM_RecordMetaInfo_t *)(*tblEntryMetaAddress));
    }
    else
    #endif /* gNvFragmentation_Enabled_d */
    /*
    * full table entry
    */
    {
        status = NvInternalCopy(*dstRecordAddress, *dstMetaAddress, srcMetaInfo, *srcTableEntryIdx, (uint16_t)(*bytesToCopy));
    }

    return status;
}

/******************************************************************************
 * Name: NvCopyPage
 * Description: Copy the active page content to the mirror page. Only the
 *              latest table entries / elements are copied. A merge operation
 *              is performed before copy if an entry has single elements
 *              saved priori and newer than the table entry. If one or more
 *              elements were singular saved and the NV page doesn't has a
 *              full table entry saved, then the elements are copied as they
 *              are.
 * Parameter(s): [IN] skipEntryId - the entry ID to be skipped when page
 *                                  copy is performed
 * Return: gNVM_InvalidPageID_c - if the source or destination page is not
 *                                valid
 *         gNVM_MetaInfoWriteError_c - if the meta information couldn't be
 *                                     written
 *         gNVM_RecordWriteError_c - if the record couldn't be written
 *         gNVM_Error_c - in case of error(s)
 *         gNVM_OK_c - page copy completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvCopyPage
(
    NvTableEntryId_t skipEntryId
)
{
    /* source page related variables */
    uint32_t srcMetaAddress;
    NVM_RecordMetaInfo_t srcMetaInfo = {0};
    uint16_t srcTableEntryIdx;

    /* destination page related variables */
    uint32_t dstMetaAddress;
    uint32_t firstMetaAddress;
    NVM_VirtualPageID_t dstPageId;
    uint32_t dstRecordAddress;

    #if gNvUseExtendedFeatureSet_d
    bool_t tableUpgraded = FALSE;
    #endif /* gNvUseExtendedFeatureSet_d */
    #if gNvFragmentation_Enabled_d
    uint32_t tblEntryMetaAddress = 0;
    #endif
    uint32_t bytesToCopy;

    /* status variable */
    NVM_Status_t status = gNVM_OK_c;

    dstPageId = (NVM_VirtualPageID_t)(uint8_t)(((uint8_t)mNvActivePageId+1u)%2u);

    /* Check if the destination page is blank. If not, erase it. */
    if(gNVM_PageIsNotBlank_c == NvVirtualPageBlankCheck(dstPageId))
    {
        status = NvEraseVirtualPage(dstPageId);
    }
    if(gNVM_OK_c == status)
    {
        /* initialise the destination page meta info start address */
        dstMetaAddress = mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c;
        #if gNvUseExtendedFeatureSet_d
        if (mNvTableUpdated)
        {
            tableUpgraded = (GetFlashTableVersion() != mNvFlashTableVersion);
        }
        #endif

        firstMetaAddress = dstMetaAddress;
        /*if src is an empty page, just copy the table and make the initialisations*/
        srcMetaAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress;
        if (srcMetaAddress != gEmptyPageMetaAddress_c)
        {
            /* initialise the destination page record start address */
            dstRecordAddress = mNvVirtualPageProperty[dstPageId].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1U;

            while(srcMetaAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
            {
                /* get current meta information */
                (void)NvGetMetaInfo(mNvActivePageId, srcMetaAddress, &srcMetaInfo);

                /* Check if meta info is vaild */
                if (!NvIsMetaInfoValid(&srcMetaAddress, &srcMetaInfo, &srcTableEntryIdx, skipEntryId, dstPageId))
                {
                    continue;
                }

                /* compute the destination record start address */
                bytesToCopy = (uint32_t)pNVM_DataTable[srcTableEntryIdx].ElementsCount * pNVM_DataTable[srcTableEntryIdx].ElementSize;

                /* Check if NvTable is changed */
                if (NvIsNvTableChanged(&srcMetaAddress, &srcMetaInfo,&srcTableEntryIdx,
                                    #if gNvUseExtendedFeatureSet_d
                                       &tableUpgraded,
                                    #endif /* gNvUseExtendedFeatureSet_d */
                                       &bytesToCopy))
                {
                    continue;
                }

                #if gNvFragmentation_Enabled_d
                if (srcMetaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c)
                {
                    #if gUnmirroredFeatureSet_d
                    if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[srcTableEntryIdx].DataEntryType)
                    {
                        tblEntryMetaAddress = 0;
                    }
                    else
                    #endif
                    {
                        tblEntryMetaAddress = NvGetTblEntryMetaAddrFromId(srcMetaAddress, srcMetaInfo.fields.NvmDataEntryID);
                    }

                    /* if the record has no full entry associated perform simple copy */
                    if (tblEntryMetaAddress == 0u)
                    {
                        /* compute the 'real record size' taking into consideration that the FTFL controller only writes in burst of 4 bytes */
                        bytesToCopy = pNVM_DataTable[srcTableEntryIdx].ElementSize;
                        dstRecordAddress -= NvUpdateSize(bytesToCopy);

                        status = NvInternalCopy(dstRecordAddress, dstMetaAddress, &srcMetaInfo, srcTableEntryIdx, (uint16_t)bytesToCopy);
                        if(status != gNVM_OK_c)
                        {
                            break;
                        }
                        #if gUnmirroredFeatureSet_d
                        if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[srcTableEntryIdx].DataEntryType)
                        {
                            OSA_InterruptDisable();
                            /* set the pointer to the flash data */
                            if (NvIsNVMFlashAddress(((void**)pNVM_DataTable[srcTableEntryIdx].pData)[srcMetaInfo.fields.NvmElementIndex]))
                            {
                                ((uint8_t**)pNVM_DataTable[srcTableEntryIdx].pData)[srcMetaInfo.fields.NvmElementIndex] = (uint8_t*)dstRecordAddress;
                            }
                            OSA_InterruptEnable();
                        }
                        #endif
                        /* update destination meta information address */
                        dstMetaAddress += sizeof(NVM_RecordMetaInfo_t);

                        /* move to the next meta info */
                        srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                        continue;
                    }
                }
                #endif
                /* Copy record operation */
                status = NvCopyRecord(&dstMetaAddress, &srcMetaAddress, &srcMetaInfo, &dstRecordAddress, &srcTableEntryIdx,
                                    #if gNvFragmentation_Enabled_d
                                      &tblEntryMetaAddress,
                                    #endif
                                    #if gNvUseExtendedFeatureSet_d
                                      &tableUpgraded,
                                    #endif
                                      &bytesToCopy);
                if (gNVM_OK_c != status)
                {
                    break;
                }

                /* update destination meta information address */
                dstMetaAddress += sizeof(NVM_RecordMetaInfo_t);

                /* move to the next meta info */
                srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
            };
        }

        if(gNVM_OK_c == status)
        {
            /* make a request to erase the old page */
            mNvErasePgCmdStatus.NvPageToErase = mNvActivePageId;
            mNvErasePgCmdStatus.NvSectorAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;
            mNvErasePgCmdStatus.NvErasePending = TRUE;

            /* update the the active page ID */
            mNvActivePageId = dstPageId;

            /* update the last meta info address */
            if(dstMetaAddress == firstMetaAddress)
            {
                mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress = gEmptyPageMetaAddress_c;
            }
            else
            {
                mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress = dstMetaAddress - sizeof(NVM_RecordMetaInfo_t);
            }

            #if gUnmirroredFeatureSet_d
            mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress;
            #endif

            mNvPageCounter++;
            /* save the current RAM table */
            if(!NvSaveRamTable(dstPageId))
            {
                status = gNVM_Error_c;
            }
            else
            {
                #if gNvUseExtendedFeatureSet_d
                if(mNvTableUpdated)
                {
                    /* update the size of the NV table stored in FLASH */
                    mNvTableSizeInFlash = NvGetFlashTableSize();

                    /* clear the flag */
                    mNvTableUpdated = FALSE;
                }
                #endif /* gNvUseExtendedFeatureSet_d */
            }
        }
    }
    return status;
}

/******************************************************************************
 * Name: NvInternalFormat
 * Description: Format the NV storage system. The function erases in place both
 *              virtual pages and then writes the page counter value to first
 *              virtual page. The provided page counter value is automatically
 *              incremented and then written to first (active) virtual page.
 * Parameter(s): [IN] pageCounterValue - the page counter value that will
 *                                       be incremented and then written to
 *                                       active page
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_FormatFailure_c - if the format operation fails
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvInternalFormat
(
    uint32_t pageCounterValue
)
{
    uint8_t retryCount = gNvFormatRetryCount_c;
    NVM_Status_t status;

    /* increment the page counter value */
    if(pageCounterValue == (uint32_t)gPageCounterMaxValue_c - 1U)
    {
        pageCounterValue = 1;
    }
    else
    {
        pageCounterValue++;
    }
    mNvPageCounter = pageCounterValue;

    while(retryCount-- != 0u)
    {
        /* erase first page */
        if (gNVM_OK_c == NvEraseVirtualPage(gFirstVirtualPage_c))
        {
            if (gNVM_OK_c == NvEraseVirtualPage(gSecondVirtualPage_c))
            {
                break;
            }
        }
    }

    /* active page after format = first virtual page */
    mNvActivePageId = gFirstVirtualPage_c;

    /* save NV table from RAM memory to FLASH memory */
    if (FALSE == NvSaveRamTable(mNvActivePageId))
    {
        status = gNVM_FormatFailure_c;
    }
    else
    {
        #if gNvUseExtendedFeatureSet_d
        /* update the size of the NV table stored in FLASH */
        mNvTableSizeInFlash = NvGetFlashTableSize();
        #endif

        /* update the page counter value */
        mNvPageCounter = pageCounterValue;

        status = NvUpdateLastMetaInfoAddress();
    }
    return status;
}

/******************************************************************************
 * Name: NvSaveRamTable
 * Description: Saves the NV table
 * Parameter(s): [IN] pageId - the virtual page ID where the table will be
 *                             saved
 * Return: TRUE if table saved successfully, FALSE otherwise
 ******************************************************************************/
NVM_STATIC bool_t NvSaveRamTable
(
    NVM_VirtualPageID_t pageId
)
{
    uint64_t tmp;
    uint32_t addr;
#if gNvUseExtendedFeatureSet_d
    uint16_t idx;
    bool_t ret = FALSE;
#endif
    bool_t status = FALSE;

    assert(NULL != pNVM_DataTable);
    /* write table qualifier start */
    addr = mNvVirtualPageProperty[pageId].NvRawSectorStartAddress;

    #if gNvUseExtendedFeatureSet_d
    tmp = ((NVM_TableInfo_t){.fields.NvPageCounter  = mNvPageCounter,
                             .fields.NvTableMarker  = mNvTableMarker,
                             .fields.NvTableVersion = mNvFlashTableVersion}).rawValue;
    #else
    tmp = ((NVM_TableInfo_t){.fields.NvPageCounter  = mNvPageCounter}).rawValue;
    #endif
    /*write page counter, table amrker, table version top*/
    if(kStatus_HAL_Flash_Success == HAL_FlashProgram(addr, sizeof(NVM_TableInfo_t), (uint8_t*)(&tmp)))
    {
        #if gNvUseExtendedFeatureSet_d
        addr += sizeof(NVM_TableInfo_t);
        idx = 0;
        while(idx < gNVM_TABLE_entries_c)
        {
            /* write data entry ID */
            tmp = ((NVM_EntryInfo_t){.fields.NvDataEntryID   = pNVM_DataTable[idx].DataEntryID,
                   .fields.NvDataEntryType = pNVM_DataTable[idx].DataEntryType,
                   .fields.NvElementsCount = pNVM_DataTable[idx].ElementsCount,
                   .fields.NvElementSize   = pNVM_DataTable[idx].ElementSize}).rawValue;
            if(kStatus_HAL_Flash_Success != HAL_FlashProgram(addr, sizeof(NVM_EntryInfo_t), (uint8_t*)(&tmp)))
            {
                ret = TRUE;
                break;
            }
            /* increment address */
            addr += sizeof(NVM_EntryInfo_t);

            /* increment table entry index */
            idx++;
        }
        if(FALSE == ret)
        {
            tmp = ((NVM_TableInfo_t){.fields.NvTableMarker = mNvTableMarker}).rawValue;
            /* write table qualifier end, the rest 6 bytes are left 0x00 */
            if(kStatus_HAL_Flash_Success != HAL_FlashProgram(addr, sizeof(NVM_EntryInfo_t), (uint8_t*)(&tmp)))
            {
                ret = TRUE;
            }
        }
        if(FALSE == ret)
        #endif
        {
            /*write page counter bottom*/
            tmp = ((NVM_TableInfo_t){.fields.NvPageCounter = mNvPageCounter}).rawValue;
            if(kStatus_HAL_Flash_Success == HAL_FlashProgram((mNvVirtualPageProperty[pageId].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1U),
                                                sizeof(NVM_TableInfo_t), (uint8_t*)&tmp))
            {
                status = TRUE;
            }
        }
    }
    return status;
}
#if (!defined(gNvLegacyTable_Disabled_d) || (gNvLegacyTable_Disabled_d == 0))
/******************************************************************************
 * Name: UpgradeLegacyTableToFlash
 * Description: Performs to update LegacyTable To Flash
 * Parameter(s): [IN] read_address - read address
 *               [IN] dstPageId - the page ID
 *               [IN] tableData - the table data
 *               [IN] legacy_page_counter - the legacy page counter
 *               [IN] last_record_offset - the last record offset
 * Return: the status of the operation
 *****************************************************************************/
NVM_STATIC NVM_Status_t UpgradeLegacyTableToFlash(uint32_t read_address, NVM_VirtualPageID_t dstPageId,NVM_TableInfo_t tableData, uint32_t legacy_page_counter, uint16_t last_record_offset )
{
    NVM_Status_t status = gNVM_OK_c;

    if (read_address >= mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
    {
        status = gNVM_AddressOutOfRange_c;
    }
    else
    {
        /* copy the data */
    #if (defined(FTFx_PHRASE_SIZE) && (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE))
        if(kStatus_HAL_Flash_Success != HAL_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + last_record_offset,
                                        mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize - last_record_offset - sizeof(legacy_page_counter) - 4,
                                        (uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + last_record_offset))
    #else
        if(kStatus_HAL_Flash_Success != HAL_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + last_record_offset - sizeof(legacy_page_counter),
                                        mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize - last_record_offset - sizeof(legacy_page_counter),
                                        (uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + last_record_offset))
    #endif
        {
            status = gNVM_Error_c;
        }
        else
        {
            /* write page counter, table marker, table version top */
        #if gNvUseExtendedFeatureSet_d
            tableData = ((NVM_TableInfo_t){.fields.NvPageCounter  = legacy_page_counter,
                                           .fields.NvTableMarker  = mNvTableMarker,
                                           .fields.NvTableVersion = 1});
        #else
            tableData = ((NVM_TableInfo_t){.fields.NvPageCounter  = mNvPageCounter});
        #endif

            if(kStatus_HAL_Flash_Success != HAL_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress, sizeof(NVM_TableInfo_t), (uint8_t*)(&tableData)))
            {
                status = gNVM_Error_c;
            }
            else
            {
                /* write page counter bottom */
                tableData = ((NVM_TableInfo_t){.fields.NvPageCounter = legacy_page_counter});
                if(kStatus_HAL_Flash_Success != HAL_FlashProgram((mNvVirtualPageProperty[dstPageId].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1U),
                                                sizeof(NVM_TableInfo_t), (uint8_t*)&tableData))
                {
                    status = gNVM_Error_c;
                }
                else
                {
                    /* erase old page */
                    mNvErasePgCmdStatus.NvPageToErase = mNvActivePageId;
                    mNvErasePgCmdStatus.NvSectorAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;
                    mNvErasePgCmdStatus.NvErasePending = TRUE;
                    /* set new active page */
                    mNvActivePageId = dstPageId;
                    status = gNVM_OK_c;
                }
            }
        }
    }
    return status;
}

/******************************************************************************
 * Name: UpgradeLegacyTable
 * Description: Upgrades an legacy table to the new format
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
NVM_STATIC NVM_Status_t UpgradeLegacyTable(void)
{
    NVM_Status_t status = gNVM_OK_c;
    uint32_t legacy_page_counter;
    uint32_t read_address;
    uint16_t offset;
    uint16_t last_record_offset = 0;
    NVM_RecordMetaInfo_t meta;
    NVM_TableInfo_t tableData = {0};

    #if gNvUseExtendedFeatureSet_d
    uint32_t legacy_table_marker;
    uint32_t value;
    NVM_EntryInfo_t tableEntry;
    bool_t ret = FALSE;
    #endif
    NVM_VirtualPageID_t dstPageId = (NVM_VirtualPageID_t)(uint8_t)(((uint8_t)mNvActivePageId+1U)%2U);

    /* Check if the destination page is blank. If not, erase it. */
    if(gNVM_PageIsNotBlank_c == NvVirtualPageBlankCheck(dstPageId))
    {
        status = NvEraseVirtualPage(dstPageId);
    }
    if(gNVM_OK_c == status)
    {
        /* read legacy page counter */
        read_address = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;
        NV_FlashRead((uint8_t*)read_address, (uint8_t*)&legacy_page_counter, sizeof(legacy_page_counter));
        legacy_page_counter += 1U;
        offset = sizeof(NVM_TableInfo_t);
        read_address += sizeof(legacy_page_counter);
        /* copy the flash table */
#if gNvUseExtendedFeatureSet_d
        #if (defined(FTFx_PHRASE_SIZE) && (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE))
        /* 4 0xFF allignement bytes */
        read_address += 4;
        #endif
        NV_FlashRead((uint8_t*)read_address, (uint8_t*)&legacy_table_marker, sizeof(legacy_table_marker));
        read_address += sizeof(legacy_table_marker);
        #if (defined(FTFx_PHRASE_SIZE) && (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE))
        /* 4 0xFF allignement bytes */
        read_address += 4;
        #endif
        while (read_address < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
        {
            NV_FlashRead((uint8_t*)read_address, (uint8_t*)&value, sizeof(value));
            if (legacy_table_marker == value)
            {
                break;
            }

            NV_FlashRead((uint8_t*)read_address, (uint8_t*)&tableEntry, sizeof(tableEntry));
            /* upgrade entry */
            #if (defined(FTFx_PHRASE_SIZE) && (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE))
            value = tableEntry.fields.NvElementSize;
            tableEntry.fields.NvElementSize = tableEntry.fields.NvDataEntryID;
            tableEntry.fields.NvDataEntryID = value;
            tableEntry.fields.NvElementsCount = tableEntry.fields.NvDataEntryType;
            #else
            value = tableEntry.fields.NvElementsCount;
            tableEntry.fields.NvElementsCount = tableEntry.fields.NvElementSize;
            tableEntry.fields.NvElementSize = (uint16_t)value;
            tableEntry.fields.NvDataEntryID = tableEntry.fields.NvDataEntryType;
            #endif
            tableEntry.fields.NvDataEntryType = (uint16_t)gNVM_MirroredInRam_c;

            #if gUnmirroredFeatureSet_d
            /* search in the ram table for the entry type, if it doesn't then default to mirrored */
            value = NvGetTableEntryIndexFromId(tableEntry.fields.NvDataEntryID);
            if((gNvInvalidTableEntryIndex_c != value) &&
               (pNVM_DataTable[value].ElementsCount == tableEntry.fields.NvElementsCount) &&
               (pNVM_DataTable[value].ElementSize == tableEntry.fields.NvElementSize))
            {
                tableEntry.fields.NvDataEntryType = pNVM_DataTable[value].DataEntryType;
            }
            #endif
            /*write table entries to the destination page*/
            if(kStatus_HAL_Flash_Success != HAL_FlashProgram(mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + offset,
                                                sizeof(NVM_EntryInfo_t), (uint8_t*)&tableEntry))
            {
                status = gNVM_Error_c;
                ret = TRUE;
                break;
            }
            offset += sizeof(NVM_EntryInfo_t);
            read_address += sizeof(NVM_EntryInfo_t);
        }
        if(gNVM_OK_c == status)
        {
            if (read_address >= mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
            {
                status = gNVM_AddressOutOfRange_c;
                ret = TRUE;
            }
            else
            {
                tableData = ((NVM_TableInfo_t){.fields.NvTableMarker = mNvTableMarker});
                /* write table qualifier end, the rest 6 bytes are left 0x00 */
                if(kStatus_HAL_Flash_Success != HAL_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + offset, sizeof(NVM_TableInfo_t), (uint8_t*)(&tableData)))
                {
                    status = gNVM_Error_c;
                    ret = TRUE;
                }
                else
                {
                    read_address += sizeof(legacy_table_marker);
                    offset += sizeof(NVM_TableInfo_t);
                }
            }
        }
        if(FALSE == ret)
#endif
        {
            #if (defined(FTFx_PHRASE_SIZE) && (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE))
            /* 4 0xFF allignement bytes */
            read_address += 4;
            #endif

            /* copy metas */
            while (read_address < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
            {
                NV_FlashRead((uint32_t *)read_address, (uint8_t*)&meta, sizeof(NVM_RecordMetaInfo_t));
                if (gNvGuardValue_c == meta.rawValue)
                {
                    break;
                }
                #if (defined(FTFx_PHRASE_SIZE) && (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE))
                if (0u != meta.fields.NvmRecordOffset)
                {
                    last_record_offset = meta.fields.NvmRecordOffset;
                }
                #else
                /* adjust the meta offset */
                if (0u != meta.fields.NvmRecordOffset)
                {
                    last_record_offset = meta.fields.NvmRecordOffset;
                    meta.fields.NvmRecordOffset -= gNvLegacyOffset_c;
                }
                #endif
                /* write the meta */
                if(kStatus_HAL_Flash_Success != HAL_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + offset,
                                                sizeof(NVM_RecordMetaInfo_t), (uint8_t*)&meta))
                {
                    status = gNVM_Error_c;
                    break;
                }
                offset += sizeof(NVM_RecordMetaInfo_t);
                read_address += sizeof(NVM_RecordMetaInfo_t);
            }

            if(gNVM_OK_c == status)
            {
                status = UpgradeLegacyTableToFlash(read_address,dstPageId, tableData, legacy_page_counter, last_record_offset);
            }
        }
    }
    return status;
}
#endif /* no FlexNVM */
#endif /* gNvLegacyTable_Disabled_d */
/******************************************************************************
 * Name: NvGetEntryFromDataPtr
 * Description: get table and element indexes based on a generic pointer address
 * Parameter(s): [IN] pData - a pointer to a NVM RAM table
 *               [OUT] pIndex - a pointer to a memory location where the
 *                              requested indexed will be stored
 * Return: gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_PointerOutOfRange_c - if the provided pointer cannot be founded
 *                                    within the RAM table
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvGetEntryFromDataPtr
(
    void* pData,
    NVM_TableEntryInfo_t* pIndex
)
{
    uint16_t idx = 0;
    NVM_Status_t status = gNVM_PointerOutOfRange_c;

    while(idx < gNVM_TABLE_entries_c)
    {
        if(((uint8_t*)pData >= (uint8_t*)pNVM_DataTable[idx].pData))
        {
            #if gUnmirroredFeatureSet_d
            if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[idx].DataEntryType)
            {
                if ((uint8_t*)pData < ((uint8_t*)pNVM_DataTable[idx].pData + (sizeof(void*) * pNVM_DataTable[idx].ElementsCount)))
                {
                    int32_t offset = (uint8_t*)pData - (uint8_t*)pNVM_DataTable[idx].pData;
                    pIndex->elementIndex = (uint16_t)offset / sizeof(void*);
                    pIndex->entryId = pNVM_DataTable[idx].DataEntryID;
                    status = gNVM_OK_c;
                    break;
                }
                idx++;
                continue;
            }
            else
            #endif
            if ((uint8_t*)pData < ((uint8_t*)pNVM_DataTable[idx].pData + (pNVM_DataTable[idx].ElementSize * pNVM_DataTable[idx].ElementsCount)))
            {
                int32_t offset = (uint8_t*)pData - (uint8_t*)pNVM_DataTable[idx].pData;
                pIndex->elementIndex = (uint16_t)offset / pNVM_DataTable[idx].ElementSize;
                pIndex->entryId = pNVM_DataTable[idx].DataEntryID;
                status = gNVM_OK_c;
                break;
            }
            else
            {
                /*MISRA rule 15.7*/
            }
        }
        /* increment the loop counter */
        idx++;
    }
    return status;
}
/******************************************************************************
 * Name: NvGetTableEntryIndexFromDataPtr
 * Description: get table and element indexes based on a generic pointer address
 * Parameter(s): [IN] pData - a pointer to a NVM RAM table
 *               [OUT] pIndex - a pointer to a memory location where the
 *                              requested indexed will be stored
 *               [OUT] pTableEntryIdx - a pointer to a memory location where the
 *                              requested TableEntry Idx will be stored
 * Return: gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_PointerOutOfRange_c - if the provided pointer cannot be founded
 *                                    within the RAM table
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvGetTableEntryIndexFromDataPtr
(
    void* pData,
    NVM_TableEntryInfo_t* pIndex,
    uint16_t *pTableEntryIdx
)
{
    NVM_Status_t status = gNVM_InvalidTableEntry_c;
    /* Get entry from the dataPtr */
    status = NvGetEntryFromDataPtr(pData, pIndex);

    if(gNVM_OK_c == status)
    {
        assert(gNvInvalidDataEntry_c != pIndex->entryId);
        if (pTableEntryIdx != NULL)
        {
            /* Get table entry index from id */
            *pTableEntryIdx = NvGetTableEntryIndexFromId(pIndex->entryId);
            assert(gNvInvalidTableEntryIndex_c != *pTableEntryIdx);
        }
     }
    return status;
}
/******************************************************************************
 * Name: NvMetaAndRecordAddressRegulate
 * Description: Performs to regulate
 * Parameter(s): [IN] pageFreeSpace - free space in active page
 *               [IN] totalRecordSize - the size of meta + record
 *               [IN] realRecordSize - the size of record aligned to Flash write size
 *               [IN] metaInfoAddress - the address of meta info will write to
 *               [IN] newRecordAddress - the address of record info will write to
 * Return: the status of the operation
 *****************************************************************************/
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
NVM_STATIC bool_t NvMetaAndRecordAddressRegulate(uint32_t pageFreeSpace, uint32_t totalRecordSize, uint32_t realRecordSize,
                                             uint32_t *metaInfoAddress, uint32_t *newRecordAddress)
{
    NVM_RecordMetaInfo_t metaInfo = {0};
    uint32_t lastRecordAddress;
    bool_t doWrite = FALSE;

    *metaInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress;

    if(gEmptyPageMetaAddress_c == *metaInfoAddress)
    {
        /* empty page, first write */

        /* set new record address */
        *newRecordAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) - realRecordSize + 1U;

        /* gEmptyPageMetaAddress_c is not a valid address and it is used only as an empty page marker;
        * therefore, set the valid value of meta information address */
        *metaInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c;
    }
    else
    {
        /* get the meta information of the last successfully written record */
        #if gUnmirroredFeatureSet_d
            (void)NvGetMetaInfo(mNvActivePageId, mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress, &metaInfo);
        #else
            /* get the last record start address (the address is always aligned) */
            (void)NvGetMetaInfo(mNvActivePageId, *metaInfoAddress, &metaInfo);
        #endif
        lastRecordAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset;
        /* set new record address */
        *newRecordAddress = lastRecordAddress - realRecordSize;

        *metaInfoAddress += sizeof(NVM_RecordMetaInfo_t);
    }

    /* make sure there is at least a free space for a meta between the last one and the data*/
    while(totalRecordSize + sizeof(NVM_RecordMetaInfo_t) < pageFreeSpace)
    {
        /* check if the space for the record is free */
        if ((FALSE == NvIsMemoryAreaAvailable(*newRecordAddress, realRecordSize) && (realRecordSize != 0u)))
        {
            /* the memory space is not blank */
            if (pageFreeSpace < realRecordSize)
            {
                /* I am unable to write the record on this page, trigger copy page */
                break;
            }
            pageFreeSpace -= realRecordSize;
            *newRecordAddress -= realRecordSize;
        }
        /* check if the space for the meta is free */
        else if(!NvIsMemoryAreaAvailable(*metaInfoAddress, sizeof(NVM_RecordMetaInfo_t)))
        {
            /* the memory space is not blank */
            if (pageFreeSpace < realRecordSize)
            {
                /* I am unable to write the meta on this page, trigger copy page */
                break;
            }
            pageFreeSpace -= sizeof(NVM_RecordMetaInfo_t);
            *metaInfoAddress += sizeof(NVM_RecordMetaInfo_t);
        }
        else
        {
            /* the memory space is blank */
            doWrite = TRUE;
            break;
        }
    }

    return doWrite;
}

/******************************************************************************
 * Name: NvWriteRecord
 * Description: writes a record
 * Parameter(s): [IN] tblIndexes - a pointer to table and element indexes
 *               [IN] tableEntryIdx - the table EntryIdx
 *               [IN] metaInfo - the meta infomation
 *               [IN] metaInfoAddress - the address of meta info will write to
 *               [IN] newRecordAddress - the address of record info will write to
  *              [IN] recordSize - the address of record will write to
 *               [IN] mirroredSrcAddress - the mirrored source address of will write to
 * Return: the status of the operation

 *****************************************************************************/
NVM_STATIC NVM_Status_t NvWriteRecordToFlash(NVM_TableEntryInfo_t* tblIndexes,uint16_t tableEntryIdx, NVM_RecordMetaInfo_t metaInfo, uint32_t metaInfoAddress, uint32_t newRecordAddress,uint32_t recordSize,uint32_t mirroredSrcAddress)
{
    NVM_Status_t status = gNVM_OK_c;
    uint32_t srcAddress;

#if gUnmirroredFeatureSet_d
    if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType)
    {
        srcAddress = (uint32_t)(uint8_t*)((uint8_t**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex];
    }
    else
#endif
    {
        srcAddress = mirroredSrcAddress;
    }

#if gUnmirroredFeatureSet_d
        if(0U == srcAddress)
        {
            /* It's an erased unmirrored dataset */
            metaInfo.fields.NvmRecordOffset = 0;
        }
        if(kStatus_HAL_Flash_Success == (srcAddress ? HAL_FlashProgramUnaligned( newRecordAddress, recordSize, (uint8_t*)srcAddress):kStatus_HAL_Flash_Success))
#else
        if(kStatus_HAL_Flash_Success == HAL_FlashProgramUnaligned( newRecordAddress, recordSize, (uint8_t*)srcAddress))
#endif
        {
            /* record successfully written, now write the associated record meta information */
            if(kStatus_HAL_Flash_Success == HAL_FlashProgram( metaInfoAddress, sizeof(NVM_RecordMetaInfo_t), (uint8_t*)(&metaInfo)))
        {
            /* update the last record meta information */
            mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress = metaInfoAddress;
            /* update the last unerased meta info address */
            #if gUnmirroredFeatureSet_d
            if(0u != metaInfo.fields.NvmRecordOffset)
            {
                mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress = metaInfoAddress;
            }
            #endif
            /* Empty macro when nvm monitoring is not enabled */
            #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
            FSCI_MsgNVWriteMonitoring(metaInfo.fields.NvmDataEntryID,tblIndexes->elementIndex,tblIndexes->saveRestoreAll);
            #endif

            #if gUnmirroredFeatureSet_d
            if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType)
            {
                if(0u != metaInfo.fields.NvmRecordOffset)
                {
                    uint8_t* pTempAddress = (uint8_t*)((uint8_t**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex];
                    ((uint8_t**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex] = (uint8_t*)newRecordAddress;
                    (void)MEM_BufferFree(pTempAddress);
                }
            }
            #endif
            status = gNVM_OK_c;
        }
        else
        {
            status = gNVM_MetaInfoWriteError_c;
        }
    }
    else
    {
        status = gNVM_RecordWriteError_c;
    }
    return status;
}
#endif /* gNvUseFlexNVM_d == FALSE */
/******************************************************************************
 * Name: NvWriteRecord
 * Description: writes a record
 * Parameter(s): [IN] tblIndexes - a pointer to table and element indexes
 * Return: gNVM_InvalidPageID_c - if the active page is not valid
 *         gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_MetaInfoWriteError_c - if the meta information couldn't be
 *                                     written
 *         gNVM_RecordWriteError_c - if the record couldn't be written
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvWriteRecord
(
    NVM_TableEntryInfo_t* tblIndexes
)
{
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    uint32_t metaInfoAddress;
    uint32_t newRecordAddress;
    NVM_RecordMetaInfo_t metaInfo;
    uint32_t realRecordSize;
    uint32_t totalRecordSize; /* record + meta */
    uint32_t pageFreeSpace;
    bool_t doWrite;
    uint32_t mirroredSrcAddress;
    uint8_t nvValidationStartByte;
    uint8_t nvValidationEndByte;
    #if gUnmirroredFeatureSet_d
    bool_t ret = FALSE;
    #endif
#else /* FlexNVM */
    uint32_t lastFlexMetaInfoAddress;
    NVM_FlexMetaInfo_t lastFlexMetaInfo;
    NVM_FlexMetaInfo_t flexMetaInfo;
    uint32_t destRecordEndAddress;
#endif
    NVM_Status_t status = gNVM_OK_c;
    uint16_t tableEntryIdx;
    uint32_t recordSize;

    tableEntryIdx = NvGetTableEntryIndexFromId(tblIndexes->entryId);

    if(gNvInvalidTableEntryIndex_c == tableEntryIdx)
    {
        status = gNVM_InvalidTableEntry_c;
    }
    else
    {
#if (gNvUseFlexNVM_d == TRUE) /* FlexNVM */
        recordSize = pNVM_DataTable[tableEntryIdx].ElementsCount * pNVM_DataTable[tableEntryIdx].ElementSize;

        NvGetFlexMetaInfoFromId(tblIndexes->entryId, &flexMetaInfo);

        if(flexMetaInfo.rawValue == gNvFlexGuardValue_c) /* no meta found for this table entry ID */
        {
            /* set entry ID */
            flexMetaInfo.fields.NvDataEntryID = tblIndexes->entryId;
            /* get last meta info tag address */
            lastFlexMetaInfoAddress = NvGetFlexLastMetaInfo();

            if(lastFlexMetaInfoAddress < gFlexNvmDriver.ftfxConfig.flexramBlockBase) /* FlexRAM empty */
            {
                flexMetaInfo.fields.NvDataOffset = gFlexNvmDriver.ftfxConfig.flexramTotalSize - recordSize;
                destRecordEndAddress = gFlexNvmDriver.ftfxConfig.flexramBlockBase + gFlexNvmDriver.ftfxConfig.flexramTotalSize;
                lastFlexMetaInfoAddress = gFlexNvmDriver.ftfxConfig.flexramBlockBase;
            }
            else
            {
                #if FSL_FEATURE_FLASH_IS_FTFL
                /* wait for EEPROM system to be ready */
                while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
                #endif
                /* read last meta tag */
                NV_FlashRead(lastFlexMetaInfoAddress, (uint8_t*)&lastFlexMetaInfo, sizeof(NVM_FlexMetaInfo_t));
                /* compute record destination end address */
                destRecordEndAddress = gFlexNvmDriver.ftfxConfig.flexramBlockBase + lastFlexMetaInfo.fields.NvDataOffset;
                /* compute record offset */
                flexMetaInfo.fields.NvDataOffset = lastFlexMetaInfo.fields.NvDataOffset - recordSize;
                /* increment the last meta info address and reused it as address of the current meta info tag */
                lastFlexMetaInfoAddress += sizeof(NVM_FlexMetaInfo_t);
            }

            #if FSL_FEATURE_FLASH_IS_FTFL
            /* wait for EEPROM system to be ready */
            while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));

            /* write record */
            if(kStatus_FLASH_Success != EEEWrite(&gFlexNvmDriver, destRecordEndAddress - recordSize, recordSize, ((uint8_t*)(pNVM_DataTable[tableEntryIdx].pData))))
            {
                status = gNVM_RecordWriteError_c;
            }
            else
            {
                /* wait for EEPROM system to be ready */
                while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));

                /* write meta */
                if(kStatus_FLASH_Success != EEEWrite(&gFlexNvmDriver, lastFlexMetaInfoAddress, sizeof(NVM_FlexMetaInfo_t), (uint8_t *)(&flexMetaInfo.rawValue)))
                {
                    status = gNVM_RecordWriteError_c;
                }
            }
            #endif /* #if FSL_FEATURE_FLASH_IS_FTFL */

            #if FSL_FEATURE_FLASH_IS_FTFE
            /* write record */
            if(kStatus_FLASH_Success != FLEXNVM_EepromWrite(&gFlexNvmDriver, destRecordEndAddress - recordSize, ((uint8_t*)(pNVM_DataTable[tableEntryIdx].pData)), recordSize))
            {
                status = gNVM_RecordWriteError_c;
            }
            else
            {
                /* write meta */
                if(kStatus_FLASH_Success != FLEXNVM_EepromWrite(&gFlexNvmDriver, lastFlexMetaInfoAddress, (uint8_t *)(&flexMetaInfo.rawValue), sizeof(NVM_FlexMetaInfo_t)))
                {
                    status = gNVM_RecordWriteError_c;
                }
            }
            #endif /* #if FSL_FEATURE_FLASH_IS_FTFE */
        }
        else /* table entry ID already in FlexRAM, update the corresponding record */
        {
            #if FSL_FEATURE_FLASH_IS_FTFL
            /* wait for EEPROM system to be ready */
            while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));

            if(kStatus_FLASH_Success != EEEWrite(&gFlexNvmDriver, (uint32_t)(gFlexNvmDriver.ftfxConfig.flexramBlockBase + flexMetaInfo.fields.NvDataOffset), recordSize,
                                   ((uint8_t*)(pNVM_DataTable[tableEntryIdx].pData))))
            {
                status = gNVM_RecordWriteError_c;
            }
            #endif /* FSL_FEATURE_FLASH_IS_FTFL */

            #if FSL_FEATURE_FLASH_IS_FTFE
            if(kStatus_FLASH_Success != FLEXNVM_EepromWrite(&gFlexNvmDriver, (uint32_t)(gFlexNvmDriver.ftfxConfig.flexramBlockBase + flexMetaInfo.fields.NvDataOffset),
                              ((uint8_t*)(pNVM_DataTable[tableEntryIdx].pData)), recordSize))
            {
                status = gNVM_RecordWriteError_c;
            }
            #endif /* FSL_FEATURE_FLASH_IS_FTFE */
        }
        /* Empty macro when nvm monitoring is not enabled */
        #if (gFsciIncluded_c && (gNvmEnableFSCIRequests_c || gNvmEnableFSCIMonitoring_c))
        if(gNVM_OK_c == status)
        {
            FSCI_MsgNVWriteMonitoring(flexMetaInfo.fields.NvDataEntryID,tblIndexes->elementIndex,tblIndexes->saveRestoreAll);
        }
        #endif
#else /* No FlexNVM */
        /* make sure i don't process the save if page copy is active */
        if (mNvCopyOperationIsPending)
        {
            status = gNVM_PageCopyPending_c;
        }
        else
        {
            #if gUnmirroredFeatureSet_d
            /* For data sets not mirrored in ram a table entry is saved separate */
            if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType)
            {
                tblIndexes->saveRestoreAll = FALSE;
            }
            #endif

            if(tblIndexes->saveRestoreAll)
            {
                recordSize = (uint32_t)pNVM_DataTable[tableEntryIdx].ElementSize * pNVM_DataTable[tableEntryIdx].ElementsCount;
                nvValidationStartByte = gValidationByteAllRecords_c;
                nvValidationEndByte = gValidationByteAllRecords_c;
                mirroredSrcAddress = (uint32_t)((uint8_t*)(((uint8_t*)(pNVM_DataTable[tableEntryIdx]).pData)));
            }
            else
            {
                recordSize = pNVM_DataTable[tableEntryIdx].ElementSize;
                nvValidationStartByte = gValidationByteSingleRecord_c;
                nvValidationEndByte = gValidationByteSingleRecord_c;
                mirroredSrcAddress = (uint32_t)((uint8_t*)(((uint8_t*)(pNVM_DataTable[tableEntryIdx]).pData)) + (tblIndexes->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize));
            }
            realRecordSize = recordSize;

            #if gUnmirroredFeatureSet_d
            /* Check if is an erase for unmirrored dataset*/
            if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType)
            {
                if(NULL == ((void**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex])
                {
                    recordSize = 0;
                    realRecordSize = 0;
                }
                /*if the dataset is allready in flash, ignore it*/
                else if(NvIsNVMFlashAddress(((void**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex]))
                {
                    /*it returns OK, because atomic save must not fail, this is not an error*/
                    status = gNVM_OK_c;
                    ret = TRUE;
                }
                else
                {
                    /*MISRA rule 15.7*/
                }
            }
            if(FALSE == ret)
            #endif
            {
                /* get active page free space */
                (void)NvGetPageFreeSpace(&pageFreeSpace);

                /* compute the 'real record size' taking into consideration that the FTFL controller only writes in burst of 4 bytes */
                realRecordSize = NvUpdateSize(realRecordSize);

                /* compute the total size (record + meta info) */
                totalRecordSize = realRecordSize + sizeof(NVM_RecordMetaInfo_t);

                /* check if the record fits the page's free space.
                * one extra meta info space must be kept always free, to be able to perform the meta info search */
                if(totalRecordSize + sizeof(NVM_RecordMetaInfo_t) > pageFreeSpace)
                {
                    /* there is no space to save the record, try to copy the current active page latest records
                    * to the other page
                    */
                    mNvCopyOperationIsPending = TRUE;
                    status = gNVM_PageCopyPending_c;
                }
                else
                {
                    /* check if the space needed by the record is really free (erased).
                    * this check is necessary because it may happens that a record to be successfully written,
                    * but the system fails (e.g. POR) before the associated meta information has been written.
                    * the theoretically free space is computed as the difference between the last meta info
                    * address and the start address of the last successfully written record. This information
                    * is valuable but may not reflect the reality, as mentioned in the explanation above */
                    doWrite = NvMetaAndRecordAddressRegulate(pageFreeSpace, totalRecordSize, realRecordSize, &metaInfoAddress, &newRecordAddress);

                    /* set associated meta info */
                    metaInfo.fields.NvValidationStartByte = nvValidationStartByte;
                    metaInfo.fields.NvValidationEndByte = nvValidationEndByte;
                    metaInfo.fields.NvmDataEntryID = pNVM_DataTable[tableEntryIdx].DataEntryID;
                    metaInfo.fields.NvmElementIndex = tblIndexes->elementIndex;
                    metaInfo.fields.NvmRecordOffset = (uint16_t)(newRecordAddress - mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress);

                    /* the offset has to be 4 bytes aligned, an extra check is performed to avoid further hard faults caused by FTFx controller */
                    if((metaInfo.fields.NvmRecordOffset & (uint16_t)0x3) != 0u)
                    {
                        status = gNVM_AlignamentError_c;
                    }
                    else
                    {
                        /* Write the record and associated meta information */
                        if(FALSE == doWrite)
                        {
                            /* there is no space to save the record, try to copy the current active page latest records
                            * to the other page
                            */
                            mNvCopyOperationIsPending = TRUE;
                            status = gNVM_PageCopyPending_c;
                        }
                        else
                        {
                            status = NvWriteRecordToFlash( tblIndexes, tableEntryIdx,  metaInfo,  metaInfoAddress,  newRecordAddress, recordSize, mirroredSrcAddress);
                        }
                    }
                }
            }
        }
#endif /* gNvUseFlexNVM_d */
    }

    return status;

}

/******************************************************************************
 * Name: NvRestoreData
 * Description: restore an element from NVM storage to its original RAM location
 * Parameter(s): [IN] tblIdx - pointer to table and element indexes
 * Return: gNVM_NullPointer_c - if the provided pointer is NULL
 *         gNVM_PageIsEmpty_c - if page is empty
 *         gNVM_Error_c - in case of error(s)
 *         gNVM_OK_c - if the operation completed successfully
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvRestoreData
(
    NVM_TableEntryInfo_t* tblIdx
)
{
    NVM_Status_t status = gNVM_MetaNotFound_c;
    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    NVM_RecordMetaInfo_t metaInfo = {0};
    uint32_t metaInfoAddress;
    #if gNvFragmentation_Enabled_d
    uint16_t cnt;
    #endif
    #else
    NVM_FlexMetaInfo_t flexMetaInfo;
    uint32_t EERamAddress;
    #endif

    uint16_t tableEntryIdx;

    tableEntryIdx = NvGetTableEntryIndexFromId(tblIdx->entryId);

    if(gNvInvalidTableEntryIndex_c == tableEntryIdx)
    {
        status = gNVM_InvalidTableEntry_c;
    }
    else
    {
#if (gNvUseFlexNVM_d == TRUE) /* FlexNVM */
        /* restore data from EERAM */
        EERamAddress = gFlexNvmDriver.ftfxConfig.flexramBlockBase;

        do
        {
            #if FSL_FEATURE_FLASH_IS_FTFL
            /* wait for EEPROM system to be ready */
            while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
            #endif
            /* read meta info tag */
            NV_FlashRead(EERamAddress, (uint8_t*)&flexMetaInfo, sizeof(flexMetaInfo));

            if(flexMetaInfo.rawValue == gNvFlexGuardValue_c) /* end of meta info space */
            {
                break;
            }

            if(tblIdx->entryId == flexMetaInfo.fields.NvDataEntryID)
            {
                if(tblIdx->saveRestoreAll)
                {
                    #if FSL_FEATURE_FLASH_IS_FTFL
                    /* wait for EEPROM system to be ready */
                    while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
                    #endif
                    /* read all elements */
                    NV_FlashRead((gFlexNvmDriver.ftfxConfig.flexramBlockBase + flexMetaInfo.fields.NvDataOffset),
                                 (uint8_t*)(pNVM_DataTable[tableEntryIdx].pData),
                                 pNVM_DataTable[tableEntryIdx].ElementSize * pNVM_DataTable[tableEntryIdx].ElementsCount);
                    status = gNVM_OK_c;
                    break;
                }
                else
                {
                    #if FSL_FEATURE_FLASH_IS_FTFL
                    /* wait for EEPROM system to be ready */
                    while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
                    #endif
                    /* read element */
                    NV_FlashRead(gFlexNvmDriver.ftfxConfig.flexramBlockBase + flexMetaInfo.fields.NvDataOffset + (tblIdx->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize),
                                 (uint8_t*)(((uint8_t*)pNVM_DataTable[tableEntryIdx].pData) + (tblIdx->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize)),
                                 pNVM_DataTable[tableEntryIdx].ElementSize);
                    status = gNVM_OK_c;
                    break;
                }
            }

            /* go to next meta tag */
            EERamAddress += sizeof(flexMetaInfo);

    #if FSL_FEATURE_FLASH_IS_FTFL
        } while(EERamAddress < (gFlexNvmDriver.ftfxConfig.flexramBlockBase + gFlexNvmDriver.ftfxConfig.flexramTotalSize));
    #endif

    #if FSL_FEATURE_FLASH_IS_FTFE
        } while(EERamAddress < (gFlexNvmDriver.ftfxConfig.flexramBlockBase + gFlexNvmDriver.ftfxConfig.flexramTotalSize));
    #endif
#else /* FlexNVM */
        /* get the last meta information address */
        metaInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress;
        if(metaInfoAddress == gEmptyPageMetaAddress_c)
        {
            /* blank page, no data to restore */
            status = gNVM_PageIsEmpty_c;
        }
        else
        {
            if(tblIdx->entryId == gNvInvalidDataEntry_c)
            {
                /* invalid table entry */
                status = gNVM_InvalidTableEntry_c;
            }
            else
            {
                /*
                * If the meta info is found, the associated record is restored,
                * otherwise the gNVM_MetaNotFound_c will be returned
                */
                status = gNVM_MetaNotFound_c;
                #if gNvFragmentation_Enabled_d
                /* clear the buffer */
                FLib_MemSet(maNvRecordsCpyOffsets, 0, (uint32_t)sizeof(uint16_t)*pNVM_DataTable[tableEntryIdx].ElementsCount);
                #endif
                /* parse meta info backwards */
                while(metaInfoAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
                {
                    /* get the meta information */
                    (void)NvGetMetaInfo(mNvActivePageId, metaInfoAddress, &metaInfo);

                    if(metaInfo.fields.NvValidationStartByte != metaInfo.fields.NvValidationEndByte)
                    {
                        /* invalid meta info, move to the previous meta info */
                        metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
                        continue;
                    }

                    if(metaInfo.fields.NvmDataEntryID == tblIdx->entryId)
                    {
                        if(tblIdx->saveRestoreAll)
                        {
                            #if gNvFragmentation_Enabled_d
                            /* single save found */
                            if ((metaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c) &&
                                (0u == maNvRecordsCpyOffsets[metaInfo.fields.NvmElementIndex]))
                            {
                                maNvRecordsCpyOffsets[metaInfo.fields.NvmElementIndex] = 1;
                                NV_FlashRead((uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset,
                                             (uint8_t*)pNVM_DataTable[tableEntryIdx].pData + metaInfo.fields.NvmElementIndex * pNVM_DataTable[tableEntryIdx].ElementSize,
                                             pNVM_DataTable[tableEntryIdx].ElementSize);
                                status = gNVM_OK_c;
                            }
                            /* full save found */
                            else if (metaInfo.fields.NvValidationStartByte == gValidationByteAllRecords_c)
                            {
                                for (cnt=0; cnt<pNVM_DataTable[tableEntryIdx].ElementsCount; cnt++)
                                {
                                    /* skip allready restored elements */
                                    if (1U == maNvRecordsCpyOffsets[cnt])
                                    {
                                        continue;
                                    }
                                    NV_FlashRead((uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset + cnt * pNVM_DataTable[tableEntryIdx].ElementSize,
                                                 (uint8_t*)pNVM_DataTable[tableEntryIdx].pData + cnt * pNVM_DataTable[tableEntryIdx].ElementSize,
                                                 pNVM_DataTable[tableEntryIdx].ElementSize);
                                }
                                status = gNVM_OK_c;
                                break;
                            }
                            else
                            {
                                /*MISRA rule 15.7*/
                            }
                            #else
                             /* single saves are not allowed if fragmentation is off */
                            if(metaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c)
                            {
                                status = gNVM_FragmentatedEntry_c;
                                break;
                            }

                            NV_FlashRead(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset,
                                         (uint8_t*)pNVM_DataTable[tableEntryIdx].pData,
                                         (uint32_t)pNVM_DataTable[tableEntryIdx].ElementsCount * (uint32_t)pNVM_DataTable[tableEntryIdx].ElementSize);
                            status = gNVM_OK_c;
                            break;
                            #endif
                        }
                        else
                        {
                            if(metaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c && metaInfo.fields.NvmElementIndex == tblIdx->elementIndex)
                            {
                                #if gUnmirroredFeatureSet_d
                                if(gNVM_MirroredInRam_c != (NVM_DataEntryType_t)pNVM_DataTable[tableEntryIdx].DataEntryType)
                                {
                                    if( 0u == metaInfo.fields.NvmRecordOffset)
                                    {
                                        ((uint8_t**)pNVM_DataTable[tableEntryIdx].pData)[tblIdx->elementIndex]=NULL;
                                    }
                                    else
                                    {
                                        ((uint8_t**)pNVM_DataTable[tableEntryIdx].pData)[tblIdx->elementIndex] =
                                            (uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset;
                                    }
                                    status = gNVM_OK_c;
                                    break;
                                }
                                else
                                #endif
                                {
                                    /* restore the element */
                                    NV_FlashRead((uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset,
                                                 (uint8_t*)((uint8_t*)pNVM_DataTable[tableEntryIdx].pData +
                                                            (metaInfo.fields.NvmElementIndex * pNVM_DataTable[tableEntryIdx].ElementSize)),
                                                 pNVM_DataTable[tableEntryIdx].ElementSize);
                                    status = gNVM_OK_c;
                                    break;
                                }
                            }

                            if(metaInfo.fields.NvValidationStartByte == gValidationByteAllRecords_c)
                            {
                                /* restore the single element from the entire table entry record */
                                NV_FlashRead((uint8_t*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset +
                                              (tblIdx->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize)),
                                ((uint8_t*)pNVM_DataTable[tableEntryIdx].pData + (tblIdx->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize)),
                                pNVM_DataTable[tableEntryIdx].ElementSize);
                                status = gNVM_OK_c;
                                break;
                            }
                        }
                    }
                    /* move to the previous meta info */
                    metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
                }
            }
        }
#endif /* gNvUseFlexNVM_d */
    }
    return status;
}

/******************************************************************************
 * Name: NvGetTableEntryIndex
 * Description: get the table entry index from the provided ID
 * Parameter(s): [IN] entryId - the ID of the table entry
 * Return: table entry index of gNvInvalidTableEntryIndex_c
 *****************************************************************************/
NVM_STATIC uint16_t NvGetTableEntryIndexFromId
(
    NvTableEntryId_t entryId
)
{
    uint16_t loopCnt = 0;

    while(loopCnt < gNVM_TABLE_entries_c)
    {
        if(pNVM_DataTable[loopCnt].DataEntryID == entryId)
        {
            break;
        }
        /* increment the loop counter */
        loopCnt++;
    }
    if(gNVM_TABLE_entries_c == loopCnt)
    {
        loopCnt = gNvInvalidTableEntryIndex_c;
    }
    return loopCnt;
}

#if !gFifoOverwriteEnabled_c
/******************************************************************************
 * Name: NvProcessFirstSaveInQueue
 * Description: processes the first save in the queue so that the queue can accept another entry
 * Parameter(s): -
 * Return: TRUE if a save has been processed, ELSE otherwise
 *****************************************************************************/
NVM_STATIC NVM_Status_t NvProcessFirstSaveInQueue
(
    NVM_TableEntryInfo_t* ptrTblIdx
)
{
    NVM_TableEntryInfo_t tblIdx;
    NVM_Status_t status = gNVM_OK_c;

    if ( 0U == mNvCriticalSectionFlag )
    {
        if (NvGetPendingSavesCount(&mNvPendingSavesQueue) != 0u)
        {
            while (NvPopPendingSave(&mNvPendingSavesQueue, &tblIdx))
            {
                /* save tblIdx */
                *ptrTblIdx = tblIdx;
                if ((gNvCopyAll_c == tblIdx.entryId) && (gNvCopyAll_c == tblIdx.elementIndex) && (TRUE == tblIdx.saveRestoreAll))
                {
                    status = gNVM_AtomicSaveRecursive_c;
                    break;
                }
                else if (gNvInvalidDataEntry_c == tblIdx.entryId)
                {
                    continue;
                }
                else
                {
                    /*MISRA rule 15.7*/
                }

                #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
                if(NvWriteRecord(&tblIdx) == gNVM_PageCopyPending_c)
                {
                    #if (((defined(gFsciIncluded_c)) && (gFsciIncluded_c > 0U)) && ((defined(gNvmEnableFSCIMonitoring_c)) && (gNvmEnableFSCIMonitoring_c > 0U)))
                    FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
                    FSCI_MsgNVVirtualPageMonitoring(FALSE,status = NvCopyPage(gNvCopyAll_c));
                    #else
                    status = NvCopyPage(gNvCopyAll_c);
                    #endif
                    if (gNVM_OK_c == status)
                    {
                        mNvCopyOperationIsPending = FALSE;
                    }
                    if (gNVM_OK_c == NvWriteRecord(&tblIdx))
                    {
                        status = gNVM_OK_c;
                        break;
                    }
                    else
                    {
                        /* return gNVM_SaveRequestRecursive_c to run again */
                        status = gNVM_SaveRequestRecursive_c;
                        break;
                    }
                }
                else
                {
                    status = gNVM_OK_c;
                    break;
                }
                #else /* FlexNVM */
                if (gNVM_OK_c == NvWriteRecord(&tblIdx))
                {
                    status = gNVM_OK_c;
                    break;
                }
                else
                {
                    /* return gNVM_SaveRequestRecursive_c to run again */
                    status = gNVM_SaveRequestRecursive_c;
                    break;
                }
                #endif
            }
        }
    }
    return status;
}
#endif

/******************************************************************************
 * Name: NvAddSaveRequestToQueue
 * Description: Add save request to save requests queue; if the request is
 *              already stored, ignore the current request
 * Parameter(s): [IN] ptrTblIdx - pointer to table index
 * Return: gNVM_OK_c - if operation completed successfully
 *         gNVM_SaveRequestRejected_c - if the request couldn't be queued
 ******************************************************************************/
NVM_STATIC NVM_Status_t NvAddSaveRequestToQueue
(
    NVM_TableEntryInfo_t* ptrTblIdx
)
{
    uint8_t loopIdx;
    bool_t  isQueued = FALSE;
    bool_t  isInvalidEntry = FALSE;
    uint8_t lastInvalidIdx = 0;
    uint8_t remaining_count;
    NVM_Status_t status = gNVM_OK_c;
    NVM_TableEntryInfo_t nvTblIdx = *ptrTblIdx;
    NVM_TableEntryInfo_t preNvTblIdx;

    do
    {
        if(mNvPendingSavesQueue.EntriesCount == 0u)
        {
            /* add request to queue */
            if(FALSE == NvPushPendingSave(&mNvPendingSavesQueue, nvTblIdx))
            {
                status = gNVM_SaveRequestRejected_c;
            }
        }
        else
        {
            /* start from the queue's head */
            loopIdx = (uint8_t)mNvPendingSavesQueue.Head;

            remaining_count = (uint8_t)mNvPendingSavesQueue.EntriesCount;
            /* check if the request is not already stored in queue */
            while(remaining_count != 0u)
            {
                if(nvTblIdx.entryId == mNvPendingSavesQueue.QData[loopIdx].entryId)
                {
                    if(mNvPendingSavesQueue.QData[loopIdx].saveRestoreAll == TRUE) /* full table entry already queued */
                    {
                        /* request is already queued */
                        isQueued = TRUE;
                        break;
                    }

                    /* single element from table entry is queued */
                    if(nvTblIdx.saveRestoreAll == TRUE) /* a full table entry is requested to be saved */
                    {
                        /* update only the flag of the already queued request */
                        mNvPendingSavesQueue.QData[loopIdx].saveRestoreAll = TRUE;
                        /* request is already queued */
                        isQueued = TRUE;
                        break;
                    }

                    /* The request is for a single element and the queued request is also for a single element;
                    * Check if the request is for the same element. If the request is for a different element,
                    * add the new request to queue.
                    */
                    if(nvTblIdx.elementIndex == mNvPendingSavesQueue.QData[loopIdx].elementIndex)
                    {
                        /* request is already queued */
                        isQueued = TRUE;
                        break;
                    }

                }
                /* Check if in the queue is an invalid entryId that can be used*/
                if((gNvInvalidDataEntry_c == mNvPendingSavesQueue.QData[loopIdx].entryId)&&
                   (isInvalidEntry == FALSE))
                {
                    isInvalidEntry = TRUE;
                    lastInvalidIdx = loopIdx;
                }
                remaining_count--;
                /* increment and wrap the loop index */
                if(++loopIdx >= (uint8_t)gNvPendingSavesQueueSize_c)
                {
                    loopIdx=0;
                }
            }

            if(!isQueued)
            {
                /* Reuse an invalid entry from the queue*/
                if(TRUE == isInvalidEntry)
                {
                    mNvPendingSavesQueue.QData[lastInvalidIdx] = nvTblIdx;
                }
                else
                {
                    /* push the request to save operation pending queue */
                    if(!NvPushPendingSave(&mNvPendingSavesQueue, nvTblIdx))
                    {
                        preNvTblIdx = nvTblIdx;
                        /* free a space */
                        status =  NvProcessFirstSaveInQueue(&nvTblIdx);
                        if(!NvPushPendingSave(&mNvPendingSavesQueue, preNvTblIdx))
                        {
                            status = gNVM_SaveRequestRejected_c;
                        }
                    }
                }
            }
        }
    }while(status == gNVM_SaveRequestRecursive_c);
    return status;
}

/******************************************************************************
 * Name: GetRandomRange
 * Description: Returns a random number between 'low' and 'high'
 * Parameter(s): [IN] low, high - generated number range
 * Return: 0..255
 ******************************************************************************/
NVM_STATIC uint8_t GetRandomRange
(
    uint8_t low,
    uint8_t high
)
{
    uint32_t random;
    uint8_t status;
    RNG_GetRandomNo(&random);

    if(high <= low)
    {
        status = low;
    }
    else
    {
        status = (uint8_t)(low + (random % ((uint32_t)high - low + 1u)));
    }
    return status;
}

/******************************************************************************
 * Name: __NvShutdown
 * Description: The function waits for all idle saves to be processed.
 * Parameter(s):  -
 * Return: -
 *****************************************************************************/
NVM_STATIC void __NvShutdown( void )
{
    uint16_t idx = 0;
    /* wait for all operations to complete */
    while(TRUE)
    {
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
        if (((NvGetPendingSavesCount(&mNvPendingSavesQueue)) != 0u) || (mNvCopyOperationIsPending))
#else
        if (NvGetPendingSavesCount(&mNvPendingSavesQueue) != 0u)
#endif
        {
            continue;
        }
        while(idx < gNVM_TABLE_entries_c)
        {
            if (maDatasetInfo[idx].saveNextInterval)
            {
                continue;
            }
            idx++;
        }
        break;
    }
}

/******************************************************************************
 * Name: NvCompletePendingOperationsUnsafe
 * Description: The function attemps to complete all the NVM related pending
 *              operations.
 * Parameter(s):  -
 * Return: -
 *****************************************************************************/
void NvCompletePendingOperationsUnsafe(void)
{
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    uint16_t idx = 0;

    if(TRUE == mNvModuleInitialized)
    {
        while(idx < gNVM_TABLE_entries_c)
        {
            if((maDatasetInfo[idx].saveNextInterval) && (maDatasetInfo[idx].ticksToNextSave != 0u))
            {
                maDatasetInfo[idx].ticksToNextSave = 0;
                mNvSaveOnIntervalEvent = TRUE;
            }
            idx++;
        }

        do
        {
            __NvIdle();
        } while((mNvErasePgCmdStatus.NvErasePending == TRUE) || (mNvCopyOperationIsPending == TRUE) || (mNvPendingSavesQueue.EntriesCount != 0u));
    }
#else /* FlexNVM */
    do
    {
        __NvIdle();
    } while(mNvPendingSavesQueue.EntriesCount != 0U);
#endif
}

#if (gNvUseFlexNVM_d == TRUE) /* FlexNVM */
/******************************************************************************
 * Name: NvGetFlexLastMetaInfo
 * Description: Get FlexRAM last meta information address
 * Parameter(s): -
 * Return: the address of the last valid meta information
 ******************************************************************************/
NVM_STATIC uint32_t NvGetFlexLastMetaInfo
(
    void
)
{
    uint32_t address, size;
    NVM_FlexMetaInfo_t flexMetaInfo;

#if FSL_FEATURE_FLASH_IS_FTFL
    address = gFlexNvmDriver.ftfxConfig.flexramBlockBase;
    size = gFlexNvmDriver.ftfxConfig.flexramTotalSize;
#endif

#if FSL_FEATURE_FLASH_IS_FTFE
    address = gFlexNvmDriver.ftfxConfig.flexramBlockBase;
    size = gFlexNvmDriver.ftfxConfig.flexramTotalSize;
#endif

    while(size)
    {
#if FSL_FEATURE_FLASH_IS_FTFL
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
#endif
        /* read meta info tag */
        NV_FlashRead(address, (uint8_t*)&flexMetaInfo, sizeof(flexMetaInfo));
        if(flexMetaInfo.rawValue == gNvFlexGuardValue_c)
        {
            break;
        }
        address += sizeof(flexMetaInfo);
        size -= sizeof(flexMetaInfo);
    }
    return address - sizeof(flexMetaInfo);
}

/******************************************************************************
 * Name: NvGetFlexMetaInfoFromId
 * Description: Get FlexRAM meta information tag from table entry ID
 * Parameter(s): [IN] tblEntryId - table entry ID
 *               [OUT] pMetaInfo - a pointer to a memory location where the
 *                                 meta information tag will be stored
 * Return: -
 ******************************************************************************/
NVM_STATIC void NvGetFlexMetaInfoFromId
(
    NvTableEntryId_t tblEntryId,
    NVM_FlexMetaInfo_t* pMetaInfo
)
{
    uint32_t address, size;
    NVM_FlexMetaInfo_t flexMetaInfo;
    bool ret = FALSE;

#if FSL_FEATURE_FLASH_IS_FTFL
    address = gFlexNvmDriver.ftfxConfig.flexramBlockBase;
    size = gFlexNvmDriver.ftfxConfig.flexramTotalSize;
#endif

#if FSL_FEATURE_FLASH_IS_FTFE
    address = gFlexNvmDriver.ftfxConfig.flexramBlockBase;
    size = gFlexNvmDriver.ftfxConfig.flexramTotalSize;
#endif

    while(size)
    {
#if FSL_FEATURE_FLASH_IS_FTFL
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlexNvmDriver.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
#endif

        NV_FlashRead(address, (uint8_t*)&flexMetaInfo, sizeof(flexMetaInfo));
        if(flexMetaInfo.rawValue == gNvFlexGuardValue_c)
        {
            break;
        }

        if(flexMetaInfo.fields.NvDataEntryID == tblEntryId)
        {
            pMetaInfo->fields.NvDataEntryID = flexMetaInfo.fields.NvDataEntryID;
            pMetaInfo->fields.NvDataOffset = flexMetaInfo.fields.NvDataOffset;
            ret = TRUE;
            break;
        }

        address += sizeof(flexMetaInfo);
        size -= sizeof(flexMetaInfo);
    }

    if(FALSE == ret)
    {
        pMetaInfo->rawValue = gNvFlexGuardValue_c;
    }
}

/******************************************************************************
 * Name: NvCheckNvmTableForFlexRAMUsage
 * Description: Check if the existing NVM table fits within the FlexRAM window
 * Parameter(s): -
 * Return: gNVM_NvTableExceedFlexRAMSize_c - the table exceed the size of
 *                                           FlexRAM window
 *         gNVM_OK_c - the table fits within the size of window FlexRAM window
 ******************************************************************************/
NVM_STATIC NVM_Status_t NvCheckNvmTableForFlexRAMUsage
(
    void
)
{
    uint16_t loopCnt = 0;
    uint32_t allDatasetSize = 0;
    NVM_Status_t status = gNVM_OK_c;

    while(loopCnt < gNVM_TABLE_entries_c)
    {
        if(gNvInvalidDataEntry_c == pNVM_DataTable[loopCnt].DataEntryID)
        {
            loopCnt++;
            continue;
        }
        /* add the record size */
        allDatasetSize += (pNVM_DataTable[loopCnt].ElementsCount * pNVM_DataTable[loopCnt].ElementSize);
        /* add the meta size */
        allDatasetSize += sizeof(NVM_FlexMetaInfo_t);
        /* increment the loop counter */
        loopCnt++;
    }

    /* add the safe guard space (equal to meta size) */
    allDatasetSize += sizeof(NVM_FlexMetaInfo_t);

#if FSL_FEATURE_FLASH_IS_FTFL
    if(allDatasetSize > gFlexNvmDriver.ftfxConfig.flexramTotalSize)
#endif

#if FSL_FEATURE_FLASH_IS_FTFE
    if(allDatasetSize > gFlexNvmDriver.ftfxConfig.flexramTotalSize)
#endif
    {
        status = gNVM_NvTableExceedFlexRAMSize_c;
    }

    return status;
}

#endif /* gNvUseFlexNVM_d */

#endif /* gNvStorageIncluded_d */

/*****************************************************************************
 *****************************************************************************
 * Public functions
 *****************************************************************************
 *****************************************************************************/

/******************************************************************************
 * Name: NvModuleInit
 * Description: Initialize the NV storage module
 * Parameter(s): -
 * Return: gNVM_ModuleAlreadyInitialized_c - if the module is already
 *                                           initialized
 *         gNVM_InvalidSectorsCount_c - if the sector count configured in the
 *                                      project linker file is invalid
 *         gNVM_MetaNotFound_c - if no meta information was found
 *         gNVM_OK_c - module was successfully initialized
 *         gNVM_CannotCreateMutex_c - no mutex available
 *****************************************************************************/
NVM_Status_t NvModuleInit
(
    void
)
{
#if gNvStorageIncluded_d
    NVM_Status_t status = gNVM_OK_c;
     if(mNvModuleInitialized)
    {
        status = gNVM_ModuleAlreadyInitialized_c;
    }
    else
    {
        status = __NvModuleInit(TRUE);
    }
    if(gNVM_OK_c == status)
    {
      (void)OSA_MutexCreate(mNVMMutexId);
      if( NULL == mNVMMutexId)
      {
          mNvModuleInitialized = FALSE;
          status = gNVM_CannotCreateMutex_c;
      }
    }
    return status;
#else
    return gNVM_Error_c;
#endif /* #if gNvStorageIncluded_d */
}

/******************************************************************************
 * Name: NvMoveToRam
 * Description: Move from NVM to Ram an unmirrored dataset
 * Parameter(s):  ppData     double pointer to the entity to be moved from flash to RAM
 * Return: gNVM_OK_c - if operation completed successfully
 *         gNVM_NoMemory_c - in case there is not a memory block free
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 *****************************************************************************/

NVM_Status_t NvMoveToRam
(
    void** ppData
)
{
#if gNvStorageIncluded_d && gUnmirroredFeatureSet_d
    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvmMoveToRam(ppData);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    ppData=ppData;
    return gNVM_Error_c;
#endif
}

/******************************************************************************
 * Name: NvErase
 * Description: Erase from NVM an unmirrored dataset
 * Parameter(s):  ppData     double pointer to the entity to be moved from flash to RAM
 * Return: gNVM_OK_c - if operation completed successfully
 *         gNVM_NoMemory_c - in case there is not a memory block free
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 *****************************************************************************/

NVM_Status_t NvErase
(
    void** ppData
)
{
#if gNvStorageIncluded_d && gUnmirroredFeatureSet_d
    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvmErase(ppData);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    ppData=ppData;
    return gNVM_Error_c;
#endif
}

/******************************************************************************
 * Name: NvSaveOnIdle
 * Description: Save the data pointed by ptrData on the next call to NvIdle()
 * Parameter(s): [IN] ptrData - pointer to data to be saved
 *               [IN] saveRestoreAll - specify if all the elements from the NVM table
 *                              entry shall be saved
 * Return: gNVM_OK_c - if operation completed successfully
 *         gNVM_Error_c - in case of error(s)
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_Status_t NvSaveOnIdle
(
    void* ptrData,
    bool_t saveAll
)
{
#if gNvStorageIncluded_d
    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvSaveOnIdle(ptrData,saveAll);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    ptrData=ptrData;
    saveAll=saveAll;
    return gNVM_Error_c;
#endif /* # gNvStorageIncluded_d */
}

/******************************************************************************
 * Name: NvSaveOnInterval
 * Description:  save no more often than a given time interval. If it has
 *               been at least that long since the last save,
 *               this function will cause a save the next time the idle
 *               task runs.
 * Parameters: [IN] ptrData - pointer to data to be saved
 * NOTE: this function saves all the element of the table entry pointed by
 *       ptrData
 * Return: NVM_OK_c - if operation completed successfully
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_Status_t NvSaveOnInterval
(
    void* ptrData
)
{
#if gNvStorageIncluded_d
    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvSaveOnInterval(ptrData);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    ptrData=ptrData;
    return gNVM_Error_c;
#endif
}                                       /* NvSaveOnInterval() */

/******************************************************************************
 * Name: NvSaveOnCount
 * Description: Decrement the counter. Once it reaches 0, the next call to
 *              NvIdle() will save the entire table entry (all elements).
 * Parameters: [IN] ptrData - pointer to data to be saved
 * Return: NVM_OK_c - if operation completed successfully
 *         Note: see also return codes of NvGetEntryFromDataPtr() function
 ******************************************************************************/
NVM_Status_t NvSaveOnCount
(
    void* ptrData
)
{
#if gNvStorageIncluded_d

    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvSaveOnCount(ptrData);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;

#else
    ptrData=ptrData;
    return gNVM_Error_c;
#endif
}                                       /* NvSaveOnCount() */

/******************************************************************************
 * Name: NvSetMinimumTicksBetweenSaves
 * Description: Set the timer used by NvSaveOnInterval(). Takes effect after
 *              the next save.
 * Parameters: [IN] newInterval - new time interval
 * Return: -
 ******************************************************************************/
void NvSetMinimumTicksBetweenSaves
(
    NvSaveInterval_t newInterval
)
{
#if gNvStorageIncluded_d
    gNvMinimumTicksBetweenSaves = newInterval;
#else
    newInterval=newInterval;
#endif
}                                       /* NvSetMinimumTicksBetweenSaves() */

/******************************************************************************
 * Name: NvSetCountsBetweenSaves
 * Description: Set the counter trigger value used by NvSaveOnCount().
 *              Takes effect after the next save.
 * Parameters: [IN] newCounter - new counter value
 * Return: -
 ******************************************************************************/
void NvSetCountsBetweenSaves
(
    NvSaveCounter_t newCounter
)
{
#if gNvStorageIncluded_d
    gNvCountsBetweenSaves = newCounter;
#else
    newCounter=newCounter;
#endif
}                                       /* NvSetCountsBetweenSaves() */

/******************************************************************************
 * Name: NvTimerTick
 * Description: Called from the idle task to process save-on-interval requests
 * Parameters: [IN] countTick - enable/disable tick count
 * Return: FALSE if the timer tick counters for all data sets have reached
 *         zero. In this case, the timer can be turned off.
 *         TRUE if any of the data sets' timer tick counters have not yet
 *         counted down to zero. In this case, the timer should be active
 ******************************************************************************/
bool_t NvTimerTick
(
    bool_t countTick
)
{
#if gNvStorageIncluded_d
    assert(mNvModuleInitialized);
    bool_t fTicksLeft;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    fTicksLeft = __NvTimerTick(countTick);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return fTicksLeft;
#else
    countTick=countTick;
    return FALSE;
#endif /* #if gNvStorageIncluded_d */
}                                       /* NvTimerTick() */

/******************************************************************************
 * Name: NvRestoreDataSet
 * Description: copy the most recent version of the element/table entry pointed
 *              by ptrData from NVM storage system to RAM memory
 * Parameter(s): [IN] ptrData - pointer to data (element) to be restored
 *               [IN] restoreAll - if FALSE restores a single element
 *                               - if TRUE restores an entire table entry
 * Return: status of the restore operation
 *****************************************************************************/
NVM_Status_t NvRestoreDataSet
(
    void* ptrData,
    bool_t restoreAll
)
{
#if gNvStorageIncluded_d
    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    /* before any data restore, complete any NVM pending operations */
    NvCompletePendingOperations();
    status = __NvRestoreDataSet(ptrData,restoreAll);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    ptrData=ptrData;
    restoreAll=restoreAll;
    return gNVM_Error_c;
#endif
}

/******************************************************************************
 * Name: NvClearCriticalSection
 * Description: leave critical section
 * Parameters: -
 * Return: -
 ******************************************************************************/
void NvClearCriticalSection
(
    void
)
{
#if (gNvStorageIncluded_d && gNvEnableCriticalSection_c)
    OSA_SR_ALLOC();
    OSA_ENTER_CRITICAL();
    if(mNvCriticalSectionFlag != 0u)  /* in case of set/clear mismatch */
    {
        --mNvCriticalSectionFlag;
    }

    OSA_EXIT_CRITICAL();
#endif
}

/******************************************************************************
 * Name: NvSetCriticalSection
 * Description: enter critical section
 * Parameters: -
 * Return: -
 ******************************************************************************/
void NvSetCriticalSection
(
    void
)
{
#if (gNvStorageIncluded_d && gNvEnableCriticalSection_c)
    OSA_SR_ALLOC();
    OSA_ENTER_CRITICAL();
    ++mNvCriticalSectionFlag;
    OSA_EXIT_CRITICAL();
#endif
}
/******************************************************************************
 * Name: NvIdle
 * Description: Called from the idle task (bare-metal) or NVM_Task (MQX,
 *              FreeRTOS) to process the pending saves, erase or copy
 *              operations.
 * Parameters: -
 * Return: -
 ******************************************************************************/
void NvIdle
(
    void
)
{
#if gNvStorageIncluded_d
    if(mNvModuleInitialized == TRUE)
    {
        if(mNvIdleTaskId == NULL)
        {
            mNvIdleTaskId = OSA_TaskGetCurrentHandle();
        }
        (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
        __NvIdle();
        (void)OSA_MutexUnlock(mNVMMutexId);
    }
#endif
}/* NvIdle() */
/******************************************************************************
 * Name: NvGetNvIdleTaskId
 * Description:
 * Parameters: -
 * Return: returns the Id of the task which hosts NvIdle function
 ******************************************************************************/
void* NvGetNvIdleTaskId
(
    void
)
{
#if gNvStorageIncluded_d
    return (void*)mNvIdleTaskId;
#else
    return NULL;
#endif
}/* NvIdle() */

/******************************************************************************
 * Name: NvIsDataSetDirty
 * Description: return TRUE if the element pointed by ptrData is dirty
 * Parameters: [IN] ptrData - pointer to data to be checked
 * Return: TRUE if the element is dirty, FALSE otherwise
 ******************************************************************************/
bool_t NvIsDataSetDirty
(
    void* ptrData
)
{
#if gNvStorageIncluded_d
    bool_t res;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    res = __NvIsDataSetDirty(ptrData);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return res;
#else
    ptrData=ptrData;
    return FALSE;
#endif
}

/******************************************************************************
 * Name: NvGetStatistics
 * Description:
 * Parameter(s): [OUT] ptrStat - pointer to a memory location where the pages
 *                               statistics (erase cycles of each page) will
 *                               be stored
 * Return: -
 *****************************************************************************/
void NvGetPagesStatistics
(
    NVM_Statistics_t* ptrStat
)
{
#if gNvStorageIncluded_d
    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    if(TRUE == mNvModuleInitialized)
    {
        if(NULL != ptrStat)
        {
            if( 0u != (mNvPageCounter%2u))
            {
                ptrStat->SecondPageEraseCyclesCount = (mNvPageCounter-1u)/2u;
                ptrStat->FirstPageEraseCyclesCount = ptrStat->SecondPageEraseCyclesCount;
            }
            else
            {
                ptrStat->FirstPageEraseCyclesCount = mNvPageCounter/2u;
                ptrStat->SecondPageEraseCyclesCount = (mNvPageCounter-2u)/2u;
            }
        }
    }
    #else /* FlexNVM */
    ptrStat->FirstPageEraseCyclesCount = 0;
    ptrStat->SecondPageEraseCyclesCount = 0;
    #endif
#else
    ptrStat=ptrStat;
#endif
}

/******************************************************************************
 * Name: NvGetPagesSize
 * Description: Retrieves the NV Virtual Page size
 * Parameter(s): [OUT] pPageSize - pointer to a memory location where the page
 *                                 size will be stored
 * Return: -
 *****************************************************************************/
void NvGetPagesSize
(
    uint32_t* pPageSize
)
{
    if(NULL != pPageSize)
    {
#if gNvStorageIncluded_d
    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
        *pPageSize = mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize;
    #else /* FlexNVM */
        *pPageSize = 0U;
    #endif
#else
        *pPageSize = 0U;
#endif
    }
}

/******************************************************************************
 * Name: NvFormat
 * Description: Format the NV storage system. The function erases both virtual
 *              pages and then writes the page counter/ram table to active page.
 * Parameter(s): -
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_FormatFailure_c - if the format operation fails
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_CriticalSectionActive_c - if the system has entered in a
 *                                        critical section
 *****************************************************************************/
NVM_Status_t NvFormat
(
    void
)
{
#if gNvStorageIncluded_d
    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvFormat();
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    return gNVM_Error_c;
#endif /* gNvStorageIncluded_d */
}

/******************************************************************************
 * Name: NvRegisterTableEntry
 * Description: The function tries to register a new table entry within an
 *              existing NV table. If the NV table contained an erased (invalid)
 *              entry, the entry will be overwritten with a new one (provided
 *              by the mean of this function arguments)
 * Parameter(s): [IN] ptrData - generic pointer to RAM data to be registered
 *                              within the NV storage system
 *               [IN] uniqueId - an unique ID of the table entry
 *               [IN] elemCount - how many elements the table entry contains
 *               [IN] elemSize - the size of an element
 *               [IN] dataEntryType - the type of the new entry
 *               [IN] overwrite - if an existing table entry shall be
 *                                overwritten
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *****************************************************************************/
NVM_Status_t NvRegisterTableEntry
(
    void* ptrData,
    NvTableEntryId_t uniqueId,
    uint16_t elemCount,
    uint16_t elemSize,
    uint16_t dataEntryType,
    bool_t overwrite
)
{
#if gNvStorageIncluded_d && gNvUseExtendedFeatureSet_d && gNvTableKeptInRam_d

    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvRegisterTableEntry(ptrData,uniqueId,elemCount,elemSize, dataEntryType, overwrite);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    ptrData=ptrData;
    uniqueId=uniqueId;
    elemCount=elemCount;
    elemSize=elemSize;
    overwrite=overwrite;
    return gNVM_Error_c;
#endif
}

/******************************************************************************
 * Name: NvEraseEntryFromStorage
 * Description: The function removes a table entry within the existing NV
 *              table.
 * Parameter(s): [IN] ptrData - a pointer to an existing RAM data that is
 *                              managed by the NV storage system
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *****************************************************************************/
NVM_Status_t NvEraseEntryFromStorage
(
    void* ptrData
)
{
#if gNvStorageIncluded_d && gNvUseExtendedFeatureSet_d && gNvTableKeptInRam_d
    NVM_Status_t status;
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIdx;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);

    status = NvGetTableEntryIndexFromDataPtr(ptrData, &tblIdx, &tableEntryIdx);
    if(gNVM_OK_c == status)
    {
        /* invalidate the table entry */
        pNVM_DataTable[tableEntryIdx].pData = NULL;
        pNVM_DataTable[tableEntryIdx].ElementsCount = 0;
        pNVM_DataTable[tableEntryIdx].ElementSize = 0;
        status = __NvEraseEntryFromStorage(tblIdx.entryId, tableEntryIdx);
    }
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    ptrData=ptrData;
    return gNVM_Error_c;
#endif
}

/******************************************************************************
 * Name: NvSyncSave
 * Description: The function saves the pointed element or the entire table
 *              entry to the storage system. The save operation is not
 *              performed on the idle task but within this function call.
 * Parameter(s): [IN] ptrData - a pointer to data to be saved
 *               [IN] saveAll - specifies if the entire table entry shall be
 *                              saved or only the pointed element
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *         gNVM_PointerOutOfRange_c - if the pointer is out of range
 *         gNVM_InvalidTableEntry_c - if the table entry is not valid
 *         gNVM_MetaInfoWriteError_c - meta tag couldn't be written
 *         gNVM_RecordWriteError_c - record couldn't be written
 *         gNVM_CriticalSectionActive_c - the module is in critical section
 *****************************************************************************/
NVM_Status_t NvSyncSave
(
    void* ptrData,
    bool_t saveAll
)
{
#if gNvStorageIncluded_d
    NVM_Status_t status;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvSyncSave(ptrData, saveAll);
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    ptrData=ptrData;
    saveAll=saveAll;
    return gNVM_Error_c;
#endif
}

/******************************************************************************
 * Name: NvAtomicSave
 * Description: The function performs an atomic save of the entire NV table
 *              to the storage system. The operation is performed
 *              in place (atomic).
 * Parameter(s):  -
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *         gNVM_PointerOutOfRange_c - if the pointer is out of range
 *         gNVM_InvalidTableEntry_c - if the table entry is not valid
 *         gNVM_MetaInfoWriteError_c - meta tag couldn't be written
 *         gNVM_RecordWriteError_c - record couldn't be written
 *         gNVM_CriticalSectionActive_c - the module is in critical section
 *****************************************************************************/
NVM_Status_t NvAtomicSave
(
    void
)
{
#if gNvStorageIncluded_d
    NVM_Status_t status ;
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvAtomicSave();
    (void)OSA_MutexUnlock(mNVMMutexId);
    return status;
#else
    return gNVM_Error_c;
#endif
}

/******************************************************************************
 * Name: NvShutdown
 * Description: The function waits for all idle saves to be processed.
 * Parameter(s):  -
 * Return: -
 *****************************************************************************/
void NvShutdown( void )
{
#if gNvStorageIncluded_d
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    __NvShutdown();
    (void)OSA_MutexUnlock(mNVMMutexId);
#endif
}

/******************************************************************************
 * Name: NvCompletePendingOperations
 * Description: The function attemps to complete all the NVM related pending
 *              operations.
 * Parameter(s):  -
 * Return: -
 *****************************************************************************/
void NvCompletePendingOperations
(
    void
)
{
#if gNvStorageIncluded_d
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    NvCompletePendingOperationsUnsafe();
    (void)OSA_MutexUnlock(mNVMMutexId);
#endif
}

#ifdef USE_MSD_BOOTLOADER
/******************************************************************************
 * Name: NvEraseSector
 * Description: The function performs the storage system sector erase.
 *              The operation is performed in place (atomic).
 * Parameter(s):  [IN] ignoreCriticalSectionFlag - if set to TRUE, the critical
 *                                                section flag is ignored
 * Return: void
 *****************************************************************************/
void NvEraseSector(uint32_t sectorAddr)
{
#if gNvStorageIncluded_d
    /* erase sector */
    (void)NV_FlashEraseSector(sectorAddr, (uint32_t)((uint8_t*)NV_STORAGE_SECTOR_SIZE));
#else
   (void)sectorAddr;
#endif
}
#endif
