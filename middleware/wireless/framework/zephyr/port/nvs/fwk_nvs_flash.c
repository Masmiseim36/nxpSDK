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

#if defined                    gAppNvsInternalFlash_c && (gAppNvsInternalFlash_c > 0)
extern const struct flash_area intflash_area;
#endif

#if defined                    gAppNvsExternalFlash_c && (gAppNvsExternalFlash_c > 0)
extern const struct flash_area extflash_area;
#endif

const struct flash_area *flash_map[] = {
#if defined              gAppNvsInternalFlash_c && (gAppNvsInternalFlash_c > 0)
    [NVS_INTFLASH_AREA_ID] = &intflash_area,
#if defined              gNvsExternalFlash_d && (gNvsExternalFlash_d > 0)
    [NVS_EXTFLASH_AREA_ID] = &extflash_area,
#endif
#else
#if defined gAppNvsExternalFlash_c && (gAppNvsExternalFlash_c > 0)
    [NVS_EXTFLASH_AREA_ID] = &extflash_area,
#endif
#endif
};

static inline struct flash_area const *get_flash_area_from_id(int idx)
{
    for (int i = 0; i < sizeof(flash_map) / sizeof(flash_map[0]); i++)
    {
        if (flash_map[i]->fa_id == idx)
        {
            return flash_map[i];
        }
    }

    return NULL;
}

int flash_area_open(uint8_t id, const struct flash_area **fap)
{
    int st = -ENOENT;

    do
    {
        const struct flash_area *area;

        area = get_flash_area_from_id(id);
        if (area == NULL)
        {
            break;
        }

        if (!area->fa_dev)
        {
            break;
        }
        *fap = area;
        st   = 0;
    } while (0 != 0);
    return st;
}

int flash_area_get_sectors(int fa_id, uint32_t *count, struct flash_sector *sectors)
{
    const struct flash_area *fa;
    *count = 0u;
    int rc;
    do
    {
        const struct flash_parameters *fp;

        rc = flash_area_open(fa_id, &fa);
        if (rc < 0 || fa == NULL)
        {
            rc = -EINVAL;
            break;
        }
        fp               = fa->fa_dev->api->get_parameters(fa->fa_dev);
        *count           = fa->fa_size / fp->sector_size;
        sectors->fs_size = fp->sector_size;
        sectors->fs_off  = fa->fa_off;
    } while (false);
    return rc;
}

int flash_get_page_info_by_offs(const struct device *dev, off_t offset, struct flash_pages_info *info)
{
    NOT_USED(dev);
    NOT_USED(offset);

    info->index        = 0;
    info->size         = (uint32_t)NV_STORAGE_SECTOR_SIZE;
    info->start_offset = (uint32_t)NV_STORAGE_START_ADDRESS;
    return 0;
}

const struct device *flash_area_get_device(const struct flash_area *fa)
{
    return fa->fa_dev;
}

/*
 * Shim for mutex zephyr <-> OSA
 */
int k_mutex_lock(struct k_mutex *mutex, uint32_t timeout)
{
    return (int)OSA_MutexLock(mutex->MutexId, timeout);
}

int k_mutex_unlock(struct k_mutex *mutex)
{
    return (int)OSA_MutexUnlock(mutex->MutexId);
}

int k_mutex_init(struct k_mutex *mutex)
{
    int ret = 0;
    if (!mutex->created)
    {
        ret = (int)OSA_MutexCreate(mutex->MutexId);
        if (0 == ret)
        {
            mutex->created = true;
        }
    }
    return ret;
}

int k_mutex_destroy(struct k_mutex *mutex)
{
    int ret = 0;
    if (mutex->created)
    {
        ret            = (int)OSA_MutexDestroy(mutex->MutexId);
        mutex->created = false;
    }
    return ret;
}