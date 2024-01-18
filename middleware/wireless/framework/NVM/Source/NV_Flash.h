/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * Private header file of the non-volatile storage module for the CORTEX-M4 processor
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef _NV_FLASH_H_
#define _NV_FLASH_H_

#include "EmbeddedTypes.h"
#include "NVM_Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 ******************************************************************************
 * Public macros
 ******************************************************************************
 ******************************************************************************/

/*
 * Name: gNvInvalidTableEntryIndex_c
 * Description: self explanatory
 */
#define gNvInvalidTableEntryIndex_c 0xFFFFU

/*
 * Name: gNvInvalidElementIndex_c
 * Description: self explanatory
 */
#define gNvInvalidElementIndex_c 0xFFFF

/*
 * Name: gValidationByteSingleRecord_c
 * Description: the value of validation byte used in meta tag to mark a single record type
 */
#define gValidationByteSingleRecord_c 0xAAU

/*
 * Name: gValidationByteAllRecords_c
 * Description: the value of validation byte used in meta tag to mark an entire table entry type
 */
#define gValidationByteAllRecords_c 0x55U

/*
 * Name: gPageCounterMaxValue_c
 * Description: self explanatory
 */
#define gPageCounterMaxValue_c 0xFFFFFFFFUL

/*
 * Name: gEmptyPageMetaAddress_c
 * Description: the value of a page that has no records yet
 */
#define gEmptyPageMetaAddress_c 0x3FFF0000UL

/*
 * Name: gFifoOverwriteEnabled_c
 * Description: if set to TRUE, it enables the pending saves queue to be overwritten
 */
#define gFifoOverwriteEnabled_c 0

/*
 * Name: gNvCopyAll_c
 * Description: definition used when a full page copy is requested
 */
#define gNvCopyAll_c 0xFFFFU

/*
 * Name: gNvFlexFormatBufferSize_c
 * Description: the size of the buffer used for FlexNVM formating. The FlexRAM
 *              is "formatted" in blocks of gNvFlexFormatBufferSize_c bytes
 */
#define gNvFlexFormatBufferSize_c 32

/*
 * Name: gNvFlexGuardValue_c
 * Description: value of the guard space between meta info and records
 *              (four 0xFF bytes)
 */
#define gNvFlexGuardValue_c 0xFFFFFFFFUL

/*
 * Name: gNvJitterDecrement_c
 * Description: the value of the jitter (in microseconds) to be substracted from the random generated one
 */
#define gNvJitterDecrement_c 50000UL

/*
 * Name: gNvJitterMultiplier_c
 * Description: the value of the jitter magnitude multiplier (1ms = 1000 us)
 */
#define gNvJitterMultiplier_c 1000UL

/*
 * Name: gNvOneSecondInMicros_c
 * Description: the value of one second expressed in microseconds
 */
#define gNvOneSecondInMicros_c 1000000UL

/*
 * Name: PGM_SIZE_BYTE
 * Description: the value of min flash write operation unit
 */

#if (defined(FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE) && (FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE > 0))
#define PGM_SIZE_BYTE FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE
#elif (defined(FSL_FEATURE_FLASH_BLOCK_PHRASE_SIZE) && (FSL_FEATURE_FLASH_BLOCK_PHRASE_SIZE > 0))
#define PGM_SIZE_BYTE FSL_FEATURE_FLASH_BLOCK_PHRASE_SIZE
#else
#define PGM_SIZE_BYTE 16U
#endif

/*****************************************************************************
 ******************************************************************************
 * Public type declarations
 ******************************************************************************
 ******************************************************************************/

/*
 * Name: NVM_RecordMetaInfo_t
 * Description: NVM record meta information type definition
 */
#pragma pack(1)
typedef union NVM_RecordMetaInfo_tag
{
    uint64_t rawValue;
    struct
    {
        uint8_t  NvValidationStartByte;
        uint16_t NvmDataEntryID;
        uint16_t NvmElementIndex;
        uint16_t NvmRecordOffset;
        uint8_t  NvValidationEndByte;
        uint8_t  Padding[PGM_SIZE_BYTE - sizeof(uint64_t)];
    } fields;
} NVM_RecordMetaInfo_t;
#pragma pack()

