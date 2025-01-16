/**********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2020-2024 NXP
 *
 *
 * Non-volatile storage module interface declarations for the CORTEX-M4 processor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef _NVM_INTERFACE_H
#define _NVM_INTERFACE_H

/*!
 * @addtogroup NVM
 * The NVM module
 *
 * NVM module provides non volatile memory service to the application and the stack.
 *
 * INFORMATION
 *
 * Data sets are only saved by the idle task. There is no save-immediately
 * capability.
 *
 * There are three save mechanism, each with it's own API function.
 *
 *      1) NvSaveOnIdle(): save the next time the idle task runs. This
 *      will save the data set at least as soon as either of the other
 *      functions, and usually sooner.
 *
 *      2) NvSaveOnInterval(): save no more often than a given time
 *      interval. If it has been at least that long since the last save,
 *      this function will cause a save the next time the idle task runs.
 *
 *      3) NvSaveOnCount(): increment a counter that is associated with the
 *      data set. When that counter equals or exceeds some trigger value,
 *      save the next time the idle task runs.
 *
 * The data set will be saved to NV storage during the next execution of
 * the idle task if:
 *      NvSaveOnIdle() has been called,
 *  OR
 *      NvSaveOnInterval() has been called, and at least the specified
 *      time has past since the last save,
 *  OR
 *      NvSaveOnCount() has been called at least as often as the counter's
 *      trigger value since the last save.
 *
 * All three functions cause NvIsDataSetDirty() to return TRUE. No API is
 * provided to allow the client to know which save function or functions
 * have been called. The data set is either believed to be identical to
 * the saved copy (== not dirty) or not identical (== dirty).
 *
 * The NV code does not require a data set to have a minimum-time-between-
 * saves or a minimum-count-between-saves. A data set may have both.
 *
 * Whenever a data set is saved for any reason:
 *      its dirty flag is cleared
 *  AND
 *      its minimum-time-between-saves timer is restarted from 0,
 *  AND
 *      its minimum-count-between-saves counter is set to 0.
 *
 * @{
 */

#include "EmbeddedTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
******************************************************************************
* Public macros
******************************************************************************
*****************************************************************************/

/*
 * Name: gNvStorageIncluded_d
 * Description: enable/disable NV storage module
 */
#ifndef gNvStorageIncluded_d
#define gNvStorageIncluded_d 1
#endif

/*
 * Name: gNvDebugEnabled_d
 * Description: enable/disable debug mode for NVM
 */
#ifndef gNvDebugEnabled_d
#define gNvDebugEnabled_d 0
#endif

/*
 * Name: gNvFragmentation_Enabled_d
 * Description: enables/disables fragmented saves/restores, i.e. a particular element from a table entry
 *              can be saved / restored.
 */
#ifndef gNvFragmentation_Enabled_d
#define gNvFragmentation_Enabled_d 1
#endif

/*
 * Name: gNvMinimumFreeBytesCountStart_c
 * Description: if the free space at init is smaller than this value, a page copy will be triggered.
 */
#ifndef gNvMinimumFreeBytesCountStart_c
#define gNvMinimumFreeBytesCountStart_c 128U
#endif

/*
 * Name: gNvDualImageSupport_d
 * Description: Used to enable/disable dual image support on NVM
 * Requires also gNvUseExtendedFeatureSet_d
 */
#ifndef gNvDualImageSupport_d
#ifdef gNvDualImageSupport
#warning "gNvDualImageSupport replaced by gNvDualImageSupport_d"
#define gNvDualImageSupport_d gNvDualImageSupport
#else
#define gNvDualImageSupport_d 0
#endif
#endif

#if gNvDualImageSupport_d
#if defined gNvUseExtendedFeatureSet_d
#if (gNvUseExtendedFeatureSet_d == 0)
#warning "gNvUseExtendedFeatureSet_d must be set when using gNvDualImageSupport_d"
#undef gNvUseExtendedFeatureSet_d
#define gNvUseExtendedFeatureSet_d 1
#endif
#else  /* defined gNvUseExtendedFeatureSet_d */
#define gNvUseExtendedFeatureSet_d 1
#endif /* defined gNvUseExtendedFeatureSet_d */
#endif /* gNvDualImageSupport_d*/
/*
 * Name: gNvUseExtendedFeatureSet_d
 * Description: enables/disables the extended feature set of the module:
 *              - removes existing NV table entries
 *              - register new NV table entries
 *              - dynamic NV RAM tables
 */
#ifndef gNvUseExtendedFeatureSet_d
#define gNvUseExtendedFeatureSet_d 0
#endif

/*
 * Name: gUnmirroredFeatureSet_d
 * Description: enables/disables the use of unmirrored in ram feature of the module:
 *              - enables the access of a data set directly from NVM
 *                without a copy in RAM
 */
#ifndef gUnmirroredFeatureSet_d
#define gUnmirroredFeatureSet_d TRUE
#endif

/*
 * Name: gNvTableKeptInRam_d
 * Description: set gNvTableKeptInRam_d to FALSE, if the NVM table is stored in FLASH memory (default)
 *              set gNvTableKeptInRam_d to TRUE, if the NVM table is stored in RAM memory
 */
#ifndef gNvTableKeptInRam_d
#define gNvTableKeptInRam_d FALSE
#endif

/*
 * Name: gNvSalvageFromEccFault_d
 * Description: set gNvSalvageFromEccFault_d to FALSE, if on ECC fault savaging procedure is required.
 *              set gNvSalvageFromEccFault_d to TRUE, if ECC faults must be recovered - slower execution
 */
#ifndef gNvSalvageFromEccFault_d
#define gNvSalvageFromEccFault_d FALSE
#endif

/*
 * Name: gNvVerifyReadBackAfterProgram_d
 * Description: set gNvVerifyReadBackAfterProgram_d to TRUE, if verification is enforced after each flash programming
 * operation. set it to FALSE otherwise. Defaults to TRUE Note that this is required if gNvSalvageFromEccFault_d is set.
 */
#ifndef gNvVerifyReadBackAfterProgram_d
#define gNvVerifyReadBackAfterProgram_d TRUE
#else
#if defined gNvSalvageFromEccFault_d && !defined gNvVerifyReadBackAfterProgram_d
#undef gNvVerifyReadBackAfterProgram_d
#define gNvVerifyReadBackAfterProgram_d TRUE
#endif
#endif

/*
 * Name: gNvLegacyTable_Disabled_d
 * Description: set gNvLegacyTable_Disabled_d to FALSE, if need to recover old format data. Now deprecated.
 *              Kept for maintenance.
 *              set gNvLegacyTable_Disabled_d to TRUE by default.
 */
