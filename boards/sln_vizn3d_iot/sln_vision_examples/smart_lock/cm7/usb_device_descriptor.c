/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 , 2018 - 2019, 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_video.h"

#include "usb_device_descriptor.h"
#include "virtual_camera.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint16_t s_total_size = 9;
/* cdc virtual com information */
/* Define endpoint for communication class */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_endpoint_struct_t g_cdcVcomCicEndpoints[USB_CDC_VCOM_CIC_ENDPOINT_COUNT] = {
    {
        USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT | (USB_IN << 7U),
        USB_ENDPOINT_INTERRUPT,
        FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
    },
};

/* Video control endpoint information */
usb_device_endpoint_struct_t g_UsbDeviceVideoControlEndpoints[USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT_COUNT] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
        USB_ENDPOINT_INTERRUPT,
        FS_INTERRUPT_IN_PACKET_SIZE,
        FS_INTERRUPT_IN_INTERVAL,
    },
};

/* Define endpoint for data class */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_endpoint_struct_t g_cdcVcomDicEndpoints[USB_CDC_VCOM_DIC_ENDPOINT_COUNT] = {
    {
        USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT | (USB_IN << 7U),
        USB_ENDPOINT_BULK,
        FS_CDC_VCOM_BULK_IN_PACKET_SIZE,
    },
    {
        USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT | (USB_OUT << 7U),
        USB_ENDPOINT_BULK,
        FS_CDC_VCOM_BULK_OUT_PACKET_SIZE,
    },
};

/* Define interface for communication class */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_interface_struct_t g_cdcVcomCicInterface[] = {{0,
                                                          {
                                                              USB_CDC_VCOM_CIC_ENDPOINT_COUNT,
                                                              g_cdcVcomCicEndpoints,
                                                          },
                                                          NULL}};

/* Define interface for data class */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_interface_struct_t g_cdcVcomDicInterface[] = {{0,
                                                          {
                                                              USB_CDC_VCOM_DIC_ENDPOINT_COUNT,
                                                              g_cdcVcomDicEndpoints,
                                                          },
                                                          NULL}};

/* Define interfaces for virtual com */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_interfaces_struct_t g_cdcVcomInterfaces[USB_CDC_VCOM_INTERFACE_COUNT] = {
    {USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS, USB_CDC_VCOM_CIC_PROTOCOL, USB_CDC_VCOM_CIC_INTERFACE_INDEX,
     g_cdcVcomCicInterface, sizeof(g_cdcVcomCicInterface) / sizeof(usb_device_interfaces_struct_t)},
    {USB_CDC_VCOM_DIC_CLASS, USB_CDC_VCOM_DIC_SUBCLASS, USB_CDC_VCOM_DIC_PROTOCOL, USB_CDC_VCOM_DIC_INTERFACE_INDEX,
     g_cdcVcomDicInterface, sizeof(g_cdcVcomDicInterface) / sizeof(usb_device_interfaces_struct_t)},
};

/* Define configurations for virtual com */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_interface_list_t g_UsbDeviceCdcVcomInterfaceList[USB_DEVICE_CONFIGURATION_COUNT] = {
    {
        USB_CDC_VCOM_INTERFACE_COUNT,
        g_cdcVcomInterfaces,
    },
};

/* cdc virtual com 2 information */
/* Define endpoint for communication class */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_endpoint_struct_t g_cdcVcomCicEndpoints_2[USB_CDC_VCOM_CIC_ENDPOINT_COUNT_2] = {
    {
        USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2 | (USB_IN << 7U),
        USB_ENDPOINT_INTERRUPT,
        FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
    },
};

/* Define endpoint for data class */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_endpoint_struct_t g_cdcVcomDicEndpoints_2[USB_CDC_VCOM_DIC_ENDPOINT_COUNT_2] = {
    {
        USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 | (USB_IN << 7U),
        USB_ENDPOINT_BULK,
        FS_CDC_VCOM_BULK_IN_PACKET_SIZE,
    },
    {
        USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 | (USB_OUT << 7U),
        USB_ENDPOINT_BULK,
        FS_CDC_VCOM_BULK_OUT_PACKET_SIZE,
    },
};

/* Define interface for communication class */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_interface_struct_t g_cdcVcomCicInterface_2[] = {{0,
                                                            {
                                                                USB_CDC_VCOM_CIC_ENDPOINT_COUNT_2,
                                                                g_cdcVcomCicEndpoints_2,
                                                            },
                                                            NULL}};

/* Define interface for data class */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_interface_struct_t g_cdcVcomDicInterface_2[] = {{0,
                                                            {
                                                                USB_CDC_VCOM_DIC_ENDPOINT_COUNT_2,
                                                                g_cdcVcomDicEndpoints_2,
                                                            },
                                                            NULL}};

