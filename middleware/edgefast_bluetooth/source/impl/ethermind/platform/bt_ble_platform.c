/*
 * Copyright 2021 NXP
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
#if (defined(CONFIG_BT_SNOOP) && (CONFIG_BT_SNOOP > 0))
	(void)USB_HostMsdFatfsInit();
#endif
#endif
}