#ifndef gNvLegacyTable_Disabled_d
#define gNvLegacyTable_Disabled_d TRUE
#endif
/*
 * Name: gNvTableEntriesCountMax_c
 * Description: the maximum count of table entries that the application is
 *              going to use.
 */
#ifndef gNvTableEntriesCountMax_c
#define gNvTableEntriesCountMax_c 32U
#endif

/*
 * Name: gNvRecordsCopiedBufferSize_c
 * Description: the size of the buffer used by page copy function;
 *              it is used to store the indexes of the elements that needs
 *              to be merged (defragmentation); the chosen value must be
 *              a power of 2
 */
#ifndef gNvRecordsCopiedBufferSize_c
#define gNvRecordsCopiedBufferSize_c 64u
#endif

/*
 * Name: gNvCacheBufferSize_c
 * Description: cache buffer size used by internal copy function (no defragmentation);
 *              the chosen value needs to be a multiple of 4
 */
#ifndef gNvCacheBufferSize_c
#define gNvCacheBufferSize_c 16u
#endif

/*
 * Name: gNvMinimumTicksBetweenSaves_c
 * Description: Default minimum-timer-ticks-between-dataset-saves, in seconds
 * Notes: See NvSaveOnInterval(). This is used for all data sets.
 */
#ifndef gNvMinimumTicksBetweenSaves_c
#define gNvMinimumTicksBetweenSaves_c 4u
#endif

/*
 * Name: gNvCountsBetweenSaves_c
 * Description: Default number-of-calls-to-NvSaveOnCount-between-dataset-saves
 * Notes: See NvSaveOnCount(). This is used for all data sets.
 */
#ifndef gNvCountsBetweenSaves_c
#define gNvCountsBetweenSaves_c 256u
#endif

/*
 * Name: gNvInvalidDataEntry_c
 * Description: macro used to mark an invalid entry in NVM table
 */
#ifndef gNvInvalidDataEntry_c
#define gNvInvalidDataEntry_c 0xFFFFU
#endif

/*
 * Name: gNvFormatRetryCount_c
 * Description: retry count of the format operation, when it fails
 */
#ifndef gNvFormatRetryCount_c
#define gNvFormatRetryCount_c 3u
#endif

/*
 * Name: gNvPendingSavesQueueSize_c
 * Description: pending saves queue size
 */
#ifndef gNvPendingSavesQueueSize_c
#define gNvPendingSavesQueueSize_c 32u
#endif

/*
 * Name: gNvTableMarker_c
 * Description: table marker (ASCII = TB)
 */
#ifndef gNvTableMarker_c
#define gNvTableMarker_c 0x4254U
#endif

/*
 * Name: gNvAppVersion_c
 * Description: application version, used for table upgrade
 */
#ifndef gNvFlashTableVersion_c
#define gNvFlashTableVersion_c 1
#endif
/*
 * Name: gNvEnableCriticalSection_c
 * Description: This macro is used to enable/disable protection of the critical
 *               sequences on the  operations nvm saves or erases . If the FLASH
 *               region used by the NVM is placed in the same program block as
 *               the ISR's executable code, the interrupts MUST be disabled.
 *               This could disrupt critical sequences.
 */
#ifndef gNvEnableCriticalSection_c
#define gNvEnableCriticalSection_c (1)
#endif

/*
 * NVM behaviour : defines the policy used to process pending flash write operation to NVM (asynchronous). in the Idle
 * task. Select gNvmUseSaveOnTimerOn_c in order to define a maximum duration between 2 asynchronous save
 * operations. gNvmUseSaveOnTimerJitter_c option is combined with gNvmUseSaveOnTimerOn_c variant to randomize a
 * bit the periodicity of NVM writes. These options may be combined together. Valid options are:
 * #define gNvmSaveOnIdlePolicy_d (gNvmUseSaveOnIntervalTimer_c | gNvmUseSaveIntervalJitter_c)
 * #define gNvmSaveOnIdlePolicy_d   gNvmUseSaveOnTimerOff_c
 * #define gNvmSaveOnIdlePolicy_d   (gNvmUseSaveOnIntervalTimer_c)
 * */
#define gNvmUseSaveOnTimerOff_c    0        /* No use of Save on interval */
#define gNvmUseSaveOnTimerOn_c     (1 << 0) /* Save on interval */
#define gNvmUseSaveOnTimerJitter_c (1 << 1) /* Add jitter to save interval */
#define gNvmUseSaveOnIdleCount_c   (1 << 2) /* Save on Idle count */

#ifndef gNvmSaveOnIdlePolicy_d
#define gNvmSaveOnIdlePolicy_d (gNvmUseSaveOnTimerOn_c | gNvmUseSaveOnTimerJitter_c)
//#define gNvmSaveOnIdlePolicy_d    gNvmUseSaveOnTimerOff_c
//#define gNvmSaveOnIdlePolicy_d    gNvmUseSaveOnTimerOn_c
#endif

/* Debugging */
#ifndef gNvmEnableFSCIRequests_c
#define gNvmEnableFSCIRequests_c (0)
#endif

#ifndef gNvmEnableFSCIMonitoring_c
#define gNvmEnableFSCIMonitoring_c (0)
#endif

#ifndef gNvmDefaultFsciInterface_c
#define gNvmDefaultFsciInterface_c (0)
#endif

#ifndef gNvmMemPoolId_c
#define gNvmMemPoolId_c (0)
#endif

/* Define section for keeping NVM table datasets */
#if (defined(__CC_ARM) || defined(__ARMCC_VERSION))

extern uint32_t Image$$RW_NVM_TABLE$$Base[];
extern uint32_t Image$$RW_NVM_TABLE$$Limit[];
extern uint32_t Image$$RO_NVM_TABLE$$Base[];
extern uint32_t Image$$RO_NVM_TABLE$$Limit[];

#define gNVM_TABLE_RO_startAddr_c ((NVM_DataEntry_t *)Image$$RO_NVM_TABLE$$Base)
#define gNVM_TABLE_RO_endAddr_c   ((NVM_DataEntry_t *)Image$$RO_NVM_TABLE$$Limit)
#define gNVM_TABLE_RW_startAddr_c ((NVM_DataEntry_t *)Image$$RW_NVM_TABLE$$Base)
#define gNVM_TABLE_RW_endAddr_c   ((NVM_DataEntry_t *)Image$$RW_NVM_TABLE$$Limit)

#elif defined(__GNUC__)

