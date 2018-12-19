/*
 * Copyright 2017 - 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_cdc_acm.h"

#include "usb_device_descriptor.h"

/*******************************************************************************
* Variables
******************************************************************************/
uint8_t g_currentConfigure = 0;
uint8_t g_interface[USB_CDC_VCOM_INTERFACE_COUNT + USB_CDC_VCOM_INTERFACE_COUNT_2];
/* Define device descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceDescriptor[USB_DESCRIPTOR_LENGTH_DEVICE] = {
    /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_LENGTH_DEVICE,
    /* DEVICE Descriptor Type */
    USB_DESCRIPTOR_TYPE_DEVICE,
    /* USB Specification Release Number in Binary-Coded Decimal (i.e., 2.10 is 210H). */
    USB_SHORT_GET_LOW(USB_DEVICE_SPECIFIC_BCD_VERSION), USB_SHORT_GET_HIGH(USB_DEVICE_SPECIFIC_BCD_VERSION),
    /* Class code (assigned by the USB-IF). */
    USB_DEVICE_CLASS,
    /* Subclass code (assigned by the USB-IF). */
    USB_DEVICE_SUBCLASS,
    /* Protocol code (assigned by the USB-IF). */
    USB_DEVICE_PROTOCOL,
    /* Maximum packet size for endpoint zero (only 8, 16, 32, or 64 are valid) */
    USB_CONTROL_MAX_PACKET_SIZE,
    /* Vendor ID (assigned by the USB-IF) */
    0xC9U, 0x1FU,
    /* Product ID (assigned by the manufacturer) */
    0xA3, 0x00,
    /* Device release number in binary-coded decimal */
    USB_SHORT_GET_LOW(USB_DEVICE_DEMO_BCD_VERSION), USB_SHORT_GET_HIGH(USB_DEVICE_DEMO_BCD_VERSION),
    /* Index of string descriptor describing manufacturer */
    0x01,
    /* Index of string descriptor describing product */
    0x02,
    /* Index of string descriptor describing the device's serial number */
    0x03,
    /* Number of possible configurations */
    USB_DEVICE_CONFIGURATION_COUNT,
};

