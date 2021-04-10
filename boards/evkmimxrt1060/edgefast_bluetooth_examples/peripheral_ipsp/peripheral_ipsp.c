/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <toolchain.h>
#include <porting.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/services/ipsp.h>

#include "fsl_debug_console.h"
#include "BT_assigned_numbers.h"

struct bt_conn *default_conn = NULL;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_IPSS_VAL))
};


/* 6LowPan entry point */
static int ipsp_rx_cb(struct net_buf *buf)
{
    int i;

    if (buf != NULL)
    {
        PRINTF("Received message: ");
        for(i = 0; i < buf->len; i++)
        {
            PRINTF("%c", buf->data[i]);
        }
        PRINTF("\r\n");
    }

    return 0;
}

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
		default_conn = bt_conn_ref(conn);
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

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};

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
    
	/* Initialize IPSP Node */
	ipsp_init(ipsp_rx_cb);
	err = ipsp_listen();
    if (!err)
    {
        PRINTF("IPSS Service ready\n");
    }
}

void peripheral_ipsp_task(void *pvParameters)
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
    }
}
