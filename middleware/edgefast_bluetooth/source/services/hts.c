/*
 * Copyright (c) 2020 SixOctets Systems
 * Copyright (c) 2019 Aaron Tsui <aaron.tsui@outlook.com>
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <errno/errno.h>
#include <toolchain.h>
#include <porting.h>
#include <fsl_debug_console.h>
#include <sys/byteorder.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/hts.h>

#define BT_DIS_MANUF     "NXP"
#define BT_DIS_NAME      "HTS Demo"
#define BT_DIS_SN        "BLESN01"

#define BT_DIS_STR_MAX   (10U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool cccd_written;
static uint8_t indicating;
static struct bt_gatt_indicate_params ind_params;

static uint8_t manuf[BT_DIS_STR_MAX] = BT_DIS_MANUF;
static uint8_t name[BT_DIS_STR_MAX] = BT_DIS_NAME;
static uint8_t sn[BT_DIS_STR_MAX] = BT_DIS_SN;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void htmc_ccc_cfg_changed(const struct bt_gatt_attr *attr,
         uint16_t value);

static void indicate_cb(struct bt_conn *conn,
      struct bt_gatt_indicate_params *params, uint8_t err);

static ssize_t read_str(struct bt_conn *conn, const struct bt_gatt_attr *attr,
          void *buf, uint16_t len, uint16_t offset);

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Health Thermometer Service Declaration */
static BT_GATT_SERVICE_DEFINE(hts_svc,
  BT_GATT_PRIMARY_SERVICE(BT_UUID_HTS),
  BT_GATT_CHARACTERISTIC(BT_UUID_HTS_MEASUREMENT, BT_GATT_CHRC_INDICATE,
             BT_GATT_PERM_NONE, NULL, NULL, NULL),
  BT_GATT_CCC(htmc_ccc_cfg_changed,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE));
/* Device Information Service Declaration */
static BT_GATT_SERVICE_DEFINE(dev_info,
        BT_GATT_PRIMARY_SERVICE(BT_UUID_DIS),
        BT_GATT_CHARACTERISTIC(BT_UUID_DIS_MANUFACTURER_NAME, BT_GATT_CHRC_READ,
             BT_GATT_PERM_READ, read_str, NULL, manuf),
        BT_GATT_CHARACTERISTIC(BT_UUID_DIS_MODEL_NUMBER, BT_GATT_CHRC_READ,
             BT_GATT_PERM_READ, read_str, NULL, name),
        BT_GATT_CHARACTERISTIC(BT_UUID_DIS_SERIAL_NUMBER, BT_GATT_CHRC_READ,
             BT_GATT_PERM_READ, read_str, NULL, sn));

/*******************************************************************************
 * Code
 ******************************************************************************/
static void htmc_ccc_cfg_changed(const struct bt_gatt_attr *attr,
         uint16_t value)
{
    cccd_written = (value == BT_GATT_CCC_INDICATE) ? true : false;
}

static void indicate_cb(struct bt_conn *conn,
      struct bt_gatt_indicate_params *params, uint8_t err)
{
    PRINTF("Indication %s\n", err != 0U ? "fail" : "success");
    indicating = 0U;
}

void bt_hts_indicate(void)
{
    /* Temperature measurements simulation */
    static uint32_t temperature = 20U;
    static uint8_t temp_type = hts_no_temp_type;
    struct temp_measurement temp_measurement;

    if(cccd_written)
    {
        /* check to not send consecutive indications before receiving a response */
        if (indicating)
        {
            return;
        }

        PRINTF("temperature is %dC\n", temperature);
        temp_measurement.flags = hts_unit_celsius_c;
        temp_measurement.flags += hts_include_temp_type;
        temp_measurement.type = temp_type;
        sys_put_le32(temperature, temp_measurement.temperature);

        ind_params.attr = &hts_svc.attrs[2];
        ind_params.func = indicate_cb;
        ind_params.data = (uint8_t *)&temp_measurement;
        ind_params.len = sizeof(temp_measurement);

        if (bt_gatt_indicate(NULL, &ind_params) == 0)
        {
            indicating = 1U;
        }

        temperature++;
        if (temperature == 25U)
        {
            temperature = 20U;
        }

        temp_type++;
        if (temp_type > hts_tympanum)
        {
            temp_type = hts_no_temp_type;
        }
    }
}

static ssize_t read_str(struct bt_conn *conn, const struct bt_gatt_attr *attr,
          void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data,
				 strlen((char *)attr->user_data));
}
