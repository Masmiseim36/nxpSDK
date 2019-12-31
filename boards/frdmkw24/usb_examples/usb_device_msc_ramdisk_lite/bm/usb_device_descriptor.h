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

#define USB_CONFIGURE_COUNT (1U)
#define USB_DEVICE_STRING_COUNT (4U)
#define USB_DEVICE_LANGUAGE_COUNT (1U)
#define USB_INTERFACE_COUNT (1U)

#define USB_MSC_CONFIGURE_INDEX (1U)

#define USB_MSC_ENDPOINT_COUNT (2U)
#define USB_MSC_BULK_IN_ENDPOINT (1U)
#define USB_MSC_BULK_OUT_ENDPOINT (2U)

/* usb descriptor length */
#define USB_DEVICE_DESCRIPTOR_LENGTH (18)
#define USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL                          \
    (USB_DESCRIPTOR_LENGTH_CONFIGURE + USB_DESCRIPTOR_LENGTH_INTERFACE + \
     USB_DESCRIPTOR_LENGTH_ENDPOINT * USB_MSC_ENDPOINT_COUNT)

#define USB_CONFIGURE_ONLY_DESCRIPTOR_LENGTH (9)
#define USB_INTERFACE_DESCRIPTOR_LENGTH (9)
#define USB_ENDPOINT_DESCRIPTOR_LENGTH (7)

#define HS_MSC_BULK_IN_PACKET_SIZE (512U)
#define HS_MSC_BULK_OUT_PACKET_SIZE (512U)
#define FS_MSC_BULK_IN_PACKET_SIZE (64U)
#define FS_MSC_BULK_OUT_PACKET_SIZE (64U)

#define USB_DESCRIPTOR_LENGTH_STRING0 (0x04U)
#define USB_DESCRIPTOR_LENGTH_STRING1 (38U)
#define USB_DESCRIPTOR_LENGTH_STRING2 (34U)
#define USB_DESCRIPTOR_LENGTH_STRING3 (34U)
#define USB_STRING_DESCRIPTOR_ERROR_LENGTH (34U)

#define USB_MSC_INTERFACE_INDEX (0U)
#define USB_MSC_INTERFACE_COUNT (1U)

#define USB_DEVICE_CLASS (0x00U)
#define USB_DEVICE_SUBCLASS (0x00U)
#define USB_DEVICE_PROTOCOL (0x00U)

#define USB_MSC_CLASS (0x08U)
/* scsi command set */
#define USB_MSC_SUBCLASS (0x06U)
/* bulk only transport protocol */
#define USB_MSC_PROTOCOL (0x50U)

extern usb_status_t USB_DeviceSetSpeed(uint8_t speed);
extern usb_status_t usb_device_standard_request(usb_setup_struct_t *setup, uint8_t **descriptor, uint32_t *size);

extern usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

#endif
