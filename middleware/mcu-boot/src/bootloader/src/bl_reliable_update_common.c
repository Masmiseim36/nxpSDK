/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "bootloader/bl_context.h"
#include "bootloader/bl_reliable_update.h"
#include "bootloader/bootloader.h"
#include "bootloader_common.h"
#include "crc/crc32.h"
#include "memory/memory.h"
#include "property/property.h"
#include "utilities/fsl_assert.h"

#define BL_FEATURE_RELIABLE_UPDATE (1)

#if BL_FEATURE_RELIABLE_UPDATE

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/*
        Flash Layout
    ------------------------
   |                        |
   |      Bootloader        |
   |      Partition         |
   |      including         |
   | Bootloader & Swap Meta |
   |------------------------|
   |                        |
   |    IMG0 Partition      |
   |	                    |
   |                        |
   |------------------------|
   |                        |
   |    IMG1 Partition      |
   |                        |
   |      including         |
   |      SWAP Scratch      |
   |                        |
   |------------------------|
   |                        |
   |                        |
   |  User Data Partitions  |
   |                        |
   |                        |
   --------------------------
 */

#define IMG_TYPE_XIP (1u << 0)
#define IMG_TYPE_NONXIP (1u << 1)

#define PARTITION_ATTR_DEFAULT (0)
#define PARTITION_ATTR_LOCK (1)
#define PARTITION_ATTR_ENCRYPT (2)

#define IMG_CHK_ALG_CRC32 (1)
#define IMG_CHK_ALG_SHA256 (2)
#define IMG_CHK_ALG_HAB (3)
#define INVALID_VERSION (0xffffffffu)

#define BREAK_IF(a) \
    if (a)          \
    {               \
        break;      \
    }

typedef struct hab_hdr
{
    uint8_t tag;    /**< Tag field */
    uint8_t len[2]; /**< Length field in bytes (big-endian) */
    uint8_t par;    /**< Parameters field */
} hab_hdr_t;

// Header IVT format
typedef struct
{
    hab_hdr_t hdr;
    uint32_t entry;
    uint32_t reserved1;
    uint32_t dcd;
    uint32_t boot_data_ptr;
    uint32_t self;
    uint32_t csf;
    uint32_t reserved;
    struct
    {
        uint32_t start;
        uint32_t size;
        uint32_t plaugin;
        uint32_t reserved;
    } boot_data;

} image_header_hab_ivt_t;

typedef struct
{
    uint32_t image_index;
    uint32_t image_state;
} running_img_info_t;

typedef struct
{
    bootloader_meta_t boot_meta;
    swap_meta_t swap_meta;
} ota_boot_ctx_t;

typedef void (*application_entry_t)(void);

const bootloader_meta_t k_boot_meta=
{
    .features =
    {
        .wdTimeout = 0xffffffffu,
        .periphDetectTimeout = 5000u,
        .enabledPeripherals = 0xff,

    },
    .partition =
    {
        [kPartition_Bootloader] = {BL_FEATURE_BOOTLOADER_START, BL_FEATURE_BOOTLOADER_SIZE},
        [kPartition_Primary] = {BL_FEATURE_PRIMARY_IMG_START, BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE},
        [kPartition_Secondary] = {BL_FEATURE_SECONDARY_IMG_START, BL_FEATURE_SECOND_IMG_PARTITION_SIZE},
        [kPartition_UserData] = {BL_FEATURE_USER_DATA_PARTITION_START, BL_FEATURE_USER_DATA_PARTITION_SIZE},
    },
    .tag = BOOTLOADER_META_TAG,
    .meta_version = 0,
};

static ota_boot_ctx_t s_boot_ctx;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

status_t load_boot_meta(bootloader_meta_t *boot_meta);
status_t do_swap_if_needed(void);
status_t load_swap_meta(swap_meta_t *swap_meta);
status_t boot_meta_check(const bootloader_meta_t *boot_meta);
status_t swap_meta_check(const swap_meta_t *swap_meta);
status_t update_swap_meta(swap_meta_t *swap_meta);
status_t update_boot_meta(const bootloader_meta_t *boot_meta);
status_t run_swap_routine(void);
status_t flash_copy(uint32_t dst, uint32_t start, uint32_t length);
status_t boot_image_check(boot_image_header_t *hdr, int32_t partition_id);
status_t boot_go(void);
void show_boot_meta_info(const bootloader_meta_t *boot_meta);
void show_swap_meta_info(const swap_meta_t *swap_meta);

