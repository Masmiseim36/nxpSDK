/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_audio.h"
#include "usb_audio_config.h"
#include "usb_device_descriptor.h"
#include "audio_generator.h"

/*******************************************************************************
* Variables
******************************************************************************/
/* hid mouse endpoint information */
usb_device_endpoint_struct_t g_UsbDeviceHidEndpoints[USB_HID_MOUSE_ENDPOINT_COUNT] = {
    {
        /* HID mouse interrupt IN pipe */
        USB_HID_MOUSE_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT), USB_ENDPOINT_INTERRUPT,
        FS_INTERRUPT_IN_PACKET_SIZE,
    },
};

/* HID mouse interface information */
usb_device_interface_struct_t g_UsbDeviceHidInterface[] = {{
    0U, /* The alternate setting of the interface */
    {
        USB_HID_MOUSE_ENDPOINT_COUNT, /* Endpoint count */
        g_UsbDeviceHidEndpoints,      /* Endpoints handle */
    },
    NULL,
}};

usb_device_interfaces_struct_t g_UsbDeviceHidInterfaces[USB_HID_MOUSE_INTERFACE_COUNT] = {{
    USB_HID_MOUSE_CLASS,           /* HID mouse class code */
    USB_HID_MOUSE_SUBCLASS,        /* HID mouse subclass code */
    USB_HID_MOUSE_PROTOCOL,        /* HID mouse protocol code */
    USB_HID_MOUSE_INTERFACE_INDEX, /* The interface number of the HID mouse */
    g_UsbDeviceHidInterface,       /* Interfaces handle */
    sizeof(g_UsbDeviceHidInterface) / sizeof(usb_device_interfaces_struct_t),
}};

usb_device_interface_list_t g_UsbDeviceHidInterfaceList[USB_DEVICE_CONFIGURATION_COUNT] = {
    {
        USB_HID_MOUSE_INTERFACE_COUNT, /* The interface count of the HID mouse */
        g_UsbDeviceHidInterfaces,      /* The interfaces handle */
    },
};

usb_device_class_struct_t g_UsbDeviceHidMouseClass = {
    g_UsbDeviceHidInterfaceList,    /* The interface list of the HID mouse */
    kUSB_DeviceClassTypeHid,        /* The HID class type */
    USB_DEVICE_CONFIGURATION_COUNT, /* The configuration count */
};

/* Audio generator stream endpoint information */
usb_device_endpoint_struct_t g_UsbDeviceAudioGeneratorEndpoints[USB_AUDIO_STREAM_ENDPOINT_COUNT] = {
    /* Audio generator ISO IN pipe */
    {
        USB_AUDIO_STREAM_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
        USB_ENDPOINT_ISOCHRONOUS, FS_ISO_IN_ENDP_PACKET_SIZE,
    },
};

/* Audio generator control endpoint information */
usb_device_endpoint_struct_t g_UsbDeviceAudioControlEndpoints[USB_AUDIO_CONTROL_ENDPOINT_COUNT] = {
    {
        USB_AUDIO_CONTROL_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
        USB_ENDPOINT_INTERRUPT, FS_INTERRUPT_IN_PACKET_SIZE,
    },
};

/* Audio generator entity struct */
usb_device_audio_entity_struct_t g_UsbDeviceAudioEntity[] = {
    {
        USB_AUDIO_CONTROL_INPUT_TERMINAL_ID, USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_INPUT_TERMINAL, 0U,
    },
    {
        USB_AUDIO_CONTROL_FEATURE_UNIT_ID, USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_FEATURE_UNIT, 0U,
    },
    {
        USB_AUDIO_CONTROL_OUTPUT_TERMINAL_ID, USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_OUTPUT_TERMINAL, 0U,
    },
};

/* Audio generator entity information */
usb_device_audio_entities_struct_t g_UsbDeviceAudioEntities = {
    g_UsbDeviceAudioEntity, sizeof(g_UsbDeviceAudioEntity) / sizeof(usb_device_audio_entity_struct_t),
};

/* Audio generator control interface information */
usb_device_interface_struct_t g_UsbDeviceAudioControInterface[] = {{
    0U,
    {
        USB_AUDIO_CONTROL_ENDPOINT_COUNT, g_UsbDeviceAudioControlEndpoints,
    },
    &g_UsbDeviceAudioEntities,
}};

