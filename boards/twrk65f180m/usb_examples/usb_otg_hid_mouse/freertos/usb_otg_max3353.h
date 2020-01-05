/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_OTG_MAX3353_H__
#define __USB_OTG_MAX3353_H__

#include "usb_otg_config.h"

#if ((defined USB_OTG_KHCI_PERIPHERAL_ENABLE) && (USB_OTG_KHCI_PERIPHERAL_ENABLE))

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* MAX3353 registers address */
#define MAX3353_CONTROL1_ADDRESS 0x10
#define MAX3353_CONTROL2_ADDRESS 0x11
#define MAX3353_STATUS_ADDRESS 0x13
#define MAX3353_INTERRUPT_MASK_ADDRESS 0x14
#define MAX3353_INTERRUPT_EDGE_ADDRESS 0x15
#define MAX3353_INTERRUPT_LATCH_ADDRESS 0x16

/* status register bit */
#define MAX3353_STATUS_VBUS_VALID_MASK (0x01U)
#define MAX3353_STATUS_SESSION_VALID_MASK (0x02U)
#define MAX3353_STATUS_SESSION_END_MASK (0x04U)
#define MAX3353_STATUS_ID_GND_MASK (0x08U)
#define MAX3353_STATUS_ID_FLOAT_MASK (0x10U)
#define MAX3353_STATUS_A_HNP_MASK (0x20U)
#define MAX3353_STATUS_B_HNP_MASK (0x40U)

#define MAX3353_STATUS_VBUS_VALID_BIT (0U)
#define MAX3353_STATUS_SESSION_VALID_BIT (1U)
#define MAX3353_STATUS_SESSION_END_BIT (2U)
#define MAX3353_STATUS_ID_GND_BIT (3U)
#define MAX3353_STATUS_ID_FLOAT_BIT (4U)
#define MAX3353_STATUS_A_HNP_BIT (5U)
#define MAX3353_STATUS_B_HNP_BIT (6U)

/* control2 register bit */
#define MAX3353_CONTROL2_SDWN_MASK (0x01U)
#define MAX3353_CONTROL2_VBUS_CHG1_MASK (0x02U)
#define MAX3353_CONTROL2_VBUS_CHG2_MASK (0x04U)
#define MAX3353_CONTROL2_VBUS_DRV_MASK (0x08U)
#define MAX3353_CONTROL2_VBUS_DISCHG_MASK (0x10U)

/* control1 register bit */
#define MAX3353_CONTROL1_IRQ_MODE_MASK (0x02U)
#define MAX3353_CONTROL1_BDISC_ACONN_MASK (0x04U)
#define MAX3353_CONTROL1_DP_PULLUP_MASK (0x10U)
#define MAX3353_CONTROL1_DM_PULLUP_MASK (0x20U)
#define MAX3353_CONTROL1_DP_PULLDWN_MASK (0x40U)
#define MAX3353_CONTROL1_DM_PULLDWN_MASK (0x80U)

/* The status for controling max3353 */
typedef struct _usb_otg_max3353_run
{
    uint8_t statusRegister;
    uint8_t control1Register;
    uint8_t control2Register;
    uint8_t peripheralStatus;
} usb_otg_max3353_run_t;
/*******************************************************************************
 * API
 ******************************************************************************/

/* Initialize max3353, call from application */
extern usb_status_t USB_OtgMax3353Init(void);

#endif

#endif
