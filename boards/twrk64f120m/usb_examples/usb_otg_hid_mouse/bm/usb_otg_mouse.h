/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_OTG_MOUSE_H__
#define __USB_OTG_MOUSE_H__

typedef enum _usb_mouse_state_type
{
    kState_None = 0U,
    kState_Host,
    kState_Device,
} usb_mouse_state_type_t;

/*! @brief otg app run status */
typedef enum _usb_otg_mouse_run_state
{
    kUSB_HostOtgRunIdle = 0, /*!< idle */
} usb_otg_mouse_run_state_t;

/*! @brief USB otg mouse instance structure */
typedef struct _usb_otg_mouse_instance
{
    usb_host_handle hostHandle;
    usb_device_handle deviceHandle;
    uint32_t otgStateMachine; /*!< otg stack's state */
    uint8_t otgMouseState;    /*!< host or device as #usb_mouse_state_type_t */
    uint8_t runState;         /*!< otg application running state as #usb_otg_mouse_run_state_t */
    uint8_t aSetBHNPEnable;   /*!< a_set_b_hnp_enable is send */
} usb_otg_mouse_instance_t;

#endif