extern uint32_t __start_NVM_TABLE[];
extern uint32_t __stop_NVM_TABLE[];
extern uint32_t __start_NVM_TABLE_RW[];
extern uint32_t __stop_NVM_TABLE_RW[];

#define gNVM_TABLE_RO_startAddr_c ((NVM_DataEntry_t *)((void *)__start_NVM_TABLE))
#define gNVM_TABLE_RO_endAddr_c   ((NVM_DataEntry_t *)((void *)__stop_NVM_TABLE))
#define gNVM_TABLE_RW_startAddr_c ((NVM_DataEntry_t *)((void *)__start_NVM_TABLE_RW))
#define gNVM_TABLE_RW_endAddr_c   ((NVM_DataEntry_t *)((void *)__stop_NVM_TABLE_RW))

#elif (defined(__IAR_SYSTEMS_ICC__))

#define gNVM_TABLE_RO_startAddr_c ((NVM_DataEntry_t *)__section_begin("NVM_TABLE"))
#define gNVM_TABLE_RO_endAddr_c   ((NVM_DataEntry_t *)__section_end("NVM_TABLE"))
#define gNVM_TABLE_RW_startAddr_c ((NVM_DataEntry_t *)__section_begin("NVM_TABLE_RW"))
#define gNVM_TABLE_RW_endAddr_c   ((NVM_DataEntry_t *)__section_end("NVM_TABLE_RW"))
#else

#define gNVM_TABLE_RO_startAddr_c ((NVM_DataEntry_t *)0)
#define gNVM_TABLE_RO_endAddr_c   ((NVM_DataEntry_t *)0)
#define gNVM_TABLE_RW_startAddr_c ((NVM_DataEntry_t *)0)
#define gNVM_TABLE_RW_endAddr_c   ((NVM_DataEntry_t *)0)

#warning Module information will not be stored!

#endif

#if gNvTableKeptInRam_d
#define gNVM_TABLE_startAddr_c gNVM_TABLE_RW_startAddr_c
#define gNVM_TABLE_endAddr_c   gNVM_TABLE_RW_endAddr_c
#else
#define gNVM_TABLE_startAddr_c gNVM_TABLE_RO_startAddr_c
#define gNVM_TABLE_endAddr_c   gNVM_TABLE_RO_endAddr_c
#endif

#define gNVM_TABLE_entries_c \
    ((uint16_t)(((uint32_t)gNVM_TABLE_endAddr_c - (uint32_t)gNVM_TABLE_startAddr_c) / sizeof(NVM_DataEntry_t)))

#define SET_DATASET_STRUCT_NAME(datasetId) gNvmTableEntry##_##datasetId

/* Initializer of NVM_TABLE Entry descriptor */
#define NVM_DataSetInit(p_Data, elementsCount, elementSize, dataEntryID, dataEntryType)                          \
    {                                                                                                            \
        .pData = p_Data, .ElementsCount = elementsCount, .ElementSize = elementSize, .DataEntryID = dataEntryID, \
        .DataEntryType = dataEntryType                                                                           \
    }

/* NVM Data sets are gathered in a section named NVM_TABLE that is constituted of NVM_DataEntry_t structures.
 * Each invocation of the NVM_RegisterDataSet macro appends an NVM_DataEntry_t element to the section.
 * This section is placed in Flash or RAM depending on gNvTableKeptInRam_d configuration.
 * The NVM firmware requires the linker to provide the symbols to determine the start address and
 * the end address of the section (or its size): gNVM_TABLE_startAddr_c and gNVM_TABLE_endAddr_c.
 * gNVM_TABLE_entries_c is the number of NVM_DataEntry_t elements comprised in the NVM_TABLE section.
 * In the case of the RAM placement the section is named NVM_TABLE_RW instead of NVM_TABLE.
 * IAR places 'naturally' the NVM_TABLE section based on the 'const' keyword, either in ROM or RAM, so the renaming
 * is not strictly required but just clearer.
 * When the gNvTableKeptInRam_d option is defined, the NVM_DataEntry_t structure od the NVM_TABLE_RW section are
 * to be initialized either at startup time by C bootstrap data initialization or explicitly by program.
 * The size of the NVM_TABLE_RW section may need to be predefined to reserve headroom for Data entries to be
 * registered 'dynamically' . In that case gNVM_TABLE_entries_c matches the dimension of the reserved array.
 * With armgcc toolchain, the name of the section must be distinguished so that the start and end addresses of the
 * section can be provided.
 *
 */
#if defined(__IAR_SYSTEMS_ICC__)

#if gNvTableKeptInRam_d
#define NV_TB_PLACEMENT_START _Pragma("location=\"NVM_TABLE_RW\"") __root
#else
#define NV_TB_PLACEMENT_START _Pragma("location=\"NVM_TABLE\"") __root const
#endif
#define NV_TB_PLACEMENT_END

#else /* not IAR */

#if gNvTableKeptInRam_d
#define NV_TB_PLACEMENT_START
#else
#define NV_TB_PLACEMENT_START const
#endif

#if (defined(__CC_ARM) || defined(__ARMCC_VERSION))
#if gNvTableKeptInRam_d
#define NV_TB_PLACEMENT_END __attribute__((section("NVM_TABLE_RW"))) __attribute__((used))
#else  /* gNvTableKeptInRam_d */
#define NV_TB_PLACEMENT_END __attribute__((section("NVM_TABLE"))) __attribute__((used))
#endif /*gNvTableKeptInRam_d */
#elif defined(__GNUC__)
#if gNvTableKeptInRam_d
#define NV_TB_PLACEMENT_END __attribute__((section(".NVM_TABLE_RW"))) __attribute__((used))
#else  /* gNvTableKeptInRam_d */
#define NV_TB_PLACEMENT_END __attribute__((section(".NVM_TABLE"), used))
#endif /*gNvTableKeptInRam_d */
#else  /* undefined toolchain */
#define NV_TB_PLACEMENT_END
#endif
#endif

#if gNvTableKeptInRam_d
#if defined(__IAR_SYSTEMS_ICC__)
#pragma section = "NVM_TABLE_RW"

#define NVM_RegisterDataSet(pData, elementsCount, elementSize, dataEntryID, dataEntryType)             \
    extern NVM_DataEntry_t                                      SET_DATASET_STRUCT_NAME(dataEntryID);  \
    _Pragma("location=\"NVM_TABLE_RW\"") __root NVM_DataEntry_t SET_DATASET_STRUCT_NAME(dataEntryID) = \
        NVM_DataSetInit(pData, elementsCount, elementSize, dataEntryID, dataEntryType)
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
#define NVM_RegisterDataSet(pData, elementsCount, elementSize, dataEntryID, dataEntryType)        \
    NVM_DataEntry_t SET_DATASET_STRUCT_NAME(dataEntryID) __attribute__((section("NVM_TABLE_RW"))) \
    __attribute__((used)) = NVM_DataSetInit(pData, elementsCount, elementSize, dataEntryID, dataEntryType)
