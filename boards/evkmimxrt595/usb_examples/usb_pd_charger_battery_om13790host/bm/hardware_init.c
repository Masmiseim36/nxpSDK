/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${standard_header:start}*/
#include <stdbool.h>
/*${standard_header:end}*/
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_adapter_gpio.h"
#include "fsl_inputmux.h"
#include "usb_pd_config.h"
#include "usb_pd.h"
#include "pd_board_config.h"
/*${header:end}*/

/*${function:start}*/
#define I2C_RELEASE_BUS_COUNT 100U
#define PD_I2C_SCL            (21U)
#define PD_I2C_SDA            (22U)
#define PD_I2C_PORT           (4U)

void BOARD_InitHardware(void)
{
    /* Use 48 MHz clock for the FLEXCOMM11 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM11);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    NVIC_SetPriority(BOARD_UART_IRQ, 5);
    I2C11_InitPins();
}

uint32_t HW_TimerGetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

uint32_t HW_I2CGetFreq(uint8_t instance)
{
    return CLOCK_GetFreq(kCLOCK_Flexcomm11Clk);
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void HW_I2CReleaseBus(void)
{
    hal_gpio_pin_config_t config;
    GPIO_HANDLE_DEFINE(pdI2cSclGpio);
    GPIO_HANDLE_DEFINE(pdI2cSdaGpio);
    uint8_t i = 0;

    /* Config pin mux as gpio */
    I2C11_DeinitPins();

    /* Init I2C GPIO */
    config.direction = kHAL_GpioDirectionOut;
    config.port      = PD_I2C_PORT;
    config.pin       = PD_I2C_SCL;
    config.level     = 1;
    HAL_GpioInit((hal_gpio_handle_t)pdI2cSclGpio, &config);

    config.pin = PD_I2C_SDA;
    HAL_GpioInit((hal_gpio_handle_t)pdI2cSdaGpio, &config);

    i2c_release_bus_delay();

    /* Drive SDA low first to simulate a start */
    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 0U);
        i2c_release_bus_delay();

        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 1U);
        i2c_release_bus_delay();

        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 0U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 0U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 1U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 1U);
    i2c_release_bus_delay();
    /* De-init I2C GPIO */
    HAL_GpioDeinit((hal_gpio_handle_t)pdI2cSclGpio);
    HAL_GpioDeinit((hal_gpio_handle_t)pdI2cSdaGpio);

    /* re-configure pin mux as i2c */
    I2C11_InitPins();
}

/*${function:end}*/

/*!
** @}
*/
