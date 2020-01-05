/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The board name */
#define BOARD_NAME "TWR-K65F180M"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART2
#define BOARD_DEBUG_UART_INSTANCE 2U
#define BOARD_DEBUG_UART_CLKSRC BUS_CLK
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetBusClkFreq()
#define BOARD_UART_IRQ UART2_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER UART2_RX_TX_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The CAN instance used for board */
#define BOARD_CAN_BASEADDR CAN0

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0
#define BOARD_ACCEL_I2C_BASEADDR I2C0

/*! @brief The ENET PHY address. */
#define BOARD_ENET0_PHY_ADDRESS (0x00U) /* Phy address of enet port 0. */

/*! @brief The FlexBus instance used for board.*/
#define BOARD_FLEXBUS_BASEADDR FB

#define BOARD_TSI_ELECTRODE_CNT 2U

/*! @brief Indexes of the TSI channels for on board electrodes */
#ifndef BOARD_TSI_ELECTRODE_1
#define BOARD_TSI_ELECTRODE_1 11U
#endif
#ifndef BOARD_TSI_ELECTRODE_2
#define BOARD_TSI_ELECTRODE_2 12U
#endif

/*! @brief The SDHC instance/channel used for board */
#define BOARD_SDHC_BASEADDR SDHC
#define BOARD_SDHC_CD_GPIO_IRQ_HANDLER PORTA_IRQHandler

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP2
#define BOARD_CMP_CHANNEL 2U

/*! @brief The i2c instance used for sai demo */
#define BOARD_SAI_DEMO_I2C_BASEADDR I2C0

/*! @brief The rtc instance used for rtc_func */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief If connected the TWR_MEM, this is spi sd card */
#ifndef BOARD_SDCARD_CARD_DETECTION_GPIO
#define BOARD_SDCARD_CARD_DETECTION_GPIO GPIOD
#endif
#define BOARD_SDCARD_CARD_DETECTION_GPIO_PORT PORTD
#define SDCARD_CARD_DETECTION_GPIO_PIN 15U
#define SDCARD_CARD_WRITE_PROTECTION_GPIO GPIOC
#define SDCARD_CARD_WRITE_PROTECTION_GPIO_PORT PORTC
#define SDCARD_CARD_WRITE_PROTECTION_GPIO_PIN 13U
#define SDCARD_SPI_HW_BASEADDR SPI1
#define SDCARD_CARD_INSERTED 0U

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_SW3_GPIO
#define BOARD_SW3_GPIO GPIOA
#endif
#ifndef BOARD_SW3_PORT
#define BOARD_SW3_PORT PORTA
#endif
#ifndef BOARD_SW3_GPIO_PIN
#define BOARD_SW3_GPIO_PIN 10
#endif
#define BOARD_SW3_IRQ PORTA_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTA_IRQHandler
#define BOARD_SW3_NAME "SW3"

#define BOARD_DEMO_I2C_CLKSRC kCLOCK_BusClk
#define BOARD_DEMO_SAI_CLKSRC kCLOCK_CoreSysClk
#define BOARD_DEMO_SAI I2S0
#define LLWU_SW_GPIO BOARD_SW3_GPIO
#define LLWU_SW_PORT BOARD_SW3_PORT
#define LLWU_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define LLWU_SW_IRQ BOARD_SW3_IRQ
#define LLWU_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define LLWU_SW_NAME BOARD_SW3_NAME

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#ifndef BOARD_LED_YELLOW_GPIO
#define BOARD_LED_YELLOW_GPIO GPIOB
#endif
#define BOARD_LED_YELLOW_GPIO_PORT PORTB
#ifndef BOARD_LED_YELLOW_GPIO_PIN
#define BOARD_LED_YELLOW_GPIO_PIN 4U
#endif
#ifndef BOARD_LED_ORANGE_GPIO
#define BOARD_LED_ORANGE_GPIO GPIOB
#endif
#define BOARD_LED_ORANGE_GPIO_PORT PORTB
#ifndef BOARD_LED_ORANGE_GPIO_PIN
#define BOARD_LED_ORANGE_GPIO_PIN 5U
#endif
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOA
#endif
#define BOARD_LED_GREEN_GPIO_PORT PORTA
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 28U
#endif
#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO GPIOA
#endif
#define BOARD_LED_BLUE_GPIO_PORT PORTA
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 29U
#endif

#define LED_YELLOW_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, output); \
    BOARD_LED_YELLOW_GPIO->PDDR |= (1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Enable target LED_YELLOW */
#define LED_YELLOW_ON() \
    GPIO_PortClear(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Turn on target LED_YELLOW */
#define LED_YELLOW_OFF() \
    GPIO_PortSet(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Turn off target LED_YELLOW */
#define LED_YELLOW_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Toggle on target LED_YELLOW */

#define LED_ORANGE_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_GPIO_PIN, output); \
    BOARD_LED_ORANGE_GPIO->PDDR |= (1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Enable target LED_ORANGE */
#define LED_ORANGE_ON() \
    GPIO_PortClear(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn on target LED_ORANGE */
#define LED_ORANGE_OFF() \
    GPIO_PortSet(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn off target LED_ORANGE */
#define LED_ORANGE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Toggle on target LED_ORANGE */

#define LED_GREEN_INIT(output)                                             \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#define LED_BLUE_INIT(output)                                            \
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output); \
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Enable target LED_BLUE */
#define LED_BLUE_ON()                                                                                     \
    GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE \ \ \ \
                                                                        */
#define LED_BLUE_OFF()                                                                                   \
    GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE \ \ \ \
                                                                      */
#define LED_BLUE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

/*! @brief Define the port interrupt number for the usb id gpio pin */
#define BOARD_ID_GPIO GPIOE
#define BOARD_ID_PORT PORTE
#define BOARD_ID_GPIO_PIN 10U
#define BOARD_ID_IRQ PORTE_IRQn

/* SDHC base address, clock and card detection pin */
#define BOARD_SDHC_BASEADDR SDHC
#define BOARD_SDHC_CLKSRC kCLOCK_CoreSysClk
#define BOARD_SDHC_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define BOARD_SDHC_IRQ SDHC_IRQn
#define BOARD_SDHC_CD_GPIO_BASE GPIOA
#ifndef BOARD_SDHC_CD_GPIO_PIN
#define BOARD_SDHC_CD_GPIO_PIN 9U
#endif
#define BOARD_MMC_VCC_SUPPLY kMMC_VoltageWindows270to360

#define BOARD_SDHC_CD_PORT_BASE PORTA
#define BOARD_SDHC_CD_PORT_IRQ PORTA_IRQn
#define BOARD_SDHC_CD_PORT_IRQ_HANDLER PORTA_IRQHandler
/* USB PHY condfiguration */
#define BOARD_USB_PHY_D_CAL (0x0CU)
#define BOARD_USB_PHY_TXCAL45DP (0x06U)
#define BOARD_USB_PHY_TXCAL45DM (0x06U)

#define BOARD_CODEC_I2C_BASEADDR I2C0
#define BOARD_CODEC_I2C_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define BOARD_CODEC_I2C_INSTANCE 0U
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_I2C_Init(I2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_I2C_Send(I2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize);
status_t BOARD_I2C_Receive(I2C_Type *base,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize);
void BOARD_Codec_I2C_Init(void);
status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
