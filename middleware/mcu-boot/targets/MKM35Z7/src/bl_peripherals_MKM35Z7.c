/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_context.h"
#include "bl_peripheral_interface.h"
#include "serial_packet.h"

extern void uart_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void i2c_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void spi_pinmux_config(uint32_t instance, pinmux_type_t pinmux);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#if !BL_CONFIG_UART && !BL_CONFIG_I2C && !BL_CONFIG_DSPI && !BL_CONFIG_CAN
#error At least one peripheral must be enabled!
#endif

//! @brief Peripheral array for K22F5256.
const peripheral_descriptor_t g_peripherals[] = {
#if BL_CONFIG_UART
    // UART2
    {.typeMask = kPeripheralType_UART,
     .instance = 2,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_uartControlInterface,
     .byteInterface = &g_uartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_UART
      
#if BL_CONFIG_I2C
    // I2C0
    {.typeMask = kPeripheralType_I2CSlave,
     .instance = 0,
     .pinmuxConfig = i2c_pinmux_config,
     .controlInterface = &g_i2cControlInterface,
     .byteInterface = &g_i2cByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_I2C
#if BL_CONFIG_SPI
    // SPI1
    {.typeMask = kPeripheralType_SPISlave,
     .instance = 1,
     .pinmuxConfig = spi_pinmux_config,
     .controlInterface = &g_spiControlInterface,
     .byteInterface = &g_spiByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_DSPI
    { 0 } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
