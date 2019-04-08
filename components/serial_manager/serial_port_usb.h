/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SERIAL_PORT_USB_H__
#define __SERIAL_PORT_USB_H__

/*******************************************************************************
* Definitions
******************************************************************************/

#define SERIAL_PORT_USB_CDC_HANDLE_SIZE          (72)

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

/*! @brief USB controller ID */
typedef enum _serial_port_usb_cdc_controller_index
{
    kSerialManager_UsbControllerKhci0 = 0U, /*!< KHCI 0U */
    kSerialManager_UsbControllerKhci1 = 1U, /*!< KHCI 1U, Currently, there are no platforms which have two KHCI IPs, this is reserved
                                  to be used in the future. */
    kSerialManager_UsbControllerEhci0 = 2U, /*!< EHCI 0U */
    kSerialManager_UsbControllerEhci1 = 3U, /*!< EHCI 1U, Currently, there are no platforms which have two EHCI IPs, this is reserved
                                  to be used in the future. */

    kSerialManager_UsbControllerLpcIp3511Fs0 = 4U, /*!< LPC USB IP3511 FS controller 0 */
    kSerialManager_UsbControllerLpcIp3511Fs1 =
        5U, /*!< LPC USB IP3511 FS controller 1, there are no platforms which have two IP3511 IPs, this is reserved
              to be used in the future. */

    kSerialManager_UsbControllerLpcIp3511Hs0 = 6U, /*!< LPC USB IP3511 HS controller 0 */
    kSerialManager_UsbControllerLpcIp3511Hs1 =
        7U, /*!< LPC USB IP3511 HS controller 1, there are no platforms which have two IP3511 IPs, this is reserved
              to be used in the future. */

    kSerialManager_UsbControllerOhci0 = 8U, /*!< OHCI 0U */
    kSerialManager_UsbControllerOhci1 = 9U, /*!< OHCI 1U, Currently, there are no platforms which have two OHCI IPs, this is reserved
                                  to be used in the future. */

    kSerialManager_UsbControllerIp3516Hs0 = 10U, /*!< IP3516HS 0U */
    kSerialManager_UsbControllerIp3516Hs1 =
        11U, /*!< IP3516HS 1U, Currently, there are no platforms which have two IP3516HS IPs, this is reserved
           to be used in the future. */
} serial_port_usb_cdc_controller_index_t;

typedef struct _serial_port_usb_cdc_config
{
    serial_port_usb_cdc_controller_index_t controllerIndex;     /*!< controller index */
} serial_port_usb_cdc_config_t;

#endif /* __SERIAL_PORT_USB_H__ */