//! @brief Update the property of reliable update status
static void update_reliable_update_status(uint32_t status);

#if BL_IS_HARDWARE_SWAP_ENABLED

//! @brief Do hardware reliable application update if backup application is valid
static status_t hardware_reliable_update(reliable_update_option_t option, uint32_t swapIndicatorAddress);

//! @brief Get the swap indicator address from IFR if swap system is in ready state.
static status_t get_swap_indicator_address_if_system_is_in_ready(uint32_t *address);
#else
//! @brief Do software reliable application update if backup application is valid
static status_t software_reliable_update(uint32_t backupApplicationBase);
#endif // BL_IS_HARDWARE_SWAP_ENABLED

//! @brief Copy source application to destination application region and return result
static bool get_result_after_copying_application(uint32_t src, uint32_t dst, uint32_t len);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

status_t get_partition_info(uint32_t partition_id, partition_t *partition)
{
    if ((partition_id >= kPartition_Max) || (partition == NULL))
    {
        return kStatus_Fail;
    }

    memcpy(partition, &k_boot_meta.partition[partition_id], sizeof(*partition));

    return kStatus_Success;
}

// Search the partition that can be updated with new image or
status_t get_update_partition(uint32_t *partition_id)
{
    status_t status = kStatus_Fail;
    *partition_id = kPartition_Secondary;
    status = kStatus_Success;

    return status;
}

status_t get_active_partition(uint32_t *partition_id)
{
    uint32_t id = kPartition_Primary;
    status_t status = get_update_partition(&id);

    if (status != kStatus_Success)
    {
        return status;
    }

    if (id == kPartition_Primary)
    {
        id = kPartition_Secondary;
    }
    else
    {
        id = kPartition_Primary;
    }

    status = kStatus_Success;

    *partition_id = id;

    return status;
}

status_t flash_read(uint32_t address, uint8_t *buffer, size_t length)
{
    if ((buffer == NULL) || (length < 1))
    {
        return kStatus_Fail;
    }

    memcpy(buffer, (void *)address, length);

    return kStatus_Success;
}

status_t swap_meta_check(const swap_meta_t *meta)
{
    status_t status = kStatus_Fail;

    if ((meta == NULL) || (meta->tag != SWAP_META_TAG))
    {
        status = kStatus_Fail;
    }
    else
    {
        if (meta->swap_type > kSwapType_Max)
        {
            status = kStatus_Fail;
        }
        if (meta->meta_version == INVALID_VERSION)
        {
            status = kStatus_Fail;
        }
        else
        {
            status = kStatus_Success;
        }
    }

    return status;
}

status_t boot_meta_check(const bootloader_meta_t *boot_meta)
{
    status_t status = kStatus_Fail;

    if ((boot_meta == NULL) || (boot_meta->tag != BOOTLOADER_META_TAG))
    {
        status = kStatus_Fail;
    }
    else
    {
        if (boot_meta->meta_version == INVALID_VERSION)
        {
            status = kStatus_Fail;
        }
        else
        {
            status = kStatus_Success;
        }
    }

    return status;
}

status_t get_image_header(uint32_t partition_id, image_header_t *img_hdr)
{
    status_t status;
    if (img_hdr == NULL)
    {
        return kStatus_InvalidArgument;
    }

    partition_t partition;
    status = get_partition_info(partition_id, &partition);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = flash_read(partition.start, (uint8_t *)img_hdr, sizeof(*img_hdr));
    if (status != kStatus_Success)
    {
        return status;
    }

    if (img_hdr->tag != IMG_HDR_TAG)
    {
        return kStatus_Fail;
    }

    status = kStatus_Success;

    return status;
}