/* Define interfaces for virtual com */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_interfaces_struct_t g_cdcVcomInterfaces_2[USB_CDC_VCOM_INTERFACE_COUNT_2] = {
    {USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS, USB_CDC_VCOM_CIC_PROTOCOL, USB_CDC_VCOM_CIC_INTERFACE_INDEX_2,
     g_cdcVcomCicInterface_2, sizeof(g_cdcVcomCicInterface_2) / sizeof(usb_device_interfaces_struct_t)},
    {USB_CDC_VCOM_DIC_CLASS, USB_CDC_VCOM_DIC_SUBCLASS, USB_CDC_VCOM_DIC_PROTOCOL, USB_CDC_VCOM_DIC_INTERFACE_INDEX_2,
     g_cdcVcomDicInterface_2, sizeof(g_cdcVcomDicInterface_2) / sizeof(usb_device_interfaces_struct_t)},
};

/* Define configurations for virtual com */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_interface_list_t g_UsbDeviceCdcVcomInterfaceList_2[USB_DEVICE_CONFIGURATION_COUNT] = {
    {
        USB_CDC_VCOM_INTERFACE_COUNT_2,
        g_cdcVcomInterfaces_2,
    },
};

/* Video device entities */
usb_device_video_entity_struct_t g_UsbDeviceVideoEntity[] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_ID,   /* The ID of input terminal */
        USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_INPUT_TERMINAL,  /* Entity sub type type is VC input terminal */
        USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_TYPE, /* VC intput terminal type */
    },
    {
        USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_ID,  /* The ID of output terminal */
        USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_OUTPUT_TERMINAL, /* Entity sub type type is VC output terminal */
        0U,
    },
    {
        USB_VIDEO_VIRTUAL_CAMERA_VC_PROCESSING_UNIT_ID,  /* The ID of processing terminal */
        USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_PROCESSING_UNIT, /* Entity sub type type is VC processing terminal */
        0U,
    },
};

/* Video device enetitis list */
usb_device_video_entities_struct_t g_UsbDeviceVideoEntities = {
    g_UsbDeviceVideoEntity,
    sizeof(g_UsbDeviceVideoEntity) / sizeof(usb_device_video_entity_struct_t),
};

/* Video device control interface */
usb_device_interface_struct_t g_UsbDeviceVideoControlInterface[] = {{
    0U, /* Alternate setting value */
    {
        USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT_COUNT, /* control endpoint count */
        g_UsbDeviceVideoControlEndpoints,                /* control endpoint list */
    },
    &g_UsbDeviceVideoEntities, /* video device entitis list pointer */
}};

/* Video stream endpoint information */
usb_device_endpoint_struct_t g_UsbDeviceVideoStreamEndpoints[USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_COUNT] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
        USB_ENDPOINT_ISOCHRONOUS,
        FS_STREAM_IN_PACKET_SIZE,
        FS_STREAM_IN_INTERVAL,
    },
};

/* Video device stream interface */
usb_device_interface_struct_t g_UsbDeviceVideoStreamInterface[] = {
    {
        0U, /* Alternate setting value is zero*/
        {
            0U, /* endpoint count is zero for this alternate setting */
            NULL,
        },
        NULL,
    },
    {
        1U, /* Alternate setting value is one */
        {
            USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_COUNT, /* endpoint count is zero for this alternate setting */
            g_UsbDeviceVideoStreamEndpoints,                /* stream endpoint list */
        },
        NULL,
    },
};

/* The video device interfaces */
usb_device_interfaces_struct_t g_UsbDeviceVideoInterfaces[USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT] = {
    {
        USB_DEVICE_VIDEO_CC_VIDEO,                        /* Class code */
        USB_DEVICE_VIDEO_SC_VIDEOCONTROL,                 /* Subclass code */
        USB_DEVICE_VIDEO_PC_PROTOCOL_UNDEFINED,           /* Protocol code */
        USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_INDEX, /* The control interface index */
        g_UsbDeviceVideoControlInterface,                 /* control interface list */
        sizeof(g_UsbDeviceVideoControlInterface) / sizeof(usb_device_interface_struct_t),
    },
    {
        USB_DEVICE_VIDEO_CC_VIDEO,                       /* Class code */
        USB_DEVICE_VIDEO_SC_VIDEOSTREAMING,              /* Subclass code */
        USB_DEVICE_VIDEO_PC_PROTOCOL_UNDEFINED,          /* Protocol code */
        USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX, /* The stream interface index */
        g_UsbDeviceVideoStreamInterface,                 /* stream interface list */
        sizeof(g_UsbDeviceVideoStreamInterface) / sizeof(usb_device_interface_struct_t),
    },
};

