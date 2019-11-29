/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* Implementation of the non-volatile storage module for the CORTEX-M4 processor
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "EmbeddedTypes.h"
#include "NV_Flash.h"
#include "TimersManager.h"
#include "RNG_Interface.h"
#include "FunctionLib.h"

#include "fsl_os_abstraction.h"
#include "Flash_Adapter.h"

#if (gFsciIncluded_c && (gNvmEnableFSCIRequests_c || gNvmEnableFSCIMonitoring_c))
#include "NV_FsciCommands.h"
#endif

#include "Messaging.h"
/*****************************************************************************
 *****************************************************************************
 * Private macros
 *****************************************************************************
 *****************************************************************************/
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
#define gNvVirtualPagesCount_c         2 /* DO NOT MODIFY */

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
#define gNvLegacyOffset_c 4

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
/*****************************************************************************
 *****************************************************************************
 * Private type definitions
 *****************************************************************************
 *****************************************************************************/
#if PGM_SIZE_BYTE == FTFx_PHRASE_SIZE
typedef uint64_t NV_baseType;
#else
typedef uint32_t NV_baseType;
#endif

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
static NVM_Status_t __NvRegisterTableEntry
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
 *              table. The RAM table must be updated previusly.
 * Parameter(s): [IN] entryId - the entry id of the entry that is removed
 *               [IN] tableEntryIndex - the index of the entry in the ram table
 * Return: gNVM_OK_c - if the operation completes successfully
 *         gNVM_ModuleNotInitialized_c - if the NVM  module is not initialized
 *         gNVM_NullPointer_c - if a NULL pointer is provided
 *****************************************************************************/
static NVM_Status_t __NvEraseEntryFromStorage
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
static void InitNVMConfig
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
static NVM_Status_t __NvAtomicSave
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
static NVM_Status_t __NvSyncSave
(
  void* ptrData,
  bool_t saveAll
);

/******************************************************************************
 * Name: __NvCompletePendingOperations
 * Description: The function attemps to complete all the NVM related pending
 *              operations.
 * Parameter(s):  -
 * Return: -
 *****************************************************************************/
static void __NvCompletePendingOperations
(
  void
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
static NVM_Status_t __NvFormat
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
static void __NvIdle
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
static NVM_Status_t __NvRestoreDataSet
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
static bool_t __NvTimerTick
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
static NVM_Status_t __NvSaveOnCount
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
static NVM_Status_t __NvSaveOnInterval
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
static NVM_Status_t __NvSaveOnIdle
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
static NVM_Status_t __NvModuleInit
(
  void
);

#if gUnmirroredFeatureSet_d

/******************************************************************************
 * Name: __NvmMoveToRam
 * Description: Move from NVM to Ram
 * Parameter(s):  ppData     double pointer to the entity to be moved from flash to RAM
 * Return: pointer to Ram location
 *****************************************************************************/
static NVM_Status_t __NvmMoveToRam
(
  void** ppData
);

/******************************************************************************
 * Name: __NvmErase
 * Description: Erase from NVM an unmirrored dataset
 * Parameter(s):  ppData     double pointer to the entity to be erased
 * Return: pointer to Ram location
 *****************************************************************************/
static NVM_Status_t __NvmErase
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
static bool_t NvIsNVMFlashAddress
(
  void* address
);

/******************************************************************************
 * Name: __NvmRestoreUnmirrored
 * Description: Restores all unmirrored entries with gNVM_NotMirroredInRamAutoRestore_c at init
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
static void __NvmRestoreUnmirrored
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
static bool_t NvInitPendingSavesQueue
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
static bool_t NvPushPendingSave
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
static bool_t NvPopPendingSave
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
static uint16_t NvGetPendingSavesCount
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
static NVM_Status_t UpgradeLegacyTable
(
  void
);

/******************************************************************************
 * Name: NvUpdateSize
 * Description: Updates the size to be a multiple of 4 or 8 depending on the flash controller
 * Parameter(s): [IN] size - size to be updated
 * Return: the computed size
 *****************************************************************************/
static uint32_t NvUpdateSize
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
static NVM_Status_t NvEraseVirtualPage
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
static void NvInitStorageSystem
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
static NVM_Status_t NvVirtualPageBlankCheck
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
static NVM_Status_t NvUpdateLastMetaInfoAddress
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
static NVM_Status_t NvGetMetaInfo
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
static NVM_Status_t NvGetPageFreeSpace
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
static bool_t NvIsMemoryAreaAvailable
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
static bool_t NvIsRecordCopied
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
static NVM_Status_t NvInternalCopy
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
static uint32_t NvGetTblEntryMetaAddrFromId
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
static NVM_Status_t NvInternalDefragmentedCopy
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
static NVM_Status_t NvCopyPage
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
static NVM_Status_t NvInternalFormat
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
static bool_t NvSaveRamTable
(
    NVM_VirtualPageID_t pageId
);

#if gNvUseExtendedFeatureSet_d

/******************************************************************************
 * Name: GetFlashTableVersion
 * Description: returns the flash table version
 * Parameter(s): -
 * Return: 0 or flash table version
 *****************************************************************************/
uint16_t GetFlashTableVersion
(
    void
);

/******************************************************************************
 * Name: RecoverNvEntry
 * Description: Reads a flash entry so that the application can handle dynamic entries.
 * Parameter(s): [IN] index - the ram entry index
 *               [OUT] entry - the flash entry at the specified index
 * Return: gNVM_OK_c - if the operation completes successfully
           gNVM_RestoreFailure_c - if the operation failed
           gNVM_AddressOutOfRange_c - if the index is too large
 *****************************************************************************/
NVM_Status_t RecoverNvEntry
(
    uint16_t index,
    NVM_DataEntry_t *entry
);
/******************************************************************************
 * Name: NvGetFlashTableSize
 * Description: Retrieves the size of the NV table
 * Parameter(s): -
 * Return: the NV table size
 ******************************************************************************/
static uint32_t NvGetFlashTableSize
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
static bool_t NvIsRamTableUpdated
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
static bool_t NvGetTableEntry
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
static NVM_Status_t NvGetEntryFromDataPtr
(
  void* pData,
  NVM_TableEntryInfo_t* pIndex
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
static NVM_Status_t NvWriteRecord
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
static NVM_Status_t NvRestoreData
(
  NVM_TableEntryInfo_t* tblIdx
);


/******************************************************************************
 * Name: NvGetTableEntryIndex
 * Description: get the table entry index from the provided ID
 * Parameter(s): [IN] entryId - the ID of the table entry
 * Return: table entry index of gNvInvalidTableEntryIndex_c
 *****************************************************************************/
static uint16_t NvGetTableEntryIndexFromId
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
static NVM_Status_t NvAddSaveRequestToQueue
(
  NVM_TableEntryInfo_t* ptrTblIdx
);


/******************************************************************************
 * Name: GetRandomRange
 * Description: Returns a random number between 'low' and 'high'
 * Parameter(s): [IN] low, high - generated number range
 * Return: 0..255
 ******************************************************************************/
static uint8_t GetRandomRange
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
static uint32_t NvGetFlexLastMetaInfo
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
static void NvGetFlexMetaInfoFromId
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
static NVM_Status_t NvCheckNvmTableForFlexRAMUsage
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
static
#endif
NVM_VirtualPageID_t mNvActivePageId;

/*
 * Name: mNvPageCounter
 * Description: page counter, used to validate the entire virtual page
 *              and also to provide statistical information about
 *              how many times the virtual page was erased
 */
static uint32_t mNvPageCounter = 0;

/*
 * Name: mNvVirtualPageProperty
 * Description: virtual page properties
 */
NVM_VirtualPageProperties_t mNvVirtualPageProperty[gNvVirtualPagesCount_c];

/*
 * Name: mNvCopyOperationIsPending
 * Description: a flag that a indicates that a page copy operation is requested
 */
static bool_t mNvCopyOperationIsPending = FALSE;

/*
 * Name: mNvErasePgCmdStatus
 * Description: a data structure used to erase a virtual page. The erase of a
 *              virtual page is performed in idle task, in a sector-by-sector
 *              manner. When the idle task runs, if the erase pending flag is
 *              set, only one flash sector will be erased. Therefore, the
 *              virtual page will be entirely erased after several runs of
 *              idle task
 */
static NVM_ErasePageCmdStatus_t mNvErasePgCmdStatus;

/*
 * Name: mNvFlashConfigInitialised
 * Description: variable that holds the hal driver and active page initialisation status
 */
static bool_t mNvFlashConfigInitialised = FALSE;

/*
 * Name: maNvRecordsCpyIdx
 * Description: An array that stores the indexes of the records already copied;
 *              Used by the defragmentation process.
 */
#if gNvFragmentation_Enabled_d
static uint16_t maNvRecordsCpyOffsets[gNvRecordsCopiedBufferSize_c];
#endif /* gNvFragmentation_Enabled_d */

#if gNvUseExtendedFeatureSet_d
/*
 * Name: mNvTableSizeInFlash
 * Description: the size of the NV table stored in the FLASH memory
 */
#if !gNvDebugEnabled_d
static
#endif
uint32_t mNvTableSizeInFlash = 0;

/*
 * Name: mNvTableMarker
 * Description: FLASH NV table marker, used only for code readability
 *              (when applying the sizeof() operator to it)
 */
static uint16_t mNvTableMarker = gNvTableMarker_c;

/*
 * Name: mNvTableMarker
 * Description: FLASH NV application version, used for determining when table upgrade
 *              happened
 */
#if !gNvDebugEnabled_d
static
#endif
uint16_t mNvFlashTableVersion = gNvFlashTableVersion_c;

/*
 * Name: mNvTableUpdated
 * Description: boolean flag used to mark if the NV table from the RAM memory
 *              has been changed. Set (or left untouched) only at module initialization,
 *              when the existing NV FLASH table (if any) is compared against
 *              the NV RAM table.
 */
static bool_t mNvTableUpdated;

#endif /* gNvUseExtendedFeatureSet_d */

#endif /* no FlexNVM */

/*
 * Name: mNvModuleInitialized
 * Description: variable that holds the NVM initialisation status
 */
static bool_t mNvModuleInitialized = FALSE;

/*
 * Name: mNvCriticalSectionFlag
 * Description: If this counter is != 0, do not save to NV Storage
 */
static uint8_t mNvCriticalSectionFlag = 0;

/*
 * Name: gNvMinimumTicksBetweenSaves
 * Description: Minimum number of calls to NvTimerTick() between saves of a given data set
 */
static NvSaveInterval_t gNvMinimumTicksBetweenSaves = gNvMinimumTicksBetweenSaves_c;

/*
 * Name: gNvCountsBetweenSaves
 * Description: Minimum number of calls to NvSaveOnIdle() between saves of a given data set
 */
static NvSaveCounter_t gNvCountsBetweenSaves = gNvCountsBetweenSaves_c;

/*
 * Name: mNvPendingSavesQueue
 * Description: a queue used for storing information about the pending saves
 */
static NVM_SaveQueue_t mNvPendingSavesQueue;

/*
 * Name: maDatasetInfo
 * Description: Data set info table
 */
static NVM_DatasetInfo_t maDatasetInfo[gNvTableEntriesCountMax_c];

/*
 * Name: mNvSaveOnIntervalEvent
 * Description: flag used to signal an 'SaveOnInterval' event
 */
static bool_t mNvSaveOnIntervalEvent;

/*
 * Name: mNvLastTimestampValue
 * Description: the value of the last timestamp used by the Save-On-Interval functionality
 */
static uint64_t mNvLastTimestampValue = 0;
/*
 * Name: mNVMMutexId
 * Description: mutex used to ensure NVM functions thread switch safety
 */
osaMutexId_t mNVMMutexId;
/*
 * Name: mNvIdleTaskId
 * Description: stores the Id of the task which hosts NvIdle function.
 */
static osaTaskId_t mNvIdleTaskId = NULL;

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
const uint8_t eraseNVMFirst[1]={0xFF};

/*
 * Name: eraseNVMSecond
 * Description: byte used to the force the erasure of the first sector of
 *              the second virtual page (thus invalidating the entire page)
 *              via IAR flashloader. Below section must be defined in the
 *              linker configuration file (*.icf)
 */
#pragma section = "sEraseNVM"
#pragma location = "sEraseNVM"
const uint8_t eraseNVMSecond[1]={0xFF};
#endif /* __IAR_SYSTEMS_ICC__  */
#endif /* gNvStorageIncluded_d */

/*****************************************************************************
 *****************************************************************************
 * Public memory declarations
 *****************************************************************************
 *****************************************************************************/

#if gNvStorageIncluded_d

#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
#if (!defined(__CC_ARM))
/*
 * Name: NV_STORAGE_END_ADDRESS
 * Description: NV_STORAGE_END_ADDRESS from linker command file is used by this code
 *              as Raw Sector Start Address. This should not be misleading because
 *              ENVM module writes meta information in address-ascending order
 *              and records in address-descending order.
 */
extern uint32_t NV_STORAGE_END_ADDRESS[];

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

#define NV_STORAGE_END_ADDRESS (Image$$NVM_region$$ZI$$Base)
#define NV_STORAGE_START_ADDRESS (Image$$NVM_region$$ZI$$Limit)
#define NVM_LENGTH ((uint32_t)((uint8_t*)NV_STORAGE_START_ADDRESS)-(uint32_t)((uint8_t*)NV_STORAGE_END_ADDRESS))
#define NV_STORAGE_SECTOR_SIZE FSL_FEATURE_FLASH_PAGE_SIZE_BYTES
#define NV_STORAGE_MAX_SECTORS (NVM_LENGTH/NV_STORAGE_SECTOR_SIZE)
#endif /* __CC_ARM */
#endif /* no FlexNVM */

#if (gNvUseFlexNVM_d == TRUE)
extern void ResetMCU(void);
#endif /* gNvUseFlexNVM_d == TRUE */

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
static NVM_Status_t __NvRegisterTableEntry
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
    
    if(!mNvModuleInitialized)
    {
        return gNVM_ModuleNotInitialized_c;
    }
    
    if(gNvInvalidDataEntry_c == uniqueId)
    {
        return gNVM_RegisterFailure_c;
    }
    
#if gNvFragmentation_Enabled_d
    if (elemCount > gNvRecordsCopiedBufferSize_c)
    {
        return gNVM_DefragBufferTooSmall_c;
    }
#endif
    
    while(loopCnt < gNVM_TABLE_entries_c)
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
                return __NvEraseEntryFromStorage(uniqueId, loopCnt);
            }
            else
            {
                return gNVM_AlreadyRegistered;
            }
        }
        /* increment the loop counter */
        loopCnt++;
    }
    
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
            return gNVM_CriticalSectionActive_c;
        }
        /*update the flash table*/
#if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
        FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
        FSCI_MsgNVVirtualPageMonitoring(FALSE,status=NvCopyPage(gNvCopyAll_c));
#else
        status = NvCopyPage(gNvCopyAll_c);
#endif
        return status;
    }
    
    return gNVM_RegisterFailure_c;
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
    InitNVMConfig();
    if (mNvActivePageId == gVirtualPageNone_c)
        return 0;
    return (*(NVM_TableInfo_t*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress)).fields.NvTableVersion;
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
static NVM_Status_t __NvEraseEntryFromStorage
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
        return gNVM_ModuleNotInitialized_c;
    }


    /* Check if is in pending queue - if yes than remove it */
    if (NvGetPendingSavesCount(&mNvPendingSavesQueue))
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
        return gNVM_CriticalSectionActive_c;
    }

    /* erase the table entry by making a copy of the active page to the inactive one,
    * but skipping while copying the table entry to be erased */
    if (gEmptyPageMetaAddress_c != mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress)
    {
        #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
            FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
            FSCI_MsgNVVirtualPageMonitoring(FALSE, status = NvCopyPage(entryId));
        #else
            status = NvCopyPage(entryId);
        #endif
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

    entry->pData = NULL;
    InitNVMConfig();
    if (mNvActivePageId == gVirtualPageNone_c)
    {
        return gNVM_RestoreFailure_c;
    }
    if (index * sizeof(NVM_EntryInfo_t) >= mNvTableSizeInFlash)
    {
        return gNVM_AddressOutOfRange_c;
    }

    addr = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + sizeof(NVM_TableInfo_t) + index * sizeof(NVM_EntryInfo_t);
    NV_FlashRead(addr, (uint8_t*)&entryInfo, sizeof(NVM_EntryInfo_t));
    entry->DataEntryID   = entryInfo.fields.NvDataEntryID;
    entry->DataEntryType = entryInfo.fields.NvDataEntryType;
    entry->ElementsCount = entryInfo.fields.NvElementsCount;
    entry->ElementSize   = entryInfo.fields.NvElementSize;
    return gNVM_OK_c;
#else
    return gNVM_Error_c;
#endif //gNvTableKeptInRam_d
}

/******************************************************************************
 * Name: NvIsRamTableUpdated
 * Description: Checks if the the NV table from RAM memory has changed since
 *              last system reset (e.g. via an OTA transfer)
 * Parameter(s): -
 * Return: TRUE if the NV RAM table has been changed / FALSE otherwise
 ******************************************************************************/