/* Define configuration descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceConfigurationDescriptor[] = {
    /* Configuration Descriptor Size*/
    USB_DESCRIPTOR_LENGTH_CONFIGURE,
    /* CONFIGURATION Descriptor Type */
    USB_DESCRIPTOR_TYPE_CONFIGURE,
    /* Total length of data returned for this configuration. */
    USB_SHORT_GET_LOW(USB_DESCRIPTOR_LENGTH_CONFIGURE +
                      (USB_IAD_DESC_SIZE + USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_CDC_HEADER_FUNC +
                       USB_DESCRIPTOR_LENGTH_CDC_CALL_MANAG + USB_DESCRIPTOR_LENGTH_CDC_ABSTRACT +
                       USB_DESCRIPTOR_LENGTH_CDC_UNION_FUNC + USB_DESCRIPTOR_LENGTH_ENDPOINT +
                       USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_ENDPOINT +
                       USB_DESCRIPTOR_LENGTH_ENDPOINT) *
                          2),
    USB_SHORT_GET_HIGH(USB_DESCRIPTOR_LENGTH_CONFIGURE +
                       (USB_IAD_DESC_SIZE + USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_CDC_HEADER_FUNC +
                        USB_DESCRIPTOR_LENGTH_CDC_CALL_MANAG + USB_DESCRIPTOR_LENGTH_CDC_ABSTRACT +
                        USB_DESCRIPTOR_LENGTH_CDC_UNION_FUNC + USB_DESCRIPTOR_LENGTH_ENDPOINT +
                        USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_ENDPOINT +
                        USB_DESCRIPTOR_LENGTH_ENDPOINT) *
                           2),
    /* Number of interfaces supported by this configuration */
    USB_INTERFACE_COUNT,
    /* Value to use as an argument to the SetConfiguration() request to select this configuration */
    USB_COMPOSITE_CONFIGURE_INDEX,
    /* Index of string descriptor describing this configuration */
    0,
    /* Configuration characteristics D7: Reserved (set to one) D6: Self-powered D5: Remote Wakeup D4...0: Reserved
       (reset to zero) */
    (USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_D7_MASK) |
        (USB_DEVICE_CONFIG_SELF_POWER << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_SELF_POWERED_SHIFT) |
        (USB_DEVICE_CONFIG_REMOTE_WAKEUP << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_REMOTE_WAKEUP_SHIFT),
    /* Maximum power consumption of the USB * device from the bus in this specific * configuration when the device is
       fully * operational. Expressed in 2 mA units *  (i.e., 50 = 100 mA).  */
    USB_DEVICE_MAX_POWER,

    /* Interface Association Descriptor */
    /* Size of this descriptor in bytes */
    USB_IAD_DESC_SIZE,
    /* INTERFACE_ASSOCIATION Descriptor Type  */
    USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,
    /* The first interface number associated with this function */
    0x00,
    /* The number of contiguous interfaces associated with this function */
    0x02,
    /* The function belongs to the Communication Device/Interface Class  */
    USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS,
    /* The function uses the No class specific protocol required Protocol  */
    0x00,
    /* The Function string descriptor index */
    0x02,

    /* Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE, USB_DESCRIPTOR_TYPE_INTERFACE, USB_CDC_VCOM_CIC_INTERFACE_INDEX, 0x00,
    USB_CDC_VCOM_CIC_ENDPOINT_COUNT, USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS, USB_CDC_VCOM_CIC_PROTOCOL, 0x00,

    /* CDC Class-Specific descriptor */
    USB_DESCRIPTOR_LENGTH_CDC_HEADER_FUNC, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE,  /* CS_INTERFACE Descriptor Type */
    USB_CDC_HEADER_FUNC_DESC, 0x10,
    0x01, /* USB Class Definitions for Communications the Communication specification version 1.10 */

    USB_DESCRIPTOR_LENGTH_CDC_CALL_MANAG, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_CALL_MANAGEMENT_FUNC_DESC,
    0x01, /*Bit 0: Whether device handle call management itself 1, Bit 1: Whether device can send/receive call
             management information over a Data Class Interface 0 */
    0x01, /* Indicates multiplexed commands are handled via data interface */

    USB_DESCRIPTOR_LENGTH_CDC_ABSTRACT,   /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_ABSTRACT_CONTROL_FUNC_DESC,
    0x06, /* Bit 0: Whether device supports the request combination of Set_Comm_Feature, Clear_Comm_Feature, and
             Get_Comm_Feature 0, Bit 1: Whether device supports the request combination of Set_Line_Coding,
             Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State 1, Bit ...  */

    USB_DESCRIPTOR_LENGTH_CDC_UNION_FUNC, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_UNION_FUNC_DESC, 0x00,        /* The interface number of the Communications or Data Class interface  */
    0x01,                                 /* Interface number of subordinate interface in the Union  */

    /*Notification Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT | (USB_IN << 7U), USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE), USB_SHORT_GET_HIGH(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE),
    FS_CDC_VCOM_INTERRUPT_IN_INTERVAL,

    /* Data Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE, USB_DESCRIPTOR_TYPE_INTERFACE, USB_CDC_VCOM_DIC_INTERFACE_INDEX, 0x00,
    USB_CDC_VCOM_DIC_ENDPOINT_COUNT, USB_CDC_VCOM_DIC_CLASS, USB_CDC_VCOM_DIC_SUBCLASS, USB_CDC_VCOM_DIC_PROTOCOL,
    0x00, /* Interface Description String Index*/

    /*Bulk IN Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT, USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT | (USB_IN << 7U),
    USB_ENDPOINT_BULK, USB_SHORT_GET_LOW(FS_CDC_VCOM_BULK_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(FS_CDC_VCOM_BULK_IN_PACKET_SIZE), 0x00, /* The polling interval value is every 0 Frames */

    /*Bulk OUT Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT, USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT | (USB_OUT << 7U),
    USB_ENDPOINT_BULK, USB_SHORT_GET_LOW(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE),
    USB_SHORT_GET_HIGH(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE), 0x00, /* The polling interval value is every 0 Frames */

    /*****VCOM_2 descriptor*****/
    /* Interface Association Descriptor */
    /* Size of this descriptor in bytes */
    USB_IAD_DESC_SIZE,
    /* INTERFACE_ASSOCIATION Descriptor Type  */
    USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,
    /* The first interface number associated with this function */
    0x02,
    /* The number of contiguous interfaces associated with this function */
    0x02,
    /* The function belongs to the Communication Device/Interface Class  */
    USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS,
    /* The function uses the No class specific protocol required Protocol  */
    0x00,
    /* The Function string descriptor index */
    0x02,
    /* CDC Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE, USB_DESCRIPTOR_TYPE_INTERFACE, USB_CDC_VCOM_CIC_INTERFACE_INDEX_2, 0x00,
    USB_CDC_VCOM_CIC_ENDPOINT_COUNT_2, USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS, USB_CDC_VCOM_CIC_PROTOCOL,
    0x00,

    /* CDC Class-Specific descriptor */
    USB_DESCRIPTOR_LENGTH_CDC_HEADER_FUNC, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE,  /* CS_INTERFACE Descriptor Type */
    USB_CDC_HEADER_FUNC_DESC, 0x10,
    0x01, /* USB Class Definitions for Communications the Communication specification version 1.10 */

    USB_DESCRIPTOR_LENGTH_CDC_CALL_MANAG, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_CALL_MANAGEMENT_FUNC_DESC,
    0x01, /*Bit 0: Whether device handle call management itself 1, Bit 1: Whether device can send/receive call
             management information over a Data Class Interface 0 */
    0x01, /* Indicates multiplexed commands are handled via data interface */

    USB_DESCRIPTOR_LENGTH_CDC_ABSTRACT,   /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_ABSTRACT_CONTROL_FUNC_DESC,
    0x06, /* Bit 0: Whether device supports the request combination of Set_Comm_Feature, Clear_Comm_Feature, and
             Get_Comm_Feature 0, Bit 1: Whether device supports the request combination of Set_Line_Coding,
             Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State 1, Bit ...  */

    USB_DESCRIPTOR_LENGTH_CDC_UNION_FUNC, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_UNION_FUNC_DESC, 0x02,        /* The interface number of the Communications or Data Class interface  */
    0x03,                                 /* Interface number of subordinate interface in the Union  */

    /*Notification Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2 | (USB_IN << 7U), USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2),
    USB_SHORT_GET_HIGH(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2), FS_CDC_VCOM_INTERRUPT_IN_INTERVAL_2,

    /* Data Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE, USB_DESCRIPTOR_TYPE_INTERFACE, USB_CDC_VCOM_DIC_INTERFACE_INDEX_2, 0x00,
    USB_CDC_VCOM_DIC_ENDPOINT_COUNT_2, USB_CDC_VCOM_DIC_CLASS, USB_CDC_VCOM_DIC_SUBCLASS, USB_CDC_VCOM_DIC_PROTOCOL,
    0x00, /* Interface Description String Index*/

    /*Bulk IN Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT, USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 | (USB_IN << 7U),
    USB_ENDPOINT_BULK, USB_SHORT_GET_LOW(FS_CDC_VCOM_BULK_IN_PACKET_SIZE_2),
    USB_SHORT_GET_HIGH(FS_CDC_VCOM_BULK_IN_PACKET_SIZE_2), 0x00, /* The polling interval value is every 0 Frames */

    /*Bulk OUT Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 | (USB_OUT << 7U), USB_ENDPOINT_BULK,
    USB_SHORT_GET_LOW(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2), USB_SHORT_GET_HIGH(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2),
    0x00, /* The polling interval value is every 0 Frames */
};
#if (defined(USB_DEVICE_CONFIG_CV_TEST) && (USB_DEVICE_CONFIG_CV_TEST > 0U))
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceQualifierDescriptor[USB_DESCRIPTOR_LENGTH_DEVICE_QUALITIER] = {
    USB_DESCRIPTOR_LENGTH_DEVICE_QUALITIER, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_DEVICE_QUALITIER,   /* DEVICE Descriptor Type */
    USB_SHORT_GET_LOW(USB_DEVICE_SPECIFIC_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_SPECIFIC_BCD_VERSION), /* USB Specification Release Number in
                                                            Binary-Coded Decimal (i.e., 2.10 is 210H). */
    USB_DEVICE_CLASS,                                    /* Class code (assigned by the USB-IF). */
    USB_DEVICE_SUBCLASS,                                 /* Subclass code (assigned by the USB-IF). */
    USB_DEVICE_PROTOCOL,                                 /* Protocol code (assigned by the USB-IF). */
    USB_CONTROL_MAX_PACKET_SIZE,                         /* Maximum packet size for endpoint zero
                                                            (only 8, 16, 32, or 64 are valid) */
    0x00U,                                               /* Number of Other-speed Configurations */
    0x00U,                                               /* Reserved for future use, must be zero */
};
#endif
/* Define string descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString0[] = {
    2U + 2U, USB_DESCRIPTOR_TYPE_STRING, 0x09, 0x04,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString1[] = {
    2U + 2U * 18U, USB_DESCRIPTOR_TYPE_STRING,
    'N',           0x00U,
    'X',           0x00U,
    'P',           0x00U,
    ' ',           0x00U,
    'S',           0x00U,
    'E',           0x00U,
    'M',           0x00U,
    'I',           0x00U,
    'C',           0x00U,
    'O',           0x00U,
    'N',           0x00U,
    'D',           0x00U,
    'U',           0x00U,
    'C',           0x00U,
    'T',           0x00U,
    'O',           0x00U,
    'R',           0x00U,
    'S',           0x00U,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString2[] = {2U + 2U * 18U, USB_DESCRIPTOR_TYPE_STRING,
                                'U',           0,
                                'S',           0,
                                'B',           0,
                                ' ',           0,
                                'C',           0,
                                'O',           0,
                                'M',           0,
                                'P',           0,
                                'O',           0,
                                'S',           0,
                                'I',           0,
                                'T',           0,
                                'E',           0,
                                ' ',           0,
                                'D',           0,
                                'E',           0,
                                'M',           0,
                                'O',           0};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString3[] = {2U + 2U * 16U, USB_DESCRIPTOR_TYPE_STRING,
                                '0',           0x00U,
                                '1',           0x00U,
                                '2',           0x00U,
                                '3',           0x00U,
                                '4',           0x00U,
                                '5',           0x00U,
                                '6',           0x00U,
                                '7',           0x00U,
                                '8',           0x00U,
                                '9',           0x00U,
                                'A',           0x00U,
                                'B',           0x00U,
                                'C',           0x00U,
                                'D',           0x00U,
                                'E',           0x00U,
                                'F',           0x00U};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString4[] = {2U + 2U * 17U, USB_DESCRIPTOR_TYPE_STRING,
                                'M',           0,
                                'C',           0,
                                'U',           0,
                                ' ',           0,
                                'C',           0,
                                'D',           0,
                                'C',           0,
                                ' ',           0,
                                'C',           0,
                                'D',           0,
                                'C',           0,
                                '2',           0,
                                ' ',           0,
                                'D',           0,
                                'E',           0,
                                'M',           0,
                                'O',           0};

/* Define string descriptor size */
uint32_t g_UsbDeviceStringDescriptorLength[USB_DEVICE_STRING_COUNT] = {
    sizeof(g_UsbDeviceString0), sizeof(g_UsbDeviceString1), sizeof(g_UsbDeviceString2), sizeof(g_UsbDeviceString3),
    sizeof(g_UsbDeviceString4)};