/* Audio generator stream interface information */
usb_device_interface_struct_t g_UsbDeviceAudioStreamInterface[] = {
    {
        0U,
        {
            0U, NULL,
        },
        NULL,
    },
    {
        1U,
        {
            USB_AUDIO_STREAM_ENDPOINT_COUNT, g_UsbDeviceAudioGeneratorEndpoints,
        },
        NULL,
    },
};

/* Define interfaces for audio generator */
usb_device_interfaces_struct_t g_UsbDeviceAudioInterfaces[USB_AUDIO_GENERATOR_INTERFACE_COUNT] = {
    {
        USB_AUDIO_CLASS,                   /* Audio class code */
        USB_SUBCLASS_AUDIOCONTROL,         /* Audio control subclass code */
        USB_AUDIO_PROTOCOL,                /* Audio protocol code */
        USB_AUDIO_CONTROL_INTERFACE_INDEX, /* The interface number of the Audio control */
        g_UsbDeviceAudioControInterface,   /* The handle of Audio control */
        sizeof(g_UsbDeviceAudioControInterface) / sizeof(usb_device_interfaces_struct_t),
    },
    {
        USB_AUDIO_CLASS,                  /* Audio class code */
        USB_SUBCLASS_AUDIOSTREAM,         /* Audio stream subclass code */
        USB_AUDIO_PROTOCOL,               /* Audio protocol code */
        USB_AUDIO_STREAM_INTERFACE_INDEX, /* The interface number of the Audio control */
        g_UsbDeviceAudioStreamInterface,  /* The handle of Audio stream */
        sizeof(g_UsbDeviceAudioStreamInterface) / sizeof(usb_device_interfaces_struct_t),
    }

};

/* Define configurations for audio generator */
usb_device_interface_list_t g_UsbDeviceAudioInterfaceList[USB_DEVICE_CONFIGURATION_COUNT] = {
    {
        USB_AUDIO_GENERATOR_INTERFACE_COUNT, g_UsbDeviceAudioInterfaces,
    },
};

