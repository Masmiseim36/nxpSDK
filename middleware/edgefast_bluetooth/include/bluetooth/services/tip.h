/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_TIP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_TIP_H_

/**
 * @brief Time Profile (TIP)
 * @defgroup bt_tip Time Profile (TIP)
 * @ingroup bluetooth
 * @{
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */

#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RTUS Data */
#define RTUS_GET_REF_UPDATE                     0x01U
#define RTUS_CANCEL_REF_UPDATE                  0x02U

#define RTUS_IDLE                               0x00U
#define RTUS_UPDATE_PENDING                     0x01U

#define RTUS_SUCCESSFUL                         0x00U
#define RTUS_CANCELED                           0x01U
#define RTUS_NO_CONN_TO_REFERENCE               0x02U
#define RTUS_REFERENCE_RESPONSE_ERROR           0x02U
#define RTUS_TIMEOUT                            0x04U
#define RTUS_UPDATE_NOT_ATTEMPTED_AFTER_RST     0x05U

typedef uint8_t time_update_command_t;
typedef uint8_t current_state_t;
typedef uint8_t rtus_result_t;

typedef struct time_update_state_tag
{
    current_state_t      current_state;
    rtus_result_t        result;
} time_update_state_t;

/* CTS Data */
#define CTS_UNKNOWN_REASON          0x00U
#define CTS_MANUAL_UPDATE           BIT0
#define CTS_EXTERNAL_REF_UPDATE     BIT1
#define CTS_CHANGE_OF_TIMEZONE      BIT2
#define CTS_CHANGE_OF_DST           BIT3

#define CTS_ACCURACY_OUT_OF_RANGE   254U
#define CTS_ACCURACY_UNKNOWN        255U

#define CTS_UNKNOWN                 0U
#define CTS_NETWORK_TIME_PROTO      1U
#define CTS_GPS                     2U
#define CTS_RADIO_TIME_SIGNAL       3U
#define CTS_MANUAL                  4U
#define CTS_ATOMIC_CLOCK            5U
#define CTS_CELLULAR_NETWORK        6U

#define UNIT_MONTH_UNKNOWN          0x00U
#define UNIT_JANUARY                0x01U
#define UNIT_FEBRUARY               0x02U
#define UNIT_MARCH                  0x03U
#define UNIT_APRIL                  0x04U
#define UNIT_MAY                    0x05U
#define UNIT_JUNE                   0x06U
#define UNIT_JULY                   0x07U
#define UNIT_AUGUST                 0x08U
#define UNIT_SEPTEMBER              0x09U
#define UNIT_OCTOBER                0x0AU
#define UNIT_NOVEMBER               0x0BU
#define UNIT_DECEMBER               0x0CU

/* Bit shift definitions */
#define BIT0              0x01U
#define BIT1              0x02U
#define BIT2              0x04U
#define BIT3              0x08U
#define BIT4              0x10U
#define BIT5              0x20U
#define BIT6              0x40U
#define BIT7              0x80U
#define BIT8             0x100U
#define BIT9             0x200U
#define BIT10            0x400U

typedef uint8_t unit_month_t;

typedef struct cts_date_time_tag
{
    uint16_t        year;
    unit_month_t    month;
    uint8_t         day;
    uint8_t         hours;
    uint8_t         minutes;
    uint8_t         seconds;
} cts_date_time_t;

typedef enum
{
    CTS_MONDAY,
    CTS_TUESDAY,
    CTS_WEDNESDAY,
    CTS_THURSDAY,
    CTS_FRIDAY,
    CTS_SATURDAY,
    CTS_SUNDAY
} cts_day_of_week_tag;

typedef uint8_t cts_day_of_week_t;

typedef struct cts_day_date_time_tag
{
    cts_date_time_t       date_time;
    cts_day_of_week_tag   day_of_week;
} cts_day_date_time_t;

typedef uint8_t cts_fractions256_t;

typedef struct cts_exact_time256_tag
{
    cts_day_date_time_t    cts_day_date_time;
    cts_fractions256_t     fractions;
} cts_exact_time256_t;

typedef uint8_t cts_adjust_reason_t;

typedef struct cts_measurement_tag
{
    cts_exact_time256_t   exact_time;
    cts_adjust_reason_t   adjust_reason;
} cts_measurement_t;

