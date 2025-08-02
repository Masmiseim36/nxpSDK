/**
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "usb_device_config.h"
#include "usb_device.h"
#include "usb_device_descriptor.h"
#include "usb_eth_adapter.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Device Descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t usbDeviceDescriptor[] = {
    USB_DESCRIPTOR_LENGTH_DEVICE, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_DEVICE,   /* DEVICE Descriptor Type */
    USB_SHORT_GET_LOW(USB_DEVICE_SPECIFIC_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_SPECIFIC_BCD_VERSION), /* USB Specification Release Number in Binary-Coded Decimal (i.e., 2.10 is 210H). */
    USB_DEVICE_CLASS,                                    /* Class code (assigned by the USB-IF). */
    USB_DEVICE_SUBCLASS,                                 /* Subclass code (assigned by the USB-IF). */
    USB_DEVICE_PROTOCOL,                                 /* Protocol code (assigned by the USB-IF). */
    USB_CONTROL_MAX_PACKET_SIZE,                         /* Maximum packet size for endpoint zero (only 8, 16, 32, or 64 are valid) */
    USB_SHORT_GET_LOW(USB_DEVICE_VID),
    USB_SHORT_GET_HIGH(USB_DEVICE_VID), /* Vendor ID (assigned by the USB-IF) */
    USB_SHORT_GET_LOW(USB_DEVICE_PID),
    USB_SHORT_GET_HIGH(USB_DEVICE_PID), /* Product ID (assigned by the manufacturer) */
    USB_SHORT_GET_LOW(USB_DEVICE_DEMO_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_DEMO_BCD_VERSION), /* Device release number in binary-coded decimal */
    USB_DEVICE_MANUFACTURER_STRING_INDEX,            /* Index of string descriptor describing manufacturer */
    USB_DEVICE_PRODUCT_STRING_INDEX,                 /* Index of string descriptor describing product */
    USB_DEVICE_SERIAL_NUMBER_STRING_INDEX,           /* Index of string descriptor describing the device's serial number */
    USB_DEVICE_CONFIGURATION_COUNT,                  /* Number of possible configurations */
};

/* Configuration Descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t usbDeviceConfigurationDescriptor[] = {
    /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_LENGTH_CONFIGURE,
    /* CONFIGURATION Descriptor Type */
    USB_DESCRIPTOR_TYPE_CONFIGURE,
    /* Total length of data returned for this configuration. */
    USB_SHORT_GET_LOW(USB_DESCRIPTOR_LENGTH_CONFIGURE +
                      USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_DEVICE_CDC_FUNC_LENGTH +
                      USB_DEVICE_CDC_FUNC_UNION_LENGTH +
                      USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_LENGTH +
                      USB_DESCRIPTOR_LENGTH_ENDPOINT +
                      USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_DESCRIPTOR_LENGTH_ENDPOINT +
                      USB_DESCRIPTOR_LENGTH_ENDPOINT),
    USB_SHORT_GET_HIGH(USB_DESCRIPTOR_LENGTH_CONFIGURE +
                       USB_DESCRIPTOR_LENGTH_INTERFACE +
                       USB_DEVICE_CDC_FUNC_LENGTH +
                       USB_DEVICE_CDC_FUNC_UNION_LENGTH +
                       USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_LENGTH +
                       USB_DESCRIPTOR_LENGTH_ENDPOINT +
                       USB_DESCRIPTOR_LENGTH_INTERFACE +
                       USB_DESCRIPTOR_LENGTH_INTERFACE +
                       USB_DESCRIPTOR_LENGTH_ENDPOINT +
                       USB_DESCRIPTOR_LENGTH_ENDPOINT),
    /* Number of interfaces supported by this configuration */
    USB_DEVICE_CDC_ECM_INTERFACE_COUNT,
    /* Value to use as an argument to the SetConfiguration() request to select this configuration */
    USB_DEVICE_CDC_ECM_CONFIGURATION_VALUE,
    /* Index of string descriptor describing this configuration */
    0, /* Configuration characteristics D7: Reserved (set to one) D6: Self-powered D5: Remote Wakeup D4...0: Reserved(reset to zero) */
    (USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_D7_MASK) |
#if defined(USB_DEVICE_CONFIG_SELF_POWER) && (USB_DEVICE_CONFIG_SELF_POWER > 0U)
        USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_SELF_POWERED_MASK |
