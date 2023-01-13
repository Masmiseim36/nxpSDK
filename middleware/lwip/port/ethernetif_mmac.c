/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ethernetif_mmac.h"

#include <stdint.h>
#include <stdbool.h>

#include "lwip/opt.h"
#include "fsl_device_registers.h"

#if LWIP_IPV6 && LWIP_IPV6_MLD
#define _NUM_MLD6_GROUP MEMP_NUM_MLD6_GROUP
#else
#define _NUM_MLD6_GROUP 0
#endif

#if LWIP_IGMP
#define _NUM_IGMP_GROUP MEMP_NUM_IGMP_GROUP
#else
#define _NUM_IGMP_GROUP 0
#endif

/**
 * Maximal possible count of IPv4 and IPv6 multicast MAC addresses
 * on all netifs.
 */
#if (defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0))
#define MMAC_CNT 0 /* LPC does not support MAC filters. It can only allow or disable all multicast packets */
#else
#define MMAC_CNT ((_NUM_MLD6_GROUP) + (_NUM_IGMP_GROUP))
#endif

#if MMAC_CNT > 0
/**
 * @brief MMAC reflist item.
 */
struct mmac_ref_item
{
    uint8_t netif_num;
    uint64_t mmac;
    uint16_t ref_cnt;
};

/**
 * @brief MAC reflist itself.
 */
static struct mmac_ref_item mmac_ref_list[MMAC_CNT];

static uint64_t mac_array_to_uint64(const uint8_t *mmac_arr)
{
    int i;
    uint64_t mmac = 0;

    for (i = 0; i < 6; i++)
    {
        mmac <<= 8;
        mmac |= ((uint64_t)mmac_arr[i] & 0xffU);
    }

    return mmac;
}
/**
 * @brief Used to retrieve a new (upcoming) index in the MAC address filter list.
 *
 * @param netif A netif pointer.
 * @return New index on success, -1 and fails an assertion if the list capacity
 *         has been reached.
 */
static int ethernetif_mmac_get_new_idx(void)
{
    int i;
    int idx_free = -1;

    for (i = 0; i < MMAC_CNT; i++)
    {
        if (mmac_ref_list[i].ref_cnt <= 0)
        {
            idx_free = i;
            break;
        }
    }

    LWIP_ASSERT("Multicast MAC list out of space", idx_free >= 0);
    return idx_free;
}
#endif /* #if MMAC_CNT > 0 */

int ethernetif_mmac_get_idx(const struct netif *netif_, const uint8_t *mmac_arr)
{
#if MMAC_CNT <= 0
    (void)(netif_);
    (void)(mmac_arr);

    return -1;
#else  /* #if MMAC_CNT <= 0 */
    /* Convert the MAC address to a 64-bit integer */
    uint64_t mmac = mac_array_to_uint64(mmac_arr);

    if (netif_ == NULL || mmac == 0)
    {
        return -1;
    }

    /* Iterate over every item */
    for (int i = 0; i < MMAC_CNT; i++)
    {
        if (mmac_ref_list[i].netif_num == netif_->num && mmac_ref_list[i].mmac == mmac)
        {
            /* Both the MAC address and the iface index match, this is the index */
            return i;
        }
    }

    /* No index found, return new item's index */
    return ethernetif_mmac_get_new_idx();
#endif /* #if MMAC_CNT <= 0 */
}

bool ethernetif_mmac_is_new(const int idx)
{
#if MMAC_CNT <= 0
    (void)idx;
    return false;
#else
    return mmac_ref_list[idx].mmac == 0;
#endif
}

bool ethernetif_mmac_is_pending(const int idx)
{
#if MMAC_CNT <= 0
    (void)idx;
    return false;
#else
    return mmac_ref_list[idx].ref_cnt <= 1 && !ethernetif_mmac_is_new(idx);
#endif
}

status_t ethernetif_mmac_ref_inc(const struct netif *netif_, const uint8_t *mmac_arr, const int idx)
{
#if MMAC_CNT <= 0
    (void)netif_;
    (void)idx;

    return kStatus_Fail;
#else  /* #if MMAC_CNT <= 0 */
    uint64_t mmac     = mac_array_to_uint64(mmac_arr);
    uint8_t netif_num = netif_->num;

    if (netif_ == NULL || idx >= MMAC_CNT || idx < 0 || mmac == 0)
    {
        return kStatus_InvalidArgument;
    }

    if (!ethernetif_mmac_is_new(idx) && mmac_ref_list[idx].mmac != mmac)
    {
        /*
         * Item with a given index exists (non-zero MAC address) and a different
         * (inconsistent) MAC address has been given.
         */
        return kStatus_InvalidArgument;
    }

    /* Simply fill the item, increment the refcounter and return. */
    mmac_ref_list[idx].mmac      = mmac;
    mmac_ref_list[idx].netif_num = netif_num;
    mmac_ref_list[idx].ref_cnt++;

    return kStatus_Success;
#endif /* #if MMAC_CNT <= 0 */
}

status_t ethernetif_mmac_ref_dec(const struct netif *netif_, const int idx)
{
#if MMAC_CNT <= 0
    (void)netif_;
    (void)idx;
    return kStatus_Fail;
#else
    if (netif_ == NULL || idx < 0 || idx >= MMAC_CNT)
    {
        return kStatus_InvalidArgument;
    }

    if (mmac_ref_list[idx].mmac == 0)
    {
        /* Item is empty ("nonexistent"), this shouldn't happen */
        return kStatus_InvalidArgument;
    }

    /* Decrease the refcounter. */
    mmac_ref_list[idx].ref_cnt--;

    if (mmac_ref_list[idx].ref_cnt == 0)
    {
        /*
         * Refcounter does not indicate any references - "remove" the item and
         * return as such.
         */
        memset(&mmac_ref_list[idx], 0, sizeof(struct mmac_ref_item));
    }

    return kStatus_Success;
#endif /* #if MMAC_CNT <= 0 */
}