/* The video device interface list */
usb_device_interface_list_t g_UsbDeviceVideoInterfaceList[USB_DEVICE_CONFIGURATION_COUNT] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT, /* the interface count */
        g_UsbDeviceVideoInterfaces,               /* the control and stream interfaces pointer */
    },
};

/* Define class information for virtual com */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_device_class_struct_t g_UsbDeviceCdcVcomConfig[2] = {
    {
        g_UsbDeviceCdcVcomInterfaceList,
        kUSB_DeviceClassTypeCdc,
        USB_DEVICE_CONFIGURATION_COUNT,
    },
    {
        g_UsbDeviceCdcVcomInterfaceList_2,
        kUSB_DeviceClassTypeCdc,
        USB_DEVICE_CONFIGURATION_COUNT,
    },
};

/* The video device all interface list */
usb_device_class_struct_t g_UsbDeviceVideoVirtualCameraConfig = {
    g_UsbDeviceVideoInterfaceList,
    kUSB_DeviceClassTypeVideo,
    USB_DEVICE_CONFIGURATION_COUNT,
};
uint8_t g_UsbCDCConfiguration[] = {
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
    USB_CDC_UNION_FUNC_DESC,
    USB_CDC_VCOM_CIC_INTERFACE_INDEX, /* The interface number of the Communications or Data Class interface  */
    USB_CDC_VCOM_DIC_INTERFACE_INDEX, /* Interface number of subordinate interface in the Union  */

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
};

uint8_t g_UsbCDC2Configuration[] = {
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
uint8_t g_UsbUVCConfiguration[] = {
    /* Interface Association Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE_ASSOCIATION,      /* size of the IAD */
    USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,        /* INTERFACE ASSOCIATION Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_INDEX, /* Interface number of the first VideoControl
                  interface that is associated with this function. */
    USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT,         /* Number of contiguous VideoStreaming interfaces that are
                  associated with this function.
                  The count includes the first VideoControl interface and all
                  its associated VideoStreaming interfaces.
                  */
    USB_DEVICE_VIDEO_CC_VIDEO,                        /* CC_VIDEO, Video Interface Class code */
    USB_DEVICE_VIDEO_SC_VIDEO_INTERFACE_COLLECTION,   /* SC_VIDEO_INTERFACE_COLLECTION. Video Interface
                  Subclass code */
    USB_DEVICE_VIDEO_PC_PROTOCOL_UNDEFINED,           /* Not used, Must be set to PC_PROTOCOL_UNDEFINED */
    0x06U,                                            /* Index of a string descriptor */

    /* Standard VC Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,                  /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_INTERFACE,                    /* INTERFACE Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_INDEX, /* Index of control interface */
    0x00U,                                            /* Index of the interface setting */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT_COUNT,  /* One endpoint of control pipe */
    USB_DEVICE_VIDEO_CC_VIDEO,                        /* CC_VIDEO */
    USB_DEVICE_VIDEO_SC_VIDEOCONTROL,                 /* SC_VIDEOCONTROL */
    USB_DEVICE_VIDEO_VIRTUAL_CAMERA_PROTOCOL,         /* Protocol */
    0x00U,                                            /* Index of this string descriptor */

    /* Class-Specific VC Interface header Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INTERFACE_HEADER_LENGTH, /* Size of this descriptor, 12+n */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,              /* CS_INTERFACE, descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_HEADER,              /* VC_HEADER, descriptor subtype */
    USB_SHORT_GET_LOW(USB_DEVICE_VIDEO_SPECIFIC_BCD_VERSION), USB_SHORT_GET_HIGH(USB_DEVICE_VIDEO_SPECIFIC_BCD_VERSION),
    /* bcdUVC */
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_VC_INTERFACE_TOTAL_LENGTH),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_VC_INTERFACE_TOTAL_LENGTH),
    /* Total number of bytes returned for the class-specific VideoControl interface descriptor.
       Includes the combined length of this descriptor header and all Unit and Terminal descriptors. */
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_CLOCK_FREQUENCY),
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_CLOCK_FREQUENCY),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_CLOCK_FREQUENCY),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_CLOCK_FREQUENCY),
    /* Use of this field has been deprecated. */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_COUNT, /* The number of VideoStreaming interfaces in the Video
                 Interface Collection to which this VideoControl
                 interface belongs: n */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX, /* Interface number of the first VideoStreaming
                 interface in the Collection */

    /* Input Terminal Descriptor (Camera) */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_LENGTH, /* Size of this descriptor, 15+n */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,            /* CS_INTERFACE, descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_INPUT_TERMINAL,    /* VC_INPUT_TERMINAL, descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_ID,     /* A non-zero constant that uniquely identifies the
                   Terminal within the video function. This value is used
                   in all requests to address this Terminal. */
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_TYPE),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_TYPE),
    /* ITT_CAMERA, Constant that characterizes the type of Terminal */
    0x00U,               /* ID of the Output Terminal to which this Input Terminal is associated */
    0x00U,               /* Index of a string descriptor */
    0x00U, 0x00U,        /* The value of Lmin, unsupported */
    0x00U, 0x00U,        /* The value of Lmax, unsupported */
    0x00U, 0x00U,        /* The value of Locular, unsupported */
    0x03U,               /* Size in bytes of the bmControls field: n */
    0x00U, 0x00U, 0x00U, /* Not control supported */

    /* Output Terminal Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_LENGTH, /* Size of this descriptor, 9+n */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,             /* CS_INTERFACE, descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_OUTPUT_TERMINAL,    /* VC_OUTPUT_TERMINAL, descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_ID,     /* A non-zero constant that uniquely identifies the
                    Terminal within the video function. This value is
                    used in all requests to address this Terminal. */
    USB_SHORT_GET_LOW(USB_DEVICE_VIDEO_TT_STREAMING), USB_SHORT_GET_HIGH(USB_DEVICE_VIDEO_TT_STREAMING),
    /* TT_STREAMING, Constant that characterizes the type of Terminal */
    0x00U, /* ID of the Input Terminal to which this Output Terminal is associated */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_ID, /* ID of the Unit or Terminal to which this Terminal is
                  connected */
    0x00U,                                         /* Index of a string descriptor */

    /* Processing Unit Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_VC_PROCESSING_UNIT_LENGTH, /* Size of this descriptor, 10+n */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,             /* CS_INTERFACE, descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_PROCESSING_UNIT,    /* VC_PROCESSING_UNIT, descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_VC_PROCESSING_UNIT_ID,     /* A non-zero constant that uniquely identifies the
                    Terminal within the video function. This value is
                    used in all requests to address this Terminal. */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_ID,      /* ID of the Unit or Terminal to which this Terminal is
                    connected */
    0x00U, 0x00U, /* This field indicates the maximum digital magnification, multiplied by 100U */
