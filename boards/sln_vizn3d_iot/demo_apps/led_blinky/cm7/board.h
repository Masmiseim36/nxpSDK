/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#ifdef BOARD_DEFINE
#include "board_define.h"
#endif

#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "SLN_VIZN3D_IOT"
#ifndef DEBUG_CONSOLE_UART_INDEX
#define DEBUG_CONSOLE_UART_INDEX 12
#endif

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#define BOARD_DEBUG_UART_CLK_FREQ 24000000

/* Used when setting clock frequency for usb apps like MSC dev in bootloader */
#define BOARD_USB_CLOCK_FREQ (24000000)

#if DEBUG_CONSOLE_UART_INDEX == 1
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_UART_IRQ            LPUART1_IRQn
#define BOARD_UART_IRQ_HANDLER    LPUART1_IRQHandler
#elif DEBUG_CONSOLE_UART_INDEX == 2
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART2
#define BOARD_DEBUG_UART_INSTANCE 2U
#define BOARD_UART_IRQ            LPUART2_IRQn
#define BOARD_UART_IRQ_HANDLER    LPUART2_IRQHandler
#elif DEBUG_CONSOLE_UART_INDEX == 12
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART12
#define BOARD_DEBUG_UART_INSTANCE 12U
#define BOARD_UART_IRQ            LPUART12_IRQn
#define BOARD_UART_IRQ_HANDLER    LPUART12_IRQHandler
#else
#error "Unsupported UART"
#endif

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* USB PHY condfiguration */
#define BOARD_USB_PHY_D_CAL     (0x0CU)
#define BOARD_USB_PHY_TXCAL45DP (0x06U)
#define BOARD_USB_PHY_TXCAL45DM (0x06U)

#ifndef BOARD_USER_BUTTON_GPIO
#define BOARD_USER_BUTTON_GPIO GPIO13
#endif
#ifndef BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_USER_BUTTON_GPIO_PIN (0U)
#endif
#define BOARD_USER_BUTTON_IRQ         GPIO13_Combined_0_31_IRQn
#define BOARD_USER_BUTTON_IRQ_HANDLER GPIO13_Combined_0_31_IRQHandler
#define BOARD_USER_BUTTON_NAME        "SW7"

#define BOARD_USER_LED_GPIO     GPIO2
#define BOARD_USER_LED_GPIO_PIN (10U) /* RED LED */

/* @Brief Board CAMERA configuration */
#define BOARD_MIPI_CAMERA_I2C_BASE          LPI2C6
#define BOARD_MIPI_CAMERA_I2C_CLOCK_ROOT    kCLOCK_Root_Lpi2c6
#define BOARD_MIPI_CAMERA_I2C_CLOCK_SOURCE  (1U)  /* OSC24M. */
#define BOARD_MIPI_CAMERA_I2C_CLOCK_DIVIDER (12U) /* Divider = 12, LPI2C clock frequency 2M. */

#define BOARD_MIPI_CAMERA_PWDN_GPIO  GPIO9
#define BOARD_MIPI_CAMERA_PWDN_PIN   25
#define BOARD_MIPI_CAMERA_RESET_GPIO GPIO11
#define BOARD_MIPI_CAMERA_RESET_PIN  15

/* FlexIO Camera configurations */
#define BOARD_FLEXIO_CAMERA_INST                 FLEXIO2
#define BOARD_FLEXIO_CAMERA_DATA_PIN_START_INDEX 24
#define BOARD_FLEXIO_CAMERA_DATA_PIN_END_INDEX   31

#define BOARD_FLEXIO_CAMERA_XCLK_PIN_INDEX 0
#define BOARD_FLEXIO_CAMERA_PCLK_PIN_INDEX 1
#define BOARD_FLEXIO_CAMERA_HREF_PIN_INDEX 2

#define BOARD_CAMERA_VSYNC_GPIO_BASE  GPIO3
#define BOARD_CAMERA_VSYNC_PIN_INDEX  31u
#define BOARD_CAMERA_VSYNC_IRQn       GPIO3_Combined_16_31_IRQn
#define BOARD_CAMERA_VSYNC_IRQHandler GPIO3_Combined_16_31_IRQHandler

#define BOARD_FLEXIO_CAMERA_I2C_BASE          LPI2C5
#define BOARD_FLEXIO_CAMERA_I2C_CLOCK_ROOT    kCLOCK_Root_Lpi2c5
#define BOARD_FLEXIO_CAMERA_I2C_CLOCK_SOURCE  (1U)  /* OSC24M. */
#define BOARD_FLEXIO_CAMERA_I2C_CLOCK_DIVIDER (12U) /* Divider = 12, LPI2C clock frequency 2M. */