#elif defined(__GNUC__)
#define NVM_RegisterDataSet(pData, elementsCount, elementSize, dataEntryID, dataEntryType)                        \
    extern NVM_DataEntry_t SET_DATASET_STRUCT_NAME(dataEntryID);                                                  \
    NVM_DataEntry_t        SET_DATASET_STRUCT_NAME(dataEntryID) __attribute__((section(".NVM_TABLE_RW"), used)) = \
        NVM_DataSetInit(pData, elementsCount, elementSize, dataEntryID, dataEntryType)
#else
#warning Unknown/undefined toolchain!
#endif

#else /* gNvTableKeptInRam_d */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma section = "NVM_TABLE"

#define NVM_RegisterDataSet(pData, elementsCount, elementSize, dataEntryID, dataEntryType)                \
    extern const NVM_DataEntry_t                                   SET_DATASET_STRUCT_NAME(dataEntryID);  \
    _Pragma("location=\"NVM_TABLE\"") __root const NVM_DataEntry_t SET_DATASET_STRUCT_NAME(dataEntryID) = \
        NVM_DataSetInit(pData, elementsCount, elementSize, dataEntryID, dataEntryType)

#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))

#define NVM_RegisterDataSet(pData, elementsCount, elementSize, dataEntryID, dataEntryType)           \
    const NVM_DataEntry_t SET_DATASET_STRUCT_NAME(dataEntryID) __attribute__((section("NVM_TABLE"))) \
    __attribute__((used)) = NVM_DataSetInit(pData, elementsCount, elementSize, dataEntryID, dataEntryType)

#elif defined(__GNUC__)
#define NVM_RegisterDataSet(pData, elementsCount, elementSize, dataEntryID, dataEntryType)                           \
    extern const NVM_DataEntry_t SET_DATASET_STRUCT_NAME(dataEntryID);                                               \
    const NVM_DataEntry_t        SET_DATASET_STRUCT_NAME(dataEntryID) __attribute__((section(".NVM_TABLE"), used)) = \
        NVM_DataSetInit(pData, elementsCount, elementSize, dataEntryID, dataEntryType)
#else
#warning Unknown/undefined toolchain!
#endif
#endif

/*****************************************************************************
******************************************************************************
* Public type declarations
******************************************************************************
*****************************************************************************/

/*!
 * \brief  Data type definition used by dataset save on count function.
 */
typedef uint16_t NvSaveCounter_t;

/*!
 * \brief  Data type definition used by dataset save on interval function.
 */
typedef uint16_t NvSaveInterval_t;

/*!
 * \brief  Data type definition for the unique ID of each NVM table entry.
 */
typedef uint16_t NvTableEntryId_t;

/*!
 * \struct NVM_DatasetInfo_t
 * \brief Data type definition for a dataset (NV table entry) information.
 */
typedef struct NVM_DatasetInfo_tag
{
    bool_t           saveNextInterval; /*!<  dataset to be saved on next interval */
    NvSaveInterval_t ticksToNextSave;  /*!<  ticks to next save interval */
    NvSaveCounter_t  countsToNextSave; /*!<  counts to next save interval */
#if gUnmirroredFeatureSet_d
    uint16_t elementIndex;             /*!<  element index */
#endif
} NVM_DatasetInfo_t;

/*!
 * \brief Enumerated data type definition for NVM Data entry.
 */
typedef enum NVM_DataEntryType_tag
{
    gNVM_MirroredInRam_c,              /*!< entry mirrored */
#if gUnmirroredFeatureSet_d
    gNVM_NotMirroredInRam_c,           /*!< entry not mirrored  */
    gNVM_NotMirroredInRamAutoRestore_c /*!< entry not mirrored, should be restored at initialization */
#endif
} NVM_DataEntryType_t;

/*!
 * \struct NVM_DataEntry_t
 * \brief Data type definition for a NV table entry.
 */
typedef struct NVM_DataEntry_tag
{
    void    *pData;         /*!< pointer to the RAM memory location where the dataset elements are stored */
    uint16_t ElementsCount; /*!< number of elements the dataset has */
    uint16_t ElementSize;   /*!< size of a single element */
    uint16_t DataEntryID;   /*!< a 16-bit unique ID of the dataset */
    uint16_t DataEntryType; /*!< type of entry (mirrored/unmirrored/unmirrored auto restore) */
} NVM_DataEntry_t;

/*!
 * \brief Enumerated data type definition for NV storage module error codes.
 */
typedef enum NVM_Status_tag
{
    gNVM_OK_c,                       /*!< operation successful */
    gNVM_Error_c,                    /*!< in case of error */
    gNVM_InvalidPageID_c,            /*!< page ID is not valid*/
    gNVM_PageIsNotBlank_c,           /*!< page is not blank */
    gNVM_SectorEraseFail_c,          /*!< page cannot be erased */
    gNVM_NullPointer_c,              /*!< provided pointer is null */
    gNVM_PointerOutOfRange_c,        /*!< pointer is out of range */
    gNVM_AddressOutOfRange_c,        /*!< address is out of range */
    gNVM_InvalidSectorsCount_c,      /*!< sector count configured in the project linker file is invalid*/
    gNVM_InvalidTableEntry_c,        /*!< table entry is not valid */
    gNVM_PageIsEmpty_c,              /*!< page is empty */
    gNVM_MetaNotFound_c,             /*!< meta info not found */
    gNVM_RecordWriteError_c,         /*!< record couldn't be written */
    gNVM_MetaInfoWriteError_c,       /*!< meta info couldn't be written */
    gNVM_ModuleNotInitialized_c,     /*!< NVM  module is not initialized */
    gNVM_CriticalSectionActive_c,    /*!< NVM  module is in critical sequence */
    gNVM_ModuleAlreadyInitialized_c, /*!< NVM  module already initialized*/
    gNVM_PageCopyPending_c,          /*!< page copy is pending */
    gNVM_RestoreFailure_c,           /*!< recovery failure */
    gNVM_FormatFailure_c,            /*!< format operation fails */
    gNVM_RegisterFailure_c,          /*!< invalid id or unmirrored data set */
    gNVM_AlreadyRegistered_c,        /*!< id already registered in another entry*/
    gNVM_SaveRequestRejected_c,      /*!< request couldn't be queued */
    gNVM_NvTableExceedFlexRAMSize_c, /*!< the table exceed the size of FlexRAM window */
    gNVM_NvWrongFlashDataIFRMap_c,   /*!< invalid data flash IFR map */
    gNVM_CannotCreateMutex_c,        /*!< no mutex available */
    gNVM_NoMemory_c,                 /*!< no free memory block */
    gNVM_IsMirroredDataSet_c,        /*!< data set is mirrored in RAM */
    gNVM_DefragBufferTooSmall_c,     /*!< buffer too small */
    gNVM_ReservedFlashTooSmall_c,    /*!< Flash buffer is too small */
    gNVM_FragmentedEntry_c,          /*!< entry fragmented */
    gNVM_AlignmentError_c,           /*!< alignment error */
    gNVM_InvalidTableEntriesCount_c, /*!< invalid table entries count */
    gNVM_SaveRequestRecursive_c,     /*!< save request flag to run again */
    gNVM_AtomicSaveRecursive_c,      /*!< atomic save request flag to run again*/
    gNVM_EccFault_c,                 /*!< ECC Fault detect on read */
    gNVM_EccFaultWritingRecord_c,    /*!< ECC Fault detected on record read back after write */
    gNVM_EccFaultWritingMeta_c,      /*!< ECC Fault detected on meta info read back after write */
    gNVM_NbStatusCodes_c             /*< Not an erro code : number of existing */

} NVM_Status_t;

