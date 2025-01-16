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
#include "EmbeddedTypes.h"
#include "mflash_drv.h"
#include "fwk_platform_definitions.h"
#include "FunctionLib.h"
#include "fwk_platform_extflash.h"

#define RD_BUF_SZ 256u

#ifdef NV_STORAGE_EXTFLASH_START_OFFSET

const struct flash_parameters external_nv_storage_params = {
    .write_block_size = 1u,
    .erase_value      = 0xffu,
    .sector_size      = PLATFORM_EXTFLASH_SECTOR_SIZE,
};

static int extflash_read(const struct device *dev, off_t offset, void *data, size_t len);
static int extflash_write(const struct device *dev, off_t offset, const void *data, size_t len);
static int extflash_erase(const struct device *dev, off_t offset, size_t size);
static int extflash_blank_check(const struct device *dev, off_t offset, size_t len);
static int extflash_init(void);
static const struct flash_parameters *extflash_get_parameters(const struct device *dev);

const struct flash_driver_api ext_flash_ops = {
    .init           = &extflash_init,
    .read           = &extflash_read,
    .write          = &extflash_write,
    .erase          = &extflash_erase,
    .blank_check    = &extflash_blank_check,
    .get_parameters = &extflash_get_parameters,
};

const struct device extflash_dev = {.api = &ext_flash_ops};

const struct flash_area extflash_area = {
    /** ID number */
    .fa_id   = 0u,
    .fa_off  = NV_STORAGE_START_OFFSET,
    .fa_size = (uint32_t)NV_STORAGE_SIZE,
    .fa_dev  = &extflash_dev,
};

static int extflash_init(void)
{
    int st = -1;
    if (PLATFORM_InitExternalFlash() == 0)
    {
        st = 0;
    }
    return st;
}

static int extflash_read(const struct device *dev, off_t offset, void *data, size_t len)
{
    int st = -1;
    NOT_USED(dev);

    uint32_t end_addr = extflash_area.fa_off + extflash_area.fa_size;

    if (((uint32_t)offset < extflash_area.fa_off) || (((uint32_t)offset + len) > end_addr))
    {
        st = -1;
    }
    else
    {
        st = PLATFORM_ReadExternalFlash((uint8_t *)data, len, offset, true);
    }
    return st;
}

static int extflash_blank_check(const struct device *dev, off_t offset, size_t len)
{
    int st = -1;
    NOT_USED(dev);

    uint32_t end_addr = extflash_area.fa_off + extflash_area.fa_size;

    if (((uint32_t)offset < extflash_area.fa_off) || (((uint32_t)offset + len) > end_addr))
    {
        st = -1;
    }
    else
    {
        if (PLATFORM_ExternalFlashAreaIsBlank(offset, len))
        {
            st = 0;
        }
        else
        {
            st = 1;
        }
    }
    return st;
}

static int extflash_write(const struct device *dev, off_t offset, const void *data, size_t len)
{
    int st;
    NOT_USED(dev);

    uint32_t end_addr = extflash_area.fa_off + extflash_area.fa_size;

    if (((uint32_t)offset < extflash_area.fa_off) || (((uint32_t)offset + len) > end_addr))
    {
        st = -1;
    }
    else
    {
        st = PLATFORM_WriteExternalFlash((uint8_t *)data, len, offset);
    }
    return st;
}

static int extflash_erase(const struct device *dev, off_t offset, size_t size)
{
    int st = -1;
    NOT_USED(dev);

    do
    {
        uint32_t erase_address;
        uint32_t end_offset = extflash_area.fa_off + extflash_area.fa_size;

        if (size == 0u)
        {
            st = 0;
            break;
        }

        if (((uint32_t)offset < extflash_area.fa_off) || (((uint32_t)offset + size) > end_offset))
        {
            st = -1;
            break;
        }

        erase_address = offset;

        st = PLATFORM_EraseExternalFlash(erase_address, size);
    } while (0 != 0);
    return st;
}

static const struct flash_parameters *extflash_get_parameters(const struct device *dev)
{
    NOT_USED(dev);
    return (const struct flash_parameters *)&external_nv_storage_params;
}
#endif
