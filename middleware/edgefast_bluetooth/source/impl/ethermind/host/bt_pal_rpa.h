/* bt_pal_rpa.h - Bluetooth Resolvable Private Addresses (RPA) generation and
 * resolution
 */

/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __RPA_H__
#define __RPA_H__

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>

bool bt_rpa_irk_matches(const uint8_t irk[16], const bt_addr_t *addr);
int bt_rpa_create(const uint8_t irk[16], bt_addr_t *rpa);

#endif /* __RPA_H__ */
