/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_I2C_DRV_H__
#define __USB_I2C_DRV_H__

extern usb_status_t USB_I2CMasterInit(void *base, uint32_t baudRate, uint32_t clockSource);

extern usb_status_t USB_I2CMasterDeinit(void *base);

extern usb_status_t USB_I2CMasterWriteRegister(void *base,
                                               uint8_t slaveAddress,
                                               uint8_t registerAddress,
                                               uint8_t value);

extern usb_status_t USB_I2CMasterReadRegister(void *base,
                                              uint8_t slaveAddress,
                                              uint8_t registerAddress,
                                              uint8_t *value);

#endif