#endif
#if defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U)
        USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_REMOTE_WAKEUP_MASK |
#endif
        0x00U,
    /* Maximum power consumption of the USB * device from the bus in this specific * configuration when the device is fully * operational. Expressed in 2 mA units *  (i.e., 50 = 100 mA).  */
    USB_DEVICE_MAX_POWER,

    /* Communication Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_DEVICE_CDC_ECM_COMM_INTERFACE_NUMBER,
    USB_DEVICE_CDC_ECM_COMM_INTERFACE_ALTERNATE,
    USB_DEVICE_CDC_ECM_COMM_ENDPOINT_NUMBER,
    USB_DEVICE_CDC_ECM_COMM_INTERFACE_CLASS_CODE,
    USB_DEVICE_CDC_ECM_COMM_INTERFACE_SUBCLASS_CODE,
    USB_DEVICE_CDC_ECM_COMM_INTERFACE_PROTOCOL_CODE,
    0x00U, /* Interface Description String Index*/

    /* Communication Class Specific Interface Descriptor */
    USB_DEVICE_CDC_FUNC_LENGTH,            /* Size of the descriptor, in bytes */
    USB_DEVICE_CDC_FUNC_TYPE_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_DEVICE_CDC_FUNC_SUBTYPE_HEADER,    /* Header Functional Descriptor Subtype */
    0x10,
    0x01, /* USB Class Definitions for Communications the Communication specification version 1.10 */

    USB_DEVICE_CDC_FUNC_UNION_LENGTH,         /* Size of the descriptor, in bytes */
    USB_DEVICE_CDC_FUNC_UNION_TYPE,           /* CS_INTERFACE Descriptor Type */
    USB_DEVICE_CDC_FUNC_UNION_SUBTYPE,        /* Union Functional Descriptor Subtype */
    USB_DEVICE_CDC_FUNC_UNION_CTRL_INTERFACE, /* The interface number of the Communications or Data Class interface */
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_NUMBER, /* Interface number of subordinate interface in the Union */

    USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_LENGTH,                   /* Size of the descriptor, in bytes */
    USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_TYPE,                     /* CS_INTERFACE Descriptor Type */
    USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_SUBTYPE,                  /* Ethernet Networking Functional Descriptor Subtype */
    USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAC_ADDRESS_STRING_INDEX, /* Index of string descriptor */
    USB_LONG_GET_BYTE0(USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_ETHERNET_STATISTICS),
    USB_LONG_GET_BYTE1(USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_ETHERNET_STATISTICS),
    USB_LONG_GET_BYTE2(USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_ETHERNET_STATISTICS),
    USB_LONG_GET_BYTE3(USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_ETHERNET_STATISTICS), /* Indicates which Ethernet statistics functions the device collects */
    USB_SHORT_GET_LOW(USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAX_SEGMENT_SIZE),
    USB_SHORT_GET_HIGH(USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAX_SEGMENT_SIZE), /* The Maximum segment size that the Ethernet device is capable of supporting */
    USB_SHORT_GET_LOW(USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MULTICAST_FILTERS_NUMBER),
    USB_SHORT_GET_HIGH(USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MULTICAST_FILTERS_NUMBER), /* Multicast filters bitmap */
    USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_POWER_FILTERS_NUMBER,                         /* Contains the number of pattern filters that are available for causing wake-up of the host */

    /* Notification Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER | (USB_IN << 7U),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_DEIVCE_CDC_ECM_COMM_INTERRUPT_IN_EP_MAXPKT_SIZE),
    USB_SHORT_GET_HIGH(USB_DEIVCE_CDC_ECM_COMM_INTERRUPT_IN_EP_MAXPKT_SIZE),
    USB_DEIVCE_CDC_ECM_COMM_INTERRUPT_IN_EP_INTERVAL_FS,

    /* Data Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_NUMBER,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_ALTERNATE0,
    0x00U,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_CLASS_CODE,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_SUBCLASS_CODE,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_PROTOCOL_CODE,
    0x00U, /* Interface Description String Index*/

    /* Data Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_NUMBER,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_ALTERNATE1,
    USB_DEVICE_CDC_ECM_DATA_ENDPOINT_NUMBER,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_CLASS_CODE,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_SUBCLASS_CODE,
    USB_DEVICE_CDC_ECM_DATA_INTERFACE_PROTOCOL_CODE,
    0x00U, /* Interface Description String Index*/

    /* Bulk IN Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | (USB_IN << 7U),
    USB_ENDPOINT_BULK,
    USB_SHORT_GET_LOW(USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_MAXPKT_SIZE_FS),
    USB_SHORT_GET_HIGH(USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_MAXPKT_SIZE_FS),
    USB_DEIVCE_CDC_ECM_DATA_BULK_IN_EP_INTERVAL,

    /* Bulk OUT Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER | (USB_OUT << 7U),
    USB_ENDPOINT_BULK,
    USB_SHORT_GET_LOW(USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_MAXPKT_SIZE_FS),
    USB_SHORT_GET_HIGH(USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_MAXPKT_SIZE_FS),
    USB_DEIVCE_CDC_ECM_DATA_BULK_OUT_EP_INTERVAL,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t usbDeviceLangIDStringDescriptor[] = {
    4U,
    USB_DESCRIPTOR_TYPE_STRING,
    USB_SHORT_GET_LOW(USB_DEVICE_LANGID),
    USB_SHORT_GET_HIGH(USB_DEVICE_LANGID),
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t usbDeviceManufacturerStringDescriptor[2U + (sizeof(USB_DEVICE_MANUFACTURER_STRING) - 1) * 2] = {
    2U,
    USB_DESCRIPTOR_TYPE_STRING,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t usbDeviceProductStringDescriptor[2U + (sizeof(USB_DEVICE_PRODUCT_STRING) - 1) * 2] = {
    2U,
    USB_DESCRIPTOR_TYPE_STRING,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t usbDeviceSerialNumberStringDescriptor[2U + (sizeof(USB_DEVICE_SERIAL_NUMBER_STRING) - 1) * 2] = {
    2U,
    USB_DESCRIPTOR_TYPE_STRING,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t usbDeviceMacAddressStringDescriptor[2U + 6 * 2 * 2] = {
    2U,
    USB_DESCRIPTOR_TYPE_STRING,
};

uint8_t *usbDeviceStringDescriptorArray[USB_DEVICE_STRING_COUNT] = {
    usbDeviceLangIDStringDescriptor,
    usbDeviceManufacturerStringDescriptor,
    usbDeviceProductStringDescriptor,
    usbDeviceSerialNumberStringDescriptor,
    usbDeviceMacAddressStringDescriptor,
};

uint32_t usbDeviceStringDescriptorLength[USB_DEVICE_STRING_COUNT] = {
    sizeof(usbDeviceLangIDStringDescriptor),
    sizeof(usbDeviceManufacturerStringDescriptor),
    sizeof(usbDeviceProductStringDescriptor),
    sizeof(usbDeviceSerialNumberStringDescriptor),
    sizeof(usbDeviceMacAddressStringDescriptor),
};

usb_language_t usbDeviceLanguageList[] = {
    {
        .languageId = USB_DEVICE_LANGID,
        .length = usbDeviceStringDescriptorLength,
        .string = usbDeviceStringDescriptorArray,
    },
};

usb_language_list_t usbDeviceLanguage = {
    .count = ARRAY_SIZE(usbDeviceLanguageList),
    .languageList = usbDeviceLanguageList,
    .languageString = usbDeviceLangIDStringDescriptor,
    .stringLength = sizeof(usbDeviceLangIDStringDescriptor),
};

/*******************************************************************************
 * Code
 ******************************************************************************/
