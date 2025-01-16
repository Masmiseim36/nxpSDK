/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_adapter_gpio.h"

#include "usb_pd_config.h"
#include "usb_pd.h"
#include "pd_board_config.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

void HW_I2CReleaseBus(void);

/* ${function:start} */
#define I2C_RELEASE_BUS_COUNT 1000U
#define PD_I2C_SCL            (16U)
#define PD_I2C_SDA            (17U)
#define PD_I2C_PORT           (1)

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((60000000U) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
/* Select IPG Clock as PERCLK_CLK clock source */
#define USBPD_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define USBPD_GPT_CLOCK_DIVIDER_SELECT (0U)
/* Get source clock for GPT driver (GPT prescaler = 0) */
#define USBPD_GPT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (USBPD_GPT_CLOCK_DIVIDER_SELECT + 1U))

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    HW_I2CReleaseBus();
    BOARD_InitDebugConsole();
    NVIC_SetPriority(BOARD_UART_IRQ, 5);
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
    /*Clock setting for GPT*/
    CLOCK_SetMux(kCLOCK_PerclkMux, USBPD_GPT_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, USBPD_GPT_CLOCK_DIVIDER_SELECT);
    LPI2C1_InitPins();
}

uint32_t HW_TimerGetFreq(void)
{
    return USBPD_GPT_CLK_FREQ;
}

uint32_t HW_I2CGetFreq(uint8_t instance)
{
    return LPI2C_CLOCK_FREQUENCY;
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
    CLOCK_EnableClock(kCLOCK_Iomuxc);

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0);

    /* Init I2C GPIO */

    config.direction = kHAL_GpioDirectionOut;

    config.port = PD_I2C_PORT;

    config.pin = PD_I2C_SCL;

    config.level = 1;

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
    LPI2C1_InitPins();
}

/*${function:end}*/

/*!
** @}
*/