/*!
 * \struct NVM_Statistics_t
 * \brief Data structure type used to store pages statistic information
 *              (erase cycle count of each page)
 */
typedef struct NVM_Statistics_tag
{
    uint32_t FirstPageEraseCyclesCount;
    uint32_t SecondPageEraseCyclesCount;
} NVM_Statistics_t;

/*!
 * \brief ECC fault notification callback function pointer.
 *  \param [in] fault_addr address where ECC fault was detected
 *  \param [in] operation 0: write (programming flash), 1: read operation
 */
typedef void (*NVM_EccFaultNotifyCb_t)(uint32_t fault_addr, int operation);

/*****************************************************************************
******************************************************************************
* Public memory declarations
*****************************************************************************
*****************************************************************************/

#if gNvStorageIncluded_d
/*
 * Name: pNVM_DataTable
 * Description: a pointer to the NV data table stored in RAM
 */
extern NVM_DataEntry_t *pNVM_DataTable;

#endif /* #if gNvStorageIncluded_d */

/*****************************************************************************
******************************************************************************
* External declarations
*****************************************************************************
*****************************************************************************/

#ifndef gHostApp_d
#if 0
/*
 * Name: BUtl_SetReceiverOff
 * Description: Turn OFF the 802.15.4 receiver
 * Parameter(s): none
 * Return: -
 */
extern void BUtl_SetReceiverOff(void);

/*
 * Name: BUtl_SetReceiverOn
 * Description: Turn ON the 802.15.4 receiver
 * Parameter(s): none
 * Return: -
 */
extern void BUtl_SetReceiverOn(void);
#endif

/*
 * Name: NvOperationStart
 * Description: Turn OFF the 802.15.4 receiver
 */
#define NvOperationStart() BUtl_SetReceiverOff()
/*
 * Name: NvOperationEnd
 * Description: Turn ON the 802.15.4 receiver
 */
#define NvOperationEnd() BUtl_SetReceiverOn()
#else
#define NvOperationStart()
#define NvOperationEnd()
#endif

/*****************************************************************************
******************************************************************************
* Public prototypes (Module Interface)
******************************************************************************
*****************************************************************************/

/*!*********************************************************************************
 * \brief Initialise the NV storage module
 *
 * \details This function is used to initialize the Flash Management and Non-Volatile
 *          Storage Module. It indirectly initializes the flash HAL driver and gets
 *          the active page ID. It initializes internal state variables and counters.
 *          If the RAM entries are different from flash entries, a page copy is triggered
 *          and the different entries are skipped in the process. It also handles NVM
 *          table changes.
 *          For example, the MCU program was changed and the NVM table was updated
 *          by automatically doing a table upgrade.
 *          To trigger this behavior, the application must change gNvFlashTableVersion_c.
 *
 * \return NVM_Status_t: \n
 *         gNVM_ModuleAlreadyInitialized_c: if the module is already initialised\n
 *         gNVM_InvalidSectorsCount_c: if the sector count configured in the
 *                                      project linker file is invalid\n
 *         gNVM_MetaNotFound_c: if no meta information was found\n
 *         gNVM_OK_c: module was successfully initialised
 ********************************************************************************* */
extern NVM_Status_t NvModuleInit(void);

/*! *********************************************************************************
 * \brief Reinit the NV module , reload from Flash to RAM the latest NVM changes.\n
 *        Usefull for RAM off use case in lowpower.
 *
 * \note  Same than __NvModuleInit without call to NV_Init(); and no mNvModuleInitialized protection.
 *
 * \return NVM_Status_t: \n
 *         gNVM_ModuleAlreadyInitialized_c: if the module is already initialised\n
 *         gNVM_InvalidSectorsCount_c: if the sector count configured in the
 *                                      project linker file is invalid\n
 *         gNVM_MetaNotFound_c: if no meta information was found\n
 *         gNVM_OK_c: module was successfully initialised
 ********************************************************************************* */
/* TODO : merge with __NvModuleInit some common processing */
extern NVM_Status_t NvModuleReInit(void);

/*! *********************************************************************************
 * \brief Force clean reset of NVM structures.
 *
 * \note  Mostly useful for unit testing.
 *
 ********************************************************************************* */
extern void NvModuleDeInit(void);

/*! *********************************************************************************
 * \brief Move from NVM to Ram
 *
 * \details This function moves the data pointed to by ppData from flash to RAM (allocates
 *          space and copies the data). It can only move a single element. It changes pData
 *          in the NVM table to point to the new location. If the specified element is
 *          already in RAM, the function cancels any pending saves and returns. The
 *          unmirrored functionality must be enabled if this function is required.
 *
 * \param[in] ppData double pointer to the entity to be moved from flash to RAM
 *
 * \return gNVM_OK_c: if operation completed successfully\n
 *         gNVM_NoMemory_c: in case there is not a memory block free\n
 *         gNVM_NullPointer_c: if the provided pointer is NULL \n
 *         gNVM_PointerOutOfRange_c: if the provided pointer is not found within the RAM table
 ********************************************************************************* */
extern NVM_Status_t NvMoveToRam(void **ppData);

