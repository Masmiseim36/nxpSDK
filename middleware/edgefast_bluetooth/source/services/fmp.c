/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include <stddef.h>
#include <string.h>
#include <errno/errno.h>
#include <porting.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>

#include <bluetooth/services/fmp.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t ias_alert_level = NO_ALERT;
static alert_ui_cb s_alert_ui_cb = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
ssize_t write_ias_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                              const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* GATT Database */
static BT_GATT_SERVICE_DEFINE(fmp_svc,
  BT_GATT_PRIMARY_SERVICE(BT_UUID_IAS),
  BT_GATT_CHARACTERISTIC(BT_UUID_ALERT_LEVEL, BT_GATT_CHRC_WRITE,
                   BT_GATT_PERM_WRITE, NULL, write_ias_alert_level, &ias_alert_level)
);

/* GATT operations callbacks */
ssize_t write_ias_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    const uint8_t* data = buf;

    ias_alert_level = data[0];

    if (s_alert_ui_cb != NULL)
    {
        if (conn != NULL)
        {
            PRINTF("Locator is setting Immediate Alert...\r\n");
        }
        else
        {
            PRINTF("Target setting Immediate Alert...\r\n");
        }
        s_alert_ui_cb(ias_alert_level);
    }

    return len;
}

/* Init/deinit APIs */
int fmp_init(alert_ui_cb cb)
{
    s_alert_ui_cb = cb;

    return 0;
}

int fmp_deinit(void)
{
    s_alert_ui_cb = NULL;

    return 0;
}

/* Local get/set APIs */
uint8_t fmp_ias_get_alert_level(void)
{
    return ias_alert_level;
}

