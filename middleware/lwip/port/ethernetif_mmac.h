/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ethernetif.h"

#ifndef __ETHERNETIF_MMAC_H__
#define __ETHERNETIF_MMAC_H__

/**
 * @brief Used to retrieve an index of an existing item in the MAC address filter list.
 *
 * @param netif A netif pointer.
 * @param mmac_arr MAC address to look up as a byte array.
 * @return non-negative int - wanted index, if the item is located in the list
           -1               - on failure (list not enabled or no free space)
 */
int ethernetif_mmac_get_idx(const struct netif *netif_, const uint8_t *mmac_arr);

/**
 * @brief Checks if an item at a given index is new (empty).
 *
 * @param idx The index to check.
 * @return true if it is new, false otherwise or if the list is disabled.
 */
bool ethernetif_mmac_is_new(const int idx);

/**
 * @brief Checks if an item will be erased after decreasing the refcounter by 1.
 *
 * @param idx The index to check
 * @return true if it will be removed, false otherwise or if the list is disabled.
 */
bool ethernetif_mmac_is_pending(const int idx);

/**
 * @brief Used to increase existing MAC address filter's reference counter or
 * add a new item.
 *
 * @param netif     A netif pointer.
 * @param mmac_addr MAC address to set, if the item happens to be new.
 * @param idx       The item's index.
 * @return - kStatus_Success         - on success
 *         - kStatus_Fail            - on list disabled
 *         - kStatus_InvalidArgument - on invalid netif or idx given or
 *                                     mismatching MAC address
 */
status_t ethernetif_mmac_ref_inc(const struct netif *netif_, const uint8_t *mmac_arr, const int idx);

/**
 * Used to decrease existing MAC address filter's reference counter or remove it
 * completely.
 *
 * @param netif A netif pointer.
 * @param idx   The item's index.
 * @return - kStatus_Success         - on success
 *         - kStatus_Fail            - on list disabled
 *         - kStatus_InvalidArgument - on invalid netif or idx given or item not found
 */
status_t ethernetif_mmac_ref_dec(const struct netif *netif_, const int idx);

#endif /* #ifdef __ETHERNETIF_MMAC_H__ */
