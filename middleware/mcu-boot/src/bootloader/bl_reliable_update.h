/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __BL_RELIABLE_UPDATE_H__
#define __BL_RELIABLE_UPDATE_H__

#include <stdbool.h>
#include <stdint.h>
#include "property/property.h"

//! @addtogroup reliable_update
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

#define BOOTLOADER_META_TAG FOUR_CHAR_CODE('B', 'L', 'M', 'T')
#define SWAP_META_TAG FOUR_CHAR_CODE('S', 'W', 'A', 'P')
#define IMG_HDR_TAG FOUR_CHAR_CODE('I', 'H', 'D', 'R')

#define BL_IS_HARDWARE_SWAP_ENABLED (BL_FEATURE_HARDWARE_SWAP_UPDATE)

typedef enum _reliable_update_option
{
    kReliableUpdateOption_Normal = 0x0U, //!< Normal option, Update main appliction only if it is invalid
    kReliableUpdateOption_Swap = 0x1U,   //!< Swap option, Always update main application
} reliable_update_option_t;

typedef enum _specified_application_type
{
    kSpecifiedApplicationType_Main = 0x0U,
    kSpecifiedApplicationType_Backup = 0x1U,
} specified_application_type_t;

//! @brief Application crc check status codes.
enum _reliable_update_status
{
    kStatus_ReliableUpdateSuccess = MAKE_STATUS(kStatusGroup_ReliableUpdate, 0), //!< Reliable Update succeeded.
    kStatus_ReliableUpdateFail = MAKE_STATUS(kStatusGroup_ReliableUpdate, 1),    //!< Reliable Update failed.
    kStatus_ReliableUpdateInactive =
        MAKE_STATUS(kStatusGroup_ReliableUpdate, 2), //!< Reliable Update Feature is inactive.
    kStatus_ReliableUpdateBackupApplicationInvalid =
        MAKE_STATUS(kStatusGroup_ReliableUpdate, 3), //!< Backup Application is invalid
    kStatus_ReliableUpdateStillInMainApplication =
        MAKE_STATUS(kStatusGroup_ReliableUpdate, 4), //!< Next boot will be still in Main Application
    kStatus_ReliableUpdateSwapSystemNotReady =
        MAKE_STATUS(kStatusGroup_ReliableUpdate, 5), //!< Cannot swap flash by default because swap system is not ready
    kStatus_ReliableUpdateBackupBootloaderNotReady =
        MAKE_STATUS(kStatusGroup_ReliableUpdate, 6), //!< Cannot swap flash because there is no valid backup bootloader
    kStatus_ReliableUpdateSwapIndicatorAddressInvalid =
        MAKE_STATUS(kStatusGroup_ReliableUpdate, 7), //!< Cannot swap flash because provided swap indicator is invalid
    kStatus_ReliableUpdateSwapSystemNotAvailable = MAKE_STATUS(kStatusGroup_ReliableUpdate, 8),
    kStatus_ReliableUpdateSwapTest =
        MAKE_STATUS(kStatusGroup_ReliableUpdate, 9), //!< Reliable update is under test mode

};

#define USER_DEFINED_PART_NUM 4
//!@brief Flash partition index
enum
{
    kPartition_Bootloader,
    kPartition_Primary,
    kPartition_Secondary,
    kPartition_UserData,
    kPartition_Max = kPartition_UserData + USER_DEFINED_PART_NUM,
};

enum
{
    kImageIndex_Primary,
    kImageIndex_Secondary,
    kImageIndex_Max
};

enum
{
    kSwapType_None,         // Default value when there is no upgradable image
    kSwapType_ReadyForTest, // The application needs to program this when finishing the update operation
    kSwapType_Test,         // The bootloader needs to set the swap_info to this value before running the test image
    kSwapType_Permanent,    // The application needs to set the swap_info to this type when the self-test is okay
    kSwapType_Fail,
    kSwapType_Fatal,
    kSwapType_Max,
};

typedef struct
{
    uint32_t start;
    uint32_t size;
    uint32_t image_state; // Active/ReadyForTest/UnderTest
    uint32_t attribute;
    uint32_t reserved[12]; // Reserved for future use
} partition_t;

//!@brief Bootloader meta data structure
typedef struct
{
    struct
    {
        uint32_t wdTimeout;
        uint32_t periphDetectTimeout;
        uint32_t enabledPeripherals;
        uint32_t reserved[12];
    } features;

    partition_t partition[kPartition_Max];
    uint32_t meta_version;
    uint32_t patition_entries;
    uint32_t reserved0;
    uint32_t tag;
} bootloader_meta_t;

// Image header is platform specific
typedef struct
{
    uint32_t tag;
    uint32_t load_addr;
    uint32_t image_type;
    uint32_t image_size;
    uint32_t algorithm;
    // Note: considering the address alignment requirement on the vector table, take RT1060 for example, the vector
    // table must be 1KB aligned
    //      the header_size can be set to 1KB in this case, and the actual image will follow the header
    uint32_t header_size;
    uint32_t image_version;
    uint32_t reserved[1];
    uint32_t checksum[32]; // Can be CRC checksum
    uint32_t padding[];
} boot_image_header_t;

typedef boot_image_header_t image_header_t;

enum
{
    kCopyStatus_Inactive = 0,
    kCopyStatus_InProgress = 0x66,
    kCopyStatus_Done = 0xf0,
    kCopyStatus_Fail = 0x33,
};

enum
{
    kSwapStage_NotStarted,
    kSwapStage_DiffCopy,
    kSwapStage_A_to_B_Scratch,
    kSwapStage_B_to_A,
    kSwapStage_Done,
    kSwapStage_Max,
};

enum
{
    kImageConfirm_Okay = 0xf0,
    kImageConfirm_Invalid = 0x33,
};

typedef struct
{
    uint32_t swap_offset;
    uint32_t scratch_size;
    uint32_t swap_status; // 1 : A -> B scratch,  2 : B -> A
    uint32_t remaining_size;
} swap_progress_t;

typedef struct
{
    uint32_t size;
    uint32_t active_flag;
} image_info_t;

//!@brief Swap meta information, used for the swapping operation
typedef struct
{
    image_info_t image_info[2];
#if !defined(BL_FEATURE_HARDWARE_SWAP_SUPPORTED) || (BL_FEATURE_HARDWARE_SWAP_SUPPORTED == 0)
    swap_progress_t swap_progress;
#endif
    uint32_t swap_type;
    uint32_t copy_status;
    uint32_t confirm_info;
    uint32_t meta_version;
    uint32_t reserved[7];
    uint32_t tag;
} swap_meta_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

#if __cplusplus
extern "C"
{
#endif

    status_t load_swap_meta(swap_meta_t *swap_meta);
    status_t load_boot_meta(bootloader_meta_t *boot_meta);
    status_t swap_meta_check(const swap_meta_t *meta);
    status_t boot_meta_check(const bootloader_meta_t *boot_meta);
    status_t get_image_header(uint32_t partition_id, image_header_t *img_hdr);
    status_t boot_image_check(image_header_t *hdr, int32_t partition_id);

    status_t get_update_partition_info(partition_t *partition);
    status_t update_image_state(uint32_t state);

    void bootloader_reliable_update_as_requested(reliable_update_option_t option, uint32_t address);
    //@}

#if __cplusplus
}
#endif

//! @}

#endif // __BL_RELIABLE_UPDATE_H__
