/**
 * Copyright 2024 - 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_H__
#define __APP_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#endif

#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif
#endif

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

#define APP_ETH_LINK_CHECK_INTERVAL_MS (200U)
#define APP_ETH_FRAME_MAX_LENGTH (1514U)

#define APP_ETH_NIC_EVENT_SET(event, offset) (event |= (uint32_t)(1U << offset))
#define APP_ETH_NIC_EVENT_UNSET(event, offset) (event &= ~((uint32_t)(1U << offset)))
#define APP_ETH_NIC_EVENT_GET(event, offset) (event & (uint32_t)(1U << offset))
#define APP_ETH_NIC_EVENT_CLEAR(event) (event &= 0)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Define the types for application */
typedef struct _usb_eth_nic_struct
{
    usb_device_handle deviceHandle;
    class_handle_t cdcEcmHandle;
    eth_adapter_handle_t *ethHandle;
    uint8_t configuration;
    uint8_t interfaceAltSetting[USB_DEVICE_CDC_ECM_INTERFACE_COUNT];
    volatile uint8_t attachStatus;
    uint8_t deviceSpeed;
    volatile uint8_t linkStatus;
    uint32_t linkSpeed;
} usb_eth_nic_t;

typedef enum _usb_eth_nic_event_enum
{
    kAPP_CheckLinkChange,
    kAPP_NotifyNetworkChange,
    kAPP_UsbDataXfer,
} usb_eth_nic_event_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern usb_eth_nic_t ethNicHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