status_t boot_image_check(image_header_t *hdr, int32_t partition_id)
{
    status_t status = kStatus_Fail;
    if ((hdr == NULL) || (hdr->tag != IMG_HDR_TAG))
    {
        return status;
    }

    partition_t partition;

    status = get_partition_info(partition_id, &partition);
    if (status != kStatus_Success)
    {
        return status;
    }

#if 0 // FIXME later
    if (hdr->algorithm == IMG_CHK_ALG_SHA256)
    {
        // Do something here
    }
    else
#endif
    if (hdr->algorithm == IMG_CHK_ALG_CRC32)
    {
        // Do something here
        uint32_t image_start = partition.start + hdr->header_size;
        uint32_t image_size = hdr->image_size;

        crc32_data_t crc32Config;
        uint32_t calcChecksum;
        crc32_init(&crc32Config);
        crc32_update(&crc32Config, (const uint8_t *)image_start, image_size);
        crc32_finalize(&crc32Config, &calcChecksum);
        if (calcChecksum == hdr->checksum[0])
        {
            // CRC check passed
            // The image is considered as valid image
            status = kStatus_Success;
        }
        else
        {
            status = kStatus_Fail;
        }
    }

    return status;
}

status_t flash_copy(uint32_t dst, uint32_t start, uint32_t length)
{
    status_t status = kStatus_Fail;
    static uint32_t temp_buffer[512 / sizeof(uint32_t)];

    debug_printf("%s: dst=%x, start=%x, length=%x\r\n", __func__, dst, start, length);
    debug_printf("Erasing FLASH from address:0x%x, length=0x%x\r\n", dst, length);
    status = mem_erase(dst, length, 0);
    if (status != kStatus_Success)
    {
        return status;
    }
    uint32_t copy_size;
    while (length)
    {
        copy_size = MIN(length, sizeof(temp_buffer));
        //        debug_printf("Copying %d bytes of data from 0x%x to 0x%x...\r\n", copy_size, start, dst);
        memcpy(temp_buffer, (void *)start, copy_size);
        BREAK_IF(status != kStatus_Success);
        status = mem_write(dst, copy_size, (const uint8_t *)&temp_buffer[0], 0);
        BREAK_IF(status != kStatus_Success);
        dst += copy_size;
        start += copy_size;
        length -= copy_size;
    }
    status = mem_flush();

    return status;
}

status_t load_boot_meta(bootloader_meta_t *boot_meta)
{
    bootloader_meta_t boot_metas[2];

    status_t status = kStatus_Fail;
    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t meta_addr = BL_FEATURE_BOOT_META_START + i * BL_FEATURE_FLASH_SECTOR_SIZE;
        status = flash_read(meta_addr, (uint8_t *)&boot_metas[i], sizeof(bootloader_meta_t));
        BREAK_IF(status != kStatus_Success);
    }

    if (status != kStatus_Success)
    {
        return status;
    }

    uint32_t active_idx = 0;
    if ((kStatus_Success != boot_meta_check(&boot_metas[0])) && (kStatus_Success != boot_meta_check(&boot_metas[1])))
    {
        // No valid boot_meta, use the default meta, and program it to the NVM
        memcpy(&boot_metas[0], &k_boot_meta, sizeof(k_boot_meta));
        mem_erase(BL_FEATURE_BOOT_META_START, BL_FEATURE_FLASH_SECTOR_SIZE, 0);
        mem_write(BL_FEATURE_BOOT_META_START, sizeof(boot_metas[0]), (uint8_t *)&boot_metas[0], 0);
        mem_flush();
    }
    else if ((kStatus_Success == boot_meta_check(&boot_metas[0])) &&
             (kStatus_Success != boot_meta_check(&boot_metas[1])))
    {
        active_idx = 0;
    }
    else if ((kStatus_Success != boot_meta_check(&boot_metas[0])) &&
             (kStatus_Success == boot_meta_check(&boot_metas[1])))
    {
        active_idx = 1;
    }
    else if ((kStatus_Success == boot_meta_check(&boot_metas[0])) && (kStatus_Success == boot_meta_check(&boot_metas[1])))
    {
        active_idx = (boot_metas[0].meta_version > boot_metas[1].meta_version) ? 0 : 1;
    }

    memcpy(boot_meta, &boot_metas[active_idx], sizeof(boot_metas[0]));
    return kStatus_Success;
}

