/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file mimxrt700evk.c
 * @brief The mimxrt700evk.c file defines GPIO pins and I2C CMSIS utilities for
 * mimxrt700evk board.
 */

#include "mimxrt700evk.h"

gpioHandleiMXSDK_t RED_LED = {
    .base = GPIO7, .pinNumber = 6, .mask = 1 << (6), .irq = GPIO70_IRQn, .portNumber = 7};
gpioHandleiMXSDK_t GREEN_LED = {
    .base = GPIO0, .pinNumber = 18, .mask = 1 << (18), .irq = GPIO00_IRQn, .portNumber = 0};
gpioHandleiMXSDK_t BLUE_LED = {
    .base = GPIO0, .pinNumber = 17, .mask = 1 << (17), .irq = GPIO00_IRQn, .portNumber = 0};

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/*
 * Execute the instrument to enter low power.
 */
static void stop(void)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __asm("WFI");
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by
 * fsl_uart_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPUART0_GetFreq(void)
{
    return CLOCK_GetLPFlexCommClkFreq(0u);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by
 * fsl_i2c_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPI2C8_GetFreq(void)
{
    /*Clock setting for LPI2C*/
    CLOCK_AttachClk(kOSC_CLK_to_FCCLK0);
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM8);
    return CLOCK_GetLPFlexCommClkFreq(8u);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by
 * fsl_spi_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPSPI14_GetFreq(void)
{
    CLOCK_AttachClk(kFRO1_DIV1_to_LPSPI14);
    CLOCK_SetClkDiv(kCLOCK_DivLpspi14Clk, 8U);
    return CLOCK_GetFreq(kCLOCK_LPSpi14Clk);
}
