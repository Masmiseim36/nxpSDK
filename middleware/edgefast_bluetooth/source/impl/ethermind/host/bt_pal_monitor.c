/** @file
 *  @brief Custom logging over UART
 */

/*
 * Copyright 2021 NXP
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stdbool.h>

#include <porting.h>

#include <sys/byteorder.h>


#include <bluetooth/buf.h>

#include "bt_pal_monitor.h"

#if (defined(CONFIG_BT_DEBUG_MONITOR) && (CONFIG_BT_DEBUG_MONITOR > 0))
void bt_monitor_send(uint16_t opcode, const void *data, size_t len)
{
}

void bt_monitor_new_index(uint8_t type, uint8_t bus, const bt_addr_t *addr,
			  const char *name)
{
}
#endif