usb_status_t USB_DeviceSetSpeed(usb_device_handle handle, uint8_t speed)
{
    usb_descriptor_union_t *descriptor;

    if (speed == USB_SPEED_HIGH)
    {
        uint32_t incr = 0U;

        for (uint32_t idx = 0U; idx < sizeof(usbDeviceConfigurationDescriptor); idx += incr)
        {
            descriptor = (usb_descriptor_union_t *)(usbDeviceConfigurationDescriptor + idx);

            if (descriptor->common.bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT)
            {
                if (descriptor->endpoint.bEndpointAddress == (USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    descriptor->endpoint.bInterval = USB_DEIVCE_CDC_ECM_COMM_INTERRUPT_IN_EP_INTERVAL_HS;
                }
                else if (descriptor->endpoint.bEndpointAddress == (USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_MAXPKT_SIZE_HS, descriptor->endpoint.wMaxPacketSize);
                }
                else if (descriptor->endpoint.bEndpointAddress == (USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_MAXPKT_SIZE_HS, descriptor->endpoint.wMaxPacketSize);
                }
            }

            incr = descriptor->common.bLength;
        }
    }
    else
    {
        uint32_t incr = 0U;

        for (uint32_t idx = 0U; idx < (uint32_t)sizeof(usbDeviceConfigurationDescriptor); idx += incr)
        {
            descriptor = (usb_descriptor_union_t *)(usbDeviceConfigurationDescriptor + idx);

            if (descriptor->common.bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT)
            {
                if (descriptor->endpoint.bEndpointAddress == (USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    descriptor->endpoint.bInterval = USB_DEIVCE_CDC_ECM_COMM_INTERRUPT_IN_EP_INTERVAL_FS;
                }
                else if (descriptor->endpoint.bEndpointAddress == (USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_MAXPKT_SIZE_FS, descriptor->endpoint.wMaxPacketSize);
                }
                else if (descriptor->endpoint.bEndpointAddress == (USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_MAXPKT_SIZE_FS, descriptor->endpoint.wMaxPacketSize);
                }
            }

            incr = descriptor->common.bLength;
        }
    }

    return kStatus_USB_Success;
}

static void USB_StrAnscii2Unicode(uint8_t *buffer, char *str)
{
    uint16_t *p = (uint16_t *)buffer;
    for (uint32_t idx = 0U; str[idx]; idx++)
    {
        *p++ = str[idx];
    }
}

static void USB_num2Unicode(uint8_t *buffer, uint8_t *array, uint32_t length)
{
    uint16_t *p = (uint16_t *)buffer;
    uint8_t num;

    for (uint32_t idx = 0U; idx < length; idx++)
    {
        num = (array[idx] & 0xF0) >> 4;
        if (num < 10)
        {
            *p++ = '0' + num;
        }
        else if (num >= 10 && num < 16)
        {
            *p++ = 'A' + num - 10;
        }
        else
        {
            *p++ = '0';
        }

        num = array[idx] & 0x0F;
        if (num < 10)
        {
            *p++ = '0' + num;
        }
        else if (num >= 10 && num < 16)
        {
            *p++ = 'A' + num - 10;
        }
        else
        {
            *p++ = '0';
        }
    }
}

void USB_FillStringDescriptorBuffer(void)
{
    USB_StrAnscii2Unicode(usbDeviceManufacturerStringDescriptor + 2, USB_DEVICE_MANUFACTURER_STRING);
    usbDeviceManufacturerStringDescriptor[0] = sizeof(usbDeviceManufacturerStringDescriptor);

    USB_StrAnscii2Unicode(usbDeviceProductStringDescriptor + 2, USB_DEVICE_PRODUCT_STRING);
    usbDeviceProductStringDescriptor[0] = sizeof(usbDeviceProductStringDescriptor);

    USB_StrAnscii2Unicode(usbDeviceSerialNumberStringDescriptor + 2, USB_DEVICE_SERIAL_NUMBER_STRING);
    usbDeviceSerialNumberStringDescriptor[0] = sizeof(usbDeviceSerialNumberStringDescriptor);

    uint8_t mac[6] = {};
    if (ETH_ADAPTER_GetMacAddress(mac) != ETH_ADAPTER_OK)
    {
        (void)usb_echo("ETH_ADAPTER_GetMacAddress() occurs error.\r\n");
    }
    USB_num2Unicode(usbDeviceMacAddressStringDescriptor + 2, mac, sizeof(mac));
    usbDeviceMacAddressStringDescriptor[0] = sizeof(usbDeviceMacAddressStringDescriptor);
}

usb_status_t USB_DeviceGetDescriptor(usb_device_handle handle, usb_setup_struct_t *setup, uint32_t *length, uint8_t **buffer)
{
    uint8_t descriptorType = USB_SHORT_GET_HIGH(setup->wValue);
    uint8_t descriptorIndex = USB_SHORT_GET_LOW(setup->wValue);

    if (USB_REQUEST_STANDARD_GET_DESCRIPTOR != setup->bRequest)
    {
        return kStatus_USB_InvalidRequest;
    }

    switch (descriptorType)
    {
        case USB_DESCRIPTOR_TYPE_STRING:
            if (descriptorIndex == 0)
            {
                *buffer = (uint8_t *)usbDeviceLanguage.languageString;
                *length = usbDeviceLanguage.stringLength;
            }
            else
            {
                uint8_t strIndex = USB_DEVICE_STRING_COUNT;
                uint8_t langIndex = 0U;

                for (; langIndex < usbDeviceLanguage.count; langIndex++)
                {
                    if (setup->wIndex == usbDeviceLanguage.languageList[langIndex].languageId)
                    {
                        if (descriptorIndex < USB_DEVICE_STRING_COUNT)
                        {
                            strIndex = descriptorIndex;
                        }
                        break;
                    }
                }

                if (USB_DEVICE_STRING_COUNT == strIndex)
                {
                    return kStatus_USB_InvalidRequest;
                }

                *buffer = (uint8_t *)usbDeviceLanguage.languageList[langIndex].string[strIndex];
                *length = usbDeviceLanguage.languageList[langIndex].length[strIndex];
            }
            break;

        case USB_DESCRIPTOR_TYPE_DEVICE:
            *buffer = usbDeviceDescriptor;
            *length = sizeof(usbDeviceDescriptor);
            break;

        case USB_DESCRIPTOR_TYPE_CONFIGURE:
            if ((descriptorIndex + 1) == USB_DEVICE_CDC_ECM_CONFIGURATION_VALUE)
            {
                *buffer = usbDeviceConfigurationDescriptor;
                *length = sizeof(usbDeviceConfigurationDescriptor);
            }
            else
            {
                return kStatus_USB_InvalidRequest;
            }
            break;

        default:
            return kStatus_USB_InvalidRequest;
    }

    return kStatus_USB_Success;
}

usb_status_t USB_DeviceGetConfigure(usb_device_handle handle, uint8_t *configure)
{
    *configure = ethNicHandle.configuration;
    return USB_DeviceCallback(handle, kUSB_DeviceEventGetConfiguration, configure);
}

usb_status_t USB_DeviceSetConfigure(usb_device_handle handle, uint8_t configure)
{
    if (configure == USB_DEVICE_CDC_ECM_CONFIGURATION_VALUE)
    {
        ethNicHandle.configuration = configure;
        return USB_DeviceCallback(handle, kUSB_DeviceEventSetConfiguration, &configure);
    }

    return kStatus_USB_InvalidRequest;
}

usb_status_t USB_DeviceGetInterface(usb_device_handle handle, uint8_t interface, uint8_t *alternateSetting)
{
    if (interface < USB_DEVICE_CDC_ECM_INTERFACE_COUNT)
    {
        *alternateSetting = ethNicHandle.interfaceAltSetting[interface];
        return USB_DeviceCallback(handle, kUSB_DeviceEventGetInterface, &interface);
    }

    return kStatus_USB_InvalidRequest;
}

usb_status_t USB_DeviceSetInterface(usb_device_handle handle, uint8_t interface, uint8_t alternateSetting)
{
    usb_status_t status = kStatus_USB_Error;

    switch (interface)
    {
        case USB_DEVICE_CDC_ECM_COMM_INTERFACE_NUMBER:
            if (alternateSetting < USB_DEVICE_CDC_ECM_COMM_INTERFACE_ALTERNATE_COUNT)
            {
                ethNicHandle.interfaceAltSetting[interface] = alternateSetting;
                status = USB_DeviceCallback(handle, kUSB_DeviceEventSetInterface, &interface);
            }
            break;

        case USB_DEVICE_CDC_ECM_DATA_INTERFACE_NUMBER:
            if (alternateSetting < USB_DEVICE_CDC_ECM_DATA_INTERFACE_ALTERNATE_COUNT)
            {
                ethNicHandle.interfaceAltSetting[interface] = alternateSetting;
                status = USB_DeviceCallback(handle, kUSB_DeviceEventSetInterface, &interface);
            }
            break;

        default:
            status = kStatus_USB_InvalidRequest;
            break;
    }

    return status;
}

usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        return USB_DeviceStallEndpoint(handle, ep);
    }
    else
    {
        return USB_DeviceUnstallEndpoint(handle, ep);
    }
}

usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    return kStatus_USB_InvalidRequest;
}
