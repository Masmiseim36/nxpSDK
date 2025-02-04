/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSDRAMPins(void);

#define BOARD_INITUSDHCPINS_IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK 0x10000000U /*!< GPIO2 and GPIO7 share same IO MUX function, GPIO_MUX2 selects one GPIO function: affected bits mask */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSDHCPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitQSPIPins(void);

#define BOARD_INITPINSM2_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK 0x01080000U /*!< GPIO1 and GPIO6 share same IO MUX function, GPIO_MUX1 selects one GPIO function: affected bits mask */

/* GPIO_AD_B1_03 (coord M12), SPDIF_IN/U15[4]/WL_RST#/J8[56]/SD_PWREN/Q5[1]/J16[8] */
/* Routed pin properties */
#define BOARD_INITPINSM2_WL_RST_PERIPHERAL                                 GPIO1   /*!< Peripheral name */
#define BOARD_INITPINSM2_WL_RST_SIGNAL                                   gpio_io   /*!< Signal name */
#define BOARD_INITPINSM2_WL_RST_CHANNEL                                      19U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINSM2_WL_RST_GPIO                                       GPIO1   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINSM2_WL_RST_GPIO_PIN                                     19U   /*!< GPIO pin number */
#define BOARD_INITPINSM2_WL_RST_GPIO_PIN_MASK                        (1U << 19U)   /*!< GPIO pin mask */
#define BOARD_INITPINSM2_WL_RST_PORT                                       GPIO1   /*!< PORT peripheral base pointer */
#define BOARD_INITPINSM2_WL_RST_PIN                                          19U   /*!< PORT pin number */
#define BOARD_INITPINSM2_WL_RST_PIN_MASK                             (1U << 19U)   /*!< PORT pin mask */

/* GPIO_AD_B1_08 (coord H13), AUD_INT/J34[5]/U25[15]/WIFI_RST_B/U9[3]/CSI_D9/J46[13]/J16[4] */
/* Routed pin properties */
#define BOARD_INITPINSM2_SDIO_RST_PERIPHERAL                               GPIO1   /*!< Peripheral name */
#define BOARD_INITPINSM2_SDIO_RST_SIGNAL                                 gpio_io   /*!< Signal name */
#define BOARD_INITPINSM2_SDIO_RST_CHANNEL                                    24U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINSM2_SDIO_RST_GPIO                                     GPIO1   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINSM2_SDIO_RST_GPIO_PIN                                   24U   /*!< GPIO pin number */
#define BOARD_INITPINSM2_SDIO_RST_GPIO_PIN_MASK                      (1U << 24U)   /*!< GPIO pin mask */
#define BOARD_INITPINSM2_SDIO_RST_PORT                                     GPIO1   /*!< PORT peripheral base pointer */
#define BOARD_INITPINSM2_SDIO_RST_PIN                                        24U   /*!< PORT pin number */
#define BOARD_INITPINSM2_SDIO_RST_PIN_MASK                           (1U << 24U)   /*!< PORT pin mask */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPinsM2(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitM2UARTPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCodecPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
