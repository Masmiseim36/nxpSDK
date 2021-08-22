/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief FRDM-TOUCH board defines */

/* Push buttons - mutual electrodes */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_1  NT_TSI_TRANSFORM_MUTUAL(FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_1,FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_1)//TF_TSI_MUTUAL_CAP_TX_CHANNEL_3      /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_2  NT_TSI_TRANSFORM_MUTUAL(FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_2,FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_2)//TF_TSI_MUTUAL_CAP_TX_CHANNEL_3      /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_3  NT_TSI_TRANSFORM_MUTUAL(FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_3,FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_3)//TF_TSI_MUTUAL_CAP_TX_CHANNEL_2      /* PTE10 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_4  NT_TSI_TRANSFORM_MUTUAL(FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_4,FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_4)//TF_TSI_MUTUAL_CAP_TX_CHANNEL_2      /* PTE10 */

#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_1 TF_TSI_MUTUAL_CAP_TX_CHANNEL_3 /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_1 TF_TSI_MUTUAL_CAP_RX_CHANNEL_7 /* PTD6  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_2 TF_TSI_MUTUAL_CAP_TX_CHANNEL_3 /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_2 TF_TSI_MUTUAL_CAP_RX_CHANNEL_6 /* PTD5  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_3 TF_TSI_MUTUAL_CAP_TX_CHANNEL_2 /* PTE10 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_3 TF_TSI_MUTUAL_CAP_RX_CHANNEL_7 /* PTD6  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_4 TF_TSI_MUTUAL_CAP_TX_CHANNEL_2 /* PTE10 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_4 TF_TSI_MUTUAL_CAP_RX_CHANNEL_6 /* PTD5 */

/* Slider - self electrodes */
#define FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_1 TF_TSI_SELF_CAP_CHANNEL_10 /* PTD7 */
#define FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_2 TF_TSI_SELF_CAP_CHANNEL_24 /* PTE3 */
#define FRDM_TOUCH_BOARD_TSI_1  TF_TSI_SELF_CAP_CHANNEL_0                  /* PTE5 */
#define FRDM_TOUCH_BOARD_TSI_2  TF_TSI_SELF_CAP_CHANNEL_1                  /* PTE4 */

/* Rotary - self electrodes */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_1 TF_TSI_SELF_CAP_CHANNEL_11 /* PTE8 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_2 TF_TSI_SELF_CAP_CHANNEL_13 /* PTE0 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_3 TF_TSI_SELF_CAP_CHANNEL_14 /* PTE1 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_4 TF_TSI_SELF_CAP_CHANNEL_19 /* PTE2 */

/* IO Headers connected to FRDM shield board */
/* X-DC-GESTURE */
/*
Touchpad electrode positioning:
TSI_CH 6   7  23   22   17  18
       R6  R5  R4  R3  R2  R1
       x   x   x   x   x   x C1 24
       x   x   x   x   x   x C2 10
       x   x   x   x   x   x C3 19
       x   x   x   x   x   x C4 14
       x   x   x   x   x   x C5 13
       x   x   x   x   x   x C5 11
                                TSI_CH
*/

#define FRDM_BOARD_TOUCHPAD_ELECTRODE_R1 TF_TSI_SELF_CAP_CHANNEL_18   /* PTA0 */ 
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_R2 TF_TSI_SELF_CAP_CHANNEL_17   /* PTA1 */ 
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_R3 TF_TSI_SELF_CAP_CHANNEL_22   /* PTC0*/
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_R4 TF_TSI_SELF_CAP_CHANNEL_23   /* PTC1*/
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_R5 TF_TSI_SELF_CAP_CHANNEL_7    /* PTD6*/
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_R6 TF_TSI_SELF_CAP_CHANNEL_6    /* PTD5*/

#define FRDM_BOARD_TOUCHPAD_ELECTRODE_C1 TF_TSI_SELF_CAP_CHANNEL_24   /* PTE3 */
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_C2 TF_TSI_SELF_CAP_CHANNEL_10   /* PTD7 */
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_C3 TF_TSI_SELF_CAP_CHANNEL_19   /* PTE2 */
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_C4 TF_TSI_SELF_CAP_CHANNEL_14   /* PTE1 */
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_C5 TF_TSI_SELF_CAP_CHANNEL_13   /* PTE0 */
#define FRDM_BOARD_TOUCHPAD_ELECTRODE_C6 TF_TSI_SELF_CAP_CHANNEL_11   /* PTE8 */

#define FRDM_BOARD_SHIELD_ELECTRODE     TF_TSI_SELF_CAP_CHANNEL_3    /* PTE11 */
/*! @brief The board name */
#define BOARD_NAME "FRDM-KE16Z"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR LPUART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_IpSrcSysOscAsync
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpuart1)
#define BOARD_UART_IRQ LPUART1_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART1_IRQHandler

    /* @Brief Board accelerator sensor configuration */
#define BOARD_ACCEL_I2C_BASEADDR LPI2C0
#define BOARD_ACCEL_I2C_CLOCK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpi2c0)

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 0U

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief Indexes of the TSI channels for on-board electrodes */
#ifndef BOARD_TSI_ELECTRODE_1
#define BOARD_TSI_ELECTRODE_1 0U
#endif
#ifndef BOARD_TSI_ELECTRODE_2
#define BOARD_TSI_ELECTRODE_2 1U
#endif

/*! @brief Indexes of the TSI mutual channels for FRDM-TOUCH board */
#define BOARD_TSI_MUTUAL_TX_ELECTRODE_1 3U
#define BOARD_TSI_MUTUAL_RX_ELECTRODE_1 7U

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_SW3_GPIO
#define BOARD_SW3_GPIO GPIOD
#endif
#ifndef BOARD_SW3_PORT
#define BOARD_SW3_PORT PORTD
#endif
#ifndef BOARD_SW3_GPIO_PIN
#define BOARD_SW3_GPIO_PIN 2U
#endif
#define BOARD_SW3_IRQ PORTBCD_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTBCD_IRQHandler
#define BOARD_SW3_NAME "SW3"

/* Board RGB LED color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOB
#endif
#define BOARD_LED_RED_GPIO_PORT PORTB
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 5U
#endif
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOB
#endif
#define BOARD_LED_GREEN_GPIO_PORT PORTB
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 4U
#endif
#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO GPIOD
#endif
#define BOARD_LED_BLUE_GPIO_PORT PORTD
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 1U
#endif

#define LED_RED1_INIT(output)                                                \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN) /*!< Enable target LED_RED1 */
#define LED_RED1_ON() \
    GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED1 */
#define LED_RED1_OFF() \
    GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED_RED1 */
#define LED_RED1_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED1 */

#define LED_GREEN1_INIT(output)                                                  \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN1 */
#define LED_GREEN1_ON() \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN1 */
#define LED_GREEN1_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN1 */
#define LED_GREEN1_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN1 */

#define LED_BLUE_INIT(output)                                                  \
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output); \
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() \
    GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize);
status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize);
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
