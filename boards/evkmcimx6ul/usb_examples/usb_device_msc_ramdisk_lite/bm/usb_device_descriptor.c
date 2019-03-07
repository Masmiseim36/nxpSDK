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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_msc.h"

#include "usb_device_descriptor.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_UsbDeviceCurrentConfigure = 0U;
uint8_t g_UsbDeviceInterface[USB_MSC_INTERFACE_COUNT];

USB_RAM_ADDRESS_NONCACHEREG(uint8_t g_UsbDeviceDescriptor[USB_DESCRIPTOR_LENGTH_DEVICE]);
uint8_t g_UsbDeviceDescriptor[USB_DESCRIPTOR_LENGTH_DEVICE] = {

    USB_DESCRIPTOR_LENGTH_DEVICE, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_DEVICE,   /* DEVICE Descriptor Type */
    USB_SHORT_GET_LOW(USB_DEVICE_SPECIFIC_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_SPECIFIC_BCD_VERSION), /* USB Specification Release Number in
                                                            Binary-Coded Decimal (i.e., 2.10 is 210H). */
    USB_DEVICE_CLASS,                                    /* Class code (assigned by the USB-IF). */
    USB_DEVICE_SUBCLASS,                                 /* Subclass code (assigned by the USB-IF). */
    USB_DEVICE_PROTOCOL,                                 /* Protocol code (assigned by the USB-IF). */
    USB_CONTROL_MAX_PACKET_SIZE,                         /* Maximum packet size for endpoint zero
                                                            (only 8, 16, 32, or 64 are valid) */
    0xC9U, 0x1FU,                                        /* Vendor ID (assigned by the USB-IF) */
    0x92U, 0x00U,                                        /* Product ID (assigned by the manufacturer) */
    USB_SHORT_GET_LOW(USB_DEVICE_DEMO_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_DEMO_BCD_VERSION), /* Device release number in binary-coded decimal */
    0x01U,                                           /* Index of string descriptor describing manufacturer */
    0x02U,                                           /* Index of string descriptor describing product */
    0x03U,               /* Index of string descriptor describing the device's serial number */
    USB_CONFIGURE_COUNT, /* Number of possible configurations */
};

