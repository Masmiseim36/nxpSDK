/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USB_DEVICE_DESCRIPTOR_H_
#define _USB_DEVICE_DESCRIPTOR_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define USB_DEVICE_SPECIFIC_BCD_VERSION (0x0200U)
#define USB_DEVICE_DEMO_BCD_VERSION (0x0101U)
#define USB_DEVICE_MAX_POWER (0x32U)

#define USB_DEVICE_VID (0x1FC9U)
#define USB_DEVICE_PID (0x0096U)

/*! @brief USB device class code */
#define USB_DEVICE_CLASS (0x00U)
#define USB_DEVICE_SUBCLASS (0x00U)
#define USB_DEVICE_PROTOCOL (0x00U)

/*! @brief PHDC class code */
#define USB_PHDC_CLASS (0x0FU)
#define USB_PHDC_SUBCLASS (0x00U)
#define USB_PHDC_PROTOCOL (0x00U)

/*! @brief Size of descriptor in bytes */
#define USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL (sizeof(g_UsbDeviceConfigurationDescriptor))
#define USB_DESCRIPTOR_LENGTH_CLASS_FUNCTION (4U)
#define USB_DESCRIPTOR_LENGTH_QOS (4U)
#define USB_DESCRIPTOR_LENGTH_METADATA_BULK_OUT (4U)
#define USB_DESCRIPTOR_LENGTH_METADATA_BULK_IN (7U)
#define USB_DESCRIPTOR_LENGTH_FUNCTION_EXTENSION (6U)
#define USB_DESCRIPTOR_LENGTH_STRING0 (sizeof(g_UsbDeviceString0))
#define USB_DESCRIPTOR_LENGTH_STRING1 (sizeof(g_UsbDeviceString1))
#define USB_DESCRIPTOR_LENGTH_STRING2 (sizeof(g_UsbDeviceString2))
#define USB_DESCRIPTOR_LENGTH_STRING_ERROR (sizeof(g_UsbDeviceStringN))

/*! @brief PHDC descriptor types */
#define USB_DESCRIPTOR_TYPE_CLASS_FUNCTION (0x20U)
#define USB_DESCRIPTOR_TYPE_QOS (0x21U)
#define USB_DESCRIPTOR_TYPE_METADATA (0x22U)
#define USB_DESCRIPTOR_TYPE_11073PHD_FUNCTION (0x30U)

/*! @brief PHDC endpoint number */
#define USB_PHDC_WEIGHT_SCALE_ENDPOINT_COUNT (3U)
#define USB_PHDC_BULK_ENDPOINT_OUT (2U)
#define USB_PHDC_BULK_ENDPOINT_IN (3U)
#define USB_PHDC_INTERRUPT_ENDPOINT_IN (1U)

/*! @brief Max endpoint packet size */
#define HS_USB_PHDC_BULK_ENDPOINT_IN_PACKET_SIZE (64U)
#define HS_USB_PHDC_BULK_ENDPOINT_OUT_PACKET_SIZE (64U)
#define FS_USB_PHDC_BULK_ENDPOINT_IN_PACKET_SIZE (64U)
#define FS_USB_PHDC_BULK_ENDPOINT_OUT_PACKET_SIZE (64U)
#define HS_USB_PHDC_INTERRUPT_ENDPOINT_IN_PACKET_SIZE (8U)
#define FS_USB_PHDC_INTERRUPT_ENDPOINT_IN_PACKET_SIZE (8U)
#define HS_USB_PHDC_INTERRUPT_ENDPOINT_IN_INTERVAL (0x04U)
#define FS_USB_PHDC_INTERRUPT_ENDPOINT_IN_INTERVAL (0x01U)

/*! @brief Endpoint QoS */
#define USB_PHDC_WEIGHT_SCALE_BULK_OUT_QOS (0x08U)
#define USB_PHDC_WEIGHT_SCALE_BULK_IN_QOS (0x08U)
#define USB_PHDC_WEIGHT_SCALE_INTERRUPT_IN_QOS (0x01U)

/*! @brief Meta-data message preamble feature */
#define META_DATA_MESSAGE_PREAMBLE_IMPLEMENTED (0U)

/*! @brief Number of USB device string descriptor */
#define USB_DEVICE_STRING_COUNT (3U)
/*! @brief Number of USB device language */
#define USB_DEVICE_LANGUAGE_COUNT (1U)

/*! @brief PHDC interface */
#define USB_PHDC_WEIGHT_SCALE_INTERFACE_COUNT (0x01U)
#define USB_PHDC_WEIGHT_SCALE_INTERFACE_INDEX (0x00U)

/*! @brief PHDC configuration */
#define USB_DEVICE_CONFIGURATION_COUNT (1U)
#define USB_PHDC_WEIGHT_SCALE_CONFIGURE_INDEX (1U)

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief USB device set speed function.
 *
 * Due to the difference of HS and FS descriptors, the device descriptors and
 * configurations need to be updated to match current speed. As the default,
 * the device descriptors and configurations are configured by using FS parameters
 * for both EHCI and KHCI. When the EHCI is enabled, the application needs to call
 * this function to update device by using current speed. The updated information
 * includes endpoint max packet size, endpoint interval, etc..
 *
 * @param speed Speed type. USB_SPEED_HIGH/USB_SPEED_FULL/USB_SPEED_LOW.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceSetSpeed(uint8_t speed);

/*!
 * @brief device callback function.
 * This function handles the usb standard event. more information, please refer to usb spec chapter 9.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return kStatus_USB_Success or error.
 */
extern usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

#if defined(__cplusplus)
}
#endif
#endif /* _USB_DEVICE_DESCRIPTOR_H_ */
