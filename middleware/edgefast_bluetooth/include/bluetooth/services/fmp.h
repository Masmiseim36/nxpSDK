/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_FMP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_FMP_H_

#include <zephyr/types.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>

/**
 * @brief Find Me Profile (FMP)
 * @defgroup bt_fmp (FMP)
 * @ingroup bluetooth
 * @{
 *
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef void (*alert_ui_cb)(uint8_t param);

enum
{
    NO_ALERT      = 0x00,
    MILD_ALERT    = 0x01,
    HIGH_ALERT    = 0x02
};

/*******************************************************************************
 * Prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/** @brief Initialize FMP Service.
 *
 *  @return Success or error.
 */
int fmp_init(alert_ui_cb cb);

 /** @brief Read IAS Alert Level locally.
 *
 *  @return The local alert level value.
 */
uint8_t fmp_ias_get_alert_level(void);

 /** @brief Deinitialize FMP Service.
 *
 *  @return Success or error.
 */
int fmp_deinit(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_FMP_H_ */