uint8_t *g_UsbDeviceStringDescriptorArray[USB_DEVICE_STRING_COUNT] = {
    g_UsbDeviceString0, g_UsbDeviceString1, g_UsbDeviceString2, g_UsbDeviceString3, g_UsbDeviceString4};

usb_language_t g_UsbDeviceLanguage[USB_DEVICE_LANGUAGE_COUNT] = {{
    g_UsbDeviceStringDescriptorArray, g_UsbDeviceStringDescriptorLength, (uint16_t)0x0409,
}};

usb_language_list_t g_UsbDeviceLanguageList = {
    g_UsbDeviceString0, sizeof(g_UsbDeviceString0), g_UsbDeviceLanguage, USB_DEVICE_LANGUAGE_COUNT,
};

/*******************************************************************************
* Code
******************************************************************************/
/*!
 * @brief Get the descriptor.
 *
 * The function is used to get the descriptor, including the device descriptor, configuration descriptor, and string
 * descriptor, etc.
 *
 * @param handle              The device handle.
 * @param setup               The setup packet buffer address.
 * @param length              It is an OUT parameter, return the data length need to be sent to host.
 * @param buffer              It is an OUT parameter, return the data buffer address.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetDescriptor(usb_device_handle handle,
                                     usb_setup_struct_t *setup,
                                     uint32_t *length,
                                     uint8_t **buffer)
{
    uint8_t descriptorType = (uint8_t)((setup->wValue & 0xFF00U) >> 8U);
    uint8_t descriptorIndex = (uint8_t)((setup->wValue & 0x00FFU));
    usb_status_t ret = kStatus_USB_Success;
    if (USB_REQUEST_STANDARD_GET_DESCRIPTOR != setup->bRequest)
    {
        return kStatus_USB_InvalidRequest;
    }
    switch (descriptorType)
    {
        case USB_DESCRIPTOR_TYPE_STRING:
        {
            if (descriptorIndex == 0)
            {
                *buffer = (uint8_t *)g_UsbDeviceLanguageList.languageString;
                *length = g_UsbDeviceLanguageList.stringLength;
            }
            else
            {
                uint8_t langId = 0;
                uint8_t langIndex = USB_DEVICE_STRING_COUNT;

                for (; langId < USB_DEVICE_LANGUAGE_COUNT; langId++)
                {
                    if (setup->wIndex == g_UsbDeviceLanguageList.languageList[langId].languageId)
                    {
                        if (descriptorIndex < USB_DEVICE_STRING_COUNT)
                        {
                            langIndex = descriptorIndex;
                        }
                        break;
                    }
                }

                if (USB_DEVICE_STRING_COUNT == langIndex)
                {
                    langId = 0;
                }
                *buffer = (uint8_t *)g_UsbDeviceLanguageList.languageList[langId].string[langIndex];
                *length = g_UsbDeviceLanguageList.languageList[langId].length[langIndex];
            }
        }
        break;
        case USB_DESCRIPTOR_TYPE_DEVICE:
        {
            *buffer = g_UsbDeviceDescriptor;
            *length = USB_DESCRIPTOR_LENGTH_DEVICE;
        }
        break;
        case USB_DESCRIPTOR_TYPE_CONFIGURE:
        {
            *buffer = g_UsbDeviceConfigurationDescriptor;
            *length = USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL;
        }
        break;
#if (defined(USB_DEVICE_CONFIG_CV_TEST) && (USB_DEVICE_CONFIG_CV_TEST > 0U))
        case USB_DESCRIPTOR_TYPE_DEVICE_QUALITIER:
        {
            /* Get Qualifier descriptor */
            *buffer = g_UsbDeviceQualifierDescriptor;
            *length = USB_DESCRIPTOR_LENGTH_DEVICE_QUALITIER;
        }
        break;
