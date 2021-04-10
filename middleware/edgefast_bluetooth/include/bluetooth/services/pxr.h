/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PXR_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PXR_H_

#include <zephyr/types.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>

/**
 * @brief Proximity Reporter (PXR)
 * @defgroup bt_pxr Proximity Reporter (PXR)
 * @ingroup bluetooth
 * @{
 *
 */

typedef void (*alert_ui_cb)(uint8_t param);

enum
{
    NO_ALERT      = 0x00,
    MILD_ALERT    = 0x01,
    HIGH_ALERT    = 0x02
};

#ifdef __cplusplus
extern "C" {
#endif

 /** @brief IAS Alert Level GATT write callback.
 *
 *  If called with conn == NULL, it is a local write.
 *
 *  @return Number of bytes written.
 */
ssize_t write_ias_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

 /** @brief IAS Alert Level GATT read callback.
 *
 *  @return Number of bytes read.
 */
ssize_t read_lls_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);

 /** @brief LLS Alert Level GATT write callback.
 *
 *  If called with conn == NULL, it is a local write.
 *
 *  @return Number of bytes written.
 */
ssize_t write_lls_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

 /** @brief TPS Power Level GATT read callback.
 *
 *  @return Number of bytes read.
 */
ssize_t read_tps_power_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);

 /** @brief TPS Power Level Descriptor GATT read callback.
 *
 *  @return Number of bytes read.
 */
ssize_t read_tps_power_level_desc(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);

 /** @brief Read LLS Alert Level locally.
 *
 *  @return Number of bytes written.
 */
uint8_t pxr_lls_get_alert_level(void);

 /** @brief Read IAS Alert Level locally.
 *
 *  @return Number of bytes written.
 */
uint8_t pxr_ias_get_alert_level(void);

 /** @brief Read TPS Power Level locally.
 *
 *  @return Number of bytes written.
 */
int8_t  pxr_tps_get_power_level(void);

 /** @brief Write TPS Power Level locally.
 *
 *  @return Number of bytes written.
 */
void pxr_tps_set_power_level(int8_t power_level);

 /** @brief Initialize PXR Service.
 *
 *  @return Success or error.
 */
int pxr_init(alert_ui_cb cb);

 /** @brief Deinitialize PXR Service.
 *
 *  @return Success or error.
 */
int pxr_deinit(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PXR_H_ */