/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016, 2018, 2020 - 2021, 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HOST_MSD_FATFS_H_
#define _HOST_MSD_FATFS_H_

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_msd.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef CONFIG_BT_HOST_USB_ENABLE
#define CONFIG_BT_HOST_USB_ENABLE 0
#endif /* CONFIG_BT_HOST_USB_ENABLE */

/*******************************************************************************
 * API
 ******************************************************************************/

int USB_HostMsdFatfsInit(void);

#endif /* _HOST_MSD_FATFS_H_ */
