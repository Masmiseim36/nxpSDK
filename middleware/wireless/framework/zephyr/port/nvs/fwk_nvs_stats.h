/*! *********************************************************************************
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file fwk_nvs_stats.h
 *
 * Interface of the NVS persistent storage wear statistics.
 *
 ********************************************************************************** */

#ifndef __FWK_NVS_STATS_H__
#define __FWK_NVS_STATS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief The NVS persistent storage wear profile
 */
typedef struct nvs_storage_wear_profile
{
    uint16_t sector_count;  /* Number of persistent storage sectors */

    uint32_t erase_count[]; /* Erase count array. The size of this array
                             * is identical to the number of sectors. Each
                             * item stores the number of erases executed
                             * by NVS on that particular sector since the
                             * statistics were last reset/initialized. */
} nvs_storage_wear_profile_t;

/**
 * \brief Calculates the size (in bytes) of the NVS storage wear profile
 *
 * Calculates the size in bytes of the wear profile depending on the size of
 * the monitored persistent storage (i.e. its number of sectors).
 *
 * \param sector_cnt [in]       The number of sectors of persistent storage.
 *
 * \return size_t : the size, in bytes required for the storage wear profile
 */
#define NVS_STORAGE_WEAR_PROFILE_SIZE(sector_cnt) sizeof(nvs_storage_wear_profile_t) + ((sector_cnt) * sizeof(uint32_t))

/**
 * \brief NVS statistics event handler
 *
 * Callback function used by the NVS statistics to notify the user of a change
 * in the values of one of the counters. The stats module calls the user and
 * provides, through the function's arguments, specific information related to
 * the notification.
 *
 * \param sector_idx [in]       The sector index whose wear counter was
 *                              incremented.
 *
 * \param wear_profile [in]     The address of the wear profile data structure.
 */
typedef void (*nvs_stats_event_handler_t)(uint16_t sector_idx, const nvs_storage_wear_profile_t *wear_profile);

/**
 * \brief Initializes NVS persistent storage wear statistics.
 *
 * This function should be called first, before any other function in this
 * API.
 *
 * \param sector_count [in]     The number of sectors available in the NVS
 *                              persistent storage partition.
 *
 * \return int : 0 if successful, a negative errno error code otherwise
 */
int nvs_stats_init(uint16_t sector_count);

/**
 * \brief Shuts down NVS statistics and releases all allocated resources.
 *
 * In order to use NVS stats again, "nvs_stats_init" must be called.
 */
void nvs_stats_free(void);

/**
 * \brief Loads a wear statistics profile provided by the user.
 *
 * This function allows users to save wear statistics at system power down and
 * resume monitoring them at the next power up. Wear statistics are usually
 * more valuable when they are gathered over extended periods of time.
 *
 * Loading a wear profile may fail if the profile is not compatible with the
 * current persistent storage parameters provided at the initialization of the
 * NVS statistics ("nvs_stats_init").
 *
 * \param wear_profile [in]     Pointer to the wear profile to load.
 *
 * \return int : 0 if successful, a negative errno error code otherwise
 */
int nvs_stats_load_profile(const nvs_storage_wear_profile_t *wear_profile);

/**
 * \brief Resets the values of all NVS persistent storage wear statistics.
 *
 * \return int : 0 if successful, a negative errno error code otherwise
 */
int nvs_stats_reset(void);

/**
 * \brief Optionally registers an event handler.
 *
 * The event handler allows an immediate notification to the caller when a
 * sector wear count was incremented.
 *
 * \param evt_handler [in]      The function to be called when a sector wear
 *                              count is incremented.
 */
void nvs_stats_config_event_handler(nvs_stats_event_handler_t evt_handler);

/**
 * \brief Increments the erase count for a specified sector.
 *
 * \param sector_addr [in]      The sector address to increment erase count
 *                              for. The NVS sector address has a special
 *                              format. It essentially contains the sector
 *                              index encoded in the highest bits of the
 *                              address.
 *
 * \return int : 0 if successful, a negative errno error code otherwise
 */
int nvs_stats_incr_erase_count(uint32_t sector_addr);

/**
 * \brief Acquires the wear count for a specified sector.
 *
 * \param sector_idx [in]       The zero based sector index to get wear count
 *                              for.
 *
 * \param wear_count [out]      The location where the wear count value will
 *                              be returned. The wear count is essentially the
 *                              number of erases executed by NVS on that
 *                              particular sector of the persistent storage
 *                              partition since the last reset/initialization
 *                              of the NVS statistics.
 *
 * \return int : 0 if successful, a negative errno error code otherwise
 */
int nvs_stats_get_wear_count(uint16_t sector_idx, uint32_t *wear_count);

/**
 * \brief Acquires the entire NVS persistent storage wear profile.
 *
 * The wear profile includes the wear counts of all sectors available in the
 * persistent storage partition.
 *
 * \param sector_count [in]     The sector count specifies the available space
 *                              in the provided "wear_data" buffer.
 *
 * \param wear_data [out]       Pointer to a user allocated array to store the
 *                              wear profile data. The function assumes this
 *                              array is large enough to store at least
 *                              "sector_count" items.
 *
 * \return int : 0 if successful, a negative errno error code otherwise
 */
int nvs_stats_get_wear_profile(uint16_t sector_count, uint32_t *wear_data);

#ifdef __cplusplus
}
#endif

#endif /* __FWK_NVS_STATS_H__ */
