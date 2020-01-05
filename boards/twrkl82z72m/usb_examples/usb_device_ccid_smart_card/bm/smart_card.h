/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_CCID_SMART_CARD_H__
#define __USB_CCID_SMART_CARD_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif

#if defined(__GIC_PRIO_BITS)
#define USB_DEVICE_INTERRUPT_PRIORITY (25U)
#else
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
#endif

#define USB_DEVICE_CCID_BUFFER_4BYTE_ALIGN(n) (((n - 1U) & 0xFFFFFFFCU) + 0x00000004U)

#define USB_DEVICE_CCID_ATR_BUFFER_LENGTH USB_DEVICE_CCID_BUFFER_4BYTE_ALIGN(USB_DEVICE_CONFIG_CCID_MAX_MESSAGE_LENGTH)
#define USB_DEVICE_CCID_COMMAND_BUFFER_LENGTH \
    USB_DEVICE_CCID_BUFFER_4BYTE_ALIGN(USB_DEVICE_CONFIG_CCID_MAX_MESSAGE_LENGTH)
#define USB_DEVICE_CCID_RESPONSE_BUFFER_LENGTH \
    USB_DEVICE_CCID_BUFFER_4BYTE_ALIGN(USB_DEVICE_CONFIG_CCID_MAX_MESSAGE_LENGTH)

#define USB_DEVICE_CCID_PARAMETER_BUFFER_LENGTH \
    USB_DEVICE_CCID_BUFFER_4BYTE_ALIGN(sizeof(usb_device_ccid_set_parameters_command_common_t))

typedef struct _usb_ccid_smart_card_struct
{
    usb_device_handle deviceHandle;
    class_handle_t classHandle;
    uint32_t clockFrequency;
    uint32_t dataRate;
    uint8_t slotsAtrBuffer[USB_DEVICE_CCID_SMART_CARD_MAX_SLOTS][USB_DEVICE_CCID_ATR_BUFFER_LENGTH];
    uint8_t slotsCommandBuffer[USB_DEVICE_CCID_SMART_CARD_MAX_SLOTS][USB_DEVICE_CCID_COMMAND_BUFFER_LENGTH];
    uint8_t slotsResponseBuffer[USB_DEVICE_CCID_SMART_CARD_MAX_SLOTS + 1U][USB_DEVICE_CCID_RESPONSE_BUFFER_LENGTH];
    uint8_t slotsCurrentParameter[USB_DEVICE_CCID_SMART_CARD_MAX_SLOTS][USB_DEVICE_CCID_PARAMETER_BUFFER_LENGTH];
    uint8_t clockStatus;
    uint8_t protocol;
    uint8_t nodeAddress;
    uint8_t remoteWakeup;
    uint8_t suspend;
    uint8_t speed;
    uint8_t attach;
    uint8_t powerOn;
    uint8_t present;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_DEVICE_CCID_SMART_CARD_INTERFACE_COUNT];
} usb_ccid_smart_card_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#endif /* __USB_CCID_SMART_CARD_H__ */