#define BOARD_FLEXIO_CAMERA_PWDN_GPIO  GPIO10
#define BOARD_FLEXIO_CAMERA_PWDN_PIN   13U
#define BOARD_FLEXIO_CAMERA_RESET_GPIO GPIO2
#define BOARD_FLEXIO_CAMERA_RESET_PIN  25U

#define BOARD_CAMERA_PWDN_GPIO GPIO9
#define BOARD_CAMERA_PWDN_PIN  25
#define BOARD_CAMERA_RST_GPIO  GPIO11
#define BOARD_CAMERA_RST_PIN   15

#define BOARD_CSI_CAMERA_PWDN_GPIO  GPIO13
#define BOARD_CSI_CAMERA_PWDN_PIN   9U
#define BOARD_CSI_CAMERA_RESET_GPIO GPIO2
#define BOARD_CSI_CAMERA_RESET_PIN  24U

#define BOARD_CSI_CAMERA_I2C_BASE          LPI2C6
#define BOARD_CSI_CAMERA_I2C_CLOCK_ROOT    kCLOCK_Root_Lpi2c6
#define BOARD_CSI_CAMERA_I2C_CLOCK_SOURCE  (kCLOCK_LPI2C6_ClockRoot_MuxOsc24MOut) /* OSC24M. */
#define BOARD_CSI_CAMERA_I2C_CLOCK_DIVIDER (12U) /* Divider = 12, LPI2C clock frequency 2M. */

#define BOARD_3D_CAMERA_I2C_BASE          LPI2C6
#define BOARD_3D_CAMERA_I2C_CLOCK_ROOT    kCLOCK_Root_Lpi2c6
#define BOARD_3D_CAMERA_I2C_CLOCK_SOURCE  (kCLOCK_LPI2C6_ClockRoot_MuxOsc24MOut) /* OSC24M. */
#define BOARD_3D_CAMERA_I2C_CLOCK_DIVIDER (12U) /* Divider = 12, LPI2C clock frequency 2M. */

#define BOARD_3D_CAMERA_PWR_GPIO          GPIO13
#define BOARD_3D_CAMERA_PWR_PIN           9U
#define BOARD_3D_CAMERA_RESET_GPIO        GPIO13
#define BOARD_3D_CAMERA_RESET_PIN         5U
#define BOARD_3D_CAMERA_TEST_MODE_GPIO    GPIO13
#define BOARD_3D_CAMERA_TEST_MODE_PIN     11U
#define BOARD_3D_CAMERA_LDP_INT_GPIO      GPIO13
#define BOARD_3D_CAMERA_LDP_INT_PIN       12U
#define BOARD_3D_CAMERA_BRIDGE_RESET_GPIO GPIO2
#define BOARD_3D_CAMERA_BRIDGE_RESET_PIN  18U

#define BOARD_BUTTON_SW0_GPIO        GPIO13
#define BOARD_BUTTON_SW0_GPIO_PIN    (0U)
#define BOARD_BUTTON_SW0_IRQ         GPIO13_Combined_0_31_IRQn
#define BOARD_BUTTON_SW0_IRQ_HANDLER GPIO13_Combined_0_31_IRQHandler

#define BOARD_BUTTON_SW1_GPIO GPIO2
#define BOARD_BUTTON_SW1_PIN  (15U)
#define BOARD_BUTTON_SW2_GPIO GPIO2
#define BOARD_BUTTON_SW2_PIN  (16U)
#define BOARD_BUTTON_SW3_GPIO GPIO2
#define BOARD_BUTTON_SW3_PIN  (17U)
#define BOARD_BUTTON_SW1_IRQ  GPIO2_Combined_0_15_IRQn
#define BOARD_BUTTON_SW2_IRQ  GPIO2_Combined_16_31_IRQn
#define BOARD_BUTTON_SW3_IRQ  GPIO2_Combined_16_31_IRQn

/* @Brief Board RGB LED configuration */

#define BOARD_LED_RED_PORT   GPIO2
#define BOARD_LED_RED_PIN    (10U)
#define BOARD_LED_GREEN_PORT GPIO2
#define BOARD_LED_GREEN_PIN  (11U)
#define BOARD_LED_BLUE_PORT  GPIO2
#define BOARD_LED_BLUE_PIN   (12U)

/* @Brief Board LCD configuration */
#define LPSPI_CLOCK_SOURCE_ROOT    kCLOCK_Root_Lpspi1
#define LPSPI_CLOCK_SOURCE_SELECT  (3U) /* Select SYS PLL2 PFD2 (328 MHz) as lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (2U)
#define LPSPI_MASTER_CLK_FREQ      (CLOCK_GetFreqFromObs(CCM_OBS_LPSPI1_CLK_ROOT))

