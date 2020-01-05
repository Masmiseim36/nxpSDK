/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader/bl_peripheral_interface.h"
#include "packet/serial_packet.h"

extern void uart_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void i2c_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void spi_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void can_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Peripheral array for KE18F15.
const peripheral_descriptor_t g_peripherals[] = {
#if BL_CONFIG_LPUART_0
    // UART0
    {.typeMask = kPeripheralType_UART,
     .instance = 0,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_lpuartControlInterface,
     .byteInterface = &g_lpuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPUART_0

#if BL_CONFIG_LPI2C
    // I2C0
    {.typeMask = kPeripheralType_I2CSlave,
     .instance = 0,
     .pinmuxConfig = i2c_pinmux_config,
     .controlInterface = &g_lpi2cControlInterface,
     .byteInterface = &g_lpi2cByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPI2C

#if BL_CONFIG_LPSPI
    // SPI0
    {.typeMask = kPeripheralType_SPISlave,
     .instance = 0,
     .pinmuxConfig = spi_pinmux_config,
     .controlInterface = &g_lpspiControlInterface,
     .byteInterface = &g_lpspiByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPSPI
#if BL_CONFIG_CAN
    // CAN0
    {.typeMask = kPeripheralType_CAN,
     .instance = 0,
     .pinmuxConfig = can_pinmux_config,
     .controlInterface = &g_mscanControlInterface,
     .byteInterface = &g_mscanByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif    // BL_CONFIG_CAN
    { 0 } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
