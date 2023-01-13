/*
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#include "bootloader.h"
#include "peripherals_pinmux.h"
#include "pin_mux.h"

#if FSL_FEATURE_SOC_GPIO_COUNT
#include "fsl_gpio.h"
#elif FSL_FEATURE_SOC_IGPIO_COUNT
#include "fsl_gpio.h"
#elif FSL_FEATURE_SOC_RGPIO_COUNT
#include "fsl_rgpio.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (BL_CONFIG_LPUART || BL_CONFIG_SCUART || BL_CONFIG_UART)

#if BL_ENABLE_PINMUX_UART1
#define BL_ENABLED_MAX_UART_INSTANCE (1)
#endif

//! UART autobaud port irq configurations
#define GPIO_IRQC_INTERRUPT_ENABLED_PRIORITY 1
#define GPIO_IRQC_INTERRUPT_RESTORED_PRIORITY 0

#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (BL_CONFIG_LPUART || BL_CONFIG_SCUART || BL_CONFIG_UART)

//! this is to store the function pointer for calling back to the function that wants
//! the UART RX instance pin that triggered the interrupt. This only supports 1 pin
//! for UART0 because UART1 is on PORTC which does not support interrupts :(
static pin_irq_callback_t s_pin_irq_func[BL_ENABLED_MAX_UART_INSTANCE + 1] = { 0 };

#endif // BL_CONFIG_LPUART

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Configure pinmux for uart module.
 *
 * This function only support switching default or gpio or fixed-ALTx mode on fixed pins
 * (Although there are many ALTx-pinmux configuration choices on various pins for the same
 * peripheral module)
 */
void uart_pinmux_config(uint32_t instance, pinmux_type_t pinmux)
{
    switch (instance)
    {
#if BL_ENABLE_PINMUX_UART1
        case 1:
            switch (pinmux)
            {
                case kPinmuxType_Default:
                    UART1_RestoreDefault();
                    break;
                case kPinmuxType_PollForActivity:
                    UART1_PollForActivity();
                    break;
                case kPinmuxType_Peripheral:
                    UART1_InitPins();
                    break;
                default:
                    break;
            }
            break;
#endif // #if BL_ENABLE_PINMUX_UART1
        default:
            break;
    }
}

void spi_pinmux_config(uint32_t instance, pinmux_type_t pinmux)
{
    switch (instance)
    {
#if BL_ENABLE_PINMUX_SPI1
        case 1:
            switch (pinmux)
            {
                case kPinmuxType_Default:
                    SPI1_RestoreDefault();
                    break;
                case kPinmuxType_Peripheral:
                case kPinmuxType_PollForActivity:
                    SPI1_InitPins();
                    break;
                default:
                    break;
            }
            break;
#endif // #if BL_ENABLE_PINMUX_SPI1
        default:
            break;
    }
}

//! @brief this is going to be used for autobaud IRQ handling for UART1
#if BL_ENABLE_PINMUX_UART1 && BL_CONFIG_LPUART
void UART1_RX_GPIO_IRQHandler(void)
{
    uint32_t interrupt_flag = (1U << UART1_RX_GPIO_PIN_NUM);
    // Check if the pin for UART1 is what triggered the RX PORT interrupt
    if ((GPIO_GetPinsInterruptFlags(UART1_RX_GPIO_BASE) & interrupt_flag) && s_pin_irq_func[1])
    {
        s_pin_irq_func[1](1);
        GPIO_ClearPinsInterruptFlags(UART1_RX_GPIO_BASE, interrupt_flag);
    }
}
#endif // #if BL_ENABLE_PINMUX_UART1

void enable_autobaud_pin_irq(uint32_t instance, pin_irq_callback_t func)
{
#if BL_ENABLE_PINMUX_UART1 && BL_CONFIG_LPUART
    s_pin_irq_func[1] = func;
    // Only look for a falling edge for our interrupts
    GPIO_SetPinInterruptConfig(UART1_RX_GPIO_BASE, UART1_RX_GPIO_PIN_NUM, kGPIO_IntFallingEdge);
#if FSL_FEATURE_SOC_IGPIO_COUNT
    UART1_RX_GPIO_BASE->IMR |= (1U << UART1_RX_GPIO_PIN_NUM);
#endif
    NVIC_SetPriority(UART1_RX_GPIO_IRQn, GPIO_IRQC_INTERRUPT_ENABLED_PRIORITY);
    NVIC_EnableIRQ(UART1_RX_GPIO_IRQn);
#endif
}

void disable_autobaud_pin_irq(uint32_t instance)
{
#if BL_ENABLE_PINMUX_UART1 && BL_CONFIG_LPUART
    NVIC_DisableIRQ(UART1_RX_GPIO_IRQn);
    NVIC_SetPriority(UART1_RX_GPIO_IRQn, GPIO_IRQC_INTERRUPT_RESTORED_PRIORITY);
    GPIO_SetPinInterruptConfig(UART1_RX_GPIO_BASE, UART1_RX_GPIO_PIN_NUM, kGPIO_NoIntmode);
    s_pin_irq_func[1] = 0;
#endif // #if BL_ENABLE_PINMUX_UART1
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