typedef enum
{
    UTCm1200 = -48,    UTCm1100 = -44,    UTCm1000 = -40,    UTCm0930 = -38,
    UTCm0900 = -36,    UTCm0800 = -32,    UTCm0700 = -28,    UTCm0600 = -24,
    UTCm0500 = -20,    UTCm0430 = -18,    UTCm0400 = -16,    UTCm0330 = -14,
    UTCm0300 = -12,    UTCm0200 = -8,     UTCm0100 = -4,     UTCp0000 = 0,
    UTCp0100 = 4,      UTCp0200 = 8,      UTCp0300 = 12,     UTCp0330 = 14,
    UTCp0400 = 16,     UTCp0430 = 18,     UTCp0500 = 20,     UTCp0530 = 22,
    UTCp0545 = 23,     UTCp0600 = 24,     UTCp0630 = 26,     UTCp0700 = 28,
    UTCp0800 = 32,     UTCp0845 = 35,     UTCp0900 = 36,     UTCp0930 = 38,
    UTCp1000 = 40,     UTCp1030 = 42,     UTCp1100 = 44,     UTCp1130 = 46,
    UTCp1200 = 48,     UTCp1245 = 51,     UTCp1300 = 52,     UTCp1400 = 56,
    CTS_OFFSET_NOT_KNOWN = -128
} cts_time_zone_tag;

typedef uint8_t cts_time_zone_t;

typedef enum
{
    CTS_STANDARD_TIME            = 0U,
    CTS_HALF_HOUR_DAYLIGHT_TIME  = BIT1,
    CTS_DAYLIGHT_TIME            = BIT2,
    CTS_DOUBLE_DAYLIGHT_TIME     = BIT3,
    CTS_DST_NOT_KNOWN            = 255U
} cts_dst_offset_tag;

typedef uint8_t cts_dst_offset_t;

typedef struct cts_local_time_info_tag
{
    cts_time_zone_t    cts_time_zone;
    cts_dst_offset_tag cts_dst_offset;
} cts_local_time_info_t;

typedef uint8_t cts_time_source_t;

typedef uint8_t cts_accuracy_t;

typedef struct cts_reference_time_info_tag
{
    cts_time_source_t    source;
    cts_accuracy_t       accuracy;
    uint8_t              days_since_update;
    uint8_t              hours_since_update;
} cts_reference_time_info_t;

typedef void (*cts_notification_enabled_cb)(void);

/* NTDS Data */
typedef enum
{
    NDCS_STANDARD_TIME          = 0U,
    NDCS_HALF_DAYLIGHT_TIME     = 2U,
    NDCS_DAYLIGHT_TIME          = 4U,
    NDCS_DOUBLE_DAYLIGHT_TIME   = 8U,
    NDCS_DST_UNKNOWN            = 255U
} dst_offset_tag;

typedef uint8_t dst_offset_t;

typedef struct ndcs_time_with_dst_tag
{
    cts_date_time_t  date;
    dst_offset_t offset;
} ndcs_time_with_dst_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/** @brief Write Time Update State locally.
*
*  @return none.
*/
void bt_rtus_set_time_update_state(time_update_state_t time_update_state);

/** @brief Read Time Update State locally.
*
*  @return Time update state value.
*/
time_update_state_t bt_rtus_get_time_update_state(void);

/** @brief Write Time Update Control Point locally.
*
*  @return none.
*/
void bt_rtus_set_time_update_control_point(time_update_command_t command);

/** @brief Read Time Update Control Point command locally.
*
*  @return Time update Control Point command value.
*/
time_update_command_t bt_rtus_get_time_update_control_point(void);

/** @brief Write Current Time Value.
*
*  @return none.
*/
void bt_cts_set_current_time(struct bt_conn *conn, cts_measurement_t current_time, uint32_t local_time);

/** @brief Read the Current Time value.
*
*  @return Current time value.
*/
cts_measurement_t bt_cts_get_current_time(void);

/** @brief Write Local Time Value.
*
*  @return none.
*/
void bt_cts_set_local_time(cts_local_time_info_t local_time);

/** @brief Read the Local Time value.
*
*  @return Local time value.
*/
cts_local_time_info_t bt_cts_get_local_time(void);

/** @brief Write Reference Time Value.
*
*  @return none.
*/
void bt_cts_set_reference_time(cts_reference_time_info_t reference_time);

/** @brief Read the Reference Time value.
*
*  @return Reference time value.
*/
cts_reference_time_info_t bt_cts_get_reference_time(void);

/** @brief Inform peer of the Current Time Value.
*
*  @return none.
*/
void cts_notify(struct bt_conn *conn);

/** @brief Application callback to be called when notifications are enabled
*
*  @return none.
*/
void bt_cts_register_notification_callback(cts_notification_enabled_cb notif_callback);

/** @brief Write Time with DST value.
*
*  @return none.
*/
void bt_ndcs_set_time_with_dst(ndcs_time_with_dst_t time_with_dst);

/** @brief Read Time with DST value.
*
*  @return Time with DST value.
*/
ndcs_time_with_dst_t bt_ndcs_get_time_with_dst(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_TIP_H_ */