/*! *********************************************************************************
 * \brief Retrieves the version stored in the flash table or 0 if no table is detected.
 *        The extended functionality must be enabled if this function is required
 *
 * \return 0 or flash table version
 ********************************************************************************* */
extern uint16_t GetFlashTableVersion(void);

/*! *********************************************************************************
 * \brief Erase from NVM an unmirrored dataset
 *
 * \details It generally does not trigger a page copy, but writes a new record in the
 *          NVM memory that specifies that this element was removed. If the page is
 *          full, a page copy is triggered. It sets pData for the specified element
 *          to NULL. It can only erase a single element. If the specified element is
 *          in RAM, it is freed but no changes are made in flash. The unmirrored
 *          functionality must be enabled if this function is required.
 *
 * \param[in] ppData double pointer to the entity to be moved from flash to RAM
 *
 * \return gNVM_OK_c: if operation completed successfully\n
 *         gNVM_NoMemory_c: in case there is not a memory block free
 *         gNVM_NullPointer_c: if the provided pointer is NULL \n
 *         gNVM_PointerOutOfRange_c: if the provided pointer is not found within the RAM table
 ********************************************************************************* */
extern NVM_Status_t NvErase(void **ppData);

/*! *********************************************************************************
 * \brief Saves the dataset pointed by ptrData on the next call to NvIdle()
 *
 * \details This function saves the element or the entire NV table entry (dataset)
 *          pointed to by the ptrData argument, as soon as the NvIdle() function is
 *          called. If ptrData belongs to an unmirrored dataset, after the save the
 *          RAM pointer is freed and pData points to the flash backup. No other saves
 *          can be made while the data is in flash.
 *
 * \param[in] ptrData pointer to data to be saved
 * \param[in] saveAll specify if all the elements from the NVM table entry shall be saved
 *
 * \return gNVM_OK_c: if operation completed successfully\n
 *         gNVM_Error_c: in case of error(s)
 *         gNVM_NullPointer_c: if the provided pointer is NULL \n
 *         gNVM_PointerOutOfRange_c: if the provided pointer is not found within the RAM table
 ********************************************************************************* */
extern NVM_Status_t NvSaveOnIdle(void *ptrData, bool_t saveAll);

/*! *********************************************************************************
 * \brief Saves a dataset no more often than a given time interval.
 *
 * \details This function saves the specified dataset no more often than at a given
 *          time interval. If it has been at least that long since the last save,
 *          this function causes a save as soon as the NvIdle() function is called.
 *          If ptrData belongs to an unmirrored dataset, after the save the RAM
 *          pointer is freed and pData points to the flash backup. No other saves
 *          can be made while the data is in flash. If ptrData belongs to a mirrored
 *          dataset, a full save is made. Otherwise, only the element indicated by
 *          the ptrData is saved. If the function is called before a previous save
 *          on interval was processed, the call is ignored.
 *
 * \param[in] ptrData pointer to data to be saved
 *
 * \return gNVM_OK_c: if operation completed successfully
 *         gNVM_NullPointer_c: if the provided pointer is NULL \n
 *         gNVM_PointerOutOfRange_c: if the provided pointer is not found within the RAM table
 ********************************************************************************* */
extern NVM_Status_t NvSaveOnInterval(void *ptrData);

/*! *********************************************************************************
 * \brief Decrement the counter. Once it reaches 0, the next call to
 *        NvIdle() will save the entire table entry (all elements).
 *
 * \details If the ptrData belongs to an unmirrored dataset, after the save, the
 *          RAM pointer is freed and pData points to the flash backup. No other saves
 *          can be made while the data is in flash. If ptrData belongs to a mirrored
 *          dataset, a full save is made. Otherwise, only the element indicated by ptrData is saved.
 *
 * \param[in] ptrData pointer to data to be saved
 *
 * \return gNVM_OK_c: if operation completed successfully
 *         gNVM_NullPointer_c: if the provided pointer is NULL \n
 *         gNVM_PointerOutOfRange_c: if the provided pointer is not found within the RAM table
 ********************************************************************************* */
extern NVM_Status_t NvSaveOnCount(void *ptrData);

/*! *********************************************************************************
 * \brief Set the timer used by NvSaveOnInterval().
 *
 * \details This function sets a new value of the timer interval that is used by
 *          the 'save on interval' mechanism. The change takes effect after the
 *          next save is completed.
 *
 * \param[in] newInterval new time interval
 ********************************************************************************* */
extern void NvSetMinimumTicksBetweenSaves(NvSaveInterval_t newInterval);

/*! *********************************************************************************
 * \brief Set the counter trigger value used by NvSaveOnCount().
 *
 * \details This function sets a new value of the counter trigger that is used by
 *          the 'save on count' mechanism. The change takes effect after the next
 *          save is completed.
 *
 * \param[in] newCounter new counter value
 ********************************************************************************* */
extern void NvSetCountsBetweenSaves(NvSaveCounter_t newCounter);

/*! *********************************************************************************
 * \brief Called from the idle task to process save-on-interval requests
 *
 * \details This function processes NvSaveOnInterval() requests. If the call of this
 *          function counts a timer tick, call it with countTick set to TRUE. Otherwise,
 *          call it with countTick set to FALSE. Regardless of the value of countTick,
 *          NvTimerTick() returns TRUE if one or more of the datasets tick counters
 *          have not yet counted down to zero, or FALSE if all data set tick counters
 *          have reached zero. This function is called automatically inside the module
 *          to process interval saves, but it can be called from the application if necessary.
 *
 * \param[in] countTick enable/disable tick count
 * \return FALSE if the timer tick counters for all data sets have reached
 *         zero. In this case, the timer can be turned off.\n
 *         TRUE if any of the data sets' timer tick counters have not yet
 *         counted down to zero. In this case, the timer should be active
 ********************************************************************************* */
extern bool_t NvTimerTick(bool_t countTick);

/*! *********************************************************************************
 * \brief Copy the most recent version of the element/table entry pointed
 *        by ptrData from NVM storage system to RAM memory
 *
 * \details This function restores the element or the entire NV table entry specified
 *          by the function argument ptrData. If a valid table entry copy is found in
 *          the flash memory, it is restored to RAM NV Table.For unmirrored datasets,
 *          the function only restores a pointer to the flash location of the entry.
 *          In order to restore the data from flash to RAM, the application has to call NvMoveToRam.
 *
 * \param[in] ptrData pointer to data (element) to be restored
 * \param[in] restoreAll if FALSE restores a single element. if TRUE restores an entire table entry
 *
 * \return status of the restore operation
 ********************************************************************************* */
