/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_DEVICE_DESCRIPTOR_H__
#define __USB_DEVICE_DESCRIPTOR_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define USB_DEVICE_SPECIFIC_BCD_VERSION (0x0200U)
#define USB_DEVICE_DEMO_BCD_VERSION     (0x0101U)

#define USB_DEVICE_MAX_POWER (0x32U)

/* usb descriptor length */
#define USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL      (sizeof(g_UsbDeviceConfigurationDescriptor))
#define USB_ENDPOINT_AUDIO_DESCRIPTOR_LENGTH         (9)
#define USB_DESCRIPTOR_LENGTH_HID                    (9U)
#define USB_DESCRIPTOR_LENGTH_HID_GENERIC_REPORT     (sizeof(g_UsbDeviceHidGenericReportDescriptor))
#define USB_DESCRIPTOR_LENGTH_AC_INTERRUPT_ENDPOINT  (9)
#define USB_AUDIO_CONTROL_INTERFACE_HEADER_LENGTH    (10)
#define USB_AUDIO_INPUT_TERMINAL_ONLY_DESC_SIZE      (12)
#define USB_AUDIO_OUTPUT_TERMINAL_ONLY_DESC_SIZE     (9)
#define USB_AUDIO_FEATURE_UNIT_ONLY_DESC_SIZE(ch, n) (0x07 + (ch + 1) * n)
#define USB_AUDIO_STREAMING_IFACE_DESC_SIZE          (7)
#define USB_AUDIO_STREAMING_ENDP_DESC_SIZE           (7)
#define USB_AUDIO_STREAMING_TYPE_I_DESC_SIZE         (11)

/* Configuration, interface and endpoint. */
#define USB_DEVICE_CONFIGURATION_COUNT (1)
#define USB_DEVICE_STRING_COUNT        (3)
#define USB_DEVICE_LANGUAGE_COUNT      (1)
#define USB_DEVICE_INTERFACE_COUNT     (4)
#define USB_AUDIO_ENDPOINT_COUNT       (1)

#define USB_AUDIO_SPEAKER_CONFIGURE_INDEX         (1)
#define USB_COMPOSITE_CONFIGURE_INDEX             (1)
#define USB_AUDIO_CONTROL_INTERFACE_INDEX         (0)
#define USB_AUDIO_RECORDER_STREAM_INTERFACE_INDEX (1)
#define USB_AUDIO_SPEAKER_STREAM_INTERFACE_INDEX  (2)
//#define USB_HID_KEYBOARD_INTERFACE_INDEX (3)

#define USB_AUDIO_SPEAKER_STREAM_ENDPOINT_COUNT  (2)
#define USB_AUDIO_CONTROL_ENDPOINT_COUNT         (1)
#define USB_AUDIO_RECORDER_STREAM_ENDPOINT_COUNT (1)
//#define USB_HID_KEYBOARD_ENDPOINT_COUNT (1)

#define USB_AUDIO_SPEAKER_STREAM_ENDPOINT   (2)
#define USB_AUDIO_CONTROL_ENDPOINT          (1)
#define USB_AUDIO_SPEAKER_FEEDBACK_ENDPOINT (2)
#define USB_AUDIO_RECORDER_STREAM_ENDPOINT  (3)
//#define USB_HID_KEYBOARD_ENDPOINT (4)

#define USB_HID_GENERIC_ENDPOINT_IN  (4U)
#define USB_HID_GENERIC_ENDPOINT_OUT (4U)

#define USB_AUDIO_COMPOSITE_INTERFACE_COUNT                                                 \
    (USB_AUDIO_SPEAKER_CONTROL_INTERFACE_COUNT + USB_AUDIO_SPEAKER_STREAM_INTERFACE_COUNT + \
     USB_AUDIO_RECORDER_STREAM_INTERFACE_COUNT)
#define USB_AUDIO_SPEAKER_CONTROL_INTERFACE_COUNT (1)
#define USB_AUDIO_SPEAKER_STREAM_INTERFACE_COUNT  (1)
#define USB_AUDIO_RECORDER_STREAM_INTERFACE_COUNT (1)
//#define USB_HID_KEYBOARD_INTERFACE_COUNT (1)
#define USB_COMPOSITE_INTERFACE_COUNT (4)

/* Audio data format */
#define AUDIO_FORMAT_CHANNELS (0x02)
#define AUDIO_FORMAT_BITS     (16)
#define AUDIO_FORMAT_SIZE     (0x02)

/* Packet size and interval. */
#define HS_AUDIO_INTERRUPT_IN_PACKET_SIZE (8)
#define FS_AUDIO_INTERRUPT_IN_PACKET_SIZE (8)
#define HS_AUDIO_INTERRUPT_IN_INTERVAL    (0x07U) /* 2^(7-1) = 8ms */
#define FS_AUDIO_INTERRUPT_IN_INTERVAL    (0x08U)
#define HS_ISO_OUT_ENDP_PACKET_SIZE                    \
    (AUDIO_SAMPLING_RATE_KHZ * AUDIO_FORMAT_CHANNELS * \
     AUDIO_FORMAT_SIZE) /* This should be changed to 192 if sampling rate is 48k */
