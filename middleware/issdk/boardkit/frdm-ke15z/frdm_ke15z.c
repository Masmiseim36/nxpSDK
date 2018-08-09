/*
 * The Clear BSD License
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
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
 * @file frdm_ke15z.c
 * @brief The frdm_ke15z.c file defines GPIO pins and I2C CMSIS utilities for FRDM-KE15Z board.
 */

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "frdm_ke15z.h"

// I2C1 Pin Handles
gpioHandleKSDK_t D15 = {.base = GPIOD,
                        .portBase = PORTD,
                        .pinNumber = 9,
                        .mask = 1 << (9),
                        .irq = PORTBCD_IRQn,
                        .clockName = kCLOCK_PortD,
                        .portNumber = PORTD_NUM};
gpioHandleKSDK_t D14 = {.base = GPIOD,
                        .portBase = PORTD,
                        .pinNumber = 8,
                        .mask = 1 << (8),
                        .irq = PORTBCD_IRQn,
                        .clockName = kCLOCK_PortD,
                        .portNumber = PORTD_NUM};

// I2C0 Handle
gpioHandleKSDK_t A5 = {.base = GPIOA,
                       .portBase = PORTA,
                       .pinNumber = 3,
                       .mask = 1 << (3),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortA,
                       .portNumber = PORTA_NUM};
gpioHandleKSDK_t A4 = {.base = GPIOA,
                       .portBase = PORTA,
                       .pinNumber = 2,
                       .mask = 1 << (2),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortA,
                       .portNumber = PORTA_NUM};

// SPI0 Handle
gpioHandleKSDK_t D13 = {.base = GPIOE,
                        .portBase = PORTE,
                        .pinNumber = 0,
                        .mask = 1 << (0),
                        .irq = PORTAE_IRQn,
                        .clockName = kCLOCK_PortE,
                        .portNumber = PORTE_NUM};
gpioHandleKSDK_t D12 = {.base = GPIOE,
                        .portBase = PORTE,
                        .pinNumber = 1,
                        .mask = 1 << (1),
                        .irq = PORTAE_IRQn,
                        .clockName = kCLOCK_PortE,
                        .portNumber = PORTE_NUM};
gpioHandleKSDK_t D11 = {.base = GPIOE,
                        .portBase = PORTE,
                        .pinNumber = 2,
                        .mask = 1 << (2),
                        .irq = PORTAE_IRQn,
                        .clockName = kCLOCK_PortE,
                        .portNumber = PORTE_NUM};


// UART0 Handle
gpioHandleKSDK_t D0 = {.base = GPIOC,
                       .portBase = PORTC,
                       .pinNumber = 8,
                       .mask = 1 << (8),
                       .irq = PORTBCD_IRQn,
                       .clockName = kCLOCK_PortC,
                       .portNumber = PORTC_NUM};
gpioHandleKSDK_t D1 = {.base = GPIOC,
                       .portBase = PORTC,
                       .pinNumber = 9,
                       .mask = 1 << (9),
                       .irq = PORTBCD_IRQn,
                       .clockName = kCLOCK_PortC,
                       .portNumber = PORTC_NUM};

// FRDM-KE15Z Arduino Connector Pin Defintion
gpioHandleKSDK_t A0 = {.base = GPIOA,
                       .portBase = PORTA,
                       .pinNumber = 0,
                       .mask = 1 << (0),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortA,
                       .portNumber = PORTA_NUM};
gpioHandleKSDK_t A1 = {.base = GPIOA,
                       .portBase = PORTA,
                       .pinNumber = 1,
                       .mask = 1 << (1),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortA,
                       .portNumber = PORTA_NUM};
gpioHandleKSDK_t A2 = {.base = GPIOA,
                       .portBase = PORTA,
                       .pinNumber = 6,
                       .mask = 1 << (6),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortA,
                       .portNumber = PORTA_NUM};
gpioHandleKSDK_t A3 = {.base = GPIOA,
                       .portBase = PORTA,
                       .pinNumber = 7,
                       .mask = 1 << (7),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortA,
                       .portNumber = PORTA_NUM};
gpioHandleKSDK_t D2 = {.base = GPIOD,
                       .portBase = PORTD,
                       .pinNumber = 12,
                       .mask = 1 << (12),
                       .irq = PORTBCD_IRQn,
                       .clockName = kCLOCK_PortD,
                       .portNumber = PORTD_NUM};
gpioHandleKSDK_t D3 = {.base = GPIOC,
                       .portBase = PORTC,
                       .pinNumber = 15,
                       .mask = 1 << (15),
                       .irq = PORTBCD_IRQn,
                       .clockName = kCLOCK_PortC,
                       .portNumber = PORTC_NUM};
gpioHandleKSDK_t D4 = {.base = GPIOE,
                       .portBase = PORTE,
                       .pinNumber = 9,
                       .mask = 1 << (9),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortE,
                       .portNumber = PORTE_NUM};
gpioHandleKSDK_t D5 = {.base = GPIOC,
                       .portBase = PORTC,
                       .pinNumber = 5,
                       .mask = 1 << (5),
                       .irq = PORTBCD_IRQn,
                       .clockName = kCLOCK_PortC,
                       .portNumber = PORTC_NUM};
