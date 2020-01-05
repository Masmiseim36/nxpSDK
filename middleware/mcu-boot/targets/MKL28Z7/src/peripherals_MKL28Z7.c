/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader/bl_peripheral_interface.h"
#include "packet/serial_packet.h"

extern void uart_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void i2c_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void spi_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Peripheral array for KL43Z4.
const peripheral_descriptor_t g_peripherals[] = {
#if BL_CONFIG_LPUART_0
    // LPUART0
    {.typeMask = kPeripheralType_UART,
     .instance = 0,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_lpuartControlInterface,
     .byteInterface = &g_lpuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPUART_0
#if BL_CONFIG_LPUART_1
    // LPUART1
    {.typeMask = kPeripheralType_UART,
     .instance = 1,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_lpuartControlInterface,
     .byteInterface = &g_lpuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPUART_1
#if BL_CONFIG_LPUART_2
    // LPUART2
    {.typeMask = kPeripheralType_UART,
     .instance = 2,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_lpuartControlInterface,
     .byteInterface = &g_lpuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPUART_1
#if BL_CONFIG_LPI2C_0
    // LPI2C0
    {.typeMask = kPeripheralType_I2CSlave,
     .instance = 0,
     .pinmuxConfig = i2c_pinmux_config,
     .controlInterface = &g_lpi2cControlInterface,
     .byteInterface = &g_lpi2cByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPI2C_0
#if BL_CONFIG_LPI2C_1
    // LPI2C1
    {.typeMask = kPeripheralType_I2CSlave,
     .instance = 1,
     .pinmuxConfig = i2c_pinmux_config,
     .controlInterface = &g_lpi2cControlInterface,
     .byteInterface = &g_lpi2cByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPI2C_1
#if BL_CONFIG_LPI2C_2
    // LPI2C2
    {.typeMask = kPeripheralType_I2CSlave,
     .instance = 2,
     .pinmuxConfig = i2c_pinmux_config,
     .controlInterface = &g_lpi2cControlInterface,
     .byteInterface = &g_lpi2cByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPI2C_2
#if BL_CONFIG_LPSPI_0
    // LPSPI0
    {.typeMask = kPeripheralType_SPISlave,
     .instance = 0,
     .pinmuxConfig = spi_pinmux_config,
     .controlInterface = &g_lpspiControlInterface,
     .byteInterface = &g_lpspiByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPSPI_0
#if BL_CONFIG_LPSPI_1
    // LPSPI1
    {.typeMask = kPeripheralType_SPISlave,
     .instance = 1,
     .pinmuxConfig = spi_pinmux_config,
     .controlInterface = &g_lpspiControlInterface,
     .byteInterface = &g_lpspiByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPSPI_1
#if BL_CONFIG_LPSPI_2
    // LPSPI2
    {.typeMask = kPeripheralType_SPISlave,
     .instance = 2,
     .pinmuxConfig = spi_pinmux_config,
     .controlInterface = &g_lpspiControlInterface,
     .byteInterface = &g_lpspiByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPSPI_2
#if (BL_CONFIG_USB_HID || BL_CONFIG_USB_MSC)
    // usbhid
    {.typeMask = kPeripheralType_USB_HID,
     .instance = 0,
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