extern NVM_Status_t NvRestoreDataSet(void *ptrData, bool_t restoreAll);

/*! *********************************************************************************
 * \brief Enter critical sequence
 *
 * \details This function increments an internal counter variable each time it is called.
 *          All the sync save/erase/copy functions are checking this counter before
 *          executing their code. If the counter has a nonzero value, the function
 *          returns with no further operations. This function guarantees that all the
 *          sync save/erase/copy operations are put in the pending queue while the
 *          critical sequence is on and processed when the critical sequence is lifter,
 *          on the idle task.
 ********************************************************************************* */
extern void NvSetCriticalSection(void);

/*! *********************************************************************************
 * \brief Leave critical sequence
 *
 * \details This function decrements an internal counter variable each time it is called.
 *          All the sync save/erase/copy functions are checking this counter before executing
 *          their code. If the counter has a nonzero value, the function returns with
 *          no further operations.
 ********************************************************************************* */
extern void NvClearCriticalSection(void);

/*! *********************************************************************************
 * \brief  Processes the NvSaveOnIdle() and NvSaveOnCount() requests.
 *
 * \details This function processes the NvSaveOnIdle() and NvSaveOnCount() requests.
 *          It also checks if the internal timer made a tick and determines if any
 *          save on interval should be processed. It also does page copy and erase.
 *          Any saves that were not processed when the critical sequence was active
 *          are processed here. It must be called from a low-priority task, such as
 *          Idle task.Called from the idle task (bare-metal) or NVM_Task (MQX,
 *          FreeRTOS) to process the pending saves, erase or copy operations.
 *
 * \return Number of operations executed.
 ********************************************************************************* */
extern int NvIdle(void);

/*! *********************************************************************************
 * \brief returns the Id of the task which hosts NvIdle function
 *
 * \return Id of the task which hosts NvIdle function
 ********************************************************************************* */
void *NvGetNvIdleTaskId(void);

/*! *********************************************************************************
 * \brief Check if element pointed is dirty(a save is pending on the specified dataset).
 *
 * \param[in] ptrData pointer to data to be checked
 * \return TRUE if the element is dirty, FALSE otherwise
 ********************************************************************************* */
extern bool_t NvIsDataSetDirty(void *ptrData);

/*! *********************************************************************************
 * \brief Returns the virtual pages statistics (how many times each virtual page has been erased).
 *
 * \param[out] ptrStat pointer to a memory location where the pages statistics (erase cycles
 *             of each page) are stored
 ********************************************************************************* */
extern void NvGetPagesStatistics(NVM_Statistics_t *ptrStat);

/*! *********************************************************************************
 * \brief Retrieves the NV Virtual Page size
 *
 * \param[out] pPageSize pointer to a memory location where the page size will be stored
 ********************************************************************************* */
void NvGetPagesSize(uint32_t *pPageSize);

/*! *********************************************************************************
 * \brief Format the NV storage system.\n
 *        The function erases both virtual pages and then writes the page counter to active page.
 *
 * \return gNVM_OK_c: if the operation completes successfully \n
 *         gNVM_FormatFailure_c:  if the format operation fails \n
 *         gNVM_ModuleNotInitialized_c: if the NVM  module is not initialised \n
 *         gNVM_CriticalSectionActive_c: if the system has entered in a critical sequence
 ********************************************************************************* */
extern NVM_Status_t NvFormat(void);

/*! *********************************************************************************
 *  \brief The function tries to register a new table entry within an existing NV table.
 *
 * \details If the NV table contained an erased (invalid) entry, the entry will be overwritten
 *          with a new one (provided by the mean of this function arguments). This triggers
 *          a page copy. If the critical sequence is active, the page copy is done as soon as
 *          it is deactivated. Extended functionality must be enabled if this function is required.
 *
 * \param[in] ptrData generic pointer to RAM data to be registered within the NV storage system
 * \param[in] uniqueId an unique ID of the table entry
 * \param[in] elemCount how many elements the table entry contains
 * \param[in] elemSize the size of an element
 * \param[in] dataEntryType the type of the new entry
 * \param[in] overwrite if an existing table entry shall be overwritten
 *            Otherwise, if overwrite is set to FALSE, the data is placed in the first
 *            free position in the table.
 *
 * \return gNVM_OK_c: if the operation completes successfully\n
 *         gNVM_ModuleNotInitialized_c: if the NVM  module is not initialised
 ********************************************************************************* */
extern NVM_Status_t NvRegisterTableEntry(void            *ptrData,
                                         NvTableEntryId_t uniqueId,
                                         uint16_t         elemCount,
                                         uint16_t         elemSize,
                                         uint16_t         dataEntryType,
                                         bool_t           overwrite);

/*! *********************************************************************************
 *  \brief Removes a table entry within the existing NV table.
 *
 * \details A page copy is triggered and the data associated with the entry is not copied.
 *          If the critical sequence is active, the page copy is done as soon as it is
 *          deactivated. It sets entrySize and entryCount to 0 and pData to NULL. EntryId
 *          remains unchanged. For unmirrored datasets, use NvErase because it can delete
 *          individual elements. The extended functionality must be enabled if this function is required
 *
 * \param[in] ptrData a pointer to an existing RAM data that is managed by the NV storage system
 *
 * \return gNVM_OK_c: if the operation completes successfully\n
 *         gNVM_ModuleNotInitialized_c: if the NVM  module is not initialised\n
 *         gNVM_NullPointer_c: if a NULL pointer is provided
 ********************************************************************************* */
extern NVM_Status_t NvEraseEntryFromStorage(void *ptrData);

/*! *********************************************************************************
 *  \brief Saves the pointed element or the entire table entry to the storage system.
 *
 * \details The save operation is not performed on the idle task but within this function call.
 *          If ptrData belongs to an unmirrored dataset, after the save, the RAM pointer is
 *          freed and pData points to the flash backup. Also, saveAll is ignored and only
 *          individual elements can be saved. No other saves can be made while the data is in flash.
 *
 * \param[in] ptrData a pointer to data to be saved
 * \param[in] saveAll specifies if the entire table entry shall be saved or only the pointed element
 * \return gNVM_OK_c: if the operation completes successfully\n
 *         gNVM_ModuleNotInitialized_c: if the NVM  module is not initialized\n
 *         gNVM_NullPointer_c: if a NULL pointer is provided\n
 *         gNVM_PointerOutOfRange_c: if the pointer is out of range\n
 *         gNVM_InvalidTableEntry_c: if the table entry is not valid\n
 *         gNVM_MetaInfoWriteError_c: meta tag couldn't be written\n
 *         gNVM_RecordWriteError_c: record couldn't be written\n
 *         gNVM_CriticalSectionActive_c: the module is in critical sequence
 ********************************************************************************* */
