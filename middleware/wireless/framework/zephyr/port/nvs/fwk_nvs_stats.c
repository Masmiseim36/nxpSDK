/*! *********************************************************************************
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file fwk_nvs_stats.c
 *
 * Implementation of the NVS persistent storage wear statistics.
 *
 ********************************************************************************** */

#include "fwk_nvs_stats.h"

#include <stdlib.h>
#include <string.h>

#include "nvs_priv.h"
#include "zephyr_errno.h"

/**
 * \brief The NVS persistent storage wear statistics control block
 */
struct nvs_stats_cb
{
    nvs_stats_event_handler_t evt_handler; /* The user defined event
                                            * handler */
    nvs_storage_wear_profile_t *stats;     /* The persistent storage wear
                                            * profile data */
};

static struct nvs_stats_cb nvs_stats = {0};

int nvs_stats_init(uint16_t sector_count)
{
    if (nvs_stats.stats)
    {
        if (nvs_stats.stats->sector_count == sector_count)
        {
            /* Statistics already initialized and compatible */
            return 0;
        }
        /*
         * Old stats are not compatible with the specified number of
         * sectors. Realloc
         */
        free(nvs_stats.stats);
    }

    /* Allocate memory for the NVS storage wear profile */
    nvs_stats.stats = (nvs_storage_wear_profile_t *)calloc(1, NVS_STORAGE_WEAR_PROFILE_SIZE(sector_count));
    if (!nvs_stats.stats)
    {
        return -ENOMEM;
    }

    nvs_stats.stats->sector_count = sector_count;

    return 0;
}

void nvs_stats_free(void)
{
    if (!nvs_stats.stats)
    {
        /* Stats not initialized */
        return;
    }

    free(nvs_stats.stats);
    nvs_stats.stats = NULL;
}

int nvs_stats_load_profile(const nvs_storage_wear_profile_t *wear_profile)
{
    if (!wear_profile)
    {
        return -EINVAL;
    }

    if (nvs_stats.stats)
    {
        /*
         * Check whether the wear profile to load is compatible with
         * the existing wear profile
         */
        if (nvs_stats.stats->sector_count != wear_profile->sector_count)
        {
            /*
             * New profile not compatible so refuse to switch to the
             * new wear profile
             */
            return -EINVAL;
        }
    }

    /* Overwrite the current statistics with the new wear profile */
    memcpy(nvs_stats.stats, wear_profile, NVS_STORAGE_WEAR_PROFILE_SIZE(wear_profile->sector_count));

    return 0;
}

int nvs_stats_reset(void)
{
    if (!nvs_stats.stats)
    {
        /* Stats not initialized */
        return -ENODEV;
    }

    memset(nvs_stats.stats->erase_count, 0, nvs_stats.stats->sector_count * sizeof(uint32_t));

    return 0;
}

void nvs_stats_config_event_handler(nvs_stats_event_handler_t evt_handler)
{
    nvs_stats.evt_handler = evt_handler;
}

int nvs_stats_incr_erase_count(uint32_t sector_addr)
{
    if (!nvs_stats.stats)
    {
        /* Stats not initialized */
        return -ENODEV;
    }

    sector_addr = (sector_addr & ADDR_SECT_MASK) >> ADDR_SECT_SHIFT;
    if (sector_addr >= nvs_stats.stats->sector_count)
    {
        /* Sector Id is out of range (don't have that many sectors) */
        return -EINVAL;
    }

    nvs_stats.stats->erase_count[sector_addr]++;
    /* Call the event handler if one was registered */
    if (nvs_stats.evt_handler)
    {
        nvs_stats.evt_handler((uint16_t)sector_addr, nvs_stats.stats);
    }

    return 0;
}

int nvs_stats_get_wear_count(uint16_t sector_idx, uint32_t *wear_count)
{
    if (!nvs_stats.stats)
    {
        /* Stats not initialized */
        return -ENODEV;
    }

    if (sector_idx >= nvs_stats.stats->sector_count || !wear_count)
    {
        return -EINVAL;
    }

    *wear_count = nvs_stats.stats->erase_count[sector_idx];

    return 0;
}

int nvs_stats_get_wear_profile(uint16_t sector_count, uint32_t *wear_data)
{
    if (!wear_data)
    {
        return -EINVAL;
    }

    if (!nvs_stats.stats)
    {
        /* Stats not initialized */
        return -ENODEV;
    }

    if (sector_count != nvs_stats.stats->sector_count)
        /* The provided storage is not suited for the data transfer */
        return -EINVAL;

    memcpy(wear_data, nvs_stats.stats->erase_count, sector_count * sizeof(uint32_t));

    return 0;
}
