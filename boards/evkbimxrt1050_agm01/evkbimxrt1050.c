/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file evkbimxrt1050.c
 * @brief The evkbimxrt1050.c file defines GPIO pins and I2C CMSIS utilities for evkbimxrt1050 board.
 */

#include "evkbimxrt1050.h"

// I2C20 Pin Handles
gpioHandleiMXSDK_t D15 = {.base = GPIO1,
                        .pinNumber = 0,
                        .mask = 1 << (0),
                        .irq = GPIO1_Combined_0_15_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D14 = {.base = GPIO1,
                        .pinNumber = 1,
                        .mask = 1 << (1),
                        .irq = GPIO1_Combined_0_15_IRQn,
                        .portNumber = GPIO1_NUM};

// I2C1 Pin Handles
gpioHandleiMXSDK_t A5 = {.base = GPIO1,
                        .pinNumber = 16,
                        .mask = 1 << (16),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t A4 = {.base = GPIO1,
                        .pinNumber = 17,
                        .mask = 1 << (17),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};

// LPSPI1 Pin Handles
gpioHandleiMXSDK_t D13 = {.base = GPIO3,
                        .pinNumber = 12,
                        .mask = 1 << (12),
                        .irq = GPIO3_Combined_0_15_IRQn,
                        .portNumber = GPIO3_NUM};
gpioHandleiMXSDK_t D12 = {.base = GPIO3,
                        .pinNumber = 15,
                        .mask = 1 << (15),
                        .irq = GPIO3_Combined_0_15_IRQn,
                        .portNumber = GPIO3_NUM};
gpioHandleiMXSDK_t D11 = {.base = GPIO3,
                        .pinNumber = 14,
                        .mask = 1 << (14),
                        .irq = GPIO3_Combined_0_15_IRQn,
                        .portNumber = GPIO3_NUM};
gpioHandleiMXSDK_t D10 = {.base = GPIO3,
                        .pinNumber = 13,
                        .mask = 1 << (13),
                        .irq = GPIO3_Combined_0_15_IRQn,
                        .portNumber = GPIO3_NUM};

// LPUART1 Handle
gpioHandleiMXSDK_t D0 = {.base = GPIO1,
                        .pinNumber = 23,
                        .mask = 1 << (23),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D1 = {.base = GPIO1,
                        .pinNumber = 22,
                        .mask = 1 << (22),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};

// EVKMIMXRT1050 Arduino Connector Pin Defintion
gpioHandleiMXSDK_t A0 = {.base = GPIO1,
                        .pinNumber = 26,
                        .mask = 1 << (26),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t A1 = {.base = GPIO1,
                        .pinNumber = 27,
                        .mask = 1 << (27),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t A2 = {.base = GPIO1,
                        .pinNumber = 20,
                        .mask = 1 << (20),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t A3 = {.base = GPIO1,
                        .pinNumber = 21,
                        .mask = 1 << (21),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D2 = {.base = GPIO1,
                        .pinNumber = 11,
                        .mask = 1 << (11),
                        .irq = GPIO1_Combined_0_15_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D3 = {.base = GPIO1,
                        .pinNumber = 24,
                        .mask = 1 << (24),
                        .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D4 = {.base = GPIO1,
                         .pinNumber = 9,
                         .mask = 1 << (9),
                         .irq = GPIO1_Combined_0_15_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D5 = {.base = GPIO1,
                         .pinNumber = 10,
                         .mask = 1 << (10),
                         .irq = GPIO1_Combined_0_15_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D6 = {.base = GPIO1,
                         .pinNumber = 18,
                         .mask = 1 << (18),
                         .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D7 = {.base = GPIO1,
                         .pinNumber = 19,
                         .mask = 1 << (19),
                         .irq = GPIO1_Combined_16_31_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D8 = {.base = GPIO1,
                         .pinNumber = 3,
                         .mask = 1 << (3),
                         .irq = GPIO1_Combined_0_15_IRQn,
                        .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t D9 = {.base = GPIO1,
                         .pinNumber = 2,
                         .mask = 1 << (2),
                         .irq = GPIO1_Combined_0_15_IRQn,
                        .portNumber = GPIO1_NUM};

// K3S Internal Peripheral Pin Definitions
// LEDs mapped to USER_LED
gpioHandleiMXSDK_t RED_LED = {.base = GPIO1,
                              .pinNumber = 9,
                              .mask = 1 << (9),
                              .irq = GPIO1_Combined_0_15_IRQn,
                              .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t GREEN_LED = {.base = GPIO1,
                              .pinNumber = 9,
                              .mask = 1 << (9),
                              .irq = GPIO1_Combined_0_15_IRQn,
                              .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t BLUE_LED = {.base = GPIO1,
                              .pinNumber = 9,
                              .mask = 1 << (9),
                              .irq = GPIO1_Combined_0_15_IRQn,
                              .portNumber = GPIO1_NUM};

// i.MXRT1050 EVK Combo Sensor Pin Definitions
gpioHandleiMXSDK_t INT1 = {.base = GPIO1,
                           .pinNumber = 10,
                           .mask = 1 << (10),
                           .irq = GPIO1_Combined_0_15_IRQn,
                           .portNumber = GPIO1_NUM};
gpioHandleiMXSDK_t INT2 = {.base = GPIO1,
                           .pinNumber = 11,
                           .mask = 1 << (11),
                           .irq = GPIO1_Combined_0_15_IRQn,
                           .portNumber = GPIO1_NUM};


/*
 * Execute the instrument to enter low power.
 */
static void stop(void)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __asm("WFI");
}

/*!
 * @brief Configures the system to WAIT power mode.
 *        API name used from Kinetis family to maintain compatibility.
 *
 * @param Power peripheral base address (dummy).
 * @return Configuration error code.
 */
status_t SMC_SetPowerModeWait(void *arg)
{
    CLOCK_SetMode(kCLOCK_ModeWait);
    stop();

    return kStatus_Success;
}

/*!
 * @brief Configures the system to VLPR power mode.
 *        API name used from Kinetis family to maintain compatibility.
 *
 * @param Power peripheral base address (dummy).
 * @return Configuration error code.
 */
status_t SMC_SetPowerModeVlpr(void *arg)
{
    CLOCK_SetMode(kCLOCK_ModeWait);
    stop();

    return kStatus_Success;
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_uart_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPUART1_GetFreq(void)
{
    return CLOCK_GetFreq((clock_name_t) kCLOCK_Lpuart1);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_i2c_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPI2C1_GetFreq(void)
{
    return CLOCK_GetFreq((clock_name_t) kCLOCK_Lpi2c1);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_spi_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetFreq((clock_name_t) kCLOCK_Lpspi1);
}
