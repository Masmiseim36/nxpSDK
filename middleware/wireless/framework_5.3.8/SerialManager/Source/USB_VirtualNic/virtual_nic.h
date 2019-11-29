/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _USB_CDC_VNIC_H_
#define _USB_CDC_VNIC_H_ 1

/*******************************************************************************
* Definitions
******************************************************************************/
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

/* Currently configured line coding */
#define LINE_CODING_SIZE (0x07)
#define LINE_CODING_DTERATE (115200)
#define LINE_CODING_CHARFORMAT (0x00)
#define LINE_CODING_PARITYTYPE (0x00)
#define LINE_CODING_DATABITS (0x08)

/* Communications feature */
#define COMM_FEATURE_DATA_SIZE (0x02)
#define STATUS_ABSTRACT_STATE (0x0000)
#define COUNTRY_SETTING (0x0000)

/* Notification of serial state */
#define NOTIF_PACKET_SIZE (0x08)
#define UART_BITMAP_SIZE (0x02)
#define NOTIF_REQUEST_TYPE (0xA1)

#define ENET_FRAME_MAX_FRAMELEN 1518U     /*!< Maximum Ethernet frame size. */
#define RNDIS_ETHER_HDR_SIZE            (14)
#define RNDIS_USB_OVERHEAD_SIZE         (44)
#define RNDIS_DATA_OFFSET               (36)
#define MAC_ADDR_SIZE                   (6)


/* Define the types for application */
typedef struct _usb_cdc_vnic
{
    usb_device_handle deviceHandle;
    class_handle_t cdcAcmHandle;
    usb_device_cdc_rndis_struct_t *rndisHandle;
    uint8_t speed;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_CDC_VNIC_INTERFACE_COUNT];
} usb_cdc_vnic_t;

/* Define packet message format  */
typedef struct _rndis_packet_msg_format
{
    /*Specifies the Remote NDIS message type.
      This is set to RNDIS_PACKET_MSG = 0x1.*/
    uint32_t messageType;
    /*Message length in bytes, including appended packet data, out-of-band
      data, per-packet-info data, and both internal and external padding*/
    uint32_t messageLen;
    /*Specifies the offset in bytes from the start of the DataOffset field of
      this message to the start of the data. This is an integer multiple of 4*/
    uint32_t dataOffset;
    /*Specifies the number of bytes in the data content of this message.*/
    uint32_t dataLen;
    /*Specifies the offset in bytes of the first out of band data record from
      the start of the DataOffset field of this message.  Set to 0 if there is
      no out-of-band data. Otherwise this is an integer multiple of 4*/
    uint32_t oobDataOffset;
    /*Specifies in bytes the total length of the out of band data.*/
    uint32_t oobDataLen;
    /*Specifies the number of out of band records in this message*/
    uint32_t numOobDataElems;
    /*Specifies in bytes the offset from the beginning of the DataOffset field
      in the RNDIS_PACKET_MSG data message to the start of the first per
      packet info data record.  Set to 0 if there is no per-packet data.
      Otherwise this is an integer multiple of 4*/
    uint32_t perPktInfoOffset;
    /*Specifies in bytes the total length of the per packet information
      contained in this message*/
    uint32_t perPktInfoLen;
    /*Reserved for connection-oriented devices.  Set to 0.*/
    uint32_t vcHandle;
    /*Reserved. Set to 0.*/
    uint32_t reserved;
} rndis_packet_msg_format_t;
#endif /* _USB_CDC_VNIC_H_ */