#endif
        default:
            ret = kStatus_USB_InvalidRequest;
            break;
    } /* End Switch */
    return ret;
}

/*!
 * @brief Set the device configuration.
 *
 * The function is used to set the device configuration.
 *
 * @param handle              The device handle.
 * @param configure           The configuration value.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceSetConfigure(usb_device_handle handle, uint8_t configure)
{
    g_currentConfigure = configure;
    return USB_DeviceCallback(handle, kUSB_DeviceEventSetConfiguration, &configure);
}

/*!
 * @brief Get the device configuration.
 *
 * The function is used to get the device configuration.
 *
 * @param handle The device handle.
 * @param configure It is an OUT parameter, save the current configuration value.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetConfigure(usb_device_handle handle, uint8_t *configure)
{
    *configure = g_currentConfigure;
    return kStatus_USB_Success;
}

/*!
 * @brief Set an interface alternate setting.
 *
 * The function is used to set an interface alternate setting.
 *
 * @param handle The device handle.
 * @param interface The interface index.
 * @param alternateSetting The new alternate setting value.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceSetInterface(usb_device_handle handle, uint8_t interface, uint8_t alternateSetting)
{
    g_interface[interface] = alternateSetting;
    return USB_DeviceCallback(handle, kUSB_DeviceEventSetInterface, &interface);
}

/*!
 * @brief Get an interface alternate setting.
 *
 * The function is used to get an interface alternate setting.
 *
 * @param handle The device handle.
 * @param interface The interface index.
 * @param alternateSetting It is an OUT parameter, save the new alternate setting value of the interface.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetInterface(usb_device_handle handle, uint8_t interface, uint8_t *alternateSetting)
{
    *alternateSetting = g_interface[interface];
    return kStatus_USB_Success;
}

/*!
 * @brief USB device set speed function.
 *
 * This function sets the speed of the USB device.
 *
 * Due to the difference of HS and FS descriptors, the device descriptors and configurations need to be updated to match
 * current speed.
 * As the default, the device descriptors and configurations are configured by using FS parameters for both EHCI and
 * KHCI.
 * When the EHCI is enabled, the application needs to call this function to update device by using current speed.
 * The updated information includes endpoint max packet size, endpoint interval, etc.
 *
 * @param handle The USB device handle.
 * @param speed Speed type. USB_SPEED_HIGH/USB_SPEED_FULL/USB_SPEED_LOW.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceSetSpeed(usb_device_handle handle, uint8_t speed)
{
    usb_descriptor_union_t *ptr1;
    usb_descriptor_union_t *ptr2;

    ptr1 = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[0]);
    ptr2 = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL - 1]);

    while (ptr1 < ptr2)
    {
        if (ptr1->common.bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT)
        {
            if (USB_SPEED_HIGH == speed)
            {
                if ((USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT ==
                    (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) && 
                    ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    ptr1->endpoint.bInterval = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                          ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_IN_PACKET_SIZE, ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_OUT_PACKET_SIZE, ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2 ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    ptr1->endpoint.bInterval = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL_2;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_IN_PACKET_SIZE_2,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else
                {
                }
            }
            else
            {
                if ((USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT ==
                    (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                    ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    ptr1->endpoint.bInterval = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_IN_PACKET_SIZE, ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE, ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2 ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) && 
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    ptr1->endpoint.bInterval = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL_2;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_IN_PACKET_SIZE_2,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 ==
                         (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else
                {
                }
            }
        }
        ptr1 = (usb_descriptor_union_t *)((uint8_t *)ptr1 + ptr1->common.bLength);
    }

    return kStatus_USB_Success;
}