static bool_t NvIsRamTableUpdated
(
    void
)
{
    uint16_t idx;
    uint32_t addr;
    uint32_t endAddr;
    bool_t idFound;
    NVM_EntryInfo_t entryInfo;

    /* address = page raw sector start address + page counter size + table marker + table version */
    addr = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + sizeof(NVM_TableInfo_t);

    /* compute the search end address */
    endAddr = addr + mNvTableSizeInFlash;

    do
    {
        /* read ID */
        NV_FlashRead(addr, (uint8_t*)&entryInfo, sizeof(NVM_EntryInfo_t));

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
            return TRUE;
        }
        if ((entryInfo.fields.NvDataEntryType != pNVM_DataTable[idx].DataEntryType) && ((gNVM_MirroredInRam_c == entryInfo.fields.NvDataEntryType) ||
            (gNVM_MirroredInRam_c == pNVM_DataTable[idx].DataEntryType)))
        {
            return TRUE;
        }
        if ((entryInfo.fields.NvElementsCount != pNVM_DataTable[idx].ElementsCount) ||
            (entryInfo.fields.NvElementSize   != pNVM_DataTable[idx].ElementSize))
        {
            return TRUE;
        }

        /* increment the address */
        addr += sizeof(NVM_EntryInfo_t);

    } while(addr < endAddr);

    return FALSE;
}
/******************************************************************************
 * Name: NvGetTableEntry
 * Description: get the NV table entry information stored on FLASH memory
 * Parameter(s): [IN] tblEntryId - table entry ID
 *               [OUT] pDataEntry - a pointer to a memory location where the
 *                                  entry information will be stored
 * Return: TRUE if the has been found / FALSE otherwise
 ******************************************************************************/
static bool_t NvGetTableEntry
(
    uint16_t tblEntryId,
    NVM_DataEntry_t* pDataEntry
)
{
    uint32_t addr;
    uint64_t tmp;

    pDataEntry->pData = NULL; /* the data pointer is not saved on FLASH table and
    * shall not be used by the caller of this function */

    addr = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;

    NV_FlashRead(addr, (uint8_t*)&tmp, sizeof(NVM_TableInfo_t));
    if (((NVM_TableInfo_t*)&tmp)->fields.NvTableMarker != mNvTableMarker)
    {
        pDataEntry->DataEntryType = 0;
        pDataEntry->ElementsCount = 0;
        pDataEntry->ElementSize = 0;
        pDataEntry->DataEntryID = gNvInvalidDataEntry_c;
        return FALSE;
    }

    /* increment address */
    addr += sizeof(NVM_TableInfo_t);

    do
    {
        NV_FlashRead(addr, (uint8_t*)&tmp, sizeof(NVM_EntryInfo_t));

        if(((NVM_TableInfo_t*)&tmp)->fields.NvTableMarker == mNvTableMarker)
        {
            /* reached end of table */
            break;
        }
        if(((NVM_EntryInfo_t*)&tmp)->fields.NvDataEntryID == tblEntryId)
        {
            pDataEntry->DataEntryID   = ((NVM_EntryInfo_t*)&tmp)->fields.NvDataEntryID;
            pDataEntry->DataEntryType = ((NVM_EntryInfo_t*)&tmp)->fields.NvDataEntryType;
            pDataEntry->ElementsCount = ((NVM_EntryInfo_t*)&tmp)->fields.NvElementsCount;
            pDataEntry->ElementSize   = ((NVM_EntryInfo_t*)&tmp)->fields.NvElementSize;
            return TRUE;
        }

        /* continue searching */
        addr += sizeof(NVM_EntryInfo_t);
    } while(addr < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress);

    pDataEntry->DataEntryType = 0;
    pDataEntry->ElementsCount = 0;
    pDataEntry->ElementSize = 0;
    pDataEntry->DataEntryID = gNvInvalidDataEntry_c;
    return FALSE;
}

/******************************************************************************
 * Name: NvGetFlashTableSize
 * Description: Retrieves the size of the NV table
 * Parameter(s): -
 * Return: the NV table size
 ******************************************************************************/
static uint32_t NvGetFlashTableSize
(
    void
)
{
    uint32_t addr;
    uint32_t size = 0;
    NVM_TableInfo_t tableInfo;

    /* compute the size of the table stored in Flash memory */
    addr = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;

    NV_FlashRead(addr, (uint8_t*)&tableInfo, sizeof(NVM_TableInfo_t));

    if(gNvTableMarker_c != tableInfo.fields.NvTableMarker)
    {
        return 0;
    }

    addr += sizeof(NVM_TableInfo_t);

    do
    {
        NV_FlashRead(addr, (uint8_t*)&tableInfo, sizeof(NVM_TableInfo_t));

        if(gNvTableMarker_c == tableInfo.fields.NvTableMarker)
        {
            return size;
        }
        size += sizeof(NVM_TableInfo_t);
        addr += sizeof(NVM_TableInfo_t);

    } while(addr < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress);

    return 0;
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
static NVM_Status_t __NvAtomicSave
(
    void
)
{
    NVM_Status_t status = gNVM_OK_c;
    uint16_t loopCnt = 0;
    NVM_TableEntryInfo_t tblIdx;
#if gUnmirroredFeatureSet_d
    uint16_t loopCnt2 = 0;
    uint16_t remaining_count;
    uint16_t tableEntryIdx;
    bool_t skip;
#endif

    /* remove all non unmirrored erase operations from the queue */
#if gUnmirroredFeatureSet_d
    if (NvGetPendingSavesCount(&mNvPendingSavesQueue))
    {
        /* Start from the queue's head */
        loopCnt = mNvPendingSavesQueue.Head;
        remaining_count = mNvPendingSavesQueue.EntriesCount;

        while (remaining_count)
        {
            skip = FALSE;
            tableEntryIdx = NvGetTableEntryIndexFromId(mNvPendingSavesQueue.QData[loopCnt].entryId);
            if (gNvInvalidTableEntryIndex_c != tableEntryIdx)
            {
                if (gNVM_MirroredInRam_c != pNVM_DataTable[tableEntryIdx].DataEntryType)
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
#else
    NvInitPendingSavesQueue(&mNvPendingSavesQueue);
#endif
    /* if critical section, add a special entry in the queue */
    if (mNvCriticalSectionFlag)
    {
        tblIdx.entryId = gNvCopyAll_c;
        tblIdx.elementIndex = gNvCopyAll_c;
        tblIdx.saveRestoreAll = TRUE;
        status = NvAddSaveRequestToQueue(&tblIdx);
        if (gNVM_SaveRequestRejected_c == status)
        {
            return gNVM_SaveRequestRejected_c;
        }
        return gNVM_CriticalSectionActive_c;
    }
    
    while(loopCnt < gNVM_TABLE_entries_c)
    {
#if gUnmirroredFeatureSet_d
        if (gNVM_MirroredInRam_c != pNVM_DataTable[loopCnt].DataEntryType)
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
                    return status;
                }
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
                return status;
            }
        }

        /* increment the loop counter */
        loopCnt++;
    }
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
static NVM_Status_t __NvSyncSave
(
    void* ptrData,
    bool_t saveAll
)
{
    NVM_TableEntryInfo_t tblIdx;
    NVM_Status_t status;

    if(!mNvModuleInitialized)
    {
        return gNVM_ModuleNotInitialized_c;
    }

    if(NULL == ptrData)
    {
        return gNVM_NullPointer_c;
    }

    if((status = NvGetEntryFromDataPtr(ptrData, &tblIdx)) != gNVM_OK_c)
    {
        return status;
    }

    /* write the save all flag */
    #if gNvFragmentation_Enabled_d
    tblIdx.saveRestoreAll = saveAll;
    #else
    tblIdx.saveRestoreAll = TRUE;
    #endif /* gNvFragmentation_Enabled_d */

    if (mNvCriticalSectionFlag)
    {
        status = NvAddSaveRequestToQueue(&tblIdx);
        if (gNVM_SaveRequestRejected_c == status)
        {
            return gNVM_SaveRequestRejected_c;
        }
        return gNVM_CriticalSectionActive_c;
    }
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    if((status = NvWriteRecord(&tblIdx)) == gNVM_PageCopyPending_c)
    {
        /* copy active page */
        #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
            FSCI_MsgNVVirtualPageMonitoring(TRUE,gNVM_OK_c);
            FSCI_MsgNVVirtualPageMonitoring(FALSE,status=NvCopyPage(gNvCopyAll_c));
        #else
            status = NvCopyPage(gNvCopyAll_c);
        #endif
        if(status != gNVM_OK_c)
        {
            return status;
        }
        mNvCopyOperationIsPending = FALSE;

        /* erase old page */
        status = NvEraseVirtualPage(mNvErasePgCmdStatus.NvPageToErase);
        if (gNVM_OK_c != status)
            return status;
        mNvVirtualPageProperty[mNvErasePgCmdStatus.NvPageToErase].NvLastMetaInfoAddress = gEmptyPageMetaAddress_c;
        mNvErasePgCmdStatus.NvErasePending = FALSE;
        /* write record */
        status = NvWriteRecord(&tblIdx);
    }
#else /* FlexNVM */
    /* write record */
    status = NvWriteRecord(&tblIdx);
#if FSL_FEATURE_FLASH_IS_FTFL
    /* wait for EEPROM system to be ready (fix@ENGR283453) */
    while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET,  FTFx_SSD_FCNFG_EEERDY)));
#endif
#endif
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
static NVM_Status_t __NvFormat
(
    void
)
{
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    NVM_Status_t status;
    NVM_TableInfo_t tableInfo;
    #if gUnmirroredFeatureSet_d
    uint16_t loopCnt;
    uint16_t loopCnt2;
    #endif

    if(!mNvModuleInitialized)
    {
        return gNVM_ModuleNotInitialized_c;
    }

    if(mNvCriticalSectionFlag)
    {
        return gNVM_CriticalSectionActive_c;
    }

    NV_FlashRead(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress, (uint8_t*)&tableInfo, sizeof(NVM_TableInfo_t));

    if(gNVM_OK_c != (status = NvInternalFormat(tableInfo.fields.NvPageCounter)))
    {
        return status;
    }
    #if gUnmirroredFeatureSet_d
    {
        for(loopCnt = 0; loopCnt < (uint16_t)gNVM_TABLE_entries_c; loopCnt++)
        {
            maDatasetInfo[loopCnt].countsToNextSave = gNvCountsBetweenSaves;
            maDatasetInfo[loopCnt].saveNextInterval = FALSE;
            if(pNVM_DataTable[loopCnt].DataEntryType != gNVM_MirroredInRam_c)
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
    NvInitPendingSavesQueue(&mNvPendingSavesQueue);
    return status;

#else /* FlexNVM */

    uint8_t buff[gNvFlexFormatBufferSize_c];
    uint32_t addr;
    uint16_t size;

    FLib_MemSet(buff, gNvErasedFlashCellValue_c, gNvFlexFormatBufferSize_c);

#if FSL_FEATURE_FLASH_IS_FTFL
    addr = gFlashConfig.EERAMBase;
    size = gFlashConfig.EEESize;

    while(size)
    {
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET , FTFx_SSD_FCNFG_EEERDY)));

        if(kStatus_FLASH_Success != EEEWrite(&gFlashConfig, addr, sizeof(buff), buff))
        {
            return gNVM_FormatFailure_c;
        }
        size -= gNvFlexFormatBufferSize_c;
        addr += gNvFlexFormatBufferSize_c;
    }
#endif /* FSL_FEATURE_FLASH_IS_FTFL */

#if FSL_FEATURE_FLASH_IS_FTFE
    addr = gFlashConfig.FlexRAMBlockBase;
    size = gFlashConfig.FlexRAMTotalSize;

    while(size)
    {
        if(kStatus_FLASH_Success != FLASH_EepromWrite(&gFlashConfig, addr, buff, sizeof(buff)))
        {
            return gNVM_FormatFailure_c;
        }
        size -= gNvFlexFormatBufferSize_c;
        addr += gNvFlexFormatBufferSize_c;
    }
#endif /* FSL_FEATURE_FLASH_IS_FTFL */

    return gNVM_OK_c;
#endif /* gNvUseFlexNVM_d */
}

/******************************************************************************
 * Name: __NvIdle
 * Description: Called from the idle task (bare-metal) or NVM_Task (MQX,
 *              FreeRTOS) to process the pending saves, erase or copy
 *              operations.
 * Parameters: -
 * Return: -
 ******************************************************************************/
