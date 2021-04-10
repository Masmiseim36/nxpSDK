/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016, 2018, 2020 - 2021 NXP
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

#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif

typedef enum _usb_host_instance
{
    kUSB_HostInstanceEhci0, /*!< The instance is ehci 0 */
    kUSB_HostInstanceEhci1, /*!< The instance is ehci 1 */
} usb_host_instance_t;

/*! @brief USB host msd configuration structure */
typedef struct _usb_host_config
{
    usb_host_instance_t controllerId;
} usb_host_config_t;

/*! @brief USB host msd configuration structure */
typedef struct _usb_host_phy_config
{
    uint32_t inputClockfreq; /* the external input clock.
                              * for example: if the external input clock is 16M, the parameter freq should be 16000000. */
    uint8_t D_CAL;     /* Decode to trim the nominal 17.78mA current source */
    uint8_t TXCAL45DP; /* Decode to trim the nominal 45-Ohm series termination resistance to the USB_DP output pin */
    uint8_t TXCAL45DM; /* Decode to trim the nominal 45-Ohm series termination resistance to the USB_DM output pin */
} usb_host_phy_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

int USB_HostMsdFatfsInit(void);

#endif /* _HOST_MSD_FATFS_H_ */
