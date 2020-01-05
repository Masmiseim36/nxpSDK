/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __USB_DEVICE_DESCRIPTOR_H__
#define __USB_DEVICE_DESCRIPTOR_H__

/*******************************************************************************
* Definitions
******************************************************************************/
#define USB_DEVICE_SPECIFIC_BCD_VERSION (0x0200U)
#define USB_DEVICE_DEMO_BCD_VERSION (0x0101U)

#define USB_DEVICE_MAX_POWER (0x32U)

/* usb descritpor length */
#define USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL (118)
#define USB_ENDPOINT_AUDIO_DESCRIPTOR_LENGTH (9)
#define USB_AUDIO_CONTROL_INTERFACE_HEADER_LENGTH (9)
#define USB_DESCRIPTOR_LENGTH_AC_INTERRUPT_ENDPOINT (9)
#define USB_AUDIO_INPUT_TERMINAL_ONLY_DESC_SIZE (12)
#define USB_AUDIO_OUTPUT_TERMINAL_ONLY_DESC_SIZE (9)
#define USB_AUDIO_FEATURE_UNIT_ONLY_DESC_SIZE (9)
#define USB_AUDIO_STREAMING_IFACE_DESC_SIZE (7)
#define USB_AUDIO_STREAMING_ENDP_DESC_SIZE (7)
#define USB_AUDIO_STREAMING_TYPE_I_DESC_SIZE (11)

/* Configuration, interface and endpoint. */
#define USB_DEVICE_CONFIGURATION_COUNT (1)
#define USB_DEVICE_STRING_COUNT (3)
#define USB_DEVICE_LANGUAGE_COUNT (1)
#define USB_INTERFACE_COUNT (1)
#define USB_AUDIO_ENDPOINT_COUNT (1)

#define USB_AUDIO_GENERATOR_CONFIGURE_INDEX (1)
#define USB_AUDIO_CONTROL_INTERFACE_INDEX (0)
#define USB_AUDIO_STREAM_INTERFACE_INDEX (1)

#define USB_AUDIO_STREAM_ENDPOINT_COUNT (1)
#define USB_AUDIO_CONTROL_ENDPOINT_COUNT (1)

#define USB_AUDIO_STREAM_ENDPOINT (2)
#define USB_AUDIO_CONTROL_ENDPOINT (1)

#define USB_AUDIO_GENERATOR_INTERFACE_COUNT \
    (USB_AUDIO_GENERATOR_CONTROL_INTERFACE_COUNT + USB_AUDIO_GENERATOR_STREAM_INTERFACE_COUNT)
#define USB_AUDIO_GENERATOR_CONTROL_INTERFACE_COUNT (1)
#define USB_AUDIO_GENERATOR_STREAM_INTERFACE_COUNT (1)

/* Packet size and interval. */
#define HS_INTERRUPT_IN_PACKET_SIZE (8)
#define FS_INTERRUPT_IN_PACKET_SIZE (8)
#if defined(AUDIO_DATA_SOURCE_DMIC) && (AUDIO_DATA_SOURCE_DMIC > 0U)
#define HS_ISO_IN_ENDP_PACKET_SIZE (32)
#define FS_ISO_IN_ENDP_PACKET_SIZE (32)
#else
#define HS_ISO_IN_ENDP_PACKET_SIZE (8)
#define FS_ISO_IN_ENDP_PACKET_SIZE (8)
#endif
#define HS_ISO_IN_ENDP_INTERVAL (0x04)
#define FS_ISO_IN_ENDP_INTERVAL (0x01)
#define ISO_IN_ENDP_INTERVAL (0x01)
#define HS_INTERRUPT_IN_INTERVAL (0x07U) /* 2^(7-1) = 8ms */
#define FS_INTERRUPT_IN_INTERVAL (0x08U)

/* String descriptor length. */
#define USB_DESCRIPTOR_LENGTH_STRING0 (4)
#define USB_DESCRIPTOR_LENGTH_STRING1 (38)
#define USB_DESCRIPTOR_LENGTH_STRING2 (30)

/* Class code. */
#define USB_DEVICE_CLASS (0x00)
#define USB_DEVICE_SUBCLASS (0x00)
#define USB_DEVICE_PROTOCOL (0x00)

#define USB_AUDIO_CLASS (0x01)
#define USB_SUBCLASS_AUDIOCONTROL (0x01)
#define USB_SUBCLASS_AUDIOSTREAM (0x02)
#define USB_AUDIO_PROTOCOL (0x00)

#define USB_AUDIO_FORMAT_TYPE_I (0x01)
#define USB_AUDIO_STREAM_ENDPOINT_DESCRIPTOR (0x25)
#define USB_AUDIO_EP_GENERAL_DESCRIPTOR_SUBTYPE (0x01)

#define USB_AUDIO_CONTROL_INPUT_TERMINAL_ID (0x01)
#define USB_AUDIO_CONTROL_FEATURE_UNIT_ID (0x02)
#define USB_AUDIO_CONTROL_OUTPUT_TERMINAL_ID (0x03)

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
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
#endif
