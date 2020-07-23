/*
 * Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader/bl_peripheral_interface.h"
#include "packet/serial_packet.h"
#include "peripherals_pinmux.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#if !BL_CONFIG_LPUART && !BL_CONFIG_I2C && !BL_CONFIG_DSPI  && !BL_CONFIG_CAN
#error At least one peripheral must be enabled!
#endif

//! @brief Peripheral array for K22F5256.
const peripheral_descriptor_t g_peripherals[] = {
#if defined(BL_CONFIG_LPUART) && BL_CONFIG_LPUART
    // LPUART0
    {.typeMask = (uint32_t)kPeripheralType_UART,
     .instance = 0u,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_lpuartControlInterface,
     .byteInterface = &g_lpuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_SCUART
#if defined(BL_CONFIG_I2C) && BL_CONFIG_I2C
    // I2C0
    {.typeMask = (uint32_t)kPeripheralType_I2CSlave,
     .instance = 0u,
     .pinmuxConfig = i2c_pinmux_config,
     .controlInterface = &g_i2cControlInterface,
     .byteInterface = &g_i2cByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_I2C
#if defined(BL_CONFIG_DSPI) && BL_CONFIG_DSPI
    // SPI0
    {.typeMask = (uint32_t)kPeripheralType_SPISlave,
     .instance = 0u,
     .pinmuxConfig = spi_pinmux_config,
     .controlInterface = &g_dspiControlInterface,
     .byteInterface = &g_dspiByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_DSPI
#if defined(BL_CONFIG_CAN) && BL_CONFIG_CAN
    // CAN0
    {.typeMask = (uint32_t)kPeripheralType_CAN,
     .instance = 0u,
     .pinmuxConfig = can_pinmux_config,
     .controlInterface = &g_flexcanControlInterface,
     .byteInterface = &g_flexcanByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_CAN
#if defined(BL_CONFIG_LIN) && BL_CONFIG_LIN 
    // LPUART1
    {.typeMask = (uint32_t)kPeripheralType_CAN,
     .instance = 1u,
     .pinmuxConfig = linlpuart_pinmux_config,
     .controlInterface = &g_linlpuartControlInterface,
     .byteInterface = &g_linlpuartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_SCUART
    { 0u } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
