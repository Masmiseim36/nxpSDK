/** @file
 *  @brief Time Profile
 */

/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno/errno.h>
#include <stdbool.h>
#include <zephyr/types.h>
#include <sys/atomic.h>

#include <porting.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>
#include <bluetooth/services/tip.h>

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_SERVICE)
#define LOG_MODULE_NAME bt_rtus
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static time_update_state_t   rtus_time_update_state = {0};
static time_update_command_t rtus_command = 0;

static cts_reference_time_info_t cts_reference_time = {0};
static cts_local_time_info_t cts_local_time = {0};
static cts_measurement_t cts_measurement = {0};
static cts_notification_enabled_cb callback = NULL;
static bool cccd_written = false;
uint32_t lastTime = 0;

static ndcs_time_with_dst_t ndcs_time_with_dst = {0};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static ssize_t read_time_update_state(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset);

static ssize_t write_time_update_control_point(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, const void *buf,
                   uint16_t len, uint16_t offset, uint8_t flags);

static ssize_t read_cts_current_time(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset);

static ssize_t read_cts_local_time(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset);

static ssize_t read_cts_reference_time(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset);

static void cts_ccc_cfg_changed(const struct bt_gatt_attr *attr,
         uint16_t value);

static cts_day_date_time_t cts_epoch_to_day_date_time (uint32_t epoch_time);
static uint8_t get_month_days (unit_month_t month, uint8_t is_leap_year);

static ssize_t read_ndcs_time_with_dst(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset);

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* GATT Database */
static BT_GATT_SERVICE_DEFINE(rtus,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_RTUS),
    BT_GATT_CHARACTERISTIC(BT_UUID_RTUS_TIME_UPDATE_STATE, BT_GATT_CHRC_READ,
                   BT_GATT_PERM_READ, read_time_update_state, NULL,
                   &rtus_time_update_state),
    BT_GATT_CHARACTERISTIC(BT_UUID_RTUS_CONTROL_POINT, BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                   BT_GATT_PERM_WRITE, NULL, write_time_update_control_point, &rtus_command),
);
static BT_GATT_SERVICE_DEFINE(cts,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_CTS),
    BT_GATT_CHARACTERISTIC(BT_UUID_CTS_CURRENT_TIME, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                   BT_GATT_PERM_READ, read_cts_current_time, NULL,
                   &cts_measurement),
    BT_GATT_CCC(cts_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_CTS_LOCAL_TIME_INFORMATION, BT_GATT_CHRC_READ,
                   BT_GATT_PERM_READ, read_cts_local_time, NULL,
                   &cts_local_time),
    BT_GATT_CHARACTERISTIC(BT_UUID_CTS_REFERENCE_TIME_INFORMATION, BT_GATT_CHRC_READ,
                   BT_GATT_PERM_READ, read_cts_reference_time, NULL,
                   &cts_reference_time),
);
static BT_GATT_SERVICE_DEFINE(ndts,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_NDTS),
    BT_GATT_CHARACTERISTIC(BT_UUID_NDTS_TIME_WITH_DTS, BT_GATT_CHRC_READ,
                   BT_GATT_PERM_READ, read_ndcs_time_with_dst, NULL,
                   &ndcs_time_with_dst),
);

/* GATT operations callbacks */
static ssize_t read_time_update_state(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &rtus_time_update_state,
                 sizeof(rtus_time_update_state));
}

static ssize_t write_time_update_control_point(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, const void *buf,
                   uint16_t len, uint16_t offset, uint8_t flags)
{
    time_update_command_t command;
    time_update_state_t time_update_state = {0};

    const time_update_command_t* data = buf;

    command = data[0];

    switch(command)
    {
        case RTUS_GET_REF_UPDATE:
        {
            time_update_state.current_state = RTUS_UPDATE_PENDING;
            break;
        }
        case RTUS_CANCEL_REF_UPDATE:
        {
            time_update_state.current_state = RTUS_IDLE;
            time_update_state.result = RTUS_CANCELED;
            break;
        }
        default:
        {
            ;
            break;
        }
    }

    bt_rtus_set_time_update_state(time_update_state);

    return len;
}

static ssize_t read_cts_current_time(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &cts_measurement,
                 sizeof(cts_measurement_t));
}

static ssize_t read_cts_local_time(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &cts_local_time,
                 sizeof(cts_local_time_info_t));
}

static ssize_t read_cts_reference_time(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &cts_reference_time,
                 sizeof(cts_reference_time_info_t));
}

static ssize_t read_ndcs_time_with_dst(struct bt_conn *conn,
                   const struct bt_gatt_attr *attr, void *buf,
                   uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &ndcs_time_with_dst,
                 sizeof(ndcs_time_with_dst_t));
}

/*******************************************************************************
 * Profile API
 ******************************************************************************/
void bt_rtus_set_time_update_state(time_update_state_t time_update_state)
{
    rtus_time_update_state = time_update_state;
}

time_update_state_t bt_rtus_get_time_update_state(void)
{
    return rtus_time_update_state;
}

