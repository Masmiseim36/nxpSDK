/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader/bl_peripheral_interface.h"
#include "packet/serial_packet.h"

extern void uart_pinmux_config(uint32_t instance, pinmux_type_t pinmux);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#if !BL_CONFIG_LPUART && !BL_CONFIG_HS_USB_HID
#error At least one peripheral must be enabled!
#endif

//! @brief Peripheral array for MIMXRT1051.
const peripheral_descriptor_t g_peripherals[] = {
#if BL_CONFIG_LPUART_1
    // LPUART1
    {.typeMask = kPeripheralType_UART,
     .instance = 1,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_lpuartControlInterface,
     .byteInterface = &g_lpuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPUART_1

#if (BL_CONFIG_HS_USB_HID)
    // USB HID
    {.typeMask = kPeripheralType_USB_HID,
     .instance = 1,
     .pinmuxConfig = NULL,
     .controlInterface = &g_usbHidControlInterface,
     .byteInterface = NULL,
     .packetInterface = &g_usbHidPacketInterface },
#endif    // BL_CONFIG_USB_HID
    { 0 } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