status_t update_boot_meta(const bootloader_meta_t *boot_meta)
{
    bootloader_meta_t boot_metas[2];
    status_t status = kStatus_Fail;

    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t meta_addr = BL_FEATURE_BOOT_META_START + i * BL_FEATURE_FLASH_SECTOR_SIZE;
        flash_read(meta_addr, (uint8_t *)&boot_metas[i], sizeof(bootloader_meta_t));
    }

    uint32_t update_idx = 0;
    if ((kStatus_Success != boot_meta_check(&boot_metas[0])) && (kStatus_Success != boot_meta_check(&boot_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == boot_meta_check(&boot_metas[0])) &&
             (kStatus_Success != boot_meta_check(&boot_metas[1])))
    {
        update_idx = 1;
    }
    else if ((kStatus_Success != boot_meta_check(&boot_metas[0])) &&
             (kStatus_Success == boot_meta_check(&boot_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == boot_meta_check(&boot_meta[0])) && (kStatus_Success == boot_meta_check(&boot_meta[1])))
    {
        update_idx = (boot_metas[0].meta_version > boot_metas[1].meta_version) ? 1 : 0;
    }

    uint32_t meta_addr = BL_FEATURE_BOOT_META_START + update_idx * BL_FEATURE_FLASH_SECTOR_SIZE;

    do
    {
        status = mem_erase(meta_addr, BL_FEATURE_FLASH_SECTOR_SIZE, 0);
        if (status != kStatus_Success)
        {
            break;
        }
        status = mem_write(meta_addr, sizeof(*boot_meta), (uint8_t *)boot_meta, 0);
        if (status != kStatus_Success)
        {
            break;
        }
        status = mem_flush();

    } while (0);

    return status;
}

status_t load_swap_meta(swap_meta_t *swap_meta)
{
    swap_meta_t swap_metas[2];
    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t meta_addr = BL_FEATURE_SWAP_META_START + i * BL_FEATURE_FLASH_SECTOR_SIZE;
        flash_read(meta_addr, (uint8_t *)&swap_metas[i], sizeof(swap_meta_t));
    }

    uint32_t active_idx = 0;
    if ((kStatus_Success != swap_meta_check(&swap_metas[0])) && (kStatus_Success != swap_meta_check(&swap_metas[1])))
    {
        // No valid swap_meta, use the default meta, and program it to the NVM
        memset(&swap_metas[0], 0, sizeof(swap_metas[0]));
        swap_metas[0].meta_version = 0;
        swap_metas[0].tag = SWAP_META_TAG;
        mem_erase(BL_FEATURE_SWAP_META_START, BL_FEATURE_FLASH_SECTOR_SIZE, 0);
        mem_write(BL_FEATURE_SWAP_META_START, sizeof(swap_metas[0]), (uint8_t *)&swap_metas[0], 0);
        mem_flush();
    }
    else if ((kStatus_Success == swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success != swap_meta_check(&swap_metas[1])))
    {
        active_idx = 0;
    }
    else if ((kStatus_Success != swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success == swap_meta_check(&swap_metas[1])))
    {
        active_idx = 1;
    }
    else if ((kStatus_Success == swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success == swap_meta_check(&swap_metas[1])))
    {
        active_idx = (swap_metas[0].meta_version > swap_metas[1].meta_version) ? 0 : 1;
    }

    memcpy(swap_meta, &swap_metas[active_idx], sizeof(swap_metas[0]));

    return kStatus_Success;
}