static void __NvIdle
(
    void
)
{
    NVM_TableEntryInfo_t tblIdx;
    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    uint32_t status;
    #endif
    
    uint64_t currentTimestampValue = 0;
    
#if gNvmUseTimerJitter_d
    uint8_t timerJitter = 0;
#endif

    if (!mNvModuleInitialized || mNvCriticalSectionFlag)
    {
        return;
    }

    #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    if(mNvCopyOperationIsPending)
    {
        #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
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
            #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
            FSCI_MsgNVPageEraseMonitoring(mNvVirtualPageProperty[mNvErasePgCmdStatus.NvPageToErase].NvRawSectorStartAddress, gNVM_OK_c);
            #endif
            return;
        }

        /* erase */
        status = NV_FlashEraseSector(mNvErasePgCmdStatus.NvSectorAddress, (uint32_t) ((uint8_t*) NV_STORAGE_SECTOR_SIZE));

        /* blank check */
        if(kStatus_FLASH_Success == NV_FlashVerifyErase (mNvErasePgCmdStatus.NvSectorAddress, 
                                                        (uint32_t)NV_STORAGE_SECTOR_SIZE
                                                        #ifndef CPU_QN908X
                                                        , kFLASH_MarginValueNormal
                                                        #endif
        ))
        
        {
            mNvErasePgCmdStatus.NvSectorAddress += (uint32_t)((uint8_t*)NV_STORAGE_SECTOR_SIZE);
            return;
        }
    }
    #endif

    /* process the save-on-interval requests */
    if(mNvSaveOnIntervalEvent)
    {
#if gNvmUseTimerJitter_d
      timerJitter = GetRandomRange(0,100);
#endif
      currentTimestampValue = TMR_GetTimestamp();
#if gNvmUseTimerJitter_d
      if(currentTimestampValue - mNvLastTimestampValue >= (uint64_t)((gNvOneSecondInMicros_c + (timerJitter * gNvJitterMultiplier_c)) - (gNvJitterDecrement_c)))
#else
      if(currentTimestampValue - mNvLastTimestampValue >= (uint64_t)gNvOneSecondInMicros_c)
#endif
      {
        mNvSaveOnIntervalEvent = __NvTimerTick(TRUE);
        mNvLastTimestampValue = currentTimestampValue;
      }
    }

    /* process the save-on-idle requests */
    if(NvGetPendingSavesCount(&mNvPendingSavesQueue))
    {
        while(NvPopPendingSave(&mNvPendingSavesQueue, &tblIdx))
        {
            if ((gNvCopyAll_c == tblIdx.entryId) && (gNvCopyAll_c == tblIdx.elementIndex) && (TRUE == tblIdx.saveRestoreAll))
            {
                __NvAtomicSave();
                continue;
            }
            else if (gNvInvalidDataEntry_c == tblIdx.entryId)
            {
                continue;
            }

            #if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
            if(NvWriteRecord(&tblIdx) == gNVM_PageCopyPending_c)
            {
                NvAddSaveRequestToQueue(&tblIdx);
                break;
            }
            #else /* FlexNVM */
            NvWriteRecord(&tblIdx);
            #endif
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
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);

    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIdx;
    uint16_t loopIdx;
    uint16_t remaining_count;

    if(!mNvModuleInitialized)
    {
        return FALSE;
    }

    if(NULL == ptrData)
    {
        return FALSE;
    }

    if(gNVM_OK_c != NvGetEntryFromDataPtr(ptrData, &tblIdx))
    {
        return FALSE;
    }
    else
    {
        tableEntryIdx = NvGetTableEntryIndexFromId(tblIdx.entryId);

        if(gNvInvalidTableEntryIndex_c == tableEntryIdx)
        {
            return FALSE;
        }
        /* Check if is in pendding queue */
        if (mNvPendingSavesQueue.EntriesCount)
        {
            /* Start from the queue's head */
            loopIdx = mNvPendingSavesQueue.Head;
            remaining_count = mNvPendingSavesQueue.EntriesCount;

            while(remaining_count)
            {
                if(mNvPendingSavesQueue.QData[loopIdx].entryId == tblIdx.entryId)
                {
                    return TRUE;
                }
                remaining_count--;
                /* increment and wrap the loop index */
                if(++loopIdx >= (uint8_t)gNvPendingSavesQueueSize_c)
                {
                    loopIdx=0;
                }
            }
        }
        return maDatasetInfo[tableEntryIdx].saveNextInterval;
    }
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
static NVM_Status_t __NvRestoreDataSet
(
void* ptrData,
bool_t restoreAll
)
{
    NVM_TableEntryInfo_t tblIdx;
#if gUnmirroredFeatureSet_d
    uint16_t tableEntryIdx;
#endif
#if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
    NVM_Status_t nvmStatus;
#endif
    
    if(!mNvModuleInitialized)
    {
#if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
        FSCI_MsgNVRestoreMonitoring(0, TRUE, (uint8_t)gNVM_ModuleNotInitialized_c);
#endif
        return gNVM_ModuleNotInitialized_c;
    }
    
    if(NULL == ptrData)
    {
#if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
        FSCI_MsgNVRestoreMonitoring(0, TRUE, (uint8_t)gNVM_NullPointer_c);
#endif
        return gNVM_NullPointer_c;
    }
    
#if gNvFragmentation_Enabled_d
    tblIdx.saveRestoreAll = restoreAll;
#else
    tblIdx.saveRestoreAll = TRUE;
#endif /* gNvFragmentation_Enabled_d */
    
    if(NvGetEntryFromDataPtr(ptrData, &tblIdx) != gNVM_OK_c)
    {
#if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
        FSCI_MsgNVRestoreMonitoring(tblIdx.entryId, TRUE, (uint8_t)gNVM_NullPointer_c);
#endif
        return gNVM_PointerOutOfRange_c;
    }
    
#if gUnmirroredFeatureSet_d
    /*make sure you can't request a full backup for unmirrored data sets*/
    tableEntryIdx = NvGetTableEntryIndexFromId(tblIdx.entryId);
    
    if(gNvInvalidTableEntryIndex_c == tableEntryIdx)
    {
        return gNVM_InvalidTableEntry_c;
    }
    if(gNVM_MirroredInRam_c != pNVM_DataTable[tableEntryIdx].DataEntryType)
    {
        tblIdx.saveRestoreAll = FALSE;
    }
#endif
    
#if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
    {
        FSCI_MsgNVRestoreMonitoring(tblIdx.entryId, TRUE, (uint8_t)gNVM_OK_c);
        nvmStatus=NvRestoreData(&tblIdx);
        FSCI_MsgNVRestoreMonitoring(tblIdx.entryId, FALSE, (uint8_t)nvmStatus);
        return nvmStatus;
    }
#else
    return NvRestoreData(&tblIdx);
#endif
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
static bool_t __NvTimerTick
(
    bool_t countTick
)
{
    bool_t fTicksLeft = FALSE;
    NVM_TableEntryInfo_t tblIdx;
    uint16_t idx = 0;

    if(!mNvModuleInitialized)
    {
        return gNVM_ModuleNotInitialized_c;
    }
    
    while(idx < gNVM_TABLE_entries_c)
    {
        if(countTick)
        {
            if(maDatasetInfo[idx].ticksToNextSave)
            {
                --maDatasetInfo[idx].ticksToNextSave;
            }
        }

        if(maDatasetInfo[idx].saveNextInterval)
        {
            if (maDatasetInfo[idx].ticksToNextSave)
            {
                fTicksLeft = TRUE;
            }
            else
            {
                tblIdx.entryId = pNVM_DataTable[idx].DataEntryID;
                #if gUnmirroredFeatureSet_d
                if (gNVM_MirroredInRam_c != pNVM_DataTable[idx].DataEntryType)
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
                if(!mNvCriticalSectionFlag)
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
static NVM_Status_t __NvSaveOnCount
(
    void* ptrData
)
{
    NVM_Status_t status;
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIdx;

    if(!mNvModuleInitialized)
    {
        return gNVM_ModuleNotInitialized_c;
    }

    if(NULL == ptrData)
    {
        return gNVM_NullPointer_c;
    }

    /* get the NVM table entry */
    if((status = NvGetEntryFromDataPtr(ptrData, &tblIdx)) != gNVM_OK_c)
    {
        return status;
    }

    if(gNvInvalidDataEntry_c == tblIdx.entryId)
    {
        return gNVM_InvalidTableEntry_c;
    }

    tableEntryIdx = NvGetTableEntryIndexFromId(tblIdx.entryId);

    if(gNvInvalidTableEntryIndex_c == tableEntryIdx)
    {
        return gNVM_InvalidTableEntry_c;
    }

    if(maDatasetInfo[tableEntryIdx].countsToNextSave)
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
static NVM_Status_t __NvSaveOnInterval
(
    void* ptrData
)
{
    NVM_Status_t status;
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIdx;

    if(!mNvModuleInitialized)
    {
        return gNVM_ModuleNotInitialized_c;
    }

    if(NULL == ptrData)
    {
        return gNVM_NullPointer_c;
    }

    /* get the NVM table entry */
    if((status = NvGetEntryFromDataPtr(ptrData, &tblIdx)) != gNVM_OK_c)
    {
        return status;
    }

    if(gNvInvalidDataEntry_c == tblIdx.entryId)
    {
        return gNVM_InvalidTableEntry_c;
    }

    tableEntryIdx = NvGetTableEntryIndexFromId(tblIdx.entryId);

    if(gNvInvalidTableEntryIndex_c == tableEntryIdx)
    {
        return gNVM_InvalidTableEntry_c;
    }

    if(maDatasetInfo[tableEntryIdx].saveNextInterval == FALSE)
    {
        maDatasetInfo[tableEntryIdx].ticksToNextSave = gNvMinimumTicksBetweenSaves;
        maDatasetInfo[tableEntryIdx].saveNextInterval = TRUE;
#if gUnmirroredFeatureSet_d
        if (gNVM_MirroredInRam_c != pNVM_DataTable[tableEntryIdx].DataEntryType)
        {
            maDatasetInfo[tableEntryIdx].elementIndex = tblIdx.elementIndex;
        }
#endif
        mNvSaveOnIntervalEvent = TRUE;
        mNvLastTimestampValue = TMR_GetTimestamp();
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
static NVM_Status_t __NvSaveOnIdle
(
    void* ptrData,
    bool_t saveAll
)
{
    NVM_Status_t status;
    NVM_TableEntryInfo_t tblIdx;

    if(!mNvModuleInitialized)
    {
        return gNVM_ModuleNotInitialized_c;
    }

    if(NULL == ptrData)
    {
        return gNVM_NullPointer_c;
    }

    /* get the NVM table entry */
    if((status = NvGetEntryFromDataPtr(ptrData, &tblIdx)) != gNVM_OK_c)
    {
        return status;
    }

    if(gNvInvalidDataEntry_c == tblIdx.entryId)
    {
        return gNVM_InvalidTableEntry_c;
    }

    /* write the save all flag */
    #if gNvFragmentation_Enabled_d
    tblIdx.saveRestoreAll = saveAll;
    #else
    tblIdx.saveRestoreAll = TRUE;
    #endif /* gNvFragmentation_Enabled_d */

    return NvAddSaveRequestToQueue(&tblIdx);
}

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
#if defined ( __IAR_SYSTEMS_ICC__ )
#pragma required=eraseNVMFirst
#pragma required=eraseNVMSecond
#endif /* __IAR_SYSTEMS_ICC__ */
static NVM_Status_t __NvModuleInit
(
    void
)
{
    uint16_t loopCnt;    
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    uint32_t pageFreeSpace;
    NVM_Status_t status;
#else
    flash_security_state_t securityStatus;
    status_t result;
    uint32_t eepromTotalSize;
#endif
    
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
#if defined ( __IAR_SYSTEMS_ICC__ )
    (void)eraseNVMFirst;
    (void)eraseNVMSecond;
#endif
#endif
    
    if(mNvModuleInitialized)
    {
        return gNVM_ModuleAlreadyInitialized_c;
    }
    
    if((gNVM_TABLE_entries_c == 0) || (gNVM_TABLE_entries_c >= gNvTableEntriesCountMax_c))
    {
        return gNVM_InvalidTableEntriesCount_c;
    }

#if ((gNvUseFlexNVM_d == FALSE) && (gNvFragmentation_Enabled_d == TRUE))
    for(loopCnt = 0; loopCnt < gNVM_TABLE_entries_c; loopCnt++)
    {
        if (pNVM_DataTable[loopCnt].ElementsCount > gNvRecordsCopiedBufferSize_c)
        {
            return gNVM_DefragBufferTooSmall_c;
        }
    }
#endif
    
    /* Initialize the pending saves queue */
    NvInitPendingSavesQueue(&mNvPendingSavesQueue);
    
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
    NV_Init();

#if FSL_FEATURE_FLASH_IS_FTFL && (DEBLOCK_SIZE != 0)
    /* check if the flash registers are configured to support FlexNVM as EEPROM,
    if not enable this feature*/
    if (0 == gFlashConfig.EEESize)
    {
        DEFlashPartition(&gFlashConfig, gNvEepromDatasetSizeCode_c, gNvEepromBackupSizeCode_c, gFlashLaunchCommand);
    }
    /* check if the NVM table fits within the size of the FlexRAM window */
    if(gNVM_OK_c != NvCheckNvmTableForFlexRAMUsage())
    {
        return gNVM_NvTableExceedFlexRAMSize_c;
    }
    
    /* check data flash IFR map */
    if(gFlashConfig.EEESize == 0)
    {
        return gNVM_NvWrongFlashDataIFRMap_c;
    }
    
    /* Enable the EERAM */
    SetEEEEnable(&gFlashConfig, EEE_ENABLE, FlashCommandSequence);

#endif /* FSL_FEATURE_FLASH_IS_FTFL */

#if FSL_FEATURE_FLASH_IS_FTFE
    securityStatus = kFLASH_SecurityStateNotSecure;

    /* Check security status. */
    result = FLASH_GetSecurityState(&gFlashConfig, &securityStatus);
    if (kStatus_FLASH_Success != result)
    {
      return gNVM_Error_c;
    }
    if (kFLASH_SecurityStateNotSecure != securityStatus)
    {
      return gNVM_FlashMemoryIsSecured_c;
    }

    /* Get total EEPROM size */
    FLASH_GetProperty(&gFlashConfig, kFLASH_PropertyEepromTotalSize, &eepromTotalSize);

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
      result = FLASH_ProgramPartition(&gFlashConfig, kFLASH_PartitionFlexramLoadOptionLoadedWithValidEepromData,
                                      eepromDataSizeCode, flexnvmPartitionCode);
      /* Enable interrupts */
      OSA_InterruptEnable();

      if (kStatus_FLASH_Success != result)
      {
          return gNVM_FlexNVMPartitioningFail_c;
      }

      /* Reset the MCU so that the changes become effective */
      ResetMCU();
    }

    /* check if the NVM table fits within the size of the FlexRAM window */
    if(gNVM_OK_c != NvCheckNvmTableForFlexRAMUsage())
    {
        return gNVM_NvTableExceedFlexRAMSize_c;
    }

    /* Enable the EERAM */
    FLASH_SetFlexramFunction(&gFlashConfig, kFLASH_FlexramFunctionOptionAvailableForEeprom);

#endif /* FSL_FEATURE_FLASH_IS_FTFE */

    /* NVM module is now initialized */
    mNvModuleInitialized = TRUE;

    return gNVM_OK_c;
    
#else /* no FlexNVM */
    
    /* check linker file symbol definition for sector count; it should be multiple of 2 */
    if (((uint32_t)NV_STORAGE_MAX_SECTORS) & 0x1)
    {
        return gNVM_InvalidSectorsCount_c;
    }

    /* Init the NVM configuration */
    InitNVMConfig();
    
    /* both pages are not valid, format the NV storage system */
    if (mNvActivePageId == gVirtualPageNone_c)
    {
        mNvActivePageId = gFirstVirtualPage_c;
        status = NvInternalFormat(0);
        if (status != gNVM_OK_c)
            return status;
    }
#if gNvUseExtendedFeatureSet_d
    /* get the size of the NV table stored in FLASH memory */
    mNvTableSizeInFlash = NvGetFlashTableSize();
    
    if(0 == mNvTableSizeInFlash) /* no NV table found in FLASH, format the system */
    {
        status = NvInternalFormat(0); /* will also save the NV table to FLASH memory */
        if (status != gNVM_OK_c)
            return status;
    }
    else /* found an valid NV table in FLASH memory */
    {
        /* check if the RAM table was updated (e.g. new binary image via OTA) */
        mNvTableUpdated = (GetFlashTableVersion() != mNvFlashTableVersion) || NvIsRamTableUpdated();
        if( mNvTableUpdated )
        {
            if(gNVM_OK_c == NvUpdateLastMetaInfoAddress())
            {
                /* copy the new RAM table and the page content */
#if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
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
                return status;
            }
            
            /* format the system */
            return NvInternalFormat(0);
        }
    }
#endif /* gNvUseExtendedFeatureSet_d */
    
    /* get the last meta information address */
    if(gNVM_OK_c == (status = NvUpdateLastMetaInfoAddress()))
    {
        /* NVM module is now initialized */
        mNvModuleInitialized = TRUE;
        
        /* get active page free space */
        if(gNVM_OK_c != (status = NvGetPageFreeSpace(&pageFreeSpace)))
        {
            return status;
        }
        if(pageFreeSpace < gNvMinimumFreeBytesCountStart_c )
        {
#if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
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
        return status;
    }
    else
    {
        /* format the system */
        return NvInternalFormat(0);
    }
#endif
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
bool_t NvIsRecordErased
(
    uint16_t srcTableEntryIdx,
    uint16_t srcTableEntryElementIdx,
    uint32_t srcMetaAddress
)
{
    NVM_RecordMetaInfo_t srcMetaInfo;
    while(srcMetaAddress < (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress))
    {
        (void)NvGetMetaInfo(mNvActivePageId, srcMetaAddress, &srcMetaInfo);
        if (gNvGuardValue_c == srcMetaInfo.rawValue)
        {
            break;
        }
        if ((srcMetaInfo.fields.NvmRecordOffset == 0) &&
            (srcMetaInfo.fields.NvmElementIndex == srcTableEntryElementIdx) &&
            (srcMetaInfo.fields.NvmDataEntryID == pNVM_DataTable[srcTableEntryIdx].DataEntryID))
        {
            return TRUE;
        }
        srcMetaAddress += sizeof(NVM_RecordMetaInfo_t);
    }
    return FALSE;
}

/******************************************************************************
 * Name: __NvmRestoreUnmirrored
 * Description: Restores all unmirrored entries that should be restored at init
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
static void __NvmRestoreUnmirrored
(
    void
)
{
    uint32_t metaInfoAddress;
    uint16_t tableEntryIdx;
    NVM_RecordMetaInfo_t metaInfo;
    uint16_t loopCnt = 0;
    uint16_t loopCnt2;
    const uint32_t erasedElement = 0xFFFFFFFF;

    /* get the last meta information address */
    if((metaInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress) == gEmptyPageMetaAddress_c)
    {
        /* blank page, no data to restore */
        return;
    }

    /* parse meta info backwards until the element is found */
    while(metaInfoAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
    {
        /* get the meta information */
        NvGetMetaInfo(mNvActivePageId, metaInfoAddress, &metaInfo);
        if ((metaInfo.fields.NvValidationStartByte != metaInfo.fields.NvValidationEndByte) ||
            (metaInfo.fields.NvValidationStartByte != gValidationByteSingleRecord_c))
        {
            metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        /* get table entry information */
        tableEntryIdx = NvGetTableEntryIndexFromId(metaInfo.fields.NvmDataEntryID);
        if ((gNvInvalidTableEntryIndex_c == tableEntryIdx) ||
            (gNVM_NotMirroredInRamAutoRestore_c != pNVM_DataTable[tableEntryIdx].DataEntryType))
        {
            metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        /* if it was allready restored, ignore it */
        if (NvIsNVMFlashAddress(((void**)pNVM_DataTable[tableEntryIdx].pData)[metaInfo.fields.NvmElementIndex]) ||
            (erasedElement == (uint32_t)((void**)pNVM_DataTable[tableEntryIdx].pData)[metaInfo.fields.NvmElementIndex]))
        {
            metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        /* erased element */
        if (!metaInfo.fields.NvmRecordOffset)
        {
            ((void**)pNVM_DataTable[tableEntryIdx].pData)[metaInfo.fields.NvmElementIndex] = (void*)erasedElement;
        }
        else
        {
            ((void**)pNVM_DataTable[tableEntryIdx].pData)[metaInfo.fields.NvmElementIndex] =
                (void*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset);
        }

        /* move to the previous meta info */
        metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
    }
    
    while(loopCnt < gNVM_TABLE_entries_c)
    {
        if(gNVM_NotMirroredInRamAutoRestore_c == pNVM_DataTable[loopCnt].DataEntryType)
        {
            for (loopCnt2=0; loopCnt2<pNVM_DataTable[loopCnt].ElementsCount; loopCnt2++)
            {
                if (erasedElement == (uint32_t)((void**)pNVM_DataTable[loopCnt].pData)[loopCnt2])
                {
                    ((void**)pNVM_DataTable[loopCnt].pData)[loopCnt2] = NULL;
                }
            }
        }
        /* increment the loop counter */
        loopCnt++;
    }
}
/******************************************************************************
 * Name: __NvmMoveToRam
 * Description: Move from NVM to Ram an unmirrored dataset
 * Parameter(s):  ppData     double pointer to the entity to be moved from flash to RAM
 * Return: pointer to Ram location
 *****************************************************************************/
static NVM_Status_t __NvmMoveToRam
(
    void** ppData
)
{
    NVM_TableEntryInfo_t tblIdx;
    uint16_t tableEntryIndex;
    NVM_Status_t status;
    void* pData=NULL;
    uint16_t loopIdx;
    uint16_t remaining_count;

    /* Get entry from NVM table */
    if((status = NvGetEntryFromDataPtr(ppData, &tblIdx)) != gNVM_OK_c)
    {
        return status;
    }

    if((tableEntryIndex = NvGetTableEntryIndexFromId(tblIdx.entryId)) == gNvInvalidTableEntryIndex_c)
    {
        return gNVM_InvalidTableEntry_c;
    }

    if(gNVM_MirroredInRam_c == pNVM_DataTable[tableEntryIndex].DataEntryType)
    {
        return gNVM_IsMirroredDataSet_c;
    }

    /* Check if entry is in ram  */
    if(!NvIsNVMFlashAddress(*ppData)&&(*ppData != NULL))
    {
        /* Check if is in pendding queue - if yes than remove it */
        if (NvGetPendingSavesCount(&mNvPendingSavesQueue))
        {
            /* Start from the queue's head */
            loopIdx = mNvPendingSavesQueue.Head;
            remaining_count = mNvPendingSavesQueue.EntriesCount;

            while(remaining_count)
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
        return gNVM_OK_c;
    }

    /* Allocate a buffer for the data set */
    pData = MEM_BufferAllocWithId(pNVM_DataTable[tableEntryIndex].ElementSize , gNvmMemPoolId_c, (void*)__get_LR());
    if(pData == NULL)
    {
        return gNVM_NoMemory_c;
    }

    /* Write from Flash to Ram */
    if(*ppData != NULL)
    {
        FLib_MemCpy(pData, *ppData, pNVM_DataTable[tableEntryIndex].ElementSize);
    }

    OSA_InterruptDisable();
    *ppData = pData;
    OSA_InterruptEnable();
    /* Check if the address is in ram */
    return gNVM_OK_c;
}

/******************************************************************************
 * Name: __NvmErase
 * Description: Erase from NVM an unmirrored dataset
 * Parameter(s):  ppData     double pointer to the entity to be erased
 * Return: pointer to Ram location
 *****************************************************************************/
static NVM_Status_t __NvmErase
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
    if((status = NvGetEntryFromDataPtr(ppData, &tblIdx)) != gNVM_OK_c)
    {
        return status;
    }

    if((tableEntryIndex = NvGetTableEntryIndexFromId(tblIdx.entryId)) == gNvInvalidTableEntryIndex_c)
    {
        return gNVM_InvalidTableEntry_c;
    }

    if(gNVM_MirroredInRam_c == pNVM_DataTable[tableEntryIndex].DataEntryType)
    {
        return gNVM_IsMirroredDataSet_c;
    }

    if(!NvIsNVMFlashAddress(*ppData))
    {
        if(*ppData != NULL)
        {
            MSG_Free(*ppData);
        }
        OSA_InterruptDisable();
        *ppData = NULL;
        OSA_InterruptEnable();
        return gNVM_OK_c;
    }

    /* Check if is in pending queue - if yes than remove it */
    if (NvGetPendingSavesCount(&mNvPendingSavesQueue))
    {
        /* Start from the queue's head */
        loopCnt = mNvPendingSavesQueue.Head;
        remaining_count = mNvPendingSavesQueue.EntriesCount;
        while (remaining_count)
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
    return __NvSyncSave(ppData, FALSE);
}

/******************************************************************************
 * Name: NvIsNVMFlashAddress
 * Description: check if the address is in Flash
 * Parameter(s): [IN] address
 *
 * Return: TRUE if the table entry is in Flash / FALSE otherwise
 ******************************************************************************/
static bool_t NvIsNVMFlashAddress
(
    void* address
)
{
    uint8_t idx;
    for(idx=0; idx < gNvVirtualPagesCount_c; idx ++)
    {
        if( ((uint32_t)address > mNvVirtualPageProperty[idx].NvRawSectorStartAddress) &&
           ((uint32_t)address < mNvVirtualPageProperty[idx].NvRawSectorEndAddress))
        {
            return TRUE;
        }
    }
    return FALSE;
}
#endif


/******************************************************************************
 * Name: NvInitPendingSavesQueue
 * Description: Initialize the pending saves queue
 * Parameters: [IN] pQueue - pointer to queue
 * Return: TRUE if the pointer is valid, FALSE otherwise
 ******************************************************************************/
static bool_t NvInitPendingSavesQueue
(
    NVM_SaveQueue_t *pQueue
)
{
    if(NULL == pQueue)
    {
        return FALSE;
    }

    pQueue->Head = 0;
    pQueue->Tail = 0;
    pQueue->EntriesCount = 0;

    return TRUE;
}

/******************************************************************************
 * Name: NvPushPendingSave
 * Description: Add a new pending save to the queue
 * Parameters: [IN] pQueue - pointer to queue
 *             [IN] data - data to be saved
 * Return: TRUE if the push operation succeeded, FALSE otherwise
 ******************************************************************************/
static bool_t NvPushPendingSave
(
    NVM_SaveQueue_t *pQueue,
    NVM_TableEntryInfo_t data
)
{
    if(NULL == pQueue)
    {
        return FALSE;
    }

    if((pQueue->Tail == pQueue->Head) && (pQueue->EntriesCount > 0))
    {
#if gFifoOverwriteEnabled_c
        /* Increment and wrap the head when it reaches gNvPendingSavesQueueSize_c */
        if(++pQueue->Head >= (uint16_t)gNvPendingSavesQueueSize_c)
        {
            pQueue->Head=0;
        }
#else
        return FALSE;
#endif
    }

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

    return TRUE;
}


/******************************************************************************
 * Name: NvPopPendingSave
 * Description: Retrieves the head element from the pending saves queue
 * Parameters: [IN] pQueue - pointer to queue
 *             [OUT] pData - pointer to the location where data will be placed
 * Return: TRUE if the pop operation succeeded, FALSE otherwise
 ******************************************************************************/
static bool_t NvPopPendingSave
(
    NVM_SaveQueue_t *pQueue,
    NVM_TableEntryInfo_t *pData
)
{
    if( (NULL == pQueue) || (pQueue->EntriesCount <= 0) || ((NULL == pData)) )
    {
        return FALSE;
    }

    *pData = pQueue->QData[pQueue->Head];

    /* Increment and wrap the head when it reaches gNvPendingSavesQueueSize_c */
    if(++pQueue->Head >= (uint16_t)gNvPendingSavesQueueSize_c)
    {
        pQueue->Head=0;
    }

    /* Decrement the entries count */
    pQueue->EntriesCount--;

    return TRUE;
}
/******************************************************************************
 * Name: NvGetPendingSavesCount
 * Description: self explanatory
 * Parameters: [IN] pQueue - pointer to queue
 * Return: Number of pending saves
 ******************************************************************************/
static uint16_t NvGetPendingSavesCount
(
    NVM_SaveQueue_t *pQueue
)
{
    if( NULL == pQueue )
    {
        return 0;
    }
    return pQueue->EntriesCount;
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
static void InitNVMConfig
(
    void
)
{
    if (mNvFlashConfigInitialised)
        return;
    /* Initialize flash HAL driver */
    NV_Init();

    /* no pending erase operations on system initialisation */
    mNvErasePgCmdStatus.NvErasePending = FALSE;

    /* Initialize the active page ID */
    mNvActivePageId = gVirtualPageNone_c;

    /* First virtual page initialisation */
    mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorStartAddress = (uint32_t)((uint8_t*)NV_STORAGE_END_ADDRESS);
    mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorsCount = (uint32_t)((uint8_t*) NV_STORAGE_MAX_SECTORS) >> 1;
    mNvVirtualPageProperty[gFirstVirtualPage_c].NvTotalPageSize = mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorsCount *
        (uint32_t)((uint8_t*)NV_STORAGE_SECTOR_SIZE);
    mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorEndAddress = mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorStartAddress +
        mNvVirtualPageProperty[gFirstVirtualPage_c].NvTotalPageSize - 1;

    /* Second virtual page initialisation */
    mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorStartAddress = mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorEndAddress + 1;
    mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorsCount = (uint32_t)((uint8_t*) NV_STORAGE_MAX_SECTORS) >> 1;
    mNvVirtualPageProperty[gSecondVirtualPage_c].NvTotalPageSize = mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorsCount *
        (uint32_t)((uint8_t*)NV_STORAGE_SECTOR_SIZE);
    mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorEndAddress = mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorStartAddress +
        mNvVirtualPageProperty[gSecondVirtualPage_c].NvTotalPageSize - 1;

    /* Initialize the storage system: get active page and page counter */
    NvInitStorageSystem(FALSE);
    if (mNvActivePageId == gVirtualPageNone_c)
    {
        /* maybe the table is in old format and needs to be updated */
        NvInitStorageSystem(TRUE);
        if (mNvActivePageId != gVirtualPageNone_c)
        {
            UpgradeLegacyTable();
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

/******************************************************************************
 * Name: NvUpdateSize
 * Description: Updates the size to be a multiple of 4 or 8 depending on the flash controller
 * Parameter(s): [IN] size - size to be updated
 * Return: the computed size
 *****************************************************************************/
static uint32_t NvUpdateSize
(
    uint32_t size
)
{
    /* compute the size that will be actually written on FLASH memory */
    uint8_t paddingBytes = size % (uint8_t)PGM_SIZE_BYTE;

    if(paddingBytes)
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
static NVM_Status_t NvEraseVirtualPage
(
    NVM_VirtualPageID_t pageID
)
{
    uint32_t status;

    if(pageID > gSecondVirtualPage_c)
        return gNVM_InvalidPageID_c;

    /* erase virtual page */
    status = NV_FlashEraseSector(mNvVirtualPageProperty[pageID].NvRawSectorStartAddress,
                                 mNvVirtualPageProperty[pageID].NvTotalPageSize);
    if(kStatus_FLASH_Success != status)
    {
        #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
            FSCI_MsgNVPageEraseMonitoring(mNvVirtualPageProperty[pageID].NvRawSectorStartAddress, status);
        #endif
        return gNVM_SectorEraseFail_c;
    }

    if(gNVM_OK_c != (status = NvVirtualPageBlankCheck(pageID)))
    {
        #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
            FSCI_MsgNVPageEraseMonitoring(mNvVirtualPageProperty[pageID].NvRawSectorStartAddress, status);
        #endif
        return (NVM_Status_t)status;
    }
    #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
        FSCI_MsgNVPageEraseMonitoring(mNvVirtualPageProperty[pageID].NvRawSectorStartAddress, status);
    #endif
    return gNVM_OK_c;
}


/******************************************************************************
 * Name: NvInitStorageSystem
 * Description: Initialize the storage system, retrieve the active page and
 *              the page counter. Called once by NvModuleInit() function.
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
static void NvInitStorageSystem
(
    bool_t read_legacy_location
)
{
    uint32_t value;
    uint32_t firstPageCounterTopValue;
    uint32_t firstPageCounterBottomValue;
    uint32_t secondPageCounterTopValue;
    uint32_t secondPageCounterBottomValue;
    uint8_t location_offset = (read_legacy_location ? gNvLegacyOffset_c : 0);

    /* read both pages counter values */
    NV_FlashRead(mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorStartAddress, (uint8_t*)&value,
                 sizeof(value));
    firstPageCounterTopValue = value;
    NV_FlashRead(mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1 + location_offset,
                (uint8_t*)&value, sizeof(value));
    firstPageCounterBottomValue = value;

    NV_FlashRead(mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorStartAddress, (uint8_t*)&value,
                 sizeof(value));
    secondPageCounterTopValue = value;
    NV_FlashRead(mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1 + location_offset,
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
                return;
            }

            /* second page is active */
            mNvPageCounter = secondPageCounterTopValue;
            mNvActivePageId = gSecondVirtualPage_c;
            return;
        }

        if(secondPageCounterTopValue != secondPageCounterBottomValue)
        {
            /* first page is active */
            mNvPageCounter = firstPageCounterTopValue;
            mNvActivePageId = gFirstVirtualPage_c;
            /* request the erase of the second page */
            if (read_legacy_location)
            {
               mNvErasePgCmdStatus.NvPageToErase = gSecondVirtualPage_c;
               mNvErasePgCmdStatus.NvSectorAddress = mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorStartAddress;
               mNvErasePgCmdStatus.NvErasePending = TRUE;
            }
            return;
        }

        /* first page is active */
        mNvPageCounter = firstPageCounterTopValue;
        mNvActivePageId = gFirstVirtualPage_c;

        if (read_legacy_location)
        {
            if(gNVM_PageIsNotBlank_c == NvVirtualPageBlankCheck(gSecondVirtualPage_c))
            {
               /* request the erase of the second page */
               mNvErasePgCmdStatus.NvPageToErase = gSecondVirtualPage_c;
               mNvErasePgCmdStatus.NvSectorAddress = mNvVirtualPageProperty[gSecondVirtualPage_c].NvRawSectorStartAddress;
               mNvErasePgCmdStatus.NvErasePending = TRUE;
            }
        }
        return;
    }

    if(firstPageCounterTopValue != firstPageCounterBottomValue) /* first page is not valid */
    {
        if((secondPageCounterTopValue == secondPageCounterBottomValue) && (gPageCounterMaxValue_c != secondPageCounterTopValue)) /* second page is valid */
        {
            /* second page is active */
            mNvPageCounter = secondPageCounterTopValue;
            mNvActivePageId = gSecondVirtualPage_c;
            /* request the erase of the first page */
            if (read_legacy_location)
            {
               mNvErasePgCmdStatus.NvPageToErase = gFirstVirtualPage_c;
               mNvErasePgCmdStatus.NvSectorAddress = mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorStartAddress;
               mNvErasePgCmdStatus.NvErasePending = TRUE;
            }
            return;
        }
        mNvActivePageId = gVirtualPageNone_c;
        return;
    }

    if((secondPageCounterTopValue == secondPageCounterBottomValue) && (gPageCounterMaxValue_c != secondPageCounterTopValue)) /* second page is valid */
    {
        /* second page is active */
        mNvPageCounter = secondPageCounterTopValue;
        mNvActivePageId = gSecondVirtualPage_c;

        if (read_legacy_location)
        {
            if(gNVM_PageIsNotBlank_c == NvVirtualPageBlankCheck(gFirstVirtualPage_c))
            {
               /* request the erase of the first page */
               mNvErasePgCmdStatus.NvPageToErase = gFirstVirtualPage_c;
               mNvErasePgCmdStatus.NvSectorAddress = mNvVirtualPageProperty[gFirstVirtualPage_c].NvRawSectorStartAddress;
               mNvErasePgCmdStatus.NvErasePending = TRUE;
            }
        }
        return;
    }

    mNvActivePageId = gVirtualPageNone_c;
}

/******************************************************************************
 * Name: NvVirtualPageBlankCheck
 * Description: checks if the specified page is blank (erased)
 * Parameter(s): [IN] pageID - the ID of the page to be checked
 * Return: gNVM_InvalidPageID_c - if the page ID is not valid
 *         gNVM_PageIsNotBlank_c - if the page is not blank
 *         gNVM_OK_c - if the page is blank (erased)
 *****************************************************************************/
static NVM_Status_t NvVirtualPageBlankCheck
(
    NVM_VirtualPageID_t pageID
)
{
    if(pageID > gSecondVirtualPage_c)
        return gNVM_InvalidPageID_c;


    if(kStatus_FLASH_Success != NV_FlashVerifyErase ( mNvVirtualPageProperty[pageID].NvRawSectorStartAddress, 
                                                      mNvVirtualPageProperty[pageID].NvTotalPageSize 
                                                      #ifndef CPU_QN908X
                                                      , kFLASH_MarginValueNormal
                                                      #endif
    ))
    {
        return gNVM_PageIsNotBlank_c;
    }

    return gNVM_OK_c;
}


/******************************************************************************
 * Name: NvUpdateLastMetaInfoAddress
 * Description: retrieve and store (update) the last meta information address
 * Parameter(s): -
 * Return: gNVM_MetaNotFound_c - if no meta information has been found
 *         gNVM_OK_c - if the meta was found and stored (updated)
 *****************************************************************************/
static NVM_Status_t NvUpdateLastMetaInfoAddress
(
    void
)
{
    NVM_RecordMetaInfo_t metaValue;
    uint32_t readAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c;

    while(readAddress < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
    {
        NV_FlashRead(readAddress, (uint8_t*)&metaValue, sizeof(metaValue));

        if(gNvGuardValue_c == metaValue.rawValue)
        {
            if(readAddress == (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
            {
                mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress = gEmptyPageMetaAddress_c;
                #if gUnmirroredFeatureSet_d
                    mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress = gEmptyPageMetaAddress_c;
                #endif
                return gNVM_OK_c;
            }

            readAddress -= sizeof(NVM_RecordMetaInfo_t);

            while(readAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
            {
                NV_FlashRead(readAddress, (uint8_t*)&metaValue, sizeof(metaValue));

                if((metaValue.fields.NvValidationStartByte == metaValue.fields.NvValidationEndByte) &&
                   ((gValidationByteSingleRecord_c == metaValue.fields.NvValidationStartByte) ||
                    (gValidationByteAllRecords_c == metaValue.fields.NvValidationStartByte)))
                {
                    mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress = readAddress;
                    #if gUnmirroredFeatureSet_d
                    {
                        while(readAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
                        {
                            if(metaValue.fields.NvmRecordOffset == 0)
                            {
                                readAddress -= sizeof(NVM_RecordMetaInfo_t);
                                NV_FlashRead(readAddress, (uint8_t*)&metaValue, sizeof(metaValue));
                            }
                            else
                            {
                                mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress = readAddress;
                                break;
                            }
                        }
                    }
                    #endif
                    return gNVM_OK_c;
                }
                readAddress -= sizeof(NVM_RecordMetaInfo_t);
            }
            return gNVM_MetaNotFound_c;
        }
        readAddress += sizeof(NVM_RecordMetaInfo_t);
    }
    return gNVM_MetaNotFound_c;
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
static NVM_Status_t NvGetMetaInfo
(
    NVM_VirtualPageID_t pageId,
    uint32_t metaInfoAddress,
    NVM_RecordMetaInfo_t* pMetaInfo
)
{
    /* check address range */
    if(metaInfoAddress < (mNvVirtualPageProperty[pageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c) ||
       metaInfoAddress > mNvVirtualPageProperty[pageId].NvRawSectorEndAddress)
    {
        return gNVM_AddressOutOfRange_c;
    }

    /* read the meta information tag */
    NV_FlashRead(metaInfoAddress, (uint8_t*)pMetaInfo, sizeof(NVM_RecordMetaInfo_t));

    return gNVM_OK_c;
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
static NVM_Status_t NvGetPageFreeSpace
(
    uint32_t* ptrFreeSpace
)
{
    NVM_RecordMetaInfo_t metaInfo;
    NVM_Status_t retVal;
    #if gUnmirroredFeatureSet_d
    NVM_RecordMetaInfo_t metaInfoUndeleted;
    NVM_Status_t ret;
    #endif

    if(gEmptyPageMetaAddress_c == mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress)
    {
        #if gNvUseExtendedFeatureSet_d
            *ptrFreeSpace = mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize - mNvTableSizeInFlash - 3*sizeof(NVM_TableInfo_t);
        #else
            *ptrFreeSpace = mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize - 2*sizeof(NVM_TableInfo_t);
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
                if(metaInfo.fields.NvmRecordOffset == 0)
                {
                    ret=NvGetMetaInfo(mNvActivePageId, mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress, &metaInfoUndeleted);
                    if(gNVM_OK_c != ret)
                    {
                        *ptrFreeSpace = 0;
                        return ret;
                    }
                    else
                    {
                        metaInfo.fields.NvmRecordOffset = metaInfoUndeleted.fields.NvmRecordOffset;
                    }
                }
            }
            #endif
            *ptrFreeSpace = (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset) -
                (mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress +
                 sizeof(NVM_RecordMetaInfo_t));
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
static bool_t NvIsMemoryAreaAvailable
(
    uint32_t address,
    uint32_t len
)
{
    NV_baseType readBuffer;

    if(len % (uint8_t)PGM_SIZE_BYTE != 0)
    {
        return FALSE;
    }


    if(address < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress ||
       address > mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
    {
        return FALSE;
    }

    if((address + len) > mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress + 1)
    {
        return FALSE;
    }

    while(len)
    {

        readBuffer = *(NV_baseType*)address;
        /*the buffer must have only 1 bits*/
        if (~readBuffer)
            return FALSE;
        len-=(uint8_t)PGM_SIZE_BYTE;
        address+=(uint8_t)PGM_SIZE_BYTE;
    }
    return TRUE;
}


/******************************************************************************
 * Name: NvIsRecordCopied
 * Description: Checks if a record or an entire table entry is already copied.
 *              Called by page copy function.
 * Parameter(s): [IN] pageId - the ID of the page where to perform the check
 *               [IN] metaInf - a pointer to source page meta information tag
 * Return: TRUE if the element is already copied, FALSE otherwise
 *****************************************************************************/
static bool_t NvIsRecordCopied
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
        NV_FlashRead(loopAddress, (uint8_t*)&metaValue, sizeof(NVM_RecordMetaInfo_t));

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
static NVM_Status_t NvInternalCopy
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

    /* Initialize the inner offset*/
    innerOffset = 0;

    /* prepare destination page meta info tag and write if after the record is entirely written.
    * the preparation is made here because the 'dstAddress' may change afterwards
    */
    dstMetaInfo.fields = srcMetaInfo->fields;
    dstMetaInfo.fields.NvmRecordOffset = dstAddress - mNvVirtualPageProperty[(mNvActivePageId+1)%2].NvRawSectorStartAddress;

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

    while(size)
    {
        if(size > (uint16_t)gNvCacheBufferSize_c)
        {
            /* copy from FLASH to cache buffer */
            NV_FlashRead(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + srcMetaInfo->fields.NvmRecordOffset + innerOffset,
                         (uint8_t*)&cacheBuffer[0], (uint16_t)gNvCacheBufferSize_c);

            /* write to destination page */
            if(kStatus_FLASH_Success == NV_FlashProgramUnaligned(dstAddress, (uint16_t)gNvCacheBufferSize_c, cacheBuffer))
            {
                /* update the destination record address copy */
                dstAddress += (uint16_t)gNvCacheBufferSize_c;
                /* update the record size */
                size -= (uint16_t)gNvCacheBufferSize_c;
                /* update the inner offset value */
                innerOffset += (uint16_t)gNvCacheBufferSize_c;

                continue;
            }
            return gNVM_RecordWriteError_c;
        }
        else
        {
            /* copy from FLASH to cache buffer */
            NV_FlashRead(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + srcMetaInfo->fields.NvmRecordOffset + innerOffset,
                         (uint8_t*)&cacheBuffer[0], size);
            /* write to destination page */
            if(kStatus_FLASH_Success == NV_FlashProgramUnaligned(dstAddress, (uint16_t)size, cacheBuffer))
            {
                break;
            }
            return gNVM_RecordWriteError_c;
        }
    }

    if(diffSize)
    {
        /* update the destination record address copy */
        dstAddress += size;

        /* check alignment and adjust it if necessary */
        misalignedBytes = dstAddress - (dstAddress & ((uint32_t)~(PGM_SIZE_BYTE-1)));

        /* initialise the inner offset */
        innerOffset = 0;

        /* check if the destination is longword aligned or not */
        if(misalignedBytes)
        {
            /* align to previous 32 bit boundary */
            dstAddress &= ((uint32_t)~(PGM_SIZE_BYTE-1));

            /* compute the loop end */
            loopEnd = PGM_SIZE_BYTE - misalignedBytes;

            /* read from destination page to cache buffer */
            NV_FlashRead(dstAddress, (uint8_t*)&cacheBuffer[0], PGM_SIZE_BYTE);

            /* update with data from RAM */
            for(loopIdx = 0; loopIdx < loopEnd; loopIdx++)
            {
                cacheBuffer[misalignedBytes] = *((uint8_t*)pNVM_DataTable[srcTblEntryIdx].pData +
                                                 (diffIdx * pNVM_DataTable[srcTblEntryIdx].ElementSize) + innerOffset);
                innerOffset++;
                misalignedBytes++;
                if (innerOffset == diffSize)
                    break;
            }

            /* write to Flash destination page */
            if(kStatus_FLASH_Success != NV_FlashProgramUnaligned(dstAddress, PGM_SIZE_BYTE, cacheBuffer))
            {
                return gNVM_RecordWriteError_c;
            }

            /* align to next 32 bit boundary */
            dstAddress += PGM_SIZE_BYTE;
        }

        /* write to Flash destination page the rest of the alligned data */
        if(kStatus_FLASH_Success != NV_FlashProgramUnaligned(dstAddress, diffSize - innerOffset,
                                               ((uint8_t*)pNVM_DataTable[srcTblEntryIdx].pData + (diffIdx * pNVM_DataTable[srcTblEntryIdx].ElementSize) + innerOffset)))
        {
            return gNVM_RecordWriteError_c;
        }
    }

    /* write the associated record meta information */
    if(kStatus_FLASH_Success != NV_FlashProgramUnaligned(dstMetaAddress, sizeof(NVM_RecordMetaInfo_t), (uint8_t*)(&dstMetaInfo)))
    {
        return gNVM_MetaInfoWriteError_c;
    }
    return gNVM_OK_c;
}


/******************************************************************************
 * Name: NvGetTblEntryMetaAddrFromId
 * Description: Gets the table entry meta address based on table entry ID
 * Parameter(s): [IN] searchStartAddress - the search start address
 *               [IN] dataEntryId - table entry ID
 * Return: the value of the meta address
 *****************************************************************************/
#if gNvFragmentation_Enabled_d
static uint32_t NvGetTblEntryMetaAddrFromId
(
    uint32_t searchStartAddress,
    uint16_t dataEntryId
)
{
    NVM_RecordMetaInfo_t metaInfo;

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
            return searchStartAddress;
        }

        searchStartAddress -= sizeof(NVM_RecordMetaInfo_t);
    }
    return 0;
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
static NVM_Status_t NvInternalDefragmentedCopy
(
    uint32_t srcMetaAddr,
    uint16_t srcTblEntryIdx,
    uint32_t dstMetaAddr,
    uint32_t dstRecordAddr,
    NVM_RecordMetaInfo_t *ownerRecordMetaInfo
)
{
    NVM_Status_t status = gNVM_OK_c;
    uint32_t metaAddress = srcMetaAddr;
    uint32_t size = pNVM_DataTable[srcTblEntryIdx].ElementSize * pNVM_DataTable[srcTblEntryIdx].ElementsCount;
    uint8_t space_left;
    uint16_t copy_amount;
    uint8_t dstBuffer[PGM_SIZE_BYTE];
    uint16_t element_idx = 0;
    uint8_t element_inner_copied = 0;
    uint16_t written_buffers_count = 0;
    NVM_RecordMetaInfo_t metaInfo;
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
        NvGetTableEntry(pNVM_DataTable[srcTblEntryIdx].DataEntryID, &flashDataEntry);
        if(pNVM_DataTable[srcTblEntryIdx].ElementsCount > flashDataEntry.ElementsCount)
        {
            /* fill the FLASH destination page with the default RAM value for the missing element(s) */
            fillFromRAM = TRUE;
        }
    }
    #endif /* gNvUseExtendedFeatureSet_d */

    /* clear the records offsets buffer */
    FLib_MemSet(maNvRecordsCpyOffsets, 0, sizeof(uint16_t)*pNVM_DataTable[srcTblEntryIdx].ElementsCount);

    while(metaAddress > (uint32_t)ownerRecordMetaInfo)
    {
        /* get meta information */
        NvGetMetaInfo(mNvActivePageId, metaAddress, &metaInfo);

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
             (0 == maNvRecordsCpyOffsets[metaInfo.fields.NvmElementIndex]))
        {
            maNvRecordsCpyOffsets[metaInfo.fields.NvmElementIndex] = metaInfo.fields.NvmRecordOffset;
        }
        metaAddress -= sizeof(NVM_RecordMetaInfo_t);
    }
    while (size)
    {
        space_left = PGM_SIZE_BYTE;
        FLib_MemSet(dstBuffer, 0xFF, PGM_SIZE_BYTE);

        /* fill the internal buffer */
        while ((0 != space_left) && (element_idx < pNVM_DataTable[srcTblEntryIdx].ElementsCount))
        {
            /* plenty of space left to copy the rest of the element */
            if (space_left >= (pNVM_DataTable[srcTblEntryIdx].ElementSize - element_inner_copied))
            {
                copy_amount = pNVM_DataTable[srcTblEntryIdx].ElementSize - element_inner_copied;
                #if gNvUseExtendedFeatureSet_d
                /* copy the rest of the data from the RAM entry */
                if (fillFromRAM && element_idx >= flashDataEntry.ElementsCount)
                {
                    FLib_MemCpy(dstBuffer + (PGM_SIZE_BYTE - space_left),
                                (void*)((uint32_t)pNVM_DataTable[srcTblEntryIdx].pData + element_idx * pNVM_DataTable[srcTblEntryIdx].ElementSize + element_inner_copied),
                                copy_amount);
                }
                else
                #endif
                /* copy from the owning full record save if no single save offset was found */
                if (0 == maNvRecordsCpyOffsets[element_idx])
                {
                    FLib_MemCpy(dstBuffer + (PGM_SIZE_BYTE - space_left),
                                (void*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + ownerRecordMetaInfo->fields.NvmRecordOffset + element_idx * pNVM_DataTable[srcTblEntryIdx].ElementSize + element_inner_copied),
                                copy_amount);
                }
                else
                {
                    FLib_MemCpy(dstBuffer + (PGM_SIZE_BYTE - space_left),
                                (void*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + maNvRecordsCpyOffsets[element_idx] + element_inner_copied),
                                copy_amount);
                }
                space_left -= copy_amount;

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
                    FLib_MemCpy(dstBuffer + (PGM_SIZE_BYTE - space_left),
                                (void*)((uint32_t)pNVM_DataTable[srcTblEntryIdx].pData + element_idx * pNVM_DataTable[srcTblEntryIdx].ElementSize + element_inner_copied),
                                copy_amount);
                }
                else
                #endif
                /* copy from the owning full record save if no single save offset was found */
                if (0 == maNvRecordsCpyOffsets[element_idx])
                {
                    FLib_MemCpy(dstBuffer + (PGM_SIZE_BYTE - space_left),
                                (void*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + ownerRecordMetaInfo->fields.NvmRecordOffset + element_idx * pNVM_DataTable[srcTblEntryIdx].ElementSize + element_inner_copied),
                                copy_amount);
                }
                else
                {
                    FLib_MemCpy(dstBuffer + (PGM_SIZE_BYTE - space_left),
                                (void*)(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + maNvRecordsCpyOffsets[element_idx] + element_inner_copied),
                                copy_amount);
                }
                element_inner_copied += copy_amount;
                break;
            }
        }
        /* write the record data */
        if(kStatus_FLASH_Success != NV_FlashProgram(dstRecordAddr + written_buffers_count * PGM_SIZE_BYTE, PGM_SIZE_BYTE, dstBuffer))
        {
            return gNVM_RecordWriteError_c;
        }
        written_buffers_count++;

        /* copied all the data, exit */
        if (size <= PGM_SIZE_BYTE)
        {
            break;
        }
        size -= PGM_SIZE_BYTE;
    }

    /* write meta information tag */
    dstMetaInfo.fields.NvValidationStartByte = gValidationByteAllRecords_c;
    dstMetaInfo.fields.NvmDataEntryID = ownerRecordMetaInfo->fields.NvmDataEntryID;
    dstMetaInfo.fields.NvmElementIndex = 0;
    dstMetaInfo.fields.NvmRecordOffset = dstRecordAddr - mNvVirtualPageProperty[(mNvActivePageId+1)%2].NvRawSectorStartAddress;
    dstMetaInfo.fields.NvValidationEndByte = gValidationByteAllRecords_c;

    /* the offset has to be 4 bytes aligned, an extra check is performed to avoid further hard faults caused by FTFx controller */
    if(dstMetaInfo.fields.NvmRecordOffset & (uint16_t)0x3)
    {
        return gNVM_AlignamentError_c;
    }

    /* write the associated record meta information */
    if(kStatus_FLASH_Success != NV_FlashProgram(dstMetaAddr, sizeof(NVM_RecordMetaInfo_t), (uint8_t*)(&dstMetaInfo)))
    {
        return gNVM_MetaInfoWriteError_c;
    }
    return status;
}
#endif /* gNvFragmentation_Enabled_d */

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
static NVM_Status_t NvCopyPage
(
    NvTableEntryId_t skipEntryId
)
{
    /* source page related variables */
    uint32_t srcMetaAddress;
    NVM_RecordMetaInfo_t srcMetaInfo;
    uint16_t srcTableEntryIdx;

    /* destination page related variables */
    uint32_t dstMetaAddress;
    uint32_t firstMetaAddress;
    NVM_VirtualPageID_t dstPageId;
    uint32_t dstRecordAddress;

    #if gNvUseExtendedFeatureSet_d
    uint16_t idx;
    bool_t entryFound;
    NVM_DataEntry_t flashDataEntry;
    bool_t tableUpgraded = FALSE;
    #endif /* gNvUseExtendedFeatureSet_d */
    #if gNvFragmentation_Enabled_d
    uint32_t tblEntryMetaAddress = 0;
    #endif
    uint32_t bytesToCopy;

    /* status variable */
    NVM_Status_t status;

    dstPageId = (NVM_VirtualPageID_t)((mNvActivePageId+1)%2);

    /* Check if the destination page is blank. If not, erase it. */
    if(gNVM_PageIsNotBlank_c == NvVirtualPageBlankCheck(dstPageId))
    {
        status = NvEraseVirtualPage(dstPageId);
        if(gNVM_OK_c != status)
        {
            return status;
        }
    }
    /* initialise the destination page meta info start address */
    dstMetaAddress = mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c;
    #if gNvUseExtendedFeatureSet_d
    if (mNvTableUpdated)
        tableUpgraded = (GetFlashTableVersion() != mNvFlashTableVersion);
    #endif

    firstMetaAddress = dstMetaAddress;
    /*if src is an empty page, just copy the table and make the initialisations*/
    srcMetaAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress;
    if (srcMetaAddress != gEmptyPageMetaAddress_c)
    {
        /* initialise the destination page record start address */
        dstRecordAddress = mNvVirtualPageProperty[dstPageId].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1;

        while(srcMetaAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
        {
            /* get current meta information */
            (void)NvGetMetaInfo(mNvActivePageId, srcMetaAddress, &srcMetaInfo);

            #if gNvUseExtendedFeatureSet_d
            /* NV RAM table has been updated */
            if(mNvTableUpdated)
            {
                idx = 0;
                entryFound = FALSE;

                /* check if the saved entry is still present in the new RAM table */
                while(idx < gNVM_TABLE_entries_c)
                {
                    if(srcMetaInfo.fields.NvmDataEntryID == pNVM_DataTable[idx].DataEntryID)
                    {
                        entryFound = TRUE;
                        break;
                    }
                    idx++;
                }

                if(!entryFound)
                {
                    /* move to the next meta info */
                    srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                    continue;
                }
            }
            #endif /* gNvUseExtendedFeatureSet_d */

            /* get table entry index */
            srcTableEntryIdx = NvGetTableEntryIndexFromId(srcMetaInfo.fields.NvmDataEntryID);

            if((srcMetaInfo.fields.NvValidationStartByte != srcMetaInfo.fields.NvValidationEndByte) ||
               (srcTableEntryIdx == gNvInvalidDataEntry_c) ||
               (srcMetaInfo.fields.NvmDataEntryID == skipEntryId) ||
               NvIsRecordCopied(dstPageId, &srcMetaInfo))
            {
                /* go to the next meta information tag */
                srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                continue;
            }

            if((srcMetaInfo.fields.NvValidationStartByte != gValidationByteSingleRecord_c) &&
               (srcMetaInfo.fields.NvValidationStartByte != gValidationByteAllRecords_c))
            {
                /* go to the next meta information tag */
                srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                continue;
            }

            #if gUnmirroredFeatureSet_d
            if (gNVM_MirroredInRam_c != pNVM_DataTable[srcTableEntryIdx].DataEntryType)
            {
                /*check if the data was erased using NvErase or is just uninitialised*/
                if (NULL == ((void**)pNVM_DataTable[srcTableEntryIdx].pData)[srcMetaInfo.fields.NvmElementIndex] &&
                    NvIsRecordErased(srcTableEntryIdx, srcMetaInfo.fields.NvmElementIndex, srcMetaAddress))
                {
                    /* go to the next meta information tag */
                    srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                    continue;
                }
            }
            #endif
            /* compute the destination record start address */
            bytesToCopy = pNVM_DataTable[srcTableEntryIdx].ElementsCount * pNVM_DataTable[srcTableEntryIdx].ElementSize;

            #if gNvUseExtendedFeatureSet_d
            /* NV RAM table has been updated */
            if(mNvTableUpdated)
            {
                if(NvGetTableEntry(pNVM_DataTable[srcTableEntryIdx].DataEntryID, &flashDataEntry))
                {
                    /* entries changed from mirrored/unmirrored and with different entry size cannot be recovered */
                    if (((flashDataEntry.DataEntryType != pNVM_DataTable[srcTableEntryIdx].DataEntryType) &&
                         ((gNVM_MirroredInRam_c == flashDataEntry.DataEntryType) || (gNVM_MirroredInRam_c == pNVM_DataTable[srcTableEntryIdx].DataEntryType)))
                        || (flashDataEntry.ElementSize != pNVM_DataTable[srcTableEntryIdx].ElementSize))
                    {
                        srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                        continue;
                    }

                    if(flashDataEntry.ElementsCount != pNVM_DataTable[srcTableEntryIdx].ElementsCount)
                    {
                        if (tableUpgraded)
                        {
                            if (flashDataEntry.ElementsCount < pNVM_DataTable[srcTableEntryIdx].ElementsCount)
                            {
                                /* copy only the bytes that were previously written to FLASH virtual page */
                                bytesToCopy = flashDataEntry.ElementsCount * flashDataEntry.ElementSize;
                            }
                            #if gNvFragmentation_Enabled_d
                            /*ignore if out of bounds*/
                            if (srcMetaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c &&
                                srcMetaInfo.fields.NvmElementIndex >= pNVM_DataTable[srcTableEntryIdx].ElementsCount)
                            {
                                srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                                continue;
                            }
                            #endif
                        }
                        else
                        {
                            srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
                            continue;
                        }
                    }
                }
            }
            #endif /* gNvUseExtendedFeatureSet_d */

            #if gNvFragmentation_Enabled_d
            if (srcMetaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c)
            {
                #if gUnmirroredFeatureSet_d
                if(gNVM_MirroredInRam_c != pNVM_DataTable[srcTableEntryIdx].DataEntryType)
                {
                    tblEntryMetaAddress = 0;
                }
                else
                #endif
                {
                    tblEntryMetaAddress = NvGetTblEntryMetaAddrFromId(srcMetaAddress, srcMetaInfo.fields.NvmDataEntryID);
                }

                /* if the record has no full entry associated perform simple copy */
                if (tblEntryMetaAddress == 0)
                {
                    /* compute the 'real record size' taking into consideration that the FTFL controller only writes in burst of 4 bytes */
                    bytesToCopy = pNVM_DataTable[srcTableEntryIdx].ElementSize;
                    dstRecordAddress -= NvUpdateSize(bytesToCopy);

                    if((status = NvInternalCopy(dstRecordAddress, dstMetaAddress, &srcMetaInfo, srcTableEntryIdx, bytesToCopy)) != gNVM_OK_c)
                    {
                        return status;
                    }
                    #if gUnmirroredFeatureSet_d
                    if(gNVM_MirroredInRam_c != pNVM_DataTable[srcTableEntryIdx].DataEntryType)
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
            /* if the copy operation must take elements from ram */
            #if gNvUseExtendedFeatureSet_d
            if(mNvTableUpdated && tableUpgraded &&
               bytesToCopy < pNVM_DataTable[srcTableEntryIdx].ElementsCount * pNVM_DataTable[srcTableEntryIdx].ElementSize)
            {
                /* make sure the adress can hold the entire space (+ what is taken from ram) */
                dstRecordAddress -= NvUpdateSize(pNVM_DataTable[srcTableEntryIdx].ElementsCount * pNVM_DataTable[srcTableEntryIdx].ElementSize);
            }
            else
            #endif
            {
                /* compute the destination record start address */
                dstRecordAddress -= NvUpdateSize(bytesToCopy);
            }

            #if gNvFragmentation_Enabled_d
            /*
            * single element record
            */
            if(srcMetaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c)
            {
                if((status = NvInternalDefragmentedCopy(srcMetaAddress, srcTableEntryIdx, dstMetaAddress, dstRecordAddress, (NVM_RecordMetaInfo_t *)tblEntryMetaAddress)) != gNVM_OK_c)
                {
                    return status;
                }
            }
            else
            #endif /* gNvFragmentation_Enabled_d */
            /*
            * full table entry
            */
            if((status = NvInternalCopy(dstRecordAddress, dstMetaAddress, &srcMetaInfo, srcTableEntryIdx, bytesToCopy)) != gNVM_OK_c)
            {
                return status;
            }

            /* update destination meta information address */
            dstMetaAddress += sizeof(NVM_RecordMetaInfo_t);

            /* move to the next meta info */
            srcMetaAddress -= sizeof(NVM_RecordMetaInfo_t);
        };
    }
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
        return gNVM_Error_c;
    }

    #if gNvUseExtendedFeatureSet_d
    if(mNvTableUpdated)
    {
        /* update the size of the NV table stored in FLASH */
        mNvTableSizeInFlash = NvGetFlashTableSize();

        /* clear the flag */
        mNvTableUpdated = FALSE;
    }
    #endif /* gNvUseExtendedFeatureSet_d */
    return gNVM_OK_c;
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
static NVM_Status_t NvInternalFormat
(
    uint32_t pageCounterValue
)
{
    uint8_t retryCount = gNvFormatRetryCount_c;

    /* increment the page counter value */
    if(pageCounterValue == (uint32_t)gPageCounterMaxValue_c - 1)
    {
        pageCounterValue = 1;
    }
    else
    {
        pageCounterValue++;
    }
    mNvPageCounter = pageCounterValue;

    while(retryCount--)
    {
        /* erase first page */
        if (gNVM_OK_c == NvEraseVirtualPage(gFirstVirtualPage_c) &&
            gNVM_OK_c == NvEraseVirtualPage(gSecondVirtualPage_c))
            break;
    }

    /* active page after format = first virtual page */
    mNvActivePageId = gFirstVirtualPage_c;

    /* save NV table from RAM memory to FLASH memory */
    if (FALSE == NvSaveRamTable(mNvActivePageId))
        return gNVM_FormatFailure_c;

    #if gNvUseExtendedFeatureSet_d
    /* update the size of the NV table stored in FLASH */
    mNvTableSizeInFlash = NvGetFlashTableSize();
    #endif

    /* update the page counter value */
    mNvPageCounter = pageCounterValue;

    return NvUpdateLastMetaInfoAddress();
}

/******************************************************************************
 * Name: NvSaveRamTable
 * Description: Saves the NV table
 * Parameter(s): [IN] pageId - the virtual page ID where the table will be
 *                             saved
 * Return: TRUE if table saved successfully, FALSE otherwise
 ******************************************************************************/
static bool_t NvSaveRamTable
(
    NVM_VirtualPageID_t pageId
)
{
    uint64_t tmp;
    uint32_t addr;
#if gNvUseExtendedFeatureSet_d
    uint16_t idx;
#endif

    if(NULL == pNVM_DataTable)
        return FALSE;

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
    if(kStatus_FLASH_Success != NV_FlashProgram(addr, sizeof(NVM_TableInfo_t), (uint8_t*)(&tmp)))
    {
        return FALSE;
    }

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

        if(kStatus_FLASH_Success != NV_FlashProgram(addr, sizeof(NVM_EntryInfo_t), (uint8_t*)(&tmp)))
        {
            return FALSE;
        }
        /* increment address */
        addr += sizeof(NVM_EntryInfo_t);

        /* increment table entry index */
        idx++;
    }

    tmp = ((NVM_TableInfo_t){.fields.NvTableMarker = mNvTableMarker}).rawValue;
    /* write table qualifier end, the rest 6 bytes are left 0x00 */
    if(kStatus_FLASH_Success != NV_FlashProgram(addr, sizeof(NVM_EntryInfo_t), (uint8_t*)(&tmp)))
    {
        return FALSE;
    }
    #endif

    /*write page counter bottom*/
    tmp = ((NVM_TableInfo_t){.fields.NvPageCounter = mNvPageCounter}).rawValue;
    if(kStatus_FLASH_Success != NV_FlashProgram((mNvVirtualPageProperty[pageId].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1),
                               sizeof(NVM_TableInfo_t), (uint8_t*)&tmp))
        return FALSE;
    return TRUE;
}

/******************************************************************************
 * Name: UpgradeLegacyTable
 * Description: Upgrades an legacy table to the new format
 * Parameter(s): -
 * Return: -
 *****************************************************************************/
static NVM_Status_t UpgradeLegacyTable()
{
    NVM_Status_t status;
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
#endif
    NVM_VirtualPageID_t dstPageId = (NVM_VirtualPageID_t)((mNvActivePageId+1)%2);

    /* Check if the destination page is blank. If not, erase it. */
    if(gNVM_PageIsNotBlank_c == NvVirtualPageBlankCheck(dstPageId))
    {
        status = NvEraseVirtualPage(dstPageId);
        if(gNVM_OK_c != status)
        {
            return status;
        }
    }
    /* read legacy page counter */
    read_address = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;
    NV_FlashRead(read_address, (uint8_t*)&legacy_page_counter, sizeof(legacy_page_counter));
    legacy_page_counter += 1;
    offset = sizeof(NVM_TableInfo_t);
    read_address += sizeof(legacy_page_counter);
    /* copy the flash table */
    #if gNvUseExtendedFeatureSet_d
        #if PGM_SIZE_BYTE == FTFx_PHRASE_SIZE
        /* 4 0xFF allignement bytes */
        read_address += 4;
        #endif
        NV_FlashRead(read_address, (uint8_t*)&legacy_table_marker, sizeof(legacy_table_marker));
        read_address += sizeof(legacy_table_marker);
        #if PGM_SIZE_BYTE == FTFx_PHRASE_SIZE
        /* 4 0xFF allignement bytes */
        read_address += 4;
        #endif
        while (read_address < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
        {
            NV_FlashRead(read_address, (uint8_t*)&value, sizeof(value));
            if (legacy_table_marker == value)
                break;
            NV_FlashRead(read_address, (uint8_t*)&tableEntry, sizeof(tableEntry));
            /* upgrade entry */
            #if PGM_SIZE_BYTE == FTFx_PHRASE_SIZE
            value = tableEntry.fields.NvElementSize;
            tableEntry.fields.NvElementSize = tableEntry.fields.NvDataEntryID;
            tableEntry.fields.NvDataEntryID = value;
            tableEntry.fields.NvElementsCount = tableEntry.fields.NvDataEntryType;
            #else
            value = tableEntry.fields.NvElementsCount;
            tableEntry.fields.NvElementsCount = tableEntry.fields.NvElementSize;
            tableEntry.fields.NvElementSize = value;
            tableEntry.fields.NvDataEntryID = tableEntry.fields.NvDataEntryType;
            #endif
            tableEntry.fields.NvDataEntryType = gNVM_MirroredInRam_c;

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
            if(kStatus_FLASH_Success != NV_FlashProgram(mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + offset,
                                       sizeof(NVM_EntryInfo_t), (uint8_t*)&tableEntry))
            {
                return gNVM_Error_c;
            }
            offset += sizeof(NVM_EntryInfo_t);
            read_address += sizeof(NVM_EntryInfo_t);
        }
        if (read_address >= mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
            return gNVM_AddressOutOfRange_c;

        tableData = ((NVM_TableInfo_t){.fields.NvTableMarker = mNvTableMarker});
        /* write table qualifier end, the rest 6 bytes are left 0x00 */
        if(kStatus_FLASH_Success != NV_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + offset, sizeof(NVM_TableInfo_t), (uint8_t*)(&tableData)))
        {
            return gNVM_Error_c;
        }
        read_address += sizeof(legacy_table_marker);
        offset += sizeof(NVM_TableInfo_t);
    #endif
    #if PGM_SIZE_BYTE == FTFx_PHRASE_SIZE
     /* 4 0xFF allignement bytes */
    read_address += 4;
    #endif

    /* copy metas */
    while (read_address < mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
    {
        NV_FlashRead(read_address, (uint8_t*)&meta, sizeof(NVM_RecordMetaInfo_t));
        if (gNvGuardValue_c == meta.rawValue)
            break;
        #if PGM_SIZE_BYTE == FTFx_PHRASE_SIZE
        if (0 != meta.fields.NvmRecordOffset)
        {
            last_record_offset = meta.fields.NvmRecordOffset;
        }
        #else
        /* adjust the meta offset */
        if (0 != meta.fields.NvmRecordOffset)
        {
            last_record_offset = meta.fields.NvmRecordOffset;
            meta.fields.NvmRecordOffset -= gNvLegacyOffset_c;
        }
        #endif
        /* write the meta */
        if(kStatus_FLASH_Success != NV_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + offset,
                                   sizeof(NVM_RecordMetaInfo_t), (uint8_t*)&meta))
        {
            return gNVM_Error_c;
        }
        offset += sizeof(NVM_RecordMetaInfo_t);
        read_address += sizeof(NVM_RecordMetaInfo_t);
    }
    if (read_address >= mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress)
        return gNVM_AddressOutOfRange_c;

    /* copy the data */
    #if PGM_SIZE_BYTE == FTFx_PHRASE_SIZE
    if(kStatus_FLASH_Success != NV_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + last_record_offset,
                                  mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize - last_record_offset - sizeof(legacy_page_counter) - 4,
                                 (uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + last_record_offset))
    #else
    if(kStatus_FLASH_Success != NV_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress + last_record_offset - sizeof(legacy_page_counter),
                                 mNvVirtualPageProperty[mNvActivePageId].NvTotalPageSize - last_record_offset - sizeof(legacy_page_counter),
                                (uint8_t*)mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + last_record_offset))
    #endif
    {
        return gNVM_Error_c;
    }

    /* write page counter, table marker, table version top */
    #if gNvUseExtendedFeatureSet_d
    tableData = ((NVM_TableInfo_t){.fields.NvPageCounter  = legacy_page_counter,
                                   .fields.NvTableMarker  = mNvTableMarker,
                                   .fields.NvTableVersion = 1});
    #else
    tableData = ((NVM_TableInfo_t){.fields.NvPageCounter  = mNvPageCounter});
    #endif

    if(kStatus_FLASH_Success != NV_FlashProgram( mNvVirtualPageProperty[dstPageId].NvRawSectorStartAddress, sizeof(NVM_TableInfo_t), (uint8_t*)(&tableData)))
    {
        return gNVM_Error_c;
    }

    /* write page counter bottom */
    tableData = ((NVM_TableInfo_t){.fields.NvPageCounter = legacy_page_counter});
    if(kStatus_FLASH_Success != NV_FlashProgram((mNvVirtualPageProperty[dstPageId].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) + 1),
                               sizeof(NVM_TableInfo_t), (uint8_t*)&tableData))
    {
        return gNVM_Error_c;
    }

    /* erase old page */
    mNvErasePgCmdStatus.NvPageToErase = mNvActivePageId;
    mNvErasePgCmdStatus.NvSectorAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;
    mNvErasePgCmdStatus.NvErasePending = TRUE;
    /* set new active page */
    mNvActivePageId = dstPageId;
    return gNVM_OK_c;
}
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
static NVM_Status_t NvGetEntryFromDataPtr
(
    void* pData,
    NVM_TableEntryInfo_t* pIndex
)
{
    uint16_t idx = 0;
    
    while(idx < gNVM_TABLE_entries_c)
    {
        if(((uint8_t*)pData >= (uint8_t*)pNVM_DataTable[idx].pData))
        {
            #if gUnmirroredFeatureSet_d
            if(gNVM_MirroredInRam_c != pNVM_DataTable[idx].DataEntryType)
            {
                if ((uint8_t*)pData < ((uint8_t*)pNVM_DataTable[idx].pData + (sizeof(void*) * pNVM_DataTable[idx].ElementsCount)))
                {
                    pIndex->elementIndex = (((uint32_t)pData - (uint32_t)pNVM_DataTable[idx].pData) / sizeof(void*));
                    pIndex->entryId = pNVM_DataTable[idx].DataEntryID;
                    return gNVM_OK_c;
                }
                idx++;
                continue;
            }
            else
            #endif
            if ((uint8_t*)pData < ((uint8_t*)pNVM_DataTable[idx].pData + (pNVM_DataTable[idx].ElementSize * pNVM_DataTable[idx].ElementsCount)))
            {
                pIndex->elementIndex = (((uint32_t)pData - (uint32_t)pNVM_DataTable[idx].pData)/(pNVM_DataTable[idx].ElementSize));
                pIndex->entryId = pNVM_DataTable[idx].DataEntryID;
                return gNVM_OK_c;
            }
        }
        /* increment the loop counter */
        idx++;
    }
    return gNVM_PointerOutOfRange_c;
}


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
static NVM_Status_t NvWriteRecord
(
    NVM_TableEntryInfo_t* tblIndexes
)
{
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    uint32_t metaInfoAddress;
    uint32_t lastRecordAddress;
    uint32_t newRecordAddress;
    NVM_RecordMetaInfo_t metaInfo;
    uint32_t realRecordSize;
    uint32_t totalRecordSize; /* record + meta */
    uint32_t pageFreeSpace;
    bool_t doWrite;
    uint32_t srcAddress;
#else /* FlexNVM */
    uint32_t lastFlexMetaInfoAddress;
    NVM_FlexMetaInfo_t lastFlexMetaInfo;
    NVM_FlexMetaInfo_t flexMetaInfo;
    uint32_t destRecordEndAddress;
#endif

    uint16_t tableEntryIdx;
    uint32_t recordSize;

    tableEntryIdx = NvGetTableEntryIndexFromId(tblIndexes->entryId);

    if(gNvInvalidTableEntryIndex_c == tableEntryIdx)
    {
        return gNVM_InvalidTableEntry_c;
    }

#if (gNvUseFlexNVM_d == TRUE) /* FlexNVM */

    recordSize = pNVM_DataTable[tableEntryIdx].ElementsCount * pNVM_DataTable[tableEntryIdx].ElementSize;

    NvGetFlexMetaInfoFromId(tblIndexes->entryId, &flexMetaInfo);

    if(flexMetaInfo.rawValue == gNvFlexGuardValue_c) /* no meta found for this table entry ID */
    {
        /* set entry ID */
        flexMetaInfo.fields.NvDataEntryID = tblIndexes->entryId;
        /* get last meta info tag address */
        lastFlexMetaInfoAddress = NvGetFlexLastMetaInfo();

        if(lastFlexMetaInfoAddress < gFlashConfig.EERAMBase) /* FlexRAM empty */
        {
            flexMetaInfo.fields.NvDataOffset = gFlashConfig.EEESize - recordSize;
            destRecordEndAddress = gFlashConfig.EERAMBase + gFlashConfig.EEESize;
            lastFlexMetaInfoAddress = gFlashConfig.EERAMBase;
        }
        else
        {
#if FSL_FEATURE_FLASH_IS_FTFL
            /* wait for EEPROM system to be ready */
            while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
#endif
            /* read last meta tag */
            NV_FlashRead(lastFlexMetaInfoAddress, (uint8_t*)&lastFlexMetaInfo, sizeof(NVM_FlexMetaInfo_t));
            /* compute record destination end address */
            destRecordEndAddress = gFlashConfig.EERAMBase + lastFlexMetaInfo.fields.NvDataOffset;
            /* compute record offset */
            flexMetaInfo.fields.NvDataOffset = lastFlexMetaInfo.fields.NvDataOffset - recordSize;
            /* increment the last meta info address and reused it as address of the current meta info tag */
            lastFlexMetaInfoAddress += sizeof(NVM_FlexMetaInfo_t);
        }

#if FSL_FEATURE_FLASH_IS_FTFL
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));

        /* write record */
        if(kStatus_FLASH_Success != EEEWrite(&gFlashConfig, destRecordEndAddress - recordSize, recordSize, ((uint8_t*)(pNVM_DataTable[tableEntryIdx].pData))))
        {
            return gNVM_RecordWriteError_c;
        }

        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));

        /* write meta */
        if(kStatus_FLASH_Success != EEEWrite(&gFlashConfig, lastFlexMetaInfoAddress, sizeof(NVM_FlexMetaInfo_t), (uint8_t *)(&flexMetaInfo.rawValue)))
        {
            return gNVM_RecordWriteError_c;
        }
#endif /* #if FSL_FEATURE_FLASH_IS_FTFL */

#if FSL_FEATURE_FLASH_IS_FTFE
        /* write record */
        if(kStatus_FLASH_Success != FLASH_EepromWrite(&gFlashConfig, destRecordEndAddress - recordSize, ((uint8_t*)(pNVM_DataTable[tableEntryIdx].pData)), recordSize))
        {
            return gNVM_RecordWriteError_c;
        }

        /* write meta */
        if(kStatus_FLASH_Success != FLASH_EepromWrite(&gFlashConfig, lastFlexMetaInfoAddress, (uint8_t *)(&flexMetaInfo.rawValue), sizeof(NVM_FlexMetaInfo_t)))
        {
            return gNVM_RecordWriteError_c;
        }
#endif /* #if FSL_FEATURE_FLASH_IS_FTFE */
    }
    else /* table entry ID already in FlexRAM, update the corresponding record */
    {
#if FSL_FEATURE_FLASH_IS_FTFL
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));

        if(kStatus_FLASH_Success != EEEWrite(&gFlashConfig, (uint32_t)(gFlashConfig.EERAMBase + flexMetaInfo.fields.NvDataOffset), recordSize,
                               ((uint8_t*)(pNVM_DataTable[tableEntryIdx].pData))))
        {
            return gNVM_RecordWriteError_c;
        }
#endif /* FSL_FEATURE_FLASH_IS_FTFL */

#if FSL_FEATURE_FLASH_IS_FTFE
        if(kStatus_FLASH_Success != FLASH_EepromWrite(&gFlashConfig, (uint32_t)(gFlashConfig.FlexRAMBlockBase + flexMetaInfo.fields.NvDataOffset),
                          ((uint8_t*)(pNVM_DataTable[tableEntryIdx].pData)), recordSize))
        {
            return gNVM_RecordWriteError_c;
        }
#endif /* FSL_FEATURE_FLASH_IS_FTFE */

    }
    /* Empty macro when nvm monitoring is not enabled */
    #if (gFsciIncluded_c && (gNvmEnableFSCIRequests_c || gNvmEnableFSCIMonitoring_c))
    FSCI_MsgNVWriteMonitoring(flexMetaInfo.fields.NvDataEntryID,tblIndexes->elementIndex,tblIndexes->saveRestoreAll);
    #endif
    return gNVM_OK_c;

#else /* No FlexNVM */
    /* make sure i don't process the save if page copy is active */
    if (mNvCopyOperationIsPending)
    {
        return gNVM_PageCopyPending_c;
    }

    #if gUnmirroredFeatureSet_d
    /* For data sets not mirrored in ram a table entry is saved separate */
    if(gNVM_MirroredInRam_c != pNVM_DataTable[tableEntryIdx].DataEntryType)
    {
        tblIndexes->saveRestoreAll = FALSE;
    }
    #endif

    if(tblIndexes->saveRestoreAll)
    {
        realRecordSize = recordSize = pNVM_DataTable[tableEntryIdx].ElementSize * pNVM_DataTable[tableEntryIdx].ElementsCount;
    }
    else
    {
        realRecordSize = recordSize = pNVM_DataTable[tableEntryIdx].ElementSize;
    }

    #if gUnmirroredFeatureSet_d
    /* Check if is an erase for unmirrored dataset*/
    if(gNVM_MirroredInRam_c != pNVM_DataTable[tableEntryIdx].DataEntryType)
    {
        if(NULL == ((void**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex])
        {
            realRecordSize = recordSize = 0;
        }
        /*if the dataset is allready in flash, ignore it*/
        else if(NvIsNVMFlashAddress(((void**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex]))
        {
            /*it returns OK, because atomic save must not fail, this is not an error*/
            return gNVM_OK_c;
        }
    }
    #endif

    /* get active page free space */
    NvGetPageFreeSpace(&pageFreeSpace);

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
        return gNVM_PageCopyPending_c;
    }
    else
    {
        metaInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress;

        if(gEmptyPageMetaAddress_c == metaInfoAddress)
        {
            /* empty page, first write */

            /* set new record address */
            newRecordAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorEndAddress - sizeof(NVM_TableInfo_t) - realRecordSize + 1;

            /* set associated meta info */

            if(tblIndexes->saveRestoreAll)
            {
                metaInfo.fields.NvValidationStartByte = gValidationByteAllRecords_c;
                metaInfo.fields.NvValidationEndByte = gValidationByteAllRecords_c;
            }
            else
            {
                metaInfo.fields.NvValidationStartByte = gValidationByteSingleRecord_c;
                metaInfo.fields.NvValidationEndByte = gValidationByteSingleRecord_c;
            }

            metaInfo.fields.NvmDataEntryID = pNVM_DataTable[tableEntryIdx].DataEntryID;
            metaInfo.fields.NvmElementIndex = tblIndexes->elementIndex;
            metaInfo.fields.NvmRecordOffset = newRecordAddress - mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;           

            /* gEmptyPageMetaAddress_c is not a valid address and it is used only as an empty page marker;
            * therefore, set the valid value of meta information address */
            metaInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c;
            }
        else
        {
            /* get the meta information of the last successfully written record */
            #if gUnmirroredFeatureSet_d
                NvGetMetaInfo(mNvActivePageId, mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress, &metaInfo);
            #else
                /* get the last record start address (the address is always aligned) */
                NvGetMetaInfo(mNvActivePageId, metaInfoAddress, &metaInfo);
            #endif
            lastRecordAddress = mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset;
            /* set new record address */
            newRecordAddress = lastRecordAddress - realRecordSize;

            /* set associated meta info */
            if(tblIndexes->saveRestoreAll)
            {
                metaInfo.fields.NvValidationStartByte = gValidationByteAllRecords_c;
                metaInfo.fields.NvValidationEndByte = gValidationByteAllRecords_c;
            }
            else
            {
                metaInfo.fields.NvValidationStartByte = gValidationByteSingleRecord_c;
                metaInfo.fields.NvValidationEndByte = gValidationByteSingleRecord_c;
            }
            metaInfo.fields.NvmDataEntryID = pNVM_DataTable[tableEntryIdx].DataEntryID;
            metaInfo.fields.NvmElementIndex = tblIndexes->elementIndex;
            metaInfo.fields.NvmRecordOffset = newRecordAddress - mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;

            metaInfoAddress += sizeof(NVM_RecordMetaInfo_t);
            }

        /* check if the space needed by the record is really free (erased).
        * this check is necessary because it may happens that a record to be successfully written,
        * but the system fails (e.g. POR) before the associated meta information has been written.
        * the theoretically free space is computed as the difference between the last meta info
        * address and the start address of the last successfully written record. This information
        * is valuable but may not reflect the reality, as mentioned in the explanation above */

        doWrite = FALSE;

        /* make sure there is at least a free space for a meta between the last one and the data*/
        while(totalRecordSize + sizeof(NVM_RecordMetaInfo_t) < pageFreeSpace)
        {
            /* check if the space for the record is free */
            if ((realRecordSize) && (!NvIsMemoryAreaAvailable(newRecordAddress, realRecordSize)))
            {
                /* the memory space is not blank */
                if (pageFreeSpace < realRecordSize)
                {
                    /* I am unable to write the record on this page, trigger copy page */
                    break;
                }
                pageFreeSpace -= realRecordSize;
                newRecordAddress -= realRecordSize;
            }
            /* check if the space for the meta is free */
            else if(!NvIsMemoryAreaAvailable(metaInfoAddress, sizeof(NVM_RecordMetaInfo_t)))
            {
                /* the memory space is not blank */
                if (pageFreeSpace < realRecordSize)
                {
                    /* I am unable to write the meta on this page, trigger copy page */
                    break;
                }
                pageFreeSpace -= sizeof(NVM_RecordMetaInfo_t);
                metaInfoAddress += sizeof(NVM_RecordMetaInfo_t);
            }
            else
            {
                /* the memory space is blank */
                doWrite = TRUE;
                break;
            }
        }

        /* update the meta info offset, if changed */
        if(newRecordAddress - mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress != metaInfo.fields.NvmRecordOffset)
        {
            metaInfo.fields.NvmRecordOffset = newRecordAddress - mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress;
        }

        /* the offset has to be 4 bytes aligned, an extra check is performed to avoid further hard faults caused by FTFx controller */
        if(metaInfo.fields.NvmRecordOffset & (uint16_t)0x3)
        {
            return gNVM_AlignamentError_c;
        }

        /* Write the record and associated meta information */
        if(FALSE == doWrite)
        {
            /* there is no space to save the record, try to copy the current active page latest records
            * to the other page
            */
            mNvCopyOperationIsPending = TRUE;
            return gNVM_PageCopyPending_c;
        }
        #if gUnmirroredFeatureSet_d
        if(gNVM_MirroredInRam_c != pNVM_DataTable[tableEntryIdx].DataEntryType)
        {
            srcAddress = (uint32_t)(uint8_t*)((uint8_t**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex];
        }
        else
        #endif
        if(tblIndexes->saveRestoreAll)
        {
            srcAddress = (uint32_t)((uint8_t*)(((uint8_t*)(pNVM_DataTable[tableEntryIdx]).pData)));
        }
        else
        {
            srcAddress = (uint32_t)((uint8_t*)(((uint8_t*)(pNVM_DataTable[tableEntryIdx]).pData)) + (tblIndexes->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize));
        }

        #if gUnmirroredFeatureSet_d
        if(0 == srcAddress)
        {
            /* It's an erased unmirrored dataset */
            metaInfo.fields.NvmRecordOffset = 0;
        }
        if(kStatus_FLASH_Success == (srcAddress ? NV_FlashProgramUnaligned( newRecordAddress, recordSize, (uint8_t*)srcAddress):kStatus_FLASH_Success))
        #else
        if(kStatus_FLASH_Success == NV_FlashProgramUnaligned( newRecordAddress, recordSize, (uint8_t*)srcAddress))
        #endif
        {
            /* record successfully written, now write the associated record meta information */
            if(kStatus_FLASH_Success == NV_FlashProgram( metaInfoAddress, sizeof(NVM_RecordMetaInfo_t), (uint8_t*)(&metaInfo)))
            {
                /* update the last record meta information */
                mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress = metaInfoAddress;
                /* update the last unerased meta info address */
                #if gUnmirroredFeatureSet_d
                if(0 != metaInfo.fields.NvmRecordOffset)
                {
                    mNvVirtualPageProperty[mNvActivePageId].NvLastMetaUnerasedInfoAddress = metaInfoAddress;
                }
                #endif
                /* Empty macro when nvm monitoring is not enabled */
                #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
                FSCI_MsgNVWriteMonitoring(metaInfo.fields.NvmDataEntryID,tblIndexes->elementIndex,tblIndexes->saveRestoreAll);
                #endif

                #if gUnmirroredFeatureSet_d
                if(gNVM_MirroredInRam_c != pNVM_DataTable[tableEntryIdx].DataEntryType)
                {
                    if(0 != metaInfo.fields.NvmRecordOffset)
                    {
                        uint8_t* pTempAddress = (uint8_t*)((uint8_t**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex];
                        ((uint8_t**)pNVM_DataTable[tableEntryIdx].pData)[tblIndexes->elementIndex] = (uint8_t*)newRecordAddress;
                        MSG_Free(pTempAddress);
                    }
                }
                #endif
                return gNVM_OK_c;
            }
            else
            {
                return gNVM_MetaInfoWriteError_c;
            }
            }
            else
            {
                return gNVM_RecordWriteError_c;
            }
    }
#endif /* gNvUseFlexNVM_d */
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
static NVM_Status_t NvRestoreData
(
    NVM_TableEntryInfo_t* tblIdx
)
{
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
    NVM_RecordMetaInfo_t metaInfo;
    uint32_t metaInfoAddress;
    NVM_Status_t status;
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
        return gNVM_InvalidTableEntry_c;
    }

#if (gNvUseFlexNVM_d == TRUE) /* FlexNVM */

    /* restore data from EERAM */
    EERamAddress = gFlashConfig.EERAMBase;

    do
    {
#if FSL_FEATURE_FLASH_IS_FTFL
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
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
                while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
#endif
                /* read all elements */
                NV_FlashRead((gFlashConfig.EERAMBase + flexMetaInfo.fields.NvDataOffset),
                             (uint8_t*)(pNVM_DataTable[tableEntryIdx].pData),
                             pNVM_DataTable[tableEntryIdx].ElementSize * pNVM_DataTable[tableEntryIdx].ElementsCount);
                return gNVM_OK_c;
            }
            else
            {
#if FSL_FEATURE_FLASH_IS_FTFL
                /* wait for EEPROM system to be ready */
                while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
#endif
                /* read element */
                NV_FlashRead(gFlashConfig.EERAMBase + flexMetaInfo.fields.NvDataOffset + (tblIdx->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize),
                             (uint8_t*)(((uint8_t*)pNVM_DataTable[tableEntryIdx].pData) + (tblIdx->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize)),
                             pNVM_DataTable[tableEntryIdx].ElementSize);
                return gNVM_OK_c;
            }
        }

        /* go to next meta tag */
        EERamAddress += sizeof(flexMetaInfo);

#if FSL_FEATURE_FLASH_IS_FTFL
    } while(EERamAddress < (gFlashConfig.EERAMBase + gFlashConfig.EEESize));
#endif

#if FSL_FEATURE_FLASH_IS_FTFE
    } while(EERamAddress < (gFlashConfig.FlexRAMBlockBase + gFlashConfig.FlexRAMTotalSize));
#endif

    return gNVM_MetaNotFound_c;

#else /* FlexNVM */
    /* get the last meta information address */
    if((metaInfoAddress = mNvVirtualPageProperty[mNvActivePageId].NvLastMetaInfoAddress) == gEmptyPageMetaAddress_c)
    {
        /* blank page, no data to restore */
        return gNVM_PageIsEmpty_c;
    }

    if(tblIdx->entryId == gNvInvalidDataEntry_c)
    {
        /* invalid table entry */
        return gNVM_InvalidTableEntry_c;
    }

    /*
    * If the meta info is found, the associated record is restored,
    * otherwise the gNVM_MetaNotFound_c will be returned
    */
    status = gNVM_MetaNotFound_c;

    /*** restore all ***/
    if(tblIdx->saveRestoreAll)
    {
        #if gNvFragmentation_Enabled_d
        /* clear the buffer */
        FLib_MemSet(maNvRecordsCpyOffsets, 0, sizeof(uint16_t)*pNVM_DataTable[tableEntryIdx].ElementsCount);

        /* parse meta info backwards */
        while(metaInfoAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
        {
            /* get the meta information */
            NvGetMetaInfo(mNvActivePageId, metaInfoAddress, &metaInfo);

            if(metaInfo.fields.NvValidationStartByte != metaInfo.fields.NvValidationEndByte)
            {
                /* invalid meta info, move to the previous meta info */
                metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
                continue;
            }

            if(metaInfo.fields.NvmDataEntryID == tblIdx->entryId)
            {
                /* single save found */
                if ((metaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c) &&
                    (0 == maNvRecordsCpyOffsets[metaInfo.fields.NvmElementIndex]))
                {
                    maNvRecordsCpyOffsets[metaInfo.fields.NvmElementIndex] = 1;
                    NV_FlashRead(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset,
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
                        if (1 == maNvRecordsCpyOffsets[cnt])
                            continue;
                        NV_FlashRead(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset + cnt * pNVM_DataTable[tableEntryIdx].ElementSize,
                                     (uint8_t*)pNVM_DataTable[tableEntryIdx].pData + cnt * pNVM_DataTable[tableEntryIdx].ElementSize,
                                     pNVM_DataTable[tableEntryIdx].ElementSize);
                    }
                    return gNVM_OK_c;
                }
            }

            metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
        }
        return status;
        #else
        /* parse meta info backwards until the full save is found */
        while(metaInfoAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
        {
            /* get the meta information */
            NvGetMetaInfo(mNvActivePageId, metaInfoAddress, &metaInfo);

            if(metaInfo.fields.NvValidationStartByte != metaInfo.fields.NvValidationEndByte)
            {
                /* invalid meta info, move to the previous meta info */
                metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
                continue;
            }

            if(metaInfo.fields.NvmDataEntryID == tblIdx->entryId)
            {
                /* single saves are not allowed if fragmentation is off */
                if(metaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c)
                {
                    return gNVM_FragmentatedEntry_c;
                }

                NV_FlashRead(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset,
                             (uint8_t*)pNVM_DataTable[tableEntryIdx].pData,
                             pNVM_DataTable[tableEntryIdx].ElementsCount * pNVM_DataTable[tableEntryIdx].ElementSize);
                return gNVM_OK_c;
            }

            metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
        }
        return status;
        #endif
    }

    /*** restore single ***/

    /* parse meta info backwards until the element is found */
    while(metaInfoAddress >= (mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + gNvFirstMetaOffset_c))
    {
        /* get the meta information */
        NvGetMetaInfo(mNvActivePageId, metaInfoAddress, &metaInfo);

        if(metaInfo.fields.NvValidationStartByte != metaInfo.fields.NvValidationEndByte)
        {
            /* invalid meta info, move to the previous meta info */
            metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
            continue;
        }

        if(metaInfo.fields.NvmDataEntryID == tblIdx->entryId)
        {
            if(metaInfo.fields.NvValidationStartByte == gValidationByteSingleRecord_c && metaInfo.fields.NvmElementIndex == tblIdx->elementIndex)
            {
                #if gUnmirroredFeatureSet_d
                if(gNVM_MirroredInRam_c != pNVM_DataTable[tableEntryIdx].DataEntryType)
                {
                    if(!metaInfo.fields.NvmRecordOffset)
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
                    NV_FlashRead(mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset,
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
                NV_FlashRead((mNvVirtualPageProperty[mNvActivePageId].NvRawSectorStartAddress + metaInfo.fields.NvmRecordOffset +
                              (tblIdx->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize)),
                ((uint8_t*)pNVM_DataTable[tableEntryIdx].pData + (tblIdx->elementIndex * pNVM_DataTable[tableEntryIdx].ElementSize)),
                pNVM_DataTable[tableEntryIdx].ElementSize);
                status = gNVM_OK_c;
                break;
            }
        }

        /* move to the previous meta info */
        metaInfoAddress -= sizeof(NVM_RecordMetaInfo_t);
    }
    return status;

#endif /* gNvUseFlexNVM_d */
}


/******************************************************************************
 * Name: NvGetTableEntryIndex
 * Description: get the table entry index from the provided ID
 * Parameter(s): [IN] entryId - the ID of the table entry
 * Return: table entry index of gNvInvalidTableEntryIndex_c
 *****************************************************************************/
static uint16_t NvGetTableEntryIndexFromId
(
    NvTableEntryId_t entryId
)
{
    uint16_t loopCnt = 0;
    
    while(loopCnt < gNVM_TABLE_entries_c)
    {
        if(pNVM_DataTable[loopCnt].DataEntryID == entryId)
        {
            return loopCnt;
        }
        /* increment the loop counter */
        loopCnt++;
    }
    return gNvInvalidTableEntryIndex_c;
}

#if !gFifoOverwriteEnabled_c
/******************************************************************************
 * Name: NvProcessFirstSaveInQueue
 * Description: processes the first save in the queue so that the queue can accept another entry
 * Parameter(s): -
 * Return: TRUE if a save has been processed, ELSE otherwise
 *****************************************************************************/
static bool NvProcessFirstSaveInQueue
(
    void
)
{
    NVM_TableEntryInfo_t tblIdx;
    NVM_Status_t status;

    if (mNvCriticalSectionFlag)
    {
        return FALSE;
    }
    if (NvGetPendingSavesCount(&mNvPendingSavesQueue))
    {
        while (NvPopPendingSave(&mNvPendingSavesQueue, &tblIdx))
        {
            if ((gNvCopyAll_c == tblIdx.entryId) && (gNvCopyAll_c == tblIdx.elementIndex) && (TRUE == tblIdx.saveRestoreAll))
            {
                __NvAtomicSave();
                continue;
            }
            else if (gNvInvalidDataEntry_c == tblIdx.entryId)
            {
                continue;
            }
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
            if(NvWriteRecord(&tblIdx) == gNVM_PageCopyPending_c)
            {
                #if (gFsciIncluded_c && gNvmEnableFSCIMonitoring_c)
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
                    return TRUE;
                }
                else
                {
                    NvAddSaveRequestToQueue(&tblIdx);
                }
            }
            else
            {
                return TRUE;
            }
#else /* FlexNVM */
            if (gNVM_OK_c == NvWriteRecord(&tblIdx))
            {
                return TRUE;
            }
            else
            {
                NvAddSaveRequestToQueue(&tblIdx);
            }
#endif
        }
    }
    return FALSE;
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
static NVM_Status_t NvAddSaveRequestToQueue
(
    NVM_TableEntryInfo_t* ptrTblIdx
)
{
    uint8_t loopIdx;
    bool_t  isQueued = FALSE;
    bool_t  isInvalidEntry = FALSE;
    uint8_t lastInvalidIdx = 0;
    uint8_t remaining_count;

    if(mNvPendingSavesQueue.EntriesCount == 0)
    {
        /* add request to queue */
        if(NvPushPendingSave(&mNvPendingSavesQueue, *ptrTblIdx))
        {
            return gNVM_OK_c;
        }
        return gNVM_SaveRequestRejected_c;
    }

    /* start from the queue's head */
    loopIdx = mNvPendingSavesQueue.Head;

    remaining_count = mNvPendingSavesQueue.EntriesCount;
    /* check if the request is not already stored in queue */
    while(remaining_count)
    {
        if(ptrTblIdx->entryId == mNvPendingSavesQueue.QData[loopIdx].entryId)
        {
            if(mNvPendingSavesQueue.QData[loopIdx].saveRestoreAll == TRUE) /* full table entry already queued */
            {
                /* request is already queued */
                isQueued = TRUE;
                break;
            }

            /* single element from table entry is queued */
            if(ptrTblIdx->saveRestoreAll == TRUE) /* a full table entry is requested to be saved */
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
            if(ptrTblIdx->elementIndex == mNvPendingSavesQueue.QData[loopIdx].elementIndex)
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
            mNvPendingSavesQueue.QData[lastInvalidIdx] = *ptrTblIdx;
            return gNVM_OK_c;
        }
        /* push the request to save operation pending queue */
        if(!NvPushPendingSave(&mNvPendingSavesQueue, *ptrTblIdx))
        {
            /* free a space */
            NvProcessFirstSaveInQueue();
            if(!NvPushPendingSave(&mNvPendingSavesQueue, *ptrTblIdx))
            {
                return gNVM_SaveRequestRejected_c;
            }
        }
    }

    return gNVM_OK_c;
}

/******************************************************************************
 * Name: GetRandomRange
 * Description: Returns a random number between 'low' and 'high'
 * Parameter(s): [IN] low, high - generated number range
 * Return: 0..255
 ******************************************************************************/
static uint8_t GetRandomRange
(
    uint8_t low,
    uint8_t high
)
{
    uint32_t random;
    RNG_GetRandomNo(&random);

    if(high <= low)
    {
        return low;
    }
    return low + (uint8_t)(random % (high - low + 1));
}

/******************************************************************************
 * Name: __NvShutdown
 * Description: The function waits for all idle saves to be processed.
 * Parameter(s):  -
 * Return: -
 *****************************************************************************/
static void __NvShutdown( void )
{
    uint16_t idx = 0;
    /* wait for all operations to complete */
    while(TRUE)
    {
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
        if ((NvGetPendingSavesCount(&mNvPendingSavesQueue)) || (mNvCopyOperationIsPending))
#else
        if (NvGetPendingSavesCount(&mNvPendingSavesQueue))
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
 * Name: __NvCompletePendingOperations
 * Description: The function attemps to complete all the NVM related pending
 *              operations.
 * Parameter(s):  -
 * Return: -
 *****************************************************************************/
static void __NvCompletePendingOperations(void)
{
#if (gNvUseFlexNVM_d == FALSE) /* no FlexNVM */
  uint16_t idx = 0;
  
  if(!mNvModuleInitialized)
  {
    return;
  }
  
  while(idx < gNVM_TABLE_entries_c)
  {
    if((maDatasetInfo[idx].saveNextInterval) && (maDatasetInfo[idx].ticksToNextSave))
    {
      maDatasetInfo[idx].ticksToNextSave = 0;
      mNvSaveOnIntervalEvent = TRUE;
    }
    idx++;
  }

  do
  {
    __NvIdle();
  } while((mNvErasePgCmdStatus.NvErasePending == TRUE) || (mNvCopyOperationIsPending == TRUE) || (mNvPendingSavesQueue.EntriesCount));
#endif
}

#if (gNvUseFlexNVM_d == TRUE) /* FlexNVM */
/******************************************************************************
 * Name: NvGetFlexLastMetaInfo
 * Description: Get FlexRAM last meta information address
 * Parameter(s): -
 * Return: the address of the last valid meta information
 ******************************************************************************/
static uint32_t NvGetFlexLastMetaInfo
(
    void
)
{
    uint32_t address, size;
    NVM_FlexMetaInfo_t flexMetaInfo;

#if FSL_FEATURE_FLASH_IS_FTFL
    address = gFlashConfig.EERAMBase;
    size = gFlashConfig.EEESize;
#endif

#if FSL_FEATURE_FLASH_IS_FTFE
    address = gFlashConfig.FlexRAMBlockBase;
    size = gFlashConfig.FlexRAMTotalSize;
#endif

    while(size)
    {
#if FSL_FEATURE_FLASH_IS_FTFL
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
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
static void NvGetFlexMetaInfoFromId
(
    NvTableEntryId_t tblEntryId,
    NVM_FlexMetaInfo_t* pMetaInfo
)
{
    uint32_t address, size;
    NVM_FlexMetaInfo_t flexMetaInfo;

#if FSL_FEATURE_FLASH_IS_FTFL
    address = gFlashConfig.EERAMBase;
    size = gFlashConfig.EEESize;
#endif

#if FSL_FEATURE_FLASH_IS_FTFE
    address = gFlashConfig.FlexRAMBlockBase;
    size = gFlashConfig.FlexRAMTotalSize;
#endif

    while(size)
    {
#if FSL_FEATURE_FLASH_IS_FTFL
        /* wait for EEPROM system to be ready */
        while(!(REG_BIT_GET(gFlashConfig.ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)));
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
            return;
        }

        address += sizeof(flexMetaInfo);
        size -= sizeof(flexMetaInfo);
    }

    pMetaInfo->rawValue = gNvFlexGuardValue_c;
}

/******************************************************************************
 * Name: NvCheckNvmTableForFlexRAMUsage
 * Description: Check if the existing NVM table fits within the FlexRAM window
 * Parameter(s): -
 * Return: gNVM_NvTableExceedFlexRAMSize_c - the table exceed the size of
 *                                           FlexRAM window
 *         gNVM_OK_c - the table fits within the size of window FlexRAM window
 ******************************************************************************/
static NVM_Status_t NvCheckNvmTableForFlexRAMUsage
(
    void
)
{
    uint16_t loopCnt = 0;
    uint32_t allDatasetSize = 0;
    
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
    if(allDatasetSize > gFlashConfig.EEESize)
#endif

#if FSL_FEATURE_FLASH_IS_FTFE
    if(allDatasetSize > gFlashConfig.FlexRAMTotalSize)
#endif
    {
        return gNVM_NvTableExceedFlexRAMSize_c;
    }

    return gNVM_OK_c;
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
    NVM_Status_t status;
    status = __NvModuleInit();
    if(status != gNVM_OK_c)
    {
        return status;
    }

    if( (mNVMMutexId = OSA_MutexCreate()) == NULL )
    {
        mNvModuleInitialized = FALSE;
        return gNVM_CannotCreateMutex_c;
    }

    #if (gFsciIncluded_c && gNvmEnableFSCIRequests_c)
    FSCI_RegisterOpGroup(gNV_FsciReqOG_d,
                         gFsciMonitorMode_c,
                         NV_FsciMsgHandler,
                         NULL,
                         gNvmDefaultFsciInterface_c);
    #endif
    return gNVM_OK_c;
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
    OSA_InterruptDisable();
    if(mNvCriticalSectionFlag)  /* in case of set/clear mismatch */
    {
        --mNvCriticalSectionFlag;
    }
    
    OSA_InterruptEnable();
    NV_Flash_ClearCriticalSection();
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
    NV_Flash_SetCriticalSection();
    OSA_InterruptDisable();
    ++mNvCriticalSectionFlag;
    OSA_InterruptEnable();
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
    if(mNvIdleTaskId == NULL)
    {
        mNvIdleTaskId = OSA_TaskGetId();
    }
    (void)OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    __NvIdle();
    (void)OSA_MutexUnlock(mNVMMutexId);

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
    if(!mNvModuleInitialized)
    {
        return;
    }

    if(NULL == ptrStat)
    {
        return;
    }

    if(mNvPageCounter%2)
    {
        ptrStat->FirstPageEraseCyclesCount = ptrStat->SecondPageEraseCyclesCount = (mNvPageCounter-1)/2;
    }
    else
    {
        ptrStat->FirstPageEraseCyclesCount = mNvPageCounter/2;
        ptrStat->SecondPageEraseCyclesCount = (mNvPageCounter-2)/2;
    }

    #else /* FlexNVM */
    ptrStat->FirstPageEraseCyclesCount = 0;
    ptrStat->SecondPageEraseCyclesCount = 0;
    return;
    #endif
#else
    ptrStat=ptrStat;
    return;
#endif
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
    OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    status = __NvRegisterTableEntry(ptrData,uniqueId,elemCount,elemSize, dataEntryType, overwrite);
    OSA_MutexUnlock(mNVMMutexId);
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
    OSA_MutexLock(mNVMMutexId, osaWaitForever_c);

    if((status = NvGetEntryFromDataPtr(ptrData, &tblIdx)) != gNVM_OK_c)
    {
        return status;
    }
    tableEntryIdx = NvGetTableEntryIndexFromId(tblIdx.entryId);
    if(gNvInvalidTableEntryIndex_c == tableEntryIdx)
    {
        return gNVM_InvalidTableEntry_c;
    }

    /* invalidate the table entry */
    pNVM_DataTable[tableEntryIdx].pData = NULL;
    pNVM_DataTable[tableEntryIdx].ElementsCount = 0;
    pNVM_DataTable[tableEntryIdx].ElementSize = 0;
    status = __NvEraseEntryFromStorage(tblIdx.entryId, tableEntryIdx);
    OSA_MutexUnlock(mNVMMutexId);
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
    (void) OSA_MutexLock(mNVMMutexId, osaWaitForever_c);
    __NvShutdown();
    (void) OSA_MutexUnlock(mNVMMutexId);
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
    __NvCompletePendingOperations();
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
/******************************************************************************
 * Name: NvProgramFlash
 * Description: The function performs flash write.
 *              The operation is performed in place (atomic).
 * Parameter(s):  [IN]address - flash address
 *                [IN]size    - size to write
 *                [IN]pData   - data to write
 * Return: void
 *****************************************************************************/
void NvProgramFlash(uint32_t address, uint8_t size,void *pData)
{
  (void)NV_FlashProgramUnaligned(address, size,(uint8_t*)(pData));

}

