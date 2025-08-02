/**
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_DEVICE_CDC_ECM_H__
#define __USB_DEVICE_CDC_ECM_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define USB_DEVICE_CDC_COMM_CLASS_CODE (0x02U)
#define USB_DEVICE_CDC_DATA_CLASS_CODE (0x0AU)
#define USB_DEVICE_CDC_ECM_CLASS_CODE (0x02U)
#define USB_DEVICE_CDC_ECM_SUBCLASS_CODE (0x06U)
#define USB_DEVICE_CDC_ECM_PROTOCOL_CODE (0x00U)

#define USB_DEVICE_CDC_NETWORK_CONNECTION (0x00U)
#define USB_DEVICE_CDC_CONNECTION_SPEED_CHANGE (0x2AU)
#define USB_DEVICE_CDC_ECM_SET_ETHERNET_MULTICAST_FILTER (0x40U)
#define USB_DEVICE_CDC_ECM_SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER (0x41U)
#define USB_DEVICE_CDC_ECM_GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER (0x42U)
#define USB_DEVICE_CDC_ECM_SET_ETHERNET_PACKET_FILTER (0x43U)
#define USB_DEVICE_CDC_ECM_GET_ETHERNET_STATISTIC (0x44U)

#define USB_DEVICE_CDC_ECM_PACKET_TYPE_MULTICAST_MASK (1U << 4)
#define USB_DEVICE_CDC_ECM_PACKET_TYPE_BROADCAST_MASK (1U << 3)
#define USB_DEVICE_CDC_ECM_PACKET_TYPE_DIRECTED_MASK (1U << 2)
#define USB_DEVICE_CDC_ECM_PACKET_TYPE_ALL_MULTICAST_MASK (1U << 1)
#define USB_DEVICE_CDC_ECM_PACKET_TYPE_PROMISCUOUS_MASK (1U << 0)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
typedef struct _usb_device_cdc_ecm_pipe_status
{
    uint8_t bulkInIsBusy;
    uint8_t bulkOutIsBusy;
    uint8_t interruptInIsBusy;
} usb_device_cdc_ecm_pipe_status_t;

usb_status_t USB_DeviceCdcEcmBulkIn(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam);

usb_status_t USB_DeviceCdcEcmBulkOut(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam);

usb_status_t USB_DeviceCdcEcmInterruptIn(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam);

usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle, usb_setup_struct_t *setup, uint32_t *length, uint8_t **buffer);

usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer);

usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle, usb_setup_struct_t *setup, uint32_t *length, uint8_t **buffer);

usb_status_t USB_DeviceCdcEcmSend(usb_device_handle handle, uint8_t ep, uint8_t *buffer, uint32_t length);

usb_status_t USB_DeviceCdcEcmRecv(usb_device_handle handle, uint8_t ep, uint8_t *buffer, uint32_t length);
#endif
