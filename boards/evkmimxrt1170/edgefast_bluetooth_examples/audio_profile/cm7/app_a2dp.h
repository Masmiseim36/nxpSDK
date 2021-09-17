/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_EDGEFAST_A2DP_H__
#define __APP_EDGEFAST_A2DP_H__

typedef struct app_a2dp_
{
    struct bt_a2dp *a2dp_handle;
    struct bt_a2dp_endpoint *a2dp_ep;
    struct bt_conn *conn;
    uint8_t peer_key_missed;
    uint8_t acl_initiated;
    uint8_t peer_bd_addr[6];
} app_a2dp_t;

extern app_a2dp_t app_a2dp;

void app_edgefast_a2dp_init(void);

void app_edgefast_a2dp_suspend(void);

void app_edgefast_a2dp_start(void);
#endif