extern NVM_Status_t NvSyncSave(void *ptrData, bool_t saveAll);

/*! *********************************************************************************
 *  \brief Performs an atomic save of the entire NV table to the storage system.\n
 *         The operation is performed in place (atomic).
 *
 * \return gNVM_OK_c: if the operation completes successfully\n
 *         gNVM_ModuleNotInitialized_c: if the NVM  module is not initialized\n
 *         gNVM_NullPointer_c : if a NULL pointer is provided\n
 *         gNVM_PointerOutOfRange_c: if the pointer is out of range\n
 *         gNVM_InvalidTableEntry_c: if the table entry is not valid\n
 *         gNVM_MetaInfoWriteError_c: meta tag couldn't be written\n
 *         gNVM_RecordWriteError_c: record couldn't be written\n
 *         gNVM_CriticalSectionActive_c: the module is in critical sequence
 ********************************************************************************* */
extern NVM_Status_t NvAtomicSave(void);

/*! *********************************************************************************
 * \brief Blocks until all the saves in queue, page copy operations, and interval
 *        saves have been processed to ensure that the MCU has the latest data before a reset.
 *
 ********************************************************************************* */
extern void NvShutdown(void);

/*! *********************************************************************************
 * \brief Attemps to complete all the NVM related pending operations, like queued
 *        writes, page copy, page erase and save on interval requests.
 *
 ********************************************************************************* */
extern void NvCompletePendingOperations(void);

/*! *********************************************************************************
 *  \brief Reads a flash entry so that the application can handle dynamic entries.
 *
 * \details It is used to determine before initialization if some flash entries are
 *          different than the RAM counterparts. To use data stored in the flash table,
 *          the application should copy the members returned by this function in their
 *          respective RAM entry and allocate pData. The extended functionality must
 *          be enabled if this function is required.
 *          Does nothing if gNvUseExtendedFeatureSet_d is unset.
 *
 * \param[in] index the ram entry index
 * \param[out] entry the flash entry at the specified index
 * \return gNVM_OK_c: if the operation completes successfully\n
           gNVM_RestoreFailure_c: if the operation failed\n
           gNVM_AddressOutOfRange_c: if the index is too large\n
           gNVM_Error_c: not supported, NVM table is stored in FLASH
 ********************************************************************************* */
extern NVM_Status_t RecoverNvEntry(uint16_t index, NVM_DataEntry_t *entry);

/*! *********************************************************************************
 *  \brief Tell if there is a pending NVM operation in the queue
 *
 * \return bool Is there a pending operation in the queue
 ********************************************************************************* */
bool_t NvIsPendingOperation(void);

/*! *********************************************************************************
 *  \brief Register data entry array.
 *  Note: used more specifically in the context of Dual Data Set feature when a single
 *  NVM storage is shared between several applications.
 *
 * \param[in] tb_array   if NULL defaults to gNVM_TABLE_startAddr_c value otherwise
 *                       load tb_array as base of table
 * \param[in] nb_entries if 0 defaults to gNVM_TABLE_entries_c otherwise use nb_entries
 *                       form tb_array.
 ********************************************************************************* */
void NvSetNvmDataTable(NVM_DataEntry_t *tb_array, uint16_t nb_entries);

/*
 * Functions below are required in some NVM tests scenarii for setup or debug reasons.
 * They should not be considered as public APIs to be used outside this context.
 */
/*! *********************************************************************************
 *  \brief Dump NV storage flash contents.
 *  Note: Requires gNvDebugEnabled_d to be defined. Requires PRINTF.
 *
 * \param[in] active_only dump active page only otherwise both pages are dumped.
 ********************************************************************************* */
void NV_ShowFlashTable(bool_t active_only);

/*! *********************************************************************************
 *  \brief Dump NVM meta data.
 *  Note: Requires gNvDebugEnabled_d to be defined. Requires PRINTF.
 *
 ********************************************************************************* */
void NV_ShowMetas(void);

/*! *********************************************************************************
 *  \brief Dump NVM RAM table.
 *  Note: Requires gNvDebugEnabled_d to be defined. Requires PRINTF.
 *
 * \param[in] end_id ID of last record to be dumped.
 ********************************************************************************* */
void NV_ShowRamTable(uint16_t end_id);

void NV_ShowDataEntry(uint8_t *ptr, uint16_t data_size);

/*! *********************************************************************************
 *  \brief Getter to retrieve first meta offset. Used in tests or debug only.
 *
 ********************************************************************************* */
uint16_t Nv_GetFirstMetaOffset(void);

/*! *********************************************************************************
 *  \brief Getter to retrieve NVM table size. Used in tests or debug.
 *  Note: Requires gNvUseExtendedFeatureSet_d to be set.
 *
 * \return: NV Table footprint in NVM. Returns 0 if gNvUseExtendedFeatureSet_d is unset.
 ********************************************************************************* */
uint32_t NvGetTableSizeInFlash(void);

/*! *********************************************************************************
 *  \brief Set flash table version. Used in tests or debug.
 * Note: Requires gNvUseExtendedFeatureSet_d to be defined to perform the action,
 * otherwise does nothing
 *
 * \param[in] version version number to set in flash table.
 ********************************************************************************* */
void NvSetFlashTableVersion(uint16_t version);

/*! *********************************************************************************
 *  \brief Register callback for NVM to notify application when ECC fault is detected.
 *
 * \param[in] version version number to set in flash table.
 *
 * \return: gNVM_Ok_c if gNvSalvageFromEccFault_d is set, gNVM_Error_c otherwise.

 ********************************************************************************* */
int NvRegisterEccFaultNotificationCb(NVM_EccFaultNotifyCb_t cb);

/*! *********************************************************************************
 *  \brief Get address of last MIT meta address. Used in tests or debug only.
 *
 ********************************************************************************* */
uint32_t Nv_GetLastMetaAddress(void);

/*! *********************************************************************************
 *  \brief Lock NVM Mutex from outside NVM module. Testing purposes only.
 *
 ********************************************************************************* */
void NV_MutexLock(void);

/*! *********************************************************************************
 *  \brief Unlock NVM Mutex from outside NVM module. Testing purposes only.
 *
 ********************************************************************************* */
void NV_MutexUnlock(void);

/*!
 * @}  end of NVM addtogroup
 */

#ifdef __cplusplus
}
#endif

#endif /* _NVM_INTERFACE_H */