void bt_rtus_set_time_update_control_point(time_update_command_t command)
{
    rtus_command = command;
}

time_update_command_t bt_rtus_get_time_update_control_point(void)
{
    return rtus_command;
}

void bt_cts_set_current_time(struct bt_conn *conn, cts_measurement_t current_time, uint32_t local_time)
{
    cts_measurement.exact_time.cts_day_date_time = cts_epoch_to_day_date_time(local_time);
    cts_measurement.exact_time.fractions = 0U;
    cts_measurement.adjust_reason = current_time.adjust_reason;

    if (current_time.adjust_reason != CTS_UNKNOWN_REASON)
    {
        if(current_time.adjust_reason != CTS_EXTERNAL_REF_UPDATE)
        {
            cts_notify(conn);
        }
        else
        {
            if (((lastTime > local_time) && (lastTime - local_time > 60U)) ||
                ((local_time > lastTime) && (local_time - lastTime > 60U)))
            {
                cts_notify(conn);
            }
        }
    }
    lastTime = local_time;
}

cts_measurement_t bt_cts_get_current_time(void)
{
    return cts_measurement;
}


void bt_cts_set_local_time(cts_local_time_info_t local_time)
{
    memcpy(&cts_local_time, &local_time, sizeof(cts_local_time_info_t));
}

cts_local_time_info_t bt_cts_get_local_time(void)
{
    return cts_local_time;
}

void bt_cts_set_reference_time(cts_reference_time_info_t reference_time)
{
    memcpy(&cts_reference_time, &reference_time, sizeof(cts_reference_time_info_t));
}

cts_reference_time_info_t bt_cts_get_reference_time(void)
{
    return cts_reference_time;
}

void cts_notify(struct bt_conn *conn)
{
    cts_measurement_t current_time;

    if (cccd_written)
    {
        memcpy(&current_time, &cts_measurement, sizeof(cts_measurement_t));
        (void)bt_gatt_notify(conn, &cts.attrs[1], &current_time, sizeof(cts_measurement_t));
    }
}

void bt_cts_register_notification_callback(cts_notification_enabled_cb notif_callback)
{
    callback = notif_callback;
}

void bt_ndcs_set_time_with_dst(ndcs_time_with_dst_t time_with_dst)
{
    memcpy(&ndcs_time_with_dst, &time_with_dst, sizeof(ndcs_time_with_dst_t));
}

ndcs_time_with_dst_t bt_ndcs_get_time_with_dst(void)
{
    return ndcs_time_with_dst;
}

/*******************************************************************************
 * Private functions
 ******************************************************************************/
static void cts_ccc_cfg_changed(const struct bt_gatt_attr *attr,
         uint16_t value)
{
    cccd_written = (value == BT_GATT_CCC_NOTIFY) ? true : false;

    if (callback != NULL)
    {
        callback();
    }
}

static uint8_t get_month_days (unit_month_t month, uint8_t is_leap_year)
{
    uint8_t result = 0;
    switch(month)
    {
        case UNIT_JANUARY: result = 31U; break;
        case UNIT_FEBRUARY:
        {
            if(is_leap_year > 0U)
            {
                result = 29U;
            }
            else
            {
                result = 28U;
            }
        }
        break;

        case UNIT_APRIL:
        case UNIT_JUNE:
        case UNIT_SEPTEMBER:
        case UNIT_NOVEMBER:
            result = 30U;
            break;
        case UNIT_MARCH:
        case UNIT_MAY:
        case UNIT_JULY:
        case UNIT_AUGUST:
        case UNIT_OCTOBER:
        case UNIT_DECEMBER:
            result = 31U;
            break;

        default: result = 0; break;
    }

    return result;
}

static cts_day_date_time_t cts_epoch_to_day_date_time (uint32_t epoch_time)
{
    cts_day_date_time_t temp_time;
    uint16_t day_of_year;
    uint16_t day_of_month;
    uint8_t month;
    uint32_t year;

    temp_time.date_time.seconds = (uint8_t)(epoch_time % 60U);
    epoch_time /= 60U;
    temp_time.date_time.minutes = (uint8_t)(epoch_time % 60U);
    epoch_time /= 60U;
    temp_time.date_time.hours = (uint8_t)(epoch_time % 24U);
    epoch_time /= 24U;
    year = (((epoch_time * 4U) + 2U) / 1461U);
    temp_time.date_time.year = (uint16_t)(year + 1970U);
    day_of_year = (uint16_t)(epoch_time - (((year * 1461U) + 1U) / 4U));

    day_of_month = (uint16_t)(day_of_year + 1U);
    for(month = 1U; month<=12U; month++)
    {
        uint8_t month_days = (uint8_t)get_month_days(month, (((year+2U) % 4U > 0U) ? 0U : 1U));
        if(day_of_month > month_days)
        {
            day_of_month -= month_days;
        }
        else
        {
            break;
        }
    }

    temp_time.date_time.month = month;
    temp_time.date_time.day = (uint8_t)day_of_month;
    temp_time.day_of_week = (cts_day_of_week_tag)((day_of_year + 4U) % 7U);

    return temp_time;
}
