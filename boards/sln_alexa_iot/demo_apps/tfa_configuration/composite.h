/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_DEVICE_COMPOSITE_H__
#define __USB_DEVICE_COMPOSITE_H__ 1

#include "audio_unified.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

typedef struct _usb_device_composite_struct
{
    usb_device_handle deviceHandle;
    usb_audio_composite_struct_t audioUnified;
    uint8_t speed;
    uint8_t attach;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_DEVICE_INTERFACE_COUNT];
} usb_device_composite_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Audio class specific callback function.
 *
 * This function handles the Audio class specific requests which recipient is interface.
 *
 * @param handle          The Audio class handle.
 * @param setup           The setup buffer address of the request.
 * @param length          The active data length.
 * @param buffer          The active data buffer need to be sent or received.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioUnifiedClassRequestIntfRecipient(usb_device_handle handle,
                                                                    usb_setup_struct_t *setup,
                                                                    uint32_t *length,
                                                                    uint8_t **buffer);
/*!
 * @brief Audio class specific callback function.
 *
 * This function handles the Audio class specific requests which recipient is endpoint.
 *
 * @param handle          The Audio class handle.
 * @param setup           The setup buffer address of the request.
 * @param length          The active data length.
 * @param buffer          The active data buffer need to be sent or received.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioUnifiedClassRequestEndpRecipient(usb_device_handle handle,
                                                                    usb_setup_struct_t *setup,
                                                                    uint32_t *length,
                                                                    uint8_t **buffer);
/*!
 * @brief USB configure endpoint function.
 *
 * This function configure endpoint status.
 *
 * @param handle The USB device handle.
 * @param ep Endpoint address.
 * @param status A flag to indicate whether to stall the endpoint. 1: stall, 0: unstall.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioUnifiedConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status);
/*!
 * @brief Audio device set configuration function.
 *
 * This function sets configuration for Audio class.
 *
 * @param handle The Audio class handle.
 * @param configure The Audio class configure index.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioUnifiedSetConfigure(usb_device_handle handle, uint8_t configure);
/*!
 * @brief Audio device set interface function.
 *
 * This function sets configuration for Audio class.
 *
 * @param handle The Audio class handle.
 * @param interface The Audio class interface index.
 * @param alternateSetting The Audio class interface alternate setting.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioUnifiedSetInterface(usb_device_handle handle,
                                                       uint8_t interface,
                                                       uint8_t alternateSetting);
/*!
 * @brief Audio device initialization function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param deviceComposite The pointer to the composite device structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioUnifiedInit(usb_device_composite_struct_t *deviceComposite);

/*!
 * @brief USB configure endpoint function.
 *
 * This function configure endpoint status.
 *
 * @param handle The USB device handle.
 * @param ep Endpoint address.
 * @param status A flag to indicate whether to stall the endpoint. 1: stall, 0: unstall.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceHidConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status);

#endif /* _USB_DESCRIPTOR_H_ */
