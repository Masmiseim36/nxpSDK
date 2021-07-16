/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <toolchain.h>
#include <porting.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/hts.h>
#include <fsl_debug_console.h>
#include <host_msd_fatfs.h>

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 static void connected(struct bt_conn *conn, uint8_t err);
 static void disconnected(struct bt_conn *conn, uint8_t reason);

/*******************************************************************************
 * Variables
 ******************************************************************************/
struct bt_conn *default_conn;

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA_BYTES(BT_DATA_UUID16_ALL,
              BT_UUID_16_ENCODE(BT_UUID_HTS_VAL),
              BT_UUID_16_ENCODE(BT_UUID_DIS_VAL)),
};

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void connected(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err)
    {
        PRINTF("Failed to connect to %s (err %u)\n", addr, err);
    }
    else
    {
        PRINTF("Connected to peer: %s\n", addr);
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    PRINTF("Disconnected (reason 0x%02x)\n", reason);

    if (default_conn) {
        bt_conn_unref(default_conn);
        default_conn = NULL;
    }
}

static void bt_ready(int err)
{
    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

    PRINTF("Bluetooth initialized\n");

    bt_conn_cb_register(&conn_callbacks);

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err)
    {
        PRINTF("Advertising failed to start (err %d)\n", err);
        return;
    }

    PRINTF("Advertising successfully started\n");
}

void peripheral_ht_task(void *pvParameters)
{
    int err;

    err = bt_enable(bt_ready);
    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

    while(1)
    {
        vTaskDelay(1000);
        bt_hts_indicate();
    }
}
