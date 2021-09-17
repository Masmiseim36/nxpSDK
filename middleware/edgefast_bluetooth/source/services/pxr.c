/*
 * Copyright 2021 NXP
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

#include <bluetooth/services/pxr.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t ias_alert_level = NO_ALERT;
static uint8_t lls_alert_level = NO_ALERT;
static int8_t  tps_power_level = 0;
static alert_ui_cb s_alert_ui_cb = NULL;
static uint8_t pres_format_desc[7] = {0x12, 0x00, 0x87, 0x27, 0x01, 0x00, 0x00};

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* GATT Database */
static BT_GATT_SERVICE_DEFINE(pxr_svc,

	BT_GATT_PRIMARY_SERVICE(BT_UUID_LLS),
	BT_GATT_CHARACTERISTIC(BT_UUID_ALERT_LEVEL, BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_lls_alert_level, write_lls_alert_level, &lls_alert_level),

    BT_GATT_PRIMARY_SERVICE(BT_UUID_IAS),
    BT_GATT_CHARACTERISTIC(BT_UUID_ALERT_LEVEL, BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                   BT_GATT_PERM_WRITE, NULL, write_ias_alert_level, &ias_alert_level),

    BT_GATT_PRIMARY_SERVICE(BT_UUID_TPS),
    BT_GATT_CHARACTERISTIC(BT_UUID_TPS_TX_POWER_LEVEL, BT_GATT_CHRC_READ,
                   BT_GATT_PERM_READ, read_tps_power_level, NULL, &tps_power_level),
    BT_GATT_DESCRIPTOR(BT_UUID_GATT_CPF, BT_GATT_PERM_READ, read_tps_power_level_desc, NULL, pres_format_desc),
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
            PRINTF("Monitor is setting Immediate Alert...\r\n");
        }
        else
        {
            PRINTF("Locally setting Immediate Alert...\r\n");
        }
        s_alert_ui_cb(ias_alert_level);
    }

    return len;
}

ssize_t read_lls_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    uint8_t lvl8 = lls_alert_level;
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &lvl8, sizeof(lvl8));
}
                             
ssize_t write_lls_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    const uint8_t* data = buf;

    lls_alert_level = data[0];

    if (conn != NULL)
    {
        PRINTF("Monitor is setting Link Loss Alert Level to ");
    }
    else
    {
        PRINTF("Locally setting Link Loss Alert Level to ");
    }

    switch(lls_alert_level)
    {
        case NO_ALERT:
            PRINTF("OFF\r\n");
            break;

        case MILD_ALERT:
            PRINTF("MILD\r\n");
            break;

        case HIGH_ALERT:
            PRINTF("HIGH\r\n");
            break;

        default:
        	   /* Misra fix MISRA C-2012 Rule 16.4 */
            break;
    }

    return len;    
}

ssize_t read_tps_power_level_desc(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    uint8_t desc[7];
    memcpy(desc, pres_format_desc, 7);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, desc, sizeof(desc));
}

ssize_t read_tps_power_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    int8_t lvl8 = tps_power_level;
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &lvl8, sizeof(lvl8));
}

/* Local get/set APIs */
uint8_t pxr_lls_get_alert_level(void)
{
    return lls_alert_level;
}
                             
uint8_t pxr_ias_get_alert_level(void)
{
    return ias_alert_level;
}
                             
int8_t pxr_tps_get_power_level(void)
{
    return tps_power_level;
}

void pxr_tps_set_power_level(int8_t power_level)
{
    tps_power_level = power_level;
}

/* Init/deinit APIs */
int pxr_init(alert_ui_cb cb)
{
    s_alert_ui_cb = cb;

    return 0;
}

int pxr_deinit(void)
{
    s_alert_ui_cb = NULL;

    return 0;
}
