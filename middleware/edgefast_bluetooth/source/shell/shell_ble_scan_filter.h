/** @file
 * @brief Bluetooth shell module
 *
 * Provides ble scan filter functions.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SHELL_BLE_SCAN_FILTER_H
#define __SHELL_BLE_SCAN_FILTER_H

#if (defined(CONFIG_BLE_ADV_REPORT_BUFFER_FILTER) && (CONFIG_BLE_ADV_REPORT_BUFFER_FILTER > 0U))
int shell_le_init_adv_list(void);
int shell_le_scan_filter_handler(const struct bt_le_scan_recv_info *info);
#endif /* CONFIG_BLE_ADV_REPORT_BUFFER_FILTER */

#endif /* __SHELL_BLE_SCAN_FILTER_H */
