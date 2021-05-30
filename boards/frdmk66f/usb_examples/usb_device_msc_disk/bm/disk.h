/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017, 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_DISK_H__
#define __USB_DISK_H__ 1

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif
#define USB_DEVICE_INTERRUPT_PRIORITY                                                                    \
    (3U) /*! @brief enable the write task. 1U supported, 0U not supported . if this macro is enable ,the \
USB_DEVICE_CONFIG_USE_TASK macro should also be enable.*/
#define USB_DEVICE_MSC_USE_WRITE_TASK (0U)
#define USB_DEVICE_MSC_BUFFER_NUMBER  (3U)
/* application define logical unit number, if LOGICAL_UNIT_SUPPORTED > USB_DEVICE_MSC_MAX_LUN, update
 * USB_DEVICE_MSC_MAX_LUN in class driver usb_device_msc.h*/
#define LOGICAL_UNIT_SUPPORTED (1U)

/* USB MSC config*/
/*buffer size for sd card example. the larger the buffer size ,the faster the data transfer speed is ,*/
/*the block size should be multiple of 512, the least value is 512*/
#if ((defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)))

#define USB_DEVICE_MSC_WRITE_BUFF_SIZE (512 * 64U)
#define USB_DEVICE_MSC_READ_BUFF_SIZE  (512 * 64U)

#else

#define USB_DEVICE_MSC_WRITE_BUFF_SIZE (512 * 32U)
#define USB_DEVICE_MSC_READ_BUFF_SIZE  (512 * 8U)

#endif

#define USB_DEVICE_MSC_ADMA_TABLE_WORDS (8U)
typedef struct _usb_msc_buffer_struct
{
    uint32_t offset; /*!< Offset of the block need to access*/
    uint32_t size;   /*!< Size of the transfered data*/
    struct _usb_msc_buffer_struct *next;
    uint8_t *buffer; /*!< Buffer address of the transferred data*/
} usb_msc_buffer_struct_t;

typedef struct _usb_msc_struct
{
    usb_device_handle deviceHandle;
    class_handle_t mscHandle;
    uint8_t diskLock;
    uint8_t read_write_error;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_MSC_INTERFACE_COUNT];
    uint8_t speed;
    uint8_t attach;
    usb_msc_buffer_struct_t *headlist;
    usb_msc_buffer_struct_t *taillist;
    usb_msc_buffer_struct_t *transferlist;
} usb_msc_struct_t;

#endif