status_t update_swap_meta(swap_meta_t *swap_meta)
{
    status_t status = kStatus_Fail;
    swap_meta_t swap_metas[2];
    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t meta_addr = BL_FEATURE_SWAP_META_START + i * BL_FEATURE_FLASH_SECTOR_SIZE;
        flash_read(meta_addr, (uint8_t *)&swap_metas[i], sizeof(swap_meta_t));
    }

    uint32_t update_idx = 0;
    if ((kStatus_Success != swap_meta_check(&swap_metas[0])) && (kStatus_Success != swap_meta_check(&swap_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success != swap_meta_check(&swap_metas[1])))
    {
        update_idx = 1;
    }
    else if ((kStatus_Success != swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success == swap_meta_check(&swap_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success == swap_meta_check(&swap_metas[1])))
    {
        update_idx = (swap_metas[0].meta_version > swap_metas[1].meta_version) ? 1 : 0;
    }

    uint32_t meta_addr = BL_FEATURE_SWAP_META_START + update_idx * BL_FEATURE_FLASH_SECTOR_SIZE;

    swap_meta->meta_version++;
    do
    {
        status = mem_erase(meta_addr, BL_FEATURE_FLASH_SECTOR_SIZE, 0);
        if (status != kStatus_Success)
        {
            break;
        }
        status = mem_write(meta_addr, sizeof(*swap_meta), (uint8_t *)swap_meta, 0);
        if (status != kStatus_Success)
        {
            break;
        }
        status = mem_flush();

    } while (0);

    show_swap_meta_info(swap_meta);

    return status;
}

status_t boot_go(void)
{
    status_t status = kStatus_Fail;
    image_header_t boot_header;
    get_image_header(kPartition_Primary, &boot_header);
    if (boot_image_check(&boot_header, kPartition_Primary) == kStatus_Success)
    {
        if (s_boot_ctx.swap_meta.swap_type == kSwapType_None)
        {
            s_boot_ctx.swap_meta.image_info[0].size = boot_header.image_size + boot_header.header_size;
            update_swap_meta(&s_boot_ctx.swap_meta);
        }

        uint32_t image_start = boot_header.load_addr;

        if (image_start == 0xffffffffu)
        {
            return kStatus_Fail;
        }
        else
        {
            g_bootloaderContext.imageStart = image_start;
            status = kStatus_Success;
        }
    }

    return status;
}

void show_boot_meta_info(const bootloader_meta_t *boot_meta)
{
    debug_printf("Boot Meta summary:\r\n---------------------------\r\n");
    debug_printf("Boot Partition: start=%x, size=%x\r\n", boot_meta->partition[kPartition_Bootloader].start,
                 boot_meta->partition[kPartition_Bootloader].size);
    debug_printf("Primary Partition: start=%x, size=%x\r\n", boot_meta->partition[kPartition_Primary].start,
                 boot_meta->partition[kPartition_Primary].size);
    debug_printf("Secondary Partition: start=%x, size=%x\r\n", boot_meta->partition[kPartition_Secondary].start,
                 boot_meta->partition[kPartition_Secondary].size);
}

void show_swap_meta_info(const swap_meta_t *swap_meta)
{
    const char *swap_type_str[kSwapType_Max] = { "kSwapType_None",      "kSwapType_ReadyForTest", "kSwapType_Test",
                                                 "kSwapType_Permenant", "kSwapType_Fail",         "kSwapType_Fatal" };

    const char *swap_stage_str[] = {
        "kSwapStage_NotStarted", "kSwapStage_DiffCopy", "kSwapStage_A_to_B_Scratch",
        "kSwapStage_B_to_A",     "kSwapStage_Done",
    };

    debug_printf("Swap Meta summary:\r\n---------------------------\r\n");
    if ((swap_meta->swap_type == kSwapType_ReadyForTest) || (swap_meta->swap_type == kSwapType_Test))
    {
        debug_printf("Swap is in progress...\r\n");
    }
    debug_printf("swap_type:%s\r\n", swap_type_str[swap_meta->swap_type]);
    debug_printf("swap_progress: offset=%x, scratch_size=%x, stage=%s, remaining_size=%x\r\n",
                 swap_meta->swap_progress.swap_offset, swap_meta->swap_progress.scratch_size,
                 swap_stage_str[swap_meta->swap_progress.swap_status], swap_meta->swap_progress.remaining_size);
    debug_printf("Image Info:image[0].size=0x%x, image[1].size=0x%x\r\n", swap_meta->image_info[0].size,
                 swap_meta->image_info[1].size);
}

void bootloader_reliable_update_as_requested(reliable_update_option_t option, uint32_t address)
{
    debug_printf("Running %s...\r\n", __func__);
    load_boot_meta(&s_boot_ctx.boot_meta);
    show_boot_meta_info(&s_boot_ctx.boot_meta);
    load_swap_meta(&s_boot_ctx.swap_meta);
    show_swap_meta_info(&s_boot_ctx.swap_meta);
    status_t status = kStatus_Fail;
    if (option == kReliableUpdateOption_Normal)
    {
        /*
        OTA bootloader
        • Load the bootloader meta
        ○ Set the first meta to default value if there is no meta data present
        • Load the swap meta
        ○ 1. Meta is invalid (tag not match) - Only check the primary boot image
        • Meta indicates the swapType == readyForTest
        ○ 2.1 Meta indicates the swap has not started
        ○ 2.2 Meta indicates the swap is in progress
        ○ 2.3 Meta indicates the swap has done
        • Meta indicates the swapType == Test
        ○ 3.1 Meta indicates the revert has not started
        ○ 3.2 Meta indicates the revert is in progress
        ○ 3.3 Meta indicates the revert has done
        •  Meta indicates the swapType == Permanent
        ○ 4.1 Boot the primary image
        */

        // Set the imageStart to invalid value first because the OTA bootloader will refresh the start address later
        g_bootloaderContext.imageStart = 0xffffffffu;
        do
        {
            status = do_swap_if_needed();
            BREAK_IF(status != kStatus_Success);
            // Try to boot
            status = boot_go();
            BREAK_IF(status != kStatus_Success);
        } while (0);

        switch (s_boot_ctx.swap_meta.swap_type)
        {
            case kSwapType_None:
                update_reliable_update_status(kStatus_ReliableUpdateInactive);
                break;
            case kSwapType_Test:
                update_reliable_update_status(kStatus_ReliableUpdateSwapTest);
                break;
            case kSwapType_Fail:
                update_reliable_update_status(kStatus_ReliableUpdateFail);
                break;
            case kSwapType_Fatal:
                update_reliable_update_status(kStatus_ReliableUpdateFail);
                break;
            case kSwapType_Permanent:
                update_reliable_update_status(kStatus_ReliableUpdateSuccess);
                break;
            default:
                update_reliable_update_status(kStatus_ReliableUpdateFail);
                break;
        }
    }
    else if (option == kReliableUpdateOption_Swap)
    {
        image_header_t boot_header;
        get_image_header(kPartition_Secondary, &boot_header);

        if (s_boot_ctx.swap_meta.swap_type == kSwapType_Test)
        {
            if (boot_image_check(&boot_header, kPartition_Primary) == kStatus_Success)
            {
                s_boot_ctx.swap_meta.confirm_info = kImageConfirm_Okay;
                update_swap_meta(&s_boot_ctx.swap_meta);
                update_reliable_update_status(kStatus_ReliableUpdateSuccess);
            }
            else
            {
                update_reliable_update_status(kStatus_ReliableUpdateFail);
            }
        }
        else if (boot_image_check(&boot_header, kPartition_Secondary) == kStatus_Success)
        {
            s_boot_ctx.swap_meta.swap_type = kSwapType_ReadyForTest;
            s_boot_ctx.swap_meta.copy_status = 0;
            s_boot_ctx.swap_meta.swap_progress.swap_offset = 0;
            s_boot_ctx.swap_meta.swap_progress.swap_status = kSwapStage_NotStarted;
            s_boot_ctx.swap_meta.image_info[1].size = boot_header.image_size + boot_header.header_size;
            s_boot_ctx.swap_meta.confirm_info = 0;
            update_swap_meta(&s_boot_ctx.swap_meta);
            update_reliable_update_status(kStatus_ReliableUpdateSwapTest);
        }
        else
        {
            update_reliable_update_status(kStatus_ReliableUpdateFail);
        }
    }
}

status_t run_swap_routine(void)
{
    status_t status = kStatus_Fail;
    swap_meta_t *p_swap_meta = &s_boot_ctx.swap_meta;
    uint32_t aligned_image_size[2];
    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t temp = s_boot_ctx.swap_meta.image_info[i].size;
        aligned_image_size[i] = ALIGN_UP(temp, BL_FEATURE_FLASH_SECTOR_SIZE);
    }
    uint32_t max_image_size = MAX(aligned_image_size[0], aligned_image_size[1]);
    uint32_t min_image_size = MIN(aligned_image_size[0], aligned_image_size[1]);
    uint32_t scratch_size =
        MIN(s_boot_ctx.boot_meta.partition[kPartition_Secondary].size - max_image_size, max_image_size);
    uint32_t swap_size = max_image_size;
    swap_progress_t *p_progress = &p_swap_meta->swap_progress;

    // Swap the primary image with the secondary image
    if (p_swap_meta->swap_type == kSwapType_None)
    {
        // Do nothing, return Success
        status = kStatus_Success;
    }
    // When kSwapType_ReadyForTest is detected, it means bootloader needs to swap primary image with the secondary image
    // Or in the middle of this operation
    else if (p_swap_meta->swap_type == kSwapType_ReadyForTest)
    {
        uint32_t swap_scratch_start = s_boot_ctx.boot_meta.partition[kPartition_Secondary].start +
                                      s_boot_ctx.boot_meta.partition[kPartition_Secondary].size - scratch_size;
        uint32_t primary_img_copy_start = s_boot_ctx.boot_meta.partition[kPartition_Primary].start;
        uint32_t secondary_img_copy_start = s_boot_ctx.boot_meta.partition[kPartition_Secondary].start;

        // Read The swap context
        if (p_swap_meta->copy_status == kCopyStatus_Inactive)
        {
            p_swap_meta->copy_status = kCopyStatus_InProgress;
            p_progress->swap_status = kSwapStage_A_to_B_Scratch;
            p_progress->swap_offset = max_image_size - MIN(max_image_size, scratch_size);
            p_progress->scratch_size = scratch_size;
            p_progress->remaining_size = max_image_size;
            update_swap_meta(&s_boot_ctx.swap_meta);
        }

        if (p_swap_meta->copy_status == kCopyStatus_InProgress)
        {
            while (p_progress->remaining_size > 0)
            {
                primary_img_copy_start =
                    s_boot_ctx.boot_meta.partition[kPartition_Primary].start + p_progress->swap_offset;
                secondary_img_copy_start =
                    s_boot_ctx.boot_meta.partition[kPartition_Secondary].start + p_progress->swap_offset;
                uint32_t current_swap_size = p_progress->scratch_size;
                if (p_progress->swap_status == kSwapStage_A_to_B_Scratch)
                {
                    // Start swap progress
                    flash_copy(swap_scratch_start, primary_img_copy_start, current_swap_size);
                    p_progress->swap_status = kSwapStage_B_to_A;
                    update_swap_meta(&s_boot_ctx.swap_meta);
                }
                if (p_progress->swap_status == kSwapStage_B_to_A)
                {
                    flash_copy(primary_img_copy_start, secondary_img_copy_start, current_swap_size);
                }
                p_progress->remaining_size -= current_swap_size;
                current_swap_size = MIN((uint32_t)p_progress->remaining_size, scratch_size);

                swap_scratch_start -= current_swap_size;
                p_progress->swap_offset = p_progress->remaining_size - current_swap_size;
                p_progress->scratch_size = current_swap_size;
                p_progress->swap_status = kSwapStage_A_to_B_Scratch;
                update_swap_meta(&s_boot_ctx.swap_meta);
            }
        }
        p_progress->swap_status = kSwapStage_Done;
        update_swap_meta(&s_boot_ctx.swap_meta);

        if ((p_progress->swap_status == kSwapStage_Done) && (p_swap_meta->copy_status != kCopyStatus_Done))
        {
            // Update the image info
            if (p_swap_meta->swap_type == kSwapType_ReadyForTest)
            {
                p_swap_meta->swap_type = kSwapType_Test;
            }
            else if (p_swap_meta->swap_type == kSwapType_Test)
            {
                // Revert
                p_swap_meta->swap_type = kSwapType_None;
            }

            // Image info switch
            image_info_t temp;
            memcpy(&temp, &p_swap_meta->image_info[0], sizeof(temp));
            memcpy(&p_swap_meta->image_info[0], &p_swap_meta->image_info[1], sizeof(temp));
            memcpy(&p_swap_meta->image_info[1], &temp, sizeof(temp));
            p_swap_meta->copy_status = kCopyStatus_Done;
            update_swap_meta(p_swap_meta);
        }

        status = kStatus_Success;
    }
    // Swap the former primary image back
    else if (p_swap_meta->swap_type == kSwapType_Test)
    {
        uint32_t swap_scratch_start = s_boot_ctx.boot_meta.partition[kPartition_Secondary].start;
        uint32_t primary_img_copy_start = s_boot_ctx.boot_meta.partition[kPartition_Primary].start;
        uint32_t secondary_img_copy_start = s_boot_ctx.boot_meta.partition[kPartition_Secondary].start;

        // Read The swap context
        if (p_progress->swap_status == kSwapStage_Done)
        {
            p_progress->swap_status = kSwapStage_A_to_B_Scratch;
            p_swap_meta->copy_status = kCopyStatus_InProgress;
            p_progress->swap_offset = 0;
            p_progress->scratch_size = MIN(max_image_size, scratch_size);
            p_progress->remaining_size = max_image_size;
            update_swap_meta(&s_boot_ctx.swap_meta);
        }

        if ((p_progress->swap_status == kSwapStage_A_to_B_Scratch) || (p_progress->swap_status == kSwapStage_B_to_A))
        {
            primary_img_copy_start = s_boot_ctx.boot_meta.partition[kPartition_Primary].start + p_progress->swap_offset;
            secondary_img_copy_start = s_boot_ctx.boot_meta.partition[kPartition_Secondary].start +
                                       s_boot_ctx.boot_meta.partition[kPartition_Secondary].size - max_image_size +
                                       p_progress->swap_offset;

            while (p_progress->remaining_size > 0)
            {
                uint32_t current_swap_size = p_progress->scratch_size;
                if (p_progress->swap_status == kSwapStage_A_to_B_Scratch)
                {
                    flash_copy(swap_scratch_start, primary_img_copy_start, current_swap_size);
                    p_progress->swap_status = kSwapStage_B_to_A;
                    update_swap_meta(&s_boot_ctx.swap_meta);
                }
                if (p_progress->swap_status == kSwapStage_B_to_A)
                {
                    flash_copy(primary_img_copy_start, secondary_img_copy_start, current_swap_size);
                }
                p_progress->remaining_size -= current_swap_size;
                primary_img_copy_start += current_swap_size;
                secondary_img_copy_start += current_swap_size;
                swap_scratch_start += current_swap_size;
                p_progress->swap_offset += current_swap_size;
                current_swap_size = MIN((uint32_t)p_progress->remaining_size, scratch_size);
                p_progress->scratch_size = current_swap_size;
                p_progress->swap_status = kSwapStage_A_to_B_Scratch;
                update_swap_meta(&s_boot_ctx.swap_meta);
            }
            p_progress->swap_status = kSwapStage_NotStarted;
            p_progress->swap_offset = 0;
            p_progress->scratch_size = 0;
            p_swap_meta->swap_type = kSwapType_None;

            if (p_swap_meta->copy_status != kCopyStatus_Done)
            {
                if (p_swap_meta->swap_type == kSwapType_Test)
                {
                    // Revert
                    p_swap_meta->swap_type = kSwapType_None;
                }

                // Image info switch
                image_info_t temp;
                memcpy(&temp, &p_swap_meta->image_info[0], sizeof(temp));
                memcpy(&p_swap_meta->image_info[0], &p_swap_meta->image_info[1], sizeof(temp));
                memcpy(&p_swap_meta->image_info[1], &temp, sizeof(temp));
                p_swap_meta->copy_status = kCopyStatus_Done;

                update_swap_meta(&s_boot_ctx.swap_meta);
            }

            status = kStatus_Success;
        }
    }
    else if (p_swap_meta->swap_type == kSwapType_Permanent)
    {
        // Do nothing
        status = kStatus_Success;
    }
    else if (p_swap_meta->swap_type == kSwapType_Fail)
    {
    }
    else if (p_swap_meta->swap_type == kSwapType_Fatal)
    {
    }

    return status;
}

status_t do_swap_if_needed(void)
{
    status_t status = kStatus_Fail;
    swap_meta_t *p_swap_meta = &s_boot_ctx.swap_meta;

    if (p_swap_meta->copy_status == kCopyStatus_Inactive)
    {
    }
    if (p_swap_meta->copy_status == kCopyStatus_Fail)
    {
        // TODO: Do something later
    }
    if ((p_swap_meta->confirm_info == kImageConfirm_Okay) && (p_swap_meta->swap_type == kSwapType_Test))
    {
        p_swap_meta->swap_type = kSwapType_Permanent;
        status = update_swap_meta(p_swap_meta);
    }
    else
    {
        status = run_swap_routine();
    }
    // It means copy has done, exit.

    return status;
}

// Update the status for reliable update
static void update_reliable_update_status(uint32_t status)
{
    property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;
    propertyStore->reliableUpdateStatus = status;
}

#endif //