gpioHandleKSDK_t D6 = {.base = GPIOA,
                       .portBase = PORTA,
                       .pinNumber = 16,
                       .mask = 1 << (16),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortA,
                       .portNumber = PORTA_NUM};
gpioHandleKSDK_t D7 = {.base = GPIOA,
                       .portBase = PORTA,
                       .pinNumber = 17,
                       .mask = 1 << (17),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortA,
                       .portNumber = PORTA_NUM};
gpioHandleKSDK_t D8 = {.base = GPIOE,
                       .portBase = PORTE,
                       .pinNumber = 8,
                       .mask = 1 << (8),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortE,
                       .portNumber = PORTE_NUM};
gpioHandleKSDK_t D9 = {.base = GPIOE,
                       .portBase = PORTE,
                       .pinNumber = 7,
                       .mask = 1 << (7),
                       .irq = PORTAE_IRQn,
                       .clockName = kCLOCK_PortE,
                       .portNumber = PORTE_NUM};
gpioHandleKSDK_t D10 = {.base = GPIOA,
                        .portBase = PORTA,
                        .pinNumber = 15,
                        .mask = 1 << (15),
                        .irq = PORTAE_IRQn,
                        .clockName = kCLOCK_PortA,
                        .portNumber = PORTA_NUM};

// FRDM-KE15Z Internal Peripheral Pin Definitions
gpioHandleKSDK_t RED_LED = {.base = GPIOD,
                            .portBase = PORTD,
                            .pinNumber = 0,
                            .mask = 1 << (0),
                            .irq = PORTBCD_IRQn,
                            .clockName = kCLOCK_PortD,
                            .portNumber = PORTD_NUM};
gpioHandleKSDK_t GREEN_LED = {.base = GPIOD,
                              .portBase = PORTD,
                              .pinNumber = 16,
                              .mask = 1 << (16),
                              .irq = PORTBCD_IRQn,
                              .clockName = kCLOCK_PortD,
                              .portNumber = PORTD_NUM};
gpioHandleKSDK_t BLUE_LED = {.base = GPIOD,
                             .portBase = PORTD,
                             .pinNumber = 15,
                             .mask = 1 << (15),
                             .irq = PORTBCD_IRQn,
                             .clockName = kCLOCK_PortD,
                             .portNumber = PORTD_NUM};

// FRDM-KE15Z I2C Inertial Sensor Pin Definitions
gpioHandleKSDK_t INT1 = {.base = GPIOB,
                         .portBase = PORTB,
                         .pinNumber = 10,
                         .mask = 1 << (10),
                         .irq = PORTBCD_IRQn,
                         .clockName = kCLOCK_PortB,
                         .portNumber = PORTB_NUM};
gpioHandleKSDK_t RST_A = {.base = GPIOB,
                         .portBase = PORTB,
                         .pinNumber = 9,
                         .mask = 1 << (9),
                         .irq = PORTBCD_IRQn,
                         .clockName = kCLOCK_PortB,
                         .portNumber = PORTB_NUM};

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_uart_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPUART0_GetFreq(void)
{
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    return CLOCK_GetIpFreq(kCLOCK_Lpuart0);
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
    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_IpSrcSircAsync);
    return CLOCK_GetIpFreq(kCLOCK_Lpuart1);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_uart_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPUART2_GetFreq(void)
{
    CLOCK_SetIpSrc(kCLOCK_Lpuart2, kCLOCK_IpSrcSircAsync);
    return CLOCK_GetIpFreq(kCLOCK_Lpuart2);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_i2c_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPI2C0_GetFreq(void)
{
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);
    return CLOCK_GetIpFreq(kCLOCK_Lpi2c0);
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
    CLOCK_SetIpSrc(kCLOCK_Lpi2c1, kCLOCK_IpSrcFircAsync);
    return CLOCK_GetIpFreq(kCLOCK_Lpi2c1);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_spi_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPSPI0_GetFreq(void)
{
    CLOCK_SetIpSrc(kCLOCK_Lpspi0, kCLOCK_IpSrcFircAsync);
    return CLOCK_GetIpFreq(kCLOCK_Lpspi0);
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
    CLOCK_SetIpSrc(kCLOCK_Lpspi1, kCLOCK_IpSrcFircAsync);
    return CLOCK_GetIpFreq(kCLOCK_Lpspi1);
}

/*! @brief       Reset the onboard FXOS8700.
 *  @details     The API to Reset the onboard FXOS8700 after hardware reset to enable I2C communication.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void BOARD_ACCEL_Reset(void)
{
    uint32_t i;
    gpio_pin_config_t pin_config;

    /* Enable Clock for Reset Port */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* PORTB9 (pin 76) is configured as PTB9 */
    PORT_SetPinMux(PORTB, 9, kPORT_MuxAsGpio);

    /* Reset sensor by reset pin*/
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1;
    GPIO_PinInit(GPIOB, 9, &pin_config);
    GPIO_WritePinOutput(GPIOB, 9, 1);
    /* Delay to ensure reliable sensor reset */
    for (i = 0; i < SystemCoreClock / 1000U; i++)
    {
        __NOP();
    }
    GPIO_WritePinOutput(GPIOB, 9, 0);

    /* Delay to wait sensor stable after reset */
    for (i = 0; i < SystemCoreClock / 1000U; i++)
    {
        __NOP();
    }
}