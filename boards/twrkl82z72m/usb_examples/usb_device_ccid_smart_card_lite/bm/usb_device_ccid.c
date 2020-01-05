/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#if ((defined(USB_DEVICE_CONFIG_CCID)) && (USB_DEVICE_CONFIG_CCID > 0U))
#include "usb_device_ccid.h"

#endif
