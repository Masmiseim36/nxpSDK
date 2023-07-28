/*
 * Copyright 2021, 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "bt_ble_platform.h"
#ifndef BT_BLE_PLATFORM_INIT_ESCAPE
#include "host_msd_fatfs.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the platform */
void bt_ble_platform_init(void)
{
#ifndef BT_BLE_PLATFORM_INIT_ESCAPE
    (void)USB_HostMsdFatfsInit();
#endif /* BT_BLE_PLATFORM_INIT_ESCAPE */
}