/*
 * Name: NVM_TableInfo_t
 * Description: NVM flash table info type definition
 */
#pragma pack(1)
typedef union NVM_TableInfo_tag
{
    uint64_t rawValue;
    struct
    {
#if gNvUseExtendedFeatureSet_d
        uint32_t NvPageCounter;
        uint16_t NvTableMarker;
        uint16_t NvTableVersion;
#else
        uint64_t NvPageCounter;
#endif
        uint8_t Padding[PGM_SIZE_BYTE - sizeof(uint64_t)];
    } fields;
} NVM_TableInfo_t;
#pragma pack()

/*
 * Name: NVM_EntryInfo_t
 * Description: NVM flash table entry type definition
 */
#pragma pack(1)
typedef union NVM_EntryInfo_tag
{
    uint64_t rawValue;
    struct
    {
        uint16_t NvDataEntryID;
        uint16_t NvDataEntryType;
        uint16_t NvElementsCount;
        uint16_t NvElementSize;
        uint8_t  Padding[PGM_SIZE_BYTE - sizeof(uint64_t)];
    } fields;
} NVM_EntryInfo_t;
#pragma pack()

/*
 * Name: NVM_VirtualPageID_t
 * Description: virtual page ID type definition
 */
typedef enum NVM_VirtualPageID_tag
{
    gFirstVirtualPage_c = 0,
    gSecondVirtualPage_c,
    gVirtualPageNone_c,
} NVM_VirtualPageID_t;

#define gVirtualPageNb_c (uint8_t) gVirtualPageNone_c
/*
 * Name: NVM_VirtualPageProperties_t
 * Description: virtual page properties type definition
 */
typedef struct NVM_VirtualPageProperties_tag
{
    uint32_t NvRawSectorStartAddress; /*< Virtual page start address in flash */
    uint32_t NvRawSectorEndAddress;   /*< Virtual page end address in flash */
    uint8_t  NvRawSectorsCount;       /*< Number of flash sectors contituting one virtual page  */
    uint32_t NvTotalPageSize;         /*< Virtual page size in bytes - number of flash sector times sector size */
    uint32_t NvLastMetaInfoAddress;   /*< Most recent record meta information written to flash */
#if gUnmirroredFeatureSet_d
    uint32_t NvLastMetaUnerasedInfoAddress; /*< Frontier above which page is still in blank state */
#endif
    bool_t has_ecc_faults;  /*< ECC fault were discovered in page at initialization :
                             *  can be true only when gNvSalvageFromEccFault_d is defined */
    uint32_t CounterTop;    /*< Virtual Page version number read at bottom of page */
    uint32_t CounterBottom; /*< Virtual Page version number read at top of page */
} NVM_VirtualPageProperties_t;

typedef struct NVM_ErasePageCmdStatus_tag
{
    bool_t              NvErasePending;
    NVM_VirtualPageID_t NvPageToErase;
    uint32_t            NvSectorAddress;
} NVM_ErasePageCmdStatus_t;

typedef enum
{
    OP_NONE,
    OP_SAVE_ALL,
    OP_SAVE_SINGLE,
    OP_ERASE_NEXT_PART,
} eNvFlashOp_t;

/*
 * Name: NVM_TableEntryInfo_t
 * Description: table entry indexes type definition
 */
typedef struct NVM_TableEntryInfo_tag
{
    NvTableEntryId_t entryId;
    uint16_t         elementIndex;
    eNvFlashOp_t     op_type;
} NVM_TableEntryInfo_t;

/*
 * Name: NVM_SaveQueue_t
 * Description: Circular queue used for pending saves data type definition
 */
typedef struct NVM_SaveQueue_tag
{
    NVM_TableEntryInfo_t QData[gNvPendingSavesQueueSize_c]; /* queue data */
    uint16_t             Head;                              /* read index */
    uint16_t             Tail;                              /* write index */
    uint16_t             EntriesCount;                      /* entries count */
} NVM_SaveQueue_t;

/*****************************************************************************
 ******************************************************************************
 * Public memory declarations
 ******************************************************************************
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _NV_FLASH_H_ */
