/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file evkmimxrt685.c
 * @brief The evkmimxrt685.c file defines GPIO pins and I2C CMSIS utilities for evkmimxrt685 board.
 */

#include "evkmimxrt685.h"

// I2C2 Pin Handles
gpioHandleiMXSDK_t D15 = {
    .base = GPIO, .pinNumber = 18, .mask = 1 << (18), .irq = GPIO_INTA_IRQn, .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D14 = {
    .base = GPIO, .pinNumber = 17, .mask = 1 << (17), .irq = GPIO_INTA_IRQn, .portNumber = GPIO1_NUM};

// I2C2 Pin Handles
gpioHandleiMXSDK_t A5 = {
    .base = GPIO, .pinNumber = 18, .mask = 1 << (18), .irq = FLEXCOMM2_IRQn, .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t A4 = {
    .base = GPIO, .pinNumber = 17, .mask = 1 << (17), .irq = FLEXCOMM2_IRQn, .portNumber = GPIO1_NUM};

// SPI5 Pin Handles
gpioHandleiMXSDK_t D13 = {
    .base = GPIO, .pinNumber = 3, .mask = 1 << (3), .irq = GPIO_INTA_IRQn, .portNumber = GPIO2_NUM};
gpioHandleiMXSDK_t D12 = {
    .base = GPIO, .pinNumber = 4, .mask = 1 << (4), .irq = GPIO_INTA_IRQn, .portNumber = GPIO2_NUM};
gpioHandleiMXSDK_t D11 = {
    .base = GPIO, .pinNumber = 5, .mask = 1 << (5), .irq = GPIO_INTA_IRQn, .portNumber = GPIO2_NUM};
gpioHandleiMXSDK_t D10 = {
    .base = GPIO, .pinNumber = 6, .mask = 1 << (6), .irq = GPIO_INTA_IRQn, .portNumber = GPIO2_NUM};

// USART0 Handle
gpioHandleiMXSDK_t D0 = {
    .base = GPIO, .pinNumber = 2, .mask = 1 << (2), .irq = GPIO_INTA_IRQn, .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D1 = {
    .base = GPIO, .pinNumber = 1, .mask = 1 << (1), .irq = GPIO_INTA_IRQn, .portNumber = GPIO1_NUM};

/*!
 * @brief Configures the system to VLPR power mode.
 *        API name used from Kinetis family to maintain compatibility.
 *
 * @param Power peripheral base address (dummy).
 * @return Configuration error code.
 */
status_t SMC_SetPowerModeVlpr(void *arg)
{
   POWER_EnterSleep();

    return kStatus_Success;
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_uart_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t USART0_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(0U);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_i2c_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t I2C2_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(2U);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_spi_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t SPI5_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(5U);
}
