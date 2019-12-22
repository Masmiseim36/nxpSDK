/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __OTA_BOOTLOADER_SUPP_H__
#define __OTA_BOOTLOADER_SUPP_H__

#include <stdint.h>
#include "fsl_common.h"

#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

#define BOOTLOADER_META_TAG FOUR_CHAR_CODE('B', 'L', 'M', 'T')
#define SWAP_META_TAG FOUR_CHAR_CODE('S', 'W', 'A', 'P')
#define IMG_HDR_TAG FOUR_CHAR_CODE('I', 'H', 'D', 'R')

#define RELIABLE_UPDATE_INTERFACE_MAGIC (0x900dbeef)

#define IMG_TYPE_XIP (1u << 0)
#define IMG_TYPE_NONXIP (1u << 1)

#define IMG_CHK_ALG_CRC32 (1)
#define IMG_CHK_ALG_SHA256 (2)
#define IMG_CHK_ALG_HAB (3)
#define INVALID_VERSION (0xffffffffu)

#define BL_IMG_HEADER_SIZE 0x400
#define BL_RELIABLE_UPDATE_IF_ADDR 0x6000201c

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

typedef struct
{
    status_t (*erase)(uint32_t start, uint32_t lengthInbytes);
    status_t (*program)(uint32_t start, uint8_t *src, uint32_t length);
} flash_driver_interface_t;

typedef struct
{
    uint32_t version;
    status_t (*get_update_partition_info)(partition_t *partition);
    status_t (*update_image_state)(uint32_t state);
    status_t (*get_image_state)(uint32_t *state);
    status_t (*update_image_state_user_api)(uint32_t state, flash_driver_interface_t *flashIf);
    status_t (*update_partition_table_user_api)(partition_t *partition,
                                                uint32_t entries,
                                                flash_driver_interface_t *flashIf);
    uint32_t magic_num;
} reliable_update_interface_t;

//! @brief State information for the CRC32 algorithm.
typedef struct Crc32Data
{
    uint32_t currentCrc;   //!< Current CRC value.
    uint32_t byteCountCrc; //!< Number of bytes processed.
} crc32_data_t;

extern uint32_t bl_crc32(const uint8_t *data, uint32_t size);
extern int32_t bl_verify_image(const uint8_t *data, uint32_t size);

extern uint32_t bl_api_version(void);
extern void bl_set_flash_driver_if(flash_driver_interface_t *drvif);

extern status_t bl_get_update_partition_info(partition_t *partition);
extern status_t bl_update_image_state(uint32_t state);
extern status_t bl_get_image_state(uint32_t *state);
extern status_t bl_update_partition_table(partition_t *partition, uint32_t entries);

#endif /* __OTA_BOOTLOADER_SUPP_H__ */