#if defined(USB_DEVICE_VIDEO_CLASS_VERSION_1_5) && (USB_DEVICE_VIDEO_CLASS_VERSION_1_5 > 0U)
    0x03U,               /* Size of the bmControls field, in bytes: n */
    0x00U, 0x00U, 0x00U, /* Not control supported */
#else
    0x02U,        /* Size of the bmControls field, in bytes: n */
    0x00U, 0x00U, /* Not control supported */
#endif     /* USB_DEVICE_VIDEO_CLASS_VERSION_1_5 */
    0x00U, /* Index of a string descriptor */
#if defined(USB_DEVICE_VIDEO_CLASS_VERSION_1_1) && (USB_DEVICE_VIDEO_CLASS_VERSION_1_1 > 0U) || \
    defined(USB_DEVICE_VIDEO_CLASS_VERSION_1_5) && (USB_DEVICE_VIDEO_CLASS_VERSION_1_5 > 0U)
    0x00U, /* A bitmap of all analog video standards supported by the Processing Unit */
#endif     /* USB_DEVICE_VIDEO_CLASS_VERSION_1_1 || USB_DEVICE_VIDEO_CLASS_VERSION_1_5 */
    /* Standard VC Interrupt Endpoint Descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, /* Size of this descriptor, in bytes: 7U */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* ENDPOINT descriptor type */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT |
        (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT), /* Endpoint address */
    USB_ENDPOINT_INTERRUPT,                                          /* Transfer type */
    USB_SHORT_GET_LOW(FS_INTERRUPT_IN_PACKET_SIZE), USB_SHORT_GET_HIGH(FS_INTERRUPT_IN_PACKET_SIZE),
    /* Max Packet Size */
    FS_INTERRUPT_IN_INTERVAL, /* Interval */

    /* Standard VS Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,                 /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_INTERFACE,                   /* INTERFACE Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX, /* Index of stream interface */
    0x00U,                                           /* Index of the interface setting */
    0U,                                              /* No endpoint of stream pipe */
    USB_DEVICE_VIDEO_CC_VIDEO,                       /* CC_VIDEO */
    USB_DEVICE_VIDEO_SC_VIDEOSTREAMING,              /* SC_VIDEOSTREAMING */
    USB_DEVICE_VIDEO_VIRTUAL_CAMERA_PROTOCOL,        /* Protocol */
    0x00U,                                           /* Index of this string descriptor */

    /* VS Specific Input Header Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_VS_INTERFACE_HEADER_LENGTH, /* Size of this descriptor, in bytes: 13+(p*n) */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,              /* CS_INTERFACE descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VS_INPUT_HEADER,        /* VS_INPUT_HEADER descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_FORMAT_COUNT,               /* One format (p = 1U)*/
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_VS_INTERFACE_TOTAL_LENGTH),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_VS_INTERFACE_TOTAL_LENGTH),
    /* Total number of bytes returned for the class-specific VideoStreaming interface descriptors including
       this header descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN | (USB_IN << 0x07U),
    /* The address of the isochronous or bulk endpoint used for video data */
    0x00U,                                                /* Dynamic Format Change unsupported */
    USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_ID,       /* The terminal ID of the Output Terminal to which the
                   video endpoint of this interface is connected */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_STILL_CAPTURE_METHOD, /* Method 2U, still image capture supported */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_STILL_CAPTURE_TRIGGER_SUPPOTED,
    /* Unsupported hardware trigger */
    0x00U, /* Specifies how the host software shall respond to a hardware trigger interrupt event from this
       interface */
    0x01U, /* Size of each bmaControl(x) field */
    0x00U, /* Not used */

    /* Uncompressed Video Format Descriptor */
    USB_VIDEO_UNCOMPRESSED_FORMAT_DESCRIPTOR_LENGTH,     /* Size of this Descriptor, in bytes: 11U */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,              /* CS_INTERFACE descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VS_FORMAT_UNCOMPRESSED, /* VS_FORMAT_UNCOMPRESSED descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX,  /* Index of this Format Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_COUNT,   /* Number of Frame Descriptors following that correspond to
  this format */

    /* MEDIASUBTYPE_UYVY */
    0x55, 0x59, 0x56, 0x59, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71,

    /*MEDIASUBTYPE_YUY2*/
    // 0x59, 0x55, 0x59, 0x32, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71,

    USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DATA_BITS *
        8U, /* Number of bits per pixel used to specify color in the decoded video frame */
    0x01U,  /* Optimum Frame Index */
    0x00U,  /* The X dimension of the picture aspect ratio */
    0x00U,  /* The Y dimension of the picture aspect ratio */
    0x00U,  /* Specifies interlace information */
    0x00U,  /* Specifies whether duplication of the video stream is restricted. */

    /* Uncompressed Video Frame Descriptor */
    USB_VIDEO_UNCOMPRESSED_FRAME_DESCRIPTOR_LENGTH,     /* Size of this Descriptor */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,             /* CS_INTERFACE descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VS_FRAME_UNCOMPRESSED, /* VS_FRAME_UNCOMPRESSED descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX,  /* First frame */
    0x00U,                                              /* D0, Only for still capture method 1U
          D1, Fixed frame-rate */
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH),
    /* Width */
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT),
    /* Height */
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_BIT_RATE),
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_BIT_RATE),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_BIT_RATE),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_BIT_RATE),
    /* Min bit Rate */
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_BIT_RATE),
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_BIT_RATE),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_BIT_RATE),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_BIT_RATE),
    /* Max bit Rate */
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE),
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE),
    /* Max Frame buffer size */
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL),
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL),
    /* Default Frame interval */
    USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_TYPE, /* frame interval type */
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_30FPS),
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_30FPS),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_30FPS),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_30FPS),
    /* 30fps */
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_10FPS),
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_10FPS),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_10FPS),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INTERVAL_10FPS),
    /* 10fps */

    /* Still Image Frame Descriptor */
    USB_VIDEO_UNCOMPRESSED_FRAME_STILL_DESCRIPTOR_LENGTH, /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,               /* CS_INTERFACE descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VS_STILL_IMAGE_FRAME,    /* VS_STILL_IMAGE_FRAME descriptor subtype */
    0x00U,                                                /* If method 3U of still image capture is used,
         this contains the address of the bulk endpoint
         used for still image capture */
    0x01U,                                                /* Number of Image Size patterns of this format: n */
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH),
    /* Width */
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT),
    /* Height */
    0x00U, /* Compression of the still image in pattern */

    /* Standard VS Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,                 /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_INTERFACE,                   /* INTERFACE Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX, /* Index of stream interface */
    0x01U,                                           /* Index of the interface setting */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_COUNT,  /* One endpoint of stream pipe */
    USB_DEVICE_VIDEO_CC_VIDEO,                       /* CC_VIDEO */
    USB_DEVICE_VIDEO_SC_VIDEOSTREAMING,              /* SC_VIDEOSTREAMING */
    USB_DEVICE_VIDEO_VIRTUAL_CAMERA_PROTOCOL,        /* Protocol */
    0x00U,                                           /* Index of this string descriptor */

    /*Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* ENDPOINT Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_ISOCHRONOUS | USB_DESCRIPTOR_ENDPOINT_ATTRIBUTE_SYNC_TYPE_ASYNC,
    USB_SHORT_GET_LOW(FS_STREAM_IN_PACKET_SIZE), USB_SHORT_GET_HIGH(FS_STREAM_IN_PACKET_SIZE),
    /* Max Packet Size */
    FS_STREAM_IN_INTERVAL, /* Interval */
};

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
    USB_SHORT_GET_LOW(USB_DEVICE_VID),
    USB_SHORT_GET_HIGH(USB_DEVICE_VID), /* Vendor ID (assigned by the USB-IF) */
    USB_SHORT_GET_LOW(USB_DEVICE_PID),
    USB_SHORT_GET_HIGH(USB_DEVICE_PID), /* Product ID (assigned by the manufacturer) */
    USB_SHORT_GET_LOW(USB_DEVICE_DEMO_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_DEMO_BCD_VERSION), /* Device release number in binary-coded decimal */
    0x01U,                                           /* Index of string descriptor describing manufacturer */
    0x02U,                                           /* Index of string descriptor describing product */
    0x00U,                                           /* Index of string descriptor describing the
                                                        device's serial number */
    USB_DEVICE_CONFIGURATION_COUNT,                  /* Number of possible configurations */
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceConfigurationDescriptor[1000] = {
    USB_DESCRIPTOR_LENGTH_CONFIGURE, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CONFIGURE,   /* CONFIGURATION Descriptor Type */
    USB_SHORT_GET_LOW(USB_DESCRIPTOR_LENGTH_CONFIGURE),
    USB_SHORT_GET_HIGH(USB_DESCRIPTOR_LENGTH_CONFIGURE), /* Total length of data returned for this configuration. */
    0,                                        /* Number of interfaces supported by this configuration is done dynamic */
    USB_VIDEO_VIRTUAL_CAMERA_CONFIGURE_INDEX, /* Value to use as an argument to the
                                       SetConfiguration() request to select this configuration */
    0x02U,                                    /* Index of string descriptor describing this configuration */
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

};

