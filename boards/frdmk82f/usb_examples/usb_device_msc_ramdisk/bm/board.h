/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
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

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "FRDM-K82F"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART4
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_Osc0ErClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetOsc0ErClkFreq()
#define BOARD_UART_IRQ LPUART4_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART4_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C3

/*! @brief The Flextimer instance/channel used for board */
#define BOARD_FTM_BASEADDR FTM3
#define BOARD_FTM_CHANNEL 4U
#define BOARD_FTM_X_CHANNEL 4U
#define BOARD_FTM_Y_CHANNEL 5U
#define BOARD_FTM_PERIOD_HZ 100
#define BOARD_FTM_IRQ_HANDLER FTM3_IRQHandler
#define BOARD_FTM_IRQ_VECTOR FTM3_IRQn

/*! @brief The bubble level demo information */
#define BOARD_FXOS8700_ADDR 0x1C
#define BOARD_ACCEL_ADDR BOARD_FXOS8700_ADDR
#define BOARD_ACCEL_BAUDRATE 100
#define BOARD_ACCEL_I2C_BASEADDR I2C3

/*! @brief The TPM instance/channel used for board */
#define BOARD_TPM_BASEADDR TPM2
#define BOARD_TPM_CHANNEL 0U

/*! @brief The FlexBus instance used for board.*/
#define BOARD_FLEXBUS_BASEADDR FB

#define BOARD_TSI_ELECTRODE_CNT 2U

/*! @brief Indexes of the TSI channels for on board electrodes */
#define BOARD_TSI_ELECTRODE_1 11U
#define BOARD_TSI_ELECTRODE_2 12U

/*! @brief The SDHC instance/channel used for board */
#define BOARD_SDHC_BASEADDR SDHC

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP1
#define BOARD_CMP_CHANNEL 3U

/*! @brief The i2c instance used for sai demo */
#define BOARD_SAI_DEMO_I2C_BASEADDR I2C0

/*! @brief The rtc instance used for rtc_func */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief If connected the TWR_MEM, this is spi sd card */
#define BOARD_SDCARD_CARD_DETECTION_GPIO GPIOD
#define BOARD_SDCARD_CARD_DETECTION_GPIO_PORT PORTD
#define SDCARD_CARD_DETECTION_GPIO_PIN 15U
#define SDCARD_CARD_WRITE_PROTECTION_GPIO GPIOC
#define SDCARD_CARD_WRITE_PROTECTION_GPIO_PORT PORTC
#define SDCARD_CARD_WRITE_PROTECTION_GPIO_PIN 13U
#define SDCARD_SPI_HW_BASEADDR SPI1
#define SDCARD_CARD_INSERTED 0U

/*! @brief Define the port interrupt number for the board switches */
#define BOARD_SW2_GPIO GPIOA
#define BOARD_SW2_PORT PORTA
#define BOARD_SW2_GPIO_PIN 4U
#define BOARD_SW2_IRQ PORTA_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTA_IRQHandler
#define BOARD_SW2_NAME "SW2"

#define BOARD_SW3_GPIO GPIOC
#define BOARD_SW3_PORT PORTC
#define BOARD_SW3_GPIO_PIN 6U
#define BOARD_SW3_IRQ PORTC_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTC_IRQHandler
#define BOARD_SW3_NAME "SW3"

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_RED_GPIO GPIOC
#define BOARD_LED_RED_GPIO_PORT PORTC
#define BOARD_LED_RED_GPIO_PIN 8U
#define BOARD_LED_GREEN_GPIO GPIOC
#define BOARD_LED_GREEN_GPIO_PORT PORTC
#define BOARD_LED_GREEN_GPIO_PIN 9U
#define BOARD_LED_BLUE_GPIO GPIOC
#define BOARD_LED_BLUE_GPIO_PORT PORTC
#define BOARD_LED_BLUE_GPIO_PIN 10U

#define LED_RED_INIT(output)                                 \
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_RED */
#define LED_RED_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

#define LED_GREEN_INIT(output)                                   \
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#define LED_BLUE_INIT(output)                                  \
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

/* FLEXIO */
#define BOARD_FLEXIO_I2C_INSTANCE (3)
#define BOARD_FLEXIO_I2C_INSTANCE_BASE (I2C3)
#define BOARD_FLEXIO_INSTANCE (0)
#define BOARD_FLEXIO_INSTANCE_BASE (FLEXIO0)
#define BOARD_FLEXIO_DATA_PINS (8)
#define BOARD_FLEXIO_DATA_PIN_START_INDEX (4)
#define BOARD_FLEXIO_DATA_PIN_END_INDEX (11)
#define BOARD_FLEXIO_PCLK_PIN_INDEX (0)
#define BOARD_FLEXIO_HREF_PIN_INDEX (3)
#define BOARD_FLEXIO_VSYNC_PORT_INDEX (1)
#define BOARD_FLEXIO_VSYNC_PIN_INDEX (2)
#define BOARD_FLEXIO_CLCK_OUT_PORT_INDEX (2)
#define BOARD_FLEXIO_CLCK_OUT_PIN_INDEX (3)
#define BOARD_FLEXIO_RESET_PORT_INDEX (2)
#define BOARD_FLEXIO_RESET_PIN_INDEX (8)

#define TICKLESS_LPTMR_BASE_PTR LPTMR0
#define TICKLESS_LPTMR_IRQn LPTMR0_LPTMR1_IRQn
#define vPortLptmrIsr LPTMR0_LPTMR1_IRQHandler

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
