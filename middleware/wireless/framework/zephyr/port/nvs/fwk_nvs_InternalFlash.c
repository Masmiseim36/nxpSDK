/*! *********************************************************************************
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file
 *
 * This is source implementation of the NVS adaptation layer.
 *
 ********************************************************************************** */

#include "nvs_port.h"
#include "fsl_adapter_flash.h"
#include "EmbeddedTypes.h"
#include "fwk_platform_definitions.h"

const struct flash_parameters internal_nv_storage_params = {
    .write_block_size = 16u,
    .erase_value      = 0xffu,
    .sector_size      = PLATFORM_INTFLASH_SECTOR_SIZE,
};

#define FLASH_DEV_OFF_2_PHYS_ADDR(offset) ((uint32_t)offset + FSL_FEATURE_FLASH_PFLASH_START_ADDRESS)
#define FLASH_DEV_2_NV_OFFSET(addr)       ((uint32_t)(addr) - (uint32_t)NV_STORAGE_START_ADDRESS)

static int intflash_read(const struct device *dev, off_t offset, void *data, size_t len);
static int intflash_write(const struct device *dev, off_t offset, const void *data, size_t len);
static int intflash_erase(const struct device *dev, off_t offset, size_t size);
static int intflash_blank_check(const struct device *dev, off_t offset, size_t len);
static int intflash_init(void);
static const struct flash_parameters *intflash_get_parameters(const struct device *dev);

const struct flash_driver_api int_flash_ops = {.init           = &intflash_init,
                                               .read           = &intflash_read,
                                               .write          = &intflash_write,
                                               .erase          = &intflash_erase,
                                               .blank_check    = &intflash_blank_check,
                                               .get_parameters = &intflash_get_parameters};

const struct device intflash_dev = {.api = &int_flash_ops};

const struct flash_area intflash_area = {
    /** ID number */
    .fa_id   = 0u,
    .fa_off  = NV_STORAGE_START_OFFSET,
    .fa_size = (uint32_t)NV_STORAGE_SIZE,
    .fa_dev  = &intflash_dev,
};

static int intflash_init(void)
{
    int st = -1;
    if (HAL_FlashInit() == kStatus_HAL_Flash_Success)
    {
        st = 0;
    }
    return st;
}

static int intflash_read(const struct device *dev, off_t offset, void *data, size_t len)
{
    int st = -1;
    NOT_USED(dev);

    do
    {
        uint8_t               *dst;
        union physical_address read_address;

        if (((uint32_t)FLASH_DEV_2_NV_OFFSET(offset) + len) > intflash_area.fa_size)
        {
            break;
        }
        dst                = data;
        read_address.value = FLASH_DEV_OFF_2_PHYS_ADDR(offset);

        memcpy(dst, read_address.pointer, len);
        // TODO understand why When reading an erased area some surprises may happen with cache.
        // Also why phrase alignment has to be forced
        // flash_cache_disable();
        // for (int i = 0; i < len; i++)
        // {
        //     uint8_t val = p[i];
        //     dst[i] = val;
        // }

        st = 0;
    } while (0 != 0);
    return st;
}

static int intflash_blank_check(const struct device *dev, off_t offset, size_t len)
{
    int st = -1;
    NOT_USED(dev);

    do
    {
        union physical_address chk_address;

        if (((uint32_t)FLASH_DEV_2_NV_OFFSET(offset) + len) > intflash_area.fa_size)
        {
            break;
        }
        chk_address.value = FLASH_DEV_OFF_2_PHYS_ADDR(offset);

        if (HAL_FlashVerifyErase(chk_address.value, len, (hal_flash_margin_value_t)0u) != kStatus_HAL_Flash_Success)
        {
            st = 1;
            break;
        }
        st = 0;
    } while (0 != 0);
    return st;
}

static int intflash_write(const struct device *dev, off_t offset, const void *data, size_t len)
{
    int st = -1;
    NOT_USED(dev);

    do
    {
        union physical_address write_address;

        if (((uint32_t)FLASH_DEV_2_NV_OFFSET(offset) + len) > intflash_area.fa_size)
        {
            break;
        }
        write_address.value = FLASH_DEV_OFF_2_PHYS_ADDR(offset);

        if (HAL_FlashProgramUnaligned(write_address.value, len, (uint8_t *)data) != kStatus_HAL_Flash_Success)
        {
            break;
        }
        st = 0;
    } while (0 != 0);
    return st;
}

static int intflash_erase(const struct device *dev, off_t offset, size_t size)
{
    int st = -1;
    NOT_USED(dev);

    do
    {
        union physical_address erase_address;

        if (((uint32_t)FLASH_DEV_2_NV_OFFSET(offset) + size) > intflash_area.fa_size)
        {
            break;
        }
        erase_address.value = FLASH_DEV_OFF_2_PHYS_ADDR(offset);

        if (HAL_FlashEraseSector(erase_address.value, size) != kStatus_HAL_Flash_Success)
        {
            break;
        }
        st = 0;
    } while (0 != 0);
    return st;
}

static const struct flash_parameters *intflash_get_parameters(const struct device *dev)
{
    NOT_USED(dev);
    return (const struct flash_parameters *)&internal_nv_storage_params;
}