/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
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

#if !BL_CONFIG_SCUART && !BL_CONFIG_I2C && !BL_CONFIG_DSPI && !BL_CONFIG_USB_HID
#error At least one peripheral must be enabled!
#endif

//! @brief Peripheral array for K64F12.
const peripheral_descriptor_t g_peripherals[] = {
#if BL_CONFIG_SCUART
#if !defined(TOWER)
    // UART0
    {.typeMask = kPeripheralType_UART,
     .instance = 0,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_scuartControlInterface,
     .byteInterface = &g_scuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#else
    // UART1
    {.typeMask = kPeripheralType_UART,
     .instance = 1,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_scuartControlInterface,
     .byteInterface = &g_scuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif //#if !defined(TOWER)
#endif // BL_CONFIG_SCUART
#if BL_CONFIG_I2C
    // I2C1
    {.typeMask = kPeripheralType_I2CSlave,
     .instance = 1,
     .pinmuxConfig = i2c_pinmux_config,
     .controlInterface = &g_i2cControlInterface,
     .byteInterface = &g_i2cByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_I2C
#if BL_CONFIG_DSPI
    // SPI0
    {.typeMask = kPeripheralType_SPISlave,
     .instance = 0,
     .pinmuxConfig = spi_pinmux_config,
     .controlInterface = &g_dspiControlInterface,
     .byteInterface = &g_dspiByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_DSPI
#if (BL_CONFIG_USB_HID || BL_CONFIG_USB_MSC)
    // USB HID
    {.typeMask = kPeripheralType_USB_HID,
     .instance = 0,
     .pinmuxConfig = NULL,
     .controlInterface = &g_usbHidControlInterface,
     .byteInterface = NULL,
     .packetInterface = &g_usbHidPacketInterface },
#endif // BL_CONFIG_USB_HID

    { 0 } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