#define BOARD_ELCDIF_DISPLAY_RESET_GPIO     GPIO2
#define BOARD_ELCDIF_DISPLAY_RESET_PIN      22U
#define BOARD_ELCDIF_DISPLAY_POWER_GPIO     GPIO13
#define BOARD_ELCDIF_DISPLAY_POWER_PIN      1
#define BOARD_ELCDIF_DISPLAY_BACKLIGHT_GPIO GPIO9
#define BOARD_ELCDIF_DISPLAY_BACKLIGHT_PIN  5U

#define BOARD_ELCDIF_DISPLAY_SPI_BASE     LPSPI1
#define BOARD_ELCDIF_DISPLAY_SPI_CS_GPIO  GPIO2
#define BOARD_ELCDIF_DISPLAY_SPI_CS_PIN   20U
#define BOARD_ELCDIF_DISPLAY_SPI_SCL_GPIO GPIO9
#define BOARD_ELCDIF_DISPLAY_SPI_SCL_PIN  4U
#define BOARD_ELCDIF_DISPLAY_SPI_SDA_GPIO GPIO2
#define BOARD_ELCDIF_DISPLAY_SPI_SDA_PIN  21U

/* @Brief Board BLE configuration */
#define BOARD_BLE_QN9090_QN_WAKEUP_GPIO  GPIO2
#define BOARD_BLE_QN9090_QN_WAKEUP_PIN   13U
#define BOARD_BLE_QN9090_BLE_WAKEUP_GPIO GPIO13
#define BOARD_BLE_QN9090_BLE_WAKEUP_PIN  0U

/*! @brief The MIPI panel pins. */
#define BOARD_MIPI_PANEL_RST_GPIO   GPIO9
#define BOARD_MIPI_PANEL_RST_PIN    1
#define BOARD_MIPI_PANEL_POWER_GPIO GPIO11
#define BOARD_MIPI_PANEL_POWER_PIN  16

#define BOARD_MIPI_PANEL_BL_GPIO GPIO9
#define BOARD_MIPI_PANEL_BL_PIN  5

/* Touch panel. */
#define BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR      LPI2C5
#define BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_ROOT    kCLOCK_Root_Lpi2c5
#define BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_SOURCE  (1U)  /* OSC24M. */
#define BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_DIVIDER (12U) /* Divider = 12, LPI2C clock frequency 2M. */
#define BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_FREQ    CLOCK_GetRootClockFreq(BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_ROOT)
#define BOARD_MIPI_PANEL_TOUCH_RST_GPIO          GPIO9
#define BOARD_MIPI_PANEL_TOUCH_RST_PIN           0
#define BOARD_MIPI_PANEL_TOUCH_INT_GPIO          GPIO8
#define BOARD_MIPI_PANEL_TOUCH_INT_PIN           31

/* SAI instance and clock */
#define MQS_SAI SAI3
/* Select Audio/Video PLL (393.24 MHz) as MQS clock source */
#define MQS_SAI_CLOCK_SOURCE_SELECT (kCLOCK_MIC_ClockRoot_MuxAudioPllOut)
/* Clock divider for MQS clock source */
#define MQS_SAI_CLOCK_SOURCE_DIVIDER (16U)
/* Get frequency of MQS clock */
#define MQS_SAI_CLK_FREQ       CLOCK_GetRootClockFreq(kCLOCK_Root_Mqs)
#define BOARD_MQS_OE_GPIO_PORT GPIO2
#define BOARD_MQS_OE_GPIO_PIN  29U

#define BOARD_FLEXSPI           FLEXSPI1
#define BOARD_FLEXSPI_CLOCK     kCLOCK_FlexSpi1
#define BOARD_FLEXSPI_AMBA_BASE FlexSPI1_AMBA_BASE
#define BOARD_FLASH_SIZE        0x1000000U

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
uint32_t BOARD_DebugConsoleSrcFreq(void);

void BOARD_InitDebugConsole(void);

void BOARD_ConfigMPU(void);
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
status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subaddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize);
status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subaddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize);
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Codec_I2C_Init(void);
status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Camera_I2C_Init(void);
status_t BOARD_Camera_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

void BOARD_MIPIPanelTouch_I2C_Init(void);
status_t BOARD_MIPIPanelTouch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_MIPIPanelTouch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */

void BOARD_RelocateVectorTableToRam(void);
void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength);
void BOARD_MMC_Pin_Config(uint32_t speed, uint32_t strength);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