/* Define class information for audio generator */
usb_device_class_struct_t g_UsbDeviceAudioClass = {
    g_UsbDeviceAudioInterfaceList, kUSB_DeviceClassTypeAudio, USB_DEVICE_CONFIGURATION_COUNT,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceHidMouseReportDescriptor[] = {
    0x05U, 0x01U, /* Usage Page (Generic Desktop)*/
    0x09U, 0x02U, /* Usage (Mouse) */
    0xA1U, 0x01U, /* Collection (Application) */
    0x09U, 0x01U, /* Usage (Pointer) */

    0xA1U, 0x00U, /* Collection (Physical) */
    0x05U, 0x09U, /* Usage Page (Buttons) */
    0x19U, 0x01U, /* Usage Minimum (01U) */
    0x29U, 0x03U, /* Usage Maximum (03U) */

    0x15U, 0x00U, /* Logical Minimum (0U) */
    0x25U, 0x01U, /* Logical Maximum (1U) */
    0x95U, 0x03U, /* Report Count (3U) */
    0x75U, 0x01U, /* Report Size (1U) */

    0x81U, 0x02U, /* Input(Data, Variable, Absolute) 3U button bit fields */
    0x95U, 0x01U, /* Report Count (1U) */
    0x75U, 0x05U, /* Report Size (5U) */
    0x81U, 0x01U, /* Input (Constant), 5U constant field */

    0x05U, 0x01U, /* Usage Page (Generic Desktop) */
    0x09U, 0x30U, /* Usage (X) */
    0x09U, 0x31U, /* Usage (Y) */
    0x09U, 0x38U, /* Usage (Z) */

    0x15U, 0x81U, /* Logical Minimum (-127) */
    0x25U, 0x7FU, /* Logical Maximum (127) */
    0x75U, 0x08U, /* Report Size (8U) */
    0x95U, 0x03U, /* Report Count (3U) */

    0x81U, 0x06U, /* Input(Data, Variable, Relative), Three position bytes (X & Y & Z)*/
    0xC0U,        /* End collection, Close Pointer collection*/
    0xC0U         /* End collection, Close Mouse collection */
};

/* Define device descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceDescriptor[] = {
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
    0x9DU, 0x00U,                                        /* Product ID (assigned by the manufacturer) */
    USB_SHORT_GET_LOW(USB_DEVICE_DEMO_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_DEMO_BCD_VERSION), /* Device release number in binary-coded decimal */
    0x01U,                                           /* Index of string descriptor describing manufacturer */
    0x02U,                                           /* Index of string descriptor describing product */
    0x00U,                                           /* Index of string descriptor describing the
                                                        device's serial number */
    USB_DEVICE_CONFIGURATION_COUNT,                  /* Number of possible configurations */
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceConfigurationDescriptor[] = {
    USB_DESCRIPTOR_LENGTH_CONFIGURE, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CONFIGURE,   /* CONFIGURATION Descriptor Type */
    USB_SHORT_GET_LOW(USB_DESCRIPTOR_LENGTH_CONFIGURE + USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_AUDIO_CONTROL_INTERFACE_HEADER_LENGTH + USB_AUDIO_INPUT_TERMINAL_ONLY_DESC_SIZE +
                      USB_AUDIO_FEATURE_UNIT_ONLY_DESC_SIZE + USB_AUDIO_OUTPUT_TERMINAL_ONLY_DESC_SIZE +
                      USB_DESCRIPTOR_LENGTH_AC_INTERRUPT_ENDPOINT + USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_DESCRIPTOR_LENGTH_INTERFACE + USB_AUDIO_STREAMING_IFACE_DESC_SIZE +
                      USB_AUDIO_STREAMING_TYPE_I_DESC_SIZE + USB_ENDPOINT_AUDIO_DESCRIPTOR_LENGTH +
                      USB_AUDIO_STREAMING_ENDP_DESC_SIZE + USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_HID +
                      USB_DESCRIPTOR_LENGTH_ENDPOINT),
    USB_SHORT_GET_HIGH(
        USB_DESCRIPTOR_LENGTH_CONFIGURE + USB_DESCRIPTOR_LENGTH_INTERFACE + USB_AUDIO_CONTROL_INTERFACE_HEADER_LENGTH +
        USB_AUDIO_INPUT_TERMINAL_ONLY_DESC_SIZE + USB_AUDIO_FEATURE_UNIT_ONLY_DESC_SIZE +
        USB_AUDIO_OUTPUT_TERMINAL_ONLY_DESC_SIZE + USB_DESCRIPTOR_LENGTH_AC_INTERRUPT_ENDPOINT +
        USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_INTERFACE + USB_AUDIO_STREAMING_IFACE_DESC_SIZE +
        USB_AUDIO_STREAMING_TYPE_I_DESC_SIZE + USB_ENDPOINT_AUDIO_DESCRIPTOR_LENGTH +
        USB_AUDIO_STREAMING_ENDP_DESC_SIZE + USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_HID +
        USB_DESCRIPTOR_LENGTH_ENDPOINT), /* Total length of data returned for this configuration. */
    USB_DEVICE_INTERFACE_COUNT,          /* Number of interfaces supported by this configuration */
    USB_COMPOSITE_CONFIGURE_INDEX,       /* Value to use as an argument to the
                                            SetConfiguration() request to select this configuration */
    0x00U,                               /* Index of string descriptor describing this configuration */
    (USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_D7_MASK) |
        (USB_DEVICE_CONFIG_SELF_POWER << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_SELF_POWERED_SHIFT) |
        (USB_DEVICE_CONFIG_REMOTE_WAKEUP << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_REMOTE_WAKEUP_SHIFT),
    /* Configuration characteristics
       D7: Reserved (set to one)
       D6: Self-powered
       D5: Remote Wakeup
       D4...0: Reserved (reset to zero)
    */
    USB_DEVICE_MAX_POWER, /* Maximum power consumption of the USB
                           * device from the bus in this specific
                           * configuration when the device is fully
                           * operational. Expressed in 2 mA units
                           *  (i.e., 50 = 100 mA).
                           */

    USB_DESCRIPTOR_LENGTH_INTERFACE,   /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_INTERFACE,     /* INTERFACE Descriptor Type */
    USB_AUDIO_CONTROL_INTERFACE_INDEX, /* Number of this interface. */
    0x00U,                             /* Value used to select this alternate setting
                                          for the interface identified in the prior field */
    0x01U,                             /* Number of endpoints used by this
                                          interface (excluding endpoint zero). */
    USB_AUDIO_CLASS,                   /*The interface implements the Audio Interface class  */
    USB_SUBCLASS_AUDIOCONTROL,         /*The interface implements the AUDIOCONTROL Subclass  */
    0x00U,                             /*The interface doesn't use any class-specific protocols  */
    0x00U,                             /* The device doesn't have a string descriptor describing this iInterface  */

    /* Audio Class Specific type of INTERFACE Descriptor */
    USB_AUDIO_CONTROL_INTERFACE_HEADER_LENGTH,   /* Size of the descriptor, in bytes  */
    USB_DESCRIPTOR_TYPE_AUDIO_CS_INTERFACE,      /* CS_INTERFACE Descriptor Type   */
    USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_HEADER, /* HEADER descriptor subtype   */
    0x00U, 0x01U, /* Audio Device compliant to the USB Audio specification version 1.00  */
    0x27, 0x00U,  /* Total number of bytes returned for the class-specific AudioControl interface descriptor.
                     Includes the combined length of this descriptor header and all Unit and Terminal
                     descriptors. */
    0x01U,        /* The number of AudioStreaming and MIDIStreaming interfaces in the Audio Interface Collection to
                     which this AudioControl interface belongs   */
    0x01U,        /* The number of AudioStreaming and MIDIStreaming interfaces in the Audio Interface baNumber */

    /* Audio Class Specific type of Input Terminal*/
    USB_AUDIO_INPUT_TERMINAL_ONLY_DESC_SIZE, /* Size of the descriptor, in bytes  */
    USB_DESCRIPTOR_TYPE_AUDIO_CS_INTERFACE,  /* CS_INTERFACE Descriptor Type   */
    USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_INPUT_TERMINAL,
    /* INPUT_TERMINAL descriptor subtype  */
    0x01U,        /* Constant uniquely identifying the Terminal within the audio function. This value is used in all
                     requests to address this Terminal.  */
    0x01U, 0x02,  /* A generic microphone that does not fit under any of the other classifications.  */
    0x00U,        /* This Input Terminal has no association  */
    0x01U,        /* This Terminal's output audio channel cluster has 1 logical output channels  */
    0x00U, 0x00U, /* Spatial locations present in the cluster */
    0x00U,        /* Index of a string descriptor, describing the name of the first logical channel.   */
    0x00U,        /* Index of a string descriptor, describing the Input Terminal.   */

    /* Audio Class Specific type of Feature Unit */
    USB_AUDIO_FEATURE_UNIT_ONLY_DESC_SIZE,             /* Size of the descriptor, in bytes   */
    USB_DESCRIPTOR_TYPE_AUDIO_CS_INTERFACE,            /* CS_INTERFACE Descriptor Type  */
    USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_FEATURE_UNIT, /* FEATURE_UNIT descriptor subtype   */
    0x02,        /* Constant uniquely identifying the Unit within the audio function. This value is used in all
                    requests to address this Unit.  */
    0x01U,       /* ID of the Unit or Terminal to which this Feature Unit is connected.    */
    0x01U,       /* Size in bytes of an element of the bmaControls() array:  */
    0x03, 0x00U, /* Master channel controls */
    0x00U,       /* Index of a string descriptor, describing this Feature Unit.   */

    /* Audio Class Specific type of  Output Terminal */
    USB_AUDIO_OUTPUT_TERMINAL_ONLY_DESC_SIZE, /* Size of the descriptor, in bytes  */
    USB_DESCRIPTOR_TYPE_AUDIO_CS_INTERFACE,   /* CS_INTERFACE Descriptor Type   */
    USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_OUTPUT_TERMINAL,
    /* OUTPUT_TERMINAL descriptor subtype  */
    0x03,         /* Constant uniquely identifying the Terminal within the audio function*/
    0x01U, 0x01U, /* A Terminal dealing with a signal carried over an endpoint in an AudioStreaming interface */
    0x00U,        /*  This Output Terminal has no association   */
    0x02,         /* ID of the Unit or Terminal to which this Terminal is connected.   */
    0x00U,        /* Index of a string descriptor, describing the Output Terminal.  */

    USB_DESCRIPTOR_LENGTH_AC_INTERRUPT_ENDPOINT, /* Size of this descriptor, in bytes: 9U */
    USB_DESCRIPTOR_TYPE_ENDPOINT,                /* ENDPOINT descriptor type */
    USB_AUDIO_CONTROL_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    /* Endpoint address */
    USB_ENDPOINT_INTERRUPT, /* Transfer type */
    USB_SHORT_GET_LOW(FS_INTERRUPT_IN_PACKET_SIZE), USB_SHORT_GET_HIGH(FS_INTERRUPT_IN_PACKET_SIZE),
    /* Max Packet Size */
    FS_INTERRUPT_IN_INTERVAL, /* Interval */
    0, 0,

    /* Audio Class Specific INTERFACE Descriptor, alternative interface 0  */
    USB_DESCRIPTOR_LENGTH_INTERFACE,  /* Descriptor size is 9 bytes  */
    USB_DESCRIPTOR_TYPE_INTERFACE,    /* INTERFACE Descriptor Type   */
    USB_AUDIO_STREAM_INTERFACE_INDEX, /* The number of this interface is 1.  */
    0x00U,                            /* The value used to select the alternate setting for this interface is 0   */
    0x00U,                    /* The number of endpoints used by this interface is 0 (excluding endpoint zero)   */
    USB_AUDIO_CLASS,          /* The interface implements the Audio Interface class   */
    USB_SUBCLASS_AUDIOSTREAM, /* The interface implements the AUDIOSTREAMING Subclass   */
    0x00U,                    /* The interface doesn't use any class-specific protocols   */
    0x00U,                    /* The device doesn't have a string descriptor describing this iInterface  */

    /* Audio Class Specific INTERFACE Descriptor, alternative interface 1 */
    USB_DESCRIPTOR_LENGTH_INTERFACE,  /* Descriptor size is 9 bytes  */
    USB_DESCRIPTOR_TYPE_INTERFACE,    /* INTERFACE Descriptor Type  */
    USB_AUDIO_STREAM_INTERFACE_INDEX, /*The number of this interface is 1.  */
    0x01U,                            /* The value used to select the alternate setting for this interface is 1  */
    0x01U,                    /* The number of endpoints used by this interface is 1 (excluding endpoint zero)    */
    USB_AUDIO_CLASS,          /* The interface implements the Audio Interface class   */
    USB_SUBCLASS_AUDIOSTREAM, /* The interface implements the AUDIOSTREAMING Subclass   */
    0x00U,                    /* The interface doesn't use any class-specific protocols  */
    0x00U,                    /* The device doesn't have a string descriptor describing this iInterface  */

    /* Audio Class Specific CS INTERFACE Descriptor*/
    USB_AUDIO_STREAMING_IFACE_DESC_SIZE,            /* Size of the descriptor, in bytes  */
    USB_DESCRIPTOR_TYPE_AUDIO_CS_INTERFACE,         /* CS_INTERFACE Descriptor Type  */
    USB_DESCRIPTOR_SUBTYPE_AUDIO_STREAMING_GENERAL, /* AS_GENERAL descriptor subtype  */
    0x03U,        /* The Terminal ID of the Terminal to which the endpoint of this interface is connected. */
    0x00U,        /* Delay introduced by the data path. Expressed in number of frames.  */
    0x02U, 0x00U, /* PCM8  */

    /* Audio Class Specific type I format INTERFACE Descriptor */
    USB_AUDIO_STREAMING_TYPE_I_DESC_SIZE,   /* Size of the descriptor, in bytes  */
    USB_DESCRIPTOR_TYPE_AUDIO_CS_INTERFACE, /* CS_INTERFACE Descriptor Type   */
    USB_DESCRIPTOR_SUBTYPE_AUDIO_STREAMING_FORMAT_TYPE,
    /* FORMAT_TYPE descriptor subtype  */
    USB_AUDIO_FORMAT_TYPE_I, /* FORMAT_TYPE_I  */
    0x01U,                   /* Indicates the number of physical channels in the audio data stream.  */
#if defined(AUDIO_DATA_SOURCE_DMIC) && (AUDIO_DATA_SOURCE_DMIC > 0U)
    0x02U,             /* The number of bytes occupied by one audio subframe. Can be 1, 2, 3 or 4.   */
    0x10,              /* The number of effectively used bits from the available bits in an audio subframe.*/
    0x01U,             /* Indicates how the sampling frequency can be programmed:   */
    0x80, 0x3E, 0x00U, /* Sampling frequency 1 in Hz for this isochronous data endpoint.   */
#else
    0x01U,             /* The number of bytes occupied by one audio subframe. Can be 1, 2, 3 or 4.   */
    0x08,              /* The number of effectively used bits from the available bits in an audio subframe.*/
    0x01U,             /* Indicates how the sampling frequency can be programmed:   */
    0x40, 0x1F, 0x00U, /* Sampling frequency 1 in Hz for this isochronous data endpoint.   */
#endif
    /* ENDPOINT Descriptor */
    USB_ENDPOINT_AUDIO_DESCRIPTOR_LENGTH,      /* Descriptor size is 9 bytes  */
    USB_DESCRIPTOR_TYPE_ENDPOINT,              /* ENDPOINT Descriptor Type   */
    USB_AUDIO_STREAM_ENDPOINT | (USB_IN << 7), /* This is an IN endpoint with endpoint number 2   */
    USB_ENDPOINT_ISOCHRONOUS,                  /* Types - Transfer: ISOCHRONOUS */
    USB_SHORT_GET_LOW(FS_ISO_IN_ENDP_PACKET_SIZE),
    USB_SHORT_GET_HIGH(FS_ISO_IN_ENDP_PACKET_SIZE), /* Maximum packet size for this endpoint is 8 Bytes.  */
    ISO_IN_ENDP_INTERVAL, /* The polling interval value is every 1 Frames. If Hi-Speed, every 1 uFrames   */
    0x00U,                /* Refresh Rate 2**n ms where n = 0   */
    0x00U,                /* Synchronization Endpoint (if used) is endpoint 0   */

    /* Audio Class Specific ENDPOINT Descriptor  */
    USB_AUDIO_STREAMING_ENDP_DESC_SIZE,      /*  Size of the descriptor, in bytes  */
    USB_AUDIO_STREAM_ENDPOINT_DESCRIPTOR,    /* CS_ENDPOINT Descriptor Type  */
    USB_AUDIO_EP_GENERAL_DESCRIPTOR_SUBTYPE, /* AUDIO_EP_GENERAL descriptor subtype  */
    0x00U,                                   /* Bit 0: Sampling Frequency 0
                                                Bit 1: Pitch 0
                                                Bit 7: MaxPacketsOnly 0   */
    0x00U,                                   /* Indicates the units used for the wLockDelay field: 0: Undefined  */
    0x00U, 0x00U, /* Indicates the time it takes this endpoint to reliably lock its internal clock recovery
                     circuitry */

    /* Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_INTERFACE,   /* INTERFACE Descriptor Type */
    0x02,                            /* Number of this interface. */
    0x00U,                           /* Value used to select this alternate setting
                                        for the interface identified in the prior field */
    USB_HID_MOUSE_ENDPOINT_COUNT,    /* Number of endpoints used by this
                                        interface (excluding endpoint zero). */
    USB_HID_MOUSE_CLASS,             /* Class code (assigned by the USB-IF). */
    USB_HID_MOUSE_SUBCLASS,          /* Subclass code (assigned by the USB-IF). */
    USB_HID_MOUSE_PROTOCOL,          /* Protocol code (assigned by the USB). */
    0x00U,                           /* Index of string descriptor describing this interface */

    USB_DESCRIPTOR_LENGTH_HID, /* Numeric expression that is the total size of the HID descriptor. */
    USB_DESCRIPTOR_TYPE_HID,   /* Constant name specifying type of HID descriptor. */
    0x00U, 0x01U,              /* Numeric expression identifying the HID Class Specification release. */
    0x00U,                     /* Numeric expression identifying country code of the localized hardware */
    0x01U, /* Numeric expression specifying the number of class descriptors(at least one report descriptor) */
    USB_DESCRIPTOR_TYPE_HID_REPORT, /* Constant name identifying type of class descriptor. */
    USB_SHORT_GET_LOW(USB_DESCRIPTOR_LENGTH_HID_MOUSE_REPORT),
    USB_SHORT_GET_HIGH(USB_DESCRIPTOR_LENGTH_HID_MOUSE_REPORT),
    /* Numeric expression that is the total size of the Report descriptor. */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* ENDPOINT Descriptor Type */
    USB_HID_MOUSE_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    /* The address of the endpoint on the USB device
       described by this descriptor. */
    USB_ENDPOINT_INTERRUPT, /* This field describes the endpoint's attributes */
    USB_SHORT_GET_LOW(FS_INTERRUPT_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(FS_INTERRUPT_IN_PACKET_SIZE), /* Maximum packet size this endpoint is capable of
                                                        sending or receiving when this configuration is selected. */
    FS_INTERRUPT_IN_INTERVAL,                        /* Interval for polling endpoint for data transfers. */
};

#if (defined(USB_DEVICE_CONFIG_CV_TEST) && (USB_DEVICE_CONFIG_CV_TEST > 0U))
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceQualifierDescriptor[] = {
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
    2U + 2U, USB_DESCRIPTOR_TYPE_STRING, 0x09U, 0x04U,
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
uint8_t g_UsbDeviceString2[] = {
    2U + 2U * 18U, USB_DESCRIPTOR_TYPE_STRING,
    'U',           0x00U,
    'S',           0x00U,
    'B',           0x00U,
    ' ',           0x00U,
    'A',           0x00U,
    'U',           0x00U,
    'D',           0x00U,
    'I',           0x00U,
    'O',           0x00U,
    '+',           0x00U,
    'H',           0x00U,
    'I',           0x00U,
    'D',           0x00U,
    ' ',           0x00U,
    'D',           0x00U,
    'E',           0x00U,
    'M',           0x00U,
    'O',           0x00U,
};

uint32_t g_UsbDeviceStringDescriptorLength[USB_DEVICE_STRING_COUNT] = {
    sizeof(g_UsbDeviceString0), sizeof(g_UsbDeviceString1), sizeof(g_UsbDeviceString2),
};

uint8_t *g_UsbDeviceStringDescriptorArray[USB_DEVICE_STRING_COUNT] = {
    g_UsbDeviceString0, g_UsbDeviceString1, g_UsbDeviceString2,
};

usb_language_t g_UsbDeviceLanguage[USB_DEVICE_LANGUAGE_COUNT] = {{
    g_UsbDeviceStringDescriptorArray, g_UsbDeviceStringDescriptorLength, (uint16_t)0x0409U,
}};

usb_language_list_t g_UsbDeviceLanguageList = {
    g_UsbDeviceString0, sizeof(g_UsbDeviceString0), g_UsbDeviceLanguage, USB_DEVICE_LANGUAGE_COUNT,
};

/*******************************************************************************
* Code
******************************************************************************/
/*!
 * @brief USB device get device descriptor function.
 *
 * This function gets the device descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param deviceDescriptor The pointer to the device descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetDeviceDescriptor(usb_device_handle handle,
                                           usb_device_get_device_descriptor_struct_t *deviceDescriptor)
{
    deviceDescriptor->buffer = g_UsbDeviceDescriptor;
    deviceDescriptor->length = USB_DESCRIPTOR_LENGTH_DEVICE;
    return kStatus_USB_Success;
}
#if (defined(USB_DEVICE_CONFIG_CV_TEST) && (USB_DEVICE_CONFIG_CV_TEST > 0U))
/* Get device qualifier descriptor request */
usb_status_t USB_DeviceGetDeviceQualifierDescriptor(
    usb_device_handle handle, usb_device_get_device_qualifier_descriptor_struct_t *deviceQualifierDescriptor)
{
    deviceQualifierDescriptor->buffer = g_UsbDeviceQualifierDescriptor;
    deviceQualifierDescriptor->length = USB_DESCRIPTOR_LENGTH_DEVICE_QUALITIER;
    return kStatus_USB_Success;
}
#endif
/*!
 * @brief USB device get configuration descriptor function.
 *
 * This function gets the configuration descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param configurationDescriptor The pointer to the configuration descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetConfigurationDescriptor(
    usb_device_handle handle, usb_device_get_configuration_descriptor_struct_t *configurationDescriptor)
{
    if (USB_COMPOSITE_CONFIGURE_INDEX > configurationDescriptor->configuration)
    {
        configurationDescriptor->buffer = g_UsbDeviceConfigurationDescriptor;
        configurationDescriptor->length = USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL;
        return kStatus_USB_Success;
    }
    return kStatus_USB_InvalidRequest;
}

/*!
 * @brief USB device get string descriptor function.
 *
 * This function gets the string descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param stringDescriptor The pointer to the string descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetStringDescriptor(usb_device_handle handle,
                                           usb_device_get_string_descriptor_struct_t *stringDescriptor)
{
    if (stringDescriptor->stringIndex == 0U)
    {
        stringDescriptor->buffer = (uint8_t *)g_UsbDeviceLanguageList.languageString;
        stringDescriptor->length = g_UsbDeviceLanguageList.stringLength;
    }
    else
    {
        uint8_t languageId = 0U;
        uint8_t languageIndex = USB_DEVICE_STRING_COUNT;

        for (; languageId < USB_DEVICE_LANGUAGE_COUNT; languageId++)
        {
            if (stringDescriptor->languageId == g_UsbDeviceLanguageList.languageList[languageId].languageId)
            {
                if (stringDescriptor->stringIndex < USB_DEVICE_STRING_COUNT)
                {
                    languageIndex = stringDescriptor->stringIndex;
                }
                break;
            }
        }

        if (USB_DEVICE_STRING_COUNT == languageIndex)
        {
            return kStatus_USB_InvalidRequest;
        }
        stringDescriptor->buffer = (uint8_t *)g_UsbDeviceLanguageList.languageList[languageId].string[languageIndex];
        stringDescriptor->length = g_UsbDeviceLanguageList.languageList[languageId].length[languageIndex];
    }
    return kStatus_USB_Success;
}

/* Get hid report descriptor request */
usb_status_t USB_DeviceGetHidReportDescriptor(usb_device_handle handle,
                                              usb_device_get_hid_report_descriptor_struct_t *hidReportDescriptor)
{
    if (USB_HID_MOUSE_INTERFACE_INDEX == hidReportDescriptor->interfaceNumber)
    {
        hidReportDescriptor->buffer = g_UsbDeviceHidMouseReportDescriptor;
        hidReportDescriptor->length = USB_DESCRIPTOR_LENGTH_HID_MOUSE_REPORT;
    }
    else
    {
        return kStatus_USB_InvalidRequest;
    }
    return kStatus_USB_Success;
}

/* Due to the difference of HS and FS descriptors, the device descriptors and configurations need to be updated to match
 * current speed.
 * As the default, the device descriptors and configurations are configured by using FS parameters for both EHCI and
 * KHCI.
 * When the EHCI is enabled, the application needs to call this function to update device by using current speed.
 * The updated information includes endpoint max packet size, endpoint interval, etc. */
usb_status_t USB_DeviceSetSpeed(usb_device_handle handle, uint8_t speed)
{
    usb_descriptor_union_t *descriptorHead;
    usb_descriptor_union_t *descriptorTail;

    descriptorHead = (usb_descriptor_union_t *)&g_UsbDeviceConfigurationDescriptor[0];
    descriptorTail =
        (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL - 1U]);

    while (descriptorHead < descriptorTail)
    {
        if (descriptorHead->common.bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT)
        {
            if (USB_SPEED_HIGH == speed)
            {
                if (USB_AUDIO_STREAM_ENDPOINT == (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK))
                {
                    descriptorHead->endpoint.bInterval = HS_ISO_IN_ENDP_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_ISO_IN_ENDP_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if (USB_HID_MOUSE_ENDPOINT ==
                         (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK))
                {
                    descriptorHead->endpoint.bInterval = HS_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_INTERRUPT_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
            }
            else
            {
                if (USB_AUDIO_STREAM_ENDPOINT == (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK))
                {
                    descriptorHead->endpoint.bInterval = FS_ISO_IN_ENDP_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_ISO_IN_ENDP_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if (USB_HID_MOUSE_ENDPOINT ==
                         (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK))
                {
                    descriptorHead->endpoint.bInterval = FS_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_INTERRUPT_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
            }
        }
        descriptorHead = (usb_descriptor_union_t *)((uint8_t *)descriptorHead + descriptorHead->common.bLength);
    }

    for (int i = 0U; i < USB_AUDIO_ENDPOINT_COUNT; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            g_UsbDeviceAudioGeneratorEndpoints[i].maxPacketSize = HS_ISO_IN_ENDP_PACKET_SIZE;
        }
        else
        {
            g_UsbDeviceAudioGeneratorEndpoints[i].maxPacketSize = FS_ISO_IN_ENDP_PACKET_SIZE;
        }
    }

    for (int i = 0U; i < USB_HID_MOUSE_ENDPOINT_COUNT; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            g_UsbDeviceHidEndpoints[i].maxPacketSize = HS_INTERRUPT_IN_PACKET_SIZE;
        }
        else
        {
            g_UsbDeviceHidEndpoints[i].maxPacketSize = FS_INTERRUPT_IN_PACKET_SIZE;
        }
    }

    return kStatus_USB_Success;
}
