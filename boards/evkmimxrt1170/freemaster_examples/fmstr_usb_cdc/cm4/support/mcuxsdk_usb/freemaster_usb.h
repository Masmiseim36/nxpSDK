/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - USB Example Implementation
 */

#ifndef __FREEMASTER_EXAMPLE_USB_H
#define __FREEMASTER_EXAMPLE_USB_H

#ifndef __FREEMASTER_H
#error Please include the freemaster.h master header file before the freemaster_usb.h
#endif

#ifndef _USB_DEVICE_CONFIG_H_
#error Please include the usb_device_config.h master header file before the freemaster_usb.h
#endif

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#define USB_INTERRUPT_HANDLER USB_DeviceEhciIsrFunction
#define USB_DATA_BUFF_SIZE    HS_CDC_VCOM_BULK_OUT_PACKET_SIZE

#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
#define USB_INTERRUPT_HANDLER USB_DeviceKhciIsrFunction
#define USB_DATA_BUFF_SIZE    FS_CDC_VCOM_BULK_OUT_PACKET_SIZE

#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define USB_INTERRUPT_HANDLER USB_DeviceLpcIp3511IsrFunction
#define USB_DATA_BUFF_SIZE    FS_CDC_VCOM_BULK_OUT_PACKET_SIZE

#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define USB_INTERRUPT_HANDLER USB_DeviceLpcIp3511IsrFunction
#define USB_DATA_BUFF_SIZE    HS_CDC_VCOM_BULK_OUT_PACKET_SIZE
#endif

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

/******************************************************************************
 * Functions definitions
 ******************************************************************************/
FMSTR_BOOL FMSTR_ExampleUsbInit(void);

#endif /* __FREEMASTER_EXAMPLE_USB_H */