#define FS_ISO_OUT_ENDP_PACKET_SIZE      (AUDIO_SAMPLING_RATE_KHZ * AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE)
#define HS_ISO_IN_ENDP_PACKET_SIZE       (AUDIO_SAMPLING_RATE_KHZ * AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE)
#define FS_ISO_IN_ENDP_PACKET_SIZE       (AUDIO_SAMPLING_RATE_KHZ * AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE)
#define HS_ISO_FEEDBACK_ENDP_PACKET_SIZE (4)
#define FS_ISO_FEEDBACK_ENDP_PACKET_SIZE (3)
#define HS_ISO_OUT_ENDP_INTERVAL         (0x04)
#define HS_ISO_IN_ENDP_INTERVAL          (0x04)
#define FS_ISO_OUT_ENDP_INTERVAL         (0x01)
#define FS_ISO_IN_ENDP_INTERVAL          (0x01)
#define ISO_OUT_ENDP_INTERVAL            (0x01)
//#define HS_HID_KEYBOARD_INTERRUPT_IN_PACKET_SIZE (8U)
//#define FS_HID_KEYBOARD_INTERRUPT_IN_PACKET_SIZE (8U)
//#define HS_HID_KEYBOARD_INTERRUPT_IN_INTERVAL (0x10U) /* 2^(6-1) = 4ms */
//#define FS_HID_KEYBOARD_INTERRUPT_IN_INTERVAL (0x10U)

/* String descriptor length. */
#define USB_DESCRIPTOR_LENGTH_STRING0 (sizeof(g_UsbDeviceString0))
#define USB_DESCRIPTOR_LENGTH_STRING1 (sizeof(g_UsbDeviceString1))
#define USB_DESCRIPTOR_LENGTH_STRING2 (sizeof(g_UsbDeviceString2))

/* Class code. */
#define USB_DEVICE_CLASS    (0x00)
#define USB_DEVICE_SUBCLASS (0x00)
#define USB_DEVICE_PROTOCOL (0x00)

#define USB_AUDIO_CLASS           (0x01)
#define USB_SUBCLASS_AUDIOCONTROL (0x01)
#define USB_SUBCLASS_AUDIOSTREAM  (0x02)
#define USB_AUDIO_PROTOCOL        (0x00)

//#define USB_HID_KEYBOARD_CLASS (0x03)
//#define USB_HID_KEYBOARD_SUBCLASS (0x00)
//#define USB_HID_KEYBOARD_PROTOCOL (0x00)

#define USB_AUDIO_FORMAT_TYPE_I                 (0x01)
#define USB_AUDIO_STREAM_ENDPOINT_DESCRIPTOR    (0x25)
#define USB_AUDIO_EP_GENERAL_DESCRIPTOR_SUBTYPE (0x01)

#define USB_AUDIO_RECORDER_CONTROL_INPUT_TERMINAL_ID  (0x01)
#define USB_AUDIO_RECORDER_CONTROL_FEATURE_UNIT_ID    (0x02)
#define USB_AUDIO_RECORDER_CONTROL_OUTPUT_TERMINAL_ID (0x03)

#define USB_AUDIO_SPEAKER_CONTROL_INPUT_TERMINAL_ID  (0x04)
#define USB_AUDIO_SPEAKER_CONTROL_FEATURE_UNIT_ID    (0x05)
#define USB_AUDIO_SPEAKER_CONTROL_OUTPUT_TERMINAL_ID (0x06)

//#define USB_HID_KEYBOARD_REPORT_LENGTH (0x01U)
// HID generic
#define USB_HID_GENERIC_CONFIGURE_INDEX (1U)
#define USB_HID_GENERIC_INTERFACE_COUNT (1U)

#define USB_HID_GENERIC_IN_BUFFER_LENGTH  (8U)
#define USB_HID_GENERIC_OUT_BUFFER_LENGTH (8U)
#define USB_HID_GENERIC_ENDPOINT_COUNT    (2U) // www fix
#define USB_HID_GENERIC_INTERFACE_INDEX   (3U)

#define USB_HID_GENERIC_CLASS    (0x03U)
#define USB_HID_GENERIC_SUBCLASS (0x00U)
#define USB_HID_GENERIC_PROTOCOL (0x00U)

#define HS_HID_GENERIC_INTERRUPT_OUT_PACKET_SIZE (8U)
#define FS_HID_GENERIC_INTERRUPT_OUT_PACKET_SIZE (8U)
#define HS_HID_GENERIC_INTERRUPT_OUT_INTERVAL    (0x04U) /* 2^(4-1) = 1ms */
#define FS_HID_GENERIC_INTERRUPT_OUT_INTERVAL    (0x01U)

#define HS_HID_GENERIC_INTERRUPT_IN_PACKET_SIZE (8U)
#define FS_HID_GENERIC_INTERRUPT_IN_PACKET_SIZE (8U)
#define HS_HID_GENERIC_INTERRUPT_IN_INTERVAL    (0x04U) /* 2^(4-1) = 1ms */
#define FS_HID_GENERIC_INTERRUPT_IN_INTERVAL    (0x01U)
/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle		  The USB device handle.
 * @param event 		  The USB device event type.
 * @param param 		  The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

extern usb_status_t USB_DeviceSetSpeed(uint8_t speed);

#endif /* _USB_DESCRIPTOR_H_ */
