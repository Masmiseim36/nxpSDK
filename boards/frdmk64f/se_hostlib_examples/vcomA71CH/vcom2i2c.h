/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef VCOM2I2C_H
#define VCOM2I2C_H

#include <sm_types.h>

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "fsl_debug_console.h"

#include "usb_device_descriptor.h"
#include "virtual_com.h"

//+ A71CH I2C
#define RJCT_OK       0x0000
#define RJCT_ARG_FAIL 0x6000

#define WAIT_FOR_CARD 0
#define APDU_DATA 1
//- A71CH I2C

U16 vcomPackageApduResponse(
    U8 messageType, U8 nodeAddress, U8* payload, U16 payloadLen, U8 *targetBuf, U16 *targetBufLen);
U16 SM_SendAPDUVcom(
    U8 *cmd, U16 cmdLen, U8 *resp, U16 *respLen);
void state_vcom_read_write(
    usb_cdc_vcom_struct_t * p_cdcVcom, volatile uint32_t * p_recvSize, const uint8_t s_currRecvBuf[DATA_BUFF_SIZE]);
void state_vcom_2_i2c(void);
void state_i2c_2_vcom(void);
void state_vcom_usbLowPower(void);

#endif /* VCOM2I2C_H */
