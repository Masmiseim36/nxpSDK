/*
 * Copyright 2023,2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EDGEFAST_BT_BLE_SPINLOCK_H__
#define __EDGEFAST_BT_BLE_SPINLOCK_H__

#include <system_c.h>

#ifdef __cplusplus
extern "C" {
#endif

struct k_spinlock
{
    uint8_t dummy;
};

struct k_spinlock_key
{
    int key;
};

typedef struct k_spinlock_key k_spinlock_key_t;

static ALWAYS_INLINE k_spinlock_key_t k_spin_lock(struct k_spinlock *l)
{
    k_spinlock_key_t k;

    k.key = DisableGlobalIRQ();

    return k;
}

static ALWAYS_INLINE void k_spin_unlock(struct k_spinlock *l,
					k_spinlock_key_t key)
{
    __ISB();
    __DSB();
	EnableGlobalIRQ(key.key);
}

#ifdef __cplusplus
}
#endif

#endif /* __EDGEFAST_BT_BLE_SPINLOCK_H__ */