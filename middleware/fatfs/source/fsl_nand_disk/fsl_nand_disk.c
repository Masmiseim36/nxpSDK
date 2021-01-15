/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ffconf.h"
/* This fatfs subcomponent is disabled by default
 * To enable it, define following macro in ffconf.h */
#ifdef NAND_DISK_ENABLE

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "fsl_nand_disk.h"
#include "fsl_nand_flash.h"
#include "map.h"
#include "dhara_nand_config.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
typedef struct _dhara_handle
{
    struct dhara_nand d;
    nand_handle_t nandHandle;
    uint8_t page_buf[1U << EXAMPLE_DHARA_NAND_LOG2_PAGE_SIZE];
}dhara_handle_t;
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Card descriptor */
struct dhara_map map;
dhara_handle_t dhara_handle;
extern nand_config_t nandConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/
DRESULT nand_disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count)
{
    assert(count == 1U);

    dhara_error_t err;

    if (pdrv != NANDDISK)
    {
        return RES_PARERR;
    }

    if (dhara_map_write(&map, sector, buff, &err) < 0)
    {
        return RES_ERROR;
    }

    return RES_OK;
}

DRESULT nand_disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count)
{
    assert(count == 1U);

    dhara_error_t err;

    if (pdrv != NANDDISK)
    {
        return RES_PARERR;
    }

    if (dhara_map_read(&map, sector, buff, &err) < 0)
    {
        return RES_ERROR;
    }

    return RES_OK;
}

DRESULT nand_disk_ioctl(BYTE pdrv, BYTE cmd, void* buff)
{
    DRESULT result = RES_OK;
    DWORD sector;
    dhara_error_t err;

    if (pdrv != NANDDISK)
    {
        return RES_PARERR;
    }

    switch (cmd)
    {
        case GET_SECTOR_COUNT:
            if (buff)
            {
                *(uint32_t *)buff = dhara_map_capacity(&map);
            }
            else
            {
                result = RES_PARERR;
            }
            break;
        case GET_SECTOR_SIZE:
            if (buff)
            {
                *(uint32_t *)buff = 1 << EXAMPLE_DHARA_NAND_LOG2_PAGE_SIZE;
            }
            else
            {
                result = RES_PARERR;
            }
            break;
        case GET_BLOCK_SIZE:
            if (buff)
            {
                *(uint32_t *)buff = 1 << EXAMPLE_DHARA_NAND_LOG2_PAGE_PER_BLOCK;
            }
            else
            {
                result = RES_PARERR;
            }
            break;
        case CTRL_SYNC:
            if(dhara_map_sync(&map, NULL) != 0U)
            {
                result = RES_ERROR;
            }
            else
            {
                result = RES_OK;
            }
            break;
        case CTRL_TRIM:
            for(sector = ((DWORD *)buff)[0]; sector <= ((DWORD *)buff)[1]; sector++)
            {
                if(dhara_map_trim(&map, sector, &err) < 0)
                {
                    result = RES_ERROR;
                    break;
                }
            }
            break;

        default:
            result = RES_PARERR;
            break;
    }

    return result;
}

DSTATUS nand_disk_status(BYTE pdrv)
{
    if (pdrv != NANDDISK)
    {
        return STA_NOINIT;
    }

    return 0;
}

DSTATUS nand_disk_initialize(BYTE pdrv)
{
    status_t status;

    if (pdrv != NANDDISK)
    {
        return STA_NOINIT;
    }

    status = Nand_Flash_Init(&nandConfig, &(dhara_handle.nandHandle));
    if (status != kStatus_Success)
    {
        return STA_NOINIT;
    }

    dhara_handle.d.log2_page_size = EXAMPLE_DHARA_NAND_LOG2_PAGE_SIZE;
    dhara_handle.d.log2_ppb = EXAMPLE_DHARA_NAND_LOG2_PAGE_PER_BLOCK;
    dhara_handle.d.num_blocks = EXAMPLE_DHARA_NAND_BLOCKS_NUMBER;

    dhara_map_init(&map, &(dhara_handle.d), dhara_handle.page_buf, EXAMPLE_DHARA_GARBAGE_COLLECTION_RATIO);
    dhara_map_resume(&map, NULL);

    return 0;
}
#endif /* NAND_DISK_ENABLE */
