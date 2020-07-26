/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ffconf.h"
/* This fatfs subcomponent is disabled by default
 * To enable it, define following macro in ffconf.h */
#ifdef SD_DISK_ENABLE

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "fsl_sd_disk.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Card descriptor */
sd_card_t g_sd;

/*******************************************************************************
 * Code
 ******************************************************************************/
DRESULT sd_disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count)
{
    if (pdrv != SDDISK)
    {
        return RES_PARERR;
    }

    if (kStatus_Success != SD_WriteBlocks(&g_sd, buff, sector, count))
    {
        return RES_ERROR;
    }

    return RES_OK;
}

DRESULT sd_disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count)
{
    if (pdrv != SDDISK)
    {
        return RES_PARERR;
    }

    if (kStatus_Success != SD_ReadBlocks(&g_sd, buff, sector, count))
    {
        return RES_ERROR;
    }

    return RES_OK;
}

DRESULT sd_disk_ioctl(BYTE pdrv, BYTE cmd, void* buff)
{
    DRESULT result = RES_OK;

    if (pdrv != SDDISK)
    {
        return RES_PARERR;
    }

    switch (cmd)
    {
        case GET_SECTOR_COUNT:
            if (buff)
            {
                *(uint32_t *)buff = g_sd.blockCount;
            }
            else
            {
                result = RES_PARERR;
            }
            break;
        case GET_SECTOR_SIZE:
            if (buff)
            {
                *(uint32_t *)buff = g_sd.blockSize;
            }
            else
            {
                result = RES_PARERR;
            }
            break;
        case GET_BLOCK_SIZE:
            if (buff)
            {
                *(uint32_t *)buff = g_sd.csd.eraseSectorSize;
            }
            else
            {
                result = RES_PARERR;
            }
            break;
        case CTRL_SYNC:
            result = RES_OK;
            break;
        default:
            result = RES_PARERR;
            break;
    }

    return result;
}

DSTATUS sd_disk_status(BYTE pdrv)
{
    if (pdrv != SDDISK)
    {
        return STA_NOINIT;
    }

    return 0;
}

DSTATUS sd_disk_initialize(BYTE pdrv)
{
    if (pdrv != SDDISK)
    {
        return STA_NOINIT;
    }

    if(g_sd.isHostReady)
    {
        /* reset host */
        SD_HostDoReset(&g_sd);
        SD_SetCardPower(&g_sd, false);
        SD_SetCardPower(&g_sd, true);
    }
    else
    {
        return STA_NOINIT;
    }

    if (kStatus_Success != SD_CardInit(&g_sd))
    {
        SD_CardDeinit(&g_sd);
        memset(&g_sd, 0U, sizeof(g_sd));
        return STA_NOINIT;
    }

    return 0;
}
#endif /* SD_DISK_ENABLE */