/* Define string descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString0[] = {
    2U + 2U,
    USB_DESCRIPTOR_TYPE_STRING,
    0x09,
    0x04,
};

const uint8_t g_UsbDeviceString1[] = {
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

const uint8_t g_UsbDeviceString2[] = {
    2U + 2U * 10U, USB_DESCRIPTOR_TYPE_STRING,
    'S',           0,
    'L',           0,
    'N',           0,
    '-',           0,
    'V',           0,
    'I',           0,
    'Z',           0,
    'N',           0,
    '3',           0,
    'D',           0,
};

const uint8_t g_UsbDeviceString3[] = {2U + 2U * 16U, USB_DESCRIPTOR_TYPE_STRING,
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

const uint8_t g_UsbDeviceString4[] = {2U + 2U * 17U, USB_DESCRIPTOR_TYPE_STRING,
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

const uint8_t g_UsbDeviceString5[] = {
    2U + 2U * 10U, USB_DESCRIPTOR_TYPE_STRING,
    'V',           0x00U,
    'I',           0x00U,
    'D',           0x00U,
    'E',           0x00U,
    'O',           0x00U,
    ' ',           0x00U,
    'D',           0x00U,
    'E',           0x00U,
    'M',           0x00U,
    'O',           0x00U,
};

const uint8_t g_UsbDeviceString6[] = {
    2U + 2U * 11U, USB_DESCRIPTOR_TYPE_STRING,
    'U',           0x00U,
    'V',           0x00U,
    'C',           0x00U,
    ' ',           0x00U,
    'D',           0x00U,
    'I',           0x00U,
    'S',           0x00U,
    'P',           0x00U,
    'L',           0x00U,
    'A',           0x00U,
    'Y',           0x00U,
};

/* Define string descriptor size */
uint32_t g_UsbDeviceStringDescriptorLength[USB_DEVICE_STRING_COUNT] = {
    sizeof(g_UsbDeviceString0), sizeof(g_UsbDeviceString1), sizeof(g_UsbDeviceString2), sizeof(g_UsbDeviceString3),
    sizeof(g_UsbDeviceString4), sizeof(g_UsbDeviceString5), sizeof(g_UsbDeviceString6)};