USB_RAM_ADDRESS_NONCACHEREG(uint8_t g_UsbDeviceConfigurationDescriptor[USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL]);
uint8_t g_UsbDeviceConfigurationDescriptor[USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL] = {
    USB_DESCRIPTOR_LENGTH_CONFIGURE, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CONFIGURE,   /* CONFIGURATION Descriptor Type */
    USB_SHORT_GET_LOW(USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL),
    USB_SHORT_GET_HIGH(
        USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL), /* Total length of data returned for this configuration. */
    USB_MSC_INTERFACE_COUNT,                      /* Number of interfaces supported by this configuration */
    USB_MSC_CONFIGURE_INDEX,                      /* Value to use as an argument to the
                                                          SetConfiguration() request to select this configuration */
    0x0U,                                         /* Index of string descriptor describing this configuration */
    (USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_D7_MASK) |
        (USB_DEVICE_CONFIG_SELF_POWER << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_SELF_POWERED_SHIFT) |
        (USB_DEVICE_CONFIG_REMOTE_WAKEUP << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_REMOTE_WAKEUP_SHIFT),
    /* Configuration characteristics
 D7: Reserved (set to one)
 D6: Self-powered
 D5: Remote Wakeup
 D4...0: Reserved (reset to zero)
*/
    USB_DEVICE_MAX_POWER,            /* Maximum power consumption of the USB
                                     * device from the bus in this specific
                                     * configuration when the device is fully
                                     * operational. Expressed in 2 mA units
                                     *  (i.e., 50 = 100 mA).
                                     */
    USB_DESCRIPTOR_LENGTH_INTERFACE, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_INTERFACE,   /* INTERFACE Descriptor Type */
    USB_MSC_INTERFACE_INDEX,         /* Number of this interface. */
    0x00U,                           /* Value used to select this alternate setting
                                                            for the interface identified in the prior field */
    USB_MSC_ENDPOINT_COUNT,          /* Number of endpoints used by this
                                                              interface (excluding endpoint zero). */

    USB_MSC_CLASS,    /* Class code (assigned by the USB-IF). */
    USB_MSC_SUBCLASS, /* Subclass code (assigned by the USB-IF). */
    USB_MSC_PROTOCOL, /* Protocol code (assigned by the USB). */
    0x00U,            /* Index of string descriptor describing this interface */

    USB_DESCRIPTOR_LENGTH_ENDPOINT, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* ENDPOINT Descriptor Type */
    USB_MSC_BULK_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    /* The address of the endpoint on the USB device
                             described by this descriptor. */
    USB_ENDPOINT_BULK, /* This field describes the endpoint's attributes */
    USB_SHORT_GET_LOW(FS_MSC_BULK_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(FS_MSC_BULK_IN_PACKET_SIZE), /* Maximum packet size this endpoint is capable of sending or
                                                       receiving when this configuration is selected. */
    0x00U,                                          /*Useless for bulk in endpoint*/

    USB_DESCRIPTOR_LENGTH_ENDPOINT, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* ENDPOINT Descriptor Type */
    USB_MSC_BULK_OUT_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    /* The address of the endpoint on the USB device
                         described by this descriptor. */
    USB_ENDPOINT_BULK, /* This field describes the endpoint's attributes */
    USB_SHORT_GET_LOW(FS_MSC_BULK_OUT_PACKET_SIZE), USB_SHORT_GET_HIGH(FS_MSC_BULK_OUT_PACKET_SIZE),
    0x00U /*For high-speed bulk/control OUT endpoints, the bInterval must specify the
         maximum NAK rate of the endpoint. refer to usb spec 9.6.6*/
};

USB_RAM_ADDRESS_NONCACHEREG(uint8_t g_UsbDeviceString0[USB_DESCRIPTOR_LENGTH_STRING0]);
uint8_t g_UsbDeviceString0[USB_DESCRIPTOR_LENGTH_STRING0] = {
    sizeof(g_UsbDeviceString0), USB_DESCRIPTOR_TYPE_STRING, 0x09U, 0x04U,
};

USB_RAM_ADDRESS_NONCACHEREG(uint8_t g_UsbDeviceString1[USB_DESCRIPTOR_LENGTH_STRING1]);
uint8_t g_UsbDeviceString1[USB_DESCRIPTOR_LENGTH_STRING1] = {
    sizeof(g_UsbDeviceString1),
    USB_DESCRIPTOR_TYPE_STRING,
    'N',
    0x00U,
    'X',
    0x00U,
    'P',
    0x00U,
    ' ',
    0x00U,
    'S',
    0x00U,
    'E',
    0x00U,
    'M',
    0x00U,
    'I',
    0x00U,
    'C',
    0x00U,
    'O',
    0x00U,
    'N',
    0x00U,
    'D',
    0x00U,
    'U',
    0x00U,
    'C',
    0x00U,
    'T',
    0x00U,
    'O',
    0x00U,
    'R',
    0x00U,
    'S',
    0x00U,
};

USB_RAM_ADDRESS_NONCACHEREG(uint8_t g_UsbDeviceString2[USB_DESCRIPTOR_LENGTH_STRING2]);
uint8_t g_UsbDeviceString2[USB_DESCRIPTOR_LENGTH_STRING2] = {sizeof(g_UsbDeviceString2),
                                                             USB_DESCRIPTOR_TYPE_STRING,
                                                             'M',
                                                             0x00U,
                                                             'C',
                                                             0x00U,
                                                             'U',
                                                             0x00U,
                                                             ' ',
                                                             0x00U,
                                                             'M',
                                                             0x00U,
                                                             'A',
                                                             0x00U,
                                                             'S',
                                                             0x00U,
                                                             'S',
                                                             0x00U,
                                                             ' ',
                                                             0x00U,
                                                             'S',
                                                             0x00U,
                                                             'T',
                                                             0x00U,
                                                             'O',
                                                             0x00U,
                                                             'R',
                                                             0x00U,
                                                             'A',
                                                             0x00U,
                                                             'G',
                                                             0x00U,
                                                             'E',
                                                             0x00U};

USB_RAM_ADDRESS_NONCACHEREG(uint8_t g_UsbDeviceString3[USB_DESCRIPTOR_LENGTH_STRING3]);
uint8_t g_UsbDeviceString3[USB_DESCRIPTOR_LENGTH_STRING3] = {sizeof(g_UsbDeviceString3),
                                                             USB_DESCRIPTOR_TYPE_STRING,
                                                             '0',
                                                             0x00U,
                                                             '1',
                                                             0x00U,
                                                             '2',
                                                             0x00U,
                                                             '3',
                                                             0x00U,
                                                             '4',
                                                             0x00U,
                                                             '5',
                                                             0x00U,
                                                             '6',
                                                             0x00U,
                                                             '7',
                                                             0x00U,
                                                             '8',
                                                             0x00U,
                                                             '9',
                                                             0x00U,
                                                             'A',
                                                             0x00U,
                                                             'B',
                                                             0x00U,
                                                             'C',
                                                             0x00U,
                                                             'D',
                                                             0x00U,
                                                             'E',
                                                             0x00U,
                                                             'F',
                                                             0x00U};
USB_RAM_ADDRESS_NONCACHEREG(uint8_t g_UsbDeviceStringN[USB_STRING_DESCRIPTOR_ERROR_LENGTH]);
uint8_t g_UsbDeviceStringN[USB_STRING_DESCRIPTOR_ERROR_LENGTH] = {sizeof(g_UsbDeviceStringN),
                                                                  USB_DESCRIPTOR_TYPE_STRING,
                                                                  'B',
                                                                  0x00U,
                                                                  'A',
                                                                  0x00U,
                                                                  'D',
                                                                  0x00U,
                                                                  ' ',
                                                                  0x00U,
                                                                  'S',
                                                                  0x00U,
                                                                  'T',
                                                                  0x00U,
                                                                  'R',
                                                                  0x00U,
                                                                  'I',
                                                                  0x00U,
                                                                  'N',
                                                                  0x00U,
                                                                  'G',
                                                                  0x00U,
                                                                  ' ',
                                                                  0x00U,
                                                                  'I',
                                                                  0x00U,
                                                                  'N',
                                                                  0x00U,
                                                                  'D',
                                                                  0x00U,
                                                                  'E',
                                                                  0x00U,
                                                                  'X',
                                                                  0x00U};

uint32_t g_UsbStringDescriptorSize[USB_DEVICE_STRING_COUNT + 1] = {
    sizeof(g_UsbDeviceString0), sizeof(g_UsbDeviceString1), sizeof(g_UsbDeviceString2),
    sizeof(g_UsbDeviceString3), sizeof(g_UsbDeviceStringN),
};

uint8_t *g_UsbStringDescriptors[USB_DEVICE_STRING_COUNT + 1] = {
    g_UsbDeviceString0, g_UsbDeviceString1, g_UsbDeviceString2, g_UsbDeviceString3, g_UsbDeviceStringN,
};
usb_language_t g_UsbLanguage[USB_DEVICE_LANGUAGE_COUNT] = {{
    g_UsbStringDescriptors, g_UsbStringDescriptorSize, (uint16_t)0x0409U,
}};

usb_language_list_t g_UsbDeviceLanguageList = {
    g_UsbDeviceString0, sizeof(g_UsbDeviceString0), g_UsbLanguage, USB_DEVICE_LANGUAGE_COUNT,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get descriptor request */
usb_status_t USB_DeviceGetDescriptor(usb_device_handle handle,
                                     usb_setup_struct_t *setup,
                                     uint32_t *length,
                                     uint8_t **buffer

                                     )
{
    uint8_t descriptorType = (uint8_t)((setup->wValue & 0xFF00U) >> 8U);
    uint8_t descriptorIndex = (uint8_t)((setup->wValue & 0x00FFU));
    usb_status_t status = kStatus_USB_Success;
    if (USB_REQUEST_STANDARD_GET_DESCRIPTOR != setup->bRequest)
    {
        return kStatus_USB_InvalidRequest;
    }
    switch (descriptorType)
    {
        case USB_DESCRIPTOR_TYPE_STRING:
        {
            if (0U == descriptorIndex)
            {
                *buffer = (uint8_t *)g_UsbDeviceLanguageList.languageString;
                *length = g_UsbDeviceLanguageList.stringLength;
            }
            else
            {
                uint8_t languageId = 0U;
                uint8_t languageIndex = USB_DEVICE_STRING_COUNT;

                for (; languageId < USB_DEVICE_LANGUAGE_COUNT; languageId++)
                {
                    if (setup->wIndex == g_UsbDeviceLanguageList.languageList[languageId].languageId)
                    {
                        if (descriptorIndex < USB_DEVICE_STRING_COUNT)
                        {
                            languageIndex = descriptorIndex;
                        }
                        break;
                    }
                }

                if (USB_DEVICE_STRING_COUNT == languageIndex)
                {
                    return kStatus_USB_InvalidRequest;
                }
                *buffer = (uint8_t *)g_UsbDeviceLanguageList.languageList[languageId].string[languageIndex];
                *length = g_UsbDeviceLanguageList.languageList[languageId].length[languageIndex];
            }
        }
        break;
        case USB_DESCRIPTOR_TYPE_DEVICE:
        {
            /* Get device descriptor */
            *buffer = g_UsbDeviceDescriptor;
            *length = USB_DEVICE_DESCRIPTOR_LENGTH;
        }
        break;
        case USB_DESCRIPTOR_TYPE_CONFIGURE:
        {
            /* Get configuration descriptor */
            *buffer = g_UsbDeviceConfigurationDescriptor;
            *length = USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL;
        }
        break;
        default:
            status = kStatus_USB_InvalidRequest;
            break;
    }
    return status;
}

/* Set current confgiuration request */
usb_status_t USB_DeviceSetConfigure(usb_device_handle handle, uint8_t configure)
{
    g_UsbDeviceCurrentConfigure = configure;
    return USB_DeviceCallback(handle, kUSB_DeviceEventSetConfiguration, &configure);
}

/* Get current confgiuration request */
usb_status_t USB_DeviceGetConfigure(usb_device_handle handle, uint8_t *configure)
{
    *configure = g_UsbDeviceCurrentConfigure;
    return kStatus_USB_Success;
}

/* Set current alternate settting of the interface request */
usb_status_t USB_DeviceSetInterface(usb_device_handle handle, uint8_t interface, uint8_t alternateSetting)
{
    g_UsbDeviceInterface[interface] = alternateSetting;
    return USB_DeviceCallback(handle, kUSB_DeviceEventSetInterface, &interface);
}
/* Get current alternate settting of the interface request */
usb_status_t USB_DeviceGetInterface(usb_device_handle handle, uint8_t interface, uint8_t *alternateSetting)
{
    *alternateSetting = g_UsbDeviceInterface[interface];
    return kStatus_USB_Success;
}
/* Due to the difference of HS and FS descriptors, the device descriptors and configurations need to be updated to match
* current speed.
* As the default, the device descriptors and configurations are configured by using FS parameters for both EHCI and
* KHCI.
* When the EHCI is enabled, the application needs to call this function to update device by using current speed.
* The updated information includes endpoint max packet size, endpoint interval, etc. */
usb_status_t USB_DeviceSetSpeed(uint8_t speed)
{
    usb_descriptor_union_t *descriptorHead;
    usb_descriptor_union_t *descriptorTail;

    descriptorHead = (usb_descriptor_union_t *)&g_UsbDeviceConfigurationDescriptor[0];
    descriptorTail =
        (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL - 1]);

    while (descriptorHead < descriptorTail)
    {
        if (descriptorHead->endpoint.bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT)
        {
            if (USB_SPEED_HIGH == speed)
            {
                if (descriptorHead->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)
                {
                    descriptorHead->endpoint.wMaxPacketSize[0] = USB_SHORT_GET_LOW(HS_MSC_BULK_IN_PACKET_SIZE);
                    descriptorHead->endpoint.wMaxPacketSize[1] = USB_SHORT_GET_HIGH(HS_MSC_BULK_IN_PACKET_SIZE);
                }
                else
                {
                    descriptorHead->endpoint.wMaxPacketSize[0] = USB_SHORT_GET_LOW(HS_MSC_BULK_OUT_PACKET_SIZE);
                    descriptorHead->endpoint.wMaxPacketSize[1] = USB_SHORT_GET_HIGH(HS_MSC_BULK_OUT_PACKET_SIZE);
                }
            }
            else
            {
                if (descriptorHead->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)
                {
                    descriptorHead->endpoint.wMaxPacketSize[0] = USB_SHORT_GET_LOW(FS_MSC_BULK_IN_PACKET_SIZE);
                    descriptorHead->endpoint.wMaxPacketSize[1] = USB_SHORT_GET_HIGH(FS_MSC_BULK_IN_PACKET_SIZE);
                }
                else
                {
                    descriptorHead->endpoint.wMaxPacketSize[0] = USB_SHORT_GET_LOW(FS_MSC_BULK_OUT_PACKET_SIZE);
                    descriptorHead->endpoint.wMaxPacketSize[1] = USB_SHORT_GET_HIGH(FS_MSC_BULK_OUT_PACKET_SIZE);
                }
            }
        }
        descriptorHead = (usb_descriptor_union_t *)((uint8_t *)descriptorHead + descriptorHead->common.bLength);
    }

    return kStatus_USB_Success;
}