uint8_t const *g_UsbDeviceStringDescriptorArray[USB_DEVICE_STRING_COUNT] = {
    g_UsbDeviceString0, g_UsbDeviceString1, g_UsbDeviceString2, g_UsbDeviceString3,
    g_UsbDeviceString4, g_UsbDeviceString5, g_UsbDeviceString6};

usb_language_t g_UsbDeviceLanguage[USB_DEVICE_LANGUAGE_COUNT] = {{
    g_UsbDeviceStringDescriptorArray,
    g_UsbDeviceStringDescriptorLength,
    (uint16_t)0x0409,
}};

usb_language_list_t g_UsbDeviceLanguageList = {
    g_UsbDeviceString0,
    sizeof(g_UsbDeviceString0),
    g_UsbDeviceLanguage,
    USB_DEVICE_LANGUAGE_COUNT,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get device descriptor request */
usb_status_t USB_DeviceGetDeviceDescriptor(usb_device_handle handle,
                                           usb_device_get_device_descriptor_struct_t *deviceDescriptor)
{
    deviceDescriptor->buffer = g_UsbDeviceDescriptor;
    deviceDescriptor->length = USB_DESCRIPTOR_LENGTH_DEVICE;
    return kStatus_USB_Success;
}

/* Get device configuration descriptor request */
usb_status_t USB_DeviceGetConfigurationDescriptor(
    usb_device_handle handle, usb_device_get_configuration_descriptor_struct_t *configurationDescriptor)
{
    if (USB_VIDEO_VIRTUAL_CAMERA_CONFIGURE_INDEX > configurationDescriptor->configuration)
    {
        configurationDescriptor->buffer = g_UsbDeviceConfigurationDescriptor;
        configurationDescriptor->length = s_total_size;
        return kStatus_USB_Success;
    }
    return kStatus_USB_InvalidRequest;
}

/* Get device string descriptor request */
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
        uint8_t languageId    = 0U;
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

usb_status_t USB_DeviceSetSpeed(usb_device_handle handle, uint8_t speed)
{
    usb_descriptor_union_t *descriptorHead;
    usb_descriptor_union_t *descriptorTail;

    descriptorHead = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[0]);
    descriptorTail = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[s_total_size - 1]);

    while (descriptorHead < descriptorTail)
    {
        if (descriptorHead->common.bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT)
        {
            if (USB_SPEED_HIGH == speed)
            {
                if ((USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT ==
                     (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                    ((descriptorHead->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    descriptorHead->endpoint.bInterval = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_OUT_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2 ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    descriptorHead->endpoint.bInterval = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL_2;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_IN_PACKET_SIZE_2,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    descriptorHead->endpoint.bInterval = HS_STREAM_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_STREAM_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else
                {
                    descriptorHead->endpoint.bInterval = HS_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_INTERRUPT_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
            }
            else
            {
                if ((USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT ==
                     (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                    ((descriptorHead->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    descriptorHead->endpoint.bInterval = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2 ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    descriptorHead->endpoint.bInterval = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL_2;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_IN_PACKET_SIZE_2,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 ==
                          (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                         ((descriptorHead->endpoint.bEndpointAddress &
                           USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else if (USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN ==
                         (descriptorHead->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK))
                {
                    descriptorHead->endpoint.bInterval = FS_STREAM_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_STREAM_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
                else
                {
                    descriptorHead->endpoint.bInterval = FS_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_INTERRUPT_IN_PACKET_SIZE,
                                                       descriptorHead->endpoint.wMaxPacketSize);
                }
            }
        }
        descriptorHead = (usb_descriptor_union_t *)((uint8_t *)descriptorHead + descriptorHead->common.bLength);
    }

    for (int i = 0; i < USB_CDC_VCOM_CIC_ENDPOINT_COUNT; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            g_cdcVcomCicEndpoints[i].maxPacketSize = HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
        }
        else
        {
            g_cdcVcomCicEndpoints[i].maxPacketSize = FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
        }
    }

    for (int i = 0; i < USB_CDC_VCOM_DIC_ENDPOINT_COUNT; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            if (g_cdcVcomDicEndpoints[i].endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)
            {
                g_cdcVcomDicEndpoints[i].maxPacketSize = HS_CDC_VCOM_BULK_IN_PACKET_SIZE;
            }
            else
            {
                g_cdcVcomDicEndpoints[i].maxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
            }
        }
        else
        {
            if (g_cdcVcomDicEndpoints[i].endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)
            {
                g_cdcVcomDicEndpoints[i].maxPacketSize = FS_CDC_VCOM_BULK_IN_PACKET_SIZE;
            }
            else
            {
                g_cdcVcomDicEndpoints[i].maxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
            }
        }
    }

    for (int i = 0; i < USB_CDC_VCOM_CIC_ENDPOINT_COUNT_2; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            g_cdcVcomCicEndpoints_2[i].maxPacketSize = HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2;
        }
        else
        {
            g_cdcVcomCicEndpoints_2[i].maxPacketSize = FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2;
        }
    }

    for (int i = 0; i < USB_CDC_VCOM_DIC_ENDPOINT_COUNT_2; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            if (g_cdcVcomDicEndpoints_2[i].endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)
            {
                g_cdcVcomDicEndpoints_2[i].maxPacketSize = HS_CDC_VCOM_BULK_IN_PACKET_SIZE_2;
            }
            else
            {
                g_cdcVcomDicEndpoints_2[i].maxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2;
            }
        }
        else
        {
            if (g_cdcVcomDicEndpoints_2[i].endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)
            {
                g_cdcVcomDicEndpoints_2[i].maxPacketSize = FS_CDC_VCOM_BULK_IN_PACKET_SIZE_2;
            }
            else
            {
                g_cdcVcomDicEndpoints_2[i].maxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2;
            }
        }
    }

    if (USB_SPEED_HIGH == speed)
    {
        g_UsbDeviceVideoControlEndpoints[0].maxPacketSize = HS_INTERRUPT_IN_PACKET_SIZE;
        g_UsbDeviceVideoStreamEndpoints[0].maxPacketSize  = HS_STREAM_IN_PACKET_SIZE;
    }
    else
    {
        g_UsbDeviceVideoControlEndpoints[0].maxPacketSize = FS_INTERRUPT_IN_PACKET_SIZE;
        g_UsbDeviceVideoStreamEndpoints[0].maxPacketSize  = FS_STREAM_IN_PACKET_SIZE;
    }

    return kStatus_USB_Success;
}

usb_status_t USB_RegisterCDC()
{
    usb_descriptor_union_t *descriptorHead;
    usb_descriptor_configuration_t *ptr2;
    static uint8_t is_registered = 0;
    if (is_registered == 1)
    {
        return kStatus_USB_Error;
    }
    is_registered  = 1;
    descriptorHead = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[0]);

    if (descriptorHead->common.bDescriptorType == USB_DESCRIPTOR_TYPE_CONFIGURE)
    {
        ptr2 = (usb_descriptor_configuration_t *)descriptorHead;
        memcpy(g_UsbDeviceConfigurationDescriptor + s_total_size, g_UsbCDCConfiguration, sizeof(g_UsbCDCConfiguration));
        s_total_size += sizeof(g_UsbCDCConfiguration);
        ptr2->wTotalLength[0] = USB_SHORT_GET_LOW(s_total_size);
        ptr2->wTotalLength[1] = USB_SHORT_GET_HIGH(s_total_size);
        ptr2->bNumInterfaces += USB_CDC_VCOM_INTERFACE_COUNT;
    }
    return kStatus_USB_Success;
}

usb_status_t USB_RegisterCDC2()
{
    usb_descriptor_union_t *descriptorHead;
    usb_descriptor_configuration_t *ptr2;
    static uint8_t is_registered = 0;
    if (is_registered == 1)
    {
        return kStatus_USB_Error;
    }
    is_registered  = 1;
    descriptorHead = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[0]);

    if (descriptorHead->common.bDescriptorType == USB_DESCRIPTOR_TYPE_CONFIGURE)
    {
        ptr2 = (usb_descriptor_configuration_t *)descriptorHead;
        memcpy(g_UsbDeviceConfigurationDescriptor + s_total_size, g_UsbCDC2Configuration,
               sizeof(g_UsbCDC2Configuration));
        s_total_size += sizeof(g_UsbCDC2Configuration);
        ptr2->wTotalLength[0] = USB_SHORT_GET_LOW(s_total_size);
        ptr2->wTotalLength[1] = USB_SHORT_GET_HIGH(s_total_size);
        ptr2->bNumInterfaces += USB_CDC_VCOM_INTERFACE_COUNT_2;
    }
    return kStatus_USB_Success;
}

usb_status_t USB_RegisterUVC()
{
    usb_descriptor_union_t *descriptorHead;
    usb_descriptor_configuration_t *ptr2;
    static uint8_t is_registered = 0;
    if (is_registered == 1)
    {
        return kStatus_USB_Error;
    }
    is_registered  = 1;
    descriptorHead = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[0]);

    if (descriptorHead->common.bDescriptorType == USB_DESCRIPTOR_TYPE_CONFIGURE)
    {
        ptr2 = (usb_descriptor_configuration_t *)descriptorHead;
        memcpy(g_UsbDeviceConfigurationDescriptor + s_total_size, g_UsbUVCConfiguration, sizeof(g_UsbUVCConfiguration));
        s_total_size += sizeof(g_UsbUVCConfiguration);
        ptr2->wTotalLength[0] = USB_SHORT_GET_LOW(s_total_size);
        ptr2->wTotalLength[1] = USB_SHORT_GET_HIGH(s_total_size);
        ptr2->bNumInterfaces += USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT;
    }
    return kStatus_USB_Success;
}
